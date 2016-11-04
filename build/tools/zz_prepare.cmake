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

absolute_path("${THEOLIZER_SOURCE}" THEOLIZER_SOURCE)
absolute_path("${THEOLIZER_BINARY}" THEOLIZER_BINARY)
absolute_path("${THEOLIZER_PREFIX}" THEOLIZER_PREFIX)
message(STATUS "THEOLIZER_SOURCE=${THEOLIZER_SOURCE}")
message(STATUS "THEOLIZER_BINARY=${THEOLIZER_BINARY}")
message(STATUS "THEOLIZER_PREFIX=${THEOLIZER_PREFIX}")

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

macro(setup_build_folder COMPLIER BIT_NUM LIB_TYPE CONFIG_TYPE BUILD_DRIVER BUILD_DOCUMENT)

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
    message(STATUS "PROC_ALL        =${PROC_ALL}")
endif()

    # パラメータ・チェック
    if("${LIB_TYPE}" STREQUAL "StaticWithBoost")
    elseif("${LIB_TYPE}" STREQUAL "Static")
    elseif("${LIB_TYPE}" STREQUAL "Shared")
    else()
        message(SEND_ERROR "unknown library type ${LIB_TYPE}")
    endif()

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

    if("${BUILD_DRIVER}" STREQUAL "TRUE")
    elseif("${BUILD_DRIVER}" STREQUAL "FALSE")
    else()
        message(SEND_ERROR "illegal BUILD_DRIVER ${BUILD_DRIVER}")
    endif()

    if("${BUILD_DOCUMENT}" STREQUAL "TRUE")
    elseif("${BUILD_DOCUMENT}" STREQUAL "FALSE")
    else()
        message(SEND_ERROR "illegal BUILD_DOCUMENT ${BUILD_DOCUMENT}")
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
    elseif("${COMPLIER}" STREQUAL "mingw540")
        set(GENERATOR "MinGW Makefiles")
        if("${BIT_NUM}" STREQUAL "64")
            set(CC_PATH "${CC64}")
        else()
            set(CC_PATH "${CC32}")
        endif()
        set(MSVC_PATH "")
    elseif("${COMPLIER}" STREQUAL "gcc540")
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
    set(BUILD_DIR "${THEOLIZER_BINARY}${COMPLIER}x${BIT_NUM}-${LIB_TYPE}")
    if(NOT "${CONFIG_TYPE}" STREQUAL "")
        set(BUILD_DIR "${BUILD_DIR}-${CONFIG_TYPE}")
    endif()
    message(STATUS "BUILD_DIR   =${BUILD_DIR}")

    # boostのフォルダ・パス
    if("${BOOST_ROOT}" STREQUAL "")
        set(BOOST_ROOT "${BOOST_PREFIX}${BIT_NUM}")
        if(NOT WIN32)
            if("${LIB_TYPE}" STREQUAL "Shared")
                set(BOOST_ROOT "${BOOST_ROOT}-fPIC")
            endif()
        endif()
    endif()
    message(STATUS "BOOST_ROOT  =${BOOST_ROOT}")

    # llvmのフォルダ・パス
    if("${BUILD_DRIVER}" STREQUAL "TRUE")
        if("${LLVM_ROOT}" STREQUAL "")
            set(LLVM_ROOT "${LLVM}/${COMPLIER}x${BIT_NUM}")
            if(NOT "${CONFIG_TYPE}" STREQUAL "")
                set(LLVM_ROOT "${LLVM_ROOT}-${CONFIG_TYPE}")
            endif()
        endif()
    else()
        set(LLVM_ROOT "")
    endif()
    message(STATUS "LLVM_ROOT   =${LLVM_ROOT}")

    # makeツール
    if ("${MAKE}" STREQUAL "")
        set(MAKE "make")
    endif()
    message(STATUS "MAKE        =${MAKE}")

    # フォルダ生成と必要ファイルのコピー
    # full_all時のみビルド・フォルダを一度削除する
    if("${PROC_ALL}" STREQUAL "full_all")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${BUILD_DIR}"
        )
    endif()
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E make_directory   "${BUILD_DIR}"
    )

    configure_file(tools/zz_process.cmake ${BUILD_DIR}/zz_process.cmake @ONLY)

    if(WIN32)
        file(GLOB SCRIPT_LIST tools/*.bat)
        file(COPY ${SCRIPT_LIST} DESTINATION  ${BUILD_DIR})
    else()
        file(GLOB SCRIPT_LIST tools/*.sh)
        file(COPY ${SCRIPT_LIST} DESTINATION  ${BUILD_DIR})
    endif()

endmacro()

#-----------------------------------------------------------------------------
#       ビルド準備とビルド：ビルド・フォルダ構築、configure、full-test
#-----------------------------------------------------------------------------

function(build_process COMPLIER BIT_NUM LIB_TYPE CONFIG_TYPE BUILD_DRIVER BUILD_DOCUMENT)
    setup_build_folder("${COMPLIER}" "${BIT_NUM}" "${LIB_TYPE}" "${CONFIG_TYPE}" "${BUILD_DRIVER}" "${BUILD_DOCUMENT}")

    execute_process(
        COMMAND ${CMAKE_COMMAND} -DPROC=config -DPROC_ALL=${PROC_ALL} "-DSUMMARY=${SUMMARY}" -P zz_process.cmake
        WORKING_DIRECTORY "${BUILD_DIR}"
        RESULT_VARIABLE RETURN_CODE
    )
    if(NOT ${RETURN_CODE} EQUAL 0)
        message(SEND_ERROR "error: ${RETURN_CODE}")
    endif()

    if(NOT "${PROC_ALL}" STREQUAL "config_all")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -DPROC=full   -DPROC_ALL=${PROC_ALL} "-DSUMMARY=${SUMMARY}" "-DPASS_LIST=${ARGN}"
                -P zz_process.cmake
            WORKING_DIRECTORY "${BUILD_DIR}"
            RESULT_VARIABLE RETURN_CODE
        )
        if(NOT ${RETURN_CODE} EQUAL 0)
            message(SEND_ERROR "error: ${RETURN_CODE}")
        endif()

    endif()

endfunction()

#-----------------------------------------------------------------------------
#       ビルド処理中継
#-----------------------------------------------------------------------------

#       ---<<< MSVC用 >>>---

function(build_by_msvc COMPLIER BIT_NUM LIB_TYPE BUILD_DRIVER BUILD_DOCUMENT PASS_LIST)

    if(${BUILD_DRIVER})
        set(DRIVER "[Build Driver] ")
    endif()
    output_title("------ ${COMPLIER}x${BIT_NUM}-${LIB_TYPE} ${DRIVER}------")
    build_process("${COMPLIER}" "${BIT_NUM}" "${LIB_TYPE}" "" "${BUILD_DRIVER}" "${BUILD_DOCUMENT}" ${PASS_LIST})

endfunction()

#       ---<<< GCC/MinGW用 >>>---

function(build_by_gcc COMPLIER BIT_NUM LIB_TYPE BUILD_DRIVER BUILD_DOCUMENT RELEASE_LIST DEBUG_LIST)

    if(${BUILD_DRIVER})
        set(DRIVER "[Build Driver] ")
    endif()
    output_title("------ ${COMPLIER}x${BIT_NUM}-${LIB_TYPE}-Release ${DRIVER}------")
    build_process("${COMPLIER}" "${BIT_NUM}" "${LIB_TYPE}" "Release" "${BUILD_DRIVER}" "${BUILD_DOCUMENT}" ${RELEASE_LIST})

    output_title("------ ${COMPLIER}x${BIT_NUM}-${LIB_TYPE}-Debug ------")
    build_process("${COMPLIER}" "${BIT_NUM}" "${LIB_TYPE}" "Debug"   "FALSE" "FALSE" ${DEBUG_LIST})

endfunction()
