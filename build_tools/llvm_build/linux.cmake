#[[###########################################################################
        Linux用ビルドCMakeスクリプト

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

#*****************************************************************************
#       使い方
#           下記CMake変数を定義する
#               LLVM_SOURCE         LLVMのソース・ツリーのルート・パス
#               LLVM_PREFIX         ビルドしたLLVMのインストール先のプリフィクス
#               CC32/CC64           msvc以外のコンパイラのパス
#               MAKE                MinGWでjomを使う場合にjomのパス
#               PARALLEL            makeを使う場合の並列処理数
#               SUMMARY             結果概要を記録するファイル名
#           下記でビルドする対象を定義する
#               build_process()
#*****************************************************************************

#-----------------------------------------------------------------------------
#       設定するパラメータ
#-----------------------------------------------------------------------------

# LLVMダウンロード指定
set(LLVM_DOWNLOAD "3.9.0")

# LLVMのソース・フォルダ
set(LLVM_SOURCE "../llvm")

# LLVMのインストール先のプリフィクス
set(LLVM_PREFIX "$ENV{HOME}/install/llvm390/")

# MinGW/gccのbinフォルダのパス
set(CC32 "/usr/bin")
set(CC64 "/usr/bin")

# MinGW/gcc用のMakeのパス名(未指定ならmakeを使用する)
#set(MAKE "/usr/bin/make")

# make時の並列処理数を指定(msvc、jom使用時は指定不要)
set(PARALLEL "-j4")

# 結果概要ファイル名
set(SUMMARY linux.log)

#-----------------------------------------------------------------------------
#       基本準備
#-----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.2.3)

set(SUMMARY ${SUMMARY})
file(WRITE ${SUMMARY} "")

include(prepare.cmake)

#-----------------------------------------------------------------------------
#       各ビルド・フォルダの準備とビルド・テスト・インストール
#
#       build_process(COMPILER BIT_NUM CONFIG_TYPE)
#           COMPILER        msvc2015/mingw540/gcc540
#           BIT_NUM         64/32
#           CONFIG_TYPE     ""/Debug/Release
#               ""はmsvcのみ可能。""の時はDebugとReleaseの両方をビルドする。
#-----------------------------------------------------------------------------

output_title("****** gcc540x64-Release ******")
build_process(gcc540 64 Release)

output_title("****** gcc540x32-Release ******")
build_process(gcc540 32 Release)

output_summary()
