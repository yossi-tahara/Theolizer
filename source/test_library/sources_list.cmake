#[[###########################################################################
        Theolizerライブラリ・テストのソース・リストを作成する
            CURRENT_SOURCE_DIR      : (in) カレントのソース・フォルダのパス
            VERSION_SOURCE          : (out)ソース・リスト

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

macro(add_sources PATH PREFIX)
    file(GLOB LIST ${PATH}/CMakeLists.txt ${PATH}/${PREFIX}*.cpp ${PATH}/${PREFIX}*.h ${PATH}/${PREFIX}*.hpp)
    set(VERSION_SOURCE ${VERSION_SOURCE} ${LIST})
endmacro()

file(GLOB VERSION_SOURCE ${CURRENT_SOURCE_DIR}/CMakeLists.txt ${CURRENT_SOURCE_DIR}/*.cmake)
add_sources(${CURRENT_SOURCE_DIR}/base "")
add_sources(${CURRENT_SOURCE_DIR}/report "")
add_sources(${CURRENT_SOURCE_DIR}/serializer test_serializer_)
add_sources(${CURRENT_SOURCE_DIR}/test_tool "")
add_sources(${CURRENT_SOURCE_DIR}/u8string "")

foreach(FILE IN LISTS VERSION_SOURCE)
#   message(STATUS "FILE=${FILE}")
endforeach()

