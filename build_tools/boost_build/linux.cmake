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

# boostのインストール先のプリフィクス(これに32/64を追加したフォルダにインストールされる)
# linuxの場合、更にTheolizerライブラリSharedへリンクするものは-fPICが追加される。
set(BOOST_PREFIX "$ENV{HOME}/install/boost")

# MinGW/gccのbinフォルダのパス
set(CC32 "/usr/bin")
set(CC64 "/usr/bin")

# 結果概要ファイル名
set(SUMMARY linux.log)

#-----------------------------------------------------------------------------
#       基本準備
#-----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.5.0)

include(prepare.cmake)

set(SUMMARY ${SUMMARY})
file(WRITE ${SUMMARY} "")

#-----------------------------------------------------------------------------
#       各ビルド・フォルダの準備とビルド・テスト・インストール
#
#       build_process(COMPLIER BIT_NUM CONFIG_TYPE fPIC)
#           COMPLIER        msvc2015/mingw540/gcc540
#           BIT_NUM         64/32
#           CONFIG_TYPE     ""/Debug/Release
#           fPIC            FALSE/TRUE
#-----------------------------------------------------------------------------

output_title("****** gcc540x64-Release ******")
build_process(gcc540 64 "Release" FALSE)
build_process(gcc540 64 "Release" TRUE)

output_summary()
