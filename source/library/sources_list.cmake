#[[###########################################################################
        Theolizerライブラリのソース・リストを作成する
            CMAKE_SOURCE_DIR        : (in) ルートのソース・フォルダのパス
            CURRENT_SOURCE_DIR      : (in) カレントのソース・フォルダのパス
            THEOLIZER_LIB_LIST      : (out)Theolizerライブラリ・ソース・リスト
            THEOLIZER_TEST_LIST     : (out)Theolizerテスト・ライブラリ・ソース・リスト
            VERSION_SOURCE          : (out)version.h/source_hash.txt用のソース・リスト

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
    "${CURRENT_SOURCE_DIR}/test_*.*"
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
