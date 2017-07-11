#[[###########################################################################
        boostビルド用補助CMakeスクリプト

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

#-----------------------------------------------------------------------------
#       サブ・ファンクション等
#-----------------------------------------------------------------------------

#       ---<<< パラメータをログへ出力 >>>---

function(parameter_log LOG_FILE)

    file(WRITE  ${LOG_FILE} "--- Parameters ---\n")
    file(APPEND ${LOG_FILE} "BOOST_VERSION   =${BOOST_VERSION}\n")
    file(APPEND ${LOG_FILE} "BOOST_PATH      =${BOOST_PATH}\n")
    file(APPEND ${LOG_FILE} "fPIC            =${fPIC}\n")
    file(APPEND ${LOG_FILE} "COMPILER        =${COMPILER}\n")
    file(APPEND ${LOG_FILE} "BIT_NUM         =${BIT_NUM}\n")
    file(APPEND ${LOG_FILE} "CONFIG_TYPE     =${CONFIG_TYPE}\n")
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
    search(${LOG_FILE} " warning")
    search(${LOG_FILE} "% tests passed, ")

    if(BREAK)
        if(NOT ${RETURN_CODE} EQUAL 0)
            message(SEND_ERROR "")
        endif()
    endif()

endmacro()

#-----------------------------------------------------------------------------
#       ダウンロードと解凍
#-----------------------------------------------------------------------------

function(boost_setup)

    set(URL "https://sourceforge.net/projects/boost/files/boost/${BOOST_VERSION}")
    message(STATUS "URL                     =${URL}")

    string(REPLACE "." "_" VERSION2 ${BOOST_VERSION})
    set(FILE_NAME "boost_${VERSION2}")
    message(STATUS "FILE_NAME               =${FILE_NAME}")

    # ダウンロード
    if(WIN32)
        set(EXT ".7z")
    else()
        set(EXT ".tar.bz2")
    endif()
    if(NOT EXISTS ${BOOST_PATH}/${FILE_NAME}${EXT})
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E make_directory "${BOOST_PATH}"
        )
        if(WIN32)
            file(DOWNLOAD ${URL}/${FILE_NAME}${EXT} ${BOOST_PATH}/${FILE_NAME}${EXT} SHOW_PROGRESS)
        else()
            execute_process(
                COMMAND wget ${URL}/${FILE_NAME}${EXT}
                WORKING_DIRECTORY "${BOOST_PATH}"
            )
        endif()
    endif()

    # 解凍
    if (NOT EXISTS ${BOOST_SOURCE})
        message(STATUS "extracting...")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar xvf "${BOOST_PATH}/${FILE_NAME}${EXT}"
            WORKING_DIRECTORY "${BOOST_PATH}"
            OUTPUT_FILE expand.log
        )
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E rename "${FILE_NAME}" "source"
            WORKING_DIRECTORY "${BOOST_PATH}"
        )
        message(STATUS "extracted.")
    endif()

endfunction()

#-----------------------------------------------------------------------------
#       ビルド
#-----------------------------------------------------------------------------

function(build_boost)

#       ---<<< ビルド準備（パラメータ設定とビルド・フォルダとサブ・ファイル用意） >>>---

    # ツールセット生成
    if("${COMPILER}" STREQUAL "msvc2017")
        set(TOOLSET "msvc-14.1")
        execute_process(
            COMMAND vswhere/vswhere.exe -version 15.0 -property installationPath
            OUTPUT_VARIABLE MSVC_PATH)
        string(REPLACE "\n" "" MSVC_PATH "${MSVC_PATH}")
        set(MSVC_PATH "${MSVC_PATH}/VC/Tools/MSVC/14.10.25017/bin/HostX86/x86")
        string(REPLACE "\\" "/" MSVC_PATH "${MSVC_PATH}")
        set(CC_PATH "")
    elseif("${COMPILER}" STREQUAL "msvc2015")
        set(TOOLSET "msvc-14.0")
        set(MSVC_PATH "$ENV{VSSDK140Install}../VC/bin")
        string(REPLACE "\\" "/" MSVC_PATH "${MSVC_PATH}")
        set(CC_PATH "")
    elseif("${COMPILER}" MATCHES "mingw")
        set(TOOLSET "gcc")
        if("${BIT_NUM}" STREQUAL "64")
            set(CC_PATH "${CC64}")
        else()
            set(CC_PATH "${CC32}")
        endif()
        set(MSVC_PATH "")
    elseif("${COMPILER}" MATCHES "gcc")
        set(TOOLSET "gcc")
        if("${BIT_NUM}" STREQUAL "64")
            set(CC_PATH "${CC64}")
        else()
            set(CC_PATH "${CC32}")
        endif()
        set(MSVC_PATH "")
    else()
        message(SEND_ERROR "unknown compiler ${COMPILER}")
    endif()
    message(STATUS "TOOLSET                 =${TOOLSET}")
    message(STATUS "MSVC_PATH               =${MSVC_PATH}")
    message(STATUS "CC_PATH                 =${CC_PATH}")

    # makeツール
    if ("${MAKE}" STREQUAL "")
        set(MAKE "make")
    endif()
    message(STATUS "MAKE                    =${MAKE}")

    # PATH設定
    if (NOT "${MSVC_PATH}" STREQUAL "")
        set(ENV{PATH} "${MSVC_PATH};$ENV{PATH}")
    endif()

    if (NOT "${CC_PATH}" STREQUAL "")
        set(ENV{PATH} "${CC_PATH};$ENV{PATH}")
    endif()

