#[[###########################################################################
        Theolizer root project

    © 2016 Theoride Technology (http://theolizer.com/) All Rights Reserved.
    "Theolizer" is a registered trademark of Theoride Technology.

    "Theolizer" License
        In the case where you are in possession of a valid “Theolizer” License,
        you may use this file in accordance with the terms and conditions of 
        the use license determined by Theoride Technology.

    General Public License Version 3 ("GPLv3")
        You may use this file in accordance with the terms and conditions of 
        GPLv3 published by Free Software Foundation.
        Please confirm the contents of GPLv3 at https://www.gnu.org/licenses/gpl.txt .
        A copy of GPLv3 is also saved in a LICENSE.TXT file.

    商用ライセンス
        あなたが有効なTheolizer商用ライセンスを保持している場合、
        セオライド テクノロジーの定める使用許諾書の条件に従って、
        このファイルを取り扱うことができます。

    General Public License Version 3(以下GPLv3)
        Free Software Foundationが公表するGPLv3の使用条件に従って、
        あなたはこのファイルを取り扱うことができます。
        GPLv3の内容を https://www.gnu.org/licenses/gpl.txt にて確認して下さい。
        またGPLv3のコピーをLICENSE.TXTファイルにおいてます。

]]############################################################################

if("${CMAKE_VERSION}" STREQUAL "")
    set(CMAKE_VERSION, 3.5.0)
endif()
cmake_minimum_required(VERSION ${CMAKE_VERSION})

message(STATUS "CI_SERVICE    =${CI_SERVICE}")

#-----------------------------------------------------------------------------
#       Project version
#-----------------------------------------------------------------------------

project(Theolizer VERSION 1.3.0)
set(TEMPORARY_VERSION "-Prerelease.")

message(STATUS "PROJECT_VERSION=${PROJECT_VERSION}${TEMPORARY_VERSION}")

file(WRITE ${CMAKE_BINARY_DIR}/compiler.txt "${CMAKE_CXX_COMPILER}")

#-----------------------------------------------------------------------------
#       Base
#-----------------------------------------------------------------------------

# LIB_TYPE
message(STATUS "LIB_TYPE=${LIB_TYPE}")
if("${LIB_TYPE}" STREQUAL "StaticWithBoost")
    set(STATIC_WITH_BOOST   ON)
    set(BUILD_SHARED_LIBS   OFF)
elseif("${LIB_TYPE}" STREQUAL "Static")
    set(STATIC_WITH_BOOST   OFF)
    set(BUILD_SHARED_LIBS   OFF)
elseif("${LIB_TYPE}" STREQUAL "Shared")
    set(STATIC_WITH_BOOST   OFF)
    set(BUILD_SHARED_LIBS   ON)
elseif("${LIB_TYPE}" STREQUAL "")
    if("${STATIC_WITH_BOOST}" STREQUAL "")
        message(SEND_ERROR "STATIC_WITH_BOOST is \"\"")
    endif()
    if("${BUILD_SHARED_LIBS}" STREQUAL "")
        message(SEND_ERROR "BUILD_SHARED_LIBS is \"\"")
    endif()
else()
    message(SEND_ERROR "unknown library type ${LIB_TYPE}")
endif()

if (BUILD_SHARED_LIBS AND STATIC_WITH_BOOST)
    message(SEND_ERROR "Not supported BUILD_SHARED_LIBS=ON and STATIC_WITH_BOOST=ON.")
endif()

# for MSVC
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
set(CMAKE_SUPPRESS_REGENERATION TRUE)

# Enable Tests
enable_testing()
#include(CTest)

# Enable Folder view for Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

message(STATUS "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")

#-----------------------------------------------------------------------------
#       Get system informations
#-----------------------------------------------------------------------------

include(ProcessorCount)
ProcessorCount(CpuCount)
message(STATUS "CpuCount=${CpuCount}")

#-----------------------------------------------------------------------------
#       Get compiler informations
#-----------------------------------------------------------------------------

