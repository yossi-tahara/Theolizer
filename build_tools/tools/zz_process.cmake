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

#message(STATUS "zz_process.cmake ${PROC} ${SUMMARY}")

#-----------------------------------------------------------------------------
#       パラメータ設定
#-----------------------------------------------------------------------------

set(THEOLIZER_SOURCE "@THEOLIZER_SOURCE@")
set(THEOLIZER_BINARY "@THEOLIZER_BINARY@")
set(THEOLIZER_PREFIX "@THEOLIZER_PREFIX@")
set(LIB_TYPE         "@LIB_TYPE@")
set(COMPLIER         "@COMPLIER@")
set(BIT_NUM          "@BIT_NUM@")
set(CONFIG_TYPE      "@CONFIG_TYPE@")
set(BUILD_DRIVER     "@BUILD_DRIVER@")
set(BUILD_DOCUMENT   "@BUILD_DOCUMENT@")
set(GENERATOR        "@GENERATOR@")
set(BUILD_DIR        "@BUILD_DIR@")
set(BOOST_ROOT       "@BOOST_ROOT@")
set(LLVM_ROOT        "@LLVM_ROOT@")
set(MAKE             "@MAKE@")
set(PARALLEL         "@PARALLEL@")
set(MSVC_PATH        "@MSVC_PATH@")
set(CC_PATH          "@CC_PATH@")
set(ORIGINAL_DIR     "@CMAKE_SOURCE_DIR@")

set(THEOLIZER_ROOT "${THEOLIZER_PREFIX}${COMPLIER}x${BIT_NUM}")

if(FALSE)
    message(STATUS "THEOLIZER_SOURCE=${THEOLIZER_SOURCE}")
    message(STATUS "THEOLIZER_BINARY=${THEOLIZER_BINARY}")
    message(STATUS "THEOLIZER_PREFIX=${THEOLIZER_PREFIX}")
    message(STATUS "LIB_TYPE        =${LIB_TYPE}")
    message(STATUS "COMPLIER        =${COMPLIER}")
    message(STATUS "BIT_NUM         =${BIT_NUM}")
    message(STATUS "CONFIG_TYPE     =${CONFIG_TYPE}")
    message(STATUS "BUILD_DRIVER    =${BUILD_DRIVER}")
    message(STATUS "BUILD_DOCUMENT  =${BUILD_DOCUMENT}")
    message(STATUS "GENERATOR       =${GENERATOR}")
    message(STATUS "BUILD_DIR       =${BUILD_DIR}")
    message(STATUS "BOOST_ROOT      =${BOOST_ROOT}")
    message(STATUS "LLVM_ROOT       =${LLVM_ROOT}")
    message(STATUS "MAKE            =${MAKE}")
    message(STATUS "PARALLEL        =${PARALLEL}")
    message(STATUS "MSVC_PATH       =${MSVC_PATH}")
    message(STATUS "CC_PATH         =${CC_PATH}")

    message(STATUS "PROC_ALL        =${PROC_ALL}")
    message(STATUS "THEOLIZER_ROOT  =${THEOLIZER_ROOT}")
endif()

set(BUILD_TYPE "")
if(NOT "${CONFIG_TYPE}" STREQUAL "")
    set(BUILD_TYPE "-DCMAKE_BUILD_TYPE=${CONFIG_TYPE}")
endif()

#-----------------------------------------------------------------------------
#       サブ・ファンクション等
#-----------------------------------------------------------------------------

#       ---<<< パラメータをログへ出力 >>>---

