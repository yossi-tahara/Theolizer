//############################################################################
/*!
    @file program.h
    @brief  ドキュメント・ファイル－プログラム開発方法詳細
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
    @page HowToMakeProject Theolizerの組み込み方とファイル構成

@section Selecting 1.Theolizerライブラリの選択

---
Theolizerは下記ライブラリを用意しています。

| リンク種別名 | Theolizer | boost | 標準ランタイム |お薦めの選択|
|--------------|-----------|-------|----------------|------------|
|StaticWithBoost|静的リンク|静的リンク(同梱)|動的リンク|あなたのプログラムが[boost](https://ja.wikipedia.org/wiki/Boost)を使っていない時|
|Static|静的リンク|静的リンク(非同梱)|動的リンク|あなたのプログラムが[boost](https://ja.wikipedia.org/wiki/Boost)を使っている時 <b>(*1)</b> |
|Shared|動的リンク|静的リンク|動的リンク|あなたのプログラムが[boost](https://ja.wikipedia.org/wiki/Boost)を使っていて<br>Theolizerライブラリのビルドを避けたい時 <b>(*2)</b> |
<b>(*1)</b> お使いの[boost](https://ja.wikipedia.org/wiki/Boost)のバージョンがTholizerの[boost](https://ja.wikipedia.org/wiki/Boost)のバージョンと異なる場合、[Theolizerライブラリのビルド](_how_to_build_theolizer.html#HowToBuildLibrary)が必要になります。<br>
<b>(*2)</b> Theolizerの動的リンク・ライブラリをパスの通ったフォルダへ置いて下さい

StaticとSharedは、それぞれ通常の静的リンク・ライブラリ、動的リンク・ライブラリです。

Theolizerは[boost](https://ja.wikipedia.org/wiki/Boost)のsystemとfilesystemを用いているため、Theolizerを静的リンクする場合は、これらのライブラリをあなたのプロジェクトでリンクする必要が有ります。\n
[boost](https://ja.wikipedia.org/wiki/Boost)を既にお使いの方ならばあまり問題はないと思いますが、そうでない方にとっては結構面倒な作業が必要となります。\n
そこで、プリビルド版の静的リンク・ライブラリとして[boost](https://ja.wikipedia.org/wiki/Boost)を同梱したライブラリも用意しました。それがStaticWithBoostです。

<br>
@section CMake 2.CMakeを使う場合

---
あなたのプロジェクトのCMakeLists.txtに下記を追加して下さい。

    find_package(THEOLIZER)
      :
    setup_theolizer(${TARGET_NAME} ${LINK_TYPE})

${TARGET_NAME}はTheolizerライブラリをリンクするあなたのプログラムを指定します。
add_executable()やadd_library()のターゲット名です。

${LINK_TYPE}はリンクするTheolizerライブラリの種別を指定します。
StaticWithBoost、Static、Sharedのどれかを指定して下さい。

CMakeLists.txt作成後、THEOLIZER_DIRに <b>Theolizerルート・フォルダ</b> を設定し、CMakeでプロジェクトを生成して下さい。<br>

<br>
@section Example 3.exampleのビルド例(CMake使用)

---

TheolizerのGitHubリポジトリのトップ・ページにあるexampleのソース・コードとCMakeLists.txtを <b>source/samples/example</b> に置いています。これは「2.CMakeを使う場合」のサンプルとなります。\n
適切なビルド・フォルダを作成して、そこで下記コマンドを実行することで、プロジェクト生成→ビルド→テスト実行が行われます。\n

なお、下記は全てReleaseビルドの例です。Debugビルドする時は、Releaseの代わりにDebugを指定して下さい。\n

#### Visual Studio 2015(32bits版)の場合

    cmake -G "Visual Studio 14" <source/samples/exampleフォルダ> -DTHEOLIZER_DIR=<Theolizerルート・フォルダ>
    cmake --build . --config Release --target BuildTest

#### Visual Studio 2015(64bits版)の場合

    cmake -G "Visual Studio 14 Win64" <source/samples/exampleフォルダ> -DTHEOLIZER_DIR=<Theolizerルート・フォルダ>
    cmake --build . --config Release --target BuildTest

#### MiGWの場合（事前にMinGWへのパスを設定下さい）

    cmake -G "MinGW Makefiles" <source/samples/exampleフォルダ> -DTHEOLIZER_DIR=<Theolizerルート・フォルダ> -DCMAKE_BUILD_TYPE=Release
    make BuildTest

#### gcc(Linux)の場合

    cmake -G "Unix Makefiles" <source/samples/exampleフォルダ> -DTHEOLIZER_DIR=<Theolizerルート・フォルダ> -DCMAKE_BUILD_TYPE=Release
    make BuildTest

<br>
@section NonCMake 4.CMakeを使わない場合

---
「5.ファイル構成」を参考にして、下記をプロジェクトへ設定下さい。
- 一般的なライブラリと同様、インクルード・パス、ライブラリ・パス、および、ライブラリを指定下さい。
- コンパイラ・オプションにてTHEOLIZER_ANALYZEマクロを定義して下さい。
- Sharedを用いる場合は、コンパイラ・オプションにてTHEOLIZER_DYN_LINKマクロを定義して下さい。

<br>
@section FileStructure 5.ファイル構成

---
<b>Theolizerルート・フォルダ</b>からの相対パスにて各ファイルが存在する場所を示します。

### Visual Studio C++

|相対パス|ファイル種別|ファイル名|
|-------|----------|--------|
|bin|Theolizerドライパ|TheolizerDriver.exe|
|include/theolizer|ヘッダ・ファイル群|`*.h`, `*.inc`|
|lib|StaticWithBoost(Release)ライブラリ|TheolizerLibStaticWithBoost.lib|
||Static(Release)ライブラリ|TheolizerLibStatic.lib|
||Shared(Release)ライブラリ|TheolizerLibShared.dll|
||StaticWithBoost(Debug)ライブラリ|TheolizerLibStaticWithBoostd.lib|
||Static(Debug)ライブラリ|TheolizerLibStaticd.lib|
||Shared(Debug)ライブラリ|TheolizerLibSharedd.dll|
|ルート|Theolizerのライセンス(GPLv3)|LICENSE.TXT|
|other_licenses|Theolizerにリンクしている他プロジェクトのライセンス|LICENSE-*.txt|

注）インクルード・パスは、include/theolizerではなく、includeを指定して下さい。

下記は各種補助ファイル群です。

|相対パス|ファイル種別|ファイル名|
|-------|----------|--------|
|ルート|CMake補助|THEOLIZERConfig.cmake|
||ドライバ・インストール|replace.bat|
||ドライバ・アンインストール|restore.bat|
|bin|ドライバ・インストール補助|internal.bat|
|doc/ja|詳細ドキュメント|index.html, html/\*.\*|
|lib|StaticWithBoost(Release)テスト用ライブラリ|TheolizerTestStaticWithBoost.lib|
||Static(Release)テスト用ライブラリ|TheolizerTestStatic.lib|
||Shared(Release)テスト用ライブラリ|TheolizerTestShared.dll|
||StaticWithBoost(Debug)テスト用ライブラリ|TheolizerTestStaticWithBoostd.lib|
||Static(Debug)テスト用ライブラリ|TheolizerTestStaticd.lib|
||Shared(Debug)テスト用ライブラリ|TheolizerTestSharedd.dll|
|samples|サンプル・プログラム|CMakeLists.txt, *.cpp, *.h|

注）テスト用ライブラリは、Theolizerの自動テストで使用しているライブラリです。

### MinGWとgcc

|相対パス|ファイル種別|ファイル名|
|-------|----------|--------|
|bin|Theolizerドライパ|TheolizerDriver.exe|
|include/theolizer|ヘッダ・ファイル群|`*.h`, `*.inc`|
|lib/Relase|StaticWithBoost(Release)ライブラリ|libTheolizerLibStaticWithBoost.a|
||Static(Release)ライブラリ|libTheolizerLibStatic.a|
||Shared(Release)ライブラリ|libTheolizerLibShared.so|
|lib/Debug|StaticWithBoost(Debug)ライブラリ|libTheolizerLibStaticWithBoost.a|
||Static(Debug)ライブラリ|libTheolizerLibStatic.a|
||Shared(Debug)ライブラリ|libTheolizerLibShared.so|
|ルート|Theolizerのライセンス(GPLv3)|LICENSE.TXT|
|other_licenses|Theolizerにリンクしている他プロジェクトのライセンス|LICENSE-*.txt|

注）インクルード・パスは、include/theolizerではなく、includeを指定して下さい。

下記は各種補助ファイル群です。

|相対パス|ファイル種別|ファイル名|
|-------|----------|--------|
|ルート|CMake補助|THEOLIZERConfig.cmake|
|doc/ja|詳細ドキュメント|index.html, html/\*.\*|
|lib/Relase|StaticWithBoost(Release)テスト用ライブラリ|libTheolizerTestStaticWithBoost.a|
||Static(Release)テスト用ライブラリ|libTheolizerTestStatic.a|
||Shared(Release)テスト用ライブラリ|libTheolizerTestShared.so|
|lib/Debug|StaticWithBoost(Debug)テスト用ライブラリ|libTheolizerTestStaticWithBoost.a|
||Static(Debug)テスト用ライブラリ|libTheolizerTestStatic.a|
||Shared(Debug)テスト用ライブラリ|libTheolizerTestShared.so|
|samples|サンプル・プログラム|CMakeLists.txt, *.cpp, *.h|

*/
