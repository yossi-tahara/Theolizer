#[[###########################################################################
        ファイル構成処理

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

message(STATUS "VERSION_STRING=${VERSION_STRING}")
message(STATUS "SOURCE_DIR    =${SOURCE_DIR}")
message(STATUS "VERSION_DIR   =${VERSION_DIR}")

# source.hash のハッシュ値をSOURCES_HASHへ
file(MD5 source.hash SOURCES_HASH)
#message(STATUS "SOURCES_HASH =${SOURCES_HASH}")

# library.hashのハッシュ値をLIBRARY_HASHへ
file(MD5 library.hash LIBRARY_HASH)
#message(STATUS "LIBRARY_HASH =${LIBRARY_HASH}")

# version.h生成
configure_file(
    ${SOURCE_DIR}/tools/version.h.in
    ${VERSION_DIR}/version.h
)
