#[[###########################################################################
        Theolizer Test Project for C# Integration

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

# 暫定開発用(Theolizerプロジェクトへ完全に組込んだら削除する)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../")

#-----------------------------------------------------------------------------
#       target definition
#-----------------------------------------------------------------------------

set(TARGET_NAME cpp_server)
set(CPP_SOURCES cpp_server.cpp cpp_server.h exchange.cpp exchange.h)
set(CPP_LIBRALY
    ../theolizer/integrator/core_integrator.cpp
    ../theolizer/integrator/core_integrator.h
    ../theolizer/integrator/integrator_dll.cpp
    ../theolizer/integrator/integrator_dll.h
    ../theolizer/integrator/memory_stream.cpp
    ../theolizer/integrator/memory_stream.h
    ../theolizer/integrator/base_integrator.h
)

#-----------------------------------------------------------------------------
#       make target
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

# build
add_library(${TARGET_NAME} SHARED ${CPP_SOURCES} ${CPP_LIBRALY})
setup_theolizer(${TARGET_NAME} StaticWithBoost)
