#[[###########################################################################
        ビルド用補助CMakeスクリプト(パラメータ展開先)

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

#message(STATUS "zz_process.cmake ${PROC} ${SUMMARY}")

#-----------------------------------------------------------------------------
#       パラメータ設定
#-----------------------------------------------------------------------------

set(THEOLIZER_SOURCE        "@THEOLIZER_SOURCE@")
set(THEOLIZER_BINARY        "@THEOLIZER_BINARY@")
set(THEOLIZER_PREFIX        "@THEOLIZER_PREFIX@")
set(LIB_TYPE                "@LIB_TYPE@")
set(COMPILER                "@COMPILER@")
set(BIT_NUM                 "@BIT_NUM@")
set(CONFIG_TYPE             "@CONFIG_TYPE@")
set(BUILD_DRIVER            "@BUILD_DRIVER@")
set(BUILD_DOCUMENT          "@BUILD_DOCUMENT@")
set(GENERATOR               "@GENERATOR@")
set(BUILD_DIR               "@BUILD_DIR@")
set(BOOST_ROOT              "@BOOST_ROOT@")
set(LLVM_ROOT               "@LLVM_ROOT@")
set(LLVM_VERSION            "@LLVM_VERSION@")
set(REQUIRE_CMAKE_VERSION   "@REQUIRE_CMAKE_VERSION@")
set(MAKE                    "@MAKE@")
set(PARALLEL                "@PARALLEL@")
set(MSVC_PATH               "@MSVC_PATH@")
set(CC_PATH                 "@CC_PATH@")
set(ORIGINAL_DIR            "@CMAKE_SOURCE_DIR@")

set(THEOLIZER_ROOT "${THEOLIZER_PREFIX}${COMPILER}x${BIT_NUM}")

if(FALSE)
    message(STATUS "THEOLIZER_SOURCE        =${THEOLIZER_SOURCE}")
    message(STATUS "THEOLIZER_BINARY        =${THEOLIZER_BINARY}")
    message(STATUS "THEOLIZER_PREFIX        =${THEOLIZER_PREFIX}")
    message(STATUS "LIB_TYPE                =${LIB_TYPE}")
    message(STATUS "COMPILER                =${COMPILER}")
    message(STATUS "BIT_NUM                 =${BIT_NUM}")
    message(STATUS "CONFIG_TYPE             =${CONFIG_TYPE}")
    message(STATUS "BUILD_DRIVER            =${BUILD_DRIVER}")
    message(STATUS "BUILD_DOCUMENT          =${BUILD_DOCUMENT}")
    message(STATUS "GENERATOR               =${GENERATOR}")
    message(STATUS "BUILD_DIR               =${BUILD_DIR}")
    message(STATUS "BOOST_ROOT              =${BOOST_ROOT}")
    message(STATUS "LLVM_ROOT               =${LLVM_ROOT}")
    message(STATUS "LLVM_VERSION            =${LLVM_VERSION}")
    message(STATUS "REQUIRE_CMAKE_VERSION   =${REQUIRE_CMAKE_VERSION}")
    message(STATUS "MAKE                    =${MAKE}")
    message(STATUS "PARALLEL                =${PARALLEL}")
    message(STATUS "MSVC_PATH               =${MSVC_PATH}")
    message(STATUS "CC_PATH                 =${CC_PATH}")
    message(STATUS "ORIGINAL_DIR            =${ORIGINAL_DIR}")

    message(STATUS "PROC_ALL                =${PROC_ALL}")
    message(STATUS "THEOLIZER_ROOT          =${THEOLIZER_ROOT}")
    message(STATUS "CI_SERVICE              =${CI_SERVICE}")
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
    file(APPEND ${LOG_FILE} "THEOLIZER_SOURCE       =${THEOLIZER_SOURCE}\n")
    file(APPEND ${LOG_FILE} "THEOLIZER_BINARY       =${THEOLIZER_BINARY}\n")
    file(APPEND ${LOG_FILE} "THEOLIZER_PREFIX       =${THEOLIZER_PREFIX}\n")
    file(APPEND ${LOG_FILE} "LIB_TYPE               =${LIB_TYPE}\n")
    file(APPEND ${LOG_FILE} "COMPILER               =${COMPILER}\n")
    file(APPEND ${LOG_FILE} "BIT_NUM                =${BIT_NUM}\n")
    file(APPEND ${LOG_FILE} "CONFIG_TYPE            =${CONFIG_TYPE}\n")
    file(APPEND ${LOG_FILE} "BUILD_DRIVER           =${BUILD_DRIVER}\n")
    file(APPEND ${LOG_FILE} "BUILD_DOCUMENT         =${BUILD_DOCUMENT}\n")
    file(APPEND ${LOG_FILE} "GENERATOR              =${GENERATOR}\n")
    file(APPEND ${LOG_FILE} "BUILD_DIR              =${BUILD_DIR}\n")
    file(APPEND ${LOG_FILE} "BOOST_ROOT             =${BOOST_ROOT}\n")
    file(APPEND ${LOG_FILE} "LLVM_ROOT              =${LLVM_ROOT}\n")
    file(APPEND ${LOG_FILE} "LLVM_VERSION           =${LLVM_VERSION}\n")
    file(APPEND ${LOG_FILE} "REQUIRE_CMAKE_VERSION  =${REQUIRE_CMAKE_VERSION}")
    file(APPEND ${LOG_FILE} "MAKE                   =${MAKE}\n")
    file(APPEND ${LOG_FILE} "PARALLEL               =${PARALLEL}\n")
    file(APPEND ${LOG_FILE} "MSVC_PATH              =${MSVC_PATH}\n")
    file(APPEND ${LOG_FILE} "CC_PATH                =${CC_PATH}\n")
    file(APPEND ${LOG_FILE} "ORIGINAL_DIR           =${ORIGINAL_DIR}\n")

    file(APPEND ${LOG_FILE} "PROC_ALL               =${PROC_ALL}\n")
    file(APPEND ${LOG_FILE} "THEOLIZER_ROOT         =${THEOLIZER_ROOT}\n")
    file(APPEND ${LOG_FILE} "CI_SERVICE             =${CI_SERVICE}\n")
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

