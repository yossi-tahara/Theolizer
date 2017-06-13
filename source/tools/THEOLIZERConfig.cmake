#[[###########################################################################

        Theolizer find package

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

#*****************************************************************************
#       THEOLIZER_DIRを設定してproject()命令後でfind_package(THEOLIZER)
#           下記を行う
#               Theolizerライブラリのインポート
#               Theolizerのルート・パス設定
#               setup_theolizer()マクロ定義
#                   Theolizerドライバへの解析指示(-DTHEOLIZER_ANALYZE)
#                   Theolizerインクルード・パス設定
#                   Theolizerライブラリ・パス設定
#                   Theolizerライブラリのリンク設定
#                   THEOLIZER_ROOTにTheolizerルートのパスを設定
#                   THEOLIZER_LIBにTheolizerライブラリのパスを設定
#*****************************************************************************

#-----------------------------------------------------------------------------
#       Theolizer root path
#-----------------------------------------------------------------------------

get_filename_component(THEOLIZER_ROOT ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
message(STATUS "THEOLIZER_ROOT=${THEOLIZER_ROOT}")

#-----------------------------------------------------------------------------
#       TheolizerライブラリのIMPORT
#-----------------------------------------------------------------------------

function(import_library_msvc LIB_NAME LIB_TYPE)
    add_library(${LIB_NAME} ${LIB_TYPE} IMPORTED)
    if(${LIB_TYPE} STREQUAL "STATIC")
        set_target_properties(${LIB_NAME}
            PROPERTIES IMPORTED_LOCATION ${THEOLIZER_ROOT}/lib/${LIB_NAME}.lib)
    else()
        set_target_properties(${LIB_NAME}
            PROPERTIES IMPORTED_LOCATION ${THEOLIZER_ROOT}/lib/${LIB_NAME}.dll)
        set_target_properties(${LIB_NAME}
            PROPERTIES IMPORTED_IMPLIB   ${THEOLIZER_ROOT}/lib/${LIB_NAME}.lib)
    endif()
endfunction()

function(import_library_gcc LIB_NAME LIB_TYPE)
    add_library(${LIB_NAME} ${LIB_TYPE} IMPORTED)
    if(${LIB_TYPE} STREQUAL "STATIC")
        set_target_properties(${LIB_NAME}
            PROPERTIES IMPORTED_LOCATION_RELEASE ${THEOLIZER_ROOT}/lib/Release/lib${LIB_NAME}.a)
        set_target_properties(${LIB_NAME}
            PROPERTIES IMPORTED_LOCATION_DEBUG   ${THEOLIZER_ROOT}/lib/Debug/lib${LIB_NAME}.a)
    elseif(WIN32)
        set_target_properties(${LIB_NAME}
            PROPERTIES IMPORTED_LOCATION_RELEASE ${THEOLIZER_ROOT}/lib/Release/lib${LIB_NAME}.dll)
        set_target_properties(${LIB_NAME}
            PROPERTIES IMPORTED_IMPLIB_RELEASE   ${THEOLIZER_ROOT}/lib/Release/lib${LIB_NAME}.dll.a)
        set_target_properties(${LIB_NAME}
            PROPERTIES IMPORTED_LOCATION_DEBUG   ${THEOLIZER_ROOT}/lib/Debug/lib${LIB_NAME}.dll)
        set_target_properties(${LIB_NAME}
            PROPERTIES IMPORTED_IMPLIB_DEBUG     ${THEOLIZER_ROOT}/lib/Debug/lib${LIB_NAME}.dll.a)
    else()
        set_target_properties(${LIB_NAME}
            PROPERTIES IMPORTED_LOCATION_RELEASE ${THEOLIZER_ROOT}/lib/Release/lib${LIB_NAME}.so)
        set_target_properties(${LIB_NAME}
            PROPERTIES IMPORTED_LOCATION_DEBUG   ${THEOLIZER_ROOT}/lib/Debug/lib${LIB_NAME}.so)
    endif()
endfunction()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    import_library_msvc(TheolizerLibStaticWithBoost STATIC)
    import_library_msvc(TheolizerTestStaticWithBoost STATIC)
    import_library_msvc(TheolizerLibStaticWithBoostd STATIC)
    import_library_msvc(TheolizerTestStaticWithBoostd STATIC)

    import_library_msvc(TheolizerLibStatic STATIC)
    import_library_msvc(TheolizerTestStatic STATIC)
    import_library_msvc(TheolizerLibStaticd STATIC)
    import_library_msvc(TheolizerTestStaticd STATIC)

    import_library_msvc(TheolizerLibShared SHARED)
    import_library_msvc(TheolizerTestShared SHARED)
    import_library_msvc(TheolizerLibSharedd SHARED)
    import_library_msvc(TheolizerTestSharedd SHARED)
else()
    set(ORIGINAL_COMPILER "${CMAKE_CXX_COMPILER}")
    set(CMAKE_CXX_COMPILER "${THEOLIZER_ROOT}/bin/TheolizerDriver.exe")
    message(STATUS "ORIGINAL_COMPILER =${ORIGINAL_COMPILER}")
    message(STATUS "CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")

    import_library_gcc(TheolizerLibStaticWithBoost STATIC)
    import_library_gcc(TheolizerTestStaticWithBoost STATIC)

    import_library_gcc(TheolizerLibStatic STATIC)
    import_library_gcc(TheolizerTestStatic STATIC)

    import_library_gcc(TheolizerLibShared SHARED)
    import_library_gcc(TheolizerTestShared SHARED)
endif()

#-----------------------------------------------------------------------------
#       setup_theolizer(TARGET_NAME LIB_TYPE)定義
#
#       ${TARGET_NAME}に対して、下記を設定します
#               Theolizerドライバへの追加パラメータ設定
#               インクルード・パス
#               ライブラリ・パス
#               リンクするTheolizerライブラリ
#       ${LIB_TYPE}は下記のどれか
#               StaticWithBoost 静的リンク・ライブラリ(標準ライブラリも静的リンク)
#               Static          静的リンク・ライブラリ(標準ライブラリは動的リンク)
#               Shared          動的リンク・ライブラリ
#-----------------------------------------------------------------------------

macro(setup_theolizer TARGET_NAME LIB_TYPE)

    #Theolizerドライバへの解析要求
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTHEOLIZER_ANALYZE")

    #共有ライブラリ対応
    if(${LIB_TYPE} STREQUAL "Shared")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTHEOLIZER_DYN_LINK")
    endif()

    # Header
    include_directories(${THEOLIZER_ROOT}/include)

    # Library
    link_directories("${THEOLIZER_ROOT}/lib/${CMAKE_BUILD_TYPE}")

    if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
        target_link_libraries(${TARGET_NAME} optimized TheolizerLib${LIB_TYPE})
        target_link_libraries(${TARGET_NAME} optimized TheolizerTest${LIB_TYPE})
        target_link_libraries(${TARGET_NAME} debug     TheolizerLib${LIB_TYPE}${CMAKE_DEBUG_POSTFIX})
        target_link_libraries(${TARGET_NAME} debug     TheolizerTest${LIB_TYPE}${CMAKE_DEBUG_POSTFIX})
        set(THEOLIZER_LIB "${THEOLIZER_ROOT}/lib")
    else()
        target_link_libraries(${TARGET_NAME} TheolizerLib${LIB_TYPE} TheolizerTest${LIB_TYPE})
        set(THEOLIZER_LIB "${THEOLIZER_ROOT}/lib/${CMAKE_BUILD_TYPE}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --theolizer-compiler=${ORIGINAL_COMPILER}")
    endif()

endmacro()
