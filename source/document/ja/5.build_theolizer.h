//############################################################################
/*!
    @brief      ドキュメント・ファイル－Theolizerのビルド手順
    @ingroup    Documents
    @file       5.build_theolizer.h
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
    @page HowToBuildTheolizer Theolizerのビルド手順

@section HowToBuildLibrary 1.Theolizerライブラリのビルド方法

---

@subsection HowToBuildLibrary11 1-1.使用するツールとリンクするライブラリ
@subsubsection HowToBuildLibrary111 1-1-1.v1.1.3以降

|アイテム|条件|検証済のアイテム|
|-----|----|-----|
|C++コンパイラ|C++11規格対応|[Visual Studio C++ 2017 Community Version 15.2](https://www.microsoft.com/ja-jp/dev)|
|||[MinGW 7.1.0 32bit posix dwarf](https://sourceforge.net/projects/mingw-w64/)|
|||[MinGW 7.1.0 64bit posix seh](https://sourceforge.net/projects/mingw-w64/)|
|||gcc 5.4.0(Ubuntu 5.4.0-6ubuntu1~16.04.2)|
|CMake|3.8.0以上|[CMake 3.8.0 64bit](https://cmake.org/)|
|boost|1.64.0以降|[boost 1.64.0](http://www.boost.org/)|

TheolzierのドキュメントはDoxygenとGraphvizを用いて生成します。現在は下記バージョンを用いています。

|Doxygen|Graphviz|
|-------|--------|
|[1.8.12](http://www.stack.nl/~dimitri/doxygen/download.html)|[2.38](http://www.graphviz.org/Download..php)

@subsubsection HowToBuildLibrary112 1-1-2.v1.1.2以前

|アイテム|条件|検証済のアイテム|
|-----|----|-----|
|C++コンパイラ|C++11規格対応|[Visual Studio C++ 2015 Community update 3](https://www.microsoft.com/ja-jp/dev)|
|||[MinGW 5.4.0 32bit posix dwarf](https://sourceforge.net/projects/mingw-w64/)|
|||[MinGW 5.4.0 64bit posix seh](https://sourceforge.net/projects/mingw-w64/)|
|||gcc 5.4.0(Ubuntu 5.4.0-6ubuntu1~16.04.2)|
|CMake|3.5.0以上|[CMake 3.5.0 32bit](https://cmake.org/)|
|boost|1.59.0以上|[boost 1.59.0](http://www.boost.org/)|

@subsection HowToBuildLibrary12 1-2.ビルド概要
TheolizerライブラリはBoostを使用しています。ヘッダオンリーではないsystem, filesystemをリンクします。そこで、Boostを[公式](http://www.boost.org/)からダウンロードしてこれらのライブラリをビルドする必要があります。<br>
<br>
Theolizerのビルド・システムでは、便利のためにcmakeスクリプトを用意しています。そのcmakeスクリプトにて以下の処理を行います。<br>

1. 指定バージョンのBoostソース・コード一式をBoost公式からダウンロード
2. Theolizerが必要とするBoostライブラリをビルド(数分で完了)
3. Theolizerライブラリをビルド
4. Theolizerライブラリの自動テスト実施
5. (Theolizerドライドのビルド準備とビルドと自動テスト実施：次節で解説します。)
6. Theolizer一式のインストール
7. インストール出来ていることの自動テスト

@subsection HowToBuildLibrary13 1-3.ビルド用スクリプトの設定とビルド
cmakeスクリプトはTholizerソースのbuild_toolsフォルダにおいてます。Windowsでビルドする場合はwindows.cmakeを、linuxでビルドする場合linux.cmakeを用いて下さい。<br>
設定内容は以下の通りです。(相対指定する時は、各cmakeスクリプトがあるフォルダが起点となります。）

|設定先|設定内容|
|------|--------|
|THEOLIZER_SOURCE|Theolizerのソースがあるルート・フォルダ|
|THEOLIZER_BINARY|ビルド時の中間ファイル群を置くフォルダ|
|THEOLIZER_PREFIX|Theolizerのインストール先ルート・フォルダ|
|BOOST_VERSION|使用するBoostのバージョン番号|
|REQUIRE_CMAKE_VERSION|必須CMakeバージョン|
|CC32|MingWまたはgccの32ビット版のbinフォルダのパス(既にパスが通っているなら指定不要)|
|CC64|MingWまたはgccの64ビット版のbinフォルダのパス(既にパスが通っているなら指定不要)|
|build_by_msvc()|Visual Studio 2015でビルドする時にビルドする組み合わせを指定する|
|build_by_gcc()|MinGWもしくはgccでビルドする時にビルドする組み合わせを指定する|

<b>build_by_msvc()とbuild_by_gcc()について補足</b><br>
- BUILD_DRIVERにTRUEを指定するとドライバをビルドします<br>
この時はLLVMの指定が必要です。
- BUILD_DOCUMENTにTRUEを指定するとドキュメントをビルドします<br>
この時は[Doxygen](http://www.doxygen.jp/)と[Graphviz](http://www.graphviz.org/)のインストールが必要になります。

下記のバッチ(Windowsの場合)、もしくは、スクリプト(linuxの場合)を起動することでTheolizerのビルド・自動テスト・インストールを行います。１組み合わせ当たり十数分でビルド完了します。

|linuxのスクリプト<br>Windowsのバッチ|処理内容|ログ・ファイル名|
|------------------------------------|--------|----------------|
|zy0_full_all.sh<br>zz0_full_all.bat |ビルド・フォルダを削除し作成<br>CMakeによるプロジェクト生成<br>ライブラリのビルドとテスト<br>(ドライバのビルドとテスト)<br>ドキュメントのビルド<br>ライブラリとドキュメントのインストール<br>インストール後のビルド・テスト|z3_full_test-full_all.log |
|zy1_config_all.sh<br>zz1_config_all.bat |ビルド・フォルダがなければ作成<br><b>CMakeによるプロジェクト生成</b>|z0_config.log |
|zy2_middle_all.sh<br>zz2_middle_all.bat |ビルド・フォルダがなければ作成<br>CMakeによるプロジェクト生成<br><b>ライブラリのビルドとテスト</b><br><b>(ドライバのビルドとテスト)</b><br>ドキュメントのビルド<br><b>Theolizerのインストール</b>|z3_full_test-middle_all.log |
|zy3_last_all.sh<br>zz3_last_all.bat |ビルド・フォルダがなければ作成<br>CMakeによるプロジェクト生成<br><b>インストール確認のためのビルド・テスト</b>|z3_full_test-last_all.log |

ビルド・フォルダに下記のバッチ(Windowsの場合)、もしくは、スクリプト(linuxの場合)が生成されます。必要に応じてお使い下さい。
また、Visual Studioの場合はソリューション・ファイル(Theolizer.sln)も生成されています。これをダブル・クリックすればVisual Studioが起動します。

|linuxのスクリプト<br>Windowsのバッチ|処理内容|ログ・ファイル名|
|------------------------------------|--------|----------------|
|zy0_config.sh<br>zz0_config.bat |CMakeによるプロジェクト生成|z0_config.log |
|zy1_short_test.sh<br>zz1_short_test.bat |ライブラリのビルドとテスト|z1_short_test.log |
|zy2_long_test.sh<br>zz1_short_test.bat |ドライバのビルドとテスト|z2_long_test.log |
|zy3_full_test.sh<br>zz3_full_test.bat |ライブラリのビルドとテスト<br>ドライバのビルドとテスト<br>ドキュメントのビルド<br>Theolizerのインストール<br>インストール確認のためのビルド・テスト|z3_full_test-.log |
|zy4_ja.sh<br>zz4_ja.bat |ドキュメントのビルド|z4_ja.log |
<br>

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
従来Windows版のログ・ファイルはShift-JISで出力していましたが、今回よりUTF-8へ変更しています。
詳しくは[GitHub Issue #37](https://github.com/yossi-tahara/Theolizer/issues/37#issuecomment-309667431)を参照下さい。
</div>

@subsection HowToBuildLibrary14 1-4.Boostの処理について補足
BoostのダウンロードとビルドはTHEOLIZER_BINARY/${BOOST_VERSION}配下の次のフォルダで処理します。

|フォルダ名|説明|
|----------|----|
|.|Boostのソースをダウンロードする|
|source|ダウンロードしたソースを解凍する|
|${COMPLIER}x${BIT_NUM}|Boostのビルド・フォルダ|
|install32|32ビット版のインストール先|
|install64|64ビット版のインストール先|
|install32-fPIC|32ビットfPICオプション付き版のインストール先(linuxのみ)|
|install64-fPIC|64ビットfPICオプション付き版のインストール先(linuxのみ)|

なお、Boostを既にビルド／インストールしている場合は下記のオプションを指定することで、それを使用可能です。その場合、systemとfilesystemをビルドしてください。また、linuxの場合はfPICオプション付きのビルドも必要になります。

|設定先|設定内容|
|------|--------|
|BOOST_INSTALLEDx32|インストール済Boostのフォルダを指定する(32Bit)|
|BOOST_INSTALLEDx64|インストール済Boostのフォルダを指定する(64Bit)|
|BOOST_INSTALLEDx32fPIC|インストール済Boostのフォルダを指定する(32Bit;fPICオプション付き)|
|BOOST_INSTALLEDx64fPIC|インストール済Boostのフォルダを指定する(64Bit;fPICオプション付き)|

<br>
@section HowToBuildDriver 2.Theolizerドライバのビルド方法

---

@subsection HowToBuildLibrary21 2-1.使用するツールとリンクするライブラリ
「1-1.使用するツールとリンクするライブラリ」に加えて下記が必要です。

|アイテム|条件|検証済のアイテム|
|-----|----|-----|
|LLVM|4.0.0|[LLVM 4.0.0](http://llvm.org/)|
|C++コンパイラ|C++11規格対応|[Visual Studio C++ 2015 Community update 3](https://www.microsoft.com/ja-jp/dev)|

LLVM 4.0.0はVisual C++ 2017ではビルドできません。代わりにVisual C++ 2015でビルドします。[2015と2017はバイナリ互換性があります](https://stackoverflow.com/questions/40854917/is-visual-c-2017-binary-compatible-with-vc-2015)ので、バージョン相違による問題は発生しない筈です。<br>
<br>
注）LLVM 3.8.0, 3.8.1, 3.9.0でもビルドできるよう対応していますが、Theolizer最新版でのテストは行っていません。

@subsection HowToBuildLibrary22 2-2.ビルド概要
Theolizerドライバはllvmの[libTooling](http://qiita.com/Chironian/items/6021d35bf2750341d80c)を用いていますので、ビルドされたlibToolingライブラリが必要になります。

- Windowsについて<br>
Theolizer用にプリビルドしたlibToolingを[build_libTooling](https://github.com/yossi-tahara/build_libTooling)で提供しています。Theolizerで用いるlibToolingライブラリだけを抽出しています。Theolizerドライバをビルドする時はこれをビルド用cmakeスクリプトにより自動的にダウンロードして使用します。

- Linuxについて<br>
ubuntu等linux用にはllvm公式にて[APTパッケージ](https://apt.llvm.org/)が提供されています。これらをインストールしてお使い下さい。

<b>ubuntu 16.04へllvmをインストールする手順は次の通りです。</b><br>
システム設定→ソフトウェアとアップデート→「他のソフトウェア」タブを開いて→「追加」ボタンを押し、次の内容を入力し、「ソースを追加」ボタンを押下して、APTリポジトリをubuntuへ登録して下さい。
    deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-4.0 main

その後、下記コマンドで/usr/lib/llvm-4.0/へインストールされます。

@code
$ sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 15CF4D18AF4F7421
$ sudo apt-get update
$ sudo apt-get install libclang-4.0-dev
$ sudo apt-get install llvm-4.0-dev
@endcode

@subsection HowToBuildLibrary23 2-3.ビルド用スクリプトの設定とビルド
camkeスクリプトはTheolizerライブラリのビルドに用いたもの(@ref HowToBuildLibrary13)を用います。

|設定先|設定内容|
|------|--------|
|LLVM_VERSION使用するllvmのバージョン番号|
|LLVM|llvmのダウンロード先パス(ダウンロードする時のみ指定)|
|LLVM_ROOT|インストール済llvmのパス(ダウンロードしない時のみ指定)|
|LLVM_DOWNLOAD|llvmのダウンロード元URL|
|LLVM_MSVC|build_libToolingからダウンロードするllvmをビルドしたmsvcのバージョン|
|LLVM_CC|build_libToolingからダウンロードするllvmをビルドしたMinGWのバージョン|

<br>
@section VersionNo 3.Theolizer自身のバージョン番号について

@subsection VersionManagement 3-1.バージョン番号の付け方
バージョン番号を下記のように付けて管理します。

`<Major>.<Minor>.<Detail>`<br>
`<Major>.<Minor>.<Detail>-Prerelease`<br>

|項目|更新するタイミング|
|----|------------------|
|Major|仕様や実装を大幅に追加／変更する時|
|Minor|仕様変更する時|
|Detail|バグ・フィックス、自動テスト等の周辺機能、ドキュメントを修正した時|
|-Prerease|暫定的な正規リリース前のバージョンに付加する<br>正規リリース後のPrereaseはバージョン番号を上げて行う|

@subsection NonVersionNo 3-2.コミットの管理
コミットの度にバージョン番号を上げるわけではないため、GitHubには同じバージョン番号のものが複数登録されることになります。<br>
それを判別できるようにするため、TheolizerドライバとTheolizerライブラリのそれぞれがversion.hを持ち、これにソース・コードのMD5ハッシュ値（kTheolizerSourcesHash）を記録しています。<br>
この値はそれぞれをビルドする際に自動的に更新されます。（linuxタイプの改行コードへ変換後、計算していますので、改行コードの異なるOSで処理しても同じ値になります。）

|項目|生成元ファイル群|
|----|----------------|
|Theolzierドライバ|ライブラリとドライバのソース、および、CMakeLists.txt|
|Theolzierライブラリ|ライブラリのソースとCMakeLists.txt|

Theolizerバイナリから下記方法でkTheolizerSourcesHashを得ることができます。

|項目|方法|
|----|----|
|Theolzierドライバ|TheolizerDriver --theolizer-versionにて表示されるSourcesHash|
|Theolzierライブラリ|theolizer::getVersionString()にて得ることができるSourcesHash|
<br>
<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
gitのコミット・ハッシュ値を用いて管理する方法も検討したのですが、Theolizerバイナリ生成後にコミットする手順にしたかったため、このような仕組みにしています。
</div>

<br>
@subsection GitHooks 3-3.gitのフックについて
２つフックしています。

@subsubsection commit-msg 3-3-1.commit-msgフック
これにより、上述のSourcesHash値を下記のフォーマットでコミット・メッセージへ自動追加しています。<br>
以上の仕組みによりSourcesHash値がGitHubに登録されている時にはソース・コードを特定できます。

@code
----------------- MD5 Hash Values -----------------
TheolizerDriver  : ce745e6dc35b70e87674e1daac64b739
TheolizerLibrary : 3de4f1e42c55188f88ececb53e88fc49
Library's Header : 97197bb96f6980c19c222542c8d89d54
@endcode

なお、コミット・メッセージを修正するような場合（最後のコミットをやり直すなど）は、これらをコミット・メッセージから手動で削除して下さい。

@subsubsection pre-commit 3-3-2.pre-commitフック
コミット操作した時、pre-commitフックにより、ソースのハッシュ値を計算し、ビルド時のソース・ハッシュ値と比較して、不一致ならコミットできないようにしています。<br>
これにより、コミット・メッセージに登録されるハッシュ値が操作ミスで不適切になることはない筈です。

@subsubsection InstallHooks 3-3-3.gitフックのインストールはコンフィグ時に自動インストール
CMakeの機能を使ってコンフィグ時に.git/hooksへインストールしています。

<br>
@section Documents 4.ドキュメントについて補足

---

ドキュメントの生成に[Doxygen](http://www.stack.nl/~dimitri/doxygen/)と[Graphviz](http://www.graphviz.org/)を用いています。
現在使っているバージョンは以下の通りです。

|項目|バージョン(Windows)|バージョン(Ubuntu)|
|----|-------------------|------------------|
|Doxygen|1.8.12|1.8.11|
|Graphviz|2.38.0|2.38.0|

Theolizerのドキュメントは、主にsource/reference_and_test/jaフォルダにおいています。<br>
また、ライブラリのソース・コードはsource/libraryですが、ユーザが直接使うことを想定しているもののみドキュメントを付けています。<br>

名前空間 <b>internal</b> 内のクラス等 全ての要素、および、<b>THEOLIZER_INTERNAL</b> もしくは <b>THEOLIZER_GENERATED</b>で始まるマクロは全てTheolizer内部用です。<br>
Theolizerのアップデート時、上位互換性を考慮しませんので使用しないようお願いします。

*/
