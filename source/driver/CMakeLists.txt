#[[###########################################################################

        Theolizer Driver Project

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

#-----------------------------------------------------------
#       setup LLVM(libTooling)
#-----------------------------------------------------------

message(STATUS "LLVM_ROOT : ${LLVM_ROOT}")

if("${LLVM_ROOT}" STREQUAL "")
    find_package(LLVM ${LLVM_VERSION} REQUIRED CONFIG)
else()
    find_package(LLVM ${LLVM_VERSION} REQUIRED CONFIG
        PATHS "${LLVM_ROOT}/share/llvm/cmake" "${LLVM_ROOT}/lib/cmake/llvm")
endif()
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

if(LLVM_BUILD_MAIN_SRC_DIR)
  include_directories(${LLVM_BUILD_MAIN_SRC_DIR}/tools/clang/include)
  include_directories(${LLVM_BUILD_BINARY_DIR}/tools/clang/include)
endif()
add_definitions(${LLVM_DEFINITIONS})

set(LIB_TOOLING_LIST                 clangFrontend clangSerialization clangDriver)
set(LIB_TOOLING_LIST ${LIB_TOOLING_LIST} clangTooling clangParse clangSema)
set(LIB_TOOLING_LIST ${LIB_TOOLING_LIST} clangAnalysis clangEdit clangAST)
set(LIB_TOOLING_LIST ${LIB_TOOLING_LIST} clangLex clangBasic clangRewrite)
set(LIB_TOOLING_LIST ${LIB_TOOLING_LIST} LLVMProfileData LLVMObject LLVMMCParser LLVMBitReader)
set(LIB_TOOLING_LIST ${LIB_TOOLING_LIST} LLVMCore LLVMMC LLVMOption LLVMSupport)

if(WIN32)
    set(WINLIB_LIST Version)
endif()

#-----------------------------------------------------------------------------
#       target definition
#-----------------------------------------------------------------------------

