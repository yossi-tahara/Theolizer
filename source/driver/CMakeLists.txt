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

set(LIBRARY_LIST ${LIBRARY_LIST} clangFrontend clangSerialization clangDriver)
set(LIBRARY_LIST ${LIBRARY_LIST} clangTooling clangParse clangSema)
set(LIBRARY_LIST ${LIBRARY_LIST} clangAnalysis clangEdit clangAST)
set(LIBRARY_LIST ${LIBRARY_LIST} clangLex clangBasic clangRewrite)
set(LIBRARY_LIST ${LIBRARY_LIST} LLVMProfileData LLVMObject LLVMMCParser LLVMBitReader)
set(LIBRARY_LIST ${LIBRARY_LIST} LLVMCore LLVMMC LLVMOption LLVMSupport)

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

add_executable(TheolizerDriver ${THEOLIZER_DRIVER_LIST})
add_dependencies(TheolizerDriver checkBuildSystem)
add_dependencies(LongTest TheolizerDriver)

target_link_libraries(TheolizerDriver ${Boost_LIBRARIES})

if (NOT IS_MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-comment")
endif()

# version.h生成
version_at_build(TheolizerDriver "" "${VERSION_SOURCE}")

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    foreach(link_lib IN LISTS LIBRARY_LIST)
        target_link_libraries(TheolizerDriver optimized ${link_lib}.lib)
        target_link_libraries(TheolizerDriver debug     ${link_lib}d.lib)
    endforeach()
else()
    target_link_libraries(TheolizerDriver ${LIBRARY_LIST})
    if (CMAKE_COMPILER_IS_MINGW)
        set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
    endif()
endif()
if(NOT "${WINLIB_LIST}" STREQUAL "")
    target_link_libraries(TheolizerDriver ${WINLIB_LIST}.lib)
endif()

llvm_map_components_to_libnames(LLVM_LIBS ${COMPONENT_LIST})
target_link_libraries(TheolizerDriver ${LLVM_LIBS})

message(STATUS "Boost_LIBRARIES  = ${Boost_LIBRARIES}")
message(STATUS "LIBRARY_LIST     = ${LIBRARY_LIST}")
message(STATUS "WINLIB_LIST      = ${WINLIB_LIST}")

message(STATUS "COMPONENT_LIST   = ${COMPONENT_LIST}")
message(STATUS "LLVM_LIBRARY_DIRS= ${LLVM_LIBRARY_DIRS}")
message(STATUS "LLVM_LIBS        = ${LLVM_LIBS}")

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

install(TARGETS TheolizerDriver CONFIGURATIONS Release DESTINATION ${BIN_DIR})

if (IS_MSVC)
    install(FILES $<TARGET_FILE:TheolizerDriver>
            CONFIGURATIONS Release
            DESTINATION msbuild-bin
            RENAME cl.exe)
endif()

#-----------------------------------------------------------
#       test target
#               install/uninstall function test
#-----------------------------------------------------------

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