function(parameter_log LOG_FILE)

    file(WRITE  ${LOG_FILE} "--- Parameters ---\n")
    file(APPEND ${LOG_FILE} "THEOLIZER_SOURCE=${THEOLIZER_SOURCE}\n")
    file(APPEND ${LOG_FILE} "THEOLIZER_BINARY=${THEOLIZER_BINARY}\n")
    file(APPEND ${LOG_FILE} "THEOLIZER_PREFIX=${THEOLIZER_PREFIX}\n")
    file(APPEND ${LOG_FILE} "LIB_TYPE        =${LIB_TYPE}\n")
    file(APPEND ${LOG_FILE} "COMPLIER        =${COMPLIER}\n")
    file(APPEND ${LOG_FILE} "BIT_NUM         =${BIT_NUM}\n")
    file(APPEND ${LOG_FILE} "CONFIG_TYPE     =${CONFIG_TYPE}\n")
    file(APPEND ${LOG_FILE} "BUILD_DRIVER    =${BUILD_DRIVER}\n")
    file(APPEND ${LOG_FILE} "BUILD_DOCUMENT  =${BUILD_DOCUMENT}\n")
    file(APPEND ${LOG_FILE} "GENERATOR       =${GENERATOR}\n")
    file(APPEND ${LOG_FILE} "BUILD_DIR       =${BUILD_DIR}\n")
    file(APPEND ${LOG_FILE} "BOOST_ROOT      =${BOOST_ROOT}\n")
    file(APPEND ${LOG_FILE} "LLVM_ROOT       =${LLVM_ROOT}\n")
    file(APPEND ${LOG_FILE} "MAKE            =${MAKE}\n")
    file(APPEND ${LOG_FILE} "PARALLEL        =${PARALLEL}\n")
    file(APPEND ${LOG_FILE} "MSVC_PATH       =${MSVC_PATH}\n")
    file(APPEND ${LOG_FILE} "CC_PATH         =${CC_PATH}\n")

    file(APPEND ${LOG_FILE} "PROC_ALL        =${PROC_ALL}\n")
    file(APPEND ${LOG_FILE} "THEOLIZER_ROOT  =${THEOLIZER_ROOT}\n")
    file(APPEND ${LOG_FILE} "\n")

endfunction()

#       ---<<< 現在時刻獲得->NOW_TIME >>>---

macro(get_current_time)

    if(WIN32)
        execute_process(COMMAND cmd /C echo "%TIME%" OUTPUT_VARIABLE NOW_TIME)
    else()
        execute_process(COMMAND date +%T OUTPUT_VARIABLE NOW_TIME)
    endif()
    string(STRIP ${NOW_TIME} NOW_TIME)

endmacro()

#       ---<<< search >>>---

function(search LOG_FILE REGEX_STRING)

    file(STRINGS  ${LOG_FILE} FOUND_LIST REGEX "${REGEX_STRING}")
    foreach(LINE IN LISTS FOUND_LIST)
        message(STATUS "${LINE}")
        if (NOT "${SUMMARY}" STREQUAL "")
            file(APPEND ${SUMMARY} "${LINE}\n")
        endif()
    endforeach()

endfunction()

#       ---<<< 開始処理:開始時刻->START_TIME >>>---

macro(start OUTPUT_MESSAGE)

    get_current_time()
    set(START_TIME ${NOW_TIME})
    message(STATUS "${OUTPUT_MESSAGE} ${START_TIME}")

    if (NOT "${SUMMARY}" STREQUAL "")
        file(APPEND ${SUMMARY} "${OUTPUT_MESSAGE} ${START_TIME}\n")
    endif()

    set(RELAY_RECORD "")

endmacro()

#       ---<<< 中間処理:開始時刻->NOW_TIME >>>---

macro(relay OUTPUT_MESSAGE)

    get_current_time()
    message(STATUS "${OUTPUT_MESSAGE} ${NOW_TIME}")
    set(RELAY_RECORD "${RELAY_RECORD}${OUTPUT_MESSAGE} ${NOW_TIME}\n")

endmacro()

#       ---<<< 終了処理 >>>---

macro(end LOG_FILE BREAK)

    get_current_time()
    parameter_log(${LOG_FILE})
    file(APPEND ${LOG_FILE} "--- Result ---\n")
    file(APPEND ${LOG_FILE} "RETURN_CODE=${RETURN_CODE}\n")
    file(APPEND ${LOG_FILE} "Start ${START_TIME}\n")
    file(APPEND ${LOG_FILE} "End   ${NOW_TIME}\n")
    file(APPEND ${LOG_FILE} "${RELAY_RECORD}")
    file(APPEND ${LOG_FILE} "---------------------------------------\n")
    file(APPEND ${LOG_FILE} "${OUTPUT_LOG}\n")

    if(${RETURN_CODE} EQUAL 0)
        message(STATUS "Done.             ${NOW_TIME}")
        if (NOT "${SUMMARY}" STREQUAL "")
            file(APPEND ${SUMMARY} "Done.             ${NOW_TIME}\n")
        endif()
    else()
        message(STATUS "error: ${RETURN_CODE} ${NOW_TIME}")
        if (NOT "${SUMMARY}" STREQUAL "")
            file(APPEND ${SUMMARY} "error: ${RETURN_CODE} ${NOW_TIME}\n")
        endif()
    endif()

    search(${LOG_FILE} "Configuring incomplete,")
    search(${LOG_FILE} ": warning")
    search(${LOG_FILE} ": error ")
    search(${LOG_FILE} ": undefined reference to ")
    search(${LOG_FILE} "% tests passed")

    if(BREAK)
        if(NOT ${RETURN_CODE} EQUAL 0)
            message(SEND_ERROR "")
        endif()
    endif()

    # テスト結果の数を確認
    if(NOT "${ARGN}" STREQUAL "")
        set(PASS_LIST ${ARGN})
        set(INDEX 0)
        file(STRINGS  ${LOG_FILE} FOUND_LIST REGEX "% tests passed")
        foreach(LINE IN LISTS FOUND_LIST)
            list(GET PASS_LIST ${INDEX} PASS_NUMBER)
            math(EXPR INDEX "${INDEX} + 1")