message(STATUS "CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")

#-----------------------------------------------------------------------------
#       copy git hook
#-----------------------------------------------------------------------------

if (EXISTS "${CMAKE_SOURCE_DIR}/../.git")
    message(STATUS "Copying applypatch-msg prepare-commit-msg to .git/hooks")
    file(REMOVE
        "${CMAKE_SOURCE_DIR}/../.git/hooks/commit-msg"
        "${CMAKE_SOURCE_DIR}/../.git/hooks/pre-commit"
    )
    file(COPY
        "${CMAKE_SOURCE_DIR}/tools/applypatch-msg"
        "${CMAKE_SOURCE_DIR}/tools/prepare-commit-msg"
        DESTINATION "${CMAKE_SOURCE_DIR}/../.git/hooks"
        FILE_PERMISSIONS
            OWNER_EXECUTE OWNER_WRITE OWNER_READ
            GROUP_READ GROUP_WRITE GROUP_EXECUTE
            WORLD_READ WORLD_WRITE WORLD_EXECUTE
    )
endif()

#-----------------------------------------------------------------------------
#       default compile options
#-----------------------------------------------------------------------------

if (MSVC)
    add_definitions(-D_UNICODE -DUNICODE)
    string(REPLACE "/W3 " "/W4 " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj -D_MSC_VER=${MSVC_VERSION}")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
endif()

#-----------------------------------------------------------------------------
#       Boost
#-----------------------------------------------------------------------------

message(STATUS "BOOST_ROOT=${BOOST_ROOT}")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost 1.54.0 REQUIRED COMPONENTS filesystem system)
message(STATUS "  Boost_INCLUDE_DIR=${Boost_INCLUDE_DIR}")
message(STATUS "  Boost_LIBRARY_DIR=${Boost_LIBRARY_DIR}")
message(STATUS "  Boost_LIBRARIES  =${Boost_LIBRARIES}")

#-----------------------------------------------------------------------------
#       Install settings
#-----------------------------------------------------------------------------

# install destinations
set(BIN_DIR bin)
set(DOC_DIR doc)
set(INCLUDE_DIR include)
set(LIB_DIR library)
set(SAMPLES_DIR samples)

if (NOT MSVC)
    set(LIB_DIR "${LIB_DIR}/${CMAKE_BUILD_TYPE}")
endif()
message(STATUS "LIB_DIR=${LIB_DIR}")

#-----------------------------------------------------------------------------
#        Judge that configuring is necessary
#-----------------------------------------------------------------------------

