#[[###########################################################################
        各serializerのテスト

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

#-----------------------------------------------------------------------------
#       Base
#-----------------------------------------------------------------------------

if("${CMAKE_VERSION}" STREQUAL "")
    set(CMAKE_VERSION, 3.5.0)
endif()
cmake_minimum_required(VERSION ${CMAKE_VERSION})

# for MSVC
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
set(CMAKE_SUPPRESS_REGENERATION TRUE)

#-----------------------------------------------------------------------------
#       test set 01
#-----------------------------------------------------------------------------

set(TARGET_NAME SerializerBase01)

set(SOURCE_LIST test_serializer_base01.cpp)
set(SOURCE_LIST ${SOURCE_LIST} test_serializer_base01a_normal.cpp)
set(SOURCE_LIST ${SOURCE_LIST} test_serializer_base01a_normal2.cpp)
set(SOURCE_LIST ${SOURCE_LIST} test_serializer_base01a_tracking.cpp)
set(SOURCE_LIST ${SOURCE_LIST} test_serializer_base01b.cpp)
set(SOURCE_LIST ${SOURCE_LIST} test_serializer_base_version.cpp)
set(SOURCE_LIST ${SOURCE_LIST} test_serializer_base.h test_serializer_base_version.h)
add_test_function(${TARGET_NAME} ${SOURCE_LIST})

#-----------------------------------------------------------------------------
#       test set 02
#-----------------------------------------------------------------------------

set(TARGET_NAME SerializerBase02)

set(SOURCE_LIST test_serializer_base02.cpp test_serializer_base02.h)
add_test_function(${TARGET_NAME} ${SOURCE_LIST})

#-----------------------------------------------------------------------------
#       test set 03
#-----------------------------------------------------------------------------

set(TARGET_NAME SerializerBase03)

set(SOURCE_LIST test_serializer_base03.cpp test_serializer_base03.h)
add_test_function(${TARGET_NAME} ${SOURCE_LIST})

#-----------------------------------------------------------------------------
#       test meta deserializer
#-----------------------------------------------------------------------------

set(TARGET_NAME SerializerMetaLoad)

set(SOURCE_LIST test_serializer_meta_load.cpp)
add_test_function(${TARGET_NAME} ${SOURCE_LIST})