function(search LOG_FILE IS_PASSED_NUMBER REGEX_STRING)

    file(READ ${LOG_FILE} OUTPUT_STRING)
    string(REGEX MATCHALL "${REGEX_STRING}" FOUND_LIST  ${OUTPUT_STRING})

    set(IS_FAIL FALSE)
    foreach(LINE IN LISTS FOUND_LIST)
        message(STATUS "${LINE}")
        if (NOT "${SUMMARY}" STREQUAL "")
            file(APPEND ${SUMMARY} "${LINE}\n")
        endif()
        if(IS_PASSED_NUMBER)
            if(NOT "${LINE}" MATCHES "100%")
                set(IS_FAIL TRUE)
            endif()
        endif()
    endforeach()

    if (NOT "${SUMMARY}" STREQUAL "")
        if(IS_PASSED_NUMBER)
            if (IS_FAIL)
                file(APPEND ${SUMMARY} "Test failed!!\n")
                set(IS_PASS FALSE PARENT_SCOPE)
            endif()
        else()
            if (NOT "${FOUND_LIST}" STREQUAL "")
                file(APPEND ${SUMMARY} "Test failed!!\n")
                set(IS_PASS FALSE PARENT_SCOPE)
            endif()
        endif()
    endif()

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

    set(IS_PASS TRUE)
    string(REGEX REPLACE
            "warning: jobserver unavailable"
            "WARNING: jobserver unavailable"
            OUTPUT_LOG ${OUTPUT_LOG})

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
            file(APPEND ${SUMMARY} "Test failed!! : ${RETURN_CODE} ${NOW_TIME}\n")
            set(IS_PASS FALSE)
        endif()
    endif()

    search(${LOG_FILE} FALSE "[^\n0-9A-Za-z_]Configuring incomplete,")
    search(${LOG_FILE} FALSE "[^\n0-9A-Za-z_]warning[^0-9A-Za-z_]............")
    search(${LOG_FILE} FALSE "[^\n0-9A-Za-z_]error[^0-9A-Za-z_(]............")
    search(${LOG_FILE} FALSE "[^\n0-9A-Za-z_]undefined reference[^0-9A-Za-z_]")
    search(${LOG_FILE} TRUE  "[0-9]+% tests passed[^0-9A-Za-z_][^\n]*")

    # テスト結果の数を確認
    if(NOT "${ARGN}" STREQUAL "")
        set(NOW_PASS_LIST ${ARGN})
        file(STRINGS  ${LOG_FILE} FOUND_LIST REGEX "% tests passed")

        set(INDEX 0)
        foreach(LINE IN LISTS FOUND_LIST)
            list(GET NOW_PASS_LIST ${INDEX} PASS_NUMBER)
            math(EXPR INDEX "${INDEX} + 1")

