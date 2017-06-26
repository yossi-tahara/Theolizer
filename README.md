
リンク：[【Theolizer公式サイト】](https://theolizer.com)[【サポート掲示板】](https://theolizer.com/customer/forums/)[【使い方解説】](https://theolizer.com/category/theolizer/)
### -- 目次 --
[1.Theolizerとは](#About)  
[2.Theolizerの特長](#Features)  
[3.対応環境](#Platforms)  
[4.その他](#etc)  

<h1 id="About">１．Theolizerとは</h1>

皆さんは C++ でデータをファイルへ保存したりシリアル通信等で他のコンピュータへ送ったりする時、どのようにしていますか？  
多くの人は、保存や通信用のデータ・フォーマットを設計し、そのフォーマットへ変換／逆変換するプログラムを１つ1つ書いていると思います。  
Theolizerはその手間をズバッと無くすC++開発ツールです。シリアライザと呼ばれます。  
（C言語＋αの知識で使えるよう設計していますのでベターCerの方にもお使い頂けます。）

例えば、ある構造体をファイルへ保存／回復する時、最初に次のような処理が考えられます。

```C++
#include <iostream>
#include <fstream>
#include <string>

struct Foo
{
    std::string name;
    int age;

    void save(std::ostream& os) {os << name << " " << age << "\n";}
    void load(std::istream& is) {is >> name >> age;}
};

int main()
{
    {
        Foo foo;
        foo.name="Taro Yamada";
        foo.age=22;

        std::ofstream ofs("sample.txt");
        foo.save(ofs);
    }
    {
        Foo foo;
        std::ifstream ifs("sample.txt");
        foo.load(ifs);

        foo.save(std::cout);	// 回復結果を簡易表示
    }
}
```
### sample.txt
```txt
Taro Yamada 22
```

しかし、残念なことに [これはバグってます。](https://wandbox.org/permlink/JIBbWBShdozp208R)nameには性と名があり、間を空白文字で区切ります。従って、保存はされますが、回復する時、nameに名前の前半のみ回復され後半はageへ回復しようとして失敗します。  
そこで、例えばnameとageをそれぞれ１行へ保存することにし下記のように[save/loadを修正する](https://wandbox.org/permlink/prfAIa2x28neITvx)ことも考えられます。  

```C++
struct Foo
{
    std::string name;
    int age;

    void save(std::ostream& os) {os << name << "\n" << age << "\n";}
    void load(std::istream& is)
    {
        std::getline(is, name);
        std::string temp;
        std::getline(is, temp);
        age=atoi(temp.c_str());
    }
};
```
### sample.txt
```txt
Taro Yamada
22
```
フィールド区切り方法を検討し、実装するので意外に手間がかかります。そして通常は保存／回復したいデータはもっと複雑です。  
色々工夫して手間を省くとは思いますが、その工夫そのものにも手間がかかります。

<b>Theolizerを使った場合：</b>  
まずTheolizerライブラリが基本的な保存／回復ライブラリを提供します。（[全ての基本形＋全てのstd::basic_string型に対応](http://doc.theolizer.com/html/_specification.html#Basic11)）更に、ほとんどの場合 [Theolizerドライバがクラスのメンバ変数を自動的に列挙し、保存／回復処理を生成します。](http://doc.theolizer.com/html/_abstract.html#Mechanism)あなたはその保存／回復処理を呼び出すだけです。

### サンプル・ソース（example.cpp）

```C++
#include <iostream>
#include <fstream>
#include <string>
#include <theolizer/serializer_json.h>

struct Foo
{
    std::string name;
    int age;
};

#include "example.cpp.theolizer.hpp"            // Theolizer自動生成先

int main()
{
    // 保存
    {
        Foo foo;
        foo.name="Taro Yamada";
        foo.age=22;

        std::ofstream ofs("sample.txt");
        theolizer::JsonOSerializer<> jos(ofs);  // シリアライザを生成
        THEOLIZER_PROCESS(jos, foo);            // ファイルへfooを保存
    }

    // 回復
    {
        Foo foo;
        std::ifstream ifs("sample.txt");
        theolizer::JsonISerializer<> jis(ifs);  // デシリアライザを生成
        THEOLIZER_PROCESS(jis, foo);            // ファイルからfooを回復

        theolizer::JsonOSerializer<> jos(std::cout);
        THEOLIZER_PROCESS(jos, foo);            // 回復結果の簡易表示
    }
}
```

構造体Fooのメンバを保存／回復する処理をTheolizerが自動生成しますので、あなたは記述する必要がありません。

### sample.txt
```txt
{
    "SerialzierName":"JsonTheolizer",
    "GlobalVersionNo":1,
    "TypeInfoList":[1]
}
{
    "name":"Taro Yamada",
    "age":22
}
```

### example.cppをビルドする手順

1. [Theolzierのダウンロードとインストール](http://doc.theolizer.com/html/_prepare.html)
2. [ビルド環境の自動生成(by CMake)とビルド](http://doc.theolizer.com/html/_how_to_make_project.html#Example)

### CMakeプロジェクトを作ってTheolizerを組み込む方法

公式サイトの[技術ブログ](https://theolizer.com/theolizer/cmake_and_cmakelists/)で解説しています。<br>
CMakeの基礎から解説していますので、CMakeの使い方を学びたい方も是非ご覧になって下さい。

---
<h1 id="Features">２．Theolizerの特長</h1>

C#のように[リフレクション](https://goo.gl/XCRSND)を提供する言語なら可能ですか、C++はそのような機能を提供しておらず、我々のC++プログラムがクラスのメンバ変数のリストを得ることができません。（C++コンパイラは持っている情報ですので扱えそうな気もして、かなり深く調べましたが残念ながらできません。）<br>

それを clangのlibToolingの力を借りてできるようにしました。これにより、C++コンパイラが持っているクラスのメンバ変数リストとenum型のシンボル名を取り出して、自動シリアライズを実現しました。<br>

そのTheolizerは、次のような特長を持っています。

## 2-1.自動シリアライズ
Theolizerは[Clang/LLVM](http://llvm.org/)に含まれるlibTooling[libTooling](http://qiita.com/Chironian/items/6021d35bf2750341d80c)の構文解析機能を用いてクラスやenum型の定義を解析し、メンバのリストを自動生成します。これにより、クラスやenum型を変更した時もメンバ・リストへの反映漏れが無く、プログラム開発が捗ります。

## 2-2.[保存先指定](http://doc.theolizer.com/html/_destinations.html)
クラスのメンバ変数を保存する時、異なるファイルへ保存したい時もあると思います。一部のメンバ変数（ファイル・ハンドル等）は保存しないでしょうし、一部のメンバ変数は設定ファイルへ保存し、また別のメンバはデータ・ファイルへ保存したいというケースは少なくないと思います。   

TheolizerはclangのAnnotation機能を使ってメンバ変数に保存先を指定することで、クラスを分割して異なるファイルへ保存したり、一部を通信で他のプログラムへ送信したり、一部はそもそも保存しないことができます。

## 2-3.[オブジェクト追跡](http://doc.theolizer.com/html/_specification.html#Tracking)
一般に、ポインタをファイルへ保存し、プログラムを再起動した後で回復しても意味はありません。  

Theolizerはポイント先を一緒にファイルへ保存してオブジェクト追跡することでポインタを回復することができます。これにより非常に複雑なデータ構造を容易に保存／回復・送受信できます。  

## 2-4.プログラム変更対応
シリアライザを使って保存するということは、内部データの構造を保ったままファイルへ保存するということです。内部データ構造はプログラムの変更に伴い変更されるため、旧プログラムで保存されたファイルを回復できるようにするためには内部データ構造の変更に強い制限がかかります。  

Theolizerはその制限を大幅に緩和しました。

- **[クラス定義](http://doc.theolizer.com/html/_changing_class.html)、[enum定義](http://doc.theolizer.com/html/_changing_enum.html)の変更**  
クラスのメンバ変数の追加／削除／順序変更、および、メンバ変数名の変更に対応します。  
enum型のシンボルの追加／削除／順序変更、および、シンボル名やシンボル値の変更に対応します。

- **[クラスについてバージョン・アップ／ダウン処理を記述可能](http://doc.theolizer.com/html/_changing_class.html#HowToModifyClass54)**  
クラスをバージョン・アップする際に行いたい処理を記述できます。  
（例えば、変数にオフセットを追加した時、そのオフセットを加える処理など）  
また、旧バージョン形式で保存したい場合はバージョン・ダウン処理を記述できます。  
（例えば、オフセットを引く処理など）

- **[バージョン・アップ／ダウンをカスケードに処理します](http://doc.theolizer.com/html/_changing_class.html#HowToModifyClass4)**  
例えば、Ver.4をVer.5へバージョンアップした時、従来はVer.4→Ver.5のプログラム開発だけでなく、Ver.1～Ver.3→Ver.4のプログラムをVer.1～Ver.3→Ver.5へ改造する必要がありました。  
Theolizerはバージョン・アップ／ダウン処理をカスケード(Ver.1→Ver.2→Ver.3→Ver.4→Ver.5)に行いますので、Ver.4→Ver.5のプログラムを開発するだけで済みます。<br>
これによりバージョンが上がった時のプログラム開発を大きく低減できます。

---
<h1 id="Platforms">３．対応環境</h1>

マルチ・プラットフォームとC++11規格コンパイラに対応できるよう設計しています。<br>
現時点でテストできている環境はWindowsとubuntuです。

|OS|C++コンパイラ|
|--------|--------|
|Windows 10 Professional 64bit|[Visual Studio C++ 2015 Community update 3](https://www.microsoft.com/ja-jp/dev)<br>[MinGW 5.4.0 32bit posix dwarf](https://sourceforge.net/projects/mingw-w64/)<br>[MinGW 5.4.0 64bit posix seh](https://sourceforge.net/projects/mingw-w64/)|
|ubuntu 16.04 LTS 64bit|gcc 5.4.0(Ubuntu 5.4.0-6ubuntu1~16.04.2)|

将来的にはマッキントッシュOS Xにも対応する予定です。

---
<h1 id="etc">４．その他</h1>

### 4-1.詳細ドキュメント
詳細なドキュメントを[http://doc.theolizer.com](http://doc.theolizer.com)に置いています。<br>
また、Theolizerをインストールした時、インストール先のdocフォルダにもあります。

### 4-2.プリビルド版
Windowsとubuntu用のプリビルド版を用意しています。<br>
当リポジトリの[relases](https://github.com/yossi-tahara/Theolizer/releases)からダウンロードして下さい。

### 4-3.自動テスト
Theolizerは信頼性を上げるために自動テストを実装しています。

- CIサービスによるサブセット・テスト（masterブランチの最新版）の状態<br>
~~Travis CIにてubuntu環境のサブセット・テストをpush毎に自動実行しています。~~<br>
~~Travis CI result : [![Build Status](https://travis-ci.org/yossi-tahara/Theolizer.svg?branch=master)](https://travis-ci.org/yossi-tahara/Theolizer) "for StaticWithBoost without sudo on ubuntu 14.04 LTS"~~<br>
（2017年6月21日現在、Travis-CIのllvmパッケージの異常により、Travis-CI環境ではTheolizerドライバをビルドできないため、一時的に停止しています。詳しくはIssue #38を参照下さい。）

- ローカル環境によるフルセット・テストについて<br>
バージョン番号に`-Prerelease`サフィックスのないものはフルセットのテストを完了しています。<br>
フルセット・テストの実行と共にプリビルド版を生成しています。これにvX.Y.Zのタグをつけて当リポジトリの[relases](https://github.com/yossi-tahara/Theolizer/releases)へ登録します。

### 4-4.不具合報告を頂く時のお願い
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
    - 不具合発生時に行っていた操作  
    例えば、Theolizerを用いたあなたのプログラムのビルド中など
    - どのような現象が発生したか？
    - 期待する動作はなにか？
    - 不具合が発生するソース・コード  
    できれば、不具合が発生する最小のソースをご提供頂ければ助かります。
5. **その他、追加情報があれば記載下さい**  
  何か気になったことや、不具合原因の推測等、なんでもよいので関連しそうな追加情報を頂けると助かります。

---
© 2016 [Theoride Technology](http://theolizer.com/) All Rights Reserved.  
["Theolizer" is a registered trademark of Theoride Technology.](http://theolizer.com/info/theolizer%E3%81%8C%E5%95%86%E6%A8%99%E7%99%BB%E9%8C%B2%E3%81%95%E3%82%8C%E3%81%BE%E3%81%97%E3%81%9F/)
