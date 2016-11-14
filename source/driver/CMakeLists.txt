#[[###########################################################################

        Theolizer Driver Project

    Copyright (c) 2016 Yohinori Tahara(Theoride Technology) - http://theolizer.com/

    商用ライセンス
        有効なTheolizer商用ライセンスを保持している人は、
        田原良則(Theoride Technology)と締結している商用ライセンス契約の
        使用条件に従って、このファイルを取り扱うことができます。

    General Public License Version 3(以下GPLv3)
        Free Software Foundationが公表するGPLv3の使用条件に従って、
        あなたはこのファイルを取り扱うことができます。
        GPLv3の内容を https://www.gnu.org/licenses/gpl.txt にて確認して下さい。
        GPLv3のコピーをLICENSE.TXTファイルにおいてます。

]]############################################################################

#-----------------------------------------------------------
#       setup LLVM(libTooling)
#-----------------------------------------------------------

message(STATUS "LLVM_ROOT : ${LLVM_ROOT}")

find_package(LLVM REQUIRED CONFIG
             PATHS "${LLVM_ROOT}/share/llvm/cmake" "${LLVM_ROOT}/lib/cmake/llvm")
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

#       ---<<< make source list >>>---

file(GLOB_RECURSE THEOLIZER_DRIVER_LIST *.cpp *.c *.h *.inc *.hpp)
file(GLOB_RECURSE VERSION_H version.h)
list(REMOVE_ITEM THEOLIZER_DRIVER_LIST ${VERSION_H} ${THEOLIZER_TEST_LIST})
file(GLOB_RECURSE HELPER_LIST helper/*)
list(REMOVE_ITEM THEOLIZER_DRIVER_LIST ${HELPER_LIST})

list(SORT THEOLIZER_DRIVER_LIST)

message(STATUS "Source of TheolizerDriver")
foreach(SOURCE IN LISTS THEOLIZER_DRIVER_LIST)
    message(STATUS "${SOURCE}")
endforeach()

#       ---<<< make version.h at config >>>---

# ドライバのソースとライブラリのソース
set(VERSION_SOURCE "${THEOLIZER_DRIVER_LIST};${VERSION_SOURCE}")
# 関連するCMakeLists.txtも含める
set(VERSION_SOURCE "${VERSION_SOURCE};${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")

#-----------------------------------------------------------
#       make target
#-----------------------------------------------------------

#       ---<<< Common >>>---

# Header
include_directories("${Boost_INCLUDE_DIR}")
include_directories("${LLVM_INCLUDE_DIRS}")
include_directories("${THEOLIZER_ROOT}/include")

# Library
link_directories("${Boost_LIBRARY_DIR}")
link_directories("${LLVM_LIBRARY_DIRS}")

#       ---<<< TheolizerDriver >>>---

add_executable(TheolizerDriver ${THEOLIZER_DRIVER_LIST})
add_dependencies(TheolizerDriver checkBuildSystem)

target_link_libraries(TheolizerDriver ${TheolizerLib})
target_link_libraries(TheolizerDriver ${Boost_LIBRARIES})

if (NOT IS_MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

# version.h生成
version_at_build(TheolizerDriver "${CMAKE_CURRENT_SOURCE_DIR}" "${VERSION_SOURCE}")

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    foreach(link_lib IN LISTS LIBRARY_LIST)
        target_link_libraries(TheolizerDriver optimized ${link_lib}.lib)
        target_link_libraries(TheolizerDriver debug     ${link_lib}d.lib)
    endforeach()
else()
    target_link_libraries(TheolizerDriver ${LIBRARY_LIST})
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
endif()
if(NOT "${WINLIB_LIST}" STREQUAL "")
    target_link_libraries(TheolizerDriver ${WINLIB_LIST}.lib)
endif()

llvm_map_components_to_libnames(llvm_libs ${COMPONENT_LIST})
target_link_libraries(TheolizerDriver ${llvm_libs})

message(STATUS "LLVM_LIBRARY_DIRS: ${LLVM_LIBRARY_DIRS}")
message(STATUS "llvm_libs        : ${llvm_libs}")

message(STATUS "User selected components = ${COMPONENT_LIST}")
message(STATUS "    = ${llvm_libs}")
message(STATUS "User selected librarys = ${LIBRARY_LIST}")

#-----------------------------------------------------------
#       install target
#-----------------------------------------------------------

install(TARGETS TheolizerDriver CONFIGURATIONS Release DESTINATION ${BIN_DIR})

if (IS_MSVC)
    install(DIRECTORY helper/msvc/ DESTINATION .)
endif()

#-----------------------------------------------------------
#       test target
#               install/uninstall function test
#-----------------------------------------------------------

#       ---<<< define test parts >>>---

# install
add_test_sub_sudo(TheolizerDriverInstall
                  TheolizerDriver --theolizer-replace=${CMAKE_CXX_COMPILER})
set_tests_properties(TheolizerDriverInstall PROPERTIES PASS_REGULAR_EXPRESSION "Completed !")
set_tests_properties(TheolizerDriverInstall PROPERTIES LABELS TestXL)

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
