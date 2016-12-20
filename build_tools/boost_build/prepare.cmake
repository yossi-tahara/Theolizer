#[[###########################################################################
        ビルド用補助CMakeスクリプト

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

#-----------------------------------------------------------------------------
#       サブ・ファンクション等
#-----------------------------------------------------------------------------

#       ---<<< パラメータをログへ出力 >>>---

function(parameter_log LOG_FILE)

    file(WRITE  ${LOG_FILE} "--- Parameters ---\n")
    file(APPEND ${LOG_FILE} "BOOST_DOWNLOAD  =${BOOST_DOWNLOAD}\n")
    file(APPEND ${LOG_FILE} "BOOST_SOURCE    =${BOOST_SOURCE}\n")
    file(APPEND ${LOG_FILE} "BOOST_PREFIX    =${BOOST_PREFIX}\n")
    file(APPEND ${LOG_FILE} "COMPILER        =${COMPILER}\n")
    file(APPEND ${LOG_FILE} "BIT_NUM         =${BIT_NUM}\n")
    file(APPEND ${LOG_FILE} "CONFIG_TYPE     =${CONFIG_TYPE}\n")
    file(APPEND ${LOG_FILE} "fPIC            =${fPIC}\n")
    file(APPEND ${LOG_FILE} "BOOST_INSTALL   =${BOOST_INSTALL}\n")
    file(APPEND ${LOG_FILE} "TOOLSET         =${TOOLSET}\n")
    file(APPEND ${LOG_FILE} "BUILD_DIR       =${BUILD_DIR}\n")
    file(APPEND ${LOG_FILE} "MAKE            =${MAKE}\n")
    file(APPEND ${LOG_FILE} "MSVC_PATH       =${MSVC_PATH}\n")
    file(APPEND ${LOG_FILE} "CC_PATH         =${CC_PATH}\n")
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
#       ビルド周りのフォルダを絶対パスへ変換する
#-----------------------------------------------------------------------------

function(absolute_path PATH_STRING RESULT)
    string(LENGTH ${PATH_STRING} LEN)
    math(EXPR BEGIN "${LEN}-1")
    string(SUBSTRING ${PATH_STRING} ${BEGIN} 1 LAST_CHAR)
    if(NOT "${LAST_CHAR}" STREQUAL "/")
        set(LAST_CHAR "")
    endif()
    get_filename_component(PATH_STRING "${PATH_STRING}" ABSOLUTE)
    set(${RESULT} "${PATH_STRING}${LAST_CHAR}" PARENT_SCOPE)
endfunction()

absolute_path("${BOOST_SOURCE}" BOOST_SOURCE)
absolute_path("${BOOST_PREFIX}" BOOST_PREFIX)
message(STATUS "BOOST_SOURCE=${BOOST_SOURCE}")
message(STATUS "BOOST_PREFIX=${BOOST_PREFIX}")

# 環境変数のPATHを確保しておく
set(ENV_PATH "$ENV{PATH}")

#-----------------------------------------------------------------------------
#       結果概要まとめ用
#-----------------------------------------------------------------------------

function(output_title MESSAGE_STRING)
    message(STATUS "${MESSAGE_STRING}")
    file(APPEND ${SUMMARY} "${MESSAGE_STRING}\n")
endfunction()

function(output_summary)
    message(STATUS "\n\n########## Summary ##########")
    file(READ ${SUMMARY} OUTPUT_STRING)
    message(STATUS ${OUTPUT_STRING})
endfunction()

#-----------------------------------------------------------------------------
#       ビルド準備（パラメータ設定とビルド・フォルダとサブ・ファイル用意）
#-----------------------------------------------------------------------------

macro(setup_build_folder COMPILER BIT_NUM fPIC)

    set(BOOST_INSTALL "${BOOST_PREFIX}${BIT_NUM}")
    if("${fPIC}" STREQUAL "TRUE")
        set(BOOST_INSTALL "${BOOST_INSTALL}-fPIC")
    endif()

if(FALSE)
    message(STATUS "BOOST_PREFIX =${BOOST_PREFIX}")
    message(STATUS "COMPILER     =${COMPILER}")
    message(STATUS "BIT_NUM      =${BIT_NUM}")
    message(STATUS "fPIC         =${fPIC}")
    message(STATUS "BOOST_INSTALL=${BOOST_INSTALL}")
