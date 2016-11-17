//############################################################################
/*!
    @brief      ドキュメント・ファイル－概要
    @ingroup    Documents
    @file       1.mainpage.h
    @author     Yoshinori Tahara
    @date       2015/08/01 Created
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
    @mainpage  --目次--

    @subpage Abstract \n
        @par
        @ref What \n
        @ref Platforms \n
        @ref Features \n
        @ref Mechanism \n
        @par ""

    @subpage Specification \n
        @par
        @ref Basic \n
        @ref Tracking \n
        @ref Modifying \n
        @par ""

    @subpage Prepare \n
        @par
        @ref Install \n
        @ref Uninstall \n
        @par ""

    @subpage HowToMakeProject \n
        @par
        @ref Selecting \n
        @ref CMake \n
        @ref Example \n
        @ref NonCMake \n
        @ref FileStructure \n
        @par ""

    @subpage HowToBuildTheolizer \n
        @par
        @ref HowToBuildLibrary \n
        @ref HowToBuildDriver \n
        @ref VersionNo \n
        @ref Documents \n
        @par ""

    @subpage UsageTotal \n
        @par
        @ref Naming \n
        @ref TestProgram \n
        @ref BasicUsage \n
        @ref Serializers \n
        @par ""

    @subpage UsageIndividual \n
        @par
        @ref BasicProcess \n
        @ref ClassVariation \n
        @ref EnumVariation \n
        @ref ObjectTracking \n
        @ref UsageSTL \n
        @ref ChangingClass \n
        @ref ChangingEnum \n
        @par ""

    @subpage ApiReference \n
        @par
        T.B.D.\n
        @ref u8string \n
        @ref Unicode \n
        @par ""
*/

/*!
    @defgroup TheolizerDriver Sources of TheolizerDriver
    @defgroup TheolizerLib Sources of TheolizerLib/Test
    @defgroup Documents Sources of documents
*/

