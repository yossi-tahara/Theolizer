#[[###########################################################################
        Theolizer紹介用サンプルCMakeLists.txt
]]############################################################################

cmake_minimum_required(VERSION 3.2.3)

message(STATUS "BOOST_ROOT=${BOOST_ROOT}")

#-----------------------------------------------------------------------------
#       MSVCの通常使わないビルド・モードとZERO_CHECKプロジェクトの削除
#-----------------------------------------------------------------------------

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
set(CMAKE_SUPPRESS_REGENERATION TRUE)

#-----------------------------------------------------------------------------
#       プロジェクト設定
#-----------------------------------------------------------------------------

#プロジェクト定義
set(TARGET_NAME example)
project(${TARGET_NAME}  VERSION 1.0.0)
set(SOURCE_LIST example.cpp example.h)

# Theolizer
find_package(THEOLIZER)

#-----------------------------------------------------------------------------
#       ビルド設定
#-----------------------------------------------------------------------------

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    add_definitions(-D_UNICODE -DUNICODE)
    set(CMAKE_DEBUG_POSTFIX "d")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    # MinGWの不具合(https://sourceforge.net/p/mingw-w64/discussion/723797/thread/c6b70624/#7f0a)暫定対処
    if((CMAKE_COMPILER_IS_MINGW) AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
    endif()
endif()

# example
add_executable(example ${SOURCE_LIST})
setup_theolizer(example StaticWithBoost)

#-----------------------------------------------------------------------------
#       テスト実行
#-----------------------------------------------------------------------------

enable_testing()
add_test(NAME example COMMAND $<TARGET_FILE:example>)

add_custom_target(BuildTest COMMAND "ctest" "-V" "-C" $<CONFIG>)
add_dependencies(BuildTest example)