endif()

    # パラメータ・チェック
    if("${BIT_NUM}" STREQUAL "64")
    elseif("${BIT_NUM}" STREQUAL "32")
    else()
        message(SEND_ERROR "unknown number of bit ${BIT_NUM}")
    endif()

    # ツールセット生成
    if("${COMPILER}" STREQUAL "msvc2015")
        set(TOOLSET "msvc-14.0")
        set(MSVC_PATH "$ENV{VSSDK140Install}../VC/bin")
        STRING(REPLACE "\\" "/" MSVC_PATH "${MSVC_PATH}")
        set(CC_PATH "")
    elseif("${COMPILER}" STREQUAL "mingw540")
        set(TOOLSET "gcc")
        if("${BIT_NUM}" STREQUAL "64")
            set(CC_PATH "${CC64}")
        else()
            set(CC_PATH "${CC32}")
        endif()
        set(MSVC_PATH "")
    elseif("${COMPILER}" STREQUAL "gcc540")
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
    message(STATUS "TOOLSET     =${TOOLSET}")

    # ビルド・フォルダ・パス
    set(BUILD_DIR "${CMAKE_SOURCE_DIR}/build/${COMPILER}x${BIT_NUM}")
    if("${fPIC}" STREQUAL "TRUE")
        set(BUILD_DIR "${BUILD_DIR}-fPIC")
    endif()
    message(STATUS "BUILD_DIR   =${BUILD_DIR}")

    # makeツール
    if ("${MAKE}" STREQUAL "")
        set(MAKE "make")
    endif()
    message(STATUS "MAKE        =${MAKE}")

endmacro()

#-----------------------------------------------------------------------------
#       ダウンロードと解凍
#-----------------------------------------------------------------------------

function(download_extract FILE_NAME EXTRACT_NAME EXTRACT_PATH)
    message(STATUS "FILE_NAME   =${FILE_NAME}")
    message(STATUS "EXTRACT_NAME=${EXTRACT_NAME}")
    message(STATUS "EXTRACT_PATH=${EXTRACT_PATH}")
    message(STATUS "ROOT        =${ROOT}")
    message(STATUS "URL         =${URL}")
    if(WIN32)
        set(EXT ".7z")
    else()
        set(EXT ".tar.bz2")
    endif()
    file(DOWNLOAD ${URL}/${FILE_NAME}${EXT}/download ${ROOT}/${FILE_NAME}${EXT} SHOW_PROGRESS)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xvf "${ROOT}/${FILE_NAME}${EXT}"
        WORKING_DIRECTORY "${EXTRACT_PATH}"
    )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E rename "${FILE_NAME}" "${EXTRACT_NAME}"
        WORKING_DIRECTORY "${EXTRACT_PATH}"
    )
endfunction()

function(boost_setup VERSION)
    string(REPLACE "." "_" VERSION2 ${VERSION})
    set(URL "https://sourceforge.net/projects/boost/files/boost/${VERSION}")

    get_filename_component(ROOT     "${BOOST_SOURCE}" DIRECTORY)
    get_filename_component(DIR_NAME "${BOOST_SOURCE}" NAME      )
    download_extract("boost_${VERSION2}" ${DIR_NAME} "${ROOT}")
endfunction()

# boostをダウンロードする
if ((NOT "${BOOST_DOWNLOAD}" STREQUAL "") AND (NOT EXISTS ${BOOST_SOURCE}))
    boost_setup(${BOOST_DOWNLOAD})
endif()

#-----------------------------------------------------------------------------
#       ビルド処理
#-----------------------------------------------------------------------------

function(build_process COMPILER BIT_NUM CONFIG_TYPE fPIC)

    if("${CONFIG_TYPE}" STREQUAL "")
        set(VARIANT "release,debug")
    elseif("${CONFIG_TYPE}" STREQUAL "Release")
        set(VARIANT "release")
    elseif("${CONFIG_TYPE}" STREQUAL "Debug")
        set(VARIANT "debug")
    else()
        message(SEND_ERROR "unknown config ${CONFIG_TYPE}")
    endif()

    setup_build_folder("${COMPILER}" "${BIT_NUM}" "${fPIC}")

    if (NOT "${MSVC_PATH}" STREQUAL "")
        set(ENV{PATH} "${MSVC_PATH};${ENV_PATH}")
    endif()

    if (NOT "${CC_PATH}" STREQUAL "")
        set(ENV{PATH} "${CC_PATH};${ENV_PATH}")
    endif()

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
            end("${BUILD_DIR}/bootstrap.log" TRUE)
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
            end("${BUILD_DIR}/bootstrap.log" TRUE)
        endif()
    endif()

    start("Build         ...")
    if(fPIC)
        execute_process(
            COMMAND ./b2
                "--prefix=${BOOST_INSTALL}"
                "--build-dir=${BUILD_DIR}"
                "toolset=${TOOLSET}"
                "address-model=${BIT_NUM}"
                --with-filesystem --with-system
                install
                variant=${VARIANT}
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
                "--build-dir=${BUILD_DIR}"
                "toolset=${TOOLSET}"
                "address-model=${BIT_NUM}"
                --with-filesystem --with-system
                install
                variant=${VARIANT}
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
    end("${BUILD_DIR}/zz1_build.log" FALSE)

endfunction()