##          message(STATUS "${LINE}")
            string(REGEX MATCH "out of [0-9]+" NUMBER "${LINE}")
            string(SUBSTRING "${NUMBER}" 7 10 NUMBER)
            if (NOT ${NUMBER} EQUAL ${PASS_NUMBER})
                message(STATUS "Test failed!! : ${LINE} -> expected ${PASS_NUMBER}")
                if (NOT "${SUMMARY}" STREQUAL "")
                    file(APPEND ${SUMMARY} "Test failed!! : ${LINE} -> expected ${PASS_NUMBER}\n")
                    set(IS_PASS FALSE)
                endif()
            endif()
        endforeach()
    endif()

    if(BREAK)
        if(NOT ${RETURN_CODE} EQUAL 0)
            message(SEND_ERROR "")
        endif()
    endif()

endmacro()

#       ---<<< ビルド実行 >>>---

macro(build TARGET CONFIG_TYPE WORKING_DIR)

message(STATUS "TARGET=${TARGET}")
message(STATUS "CONFIG_TYPE=${CONFIG_TYPE}")
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
            ENCODING AUTO
        )
    elseif("${CI_SERVICE}" STREQUAL "")
        # MinGWの時、CMake 3.8.0以降なら、UTF-8へ統一するため
        if ("${CMAKE_VERSION}" VERSION_LESS "3.8.0")
            execute_process(
                COMMAND ${MAKE} ${TARGET} ${PARALLEL}
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
                ENCODING AUTO
            )
        endif()
    else()
        execute_process(
            COMMAND bash -c "${MAKE} ${TARGET} 2>&1 | tee temp.txt" ${PARALLEL}
            WORKING_DIRECTORY "${WORKING_DIR}"
            RESULT_VARIABLE RETURN_CODE
        )
        file(READ temp.txt TEMP)
        set(OUTPUT_LOG "${OUTPUT_LOG}${TEMP}")
    endif()

    # CMake 3.8.0以降のWindows版のCTestはUTF-8で出力するのENCODING指定不要
    if ("${RETUEN_CODE}" STREQUAL "")
        set(LABEL "")
        if    ("${TARGET}" STREQUAL "ShortTest")
            set(LABEL "Test.*S")
        elseif("${TARGET}" STREQUAL "LongTest")
            set(LABEL "Test.*L.*")
        elseif("${TARGET}" STREQUAL "FullTest")
            set(LABEL "TestF.*")
        elseif("${TARGET}" STREQUAL "FullTest1")
            set(LABEL "TestF[1L].*")
        elseif("${TARGET}" STREQUAL "FullTest2")
            set(LABEL "TestF[2L].*")
        elseif("${TARGET}" STREQUAL "FullTest3")
            set(LABEL "TestF[3L].*")
        endif()
message(STATUS "LABEL=${LABEL}")
        if (NOT "${LABEL}" STREQUAL "")
            execute_process(
                COMMAND "ctest" "-V" "-C" ${CONFIG_TYPE} "-L" "${LABEL}"
                WORKING_DIRECTORY "${WORKING_DIR}"
                OUTPUT_VARIABLE OUTPUT_LOG_TMP
                ERROR_VARIABLE  OUTPUT_LOG_TMP
                RESULT_VARIABLE RETURN_CODE
            )
            set(OUTPUT_LOG ${OUTPUT_LOG}${OUTPUT_LOG_TMP})
        endif()
    endif()

#message(STATUS "build(${TARGET} ${CONFIG_TYPE} ${WORKING_DIR}) RETURN_CODE=${RETURN_CODE}")
#message(STATUS "OUTPUT_LOG=${OUTPUT_LOG}")

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
        if("${CI_SERVICE}" STREQUAL "")
            build(BuildTest Debug "${CMAKE_SOURCE_DIR}/${TARGET}")
            set(TEMP_LOG2 "${TEMP_LOG2}${OUTPUT_LOG}")
        endif()
    else()
        build(BuildTest ${CONFIG_TYPE} "${CMAKE_SOURCE_DIR}/${TARGET}")
        set(TEMP_LOG2 "${TEMP_LOG2}${OUTPUT_LOG}")
    endif()

