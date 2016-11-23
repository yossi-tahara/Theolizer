#[[###########################################################################
        Theolizerドライバのソース・リストを作成する
            CURRENT_SOURCE_DIR      : (in) カレントのソース・フォルダのパス
            VERSION_SOURCE          : (in) Theolizerライブラリのハッシュ用ソース・リスト
            THEOLIZER_DRIVER_LIST   : (out)ドライバ・ビルド用のソース・リスト
            VERSION_SOURCE          : (out)version.h/source_hash.txt用のソース・リスト

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

#       ---<<< make source list >>>---

file(GLOB_RECURSE THEOLIZER_DRIVER_LIST
    "${CURRENT_SOURCE_DIR}/*.cpp"
    "${CURRENT_SOURCE_DIR}/*.c"
    "${CURRENT_SOURCE_DIR}/*.h"
    "${CURRENT_SOURCE_DIR}/*.inc"
    "${CURRENT_SOURCE_DIR}/*.hpp"
)
file(GLOB_RECURSE VERSION_H "${CURRENT_SOURCE_DIR}/version.h")
if (NOT "${VERSION_H}" STREQUAL "")
    list(REMOVE_ITEM THEOLIZER_DRIVER_LIST ${VERSION_H})
endif()
file(GLOB_RECURSE HELPER_LIST "${CURRENT_SOURCE_DIR}/helper/*")
if (NOT "${HELPER_LIST}" STREQUAL "")
    list(REMOVE_ITEM THEOLIZER_DRIVER_LIST ${HELPER_LIST})
endif()

list(SORT THEOLIZER_DRIVER_LIST)

#       ---<<< make version.h at config >>>---

# ドライバのソースとライブラリのソース
set(VERSION_SOURCE "${THEOLIZER_DRIVER_LIST};${VERSION_SOURCE}")
# 関連するCMakeLists.txtも含める
set(VERSION_SOURCE "${VERSION_SOURCE};${CURRENT_SOURCE_DIR}/CMakeLists.txt")