##          message(STATUS "${LINE}")
            if(NOT "${LINE}" MATCHES "100%")
                message(STATUS "error: ${LINE}")
                if (NOT "${SUMMARY}" STREQUAL "")
                    file(APPEND ${SUMMARY} "error: ${LINE}\n")
                endif()
        continue()
            endif()

            string(REGEX MATCH "out of [0-9]+" NUMBER "${LINE}")
            string(SUBSTRING "${NUMBER}" 7 10 NUMBER)
            if (NOT ${NUMBER} EQUAL ${PASS_NUMBER})
                message(STATUS "error: ${LINE} -> expected ${PASS_NUMBER}")
                if (NOT "${SUMMARY}" STREQUAL "")
                    file(APPEND ${SUMMARY} "error: ${LINE} -> expected ${PASS_NUMBER}\n")
                endif()
            endif()
        endforeach()
    endif()

endmacro()

#       ---<<< ビルド実行 >>>---

macro(build TARGET CONFIG_TYPE WORKING_DIR)

    if(${GENERATOR} MATCHES "Visual Studio")
        execute_process(
            COMMAND ${CMAKE_COMMAND}
                --build . 
                --config ${CONFIG_TYPE}
                --target ${TARGET}
            WORKING_DIRECTORY "${WORKING_DIR}"
            OUTPUT_VARIABLE OUTPUT_LOG
            ERROR_VARIABLE  OUTPUT_LOG
            RESULT_VARIABLE RETURN_CODE
        )
    else()
        execute_process(
            COMMAND ${MAKE} ${TARGET} ${PARALLEL}
            WORKING_DIRECTORY "${WORKING_DIR}"
            OUTPUT_VARIABLE OUTPUT_LOG
            ERROR_VARIABLE  OUTPUT_LOG
            RESULT_VARIABLE RETURN_CODE
        )
    endif()

endmacro()

#       ---<<< 最後のテスト >>>---

# サンプルのビルド
macro(prepare_sample TARGET)

    # サンプルのビルド・フォルダ準備
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${TARGET}"
    )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${THEOLIZER_SOURCE}/samples/${TARGET}" "${TARGET}"
    )
    # サンプルのコンフィグ
    execute_process(
        COMMAND ${CMAKE_COMMAND}
            -G "${GENERATOR}" .
            "-DTHEOLIZER_DIR=${THEOLIZER_ROOT}"
            "-DBOOST_ROOT=${BOOST_ROOT}"
            "-DLIB_TYPE=${LIB_TYPE}"
            ${BUILD_TYPE}
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/${TARGET}"
        OUTPUT_VARIABLE OUTPUT_LOG
        ERROR_VARIABLE  OUTPUT_LOG
        RESULT_VARIABLE RETURN_CODE1
    )
    set(TEMP_LOG2 "${TEMP_LOG2}${OUTPUT_LOG}")

    # サンプルのビルドと実行
    if("${CONFIG_TYPE}" STREQUAL "")
        build(BuildTest Release "${CMAKE_SOURCE_DIR}/${TARGET}")
        set(TEMP_LOG2 "${TEMP_LOG2}${OUTPUT_LOG}")
        build(BuildTest Debug "${CMAKE_SOURCE_DIR}/${TARGET}")
        set(TEMP_LOG2 "${TEMP_LOG2}${OUTPUT_LOG}")
    else()
        build(BuildTest ${CONFIG_TYPE} "${CMAKE_SOURCE_DIR}/${TARGET}")
        set(TEMP_LOG2 "${TEMP_LOG2}${OUTPUT_LOG}")
    endif()

