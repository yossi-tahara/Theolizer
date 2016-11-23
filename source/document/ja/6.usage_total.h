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
//############################################################################
@section Naming 1.名前の付け方
//############################################################################

TheolizerのAPIは名前空間に入れています。また、マクロは決まったプリフィクスを付けています。
少し意味を持たせていますので説明します。

// ***************************************************************************
@subsection Namespace 1-1.名前空間
// ***************************************************************************

ほぼ全てのシンボルを <b>theolizer</b> 名前空間へ入れています。<br>
一部、入れると実装が難しいものについては、シンボルに <b>Theolizer</b> を含みます。<br>
これにより既存のコードと被ることはまずない筈です。

次に、内部的に使用するシンボルは <b>internal</b> 名前空間へ入れています。<br>
これらのシンボルが付けられたクラスや関数等は、Theolizerのアップデート時、上位互換性を考慮しませんので使用しないようお願いします。

// ***************************************************************************
@subsection MacroName 1-2.マクロ名
// ***************************************************************************

全てのマクロは <b>THEOLIZER_</b> で始めています。<br>
これにより既存のコードと被ることはまずない筈です。

次に、内部的に使用するマクロは、 <b>THEOLIZER_INTERNAL_</b> で始めています。<br>
また、Theolizerが自動生成するマクロを、 <b>THEOLIZER_GENERATED_</b>  で始めています。<br>
これらのマクロは、Theolizerのアップデート時、上位互換性を考慮しませんので使用しないようお願いします。
<br>
//############################################################################
@section BasicUsage 2.基本的な使い方
//############################################################################

Theolizerをインストールした後、あなたのデータをTheolizerでシリアライズするために、あなたのプログラムを下記の順序を守るようにして下さい。

1. Theolizerヘッダをインクルード
2. シリアライズするクラスとenum型の定義
3. Theolizerの自動生成ファイルをインクルード
4. シリアライズ処理

// ***************************************************************************
@subsection TheolizerHeader 2-1.Theolizerヘッダをインクルード
// ***************************************************************************

一部例外(*1)はありますが原則として、<b>シリアライズするクラスとenum型の定義</b> 前にシリアライザのヘッダをインクルードして下さい。<br>

<b>Json形式シリアライザ・ヘッダのインクルードと型定義の例：(source/samples/example/example.h）</b>

@dontinclude example.h
@skip ********
@until };
@until };

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
(*1) 例外：<b>非侵入型</b>(@ref Basic12)のクラスやenum型については、<b>シリアライザのヘッダをインクルード</b>する前でも定義できます。<br>
</div>

現在、サポートしているシリアライザはJson形式、独自Binary形式、メモリ内専用のFast形式の3種類です。
それぞれ、下記ヘッダをインクールドして下さい。


|形式|インクルードするヘッダ|注意事項|
|----|----------------------|--------|
|Json形式|<theolizer/serializer_json.h>|fstreamはテキスト・モードでオープンする|
|独自Binary形式|<theolizer/serializer_binary.h>|fstreamは<b>バイナリ・モード(std::ios_base::binary)</b>でオープンする|
|メモリ内専用Fast形式|上記のどちから、もしくは、<theolizer/serializer.h>|fstreamは<b>バイナリ・モード(std::ios_base::binary)</b>でオープンする|
<br>
<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
@anchor バイナリ・モード
<b>バイナリ形式における注意事項：</b><br>
バイナリ形式のシリアライザをfstreamで用いる時は、必ずバイナリ・モード（std::ios_base::binary）でfstreamをオープンして下さい。<br>
fstreamがテキスト・モードでオープンされている場合、ファイル・ストリームへ数値26(0x1A)が出力されると、これはEOFコードなので回復時にEOFエラーになります。また、Windowsで数値10(0x0A)が出力されるとCR LFへ展開されてしまい、適切に回復できません。<br>
このような事態をさけるため、Theolizer側でエラーにしたいのですが、iostreamではそのオープン・モードを確認できないためチェックが困難なのです。<br>
</div>
<br>
// ***************************************************************************
@subsection DefineClassEnum 2-2.シリアライズするクラスとenum型の定義
// ***************************************************************************

