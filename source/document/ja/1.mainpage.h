//############################################################################
/*!
    @brief      ドキュメント・ファイル－概要
    @ingroup    Documents
    @file       1.mainpage.h
    @author     Yoshinori Tahara
    @date       2015/08/01 Created
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
    @mainpage  --目次--

    @subpage Abstract <br>
        @par
        @ref What <br>
        @ref Platforms <br>
        @ref Features <br>
        @ref Mechanism <br>
        @par ""

    @subpage Specification <br>
        @par
        @ref Basic <br>
        @ref Tracking <br>
        @ref Modifying <br>
        @par ""

    @subpage Prepare <br>
        @par
        @ref Install <br>
        @ref Uninstall <br>
        @par ""

    @subpage HowToMakeProject <br>
        @par
        @ref Selecting <br>
        @ref CMake <br>
        @ref Example <br>
        @ref NonCMake <br>
        @ref FileStructure <br>
        @par ""

    @subpage HowToBuildTheolizer <br>
        @par
        @ref HowToBuildLibrary <br>
        @ref HowToBuildDriver <br>
        @ref VersionNo <br>
        @ref Documents <br>
        @par ""

    @subpage UsageTotal <br>
        @par
        @ref Naming <br>
        @ref BasicUsage <br>
        @ref Serializers <br>
        @ref TestProgram <br>
        @par ""

    @subpage UsageIndividual <br>
        @par
        @ref BasicProcess <br>
        @ref ClassVariation <br>
        @ref EnumVariation <br>
        @ref ObjectTracking <br>
        @ref Destinations <br>
        @ref SupportSTL <br>
        @ref ChangingEnum <br>
        @ref ChangingClass <br>
        @ref ErrorReport <br>
        @par ""

    @subpage ApiReference <br>
        @par
        @ref u8string <br>
        @ref Unicode <br>
        @ref Test <br>
        @par ""
*/

/*!
    @defgroup Documents Sources of documents
    @defgroup TheolizerLib Theolizer Library(main)
    @defgroup SerializationStl Serialization STL
    @defgroup Unicode Unicode Helper
    @defgroup RbFor Range-based-for Helper
    @defgroup Test Test Tools
*/