/*!
    @page Abstract 概要

<br>
//############################################################################
@section What 1.Theolizerとは
//############################################################################

---

Theolizerは[C++11](https://ja.wikipedia.org/wiki/C%2B%2B11)で記述されたC++用のシリアライザです。<br>
Theolizerを使うことで、クラスや構造体を「ファイルへ保存／回復」、「通信回線で送受信」するプログラムの開発が容易になります。  <br>
C++をベターCとしてお使いの方にも使って頂けるよう設計しておりますのでお気軽にお試し下さい。

__「シリアライズ」という用語について__<br>
シリアライズ(Serialize)の逆はデシリアライズ(Deserialize)なのですが、この資料ではデシリアライズも含めて「シリアライズ」と呼んでいます。<br>
シリアライザも同様にデシリアライザを含めて呼んでいます。<br>
両方を含めた用語が欲しいため、このようにしました。ご了承下さい。

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

将来的にOS X El Capitan + apple-llvmに対応する準備を進めています。
<br>
ユーザ・プログラムをビルドする際に関係あるツールは以下の通りです。

|その他のツール|バージョン|備考|
|--------------|----------|----|
|CMake|3.5.0以上|ユーザ・プログラムビルド<br>Theolizerのビルド|
|boost|1.59.0以上|Theolizer内部使用|

CMakeは必要ではありませんが、これがあるとプロジェクト生成が楽になります。<br>
あなたのプログラムでboostを使っている場合には注意が必要です。
@ref Selecting を参照下さい。

<br>
//############################################################################
@section Features 3.Theolizerの特長
//############################################################################

---

// ***************************************************************************
@subsection Features31 3-1.自動シリアライズ
// ***************************************************************************
C++は、クラスや構造体（以下、まとめてクラスと呼びます）に含まれるメンバのリストをプログラムがアクセスすることはできません。
ですので、クラスをシリアライズするためにはメンバのリストを、クラス定義とは別に改めて作成する必要がありました。<br>
Theolizerは[Clang/LLVM](http://llvm.org/)に含まれるlibTooling[libTooling](http://qiita.com/Chironian/items/6021d35bf2750341d80c)の構文解析機能を用いてクラス定義を解析し、メンバのリストを自動生成します。
従って、クラス定義を変更した時もメンバ・リストへの反映漏れが無く、プログラム開発が捗ります。

// ***************************************************************************
@subsection Features32 3-2.保存先指定
// ***************************************************************************
クラスのメンバ変数を保存する時、異なるファイルへ保存したい時もあると思います。一部のメンバ変数（ファイル・ハンドル等）は保存しないでしょうし、一部のメンバ変数は設定ファイルへ保存し、また別のメンバはデータ・ファイルへ保存したいというケースは少なくないと思います。<br>
Theolizerはメンバ変数に保存先を指定することで、クラスを分割して異なるファイルへ保存したり、一部を通信で他のプログラムへ送信したりすることができます。

// ***************************************************************************
@subsection Features33 3-3.オブジェクト追跡
// ***************************************************************************
一般に、ポインタをファイルへ保存し、プログラムを再起動した後で回復しても意味はありません。<br>
Theolizerはポイント先を一緒にファイルへ保存してオブジェクト追跡することでポインタを回復することができます。もちろん通信時も同様です。<br>
また、ポイント先を保存する際にも、保存先指定することができますので全てを保存する必要はありません。<br>
これにより複雑なデータ構造を容易に保存／回復・送受信できます。<br>
この機能はboost::serializationを参考に開発しました。

// ***************************************************************************
@subsection Features34 3-4.プログラム変更対応
// ***************************************************************************
シリアライザを使って保存するということは、内部データの構造を保ったままファイルへ保存するということです。内部データ構造はプログラムの変更に伴い変更されるため、旧プログラムで保存されたファイルを回復できるようにするためには内部データ構造の変更に強い制限がかかります。<br>
Theolizerはその制限を大幅に緩和しました。

- **クラス定義、enum定義の変更**<br>
クラスのメンバ変数の追加／削除／順序変更に対応しています。<br>
enum型のシンボルの追加／削除、および、シンボル名で保存／回復するのでシンボル値も自由に変更できます。

- **クラスについてバージョン・アップ／ダウン処理を記述可能**<br>
クラスをバージョン・アップする際に行いたい処理を記述できます。<br>
（例えば、変数にオフセットを追加した時、そのオフセットを加える処理など）<br>
また、旧バージョン形式で保存したい場合はバージョン・ダウン処理を定義することもできます。<br>
（例えば、オフセットを引く処理など）

- **バージョン・アップ／ダウンをカスケードに処理します**<br>
例えば、Ver.4をVer.5へバージョンアップした時、従来はVer.4→Ver.5のプログラム開発だけでなく、Ver.1～Ver.3→Ver.4のプログラムをVer.1～Ver.3→Ver.5へ改造する必要がありました。<br>
Theolizerはバージョン・アップ／ダウン処理をカスケード(Ver.1→Ver.2→Ver.3→Ver.4→Ver.5)に行いますので、Ver.4→Ver.5のプログラムを開発するだけで済みます。<br>
これによりバージョンが上がった時のプログラム開発を大きく低減できます。

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
<b>ドライバ動作の補足</b><br>
Theolizerドライバは、THEOLIZER_ANALYZEマクロを定義するコマンドライン・オプション(例えば、-DTHEOLIZER_ANALYZE)とともに起動された時のみ、上記の1～3を行います。このオプション無しで起動された時は何もせずにそのまま元のコンパイラへパス・スルーしますので、Theolizerを用いないプロジェクトへの影響は事実上ありません。<br>
Windows 10、および、ubuntu 16.04にて、コンパイラを置き換えたままCMakeのジェネレーション、および、llvmのビルドができることを確認しております。
</div>

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
<b>自動生成するソースのファイル名</b><br>
自動生成するソース・ファイル名は、コンパイルするソースファイル名.theolizer.hppです。
例えば、main.cppの場合、main.cpp.theolizer.hppとなります。
</div>

// ***************************************************************************
@subsection Mechanism42 4-2.ライブラリ
// ***************************************************************************
<b>Theolizerライブラリ</b>はあなたのプログラムとリンクして、データ構造のシリアライズとデシリアライズを行います。

*/
