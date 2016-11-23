#[[###########################################################################
        Theolizerライブラリのソース・リストを作成する
            CMAKE_SOURCE_DIR        : (in) ルートのソース・フォルダのパス
            CURRENT_SOURCE_DIR      : (in) カレントのソース・フォルダのパス
            THEOLIZER_LIB_LIST      : (out)Theolizerライブラリ・ソース・リスト
            THEOLIZER_TEST_LIST     : (out)Theolizerテスト・ライブラリ・ソース・リスト
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

# TheolzierLib
file(GLOB_RECURSE THEOLIZER_LIB_LIST
    "${CURRENT_SOURCE_DIR}/*.cpp"
    "${CURRENT_SOURCE_DIR}/*.c"
    "${CURRENT_SOURCE_DIR}/*.h"
    "${CURRENT_SOURCE_DIR}/*.inc"
    "${CURRENT_SOURCE_DIR}/*.hpp"
)

# TheolizerTest
file(GLOB_RECURSE THEOLIZER_TEST_LIST
    "${CURRENT_SOURCE_DIR}/test_tool.*"
)
list(SORT THEOLIZER_TEST_LIST)

# version.hとTheolizerTestを除く
file(GLOB_RECURSE VERSION_H "${CURRENT_SOURCE_DIR}/version.h")
if (NOT "${VERSION_H}" STREQUAL "")
    list(REMOVE_ITEM THEOLIZER_LIB_LIST ${VERSION_H})
endif()

if (NOT "${THEOLIZER_TEST_LIST}" STREQUAL "")
    list(REMOVE_ITEM THEOLIZER_LIB_LIST ${THEOLIZER_TEST_LIST})
endif()
list(SORT THEOLIZER_LIB_LIST)

# ハッシュ値計算用ソース（CMakeLists.txt追加)
set(VERSION_SOURCE "${THEOLIZER_LIB_LIST}")
# 関連するCMakeLists.txtも含める
set(VERSION_SOURCE "${VERSION_SOURCE};${CURRENT_SOURCE_DIR}/../CMakeLists.txt")
set(VERSION_SOURCE "${VERSION_SOURCE};${CURRENT_SOURCE_DIR}/CMakeLists.txt")
