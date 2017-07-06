#[[###########################################################################
        Windows用ビルドCMakeスクリプト

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
set(BOOST_DOWNLOAD "1.64.0")

# Boostのソース・フォルダ
set(BOOST_SOURCE "../boost")

# boostのインストール先のプリフィクス(これに32/64を追加したフォルダにインストールされる)
set(BOOST_PREFIX "C:/Boost")

# MinGW/gccのbinフォルダのパス
set(CC32 "C:/mingw-w64/i686-7.1.0-posix-dwarf-rt_v5-rev0/mingw32/bin")
set(CC64 "C:/mingw-w64/x86_64-7.1.0-posix-seh-rt_v5-rev0/mingw64/bin")

# 結果概要ファイル名
set(SUMMARY windows.log)

#-----------------------------------------------------------------------------
#       基本準備
#-----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.8.0)

include(prepare.cmake)

set(SUMMARY ${SUMMARY})
file(WRITE ${SUMMARY} "")

#-----------------------------------------------------------------------------
#       各ビルド・フォルダの準備とビルド・テスト・インストール
#
#       build_process(COMPILER BIT_NUM CONFIG_TYPE fPIC)
#           COMPILER        msvc2017/msvc2015/mingw710/mingw540/gcc540
#           BIT_NUM         64/32
#           CONFIG_TYPE     ""/Debug/Release
#           fPIC            FALSE/TRUE
#-----------------------------------------------------------------------------

output_title("****** msvc2017x64 ******")
build_process(msvc2017 64 "" FALSE)

output_title("****** msvc2017x32 ******")
build_process(msvc2017 32 "" FALSE)

output_title("****** mingw710x64-Release ******")
build_process(mingw710 64 "" FALSE)

output_title("****** mingw710x32-Release ******")
build_process(mingw710 32 "" FALSE)

output_summary()