/*!
    @page Abstract 概要

<br>
//############################################################################
@section What 1.Theolizerとは
//############################################################################

---

Theolizerは[C++11](https://ja.wikipedia.org/wiki/C%2B%2B11)で記述されたC++用のシリアライザです。<br>
Theolizerを使うことで次のようなプログラムの開発が容易になります。

- クラスや構造体等とその組合せで作れられた複雑なデータ構造を
- 「ファイルへ保存／回復」するプログラム
- 「通信回線で送受信」するプログラム

また、C++をベターCとしてお使いの方にも使って頂けるよう設計しておりますのでお気軽にお試し下さい。

<br>
//############################################################################
@section Platforms 2.対応環境
//############################################################################

---

マルチ・プラットフォームでC++11規格コンパイラに対応できるよう設計しています。<br>
現時点でテストできている環境は下記の通りです。

|OS|C++コンパイラ|
|--------|--------|
|Windows 10 Professional 64bit|[Visual Studio C++ 2015](https://www.visualstudio.com/ja/downloads/) Community update 3<br>[MinGW 5.4.0 32bit posix dwarf](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/5.4.0/threads-posix/dwarf/i686-5.4.0-release-posix-dwarf-rt_v5-rev0.7z/download)<br>[MinGW 5.4.0 64bit posix seh](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/5.4.0/threads-posix/seh/x86_64-5.4.0-release-posix-seh-rt_v5-rev0.7z/download)|
|ubuntu 16.04 LTS 64bit|gcc 5.4.0(Ubuntu 5.4.0-6ubuntu1~16.04.2)|

将来的にOS X + apple-llvmに対応する準備を進めています。<br>
<br>
次に、ユーザ・プログラムをビルドする際に関連するツールは以下の通りです。<br>

|その他のツール|バージョン|備考|
|--------------|----------|----|
|CMake|Visual C++は3.8.0以上<br>その他は3.5.0以上|ユーザ・プログラムビルド<br>Theolizerのビルド|
|boost|1.59.0以上|Theolizer内部使用|

CMakeは必須ではありませんが、使用することを強く推奨します。<br>
あなたのプログラムでboostを使っている場合、それがTheolizerで使っているものと異なるバージョンの時、注意が必要になります。「@ref Selecting 」を参照下さい。<br>

<br>
//############################################################################
@section Features 3.Theolizerの特長
//############################################################################

---

// ***************************************************************************
@subsection Features31 3-1.自動シリアライズ
// ***************************************************************************
C++は、クラスや構造体（以下、まとめてクラスと呼びます）に含まれるメンバのリストをプログラムがアクセスすることはできません。（メンバ1つ1つを名前を指定してアクセスすることはできますが、forループ等でメンバを次々と列挙することができないのです。）
ですので、クラスをシリアライズするためにはメンバのリストを、クラス定義とは別に改めて作成する必要がありました。<br>
Theolizerは[Clang/LLVM](http://llvm.org/)に含まれるlibTooling[libTooling](http://qiita.com/Chironian/items/6021d35bf2750341d80c)の構文解析機能を用いてクラス定義を解析し、メンバのリストを自動生成します。<br>
従って、クラス定義を変更した時もメンバ・リストへの反映漏れが無く、プログラム開発が捗ります。

// ***************************************************************************
@subsection Features32 3-2.保存先指定
// ***************************************************************************
１つのクラスに属するメンバ変数を保存する時、複数の異なるファイルへ保存したい時もあると思います。一部のメンバ変数は設定ファイルへ保存し、また別のメンバはデータ・ファイルへ保存したいというケースは少なくないと思います。また、一部のメンバ変数（ファイル・ハンドル等）は保存しないことも多いでしょう。<br>
<br>
Theolizerはメンバ変数に保存先や保存の有無を指定することでクラスを分割して異なるファイルへ保存したり、一部を通信で他のプログラムへ送信したりすることができます。

// ***************************************************************************
@subsection Features33 3-3.オブジェクト追跡
// ***************************************************************************
一般にポインタをファイルへ保存し、プログラムを再起動した後でそのポインタを回復しても意味はありません。ポイント先のメモリ・アドレスはプログラムの再起動前後で異なることが一般的だからです。<br>
<br>
Theolizerはポイント先のデータも一緒にファイルへ保存するのであればオブジェクト追跡によりポインタを有意味に回復することができます。もちろん通信時も同様です。<br>
また、ポイント先がクラスの場合、その各メンバに保存先指定することができますので全てを保存する必要はありません。<br>
これにより複雑なデータ構造を容易に保存／回復・送受信できます。<br>
オブジェクト追跡機能はboost::serializationを参考に開発しました。

// ***************************************************************************
@subsection Features34 3-4.プログラム変更対応
// ***************************************************************************
シリアライザを使って保存するということは、内部データの構造を保ったままファイルへ保存するということです。内部データ構造はプログラムの変更に伴い変更されるため、旧プログラムで保存されたファイルを回復できるようにするためには内部データ構造の変更に強い制限がかかります。<br>
<br>
Theolizerはその制限を大幅に緩和し、古いプログラムで保存したデータを新しいプログラムで回復できます。<br>
更に、新しいプログラムで古いプログラムが読めるデータを保存するプログラムを開発することも可能です。<br>
<br>
クラス(class/struct)とenum型の定義を変更するに際して、2種類の変更対応方法を用意しています。

1. <b>アップデート</b><br>
バージョン番号を変更しない方法です。

2. <b>バージョン・アップ</b><br>
クラスやenum型に割り当てた（ローカル)バージョン番号を１つ上げて対応する方法です。

それぞれ特徴があります。

|方式|特徴|
|----|----|
|アップデート|1.変更が容易です。<br>2.自動生成するソース・コードが増えないためコンパイラへの負荷が軽いです。<br>3.古いデータを新しいプログラムで回復できますが、逆はサポートしていません。|
|バージョン・アップ|1.より大幅な変更に対応できます。<br>2.旧バージョン形式のデータを保存することができます。<br>3.変更作業は少し手間がかかります。<br>4.旧バージョンの変数リストやシンボル・リストを保持するため、<br>バージョン番号を上げる度に自動生成されるソース・コードが増えます。|

可能な時はアップデートにて変更対応し、それでは不足の場合にバージョン・アップすることがお薦めです。<br>
<br>
まとめると次のような特長があります。

1. <b>クラス定義、enum定義の変更（アップデート／バージョンアップ）</b><br>
  - クラスのメンバ変数の追加／削除／順序変更に対応しています。<br>
  - enum型のシンボルの追加／削除／定義順序変更に対応しています。<br>
  また、シンボル名で保存／回復する時はシンボル値を、シンボル値保存の時はシンボル名を自由に変更できます。<br>
<br>

2. <b>クラスについてバージョン・アップ／ダウン処理を記述できます</b><br>
  - クラスをバージョン・アップする際に行いたい処理を記述できます。<br>
例えば、変数にオフセットを追加した時、そのオフセットを加える処理などです。<br>
  - バージョン・ダウン処理を定義できます。<br>
例えば、オフセットを引く処理などです。<br>
  - クラスは名前対応と順序対応を、enum型はシンボル名保存とシンボル値保存を切り替えることができます。<br>
開発当初は名前対応／シンボル名保存にて変更しやすくしておき、安定したら順序対応／シンボル値保存として効率アップすることが可能です。更に元へ戻すこともできます。<br>
<br>

3. <b>バージョン・アップ／ダウンをカスケードに処理します</b><br>
例えば、Ver.4をVer.5へバージョンアップした時、従来はVer.4→Ver.5のプログラム開発だけでなく、Ver.1～Ver.3→Ver.4のプログラムをVer.1～Ver.3→Ver.5へ改造する必要がありました。Theolizerはバージョン・アップ／ダウン処理をカスケード(Ver.1→Ver.2→Ver.3→Ver.4→Ver.5)に行いますので、Ver.4→Ver.5のプログラムを開発するだけで済みます。<br>
これによりバージョンが上がった時のプログラム開発を大きく低減できます。(注)<br>
<br>

4. <b>旧バージョン形式のデータを保存することができます</b><br>
保存したい形式のバージョン番号を指定することで、新しいバージョンのプログラムが古いバージョンのプログラムが保存する形式でデータ保存可能です。<br>
なお、指定された古いバージョンに複数のアップデートが含まれる場合は最後のアップデート版が保存されます。

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
<b>（注）バージョン・アップ／ダウンにおける不可逆性に注意</b><br>
Theolizerのバージョン・ダウン機能は旧プログラムで回復可能なデータを保存することを目的としています。<br>
<br>
バージョン・アップする時はenum値や変数の追加を行うケースが多いですが、古い形式のデータを保存する時には、その追加されたデータを削除します。（古いバージョンのプログラムはその追加されたデータの存在を知らないため、もしも残すと古いプログラムが処理できなくなるためです。）
なので、新バージョン・プログラムが保存した<b>旧バージョン</b>・データを、再度新バージョン・プロクラムで回復した時、元のデータに戻ることを本質的に期待できません。<br>
</div>



<br>
//############################################################################
@section Mechanism 4.Theolizerの仕組み
//############################################################################

---
　Theolizerはドライバとライブラリの２つで構成されています。

| 項目 | 内容 |
|--------|--------|
|ドライバ|あなたのソースを解析して、シリアライズ処理に必要なコードを自動生成します。|
|ライブラリ|あなたのプログラムとリンクして、シリライズ処理を行います。|

// ***************************************************************************
@subsection Mechanism41 4-1.ドライバ
// ***************************************************************************
<b>Theolizerドライバ</b>はあなたのビルド・プロジェクト（makeやVisual Studio）とコンパイラの間に割り込み、下記を自動的に行います。

1. ビルド・プロジェクトの指示に従ってあなたのソース・コードを解析します。<br>
2. シリアライズ／デシリアライズ処理を行うために必要なソース・コード（主にクラス・メンバのリスト）を自動生成します。<br>
C++ソース・コードの解析には、[Clang/LLVM](http://llvm.org/)に含まれるlibToolingを用いて構文解析を行っています。
3. 通常のコンパイラとリンカがあなたのプログラムと自動生成されたソース・コードをビルドし、<b>Theolizerライブラリ</b>をリンクします。<br>

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
<b>自動生成するソースのファイル名</b><br>
自動生成するソース・ファイル名は、コンパイルするソースファイル名.theolizer.hppです。
例えば、main.cppの場合、main.cpp.theolizer.hppとなります。
</div>

// ***************************************************************************
@subsection Mechanism42 4-2.ライブラリ
// ***************************************************************************
<b>Theolizerライブラリ</b>はあなたのプログラムとリンクして、データ構造のシリアライズとデシリアライズを行います。

// ***************************************************************************
@subsection Mechanism43 4-3.Theolizerドライバが割り込む仕組み
// ***************************************************************************

ビルド・プロジェクトとコンパイラの間にTheolizerドライバを割り込む仕組みのポイントは次の３点です。

1. ビルド・システムにTheolizerドライバをコンパイラとして起動させる
2. Theolizerドライバにソース自動生成が必要であることを伝達し、自動生成処理する
3. Theolizerドライバに元コンパイラのパスを伝達し、元のコンパイラへ処理を引き継ぐ

@subsubsection Mechanism431 4-3-1. Theolizerドライバをコンパイラとして起動させる

#### 4-3-1-1.Visual Studioの場合

- Theolizerインストール時に準備<br>
Theolizerのインストール先フォルダにmsbuild-binフォルダを追加し、そこへTheolizerドライバをcl.exeとして保存します。

- ビルド・プロジェクト生成

CMake 3.8.0にて[VS_USER_PROPS](https://cmake.org/cmake/help/v3.8/prop_tgt/VS_USER_PROPS.html)と言うVC++のプロジェクトへインクルード指定を追加するプロパティが追加されました。<br>
これを使ってターゲットのプロジェクト・ファイル（*.vcxproj）に`<Import Project>`キーを追加し、theolizer.propsをインクルードするようにしました。<br>
そして、`theolizer.props`にて、コンパイル時に使われる`PATH`を指定する`<ExecutablePath>`の先頭に上記`msbuild-bin`フォルダを追加しました。<br>

以上により、Theolizerを使用するプロジェクトにおいてTheolizerドライバがコンパイラとして起動します。

#### 4-3-1-2.その他（gcc/mingw)の場合
対象のプロジェクトのCMAKE_CXX_COMPILERにTheolizerドライバのフルパスを設定しました。<br>
これはmakefileのコンパイラに反映されますので、呼び出すコンパイラを変更できます。<br>
（なお、Visual Studioはmakeではなくマイクロソフト独自のMSBuildでビルドするためこの方法が機能しません。）<br>

@subsubsection Mechanism432 4-3-2.Theolizerドライバにソース自動生成が必要であることを伝達する

CMakeで`THEOLIZER_DO_PROCESS`マクロをコンパイラ・オプションで定義するようにしまた。
このマクロがオプションで指定された時、Theolizerドライバはソース自動生成処理を行います。
そうでない時は、Theolizer関連オプションを除く全ての引数で元コンパイラを起動（パススルー）します。

@subsubsection Mechanism433 4-3-3.Theolizerドライバに元コンパイラのパスを伝達する

#### 4-3-3-1.Visual Studioの場合
CMakeは*.rspファイルに全てのオプションを記述してコンパイラへ渡します。その為、Theolizerドライバでそのオプションを削除して元コンパイラへ引き継ぐことが難しいです。しかし、削除しないとcl.exeが「知らないオプション」として警告します。
そこで、マクロ定義（`/Dtheolizer_original_compiler=<元コンパイラのパス>`）で伝達することで警告表示されないようにしました。マクロ定義はリンカへ伝達する必要がないので伝わらないようです。

#### 4-3-3-2.その他（gcc/mingw)の場合
CMakeはgcc/mingw用にはコンパイルとリンクを分解してg++を呼び出します。
リンクの時ldを直接呼ぶわけでなく、g++にリンク指定して呼び出します。
どちらの場合もTheolizerドライバが起動されることになります。
そして、リンク動作時は`-D`オプションは渡ってきません。マクロ定義はコンパイラのみ必要なオプションだからだと思います。
しかし、その結果、リンク時に起動されたTheolizerドライバが元コンパイラのパスを得ることができず、元コンパイラをパススルー起動できません。

そこで、msvc以外の時は`--theolizer_original_compiler=<元コンパイラのパス>`オプションで元コンパイラのパスを伝達するようにしました。

@subsubsection Mechanism434 4-3-4. 使い方の変更について
上記はfind_packageで用いるTHEOLIZERConfig.cmake内部で対応しました。ですので、ユーザ・プログラム用のCMakeLists.txtは従来のままで使用できます。従来必要であったコンパイラのリプレース操作が不要になります。<br>

なお、Visual Studioをお使いの場合は、CMakeを3.8.0以上へアップデートする必要があります。

*/
