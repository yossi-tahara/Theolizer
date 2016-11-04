//############################################################################
/*!
    @file build_theolizer.h
    @brief  ドキュメント・ファイル－Theolizerのビルド手順
    @author Yoshinori Tahara
    @date   2016/11/01  Created
*/
/*
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
*/
//############################################################################

/*!
    @page HowToBuildTheolizer Theolizerのビルド手順

@section HowToBuildLibrary 1.Theolizerライブラリのビルド方法

---

@subsection HowToBuildLibrary11 1-1.使用するツールとリンクするライブラリ
|アイテム|条件|検証済のアイテム|
|-----|----|-----|
|C++コンパイラ|C++11規格対応|[Visual Studio C++ 2015 Community update 3](https://www.microsoft.com/ja-jp/dev)|
|||[MinGW 5.4.0 32bit posix dwarf](https://sourceforge.net/projects/mingw-w64/)|
|||[MinGW 5.4.0 64bit posix seh](https://sourceforge.net/projects/mingw-w64/)|
|||gcc 5.4.0(Ubuntu 5.4.0-6ubuntu1~16.04.2)|
|CMake|3.5.0以上|[CMake 3.5.0 32bit](https://cmake.org/)|
|boost|1.59.0以上|[boost 1.59.0](http://www.boost.org/)|

TheolzierのドキュメントはDoxygenとGraphvizを用いて生成します。現在は下記バージョンを用いています。

|Doxygen|Graphviz|
|-------|--------|
|[1.8.12](http://www.stack.nl/~dimitri/doxygen/download.html)|[2.38](http://www.graphviz.org/Download..php)

@subsection HowToBuildLibrary12 1-2.ビルド手順

下記手順でTheolizerライブラリをビルドします。

1. boostのsystemとfilesystemをビルドしてインストール
2. Theolizerライブラリをビルドしてインストール

@subsubsection HowToBuildLibrary121 1-2-1.boostのsystemとfilesystemをビルドしてインストール
コンパイラはTheolizerをビルドするものと同じものを使用します。<br>

まず、使いたいバージョンの[boost](http://www.boost.org/users/history/)をダウンロードして適切なフォルダへ解凍しておいて下さい。

次に、boostをビルドします。便利のためにcmakeスクリプトを用意しています。Theolizerのソースのbuild/boost_buildフォルダにおいてますので、その中のファイルをboostのソースがあるフォルダ（1.59.0ならboost_1_59_0の直下)へコピーしてお使い下さい。<br>
Windowsでビルドする場合はwindows.cmakeを、linuxでビルドする場合linux.cmakeを用います。
使う方のcmakeスクリプトをエディタで開き、下記を指定して下さい。

|設定先|設定内容|
|------|--------|
|BOOST_PREFIX|boostのインストール先フォルダ名のプリフィクス|
|CC32|MingWまたはgccの32ビット版のbinフォルダのパス(既にパスが通っているなら指定不要)|
|CC64|MingWまたはgccの64ビット版のbinフォルダのパス(既にパスが通っているなら指定不要)|
|build_process()|ビルドする組み合わせを指定|

そして、Windowsならzz_full_all.bat、linuxならzz_full_all.shを起動すればビルドとインストールされます。<br>
１組み合わせ当たり数分でビルド完了します。

boostのソースがあるフォルダの直下にbuildフォルダが生成され、その中に中間ファイル群が置かれます。\n
ビルド中のログもここに保存しますので必要に応じて確認下さい。

@subsubsection HowToBuildLibrary122 1-2-2.Theolizerライブラリをビルドしてインストール

boostのビルドと同様、Theolizerをビルドするためのcmakeスクリプトを用意していますので、修正してお使い下さい。

|設定先|設定内容|
|------|--------|
|THEOLIZER_SOURCE|Theolizerのソースがあるルート・フォルダ|
|THEOLIZER_BINARY|ビルド時の中間ファイル群を置くフォルダ|
|THEOLIZER_PREFIX|Theolizerのインストール先ルート・フォルダ|
|BOOST|boostがインストールされているフォルダのプリフィックス|
|LLVM|LLVMのインストール先のパス(Theolizerライブラリのビルド時は指定不要)|
|CC32|MingWまたはgccの32ビット版のbinフォルダのパス(既にパスが通っているなら指定不要)|
|CC64|MingWまたはgccの64ビット版のbinフォルダのパス(既にパスが通っているなら指定不要)|
|MAKE|[jom](https://wiki.qt.io/Jom)等の高速なmakeを使う場合にそのフルパス(標準のmakeを使う時は指定不要)|
|build_by_msvc()|Visual Studio 2015でビルドする時にビルドする組み合わせを指定する|
|build_by_gcc()|MinGWもしくはgccでビルドする時にビルドする組み合わせを指定する|

build_by_msvc()とbuild_by_gcc()について補足
- BUILD_DRIVERにTRUEを指定するとドライバをビルドする<br>
この時はLLVMの指定が必要
- BUILD_DOCUMENTにTRUEを指定するとドキュメントをビルドする<br>
この時は[Doxygen](http://www.doxygen.jp/)と[Graphviz](http://www.graphviz.org/)のインストールが必要

下記のバッチ(Windowsの場合)、もしくは、スクリプト(linuxの場合)を起動することでTheolizerのビルド・自動テスト・インストールを行います。<br>
１組み合わせ当たり数分でビルド完了します。

|linuxのスクリプト<br>Windowsのバッチ|処理内容|
|------------------------------------|--------|
|zy0_full_all.sh<br>zz0_full_all.bat |ビルド・フォルダを削除し作成<br>CMakeによるプロジェクト生成<br>ライブラリのビルドとテスト<br>(ドライバのビルドとテスト)<br>ドキュメントのビルド<br>ライブラリとドキュメントのインストール<br>インストール後のビルド・テスト|
|zy1_config_all.sh<br>zz1_config_all.bat |ビルド・フォルダがなければ作成<br><b>CMakeによるプロジェクト生成</b>|
|zy2_middle_all.sh<br>zz2_middle_all.bat |ビルド・フォルダがなければ作成<br>CMakeによるプロジェクト生成<br><b>ライブラリのビルドとテスト</b><br><b>(ドライバのビルドとテスト)</b><br>ドキュメントのビルド<br><b>Theolizerのインストール</b>|
|zy3_last_all.sh<br>zz3_last_all.bat |ビルド・フォルダがなければ作成<br>CMakeによるプロジェクト生成<br><b>インストール確認のためのビルド・テスト</b>|

ビルド・フォルダに下記のバッチ(Windowsの場合)、もしくは、スクリプト(linuxの場合)が生成されます。必要に応じてお使い下さい。
また、Visual Studioの場合はソリューション・ファイル(Theolizer.sln)も生成されています。これをダブル・クリックすればVisual Studioが起動します。

|linuxのスクリプト<br>Windowsのバッチ|処理内容|
|------------------------------------|--------|
|zy0_config.sh<br>zz0_config.bat |CMakeによるプロジェクト生成|
|zy1_short_test.sh<br>zz1_short_test.bat |ライブラリのビルドとテスト|
|zy2_long_test.sh<br>zz1_short_test.bat |ドライバのビルドとテスト|
|zy3_full_test.sh<br>zz3_full_test.bat |ライブラリのビルドとテスト<br>ドライバのビルドとテスト<br>ドキュメントのビルド<br>Theolizerのインストール<br>インストール確認のためのビルド・テスト|
|zy4_ja.sh<br>zz4_ja.bat |ドキュメントのビルド|

<br>
@section HowToBuildDriver 2.Theolizerドライバのビルド方法

---

@subsection HowToBuildLibrary21 2-1.使用するツールとリンクするライブラリ
「1-1.使用するツールとリンクするライブラリ」に加えて、下記が必要です。

|アイテム|条件|検証済のアイテム|
|-----|----|-----|
|LLVM|3.9.0|[LLVM 3.9.0](http://llvm.org/)|

注）LLVM 3.8.1でもビルドできると思いますが、最終的なテストは行っていません。

@subsection HowToBuildLibrary22 2-2.ビルド手順

下記手順でTheolizerライブラリをビルドします。

1. boostのsystemとfilesystemをビルドしてインストール
2. LLVM(libTooling含む)をビルドしてインストール
3. Theolizerライブラリをビルドしてインストール

@subsubsection HowToBuildLibrary221 2-2-1.boostのsystemとfilesystemをビルドしてインストール

@ref HowToBuildLibrary121 と同じなので省略します。

@subsubsection HowToBuildLibrary222 2-2-2.LLVM(libTooling含む)をビルドしてインストール
コンパイラはTheolizerをビルドするものと同じものを使用して下さい。

まず、[LLVM](http://llvm.org/releases/download.html#3.9.0)をダウンロードして適切なフォルダへ解凍して下さい。

|ダウンロードするアイテム|ダウンロードするファイル名|解凍先フォルダ|
|------------------------|--------------------------|--------------|
|LLVM source code|llvm-3.9.0.src.tar.xz|llvm|
|Clang source code|cfe-3.9.0.src.tar.xz|llvm/tools/clang|
|clang-tools-extra|clang-tools-extra-3.9.0.src.tar.xz|llvm/tools/clang/tools/extra|

次に、LLVMをビルドしますが、そのためのcmakeスクリプトを用意しています。Theolizerのソースのbuild/llvm_buildフォルダにおいてますので、llvmフォルダと同じ階層のフォルダへコピーして下さい。<br>

下記のようなフォルダ構成になります

    llvm        解凍したLLVMソース
    llvm_build  linux.cmake, prepare.cmake, windows.cmake, zz_full_all.bat, zz_full_all.sh

Windowsでビルドする場合はwindows.cmakeを、linuxでビルドする場合linux.cmakeを用います。
使う方のcmakeスクリプトをエディタで開き、下記を修正して下さい。

|設定先|設定内容|
|------|--------|
|LLVM_PREFIX|LLVMのインストール先パス名のプリフィックス|
|CC32|MingWまたはgccの32ビット版のbinフォルダのパス(既にパスが通っているなら指定不要)|
|CC64|MingWまたはgccの64ビット版のbinフォルダのパス(既にパスが通っているなら指定不要)|
|MAKE|[jom](https://wiki.qt.io/Jom)等の高速なmakeを使う場合にそのフルパス(標準のmakeを使う時は指定不要)|
|build_process()|ビルドする組み合わせを指定|

そして、Windowsならzz_full_all.bat、linuxならzz_full_all.shを起動すればビルドとインストールされます。<br>
１組み合わせ当たり<b>１～２時間かかります</b>ので、時間に余裕のある時にビルドして下さい。

llvm_buildフォルダの直下にコンパイラ毎にbuild用フォルダが生成され、その中に中間ファイル群が置かれます。\n
ビルド中のログもここに保存しますので必要に応じて確認下さい。

@subsubsection HowToBuildLibrary223 2-2-3.Theolizerライブラリをビルドしてインストール

@ref HowToBuildLibrary122 と同じなので省略します。

<br>
@section Documents 3.ドキュメントについて補足

---

Theolizerのドキュメントは、主にsource/reference_and_test/jaフォルダにおいています。<br>
また、ライブラリのソース・コードはsource/libraryですが、ユーザが直接使うことを想定しているもののみドキュメントを付けています。<br>

Theolizer内部用の要素についてはDoxygen化していません。<br>
名前空間 <b>internal</b> 内のクラス等 全ての要素、および、<b>THEOLIZER_INTERNAL</b> もしくは <b>THEOLIZER_GENERATED</b>で始まるマクロは全てTheolizer内部用です。<br>
Theolizerのアップデート時、上位互換性を考慮しませんので使用しないようお願いします。

*/