endmacro()

# 最後のテスト処理
macro(last_test)

    set(SUDO "")
    if((NOT WIN32) AND ("${CI_SERVICE}" STREQUAL ""))
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
        message(AUTHOR_WARNING "error: last_test(${RETURN_CODE})")
        set(OUTPUT_LOG "${OUTPUT_LOG}\nerror: last_test(${RETURN_CODE})\n")
    else()
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
    endif()

endmacro()

#       ---<<< PASS_LISTから１つ取り出してPASS_CHECK_LISTへ追加する >>>---

macro(get_pass_list)

    if(NOT "${PASS_LIST}" STREQUAL "")
        list(GET PASS_LIST ${INDEX} PASS_NUMBER)
        set(PASS_CHECK_LIST ${PASS_CHECK_LIST} ${PASS_NUMBER})
    else()
        set(PASS_CHECK_LIST "")
    endif()

endmacro()

#       ---<<< PASSならsources_hash.txt生成 >>>---

function(check_pass)

    if(IS_PASS)
        execute_process(
            COMMAND ${CMAKE_COMMAND}
                --build . 
                --config Release
                --target makeHashTxt
            OUTPUT_QUIET
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        )
    endif()

endfunction()

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
                "-DBUILD_DRIVER=${BUILD_DRIVER}"
                "-DLLVM_ROOT=${LLVM_ROOT}"
                "-DLLVM_VERSION=${LLVM_VERSION}"
                "-DCMAKE_VERSION=${REQUIRE_CMAKE_VERSION}"
                "-DBUILD_DOCUMENT=${BUILD_DOCUMENT}"
                "-DCI_SERVICE=${CI_SERVICE}"
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
                "-DCI_SERVICE=${CI_SERVICE}"
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
    check_pass()

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
            build(FullTest$ENV{PARALLEL_TEST} Release .)
            set(TEMP_LOG "${TEMP_LOG}${OUTPUT_LOG}")
        endif()
        get_pass_list()
        math(EXPR INDEX "${INDEX} + 1")

        # 非Travis-CI時のみインストールする
        if((${RETURN_CODE} EQUAL 0) AND ("${CI_SERVICE}" STREQUAL ""))
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
            if("${CI_SERVICE}" STREQUAL "")
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
            math(EXPR INDEX "${INDEX} + 1")
        endif()

        # ライセンス・ファイルのコピー
        file(GLOB_RECURSE LICENSE_LIST "${ORIGINAL_DIR}/other_licenses/*")
        file(COPY "${THEOLIZER_SOURCE}/../LICENSE.TXT" DESTINATION "${THEOLIZER_ROOT}")
        file(COPY ${LICENSE_LIST} DESTINATION "${THEOLIZER_ROOT}/other_licenses")

    else()

        math(EXPR INDEX "${INDEX} + 1")
        if(${GENERATOR} MATCHES "Visual Studio")
            math(EXPR INDEX "${INDEX} + 1")
        endif()

    endif()

    # インストールされたTheolizerを用いたテスト
    if(NOT "${PROC_ALL}" STREQUAL "middle_all")
        if(${RETURN_CODE} EQUAL 0)
            relay("  Last Test   ...")
            last_test()
            set(TEMP_LOG "${TEMP_LOG}${OUTPUT_LOG}")
            get_pass_list()
            math(EXPR INDEX "${INDEX} + 1")

            if(${GENERATOR} MATCHES "Visual Studio")
                if("${CI_SERVICE}" STREQUAL "")
                    get_pass_list()
                endif()
                math(EXPR INDEX "${INDEX} + 1")
            endif()
        endif()
    endif()

    # 終了
    set(OUTPUT_LOG "${TEMP_LOG}")
    end("z3_full_test-${PROC_ALL}.log" FALSE ${PASS_CHECK_LIST})
    check_pass()

#       ---<<< build document by Doxygen >>>---

else()

    start("Build ${PROC}      ...")
    build(${PROC} Release .)
    end("z4_${PROC}.log" FALSE)

endif()

