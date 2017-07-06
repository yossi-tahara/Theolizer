#[[###########################################################################
        buildエラー・テスト

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

message(STATUS "BuildingError Test")
message(STATUS "BINARY_DIR=${BINARY_DIR}")
message(STATUS "CONFIG=${CONFIG}")
message(STATUS "TARGET_NAME=${TARGET_NAME}")
message(STATUS "ERROR_NUMBER=${ERROR_NUMBER}")

if ("${CMAKE_VERSION}" VERSION_LESS "3.8.0")
    execute_process(
        COMMAND ${CMAKE_COMMAND}
            --build "${BINARY_DIR}"
            --config ${CONFIG}
            --target ${TARGET_NAME}
        OUTPUT_VARIABLE OUTPUT_STRING
        ERROR_VARIABLE  OUTPUT_STRING
    )
else()
    execute_process(
        COMMAND ${CMAKE_COMMAND}
            --build "${BINARY_DIR}"
            --config ${CONFIG}
            --target ${TARGET_NAME}
        OUTPUT_VARIABLE OUTPUT_STRING
        ERROR_VARIABLE  OUTPUT_STRING
        ENCODING AUTO
    )
endif()

string(REGEX MATCHALL "${ERROR_MESSAGE}" RESULT "${OUTPUT_STRING}")
list(LENGTH RESULT NUMBER)
message(STATUS "NUMBER=${NUMBER}")

if(NOT NUMBER EQUAL ${ERROR_NUMBER})
    message(STATUS "OUTPUT_STRING=${OUTPUT_STRING}")
    message(STATUS "ERROR_MESSAGE=${ERROR_MESSAGE}")
    message(STATUS "TARGET_NAME=${TARGET_NAME}")
    message(STATUS "ERROR_NUMBER=${ERROR_NUMBER}")
    message(STATUS "NUMBER=${NUMBER}")
    message(SEND_ERROR "${TARGET_NAME} test is failure.")
endif()
