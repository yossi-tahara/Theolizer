#[[###########################################################################
        ビルド用補助CMakeスクリプト(パラメータ展開先)

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

message(STATUS "copy.cmake : SOURCE_DIR=${SOURCE_DIR}")
message(STATUS "copy.cmake : PREV_DIR  =${PREV_DIR}")
message(STATUS "copy.cmake : BINARY_DIR=${BINARY_DIR}")
message(STATUS "copy.cmake : MATCH_STRING  =${MATCH_STRING}")
message(STATUS "copy.cmake : REPLACE_STRING=${REPLACE_STRING}")

file(GLOB COMMON_LIST ${SOURCE_DIR}/*.h ${SOURCE_DIR}/*.inc)
foreach(FILE IN LISTS COMMON_LIST)
    message(STATUS "${FILE}")
    configure_file("${FILE}" "${BINARY_DIR}/" COPYONLY)
endforeach()

# 前バージョンが現バージョンと異なるなら、*.theolizer.hppをコピーする
if(NOT "${PREV_DIR}" STREQUAL "${BINARY_DIR}")
    file(GLOB HPP_LIST ${PREV_DIR}/*.theolizer.hpp)
    foreach(FILE IN LISTS HPP_LIST)
        message(STATUS "${FILE}")
        configure_file("${FILE}" "${BINARY_DIR}/" COPYONLY)
        if (NOT "${MATCH_STRING}" STREQUAL "")
            get_filename_component(FILE_NAME "${FILE}" NAME)
            set(DEST_FILE "${BINARY_DIR}/${FILE_NAME}")
            file(READ ${DEST_FILE} STRINGS)
            string(REPLACE "${MATCH_STRING}" "${REPLACE_STRING}" STRINGS "${STRINGS}")
            file(WRITE ${DEST_FILE} "${STRINGS}")
        endif()
    endforeach()
endif()
