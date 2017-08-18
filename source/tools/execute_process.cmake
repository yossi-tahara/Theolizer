#[[###########################################################################
        CTest内部でexecute_process中継する
            CMake 3.8.0にて、CTestは子プロセス出力をUTF-8固定になったため、
            このexecute_processでUTF-8へ変換する。

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

message(STATUS "TARGET_NAME     =${TARGET_NAME}")
message(STATUS "WILL_FAIL       =${WILL_FAIL}")
message(STATUS "PARAM_LIST      =${PARAM_LIST}")

set(VALGRIND "")
set(VALGRIND_PARAM "")
if(UNIX)
    execute_process(COMMAND which valgrind OUTPUT_VARIABLE WHICH_VALGRIND RESULT_VARIABLE RETUEN_CODE)
    string(REPLACE "\n" "" WHICH_VALGRIND "${WHICH_VALGRIND}")
    message(STATUS "WHICH_VALGRIND=${WHICH_VALGRIND}")
    message(STATUS "RETUEN_CODE   =${RETUEN_CODE}")
    if(${RETUEN_CODE} EQUAL 0) 
        set(VALGRIND "valgrind")
        set(VALGRIND_PARAM "-q" "--leak-check=full")
    endif()
endif()

if ("${CMAKE_VERSION}" VERSION_LESS "3.8.0")

    execute_process(COMMAND ${VALGRIND} ${VALGRIND_PARAM}
        ${TARGET_NAME} ${PARAM_LIST} RESULT_VARIABLE RETUEN_CODE)

elseif (("${TARGET_NAME}" STREQUAL "${CMAKE_COMMAND}") AND (NOT "${PARAM_LIST}" MATCHES "--build"))

    execute_process(COMMAND ${VALGRIND} ${VALGRIND_PARAM}
        ${TARGET_NAME} ${PARAM_LIST} RESULT_VARIABLE RETUEN_CODE ENCODING UTF8)

else()

    execute_process(COMMAND ${VALGRIND} ${VALGRIND_PARAM}
        ${TARGET_NAME} ${PARAM_LIST} RESULT_VARIABLE RETUEN_CODE ENCODING AUTO)

endif()
message(STATUS "VALGRIND=${VALGRIND}")

if ((NOT ${RETUEN_CODE} EQUAL 0) AND ("${WILL_FAIL}" STREQUAL ""))
    message(SEND_ERROR "RETUEN_CODE=${RETUEN_CODE}")
elseif ((${RETUEN_CODE} EQUAL 0) AND (NOT "${WILL_FAIL}" STREQUAL ""))
    message(SEND_ERROR "RETUEN_CODE=${RETUEN_CODE} at WILL_FAIL=${WILL_FAIL}")
endif()

#execute_process(COMMAND ${TARGET_NAME} ${PARAM_LIST})
#execute_process(
#    COMMAND ${TARGET_NAME} ${PARAM_LIST}
#    OUTPUT_FILE ${CMAKE_BINARY_DIR}/temp.log
#    ERROR_FILE  ${CMAKE_BINARY_DIR}/temp.log
#)
