#[[###########################################################################
        Travis CI用ビルドCMakeスクリプト

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
#       使い方
#           当スクリプト呼び出し時に-DPROC_ALL=xxxで下記のどれかを定義する
#               省略        ビルド・フォルダを削除後、全ての処理を行う。
#               short_all   コンフィグからTheolizerのインストールまでを行う。
#               last_all    インストールされたTheolizerが使えることを確認する。
#           下記CMake変数を定義する
#               THEOLIZER_SOURCE    Theolizerのソース・ツリーのルート・パス
#               THEOLIZER_BINARY    Theolizerのビルド・フォルダのプリフィクス
#               THEOLIZER_PREFIX    ビルドしたTheolizerのインストール先のプリフィクス
#               BOOST_PREFIX        boostがインストールされているルート・パスのプリフィクス
#               LLVM                llvmがインストールされているルート・パス
#               CC32/CC64           msvc以外のコンパイラのパス
#               MAKE                MinGWでjomを使う場合にjomのパス
#               PARALLEL            makeを使う場合の並列処理数
#               SUMMARY             結果概要を記録するファイル名
#           下記でビルドする対象を定義する
#               build_by_msvc()
#               build_by_gcc()
#*****************************************************************************

#-----------------------------------------------------------------------------
#       設定するパラメータ
#-----------------------------------------------------------------------------

set(CI_SERVICE "Travis")
message(STATUS "CI_SERVICE=${CI_SERVICE}")

# Theolizerのソース・フォルダ
set(THEOLIZER_SOURCE "../source")

# Theolizerのビルド・フォルダのプリフィクス
set(THEOLIZER_BINARY "../../build/")

# Theolizerのインストール先のプリフィクス
set(THEOLIZER_PREFIX "$ENV{TRAVIS_BUILD_DIR}/../install/Theolizer-")

# Boostバージョン指定
set(BOOST_VERSION "1.59.0")

# インストール済Boostを使う場合に指定する
# linuxでは.soファイルへリンクする場合、fPICオプションによるビルドが必要
if(FALSE)
    set(BOOST_INSTALLEDx32 "../../build/boost/${BOOST_VERSION}/install32")
    set(BOOST_INSTALLEDx64 "../../build/boost/${BOOST_VERSION}/install64")
    set(BOOST_INSTALLEDx32fPIC "../../build/boost/${BOOST_VERSION}/install32fPIC")
    set(BOOST_INSTALLEDx64fPIC "../../build/boost/${BOOST_VERSION}/install64fPIC")
endif()

# LLVMのバージョン
#   複数のLLVMがシステムにインストールされている時に指定する
set(LLVM_VERSION 4.0.0)

# llvmのダウンロード先フォルダ(ドライバをビルドしない時は未指定でOK)
#   msvc   : ${LLVM}/${LLVM_MSVC}x${BIT_NUM}                LLVM_MSVC未定義の時はCOMPILER
#   その他 : ${LLVM}/${LLVM_CC}x${BIT_NUM}-${CONFIG_TYPE}   LLVM_CC  未定義の時はCOMPILER
#   これ以外のフォルダを指定する時は、LLVMを""にしてLLVM_ROOTを設定する
set(LLVM_ROOT "/usr/lib/llvm-4.0/")

# llvmのダウンロード元URL
#   LLVM_DOWNLOADとLLVMの両方の指定が有る時は以下をダウンロードする
#       msvc   : ${LLVM_DOWNLOAD}/${LLVM_MSVC}x${BIT_NUM}.${LLVM_EXT}   LLVM_MSVC未定義の時はCOMPILER
#       その他 : ${LLVM_DOWNLOAD}/${LLVM_CC}x${BIT_NUM}.${LLVM_EXT}     LLVM_CC  未定義の時はCOMPILER
#set(LLVM_DOWNLOAD "")

# LLVMをコンパイルしたコンパイラ
#   プリビルド版使用時、もしくは、ターゲットと同じ時は""
#set(LLVM_MSVC msvc2015)
#set(LLVM_CC mingw710)

# 必須CMakeバージョン
set(REQUIRE_CMAKE_VERSION 3.2.2)

# MinGW/gccのbinフォルダのパス
set(CC32 "/usr/bin")
set(CC64 "/usr/bin")

# MinGW/gcc用のMakeのパス名(未指定ならmakeを使用する)
#set(MAKE "/usr/bin/make")

# make時の並列処理数を指定(msvc、jom使用時は指定不要)
set(PARALLEL "-j4")

# 結果概要ファイル名
set(SUMMARY linux-${PROC_ALL}.log)

#-----------------------------------------------------------------------------
#       基本準備
#-----------------------------------------------------------------------------

cmake_minimum_required(VERSION ${REQUIRE_CMAKE_VERSION})

include(tools/zz_prepare.cmake)

set(SUMMARY ${THEOLIZER_BINARY}${SUMMARY})
file(WRITE ${SUMMARY} "")

set(ENV{LC_ALL} "C")

#-----------------------------------------------------------------------------
#       各ビルド・フォルダの準備とビルド・テスト・インストール
#
#       build_by_gcc(COMPILER BIT_NUM LIB_TYPE BUILD_DRIVER BUILD_DOCUMENT RELEASE_LIST DEBUG_LIST)
#           COMPILER        msvc2015/mingw540/gcc540
#           BIT_NUM         64/32
#           LIB_TYPE        StaticWithBoost/Static/Shared
#           BUILD_DRIVER    TRUE/FALSE
#           RELEASE_LIST    CTest結果の正しいテスト数のリスト(Release用)
#           DEBUG_LIST      CTest結果の正しいテスト数のリスト(Debug用)
#-----------------------------------------------------------------------------

output_title("****** StaticWithBoost ******")
if("$ENV{PARALLEL_TEST}" STREQUAL "1")
    build_by_gcc(gcc540 64 StaticWithBoost TRUE FALSE "15 1" "11 1")
elseif("$ENV{PARALLEL_TEST}" STREQUAL "2")
    build_by_gcc(gcc540 64 StaticWithBoost TRUE FALSE "14 1" "11 1")
else()
    build_by_gcc(gcc540 64 StaticWithBoost TRUE FALSE "32 1" "11 1")
endif()

#output_title("****** Static ******")
#build_by_gcc(gcc540 64 Static FALSE FALSE "13 1" "11 1")

#output_title("****** Shared ******")
#build_by_gcc(gcc540 64 Shared FALSE FALSE "13 1" "11 1")

output_summary()