endmacro()

# 最後のテスト処理
macro(last_test)

    set(SUDO "")
    if(NOT WIN32)
        set(SUDO "sudo")
    endif()

    set(TEMP_LOG2 "")
    file(READ ${CMAKE_SOURCE_DIR}/compiler.txt COMPILER)
#   message(STATUS "COMPILER=${COMPILER}")

    # Theolizerドライバーがリプレースされていたらリストアしておく
    execute_process(
        COMMAND ${SUDO} ${THEOLIZER_ROOT}/bin/TheolizerDriver --theolizer-restore=${COMPILER}
        OUTPUT_VARIABLE OUTPUT_LOG
        ERROR_VARIABLE  OUTPUT_LOG
    )
    set(TEMP_LOG2 "${TEMP_LOG2}${OUTPUT_LOG}")

    # Theolizerドライバーのリプレース
    execute_process(
        COMMAND ${SUDO} ${THEOLIZER_ROOT}/bin/TheolizerDriver --theolizer-replace=${COMPILER}
        OUTPUT_VARIABLE OUTPUT_LOG
        ERROR_VARIABLE  OUTPUT_LOG
        RESULT_VARIABLE RETURN_CODE
    )
    set(TEMP_LOG2 "${TEMP_LOG2}${OUTPUT_LOG}")
    if(NOT ${RETURN_CODE} EQUAL 0)
        message(SEND_ERROR "${RETURN_CODE}")
return()
    endif()

    # サンプルのビルドとテスト
    prepare_sample(example)

    # Theolizerドライバーのリストア
    execute_process(
        COMMAND ${SUDO} ${THEOLIZER_ROOT}/bin/TheolizerDriver --theolizer-restore=${COMPILER}
        OUTPUT_VARIABLE OUTPUT_LOG
        ERROR_VARIABLE  OUTPUT_LOG
        RESULT_VARIABLE RETURN_CODE2
    )
    set(OUTPUT_LOG "${TEMP_LOG2}${OUTPUT_LOG}")

    if(NOT ${RETURN_CODE1} EQUAL 0)
        set(RETURN_CODE ${RETURN_CODE1})
    endif()
    if(NOT ${RETURN_CODE2} EQUAL 0)
        set(RETURN_CODE ${RETURN_CODE2})
    endif()

endmacro()

#       ---<<< PASS_LISTから１つ取り出してPASS_CHECK_LISTへ追加する >>>---

macro(get_pass_list)

    if(NOT "${PASS_LIST}" STREQUAL "")
        list(GET PASS_LIST ${INDEX} PASS_NUMBER)
        math(EXPR INDEX "${INDEX} + 1")
        set(PASS_CHECK_LIST ${PASS_CHECK_LIST} ${PASS_NUMBER})
    endif()

endmacro()

#-----------------------------------------------------------------------------
#       実行
#-----------------------------------------------------------------------------

if (NOT "${MSVC_PATH}" STREQUAL "")
    set(ENV{PATH} "${MSVC_PATH};$ENV{PATH}")
endif()

if (NOT "${CC_PATH}" STREQUAL "")
    set(ENV{PATH} "${CC_PATH};$ENV{PATH}")
endif()

#       ---<<< configure >>>---

if("${PROC}" STREQUAL "config")

    start("Configuring   ...")
    if("${BUILD_DRIVER}" STREQUAL "TRUE")
        execute_process(
            COMMAND ${CMAKE_COMMAND}
                -G "${GENERATOR}"
                "${THEOLIZER_SOURCE}"
                "-DCMAKE_INSTALL_PREFIX=${THEOLIZER_ROOT}"
                "-DLIB_TYPE=${LIB_TYPE}"
                "-DBOOST_ROOT=${BOOST_ROOT}"
                "-DLLVM_ROOT=${LLVM_ROOT}"
                "-DBUILD_DOCUMENT=${BUILD_DOCUMENT}"
                ${BUILD_TYPE}
            OUTPUT_VARIABLE OUTPUT_LOG
            RESULT_VARIABLE RETURN_CODE
        )
    else()
        execute_process(
            COMMAND ${CMAKE_COMMAND}
                -G "${GENERATOR}"
                "${THEOLIZER_SOURCE}"
                "-DCMAKE_INSTALL_PREFIX=${THEOLIZER_ROOT}"
                "-DLIB_TYPE=${LIB_TYPE}"
                "-DBOOST_ROOT=${BOOST_ROOT}"
                "-DBUILD_DOCUMENT=${BUILD_DOCUMENT}"
                ${BUILD_TYPE}
            OUTPUT_VARIABLE OUTPUT_LOG
            RESULT_VARIABLE RETURN_CODE
        )
    endif()
    end("z0_config.log" TRUE)

