//############################################################################
/*!
    @brief      ドキュメント・ファイル－プログラム開発方法詳細
    @ingroup    Documents
    @file       4.project.h
    @author     Yoshinori Tahara
    @date       2016/11/01 Created
*/
/*
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
*/
//############################################################################

/*!
    @page HowToMakeProject Theolizerの組み込み方とファイル構成

<br>
//############################################################################
@section Selecting 1.Theolizerライブラリの選択
//############################################################################

---
Theolizerは下記ライブラリを用意しています。

| リンク種別名 | Theolizer | boost | 標準<br>ランタイム |お薦めの選択|
|--------------|-----------|-------|----------------|------------|
|StaticWithBoost|静的リンク|静的リンク（同梱）|動的リンク|あなたのプログラムが[boost](https://ja.wikipedia.org/wiki/Boost)を使っていない時|
|Static|静的リンク|静的リンク（非同梱）|動的リンク|あなたのプログラムが[boost](https://ja.wikipedia.org/wiki/Boost)を使っている時 <b>(*1)</b> |
|Shared|動的リンク|静的リンク|動的リンク|あなたのプログラムが[boost](https://ja.wikipedia.org/wiki/Boost)を使っていて<br>Theolizerライブラリのビルドを避けたい時 <b>(*2)</b> |
<b>(*1)</b> お使いの[boost](https://ja.wikipedia.org/wiki/Boost)のバージョンがTholizerの[boost](https://ja.wikipedia.org/wiki/Boost)のバージョンと異なる場合、[Theolizerライブラリのビルド](_how_to_build_theolizer.html#HowToBuildLibrary)が必要になります。<br>
<b>(*2)</b> Theolizerの動的リンク・ライブラリをパスの通ったフォルダへ置いて下さい

StaticとSharedは、それぞれ通常の静的リンク・ライブラリ、動的リンク・ライブラリです。

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
Theolizerは[boost](https://ja.wikipedia.org/wiki/Boost)のsystemとfilesystemを用いているため、Theolizerを静的リンクする場合は、これらのライブラリをあなたのプロジェクトでリンクする必要が有ります。[boost](https://ja.wikipedia.org/wiki/Boost)を既にお使いの方ならばあまり問題はないと思いますが、そうでない方にとってはboostのインストールと一部のビルドが新たに必要となります。<br>
その手間を省くため、プリビルド版の静的リンク・ライブラリとして[boost](https://ja.wikipedia.org/wiki/Boost)を同梱したライブラリも用意しました。それがStaticWithBoostです。
</div>

<br>
//############################################################################
@section CMake 2.CMakeを使う場合
//############################################################################

---
あなたのプロジェクトのCMakeLists.txtに下記を追加して下さい。

    project(<あなたのプロジェクト名>)
      :
    find_package(THEOLIZER)
      :
    add_executable(<ターゲット名>)
      or
    add_library(<ターゲット名>)
      :
    setup_theolizer(<ターゲット名> <リンク種別>)

`<ターゲット名>`はTheolizerライブラリをリンクするあなたのプログラムを指定します。
add_executable()やadd_library()のターゲット名です。

`<リンク種別>`はリンクするTheolizerライブラリの種別を指定します。
StaticWithBoost、Static、Sharedのどれかを指定して下さい。

CMakeLists.txt作成後、THEOLIZER_DIRに <b>Theolizerルート・フォルダ</b> を設定し、CMakeでプロジェクトを生成して下さい。<br>

<br>
//############################################################################
@section Example 3.exampleのビルド例(CMake使用)
//############################################################################

---

TheolizerのGitHubリポジトリのトップ・ページにあるexampleのソース・コードとCMakeLists.txtを <b>source/samples/example</b> に置いています。これは「2.CMakeを使う場合」のサンプルとなります。<br>
適切なビルド・フォルダを作成して、そこで下記コマンドを実行することで、プロジェクト生成→ビルド→テスト実行が行われます。<br>

全てsource/samples/exampleフォルダで作業して下さい。<br>
なお、下記は全てReleaseビルドの例です。Debugビルドする時は、Releaseの代わりにDebugを指定して下さい。<br>

#### Visual Studio 2015(32bits版)の場合

    mkdir msvc32
    cd msvc32
    cmake -G "Visual Studio 14" .. -DTHEOLIZER_DIR=<Theolizerルート・フォルダ>
    cmake --build . --config Release --target BuildTest

#### Visual Studio 2015(64bits版)の場合

    mkdir msvc64
    cd msvc64
    cmake -G "Visual Studio 14 Win64" .. -DTHEOLIZER_DIR=<Theolizerルート・フォルダ>
    cmake --build . --config Release --target BuildTest

#### MiGWの場合（事前にMinGWへのパスを設定下さい）

    mkdir mingw
    cd mingw
    cmake -G "MinGW Makefiles" .. -DTHEOLIZER_DIR=<Theolizerルート・フォルダ> -DCMAKE_BUILD_TYPE=Release
    make BuildTest

#### gcc(Linux)の場合

    mkdir gcc
    cd gcc
    cmake -G "Unix Makefiles" .. -DTHEOLIZER_DIR=<Theolizerルート・フォルダ> -DCMAKE_BUILD_TYPE=Release
    make BuildTest

<br>
//############################################################################
@section NonCMake 4.CMakeを使わない場合
//############################################################################

---
「@ref Mechanism43 」にて仕組み記述しています。また「@ref FileStructure 」に`<Theolizerルート・フォルダ>`からのフォルダ構成を記載しています。これらを参考に下記をビルド・プロジェクトへ設定下さい。

|コンパイラ・オプション|指定内容|
|----------------------|--------|
|コンパイラ|Theolizerルート・フォルダ/bin/TheolizerDriver|
|インクルード・パス|Theolizerルート・フォルダ/include|
|ライブラリ・パス|Theolizerルート・フォルダ/lib|
|ライブラリ・ファイル|「@ref Selecting 」で選択したライブラリ。<br>ファイル名は「@ref FileStructure 」参照。|
|マクロ定義|THEOLIZER_DO_PROCESS|
|マクロ定義(Visual C++の場合)|`theolizer_original_compiler=<元コンパイラのパス>`|
|オプション指定(その他の場合)|`--theolizer_original_compiler=<元コンパイラのパス>`|
|Sharedの場合は追加マクロ定義|THEOLIZER_DYN_LINK|

<br>
//############################################################################
@section FileStructure 5.ファイル構成
//############################################################################

---
<b>Theolizerルート・フォルダ</b>からの相対パスにて各ファイルが存在する場所を示します。

#### Visual Studio C++

|相対パス|ファイル種別|ファイル名|
|-------|----------|--------|
|ルート|cmakeのfind_package用<br>Theolizerのライセンス(GPLv3)|THEOLIZERConfig.cmake,theolizer.props.in<br>LICENSE.TXT|
|bin|Theolizerドライパ|TheolizerDriver.exe|
|include/theolizer|ヘッダ・ファイル群|`*.h`, `*.inc`|
|lib|StaticWithBoost(Release)ライブラリ|TheolizerLibStaticWithBoost.lib|
||Static(Release)ライブラリ|TheolizerLibStatic.lib|
||Shared(Release)ライブラリ|TheolizerLibShared.dll|
||StaticWithBoost(Debug)ライブラリ|TheolizerLibStaticWithBoostd.lib|
||Static(Debug)ライブラリ|TheolizerLibStaticd.lib|
||Shared(Debug)ライブラリ|TheolizerLibSharedd.dll|
|msbuild-bin|Theolizerドライパをリネームしたファイル|cl.exe|
|other_licenses|Theolizerにリンクしている他プロジェクトのライセンス|LICENSE-*.txt|

注）インクルード・パスは、include/theolizerではなく、includeを指定して下さい。

下記は各種補助ファイル群です。

|相対パス|ファイル種別|ファイル名|
|-------|----------|--------|
|doc/ja|詳細ドキュメント|index.html, html/\*.\*|
|lib|StaticWithBoost(Release)テスト用ライブラリ|TheolizerTestStaticWithBoost.lib|
||Static(Release)テスト用ライブラリ|TheolizerTestStatic.lib|
||Shared(Release)テスト用ライブラリ|TheolizerTestShared.dll|
||StaticWithBoost(Debug)テスト用ライブラリ|TheolizerTestStaticWithBoostd.lib|
||Static(Debug)テスト用ライブラリ|TheolizerTestStaticd.lib|
||Shared(Debug)テスト用ライブラリ|TheolizerTestSharedd.dll|
|samples|サンプル・プログラム|CMakeLists.txt, *.cpp, *.h|

注）テスト用ライブラリは、Theolizerの自動テストで使用しているライブラリです。

#### MinGWとgcc

|相対パス|ファイル種別|ファイル名|
|-------|----------|--------|
|ルート|cmakeのfind_package用<br>Theolizerのライセンス(GPLv3)|THEOLIZERConfig.cmake<br>LICENSE.TXT|
|bin|Theolizerドライパ|TheolizerDriver.exe|
|include/theolizer|ヘッダ・ファイル群|`*.h`, `*.inc`|
|lib/Relase|StaticWithBoost(Release)ライブラリ|libTheolizerLibStaticWithBoost.a|
||Static(Release)ライブラリ|libTheolizerLibStatic.a|
||Shared(Release)ライブラリ|libTheolizerLibShared.so|
|lib/Debug|StaticWithBoost(Debug)ライブラリ|libTheolizerLibStaticWithBoost.a|
||Static(Debug)ライブラリ|libTheolizerLibStatic.a|
||Shared(Debug)ライブラリ|libTheolizerLibShared.so|
|other_licenses|Theolizerにリンクしている他プロジェクトのライセンス|LICENSE-*.txt|

注）インクルード・パスは、include/theolizerではなく、includeを指定して下さい。

下記は各種補助ファイル群です。

|相対パス|ファイル種別|ファイル名|
|-------|----------|--------|
|doc/ja|詳細ドキュメント|index.html, html/\*.\*|
|lib/Relase|StaticWithBoost(Release)テスト用ライブラリ|libTheolizerTestStaticWithBoost.a|
||Static(Release)テスト用ライブラリ|libTheolizerTestStatic.a|
||Shared(Release)テスト用ライブラリ|libTheolizerTestShared.so|
|lib/Debug|StaticWithBoost(Debug)テスト用ライブラリ|libTheolizerTestStaticWithBoost.a|
||Static(Debug)テスト用ライブラリ|libTheolizerTestStatic.a|
||Shared(Debug)テスト用ライブラリ|libTheolizerTestShared.so|
|samples|サンプル・プログラム|CMakeLists.txt, *.cpp, *.h|

注）テスト用ライブラリは、Theolizerの自動テストで使用しているライブラリです。

*/
