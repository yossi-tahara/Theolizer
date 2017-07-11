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

    string(REGEX MATCH "Test failed!!" RESULT "${OUTPUT_STRING}")
    if("${RESULT}" STREQUAL "")
        message(STATUS "\n########## Test passed. ##########\n")
    else()
        message(SEND_ERROR "\n########## Test failed. ##########\n")
    endif()
endfunction()

#-----------------------------------------------------------------------------
#       ビルド準備（パラメータ設定とビルド・フォルダとサブ・ファイル用意）
#-----------------------------------------------------------------------------

macro(setup_build_folder COMPILER BIT_NUM LIB_TYPE CONFIG_TYPE BUILD_DRIVER BUILD_DOCUMENT)

if(TRUE)
    message(STATUS "THEOLIZER_SOURCE        =${THEOLIZER_SOURCE}")
    message(STATUS "THEOLIZER_BINARY        =${THEOLIZER_BINARY}")
    message(STATUS "THEOLIZER_PREFIX        =${THEOLIZER_PREFIX}")
    message(STATUS "LIB_TYPE                =${LIB_TYPE}")
    message(STATUS "COMPILER                =${COMPILER}")
    message(STATUS "BIT_NUM                 =${BIT_NUM}")
    message(STATUS "CONFIG_TYPE             =${CONFIG_TYPE}")
    message(STATUS "BOOST_VERSION           =${BOOST_VERSION}")
    message(STATUS "BOOST_INSTALLEDx32      =${BOOST_INSTALLEDx32}")
    message(STATUS "BOOST_INSTALLEDx64      =${BOOST_INSTALLEDx64}")
    message(STATUS "BOOST_INSTALLEDx32fPIC  =${BOOST_INSTALLEDx32fPIC}")
    message(STATUS "BOOST_INSTALLEDx64fPIC  =${BOOST_INSTALLEDx64fPIC}")
    message(STATUS "BUILD_DRIVER            =${BUILD_DRIVER}")
    message(STATUS "BUILD_DOCUMENT          =${BUILD_DOCUMENT}")
    message(STATUS "PROC_ALL                =${PROC_ALL}")
    message(STATUS "CI_SERVICE              =${CI_SERVICE}")
    message(STATUS "REQUIRE_CMAKE_VERSION   =${REQUIRE_CMAKE_VERSION}")
