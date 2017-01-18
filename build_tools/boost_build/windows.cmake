#[[###########################################################################
        Windows用ビルドCMakeスクリプト

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
#               BOOST_PREFIX        ビルドしたboostのインストール先のプリフィクス
#               CC32/CC64           msvc以外のコンパイラのパス
#               MAKE                MinGWでjomを使う場合にjomのパス
#               SUMMARY             結果概要を記録するファイル名
#           下記でビルドする対象を定義する
#               build_process()
#*****************************************************************************

#-----------------------------------------------------------------------------
#       設定するパラメータ
#-----------------------------------------------------------------------------

# Boostダウンロード指定
set(BOOST_DOWNLOAD "1.59.0")

# Boostのソース・フォルダ
set(BOOST_SOURCE "../boost")

# boostのインストール先のプリフィクス(これに32/64を追加したフォルダにインストールされる)
set(BOOST_PREFIX "C:/Boost")

# MinGW/gccのbinフォルダのパス
set(CC32 "C:/mingw-w64/i686-5.4.0-posix-dwarf-rt_v5-rev0/mingw32/bin")
set(CC64 "C:/mingw-w64/x86_64-5.4.0-posix-seh-rt_v5-rev0/mingw64/bin")

# 結果概要ファイル名
set(SUMMARY windows.log)

#-----------------------------------------------------------------------------
#       基本準備
#-----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.2.2)

include(prepare.cmake)

set(SUMMARY ${SUMMARY})
file(WRITE ${SUMMARY} "")

#-----------------------------------------------------------------------------
#       各ビルド・フォルダの準備とビルド・テスト・インストール
#
#       build_process(COMPILER BIT_NUM CONFIG_TYPE fPIC)
#           COMPILER        msvc2015/mingw540/gcc540
#           BIT_NUM         64/32
#           CONFIG_TYPE     ""/Debug/Release
#           fPIC            FALSE/TRUE
#-----------------------------------------------------------------------------

output_title("****** msvc2015x64 ******")
build_process(msvc2015 64 "" FALSE)

output_title("****** msvc2015x32 ******")
build_process(msvc2015 32 "" FALSE)

output_title("****** mingw540x64-Release ******")
build_process(mingw540 64 "" FALSE)

output_title("****** mingw540x32-Release ******")
build_process(mingw540 32 "" FALSE)

output_summary()
