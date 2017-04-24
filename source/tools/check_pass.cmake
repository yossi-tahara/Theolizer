#[[###########################################################################
        Theolizerソースの修正後、テストにPASSしていることをチェックする
            テストに通ったら、sources_hash.txtが更新されていることが必須。
            修正後PASSしていない場合、1で終了する

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

message(STATUS "checking PASS or Not PASS")
#message(STATUS "CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")
#message(STATUS "BINARY_DIR=${BINARY_DIR}")

set(IS_NOT_PASS FALSE)

# ライブラリのチェック
message(STATUS "Library...")
set(SOURCES_HASH_TXT "${BINARY_DIR}/library/sources_hash.txt")
if (NOT EXISTS "${SOURCES_HASH_TXT}")
    message(STATUS "no sources_hash.txt.")
    set(IS_NOT_PASS TRUE)
else()
    set(CURRENT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/source/library")
    include("${CMAKE_SOURCE_DIR}/source/tools/sources_hash.cmake")

    file(READ "${SOURCES_HASH_TXT}" SOURCES_HASH_BUILD OFFSET 3)
    string(STRIP "${SOURCES_HASH_BUILD}" SOURCES_HASH_BUILD)
    message(STATUS "${SOURCES_HASH_BUILD}")

    if (NOT "${SOURCES_HASH}" STREQUAL "${SOURCES_HASH_BUILD}")
        set(IS_NOT_PASS TRUE)
    endif()
endif()

# ドライバのチェック
message(STATUS "Driver...")
set(SOURCES_HASH_TXT "${BINARY_DIR}/driver/sources_hash.txt")
if (NOT EXISTS "${SOURCES_HASH_TXT}")
    message(STATUS "no sources_hash.txt.")
    set(IS_NOT_PASS TRUE)
else()
    set(CURRENT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/source/driver")
    include("${CMAKE_SOURCE_DIR}/source/tools/sources_hash.cmake")

    file(READ "${SOURCES_HASH_TXT}" SOURCES_HASH_BUILD OFFSET 3)
    string(STRIP "${SOURCES_HASH_BUILD}" SOURCES_HASH_BUILD)
    message(STATUS "${SOURCES_HASH_BUILD}")

    if (NOT "${SOURCES_HASH}" STREQUAL "${SOURCES_HASH_BUILD}")
        set(IS_NOT_PASS TRUE)
    endif()
endif()

if (IS_NOT_PASS)
    message(SEND_ERROR "No test passed.")
else()
    message(STATUS "Test passed.")
endif()