#       ---<<< ビルド >>>---

    if(WIN32)
        if(NOT EXISTS "${BOOST_SOURCE}/b2.exe")
            start("bootstrap     ...")
            execute_process(
                COMMAND bootstrap.bat
                OUTPUT_VARIABLE OUTPUT_LOG
                ERROR_VARIABLE  OUTPUT_LOG
                RESULT_VARIABLE RETURN_CODE
                WORKING_DIRECTORY "${BOOST_SOURCE}"
            )
            end("${BOOST_BUILD}/bootstrap.log" TRUE)
        endif()
    else()
        if(NOT EXISTS "${BOOST_SOURCE}/b2")
            start("bootstrap     ...")
            execute_process(
                COMMAND ./bootstrap.sh
                OUTPUT_VARIABLE OUTPUT_LOG
                ERROR_VARIABLE  OUTPUT_LOG
                RESULT_VARIABLE RETURN_CODE
                WORKING_DIRECTORY "${BOOST_SOURCE}"
            )
            end("${BOOST_BUILD}/bootstrap.log" TRUE)
        endif()
    endif()

    start("Build         ...")
    if(fPIC)
        execute_process(
            COMMAND ./b2
                "--prefix=${BOOST_INSTALL}"
                "--build-dir=${BOOST_BUILD}"
                "toolset=${TOOLSET}"
                "address-model=${BIT_NUM}"
                --with-filesystem --with-system
                install
                variant=release,debug
                link=static runtime-link=shared
                threading=multi
                -a
                -j 8
                "cxxflags=-fPIC -fvisibility=hidden"
                "cflags=-fPIC -fvisibility=hidden"
            OUTPUT_VARIABLE OUTPUT_LOG
            ERROR_VARIABLE  OUTPUT_LOG
            RESULT_VARIABLE RETURN_CODE
            WORKING_DIRECTORY "${BOOST_SOURCE}"
        )
    else()
        execute_process(
            COMMAND ./b2
                "--prefix=${BOOST_INSTALL}"
                "--build-dir=${BOOST_BUILD}"
                "toolset=${TOOLSET}"
                "address-model=${BIT_NUM}"
                --with-filesystem --with-system
                install
                variant=release,debug
                link=static runtime-link=shared
                threading=multi
                -a
                -j 8
            OUTPUT_VARIABLE OUTPUT_LOG
            ERROR_VARIABLE  OUTPUT_LOG
            RESULT_VARIABLE RETURN_CODE
            WORKING_DIRECTORY "${BOOST_SOURCE}"
        )
    endif()
    end("${BOOST_BUILD}/zz0_boost.log" FALSE)

endfunction()

#-----------------------------------------------------------------------------
#       boostのソースを準備してビルドする
#-----------------------------------------------------------------------------

if("${${BOOST_VARIABLE_NAME}}" STREQUAL "")

    # boostのソース・パス
    set(BOOST_SOURCE  "${BOOST_PATH}/source")
    message(STATUS "BOOST_SOURCE            =${BOOST_SOURCE}")

    # boostのダウンロードと解凍
    boost_setup()

    # boostをビルドする
    if((NOT EXISTS "${BOOST_BUILD}") AND (NOT "${CONFIG_TYPE}" STREQUAL "Debug"))
        build_boost()
    endif()
 
endif()
