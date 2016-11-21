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

#-----------------------------------------------------------------------------
#       サブ・ファンクション等
#-----------------------------------------------------------------------------

#       ---<<< パラメータをログへ出力 >>>---

function(parameter_log LOG_FILE)

    file(WRITE  ${LOG_FILE} "--- Parameters ---\n")
    file(APPEND ${LOG_FILE} "LLVM_DOWNLOAD   =${LLVM_DOWNLOAD}\n")
    file(APPEND ${LOG_FILE} "LLVM_SOURCE     =${LLVM_SOURCE}\n")
    file(APPEND ${LOG_FILE} "LLVM_PREFIX     =${LLVM_PREFIX}\n")
    file(APPEND ${LOG_FILE} "COMPLIER        =${COMPLIER}\n")
    file(APPEND ${LOG_FILE} "BIT_NUM         =${BIT_NUM}\n")
    file(APPEND ${LOG_FILE} "CONFIG_TYPE     =${CONFIG_TYPE}\n")
    file(APPEND ${LOG_FILE} "LLVM_INSTALL    =${LLVM_INSTALL}\n")
    file(APPEND ${LOG_FILE} "GENERATOR       =${GENERATOR}\n")
    file(APPEND ${LOG_FILE} "BUILD_DIR       =${BUILD_DIR}\n")
    file(APPEND ${LOG_FILE} "MAKE            =${MAKE}\n")
    file(APPEND ${LOG_FILE} "PARALLEL        =${PARALLEL}\n")
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

#       ---<<< ビルド実行 >>>---

macro(build TARGET CONFIG_TYPE WORKING_DIR)

    if(NOT "${TARGET}" STREQUAL "")
        set(TARGET_PARAM "--target;${TARGET}")
    endif()

    if(${GENERATOR} MATCHES "Visual Studio")
        execute_process(
            COMMAND ${CMAKE_COMMAND}
                --build .
                --config "${CONFIG_TYPE}"
                ${TARGET_PARAM}
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

absolute_path("${LLVM_SOURCE}" LLVM_SOURCE)
absolute_path("${LLVM_PREFIX}" LLVM_PREFIX)
message(STATUS "LLVM_SOURCE=${LLVM_SOURCE}")
message(STATUS "LLVM_PREFIX=${LLVM_PREFIX}")

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

macro(setup_build_folder COMPLIER BIT_NUM CONFIG_TYPE)

    set(LLVM_INSTALL "${LLVM_PREFIX}${COMPLIER}x${BIT_NUM}")
    if(NOT "${COMPLIER}" STREQUAL "msvc2015")
        if(NOT "${CONFIG_TYPE}" STREQUAL "")
            set(LLVM_INSTALL "${LLVM_INSTALL}-${CONFIG_TYPE}")
        endif()
    endif()

if(FALSE)
    message(STATUS "LLVM_SOURCE =${LLVM_SOURCE}")
    message(STATUS "LLVM_PREFIX =${LLVM_PREFIX}")
    message(STATUS "COMPLIER    =${COMPLIER}")
    message(STATUS "BIT_NUM     =${BIT_NUM}")
    message(STATUS "CONFIG_TYPE =${CONFIG_TYPE}")
    message(STATUS "LLVM_INSTALL=${LLVM_INSTALL}")
endif()

    # パラメータ・チェック
    if("${BIT_NUM}" STREQUAL "64")
    elseif("${BIT_NUM}" STREQUAL "32")
    else()
        message(SEND_ERROR "unknown number of bit ${BIT_NUM}")
    endif()

    if("${CONFIG_TYPE}" STREQUAL "")
    elseif("${CONFIG_TYPE}" STREQUAL "Release")
    elseif("${CONFIG_TYPE}" STREQUAL "Debug")
    else()
        message(SEND_ERROR "unknown config ${CONFIG_TYPE}")
    endif()

    # ジェネレータ生成
    if("${COMPLIER}" STREQUAL "msvc2015")
        set(GENERATOR "Visual Studio 14")
        if("${BIT_NUM}" STREQUAL "64")
            set(GENERATOR "${GENERATOR} Win64")
        endif()
        if(NOT "${CONFIG_TYPE}" STREQUAL "")
            message(SEND_ERROR "${CONFIG_TYPE} have to be \"\" for msvc")
        endif()
        set(MSVC_PATH "$ENV{VSSDK140Install}../VC/bin")
        STRING(REPLACE "\\" "/" MSVC_PATH "${MSVC_PATH}")
        set(CC_PATH "")
    elseif("${COMPLIER}" MATCHES "mingw")
        set(GENERATOR "MinGW Makefiles")
        if("${BIT_NUM}" STREQUAL "64")
            set(CC_PATH "${CC64}")
        else()
            set(CC_PATH "${CC32}")
        endif()
        set(MSVC_PATH "")
    elseif("${COMPLIER}" MATCHES "gcc")
        set(GENERATOR "Unix Makefiles")
        if("${BIT_NUM}" STREQUAL "64")
            set(CC_PATH "${CC64}")
        else()
            set(CC_PATH "${CC32}")
        endif()
        set(MSVC_PATH "")
    else()
        message(SEND_ERROR "unknown compiler ${COMPLIER}")
    endif()
    message(STATUS "GENERATOR   =${GENERATOR}")

    # ビルド・フォルダ・パス
    set(BUILD_DIR "${COMPLIER}x${BIT_NUM}")
    if(NOT "${CONFIG_TYPE}" STREQUAL "")
        set(BUILD_DIR "${BUILD_DIR}-${CONFIG_TYPE}")
    endif()
    message(STATUS "BUILD_DIR   =${BUILD_DIR}")

    # makeツール
    if ("${MAKE}" STREQUAL "")
        set(MAKE "make")
    endif()
    message(STATUS "MAKE        =${MAKE}")

    # フォルダ生成と必要ファイルのコピー
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E make_directory   "${BUILD_DIR}"
    )

