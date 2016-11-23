#[[###########################################################################
        Theolizerソースの修正後、ビルドされていことをチェックする
            修正後ビルドされていない場合、1で終了する

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

set(IS_DIRTY FALSE)

message(STATUS "CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/source")

# ライブラリのチェック
message(STATUS "Library...")
set(SOURCES_HASH_TXT "${SOURCE_DIR}/library/theolizer/internal/sources_hash.txt")
if (NOT EXISTS "${SOURCES_HASH_TXT}")
    message(STATUS "no sources_hash.txt.")
    set(IS_DIRTY TRUE)
else()
    set(CURRENT_SOURCE_DIR "${SOURCE_DIR}/library")
    include("${SOURCE_DIR}/sources_hash.cmake")

    file(READ "${SOURCES_HASH_TXT}" SOURCES_HASH_BUILD OFFSET 3)
    string(STRIP "${SOURCES_HASH_BUILD}" SOURCES_HASH_BUILD)
    message(STATUS "${SOURCES_HASH_BUILD}")

    if (NOT "${SOURCES_HASH}" STREQUAL "${SOURCES_HASH_BUILD}")
        set(IS_DIRTY TRUE)
    endif()
endif()

# ドライバのチェック
message(STATUS "Driver...")
set(SOURCES_HASH_TXT "${SOURCE_DIR}/driver/sources_hash.txt")
if (NOT EXISTS "${SOURCES_HASH_TXT}")
    message(STATUS "no sources_hash.txt.")
    set(IS_DIRTY TRUE)
else()
    set(CURRENT_SOURCE_DIR "${SOURCE_DIR}/driver")
    include("${SOURCE_DIR}/sources_hash.cmake")

    file(READ "${SOURCES_HASH_TXT}" SOURCES_HASH_BUILD OFFSET 3)
    string(STRIP "${SOURCES_HASH_BUILD}" SOURCES_HASH_BUILD)
    message(STATUS "${SOURCES_HASH_BUILD}")

    if (NOT "${SOURCES_HASH}" STREQUAL "${SOURCES_HASH_BUILD}")
        set(IS_DIRTY TRUE)
    endif()
endif()

if (IS_DIRTY)
    message(SEND_ERROR "sources are dirty.")
else()
    message(STATUS "sourcee are clean.")
endif()
