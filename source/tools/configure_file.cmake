#[[###########################################################################
        ファイル構成処理

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
