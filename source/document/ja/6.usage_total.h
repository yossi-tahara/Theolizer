//############################################################################
/*!
    @brief      ドキュメント・ファイル－使用方法（全体）
    @ingroup    Documents
    @file       6.usage_total.h
    @author     Yoshinori Tahara
    @date       2016/11/01 Created
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
    @page UsageTotal 使用方法（全体）

ここでは、Theolizerの使い方を説明します。

TheolizerはAPIの機能テストをできるだけ自動化してます。<br>
そして、APIの機能テストは詳細仕様を規定することでもあります。複雑なプログラムでは文書として読み取ることは困難ですが、単純なプログラムであれば、文書としての機能を果たせるのではないかと考えています。

そこで、各機能についてそれをテストするための多数のテスト群の内、代表的なものを使って説明し、詳細テストの関連部分のソースも提示します。できるだけ読み取るのに苦労しない形式でプログラムを記述し、詳細仕様書として有用であることを目指します。

まず、1～3節で全体的な説明を行い、4節以降で各機能について説明します。

<br>
@section Naming 1.名前の付け方

TheolizerのAPIは名前空間に入れています。また、マクロは決まったプリフィクスを付けています。
少し意味を持たせていますので説明します。

@subsection Namespace 1-1.名前空間

ほぼ全てのシンボルを <b>theolizer</b> 名前空間へ入れています。<br>
一部、入れると実装が難しいものについては、シンボルに <b>Theolizer</b> を含みます。<br>
これにより既存のコードと被ることはまずない筈です。

次に、内部的に使用するシンボルは <b>internal</b> 名前空間へ入れています。<br>
これらのシンボルが付けられたクラスや関数等は、Theolizerのアップデート時、上位互換性を考慮しませんので使用しないようお願いします。

@subsection MacroName 1-2.マクロ名

全てのマクロは <b>THEOLIZER_</b> で始めています。<br>
これにより既存のコードと被ることはまずない筈です。

次に、内部的に使用するマクロは、 <b>THEOLIZER_INTERNAL_</b> で始めています。<br>
また、Theolizerが自動生成するマクロを、 <b>THEOLIZER_GENERATED_</b>  で始めています。<br>
これらのマクロは、Theolizerのアップデート時、上位互換性を考慮しませんので使用しないようお願いします。

<br>
@section TestProgram 2.テスト・プログラムの構造
主な機能テスト・プログラムは、 <b>source/reference_and_test</b> 以下にテストの分類に応じてフォルダを分けて入れています。

@subsection TestProgramStructure 2-1.構造について
定義変更するということはプログラム自体が異なります。それをテストするため、変更したプログラム毎に異なるフォルダへテスト・プログラムを入れています。<br>
保存先は、 <b>source/reference_and_test/version/ver*</b> です。
<b>ver*</b> はバージョン番号、及び、バージョン番号を変更せずにプログラム変更したもの用のフォルダ名です。<br>
例えば、<b>ver1</b>はバージョン1用、<b>ver1a</b>はそれを少し変更したものを入れてます。

これらのテスト・プログラムの構造は共通です。現在は下記のような構造になっています。（機能テストの追加に伴い、ファイルと関数を追加します。）

|ファイル|関数|概要|
|--------|----|----|
|main.cpp |main()|パラメータ解析<br>GlobalVersionNoを振ってvaryGlobalVersionNo()呼び出し|
|↑|varyGlobalVersionNo()|CheckModeを振ってvaryCheckMode()呼び出し|
|↑|varyCheckMode()|必要なシリアライザのインスタンスを生成し、saveBasic(), loadBasic()呼び出し|
|↑|saveBasic()|自動テスト基本部の保存処理。個別テストを呼び出す|
|↑|loadBasic()|自動テスト基本部の回復処理。個別テストを呼び出す|
|test_basic_process.cpp ||THEOLIZER_PROCESS()の網羅的な使用例（自動テスト）|
|↑|saveBasicProcess()|保存|
|↑|loadBasicProcess()|回復|

@subsection MacroForDescription 2-2.説明で用いるマクロについて
テスト用のマクロはtest_tool.hで定義しています。<br>
その内、使い方の説明（兼 自動テスト）で用いるマクロについてここで簡単に説明します。

### THEOLIZER_EQUAL(dLhs, dRhs, ...)
(dLhs == dRhs) ならばPASS、そうでないならFAILと判定します。<br>
PASSならば、テストの数とPASS数をインクリメントします。<br>
FAILならば、テストの数とFAIL数をインクリメントし、テストを失敗させます。<br>
また、dRhsとそれ以降のアイテム（1個以上7個まで）を標準出力へ出力します。

下記はシリアライザを使って回復したint型のaIntの値が-103であることをチェックしています。

@dontinclude test_basic_process.cpp
@skip int aInt;
@until THEOLIZER_EQUAL

<br>
@section BasicUsage 3.基本的な使い方

Theolizerをインストールした後、あなたのデータをTheolizerでシリアライズするために、あなたのプログラムを下記の順序を守るようにして下さい。

1. Theolizerヘッダをインクルード
2. シリアライズするクラスとenum型の定義
3. Theolizerの自動生成ファイルをインクルード
4. シリアライズ処理

@subsection TheolizerHeader 3-1.Theolizerヘッダをインクルード
一部例外はありますが原則として、<b>シリアライズするクラスとenum型の定義</b> 前にTheolizerのヘッダをインクルードして下さい。<br>
例外は <b>非侵入型</b>のクラスとenum型です。(@ref Basic12) <br>
これらについては <b>Theolizerヘッダをインクルード</b> する前に定義しても問題ありません。

現在、サポートしているシリアライザはJson形式、独自Binary形式、メモリ内専用のFast形式の3種類です。
それぞれ、下記ヘッダをインクールドして下さい。


|形式|インクルードするヘッダ|注意事項|
|----|----------------------|--------|
|Json形式|<theolizer/serializer_json.h>|fstreamの時はテキスト・モードでオープンする|
|独自Binary形式|<theolizer/serializer_binary.h>|fstreamの時は<b>バイナリ・モード(std::ios_base::binary)</b>でオープンする|
|メモリ内専用Fast形式|上記のどちから、もしくは、<theolizer/serializer.h>|fstreamの時は<b>バイナリ・モード(std::ios_base::binary)</b>でオープンする|

<b>Theolizerヘッダのインクルード例：(source/samples/example/example.h）</b>

@dontinclude example.h
@skip theolizer
@until theolizer

<b>特記事項：</b><br>
theolizer/serializer.hはシリアライザ用の基本ヘッダです。serializer_jsonとserializer_binaryからもインクルードされています。<br>
メモリ内専用Fast形式(FastSerializer)はTheolizerが内部的に使用しているため、serializerからインクルードされています。

<b>バイナリ形式における注意事項：</b><br>
バイナリ形式のシリアライザをfstreamで用いる時は、必ずバイナリ・モードでfstreamをオープンして下さい。<br>
ファイル・ストリームへ数値26(0x1A)が出力されるとこれはEOFコードのため、回復時にEOFエラーになります。<br>
また、Windowsの場合、数値10(0x0A)が出力されるとCR LFへ展開されてしまい、適切に回復できません。<br>
Theolizer側でエラーにしたいのですが、iostreamではそのオープン・モードを確認できないため、チェックが困難なのです。<br>

<b>独自Binary形式について：</b><br>
Big EndianとLittle Endianについては自動的に吸収しています。<br>

浮動小数点型はIEEE754フォーマットを期待しています。また、バイト単位でEndian変換します。<br>
long doubleはbinary64と「radix==2、digits==64、max_exponent==16384」の80ビット拡張精度形式に対応しています。<br>
浮動小数点型のエンコード／デコード処理は、source/library/theolizer/internal/serializer_binary.cppの下記関数テンプレートで行っています。
  - BinaryMidOSerializer::saveFloat()
  - BinaryMidISerializer::loadFloat()

@subsection DefineClassEnum 3-2.シリアライズするクラスとenum型の定義
<b>Theolizerの自動生成ファイルをインスクルード</b> する前に、<b>シリアライズするクラスとenum型の定義</b> を行います。この順序に例外はありません。<br>
標準ライブラリを含む他のライブラリのヘッダについては、通常通りインクルードして下さい。

<b>クラスとenum型定義例：(source/samples/example/example.h）</b>

@skip EnumType
@until };
@until };

@subsection IncludeAutoGeneratedFile 3-3.Theolizerの自動生成ファイルをインクルード
@ref Mechanism で説明したようにTheolizerはシリアライズに必要なソース・コードを自動生成します。<br>
このファイルはコンパイル単位（通常は.cppファイル）毎に生成され、当該.cppファイルと同じフォルダへ自動的に生成されます。<br>
Theolizerは、バージョンを上げた時に古いクラス定義やenum型定義をここに残します。これにより古いシリアライズ・データを回復できます。<br>
そこで、このファイルをお使いのバージョン管理システム（gitやsvn等）へ登録することを強くお勧めします。

自動生成するファイルのファイル名は、そのコンパイル単位のファイル名に".theolzier.hpp"を繋げたものです。<br>
例えば、<b>example.cpp</b> の場合は、<b>example.cpp.theolizer.hpp</b> となります。

<b>例：(source/samples/example/example.cpp）</b>

@dontinclude example.cpp
@skip include
@until example.cpp.theolizer.hpp

@subsection SerializingProcess 3-4.シリアライズ処理
シリアライズは下記の３つで行います。
  - シリアライザ・インスタンスの生成
  - シリアライズ処理要求（シリアライズするインスタンスを指定する）
  - シリアライザ・インスタンスの破棄

@subsubsection ConstructSerializer 3-4-1.シリアライザ・インスタンスの生成
その際、シリアライズ先のデータ・ストリームを指定します。ファイル・ストリームや[TCP/IPストリーム](http://localhost/theolizer/tcp_ip_sample/)を指定して下さい。<br>
保存や送信時はstd::ostreamを、回復や受信時はstd::istreamを与えて下さい。

 - <b>保存（送信用）シリアライザ</b> <br>
    theolizer::JsonOSerialzier<br>
    theolizer::BinaryOSerialzier<br>
    theolizer::FastOSerialzier<br>

 - <b>回復（受信用）シリアライザ（デシリアライザ）</b> <br>
    theolizer::JsonISerialzier<br>
    theolizer::BinaryISerialzier<br>
    theolizer::FastISerialzier<br>

@subsubsection Request 3-4-2.シリアライズ処理要求
下記マクロでシリアライズします。保存用シリアライザを指定すると保存、回復用シリアライザを指定すると回復動作となります。<br>
これはいつくでも記述して良いです。

<b>THEOLIZER_PROCESS(dSerializer, dInstance)</b>
  - dSerializer : シリアライザのインスタンスを指定します。<br>
  - dInstance   : 保存するインスタンスを指定します。<br>

dInstanceを保存／回復します。<br>
ポインタ型を指定した場合は、アドレス回復のためにポイント先のオブジェクト追跡を行います。<br>

他にTHEOLIZER_PROCESS_POINTEE()とTHEOLIZER_PROCESS_OWNER()がありますが、これらについては「@ref ObjectTracking 」で説明します。

@subsubsection DestructSerializer 3-4-3.シリアライザ・インスタンスの破棄
最後にシリアライザ・インスタンスを破棄することでシリアライズ処理を完了します。<br>
生成時に例外発生を禁止していた場合に、エラー状態をリセットしないまま破棄すると、プロセスをアボートします。これはエラー状態の見逃しを回避するための仕様です。<br>
例外発生を禁止している場合は、破棄する前には必ずエラー情報をチェクした上で、エラー状態をリセットして下さい。<br>
具体的手順はT.B.D.にて説明します。

<b>ファイルへの保存例：(source/samples/example/example.cpp）</b>

@dontinclude example.cpp
@skip try
@skip {
@until }

<b>ファイルからの回復例：(source/samples/example/example.cpp）</b>

@skip try
@skip {
@until }

*/
