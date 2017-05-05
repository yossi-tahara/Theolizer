### -- 目次 --
[1.Theolizerとは](#About)  
[2.対応環境](#Platforms)  
[3.Theolizerの特長](#Features)  
[4.Theolizerの仕組み](#Mechanism)  
[5.サンプル・ソース](#Example)  
[6.Theolizerの開発状況について](#Status)  
[7.不具合報告を頂く時のお願い](#DefectReport)  

### -- 自動テスト --
Theolizerは信頼性を上げるために自動テストを実装しています。

- CIサービスによるサブセット・テスト（masterブランチの最新版）の状態<br>
Travis CIにてubuntu環境のサブセット・テストをpush毎に自動実行しています。<br>
Travis CI result : [![Build Status](https://travis-ci.org/yossi-tahara/Theolizer.svg?branch=master)](https://travis-ci.org/yossi-tahara/Theolizer) "for StaticWithBoost without sudo on ubuntu 14.04 LTS"

- ローカル環境によるフルセット・テストについて<br>
バージョン番号に`-Prerelease`サフィックスのないものはフルセットのテストを完了しています。<br>
フルセット・テストにより、プリビルド版を生成してvX.Y.Zのタグをつけて当リポジトリの[relases](https://github.com/yossi-tahara/Theolizer/releases)へ登録しています。

### -- プリビルド版 --
Windowsとubuntu用のプリビルド版を用意しています。<br>
当リポジトリの[relases](https://github.com/yossi-tahara/Theolizer/releases)からダウンロードして下さい。

### -- 詳細ドキュメント --
詳細なドキュメントを[http://doc.theolizer.com](http://doc.theolizer.com)に置いています。<br>
また、Theolizerをインストールした時、インストール先のdocフォルダにもあります。

___
<h1 id="About">1.Theolizerとは</h1>

Theolizerは[C++11](https://ja.wikipedia.org/wiki/C%2B%2B11)で記述されたC++用のシリアライザです。<br>
Theolizerを使うことで、クラスや構造体を「ファイルへ保存／回復」、「通信回線で送受信」するプログラムの開発が容易になります。  <br>
C++をベターCとしてお使いの方にも使って頂けるよう設計しておりますのでお気軽にお試し下さい。

現在、オープンβとして公開しております。

<br>
<h1 id="Platforms">2.対応環境</h1>

マルチ・プラットフォームとC++11規格コンパイラに対応できるよう設計しています。<br>
現時点でテストできている環境はWindowsとubuntuです。

|OS|C++コンパイラ|
|--------|--------|
|Windows 10 Professional 64bit|[Visual Studio C++ 2015 Community update 3](https://www.microsoft.com/ja-jp/dev)<br>[MinGW 5.4.0 32bit posix dwarf](https://sourceforge.net/projects/mingw-w64/)<br>[MinGW 5.4.0 64bit posix seh](https://sourceforge.net/projects/mingw-w64/)|
|ubuntu 16.04 LTS 64bit|gcc 5.4.0(Ubuntu 5.4.0-6ubuntu1~16.04.2)|

将来的にOS X + apple-llvmに対応する準備を進めています。

<br>
ユーザ・プログラムをビルドする際に関係するツールは以下の通りです。

|その他のツール|バージョン|備考|
|--------------|----------|----|
|CMake|3.5.0以上|ユーザ・プログラムビルド<br>Theolizerのビルド|
|boost|1.59.0以上|Theolizer内部使用|

CMakeは必要ではありませんが、これがあるとプロジェクト生成が楽になります。<br>
あなたのプログラムでboostを使っている場合には注意が必要です。
[1.Theolizerライブラリの選択](http://doc.theolizer.com/html/_how_to_make_project.html#Selecting)を参照下さい。

<br>
<h1 id="Features">3.Theolizerの特長</h1>

## 3-1.自動シリアライズ
C++は、クラスや構造体（以下、まとめてクラスと呼びます）に含まれるメンバのリストをプログラムがアクセスすることはできません。
ですので、クラスをシリアライズするためにはメンバのリストを、クラス定義とは別に改めて作成する必要がありました。<br>
Theolizerは[Clang/LLVM](http://llvm.org/)に含まれるlibTooling[libTooling](http://qiita.com/Chironian/items/6021d35bf2750341d80c)の構文解析機能を用いてクラス定義を解析し、メンバのリストを自動生成します。
従って、クラス定義を変更した時もメンバ・リストへの反映漏れが無く、プログラム開発が捗ります。

## 3-2.保存先指定
クラスのメンバ変数を保存する時、異なるファイルへ保存したい時もあると思います。一部のメンバ変数（ファイル・ハンドル等）は保存しないでしょうし、一部のメンバ変数は設定ファイルへ保存し、また別のメンバはデータ・ファイルへ保存したいというケースは少なくないと思います。<br>
Theolizerはメンバ変数に保存先を指定することで、クラスを分割して異なるファイルへ保存したり、一部を通信で他のプログラムへ送信したりすることができます。

## 3-3.オブジェクト追跡
一般に、ポインタをファイルへ保存し、プログラムを再起動した後で回復しても意味はありません。<br>
Theolizerはポイント先を一緒にファイルへ保存してオブジェクト追跡することでポインタを回復することができます。もちろん通信時も同様です。<br>
また、ポイント先を保存する際にも、保存先指定することができますので全てを保存する必要はありません。<br>
これにより複雑なデータ構造を容易に保存／回復・送受信できます。<br>
この機能はboost::serializationを参考に開発しました。

## 3-4.プログラム変更対応
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
<h1 id="Mechanism">4.Theolizerの仕組み</h1>

　Theolizerはドライバとライブラリの２つで構成されています。

| 項目 | 内容 |
|--------|--------|
|ドライバ|あなたのソースを解析して、シリアライズ処理に必要なコードを自動生成します。|
|ライブラリ|あなたのプログラムとリンクして、シリライズ処理を行います。|

## 4-1.ドライバ
<b>Theolizerドライバ</b>はあなたのビルド・プロジェクト（makeやVisual Studio）とコンパイラの間に割り込み、下記を自動的に行います。

1. ビルド・プロジェクトの指示に従ってあなたのソース・コードを解析します。<br>
2. シリアライズ／デシリアライズ処理を行うために必要なソース・コード（主にクラス・メンバのリスト）を自動生成します。<br>
C++ソース・コードの解析には、[Clang/LLVM](http://llvm.org/)に含まれるlibToolingを用いて構文解析を行っています。
3. 通常のコンパイラとリンカがあなたのプログラムと自動生成されたソース・コードをビルドし、<b>Theolizerライブラリ</b>をリンクします。<br>

> <b>ドライバ動作の補足</b><br>
> Theolizerドライバは、THEOLIZER_ANALYZEマクロを定義するコマンドライン・オプション(例えば、-DTHEOLIZER_ANALYZE)とともに起動された時のみ、上記の1～3を行います。このオプション無しで起動された時は何もせずにそのまま元のコンパイラへパス・スルーしますので、Theolizerを用いないプロジェクトへの影響は事実上ありません。
> Windows 10、および、ubuntu 16.04にて、コンパイラを置き換えたままCMakeのジェネレーション、および、llvmのビルドができることを確認しております。

<br>
> <b>自動生成するソースのファイル名</b><br>
> 自動生成するソース・ファイル名は、コンパイルするソースファイル名.theolizer.hppです。
> 例えば、main.cppの場合、main.cpp.theolizer.hppとなります。

## 4-2.ライブラリ
<b>Theolizerライブラリ</b>はあなたのプログラムとリンクして、データ構造のシリアライズとデシリアライズを行います。

<br>
<h1 id="Example">5.サンプル・ソース</h1>

### example.h
```c++
//############################################################################
//      Theolizer紹介用サンプル・プログラム
//############################################################################

#if !defined(EXAMPLE_H)
#define EXAMPLE_H

// ***************************************************************************
//      インクルード
// ***************************************************************************

#include <string>
#include <theolizer/serializer_json.h>

// ***************************************************************************
//      型定義
// ***************************************************************************

enum EnumType
{
    None,
    EnumA,
    EnumB,
    EnumC
};

struct StructType
{
    EnumType    mEnum;
    int         mInt;
    std::string mString;
    StructType() : mEnum(None), mInt(0), mString("") { }
};

#endif  // EXAMPLE_H
```
### example.cpp
```c++
//############################################################################
//      Theolizer紹介用サンプル・プログラム
//############################################################################

// ***************************************************************************
//      インクルード
// ***************************************************************************

#include <fstream>
#include "example.h"
#include "example.cpp.theolizer.hpp"                // Theolizer自動生成先

// ***************************************************************************
//      メイン
// ***************************************************************************

int main(int argc, char* argv[])
{
//----------------------------------------------------------------------------
//      保存
//----------------------------------------------------------------------------

    try
    {
        // データを生成する
        StructType aStructType;
        aStructType.mEnum   = EnumB;
        aStructType.mInt    = 1001;
        aStructType.mString = u8"ＵＴＦ－８　ｓｔｒｉｎｇ";

        std::ofstream   aStream("example.json");    // 保存先のファイルをオープンする
        theolizer::JsonOSerializer<> js(aStream);   // シリアライザを用意する
        THEOLIZER_PROCESS(js, aStructType);         // example.jsonファイルへ保存する
    }
    catch(theolizer::ErrorInfo& e)
    {
        std::cerr << e.getString() << "\n";
return 1;
    }

//----------------------------------------------------------------------------
//      回復
//----------------------------------------------------------------------------

    try
    {
        StructType aStructType;                     // データ領域を獲得する
        std::ifstream   aStream("example.json");    // 回復元のファイルをオープンする
        theolizer::JsonISerializer<> js(aStream);   // シリアライザを用意する
        THEOLIZER_PROCESS(js, aStructType);         // example.jsonファイルから回復する

        // 回復結果を表示する
        std::cout <<theolizer::print
                    (
                        u8"mEnum=%d mInt=%d mString=[%s]\n",
                        aStructType.mEnum,
                        aStructType.mInt,
                        aStructType.mString
                    );
    }
    catch(theolizer::ErrorInfo& e)
    {
        std::cerr << e.getString() << "\n";
return 2;
    }

    return 0;
}
```

<br>
<h1 id="Status">6.Theolizerの開発状況について</h1>

現在、Theolizerはオープンβ中です。<br>
自動テスト実装、および、ドキュメント記述完了によりオープンβを終了し、正式リリースの予定です。

- 仕様について<br>
正規公開後は上位互換を維持するよう仕様変更は注意深く行います。（既存の機能テストを修正しないことが目標）<br>
オープンβ期間中は、他の方にも使って頂いて現仕様の問題を洗い出し、可能な部分を修正したいと思います。<br>
そのため、オープンβ中は互換性より使い勝手や性能を優先して仕様変更する予定です。

- 機能の実装完了<br>
コア機能は実装完了しています。STLコンテナ対応もv0.4.3にて対応完了しました。<br>
これにより正規リリースに向けた機能の実装は完了しました。

- 自動テスト実装中<br>
自動テストの実装を進めています。ライブラリは網羅的な自動テストをほぼ実装しています。<br>
ドライバの自動テストの実装を進めています。

- [ドキュメント作成中](http://doc.theolizer.com)<br>
自動テストの実装と同時に記述を進めています。

自動テストは主に機能テスト（外部APIのテスト）を実装します。（機能テストでは不十分な部分について一部内部テストも実装します。）  
適切に作ることができれば機能テスト自身が詳細仕様の定義になると考えています。  
そして、**この形式の詳細仕様はプログラムの仕様と一致することを保証できます。**<br>  
各機能について使い方の説明、および、機能テストの概要を説明しています。疑義が有る時は自動テストのソース・コードをみることで詳細仕様を把握できるのではないかと考えています。<br>
<br>
<h1 id="DefectReport">7.不具合報告を頂く時のお願い</h1>
不具合報告は他の報告同様、当リポジトリのIsshuesへお願いします。  
不具合をできるだけ遅滞なく伝達できるよう下記の点に注意を払って頂けると助かります。  
全て守って頂きたいという意味ではなく可能な部分についてご配慮頂けることをお願い致します。  

1. **他のIssuesをチェックして同様な不具合報告がないか探して下さい**  
もし、見つかった場合、あなたの環境で発生した旨の追記をそのIssueへお願いします。  
不具合が発生している人数も対策優先順位を決める際の参考となりますので。
2. **できるだけ具体的なタイトルを付けて下さい**  
  他の方も同じ不具合の発生を探しやすいように、できるだけ どの部分のどんな不具合か分かるようなタイトルを付けて下さい。
3. **不具合発生環境を記載下さい**  
  下記情報の記載をお願い致します。
    - Theolizerの種類  
    プリビルド版ならどのプリビルド版か。  
    ソース版ならビルドに用いたコンパイラ、LLVMのバージョン、boostのバージョン。  
    - OSの種類とバージョン、32bit or 64bit
    - コンパイラの種類とバージョン、32bit or 64bit
    - CMakeのバージョン、32bit or 64bit
4. **下記不具合情報をできるだけご記載下さい**
    - **不具合発生時に行っていた操作**  
    例えば、Theolizerを用いたあなたのプログラムのビルド中など
    - **どのような現象が発生したか？**
    - **期待する動作はなにか？**
    - **不具合が発生するソース・コード**  
    できれば、不具合が発生する最小のソースをご提供頂ければ助かります。
5. **その他、追加情報があれば記載下さい**  
  何か気になったことや、不具合原因の推測等、なんでもよいので関連しそうな追加情報を頂けると助かります。