#       ---<<< short-test (build & test library only) >>>---

elseif("${PROC}" STREQUAL "short")

    start("Short Test    ...")
    build(ShortTest Release .)
    end("z1_short_test.log" FALSE)

#       ---<<< long-test (build & test library and driver) >>>---

elseif("${PROC}" STREQUAL "long")

    start("Long Test     ...")
    build(LongTest Release .)
    end("z2_long_test.log" FALSE)

#       ---<<< full-test (build & test library and driver, install & test) >>>---

elseif("${PROC}" STREQUAL "full")

    # 開始
    set(TEMP_LOG "")
    start("Full Test     ...")
    set(RETURN_CODE 0)
    set(PASS_CHECK_LIST "")
    set(INDEX 0)
    string(REPLACE " " ";" PASS_LIST "${PASS_LIST}")

    # Theolizerの用意
    if(NOT "${PROC_ALL}" STREQUAL "last_all")

        if("${CONFIG_TYPE}" STREQUAL "Debug")
            relay("  Debug Build ...")
            build(ShortTest Debug .)
            set(TEMP_LOG "${TEMP_LOG}${OUTPUT_LOG}")
        else()
            relay("  Release Build..")
            build(FullTest Release .)
            set(TEMP_LOG "${TEMP_LOG}${OUTPUT_LOG}")
        endif()

        get_pass_list()

        if(${RETURN_CODE} EQUAL 0)
            relay("  Installing  ...")

            # インストール先を一旦削除してからインストールする
            if((BUILD_DRIVER) AND ("${PROC_ALL}" STREQUAL "full_all"))
                execute_process(
                    COMMAND ${CMAKE_COMMAND} -E remove_directory "${THEOLIZER_ROOT}"
                )
            endif()

            build(install Release .)
            set(TEMP_LOG "${TEMP_LOG}${OUTPUT_LOG}")
        endif()

        if(${GENERATOR} MATCHES "Visual Studio")
            # デバッグ・ビルド時のTheolizerドライバは異常に遅いので
            # ドライバによるビルド・テストはしない
            relay("  Debug Build ...")
            build(ShortTest Debug .)
            set(TEMP_LOG "${TEMP_LOG}${OUTPUT_LOG}")

            get_pass_list()

            if(${RETURN_CODE} EQUAL 0)
                relay("  Installing  ...")
                build(install Debug .)
                set(TEMP_LOG "${TEMP_LOG}${OUTPUT_LOG}")
            endif()
        endif()

        # ライセンス・ファイルのコピー
        file(GLOB_RECURSE LICENSE_LIST "${ORIGINAL_DIR}/other_licenses/*")
        file(COPY ${THEOLIZER_SOURCE}/../LICENSE.TXT DESTINATION "${THEOLIZER_ROOT}")
        file(COPY ${LICENSE_LIST} DESTINATION "${THEOLIZER_ROOT}/other_licenses")

    else()
        if(${GENERATOR} MATCHES "Visual Studio")
            math(EXPR INDEX "${INDEX} + 1")
        endif()
        math(EXPR INDEX "${INDEX} + 1")
    endif()

    # インストールされたTheolizerを用いたテスト
    if(NOT "${PROC_ALL}" STREQUAL "middle_all")
        if(${RETURN_CODE} EQUAL 0)
            relay("  Last Test   ...")
            last_test()
            set(TEMP_LOG "${TEMP_LOG}${OUTPUT_LOG}")

            get_pass_list()
            if("${CONFIG_TYPE}" STREQUAL "")
                get_pass_list()
            endif()
        endif()
    endif()

    # 終了
    set(OUTPUT_LOG "${TEMP_LOG}")
    end("z3_full_test-${PROC_ALL}.log" FALSE ${PASS_CHECK_LIST})

#       ---<<< build document by Doxygen >>>---

else()

    start("Build ${PROC}      ...")
    build(${PROC} Release .)
    end("z4_${PROC}.log" FALSE)

endif()

