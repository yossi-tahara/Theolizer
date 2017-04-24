#[[###########################################################################
        TheolizerソースのHash値計算
            VERSION_SOURCEのソース・リストに対して行う。
            なお、修正チェック用なのでCRLFの正規化は行わない。

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
