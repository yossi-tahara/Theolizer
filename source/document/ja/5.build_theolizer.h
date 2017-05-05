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

便利のためにcmakeスクリプトを用意しています。<br>
Theolizerのソースのbuild/boost_buildフォルダにおいてます。Windowsでビルドする場合はwindows.cmakeを、linuxでビルドする場合linux.cmakeを用いて下さい。<br>
下記のように使います。<br>

1. 作業に用いるフォルダを作成して下さい。<br>
    例えば、boost1.59.0をビルドする場合、boost-159のようにboostのバージョン番号を反映したフォルダ名をお勧めします。（以下作業フォルダをboost-159と仮定して説明します。）<br>
<br>

2. そのフォルダ(boost-159)の下へ使いたいバージョンの[boost](http://www.boost.org/users/history/)をダウンロードして解凍して下さい。（フォルダ名をboostへ変更しておくことをお勧めします。）<br>
    この作業はオプションです。後述のBOOST_DOWNLOADを有効にすれば自動的にダウンロードします。<br>
    ただし、Windows上にインストールしたVirtualBox上のubuntuではダウンロードに成功しませんでした。<br>
    そのような場合は手動でダウンロードし解凍して下さい。<br>
<br>

3. そのフォルダ(boost-159)の下へboost_buildフォルダをコピーして下さい。<br>
<br>

4. 下記の<b>設定表</b>に従って、cmakeスクリプトのオプションを設定して下さい<br>
<br>

5. そして、Windowsならzz_full_all.bat、linuxならzz_full_all.shを起動すればビルドとインストールされます。<br>
    １組み合わせ当たり数分でビルド完了します。<br>
<br>

6. ビルド完了時のフォルダ構成<br>
    boost-159                   ダウンロードした圧縮ファイル
    boost-159/boost             boostのソース・ファイル群<br>
    boost-159/boost_build       ビルド用スクリプト群<br>
    boost-159/boost_build/build ビルド・フォルダ（ビルド・ログや中間ファイル群)<br>

<b>設定表</b>

|設定先|設定内容|
|------|--------|
|BOOST_DOWNLOAD|設定した場合、boostを自動ダウンロードします。<br>ダウンロードするバージョン番号を設定して下さい。例："1.59.0"|
|BOOST_SOURCE|boostのソースのフォルダ|
|BOOST_PREFIX|boostのインストール先フォルダ名のプリフィクス|
|CC32|MingWまたはgccの32ビット版のbinフォルダのパス(既にパスが通っているなら指定不要)|
|CC64|MingWまたはgccの64ビット版のbinフォルダのパス(既にパスが通っているなら指定不要)|
|build_process()|ビルドする組み合わせを指定|

#### BOOST_DOWNLOADの補足
BOOST_DOWNLOADを設定し、かつ、BOOST_SOURCEが存在しない時のみ自動ダウンロードします。<br>
既にダウンロードが完了しているのに再度処理することを回避するためです。

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

下記手順でTheolizerドライバをビルドします。

1. boostのsystemとfilesystemをビルドしてインストール
2. LLVM(libTooling含む)をビルドしてインストール
3. Theolizerドライバをビルドしてインストール

@subsubsection HowToBuildLibrary221 2-2-1.boostのsystemとfilesystemをビルドしてインストール

@ref HowToBuildLibrary121 と同じなので省略します。

@subsubsection HowToBuildLibrary222 2-2-2.LLVM(libTooling含む)をビルドしてインストール
コンパイラはTheolizerをビルドするものと同じものを使用して下さい。

boostのビルドと同様、llvmをビルドするためのcmakeスクリプトを用意しています。<br>
Theolizerのソースのbuild/llvm_buildフォルダにおいてます。Windowsでビルドする場合はwindows.cmakeを、linuxでビルドする場合linux.cmakeを用いて下さい。<br>
下記のように使います。<br>

1. 作業に用いるフォルダを作成して下さい。<br>
    例えば、llvm3.9.0をビルドする場合、llvm-390のようにllvmのバージョン番号を反映したフォルダ名をお勧めします。（以下作業フォルダをllvm-390と仮定して説明します。）<br>
<br>

2. そのフォルダ(llvm-390)の下へ使いたいバージョンの[llvm](http://llvm.org/releases/download.html)をダウンロードして解凍して下さい。<br>
    下記の<b>ダウンロード表</b>に従ってダウンロードし解凍して下さい。<br>
    この作業はオプションです。後述のLLVM_DOWNLOADを有効にすれば自動的にダウンロードします。<br>
    llvmはboostの場合と異なり、ubuntu環境からも正常にダウンロードできました。<br>
<br>

3. そのフォルダ(llvm-390)の下へllvm_buildフォルダをコピーして下さい。<br>
<br>

4. 下記の<b>設定表</b>に従って、cmakeスクリプトのオプションを設定して下さい<br>
<br>

5. そして、Windowsならzz_full_all.bat、linuxならzz_full_all.shを起動すればビルドとインストールされます。<br>
    １組み合わせ当たり<b>１～２時間かかります</b>ので、時間に余裕のある時にビルドして下さい。<br>
<br>

6. ビルド完了時のフォルダ構成<br>
    llvm-390                    ダウンロードした圧縮ファイル群
    llvm-390/llvm               llvmのソース・ファイル群<br>
    llvm-390/llvm_build         ビルド用スクリプト群<br>
    llvm-390/llvm_build/build   ビルド・フォルダ（ビルド・ログや中間ファイル群)<br>

<b>ダウンロード表</b>

|ダウンロードするアイテム|ダウンロードするファイル名|解凍後のフォルダ名|
|------------------------|--------------------------|------------------|
|LLVM source code|llvm-3.9.0.src.tar.xz|llvm|
|Clang source code|cfe-3.9.0.src.tar.xz|llvm/tools/clang|
|clang-tools-extra|clang-tools-extra-3.9.0.src.tar.xz|llvm/tools/clang/tools/extra|

<b>設定表</b>

|設定先|設定内容|
|------|--------|
|LLVM_DOWNLOAD|設定した場合、必要なllvmを自動ダウンロードします。<br>ダウンロードするバージョン番号を設定して下さい。例："3.9.0"|
|LLVM_PREFIX|LLVMのインストール先パス名のプリフィックス|
|CC32|MingWまたはgccの32ビット版のbinフォルダのパス(既にパスが通っているなら指定不要)|
|CC64|MingWまたはgccの64ビット版のbinフォルダのパス(既にパスが通っているなら指定不要)|
|MAKE|[jom](https://wiki.qt.io/Jom)等の高速なmakeを使う場合にそのフルパス(標準のmakeを使う時は指定不要)|
|build_process()|ビルドする組み合わせを指定|

@subsubsection HowToBuildLibrary223 2-2-3.Theolizerドライバをビルドしてインストール

@ref HowToBuildLibrary122 と同じなので省略します。

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
