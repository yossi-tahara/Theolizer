#[[###########################################################################
        Theolizerソースの修正後、テストにPASSしていることをチェックする
            テストに通ったら、sources_hash.txtが更新されていることが必須。
            修正後PASSしていない場合、1で終了する

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

# test_libraryのチェック
message(STATUS "test_library...")
set(SOURCES_HASH_TXT "${BINARY_DIR}/test_library/sources_hash.txt")
if (NOT EXISTS "${SOURCES_HASH_TXT}")
    message(STATUS "no sources_hash.txt.")
    set(IS_NOT_PASS TRUE)
else()
    set(CURRENT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/source/test_library")
    include("${CMAKE_SOURCE_DIR}/source/tools/sources_hash.cmake")

    file(READ "${SOURCES_HASH_TXT}" SOURCES_HASH_BUILD OFFSET 3)
    string(STRIP "${SOURCES_HASH_BUILD}" SOURCES_HASH_BUILD)
    message(STATUS "${SOURCES_HASH_BUILD}")

    if (NOT "${SOURCES_HASH}" STREQUAL "${SOURCES_HASH_BUILD}")
        set(IS_NOT_PASS TRUE)
    endif()
endif()

# reference_and_testのチェック
message(STATUS "reference_and_test...")
set(SOURCES_HASH_TXT "${BINARY_DIR}/reference_and_test/sources_hash.txt")
if (NOT EXISTS "${SOURCES_HASH_TXT}")
    message(STATUS "no sources_hash.txt.")
    set(IS_NOT_PASS TRUE)
else()
    set(CURRENT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/source/reference_and_test")
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
