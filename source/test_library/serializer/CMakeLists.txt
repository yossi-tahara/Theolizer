#[[###########################################################################
        各serializerのテスト

    Copyright (c) 2016 Yohinori Tahara(Theoride Technology) - http://theolizer.com/
    Theolizer is registered trademark of Theoride Technology.

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

#-----------------------------------------------------------------------------
#       debug
#-----------------------------------------------------------------------------

if(FALSE)

set(TARGET_NAME Debug_basic)
set(SOURCE_LIST                main.cpp)
set(SOURCE_LIST ${SOURCE_LIST} test_basic_process.cpp)
set(SOURCE_LIST ${SOURCE_LIST} test_class_variation.cpp)
set(SOURCE_LIST ${SOURCE_LIST} test_enum_variation.cpp)

elseif(FALSE)

set(TARGET_NAME Debug_basic2)
set(SOURCE_LIST                main.cpp)
set(SOURCE_LIST ${SOURCE_LIST} test_object_tracking.cpp)
set(SOURCE_LIST ${SOURCE_LIST} test_object_tracking2.cpp)
set(SOURCE_LIST ${SOURCE_LIST} test_object_tracking3.cpp)
set(SOURCE_LIST ${SOURCE_LIST} test_polymorphism.cpp)
set(SOURCE_LIST ${SOURCE_LIST} test_destinations.cpp)
set(SOURCE_LIST ${SOURCE_LIST} test_support_stl.cpp)

else()

set(TARGET_NAME Debug_ver1b)
set(SOURCE_LIST                main.cpp)
set(SOURCE_LIST ${SOURCE_LIST} test_modify_enum.cpp)
set(SOURCE_LIST ${SOURCE_LIST} test_modify_class.cpp)
set(SOURCE_LIST ${SOURCE_LIST} test_modify_complex.cpp)

endif()

add_test_function(${TARGET_NAME} ${SOURCE_LIST})