set(CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
include(sources_list.cmake)

message(STATUS "Sources of TheolizerDriver")
foreach(SOURCE IN LISTS THEOLIZER_DRIVER_LIST)
    message(STATUS "${SOURCE}")
endforeach()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin/Debug")

#-----------------------------------------------------------
#       make target
#-----------------------------------------------------------

#       ---<<< Common >>>---

# Header
include_directories("${Boost_INCLUDE_DIR}")
include_directories("${LLVM_INCLUDE_DIRS}")

# Library
link_directories("${Boost_LIBRARY_DIR}")
link_directories("${LLVM_LIBRARY_DIRS}")

#       ---<<< TheolizerDriver >>>---

add_executable(TheolizerDriver EXCLUDE_FROM_ALL ${THEOLIZER_DRIVER_LIST})
#set_target_properties(TheolizerDriver PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_RELEASE FALSE)
add_dependencies(TheolizerDriver checkBuildSystem)
add_dependencies(FullTest  TheolizerDriver)
add_dependencies(FullTest1 TheolizerDriver)
add_dependencies(FullTest2 TheolizerDriver)
add_dependencies(FullTest3 TheolizerDriver)
add_dependencies(LongTest  TheolizerDriver)

# MSVCの場合はReleaseビルド版をリンクする(LLVMライブラリのReleaseビルドを使うため統一が必要)
if (NOT MSVC)
    target_link_libraries(TheolizerDriver ${Boost_LIBRARIES})
    target_link_libraries(TheolizerDriver ${LIBRARY_LIST} ${LIB_TOOLING_LIST})
else()
    target_compile_definitions(TheolizerDriver PRIVATE _ITERATOR_DEBUG_LEVEL=0)
    string(REPLACE "/MDd" "/MD" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    message(STATUS "CMAKE_CXX_FLAGS_DEBUG = ${CMAKE_CXX_FLAGS_DEBUG}")
    message(STATUS "LINK_FLAGS_DEBUG      = ${LINK_FLAGS_DEBUG}")

    set(Boost_LIBRARIES_MSVC "")
    set(IS_RELEASE "")
    foreach(LIB IN LISTS Boost_LIBRARIES)
        if("${LIB}" STREQUAL "optimized")
            set(IS_RELEASE "TRUE")
        elseif("${LIB}" STREQUAL "debug")
            set(IS_RELEASE "FALSE")
        elseif("${IS_RELEASE}" STREQUAL "TRUE")
            set(Boost_LIBRARIES_MSVC ${Boost_LIBRARIES_MSVC} ${LIB})
        elseif("${IS_RELEASE}" STREQUAL "FALSE")
        else()
            message(SEND_ERROR "IS_RELEASE=${IS_RELEASE} : ${Boost_LIBRARIES}")
        endif()
    endforeach()
    target_link_libraries(TheolizerDriver ${Boost_LIBRARIES_MSVC})

    get_target_property(THEOLIZER_LIB_NAME ${TheolizerLib} OUTPUT_NAME)
    if("${THEOLIZER_LIB_NAME}" MATCHES "-NOTFOUND")
        set(THEOLIZER_LIB_NAME ${TheolizerLib})
    endif()
    message(STATUS "THEOLIZER_LIB_NAME  = ${THEOLIZER_LIB_NAME}")

    get_target_property(THEOLIZER_TEST_NAME ${TheolizerTest} OUTPUT_NAME)
    if("${THEOLIZER_TEST_NAME}" MATCHES "-NOTFOUND")
        set(THEOLIZER_TEST_NAME ${TheolizerTest})
    endif()
    message(STATUS "THEOLIZER_TEST_NAME = ${THEOLIZER_TEST_NAME}")

    target_link_libraries(TheolizerDriver ${CMAKE_BINARY_DIR}/library/Release/${THEOLIZER_LIB_NAME}.lib)
    target_link_libraries(TheolizerDriver ${CMAKE_BINARY_DIR}/library/Release/${THEOLIZER_TEST_NAME}.lib)
    add_dependencies(TheolizerDriver ${TheolizerLib} ${TheolizerTest})
endif()

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-comment")
else()
    add_custom_command(TARGET TheolizerDriver
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:TheolizerDriver> $<TARGET_FILE_DIR:TheolizerDriver>/cl.exe)
endif()

# version.h生成
version_at_build(TheolizerDriver "" "${VERSION_SOURCE}")

if (NOT MSVC)
    if (CMAKE_COMPILER_IS_MINGW)
        set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
    endif()
else()
    foreach(link_lib IN LISTS LIB_TOOLING_LIST)
        target_link_libraries(TheolizerDriver ${link_lib}.lib)
    endforeach()
endif()
if(NOT "${WINLIB_LIST}" STREQUAL "")
    target_link_libraries(TheolizerDriver ${WINLIB_LIST}.lib)
endif()

llvm_map_components_to_libnames(LLVM_LIBS ${COMPONENT_LIST})
target_link_libraries(TheolizerDriver ${LLVM_LIBS})

message(STATUS "Boost_LIBRARIES      = ${Boost_LIBRARIES}")
message(STATUS "Boost_LIBRARIES_MSVC = ${Boost_LIBRARIES_MSVC}")
message(STATUS "LIBRARY_LIST         = ${LIBRARY_LIST}")
message(STATUS "LIB_TOOLING_LIST     = ${LIB_TOOLING_LIST}")
message(STATUS "WINLIB_LIST          = ${WINLIB_LIST}")

message(STATUS "COMPONENT_LIST       = ${COMPONENT_LIST}")
message(STATUS "LLVM_LIBRARY_DIRS    = ${LLVM_LIBRARY_DIRS}")
message(STATUS "LLVM_LIBS            = ${LLVM_LIBS}")

message(STATUS "LINK_DIRECTORIES     = ${LINK_DIRECTORIES}")

# 以下デバッグ用
get_property(LIBRARIES TARGET TheolizerDriver PROPERTY LINK_LIBRARIES)
message(STATUS "LIBRARIES = ${LIBRARIES}")
message(STATUS "CMAKE_CXX_IMPLICIT_LINK_LIBRARIES = ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES}")
message(STATUS "CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_C_FLAGS_RELEASE = ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE = ${CMAKE_CXX_FLAGS_RELEASE}")

#-----------------------------------------------------------
#       install target
#-----------------------------------------------------------

install(PROGRAMS $<TARGET_FILE:TheolizerDriver> CONFIGURATIONS Release DESTINATION ${BIN_DIR})

if (MSVC)
    install(FILES $<TARGET_FILE:TheolizerDriver>
            CONFIGURATIONS Release
            DESTINATION bin
            RENAME cl.exe)
endif()

#-----------------------------------------------------------
#       test target
#               parameter error test
#               install/uninstall function test
#-----------------------------------------------------------

#       ---<<< parameter error test >>>---

add_test_sub_base(DriverError $<TARGET_FILE:TheolizerDriver> "WILL_FAIL")
set_tests_properties(DriverError PROPERTIES LABELS TestF1L)
#set_tests_properties(DriverError PROPERTIES WILL_FAIL true)
set_tests_properties(DriverError PROPERTIES PASS_REGULAR_EXPRESSION
                     "No theolizer_original_compiler option")

#       ---<<< define test parts >>>---

if(FALSE)
# sudoが必要な処理の入り口(TARGET_NAMEはプロジェクト名)
function(add_test_sub_sudo TEST_NAME TARGET_NAME)
    set(SUDO "")
    if((NOT WIN32) AND ("${CI_SERVER}" STREQUAL ""))
        set(SUDO "sudo")
    endif()
    add_test_sub_base(${TEST_NAME} ${SUDO} $<TARGET_FILE:${TARGET_NAME}> ${ARGN})
endfunction(add_test_sub_sudo)

# prepare
add_test_sub_sudo(TheolizerDriverPrepare
                  TheolizerDriver --theolizer-restore=${CMAKE_CXX_COMPILER})
set_tests_properties(TheolizerDriverPrepare PROPERTIES LABELS TestXL)

# install
add_test_sub_sudo(TheolizerDriverInstall
                  TheolizerDriver --theolizer-replace=${CMAKE_CXX_COMPILER})
set_tests_properties(TheolizerDriverInstall PROPERTIES PASS_REGULAR_EXPRESSION "Completed !")
set_tests_properties(TheolizerDriverInstall PROPERTIES LABELS TestXL)
set_tests_properties(TheolizerDriverInstall PROPERTIES DEPENDS TheolizerDriverPrepare)

# reinstall
add_test_sub_sudo(TheolizerDriverReInstall
                  TheolizerDriver --theolizer-replace=${CMAKE_CXX_COMPILER})
set_tests_properties(TheolizerDriverReInstall PROPERTIES PASS_REGULAR_EXPRESSION "Already replaced")
set_tests_properties(TheolizerDriverReInstall PROPERTIES LABELS TestXL)
set_tests_properties(TheolizerDriverReInstall PROPERTIES DEPENDS TheolizerDriverInstall)

# uninstall
add_test_sub_sudo(TheolizerDriverUninstall
                  TheolizerDriver --theolizer-restore=${CMAKE_CXX_COMPILER})
set_tests_properties(TheolizerDriverUninstall PROPERTIES PASS_REGULAR_EXPRESSION "Completed !")
set_tests_properties(TheolizerDriverUninstall PROPERTIES LABELS TestXL)
set_tests_properties(TheolizerDriverUninstall PROPERTIES DEPENDS TheolizerDriverReInstall)

# reuninstall
add_test_sub_sudo(TheolizerDriverReUninstall
                  TheolizerDriver --theolizer-restore=${CMAKE_CXX_COMPILER})
set_tests_properties(TheolizerDriverReUninstall PROPERTIES PASS_REGULAR_EXPRESSION "Already restored")
set_tests_properties(TheolizerDriverReUninstall PROPERTIES LABELS TestXL)
set_tests_properties(TheolizerDriverReUninstall PROPERTIES DEPENDS TheolizerDriverUninstall)
endif()