file(GLOB_RECURSE MAKE_LIST CMakeLists.txt *.in *.cmake)
file(GLOB_RECURSE SAMPLE_LIST ${SAMPLES_DIR}/*)
list(REMOVE_ITEM MAKE_LIST ${SAMPLE_LIST})
list(SORT MAKE_LIST)
message(STATUS "File list for build-system")
foreach(FILE IN LISTS MAKE_LIST)
    message(STATUS "${FILE}")
endforeach()

# コンフィグ時のHash生成
execute_process(COMMAND ${CMAKE_COMMAND} -E md5sum ${MAKE_LIST} OUTPUT_VARIABLE CONFIG_HASH)
#message(STATUS "CONFIG_HASH=${CONFIG_HASH}")
file(WRITE ${CMAKE_BINARY_DIR}/config.hash ${CONFIG_HASH})

#ビルド時のHashチェック
add_custom_target(checkBuildSystem ALL
    COMMAND ${CMAKE_COMMAND} -E md5sum ${MAKE_LIST} >build.hash
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/tools/compare_files.cmake
)

#-----------------------------------------------------------------------------
#       Define library
#-----------------------------------------------------------------------------

# for library
if (${BUILD_SHARED_LIBS})
    set(POST_FIX "Shared")
else()
    if (${STATIC_WITH_BOOST})
        set(POST_FIX "StaticWithBoost")
    else()
        set(POST_FIX "Static")
    endif()
endif()

set(TheolizerLib  TheolizerLib${POST_FIX})
set(TheolizerTest TheolizerTest${POST_FIX})
message(STATUS "TheolizerLib =${TheolizerLib}")
message(STATUS "TheolizerTest=${TheolizerTest}")
set(LIBRARY_LIST ${TheolizerLib} ${TheolizerTest})

#-----------------------------------------------------------------------------
#       Define Functions
#-----------------------------------------------------------------------------

#       ---<<< make version.h at build >>>---

function(version_at_build TARGET_NAME VERSION_DIR SOURCES)
    #message(STATUS "SOURCES=${SOURCES}")
    add_custom_target(VersionOf${TARGET_NAME}
        COMMAND $<TARGET_FILE:remove_crlf> source.hash  -- ${SOURCES}
        COMMAND $<TARGET_FILE:remove_crlf> library.hash -- ${LIBRARY_HEADERS}
        COMMAND ${CMAKE_COMMAND}
            -D VERSION_STRING=\"${PROJECT_VERSION}${TEMPORARY_VERSION}\"
            -D SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\"
            -D VERSION_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}${VERSION_DIR}\"
            -P ${CMAKE_SOURCE_DIR}/tools/configure_file.cmake
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    add_dependencies(${TheolizerLib} VersionOf${TARGET_NAME})
    add_dependencies(makeVersionH VersionOf${TARGET_NAME})

    add_custom_target(HashOf${TARGET_NAME}
        COMMAND ${CMAKE_COMMAND}
            -D CURRENT_SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"
            -P \"${CMAKE_SOURCE_DIR}/tools/sources_hash.cmake\"
             > \"${CMAKE_CURRENT_BINARY_DIR}/sources_hash.txt\"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    add_dependencies(makeHashTxt HashOf${TARGET_NAME})
endfunction()

#       ---<<< EXCLUDE sub >>>---

function(exclude_all_target TARGET_NAME)
    set_target_properties(${TARGET_NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE)
    set_target_properties(${TARGET_NAME} PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD TRUE)
endfunction()

function(exclude_all_test TEST_NAME)
    set_tests_properties(${TEST_NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE)
    set_tests_properties(${TEST_NAME} PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD TRUE)
endfunction()

#       ---<<< test target >>>---

# Full
#add_custom_target(FullTest COMMAND "ctest" "-V" "-C" $<CONFIG> "-L" "TestF.*")
add_custom_target(FullTest)
set_target_properties(FullTest PROPERTIES FOLDER "TestTargets")
exclude_all_target(FullTest)

#add_custom_target(FullTest1 COMMAND "ctest" "-V" "-C" $<CONFIG> "-L" "TestF[1L].*")
add_custom_target(FullTest1)
set_target_properties(FullTest1 PROPERTIES FOLDER "TestTargets")
exclude_all_target(FullTest1)

#add_custom_target(FullTest2 COMMAND "ctest" "-V" "-C" $<CONFIG> "-L" "TestF[2L].*")
add_custom_target(FullTest2)
set_target_properties(FullTest2 PROPERTIES FOLDER "TestTargets")
exclude_all_target(FullTest2)

#add_custom_target(FullTest3 COMMAND "ctest" "-V" "-C" $<CONFIG> "-L" "TestF[3L].*")
add_custom_target(FullTest3)
set_target_properties(FullTest3 PROPERTIES FOLDER "TestTargets")
exclude_all_target(FullTest3)

# Long(Build using Driver Test)
#add_custom_target(LongTest COMMAND "ctest" "-V" "-C" $<CONFIG> "-L" "Test.*L.*")
add_custom_target(LongTest)
exclude_all_target(LongTest)
set_target_properties(LongTest PROPERTIES FOLDER "TestTargets")

# Short(Library Test)
#add_custom_target(ShortTest COMMAND "ctest" "-V" "-C" $<CONFIG> "-L" "Test.*S")
add_custom_target(ShortTest)
set_target_properties(ShortTest PROPERTIES FOLDER "TestTargets")
exclude_all_target(ShortTest)

#       ---<<< for hash (version.h and checkPASS) >>>---

add_custom_target(makeVersionH)
add_custom_target(makeHashTxt)

#       ---<<< test exe >>>---

# 入り口(TARGET_NAMEはプロジェクト名)
function(add_test_sub TEST_NAME TARGET_NAME)
    add_test_sub_base(${TEST_NAME} $<TARGET_FILE:${TARGET_NAME}> ${ARGN})
endfunction(add_test_sub)

# 実装部(TARGET_NAMEはコマンド名)
#       WILL_FAILは"WILL_FAIL"、もしくは、先頭パラメータ
#       先頭パラメータが"WILL_FAIL"になることはない。
function(add_test_sub_base TEST_NAME TARGET_NAME)
    set(COMMAND_LINE "${TARGET_NAME}")

    set(WILL_FAIL "")
    foreach(PARAM IN LISTS ARGN)
        if("${PARAM}" STREQUAL "WILL_FAIL")
            set(WILL_FAIL TRUE)
        else()
            set(PARAM_LIST "${PARAM_LIST};${PARAM}")
        endif()
    endforeach()
    message(STATUS "COMMAND_LINE=${COMMAND_LINE}")

    add_test(NAME ${TEST_NAME} COMMAND ${CMAKE_COMMAND}
        -D "TARGET_NAME=${TARGET_NAME}"
        -D "WILL_FAIL=${WILL_FAIL}"
        -D "PARAM_LIST=${PARAM_LIST}"
        -P ${CMAKE_SOURCE_DIR}/tools/execute_process.cmake
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
    set_tests_properties(${TEST_NAME} PROPERTIES FAIL_REGULAR_EXPRESSION
                         "<<<FAIL>>>;Detected memory leaks!;==[0-9]+==")
    exclude_all_test(${TEST_NAME})
    # make ENVIRONMENT. ";" is cmake-list's separator, it is necessary to escape.
    set(PATH "$ENV{PATH}")

    if(${BUILD_SHARED_LIBS})
        if(WIN32)
            set(PATH "${CMAKE_BINARY_DIR}/library;${CMAKE_BINARY_DIR}/library/$<CONFIG>;${PATH}")
            STRING(REPLACE ";" "\\;" PATH "${PATH}")
        else()
            STRING(REPLACE ";" ":" PATH "${PATH}")
            set(LD_LIBRARY_PATH ";LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/library/${CMAKE_BUILD_TYPE}")
        endif()
    endif()

    set(INCLUDE "$ENV{INCLUDE}")
    STRING(REPLACE ";" "\\;" INCLUDE "${INCLUDE}")

    # set ENVIRONMENT PATH.
    set_tests_properties(${TEST_NAME} PROPERTIES
                         ENVIRONMENT "INCLUDE=${INCLUDE};PATH=${PATH}${LD_LIBRARY_PATH}")
endfunction(add_test_sub_base)

#       ---<<< for test(normal) >>>---

function(add_test_function TARGET_NAME WILL_FAIL)
    if("${WILL_FAIL}" STREQUAL "WILL_FAIL")
        add_executable(${TARGET_NAME} ${ARGN})
    else()
        add_executable(${TARGET_NAME} ${WILL_FAIL} ${ARGN})
        set(WILL_FAIL "")
    endif()
    add_dependencies(${TARGET_NAME} checkBuildSystem)

    if (NOT ${STATIC_WITH_BOOST})
        target_link_libraries(${TARGET_NAME} ${Boost_LIBRARIES})
    endif()

    if (MSVC)
        add_definitions(-D_UNICODE -DUNICODE)
        foreach(LINK_LIB IN LISTS LIBRARY_LIST)
            target_link_libraries(${TARGET_NAME} ${LINK_LIB}.lib)
        endforeach()
    else()
        target_link_libraries(${TARGET_NAME} ${LIBRARY_LIST})

        # https://sourceforge.net/p/mingw-w64/discussion/723797/thread/c6b70624/#7f0a
        # https://github.com/assimp/assimp/issues/177
        if((CMAKE_COMPILER_IS_MINGW) AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
#           add_definitions(-DASSIMP_BUILD_NO_IFC_IMPORTER)
#           message(STATUS "add_definitions(-DASSIMP_BUILD_NO_IFC_IMPORTER)")
#           add_definitions(-DASSIMP_BUILD_ASSIMP_TOOLS=NO -DASSIMP_BUILD_TESTS=OFF)
#           message(STATUS "add_definitions(-DASSIMP_BUILD_ASSIMP_TOOLS=NO -DASSIMP_BUILD_TESTS=OFF)")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
            message(STATUS "add(CMAKE_CXX_FLAGS -Wa,-mbig-obj)")
        endif()
    endif()
    add_dependencies(${TARGET_NAME} ${LIBRARY_LIST})

    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "TestPrograms")

    add_test_sub(${TARGET_NAME} ${TARGET_NAME} ${WILL_FAIL})
    set_tests_properties(${TARGET_NAME}  PROPERTIES LABELS TestFLS)
    add_dependencies(FullTest  ${TARGET_NAME})
    add_dependencies(FullTest1 ${TARGET_NAME})
    add_dependencies(FullTest2 ${TARGET_NAME})
    add_dependencies(FullTest3 ${TARGET_NAME})
    add_dependencies(LongTest  ${TARGET_NAME})
    add_dependencies(ShortTest ${TARGET_NAME})
endfunction(add_test_function)

#-----------------------------------------------------------------------------
#       Sub-Projects
#-----------------------------------------------------------------------------

# インストール前のテストに用いるTHEOLIZER_DIR
set(THEOLIZER_DIR "${CMAKE_BINARY_DIR}" CACHE PATH "for pre install test" FORCE)

#       ---<<< sub-tool >>>---

add_subdirectory(tools)

#       ---<<< Theolizer is shared library >>>---

if(${BUILD_SHARED_LIBS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTHEOLIZER_DYN_LINK")
endif()

#       ---<<< Theolizer Library >>>---

# ライブラリのハッシュ計算用ソース
set(VERSION_SOURCE "")

include_directories("${CMAKE_SOURCE_DIR}/library")
add_subdirectory(library)
link_directories("${CMAKE_BINARY_DIR}/library")

# テスト
add_subdirectory(test_library)

#       ---<<< Theolizer Driver >>>---

if(BUILD_DRIVER)
    message(STATUS "add_subdirectory(driver)")
    add_subdirectory(driver)
endif()

# テストに必要なツールのコピー
if (MSVC)
    file(COPY ${CMAKE_SOURCE_DIR}/tools/theolizer.props.in    DESTINATION ${CMAKE_BINARY_DIR})
endif()
file(COPY ${CMAKE_SOURCE_DIR}/tools/THEOLIZERConfig.cmake DESTINATION ${CMAKE_BINARY_DIR})

# テスト
add_subdirectory(reference_and_test)

#       ---<<< C# Integration >>>---

# MinGW/gccで成功しない。全体の姿が見えてから対処する
# 更にmsvcのSharedとStaticでエラー(cs_testがStaticWithBoostのみ指定しているため)
# 上記同様全体の姿が見えてから対処する
if (MSVC AND STATIC_WITH_BOOST)
    add_subdirectory(cs_integration_test)
endif()

#       ---<<< Theolizer Document >>>---

if(BUILD_DOCUMENT)
    add_subdirectory(document)
endif()

#-----------------------------------------------------------------------------
#       Sample-Project
#-----------------------------------------------------------------------------

#       ---<<< CMake Helper >>>---

if (MSVC)
    install(FILES tools/theolizer.props.in DESTINATION .)
endif()

install(FILES tools/THEOLIZERConfig.cmake DESTINATION .)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${SAMPLES_DIR} DESTINATION .)
