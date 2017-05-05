#[[###########################################################################
        TheolizerソースのHash値計算
            VERSION_SOURCEのソース・リストに対して行う。
            なお、修正チェック用なのでCRLFの正規化は行わない。

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

if (${CURRENT_SOURCE_DIR} MATCHES "driver")
    set(CURRENT_SOURCE_DIR_BK "${CURRENT_SOURCE_DIR}")
    set(CURRENT_SOURCE_DIR "${CURRENT_SOURCE_DIR}/../library")
    include("${CURRENT_SOURCE_DIR}/sources_list.cmake")
    set(CURRENT_SOURCE_DIR "${CURRENT_SOURCE_DIR_BK}")
endif()

include("${CURRENT_SOURCE_DIR}/sources_list.cmake")

set(HASH_LIST "")
foreach(SOURCE IN LISTS VERSION_SOURCE)
    #message(STATUS "${SOURCE}")
    file(MD5 "${SOURCE}" HASH)
    set(HASH_LIST "${HASH_LIST} ${HASH}")
endforeach()
string(MD5 SOURCES_HASH "${HASH_LIST}")
message(STATUS ${SOURCES_HASH})
