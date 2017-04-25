#[[###########################################################################
        buildエラー・テスト

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

message(STATUS "building")
message(STATUS "BINARY_DIR=${BINARY_DIR}")
message(STATUS "CONFIG=${CONFIG}")
message(STATUS "TARGET_NAME=${TARGET_NAME}")
message(STATUS "ERROR_NUMBER=${ERROR_NUMBER}")

execute_process(
    COMMAND ${CMAKE_COMMAND}
        --build "${BINARY_DIR}"
        --config ${CONFIG}
        --target ${TARGET_NAME}
    OUTPUT_VARIABLE OUTPUT_STRING
    ERROR_VARIABLE  OUTPUT_STRING
)
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
