#[[###########################################################################
        Theolizer紹介用サンプルCMakeLists.txt
]]############################################################################

if("${CMAKE_VERSION}" STREQUAL "")
    set(CMAKE_VERSION, 3.5.0)
endif()
cmake_minimum_required(VERSION ${CMAKE_VERSION})

message(STATUS "BOOST_ROOT=${BOOST_ROOT}")

#-----------------------------------------------------------------------------
#       プロジェクト設定
#-----------------------------------------------------------------------------

set(TARGET_NAME example)
set(SOURCE_LIST example.cpp)

project(${TARGET_NAME}  VERSION 1.0.0)

#-----------------------------------------------------------------------------
#       ビルド設定
#-----------------------------------------------------------------------------

# MSVCの通常使わないビルド・モードとZERO_CHECKプロジェクトの削除
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
set(CMAKE_SUPPRESS_REGENERATION TRUE)

# Theolizer
find_package(THEOLIZER)

# Options
if (MSVC)
    add_definitions(-D_UNICODE -DUNICODE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /bigobj")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

    # MinGWの不具合(https://sourceforge.net/p/mingw-w64/discussion/723797/thread/c6b70624/#7f0a)暫定対処
    if((CMAKE_COMPILER_IS_MINGW) AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
    endif()
endif()

# example
add_executable(${TARGET_NAME} ${SOURCE_LIST})
setup_theolizer(${TARGET_NAME} StaticWithBoost)

#-----------------------------------------------------------------------------
#       テスト登録
#-----------------------------------------------------------------------------

enable_testing()
add_test(NAME example COMMAND $<TARGET_FILE:example>)