<b>シリアライズするクラスとenum型の定義</b>後に、<b>Theolizerの自動生成ファイルをインスクルード</b> します。この順序に例外はありません。<br>

なお、シリアライズ対象の型を含まないヘッダ・ファイルは、Theolizerに関する順序制限はありません。

<b>クラスとenum型定義例：(source/samples/example/example.h）</b>

// ***************************************************************************
@subsection IncludeAutoGeneratedFile 2-3.Theolizerの自動生成ファイルをインクルード
// ***************************************************************************

@ref Mechanism で説明したようにTheolizerはシリアライズに必要なソース・コードを自動生成します。<br>
このファイルはコンパイル単位（通常は.cppファイル）毎に生成され、当該.cppファイルと同じフォルダへ自動的に生成されます。<br>
Theolizerは、バージョンを上げた時に古いクラス定義やenum型定義をここに残します。これにより古いシリアライズ・データを回復できます。<br>
そこで、このファイルをお使いのバージョン管理システム（gitやsvn等）へ登録することをお勧めします。

自動生成するファイルのファイル名は、そのコンパイル単位のファイル名に".theolzier.hpp"を繋げたものです。<br>
例えば、<b>example.cpp</b> の場合は、<b>example.cpp.theolizer.hpp</b> となります。

<b>例：(source/samples/example/example.cpp）</b>

@dontinclude example.cpp
@skip include
@until example.cpp.theolizer.hpp

// ***************************************************************************
@subsection SerializingProcess 2-4.シリアライズ処理
// ***************************************************************************

シリアライズは下記の３つで行います。
  - シリアライザ・インスタンスの生成
  - シリアライズ処理要求（シリアライズするインスタンスを指定する）
  - シリアライザ・インスタンスの破棄

@subsubsection ConstructSerializer 2-4-1.シリアライザ・インスタンスの生成
その際、シリアライズ先のデータ・ストリームを指定します。ファイル・ストリームや[TCP/IPストリーム](http://localhost/theolizer/tcp_ip_sample/)を指定して下さい。<br>
保存や送信時はstd::ostreamを、回復や受信時はstd::istreamを与えて下さい。

 - <b>保存（送信用）シリアライザ</b> <br>
    theolizer::JsonOSerialzier<br>
    theolizer::BinaryOSerialzier<br>
    theolizer::FastOSerialzier<br>

 - <b>回復（受信用）シリアライザ（デシリアライザ）</b> <br>
    theolizer::JsonISerialzier<br>
    theolizer::BinaryISerializer<br>
    theolizer::FastISerialzier<br>

@subsubsection Request 2-4-2.シリアライズ処理要求
下記マクロでシリアライズします。保存用シリアライザを指定すると保存、回復用シリアライザを指定すると回復動作となります。<br>
これはいつくでも記述して良いです。

<b>THEOLIZER_PROCESS(dSerializer, dInstance)</b>
  - dSerializer : シリアライザのインスタンスを指定します。<br>
  - dInstance   : 保存するインスタンスを指定します。<br>

dInstanceを保存／回復します。<br>
ポインタ型を指定した場合は、アドレス回復のためにポイント先のオブジェクト追跡を行います。<br>

他にTHEOLIZER_PROCESS_POINTEE()とTHEOLIZER_PROCESS_OWNER()がありますが、これらについては「@ref ObjectTracking 」で説明します。

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
このようにシリアライザを問わず、全て同じマクロでシリアライズ処理できます。<br>
xmlも将来的にサポートする予定ですが、その際も同じマクロで対応できる筈です。<br>
（マクロなので、\#演算によりインスタンス名をC言語文字列として取り出して用います。）
</div>

@subsubsection DestructSerializer 2-4-3.シリアライザ・インスタンスの破棄
最後にシリアライザ・インスタンスを破棄することでシリアライズ処理を完了します。<br>
生成時に例外発生を禁止していた場合に、エラー状態をリセットしないまま破棄すると、プロセスをアボートします。これはエラー状態の見逃しを回避するための仕様です。<br>
例外発生を禁止している場合は、破棄する前には必ずエラー情報をチェクした上で、エラー状態をリセットして下さい。<br>
具体的手順は@ref ErrorReport にて説明します。

<b>ファイルへの保存例：(source/samples/example/example.cpp）</b>

@dontinclude example.cpp
@skip try
@skip {
@until }

<b>ファイルからの回復例：(source/samples/example/example.cpp）</b>

@skip try
@skip {
@until }

<br>
//############################################################################
@section Serializers 3.各シリアライザの説明
//############################################################################
現在、サポートしているシリアライザはJson形式、独自Binary形式、メモリ内専用のFast形式の3種類です。<br>
ここではそれぞれの使い方を説明します。

<br>
// ***************************************************************************
@subsection Serializer 3-1.共通事項
// ***************************************************************************

全てのシリアライザについて共通な事項について説明します。

@subsubsection CheckMode 3-1-1.型チェック・モード
Theolizerのシリアライザは回復時に型が一致していることをチェックできます。<br>
その方法としてシリライズ・データ内に「型名を保存する方法」と「型に割り当てたインデックス番号（TypeIndex）を保存する方法」の２種類を用意しています。

theolizer::CheckMode
|列挙値|意味|
|------|----|
|NoTypeCheck|型チェック無し|
|TypeCheck|型名による型チェック|
|TypeCheckByIndex|TypeIndexによる型チェック|

NoTypeCheckは型情報をシリライズ・データに含みませんのでデータ量が少ない場合の効率は良いです。しかし、メンバ名をヘッダではなくデータ側に含むためデータ量が多くなると効率は悪化します。<br>
TypeCheckはテキスト型の場合、データを目視確認し易いです。データ量が少ない時の効率はNoTypeCheckの次に良いです。<br>
TypeCheckByIndexはデータ量が多い時は３種のCheckModeの中で最大の効率を発揮します。

型情報等の管理データを下記のように記録します。
  - <b>シリアライズ・データのヘッダ部</b><br>
    <b>NoTypeCheck：</b>型情報を記録しません。<br>
    <b>TypeCheck：</b>クラスについて、各メンバのメンバ名（@link Basic121 名前対応@endlink時）と型名を記録します。<br>
    <b>TypeCheckByIndex：</b>全ての型について、TypeIndexに対応する型名を記録します。
    更にクラスについては、各メンバのメンバ名（@link Basic121 名前対応@endlink時）とTypeIndexを記録します。<br>
<br>

  - <b>シリアライズ・データの各データ部</b><br>
    <b>NoTypeCheck：</b>クラスについて、各メンバのデータとメンバ名（@link Basic121 名前対応@endlink時）をセットで記録します。<br>
    <b>TypeCheck：</b>THEOLIZER_PROCESS()マクロで指定したデータと共にその型名を記録します。<br>
    <b>TypeCheckByIndex：</b>THEOLIZER_PROCESS()マクロで指定したデータと共にそのTypeIndexを記録します。<br>

回復時に回復先の変数の型と上記の情報と照らし合わせることで型チェックを行います。<br>

@subsubsection DerivedMembers 3-1-2.派生メンバ
各シリアライザが継承しているメンバの内、公開しているものは下記です。

|メンバ名|意味|
|--------|----|
|static const bool kIsSaver ;|保存用はtrue／回復用はfalse|
|static const bool kIsFastSerialzier ;|FastSerializerのみtrue|
|void clearTracking() ;|オブジェクト追跡の区切り（@ref ObjectTracking 参照）|
|theolizer::ErrorInfo const& getError() ;|エラー情報返却|
|void resetError() ;|エラー状態解除|
|bool isTerminated() const ;|処理中のクラスの読み出しが終了(mTerminated)していたらtrueを返却します。<br>非侵入型手動クラスで回復処理を実装する際に使用します。|

<br>
// ***************************************************************************
@subsection JsonSerializer 3-2.Json形式（JsonSerializer）
// ***************************************************************************
Json形式でシリアライズする場合は、<b>theolizer/serializer_json.h</b>をインクルードして下さい。

@subsubsection JsonOSerialzier 3-2-1.保存用JsonSerialzier
@link theolizer::JsonOSerializer::JsonOSerializer(std::ostream&,unsigned,CheckMode,bool,bool)
@copybrief theolizer::JsonOSerializer::JsonOSerializer(std::ostream&,unsigned,CheckMode,bool,bool)
@endlink

@code
JsonOSerializer
(
    std::ostream& iOStream,
    unsigned iGlobalVersionNo=kLastGlobalVersionNo,
    CheckMode iCheckMode=CheckMode::NoTypeCheck,
    bool iNoPrettyPrint=false,
    bool iNoThrowException=false
);
@endcode

@link theolizer::JsonOSerializer::JsonOSerializer(std::ostream&,CheckMode,bool,bool)
@copybrief theolizer::JsonOSerializer::JsonOSerializer(std::ostream&,CheckMode,bool,bool)
@endlink

@code
JsonOSerializer
(
    std::ostream& iOStream,
    CheckMode iCheckMode,
    bool iNoPrettyPrint=false,
    bool iNoThrowException=false
);
@endcode

|パラメータ名|意味|
|------------|----|
|iOStream|出力先のストリーム(ofstreamはテキスト・モードでオープンして下さい）|
|iGlobalVersionNo|保存するグローバル・バージョン番号(省略時は最新版)|
|iCheckMode|型チェック・モード（省略時はNoTypeCheck)|
|iNoPrettyPrint|整形出力しない時true（省略時はfalse)|
|iNoThrowException|例外禁止時true（省略時はfalse)|

#### 専用継承関数

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
<b>void setCharIsMultiByte(bool iCharIsMultiByte);</b><br>
iCharIsMultiByte=trueの時、std::stringをWindowsマルチ・バイト文字コードとして取り扱います。<br>
コンストラクト直後はiCharIsMultiByte=falseです。<br>
Windowsのみ機能します。それ以外のOSではUTF-8のままです。
</div>

@subsubsection JsonISerialzier 3-2-2.回復用JsonSerialzier
@link theolizer::JsonISerializer::JsonISerializer(std::istream&,bool)
@copybrief theolizer::JsonISerializer::JsonISerializer(std::istream&,bool)
@endlink

@code
JsonISerializer
(
    std::istream& iIStream,
    bool iNoThrowException=false
);
@endcode

|パラメータ名|意味|
|------------|----|
|iIStream|入力元のストリーム(ofstreamならテキスト・モードでオープンして下さい）|
|iNoThrowException|例外禁止時true（省略時はfalse)|

#### 専用継承関数

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
<b>void setCharIsMultiByte(bool iCharIsMultiByte);</b><br>
iCharIsMultiByte=trueの時、std::stringをWindowsマルチ・バイト文字コードとして取り扱います。<br>
コンストラクト直後はiCharIsMultiByte=falseです。<br>
Windowsのみ機能します。それ以外のOSではUTF-8のままです。
</div>

<br>
// ***************************************************************************
@subsection BinarySerializer 3-3.独自Binary形式（BinarySerializer）
// ***************************************************************************
独自Binary形式でシリアライズする場合は、<b>theolizer/serializer_binary.h</b>をインクルードして下さい。<br>
また、ファイル・ストリームは必ず@ref バイナリ・モード でオープンして下さい。

Big Endianでエンコードします。Little Endianの処理系の場合Big Endianとの間で自動変換します。<br>
整数型は値を表現するために十分なバイト数で保存します。例えば、long long型でも値が10ならタグと値で合わせて2バイトで保存します。<br>
浮動小数点型はIEEE754フォーマットのみサポートします。バイト単位でEndian変換します。<br>
long doubleは「radix==2、digits==64、max_exponent==16384」の80ビット拡張精度形式である処理系（gcc）とbinary64である処理系(msvc)に対応しています。<br>
文字コードの変換は行いません。Endianのみ変換してシリアライズします。<br>

@subsubsection BinaryOSerialzier 3-3-1.保存用BinarySerialzier
@link theolizer::BinaryOSerializer::BinaryOSerializer(std::ostream&,unsigned,CheckMode,bool)
@copybrief theolizer::BinaryOSerializer::BinaryOSerializer(std::ostream&,unsigned,CheckMode,bool)
@endlink

@code
BinaryOSerializer
(
    std::ostream& iOStream,
    unsigned iGlobalVersionNo=kLastGlobalVersionNo,
    CheckMode iCheckMode=CheckMode::NoTypeCheck,
    bool iNoThrowException=false
);
@endcode

@link theolizer::BinaryOSerializer::BinaryOSerializer(std::ostream&,CheckMode,bool)
@copybrief theolizer::BinaryOSerializer::BinaryOSerializer(std::ostream&,CheckMode,bool)
@endlink

@code
BinaryOSerializer
(
    std::ostream& iOStream,
    CheckMode iCheckMode,
    bool iNoThrowException=false
);
@endcode

|パラメータ名|意味|
|------------|----|
|iOStream|出力先のストリーム(ofstreamは<b>@ref バイナリ・モード </b>でオープンして下さい）|
|iGlobalVersionNo|保存するグローバル・バージョン番号(省略時は最新版)|
|iCheckMode|型チェック・モード（省略時はNoTypeCheck)|
|iNoThrowException|例外禁止時true（省略時はfalse)|

@subsubsection BinaryISerializer 3-3-2.回復用BinarySerialzier
@link theolizer::BinaryISerializer::BinaryISerializer(std::istream&,bool)
@copybrief theolizer::BinaryISerializer::BinaryISerializer(std::istream&,bool)
@endlink

@code
BinaryISerializer
(
    std::istream& iIStream,
    bool iNoThrowException=false
);
@endcode

|パラメータ名|意味|
|------------|----|
|iIStream|入力元のストリーム(ofstreamは<b>@ref バイナリ・モード </b>でオープンして下さい）|
|iNoThrowException|例外禁止時true（省略時はfalse)|

<br>
// ***************************************************************************
@subsection FastSerializer 3-4.メモリ内専用のFast形式（FastSerializer）
// ***************************************************************************
FastSerializerの使用目的はデータ構造のプログラム内コピーです。外部プログラムとのデータ交換は想定していません<br>

Theolizerが内部的に使用していますので、他のシリアライザのヘッダをインクルードすれば改めてヘッダをインクルードする必要はありません。<br>
もし、他のシリアライザを使用しない時は、theolizer/serializer.hをインクルードして下さい。<br>
また、ストリームはstd::stringstreamを用いることを想定していますが、もしも、ファイル・ストリームを与える場合は必ず@ref バイナリ・モード でオープンして下さい。<br>
FastSerializerはデータ変換しません。バージョンの相違にも対応していません。<br>
オーナー指定ポインタでない通常のポインタは、ポイント先をシリアライズしていない場合はシャロー・コピーになります。（ポインタ値を単純にコピーする。）<br>

@subsubsection FastOSerializer 3-4-1.保存用FastSerializer
@link theolizer::FastOSerializer::FastOSerializer(std::ostream&,bool)
@copybrief theolizer::FastOSerializer::FastOSerializer(std::ostream&,bool)
@endlink

@code
FastOSerializer
(
    std::ostream& iOStream,
    bool iNoThrowException=false
);
@endcode

|パラメータ名|意味|
|------------|----|
|iOStream|出力先のストリーム(ofstreamは<b>@ref バイナリ・モード </b>でオープンして下さい）|
|iNoThrowException|例外禁止時true（省略時はfalse)|

@subsubsection FastISerializer 3-4-2.回復用FastSerializer
@link theolizer::FastISerializer::FastISerializer(std::istream&,bool)
@copybrief theolizer::FastISerializer::FastISerializer(std::istream&,bool)
@endlink

@code
FastISerializer
(
    std::istream& iIStream,
    bool iNoThrowException=false
);
@endcode

|パラメータ名|意味|
|------------|----|
|iIStream|入力元のストリーム(ofstreamは<b>@ref バイナリ・モード </b>でオープンして下さい）|
|iNoThrowException|例外禁止時true（省略時はfalse)|

#### FastSerializerを用いたグローバル関数

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
<b>template<typename tType><br>
void copySerializable(tType const& iSource, tType& oDestination);</b><br>
tType型の変数iSourceをoDestinationへコピーします。<br>
iSourceをFastSerializerでメモリ・ストリームへシリアライズし、続けてoDestinationへ回復することでコピーします。<br>
保存先指定も有効ですので、柔軟なデータ構造のコピーを容易に実装できます。
</div>

<br>
//############################################################################
@section TestProgram 4.テスト・プログラムの構造
//############################################################################
主な機能テスト・プログラムは、 <b>source/reference_and_test</b> 以下にテストの分類に応じてフォルダを分けて入れています。

<br>
// ***************************************************************************
@subsection TestProgramStructure 4-1.構造について
// ***************************************************************************
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

<br>
// ***************************************************************************
@subsection MacroForDescription 4-2.説明で用いるマクロについて
// ***************************************************************************
テスト用のマクロはtest_tool.hで定義しています。<br>
その内、使い方の説明（兼 自動テスト）で用いるマクロについてここで簡単に説明します。

### THEOLIZER_EQUAL(dLhs, dRhs, ...)
(dLhs == dRhs) ならばPASS、そうでないならFAILと判定します。<br>
PASSならば、テストの数とPASS数をインクリメントします。<br>
FAILならば、テストの数とFAIL数をインクリメントし、テストを失敗させます。<br>
また、dRhsとそれ以降のアイテム（1個以上7個まで）を標準出力へ出力します。

下記はシリアライザを使って回復したint型のaIntの値が-3000であることをチェックしています。

@dontinclude test_basic_process.cpp
@skip aInt=0;
@until THEOLIZER_EQUAL

<br>
// ***************************************************************************
@subsection MadeTestFiles 4-3.自動テストで生成されるファイルについて
// ***************************************************************************
自動テストで多数のファイルが<b>ビルド・フォルダ/Testing</b>配下に生成されます。<br>
生成されるファイルは以下の通りです。<br>

|シリアライザ|iGlobalVersionNo|iCheckMode|iNoPrettyPrint|ファイル名|
|------------|----------------|----------|--------------|----------|
|Json        |省略            |省略      |省略          |json-default.json|
|↑          |N         |NoTypeCheck     |false         |json-pp-verN-NoTypeCheck.json|
|↑          |N         |TypeCheck       |false         |json-pp-verN-TypeCheck.json|
|↑          |N         |TypeCheckByIndex|false         |json-pp-verN-TypeCheckByIndex.json|
|↑          |N         |NoTypeCheck     |true          |json-np-verN-NoTypeCheck.json|
|↑          |N         |TypeCheck       |true          |json-np-ver1-TypeCheck.json|
|↑          |N         |TypeCheckByIndex|true          |json-np-ver1-TypeCheckByIndex.json|
|Binary      |省略            |省略      |無し          |binary-default.bin|
|↑          |N         |NoTypeCheck     |無し          |binary-verN-NoTypeCheck.bin|
|↑          |N         |TypeCheck       |無し          |binary-verN-TypeCheck.bin|
|↑          |N         |TypeCheckByIndex|無し          |binary-verN-TypeCheckByIndex.bin|
|Fast        |無し            |無し      |無し          |fast-default.bin|

Nは当該テスト・プログラムの最新のグローバル・バージョン番号となります。<br>
1～Nについてテストします。

*/