endmacro()

#-----------------------------------------------------------------------------
#       ダウンロードと解凍
#-----------------------------------------------------------------------------

function(download_extract FILE_NAME EXTRACT_NAME EXTRACT_PATH)
    message(STATUS "FILE_NAME   =${FILE_NAME}")
    message(STATUS "EXTRACT_NAME=${EXTRACT_NAME}")
    message(STATUS "EXTRACT_PATH=${EXTRACT_PATH}")
    message(STATUS "ROOT        =${ROOT}")
    file(DOWNLOAD ${URL}/${FILE_NAME}.tar.xz ${ROOT}/${FILE_NAME}.tar.xz SHOW_PROGRESS)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xvf "${ROOT}/${FILE_NAME}.tar.xz"
        WORKING_DIRECTORY "${EXTRACT_PATH}"
    )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E rename "${FILE_NAME}" "${EXTRACT_NAME}"
        WORKING_DIRECTORY "${EXTRACT_PATH}"
    )
endfunction()

function(llvm_setup VERSION)
    if(    "${VERSION}" STREQUAL "3.9.0")
        set(URL "http://llvm.org/releases/3.9.0")
    elseif("${VERSION}" STREQUAL "3.8.1")
        set(URL "http://llvm.org/releases/3.8.1")
    else()
        message(SEND_ERROR "Not supported LLVM version. ${VERSION}")
    endif()

    get_filename_component(ROOT     "${LLVM_SOURCE}" DIRECTORY)
    get_filename_component(DIR_NAME "${LLVM_SOURCE}" NAME      )
    download_extract("llvm-${VERSION}.src"              ${DIR_NAME} "${ROOT}"                         )
    download_extract("cfe-${VERSION}.src"               clang       "${LLVM_SOURCE}/tools"            )
    download_extract("clang-tools-extra-${VERSION}.src" extra       "${LLVM_SOURCE}/tools/clang/tools")
