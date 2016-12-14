#[[###########################################################################
        TheolizerDriverを使うビルドのテスト

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

set(TARGET_NAME version_)

#-----------------------------------------------------------------------------
#       Projects
#-----------------------------------------------------------------------------

set(VERSION ver1)
set(SOURCE_LIST                ${CMAKE_CURRENT_BINARY_DIR}/${VERSION}/main.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${VERSION}/test_basic_process.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${VERSION}/test_class_variation.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${VERSION}/test_enum_variation.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${VERSION}/test_object_tracking.cpp)

message(STATUS "start of add_build_and_test(${TARGET_NAME}${VERSION})")
add_build_and_test(${TARGET_NAME} ${VERSION} "" ${SOURCE_LIST})
