#[[###########################################################################
        各serializerのテスト

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

#-----------------------------------------------------------------------------
#       Base
#-----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.2.2)

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

#-----------------------------------------------------------------------------
#       debug
#-----------------------------------------------------------------------------

if(FALSE)
set(TARGET_NAME Debug)

set(SOURCE_LIST main.cpp main.h main.cpp.theolizer.hpp)
add_test_function(${TARGET_NAME} ${SOURCE_LIST})

endif()