endfunction()

# llvmをダウンロードする
if ((NOT "${LLVM_DOWNLOAD}" STREQUAL "") AND (NOT EXISTS ${LLVM_SOURCE}))
    llvm_setup(${LLVM_DOWNLOAD})
endif()

#-----------------------------------------------------------------------------
#       ビルド処理
#-----------------------------------------------------------------------------

function(build_process COMPLIER BIT_NUM CONFIG_TYPE)

    setup_build_folder("${COMPLIER}" "${BIT_NUM}" "${CONFIG_TYPE}")

    if (NOT "${MSVC_PATH}" STREQUAL "")
        set(ENV{PATH} "${MSVC_PATH};${ENV_PATH}")
    endif()

    if (NOT "${CC_PATH}" STREQUAL "")
        set(ENV{PATH} "${CC_PATH};${ENV_PATH}")
    endif()

    start("Configuring   ...")
    if("${COMPLIER}" STREQUAL "msvc2015")
        execute_process(
            COMMAND ${CMAKE_COMMAND}
                -G "${GENERATOR}"
                "${LLVM_SOURCE}"
                "-DCMAKE_INSTALL_PREFIX=${LLVM_INSTALL}"
                "-DCMAKE_DEBUG_POSTFIX=d"
            WORKING_DIRECTORY "${BUILD_DIR}"
            OUTPUT_VARIABLE OUTPUT_LOG
            ERROR_VARIABLE  OUTPUT_LOG
            RESULT_VARIABLE RETURN_CODE
        )
    elseif("${COMPLIER}" MATCHES "mingw")
        execute_process(
            COMMAND ${CMAKE_COMMAND}
                -G "${GENERATOR}"
                "${LLVM_SOURCE}"
                "-DCMAKE_INSTALL_PREFIX=${LLVM_INSTALL}"
                "-DCMAKE_BUILD_TYPE:STRING=Release"
                "-DCMAKE_C_FLAGS:STRING=-static -static-libgcc -static-libstdc++"
                "-DCMAKE_CXX_FLAGS:STRING=-static -static-libgcc -static-libstdc++"
            WORKING_DIRECTORY "${BUILD_DIR}"
            OUTPUT_VARIABLE OUTPUT_LOG
            ERROR_VARIABLE  OUTPUT_LOG
            RESULT_VARIABLE RETURN_CODE
        )
    else()
        execute_process(
            COMMAND ${CMAKE_COMMAND}
                -G "${GENERATOR}"
                "${LLVM_SOURCE}"
                "-DCMAKE_INSTALL_PREFIX=${LLVM_INSTALL}"
                "-DCMAKE_BUILD_TYPE:STRING=Release"
            WORKING_DIRECTORY "${BUILD_DIR}"
            OUTPUT_VARIABLE OUTPUT_LOG
            ERROR_VARIABLE  OUTPUT_LOG
            RESULT_VARIABLE RETURN_CODE
        )
    endif()
    end("${BUILD_DIR}/zz0_config.log" TRUE)

    if(("${CONFIG_TYPE}" STREQUAL "") OR ("${CONFIG_TYPE}" STREQUAL "Release"))
        start("Build Release ...")
        build(""      Release "${BUILD_DIR}")
        relay("  Installing  ...")
        build(install Release "${BUILD_DIR}")
        end("${BUILD_DIR}/zz1_build_release.log" FALSE)
    endif()

    if(("${CONFIG_TYPE}" STREQUAL "") OR ("${CONFIG_TYPE}" STREQUAL "Debug"))
        start("Build Debug   ...")
        build(""      Debug "${BUILD_DIR}")
        relay("  Installing  ...")
        build(install Debug "${BUILD_DIR}")
        end("${BUILD_DIR}/zz2_build_debug.log" FALSE)
    endif()

endfunction()