endif()

    #       ---<<< パラメータ・チェック >>>---

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

    #       ---<<< ジェネレータ名生成 >>>---

    if(("${COMPILER}" STREQUAL "msvc2015")
    OR ("${COMPILER}" STREQUAL "msvc2017"))
        if ("${COMPILER}" STREQUAL "msvc2015")
            set(GENERATOR "Visual Studio 14 2015")
        else()
            set(GENERATOR "Visual Studio 15 2017")
        endif()
        if("${BIT_NUM}" STREQUAL "64")
            set(GENERATOR "${GENERATOR} Win64")
        endif()
        if(NOT "${CONFIG_TYPE}" STREQUAL "")
            message(SEND_ERROR "${CONFIG_TYPE} have to be \"\" for msvc")
        endif()
    elseif(("${COMPILER}" STREQUAL "gcc540")
        OR ("${COMPILER}" STREQUAL "gcc710"))
        set(GENERATOR "Unix Makefiles")
        if("${BIT_NUM}" STREQUAL "64")
            set(CC_PATH "${CC64}")
        else()
            set(CC_PATH "${CC32}")
        endif()
    elseif(("${COMPILER}" STREQUAL "mingw540")
        OR ("${COMPILER}" STREQUAL "mingw710"))
        set(GENERATOR "MinGW Makefiles")
        if("${BIT_NUM}" STREQUAL "64")
            set(CC_PATH "${CC64}")
        else()
            set(CC_PATH "${CC32}")
        endif()
    else()
        message(SEND_ERROR "unknown compiler ${COMPILER}")
    endif()
    message(STATUS "GENERATOR               =${GENERATOR}")

    #       ---<<< ビルド・フォルダ・パス設定 >>>---

    set(BUILD_DIR "${THEOLIZER_BINARY}${COMPILER}x${BIT_NUM}-${LIB_TYPE}")
    if(NOT "${CONFIG_TYPE}" STREQUAL "")
        set(BUILD_DIR "${BUILD_DIR}-${CONFIG_TYPE}")
    endif()
    message(STATUS "BUILD_DIR               =${BUILD_DIR}")

    #       ---<<< boostのフォルダ・パス生成 >>>---

    # boostのフォルダ・パス(BOOST_PATH)と
    # そのCMake変数名生成(BOOST_VARIABLE_NAME)
    #   BOOST_INSTALLEDx32
    #   BOOST_INSTALLEDx64
    #   BOOST_INSTALLEDx32fPIC
    #   BOOST_INSTALLEDx64fPIC
    set(BOOST_PATH "${THEOLIZER_BINARY}boost/${BOOST_VERSION}")
    set(BOOST_VARIABLE_NAME "BOOST_INSTALLEDx${BIT_NUM}")
    set(fPIC FALSE)
    if(NOT WIN32)
        if("${LIB_TYPE}" STREQUAL "Shared")
            set(BOOST_VARIABLE_NAME "${BOOST_VARIABLE_NAME}fPIC")
            set(fPIC TRUE)
        endif()
    endif()
    message(STATUS "BOOST_PATH              =${BOOST_PATH}")
    message(STATUS "BOOST_VARIABLE_NAME     =${BOOST_VARIABLE_NAME}")

    # ビルド・フォルダ・パス
    set(BOOST_BUILD "${BOOST_PATH}/${COMPILER}x${BIT_NUM}")
    if(fPIC)
        set(BOOST_BUILD "${BOOST_BUILD}-fPIC")
    endif()
    if(NOT "${CONFIG_TYPE}" STREQUAL "")
        set(BOOST_BUILD "${BOOST_BUILD}-${CONFIG_TYPE}")
    endif()
    message(STATUS "BOOST_BUILD             =${BOOST_BUILD}")

    # インストール・フォルダ・パス
    set(BOOST_INSTALL "${BOOST_PATH}/install${BIT_NUM}")
    if(fPIC)
        set(BOOST_INSTALL "${BOOST_INSTALL}-fPIC")
    endif()
    message(STATUS "BOOST_INSTALL           =${BOOST_INSTALL}")

    # BOOST_ROOTへ反映
    if("${${BOOST_VARIABLE_NAME}}" STREQUAL "")
        set(BOOST_ROOT "${BOOST_INSTALL}")
    else()
        set(BOOST_ROOT "${${BOOST_VARIABLE_NAME}}")
    endif()
    message(STATUS "BOOST_ROOT              =${BOOST_ROOT}")

    #       ---<<< llvmのフォルダ・パス生成 >>>---

    if("${BUILD_DRIVER}" STREQUAL "TRUE")
        if(NOT "${LLVM}" STREQUAL "")
            if("${COMPILER}" MATCHES "msvc")
                if("${LLVM_MSVC}" STREQUAL "")
                    set(LLVM_ROOT "${LLVM}/${COMPILER}x${BIT_NUM}")
                else()
                    set(LLVM_ROOT "${LLVM}/${LLVM_MSVC}x${BIT_NUM}")
                endif()
            else()
                if("${LLVM_CC}" STREQUAL "")
                    set(LLVM_ROOT "${LLVM}/${COMPILER}x${BIT_NUM}")
                else()
                    set(LLVM_ROOT "${LLVM}/${LLVM_CC}x${BIT_NUM}")
                endif()
            endif()
            if(NOT "${CONFIG_TYPE}" STREQUAL "")
                set(LLVM_ROOT "${LLVM_ROOT}-${CONFIG_TYPE}")
            endif()
        endif()
    else()
        set(LLVM_ROOT "")
    endif()
    message(STATUS "LLVM_ROOT               =${LLVM_ROOT}")

    #       ---<<< makeツール設定 >>>---

    if ("${MAKE}" STREQUAL "")
        set(MAKE "make")
    endif()
    message(STATUS "MAKE                    =${MAKE}")

    #       ---<<< ビルド・フォルダ生成 >>>---

    # full_all時のみビルド・フォルダを一度削除する
    if("${PROC_ALL}" STREQUAL "full_all")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${BUILD_DIR}"
        )
    endif()
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E make_directory   "${BUILD_DIR}"
    )

    #       ---<<< スクリプト・ファイルのコピー >>>---

    configure_file(tools/zz_process.cmake ${BUILD_DIR}/zz_process.cmake @ONLY)
    if(WIN32)
        file(GLOB SCRIPT_LIST tools/*.bat)
        file(COPY ${SCRIPT_LIST} DESTINATION  ${BUILD_DIR})
    else()
        file(GLOB SCRIPT_LIST tools/*.sh)
        file(COPY ${SCRIPT_LIST} DESTINATION  ${BUILD_DIR})
    endif()

    #       ---<<< boostの準備 >>>---

    include(tools/zz_boost.cmake)

endmacro()

#-----------------------------------------------------------------------------
#       ビルド準備とビルド：ビルド・フォルダ構築、configure、full-test
#-----------------------------------------------------------------------------

function(build_process COMPILER BIT_NUM LIB_TYPE CONFIG_TYPE BUILD_DRIVER BUILD_DOCUMENT)

    setup_build_folder("${COMPILER}" "${BIT_NUM}" "${LIB_TYPE}" "${CONFIG_TYPE}" "${BUILD_DRIVER}" "${BUILD_DOCUMENT}")

     # CI_SERVICEに於けるconfig_allはboostの準備までに限定する
    if(("${PROC_ALL}" STREQUAL "config_all") AND (NOT "${CI_SERVICE}" STREQUAL ""))
return()
    endif()

    # Theolizerプロジェクトのコンフィグ
    execute_process(
        COMMAND ${CMAKE_COMMAND} "-DPROC=config" "-DPROC_ALL=${PROC_ALL}" "-DSUMMARY=${SUMMARY}"
                "-DCI_SERVICE=${CI_SERVICE}" "-DPASS_LIST=${ARGN}"
                -P zz_process.cmake
        WORKING_DIRECTORY "${BUILD_DIR}"
        RESULT_VARIABLE RETURN_CODE
    )
    if(NOT ${RETURN_CODE} EQUAL 0)
        message(SEND_ERROR "error: ${RETURN_CODE}")
return()
    endif()

    # Theolizerプロジェクトのビルド（全フォルダのコンフィグの時はビルドしない)
    if(NOT "${PROC_ALL}" STREQUAL "config_all")
        execute_process(
            COMMAND ${CMAKE_COMMAND} "-DPROC=full" "-DPROC_ALL=${PROC_ALL}" "-DSUMMARY=${SUMMARY}"
                "-DCI_SERVICE=${CI_SERVICE}" "-DPASS_LIST=${ARGN}"
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

function(build_by_msvc COMPILER BIT_NUM LIB_TYPE BUILD_DRIVER BUILD_DOCUMENT PASS_LIST)

    if(${BUILD_DRIVER})
        set(DRIVER "[Build Driver] ")
    endif()
    output_title("------ ${COMPILER}x${BIT_NUM}-${LIB_TYPE} ${DRIVER}------")
    build_process("${COMPILER}" "${BIT_NUM}" "${LIB_TYPE}" "" "${BUILD_DRIVER}" "${BUILD_DOCUMENT}" ${PASS_LIST})

endfunction()

#       ---<<< GCC/MinGW用 >>>---

function(build_by_gcc COMPILER BIT_NUM LIB_TYPE BUILD_DRIVER BUILD_DOCUMENT RELEASE_LIST DEBUG_LIST)

    if(${BUILD_DRIVER})
        set(DRIVER "[Build Driver] ")
    endif()
    output_title("------ ${COMPILER}x${BIT_NUM}-${LIB_TYPE}-Release ${DRIVER}------")
    build_process("${COMPILER}" "${BIT_NUM}" "${LIB_TYPE}" "Release" "${BUILD_DRIVER}" "${BUILD_DOCUMENT}" ${RELEASE_LIST})

    if("${CI_SERVICE}" STREQUAL "")
        output_title("------ ${COMPILER}x${BIT_NUM}-${LIB_TYPE}-Debug ------")
        build_process("${COMPILER}" "${BIT_NUM}" "${LIB_TYPE}" "Debug"   "FALSE" "FALSE" ${DEBUG_LIST})
    endif()

endfunction()
