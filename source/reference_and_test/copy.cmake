#[[###########################################################################
        ビルド用補助CMakeスクリプト(パラメータ展開先)

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

message(STATUS "copy.cmake : SOURCE_DIR=${SOURCE_DIR}")
message(STATUS "copy.cmake : PREV_DIR  =${PREV_DIR}")
message(STATUS "copy.cmake : BINARY_DIR=${BINARY_DIR}")

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
    endforeach()
endif()
