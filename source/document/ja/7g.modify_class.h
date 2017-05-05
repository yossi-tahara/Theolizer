//############################################################################
/*!
    @brief      ドキュメント・ファイル－使用方法（個別）
    @ingroup    Documents
    @file       7g.modify_class.h
    @author     Yoshinori Tahara
    @date       2017/03/19 Created
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
    @page ChangingClass クラスのアップデート／バージョン・アップ方法
ここでは、クラス(classとstruct)を修正した時、古いプログラムが保存したデータを回復するための各種指定方法を説明します。<br>


<br>
//############################################################################
@section HowToModifyClass1 1.バージョン番号を変えないでクラスを修正
//############################################################################

クラスのバージョン番号の更新無しで行う修正は、enum型と異なりTHEOLIZER_ANNOTATE()を用いて指定することは特にありません。通常通りクラス定義を変更するだけです。

その際、@ref Basic121 で説明した「名前対応」と「順序対応」でできることが異なります。<br>
また、Theolizerは考え方としては配列も一種のクラス（複数の変数の集まり）として取り扱います。<br>
例えば2次元配列は1次元配列の配列とします。これはC++の考え方に準じます。<br>

<br>
@subsection HowToModifyClass11 1-1.名前対応の場合
メンバ変数のメモリ上のデータとシリアライズ・データ間を変数名で対応しますので、柔軟な対応が可能です。<br>
- メンバ変数の追加<br>
メンバ変数を任意の位置へ追加できます。古いシリアライズ・データを回復する時、新規追加したメンバ変数の値は変更せず、そのまま維持します。ただし、オーナー指定ポインタへ回復する際に領域を獲得した時はコンストラクタにて初期化された値となります。<br>
<br>

- メンバ変数の削除<br>
任意の位置のメンバ変数を削除できます。古いシリアライズ・データには削除したメンバ変数に対応するデータが含まれていますが、そのデータは破棄されます。<br>
<br>

- メンバ変数の順序変更<br>
メンバ変数の定義順序の変更が可能です。メンバ変数名で対応付けて正しい変数へ回復します。<br>
<br>

- 基底クラスについて<br>
基底クラスの追加／削除／順序変更はメンバ変数の変更に準じます。<br>
なお、基底クラスは"{基底クラス名}"と言う名前でシリアライス・データへ記録します。<br>
<br>

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
<b>注意事項：</b><br>
enum型はバージョン・アップ無しでのシンボル名／値変更に対応していましたが、メンバ変数名を変更する時はバージョン・アップが必要です。原理的には対応可能ですので要望があれば対応します。
</div>

<br>
@subsection HowToModifyClass12 1-2.順序対応の場合
メンバ変数のメモリ上のデータとシリアライズ・データ間を双方の順序で対応します。例えば、先頭のメンバ変数はシリアライズ・データ先頭から回復されます。そのため、変更対応はあまりできません。<br>
- メンバ変数の追加<br>
メンバ変数の並びの最後へのみ追加できます。古いシリアライズ・データを回復する時、新規追加したメンバ変数の値は変更せず、そのまま維持します。ただし、オーナー指定ポインタへ回復する際に領域を獲得した時はコンストラクタにて初期化された値となります。<br>
メンバ変数の削除と併用するのは危険です。順序だけで対応するため、シリアライズ・データに記録された旧メンバ変数を新メンバ変数へ回復しようとします。どうしても必要な場合はバージョン番号変更により対応して下さい。<br>
<br>

- メンバ変数の削除<br>
メンバ変数の並びの最後からのみ削除できます。（途中を削除することはできません。）古いシリアライズ・データには削除したメンバ変数に対応するデータが含まれていますが、そのデータは破棄されます。<br>
メンバ変数の追加と同様、メンバ変数の削除を追加と併用するのは危険です。<br>
<br>

- メンバ変数の順序変更<br>
頭から順に回復しますので、定義順序の変更には対応できません。<br>
<br>

- 基底クラスについて<br>
基底クラスの追加／削除／順序変更はメンバ変数の変更に準じます。<br>
<br>

<br>
@subsection HowToModifyClass13 1-3.配列の場合
- 考え方<br>
配列は、順序対応に準じた考え方です。<br>
<br>

- 要素数の増加<br>
要素を増やすと、従来の要素列の最後に追加されます。古いシリアライズ・データを回復する時、新規追加された要素の値は変更せず、そのまま維持します。<br>
<br>

- 要素数の減少<br>
要素を減らすと、従来の要素列の際がから削除されます。古いシリアライズ・データには削除された要素に対応するデータが含まれていますが、そのデータは破棄されます。<br>
<br>

- 配列の要素数の上限<br>
バージョン番号変更時に要素毎の引き継ぎが必要です。それをコンパイル時に再帰関数を用いて処理していますので、あまり大きな配列に対応できません。<br>
現在、プリビルド版を提供している環境では要素数はsource/reference_and_test/all_common.hのkArrayMaxでテストしています。（v0.5.0では160）<br>
<br>

<br>
@subsection HowToModifyClass14 1-4.違反した場
例えば、順序対応において変数の並び順を変更した場合、シリアライズ・データ内の変数と異なる変数へデータを回復しようとします。もしも、同じ型の変数であった場合はそのまま回復してしまいます。もしも、異なる型の変数だった場合、２つの事態があります。
- シリアライザのコンストラクト時にTypeCheckかTypeCheckByIndexを指定した場合<br>
この場合は型チェックを行います。互換性のない型から回復しようとした場合、エラーになります。
- それ以外の場合<br>
異常動作します。多くの場合はフォーマット・エラーが発生しますが、エラーになることを保証できません。<br>
<br>
<br>
@subsection HowToModifyClass15 1-5.サンプル・ソース

@subsubsection HowToModifyClass111 1-1-1.名前対応クラス

<br>
<b>変更前のソース例：(source/reference_and_test/ver1a/test_modify_class.h）</b><br>

@dontinclude ver1a/test_modify_class.h
@skip struct ModifyClassName :
@until 以下略
@skip };
@until };

<br>
<b>変更後のソース例：(source/reference_and_test/ver1b/test_modify_class.h）</b><br>

@dontinclude ver1b/test_modify_class.h
@skip struct ModifyClassName :
@until 以下略
@skip };
@until };

<br>
<b>変更前(ver1a)にシリアライズされたModifyClassNameのデータ（該当データのみ抽出）：</b><br>

@code

{
    "{ModifyFullAuto}":{
        "mFullAuto":100
    },
    "{ModifyHalfAuto}":{
        "mHalfAuto":101
    },
    "{ModifyManual}":[
        102
    ],
    "mFullAutoMember":{
        "mFullAuto":110
    },
    "mHalfAutoMember":{
        "mHalfAuto":111
    },
    "mManualMember":[
        112
    ],
    "mShort":120,
    "mInt":121,
    "mUnsigned":122
}

@endcode

<br>
<b>それを変更後のプログラムで回復してJsonで出力：</b><br>

@code

{
    "SerialzierName":"JsonTheolizer",
    "GlobalVersionNo":1,
    "TypeInfoList":[1]
}
{
    "{ModifyManual}":[
        102
    ],
    "{ModifyFullAuto}":{
        "mFullAuto":100
    },
    "{ModifyHalfAutoX}":{
        "mHalfAutoX":""
    },
    "mManualMember":[
        112
    ],
    "mFullAutoMember":{
        "mFullAuto":110
    },
    "mHalfAutoXMember":{
        "mHalfAutoX":""
    },
    "mUnsigned":122,
    "mShort":120,
    "mLong":0
}

@endcode

下記のように回復されます。

- ver1aとver1bの両方に存在するデータ<br>
これらはそのまま回復されます。基底クラスはクラス名、メンバ変数は変数名で対応しますので、定義順序を変更しても回復できます。<br>
<br>
- 新規追加されたModifyHalfAutoX基底クラスとmHalfAutoXMemberとmLongメンバ変数<br>
ver1aのシリアライズ・データには存在しませんので、シリアライズ前の値が維持されます。<br>
サンプル・ソースでは0もしくは""（長さ0の文字列）でクリア後にデシリアライズしていますので0、""になっています。<br>
また、オーナー指定ポインタ経由で保存され、回復時にコンストラクトされた場合は、コンストラクタで設定した値が維持されます。<br>
<br>
- ver1bで削除された基底クラスModifyHalfAutoとメンバ変数mHalfAutoMember<br>
これらはシリアライズ・データ内に記録されていますが、回復先がありませんので破棄されます。
<br>

ちなみに下記コードでJsonフォーマットで標準出力へ出力したものです。
@code

{
    theolizer::JsonOSerializer<> jos(std::cout);
    THEOLIZER_PROCESS(jos, aModifyClassName);
}

@endcode

<br>
@subsubsection HowToModifyClass152 1-5-2.順序対応クラス

<br>
<b>変更前のソース例：(source/reference_and_test/ver1a/test_modify_class.h）</b><br>

@dontinclude ver1a/test_modify_class.h
@skip struct ModifyClassOrder :
@until 以下略
@skip };
@until };

<br>
<b>変更後のソース例：(source/reference_and_test/ver1b/test_modify_class.h）</b><br>

@dontinclude ver1b/test_modify_class.h
@skip struct ModifyClassOrder :
@until 以下略
@skip };
@until };

<br>
<b>変更前(ver1a)のプログラムで保存されたModifyClassOrderのデータ（該当データのみ抽出）：</b><br>

@code

[
    {
        "mFullAuto":200
    },
    {
        "mHalfAuto":201
    },
    [
        202
    ],
    {
        "mFullAuto":210
    },
    {
        "mHalfAuto":211
    },
    [
        212
    ],
    220,
    221,
    222
]

@endcode

<br>
<b>上記データを変更後のプログラムで回復してJson形式で出力：</b><br>

@code

{
    "SerialzierName":"JsonTheolizer",
    "GlobalVersionNo":1,
    "TypeInfoList":[1]
}
[
    {
        "mFullAuto":200
    },
    {
        "mHalfAuto":201
    },
    [
        202
    ],
    {
        "mFullAuto":210
    },
    {
        "mHalfAuto":211
    },
    [
        212
    ],
    220,
    221,
    222,
    {
        "mHalfAutoX":""
    },
    0
]

@endcode

新規追加されたmHalfAutoXMemberとmLongメンバ変数はシリアライズ前のデータが維持されます。<br>
同じバージョン内で可能な変更は、最後への追加、もしくは最後からの削除のどちらか一方のみですので、今回は追加してます。削除した場合は当該シリアライズ・データは単純に破棄されます。<br>
基底クラス→メンバ変数の順序でシリアライズするため、メンバ変数が無い場合は、最後の基底クラスの後への基底クラスの追加、もしくは、最後の方からの基底クラスの削除のどちらかが可能です。<br>
<br>

ちなみに下記コードでJsonフォーマットで標準出力へ出力しています。
@code

theolizer::JsonOSerializer<> jos(std::cout);
THEOLIZER_PROCESS(jos, aModifyClassOrder);

@endcode

<br>
@subsubsection HowToModifyClass153 1-5-3.配列の要素数の変更サンプル

<br>
<b>変更前のソース例：(source/reference_and_test/ver1a/test_modify_class.h）</b><br>

@dontinclude ver1a/test_modify_class.h
@skip struct ArrayTest
@until {
@skip kSizeA=
@until 以下略
@skip };
@until };

<br>
<b>変更後のソース例：(source/reference_and_test/ver1b/test_modify_class.h）</b><br>

@dontinclude ver1b/test_modify_class.h
@skip struct ArrayTest
@until {
@skip kSizeA=
@until 以下略
@skip };
@until };

<br>
<b>変更前(ver1a)のプログラムで保存されたModifyClassOrderのデータ（該当データのみ抽出）：</b><br>
（長いのでmArray3Dを省略）

@code

[
    [1,"ArrayTest",{
        "mArray1D":[
            0,
            1
        ],
        "mArray2D":[
            [
                0,
                1,
                2       //★ver1bでは破棄
            ],
            [
                1000,
                1001,
                1002    //★ver1bでは破棄
            ]
        ],
        "mArray3D":[
            省略
        ]
    }]
]

@endcode

<br>
<b>上記データを変更後のプログラムで回復してJson形式で出力：</b><br>
（長いのでmArray3Dを省略）

@code

{
    "SerialzierName":"JsonTheolizer",
    "GlobalVersionNo":1,
    "TypeInfoList":[1]
}
[
    [1,"ArrayTest",{
        "mArray1D":[
            0,
            1,
            0       //★未変更
        ],
        "mArray2D":[
            [
                0,
                1
            ],
            [
                1000,
                1001
            ],
            [
                0,  //★未変更
                0   //★未変更
            ]
        ],
        "mArray3D":[
            省略
        ]
    }]
]

@endcode

ver1bにあってver1aにない要素の内容は、ver1bで生成された時の0のまま変更されていません。<br>
また、ver1aになってver1bにない要素は破棄されています。

<br>
//############################################################################
@section HowToMakeGlobalVersionNoTable2 2.グローバル・バージョン番号テーブル生成
//############################################################################

クラスについてもバージョン・アップする際にはグローバル・バージョン番号テーブルが必要です。<br>
enum型の場合と同じですので、詳しくは@ref HowToMakeGlobalVersionNoTable を参照下さい。


<br>
//############################################################################
@section HowToModifyClass3 3.バージョン番号を変えることで可能な修正
//############################################################################

enum型と同様に各クラスへバージョン番号を割り当てることで大きな修正を行うことができます。<br>

@subsection HowToModifyClass31 3-1.名前対応の場合
名前対応クラスは下記の変更に対応できます。

- メンバ変数、および、基底クラスの追加／削除／順序変更<br>
これらに付いてはバージョン番号を変えない場合と同じです。<br>
<br>
- メンバ変数名の変更<br>
型を変更せず名前だけの変更であれば参照できますので、変更可能です。THEOLIZER_ANNOTATE()で指定します。<br>
<br>
- クラス名の変更<br>
次の場合、シリアライズ・データ中に型名が記録されていますので、クラス名を変更すると通常は型チェックに通りません。<br>
しかし、適切にリファクタリングすればクラス名の変更も可能です。<br>
  - 型チェック有り(TypeCheckByかTypeCheckByIndexを指定して派生シリアライザを生成した時)<br>
  - クラスへのポインタをオーナー指定している時（ポリモーフィズム対応のため）<br>
<br>
- 順序対応への変更<br>
クラス定義が枯れてきたら効率の良い順序対応へ変更できます。<br>
<br>

@subsection HowToModifyClass32 3-2.順序対応の場合
順序対応クラスは下記の変更に対応できます。

- メンバ変数、および、基底クラスの追加／削除／順序変更<br>
これらに付いてはバージョン番号を変えない場合と同じです。<br>
<br>
- メンバ変数名の変更とクラス名の変更<br>
名前対応の場合と同じです。<br>
<br>
- 名前対応への変更<br>
クラスの大幅変更が必要になった時に一度名前対応へ変更すると効率良く開発できます。<br>
<br>

@subsection HowToModifyClass33 3-3.配列の場合
配列は下記の変更に対応できます。なお、基底の型はクラスに限りません。プリミティブやenum型を含む任意の型に対応しています。

- 考え方<br>
配列は、順序対応に準じた考え方です。
配列の要素数はバージョン番号変更無しで変更できますが、次元数はバージョン変更時にのみ可能です。ただし、配列単独ではバージョン番号を管理できる仕組みに対応していませんので、クラス・メンバとしての配列のみ次元数変更が可能です。<br>
<br>

- 要素数の増加／減少<br>
これらに付いてはバージョン番号を変えない場合と同じです。<br>
<br>

- 次元数の増加<br>
最上位の次元へ追加されます。対応先がないものは無視されます。<br>
例えば、int array[2][3];をint array[2][3][2];と増加した場合、次のように対応します。<br>
|変更前|変更後|
|:----:|:---:|
|array[0][0]|array[0][0][0]|
|array[0][1]|array[0][0][1]|
|array[0][2]|---|
|array[1][0]|array[0][1][0]|
|array[1][1]|array[0][1][1]|
|array[1][2]|---|
        |---|array[0][2][0]|
        |---|array[0][2][1]|
        |---|array[1][0][0]|
        |---|array[1][0][1]|
        |---|array[1][1][0]|
        |---|array[1][1][1]|
        |---|array[1][2][0]|
        |---|array[1][2][1]|
<br>

- 次元数の減少<br>
最上位の次元から削除されます。対応先がないものは無視されます。<br>
例えば、int array[2][2][3];をint array[3][2];と増加した場合、次のように対応します。<br>
|変更前|変更後|
|:----:|:---:|
|array[0][0][0]|array[0][0]|
|array[0][0][1]|array[0][1]|
           |---|array[0][2]|
|array[0][1][0]|array[1][0]|
|array[0][1][1]|array[1][1]|
           |---|array[1][2]|
|array[0][0][2]|---|
|array[0][1][2]|---|
|array[1][0][0]|---|
|array[1][0][1]|---|
|array[1][0][2]|---|
|array[1][1][0]|---|
|array[1][1][1]|---|
|array[1][1][2]|---|
<br>

<br>
//############################################################################
@section HowToModifyClass4 4.バージョン番号を変えてクラス修正する仕組み概要
//############################################################################

@subsection HowToModifyClass41 4-1.コア・データ構造
バージョン毎にシリアライズ対象のメンバ変数だけを集めたクラス(TheolizerVersion)を定義しています。これは、当該バージョンで持っていたメンバ変数を管理する内部クラスです。<br>

各バージョンのTheolizerVersionは、次バージョンのTheolizerバージョンとの関係を管理します。バージョンnのTheolzierVersionの各メンバ変数には、バージョンn+1のTheolizerバージョンのどのメンバ変数と対応するのか記録され、自動的に引継ぎ処理が実行されます。<br>

また、この自動引継ぎ処理は、メンバ変数のアドレスも引継ぎますので、旧バージョンでオブジェクト追跡されていた情報も回復できます。メンバ変数名を変更してもアドレス引継ぎは機能します。ただし、メンバ変数の型を変更した場合は、アドレスを引き継げません。<br>

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
<b>重要事項：</b><br>
Theolizerドライバの主な役割はTheolizerVersionを自動生成することであり、*.theolzier.hppファイルには当該コンパイル単位でシリアライズ指定されているクラスとenum型のTheolizerVersionの定義が生成されます。旧バージョンのTheolizerVersion定義もここに入っていますので、*.theolzier.hppは自動生成ファイルですがバージョン管理システムの管理対象ソースとして登録して下さい。
</div>

@subsection HowToModifyClass42 4-2.補助データ構造
@ref Basic33 で説明したように、足並みを揃えて(Keep-step)バージョン・アップ／ダウンする変数としない変数があります。<br>

Keep-step処理する基底クラスやメンバ変数は、4-1.で説明したTheolizerVersionクラスにて管理されます。<br>
Keep-step処理しない（Non-keep-step）基底クラスやメンバ変数用には異なるクラスで管理しています。

- Non-keep-step基底クラス（非侵入型手動クラス）<br>
`TheolizerBase<基底クラス>`クラスを用いて、シリアライズ対象クラス・インスタンスへの参照を管理しています。<br>
<br>

- Non-keep-stepメンバ変数<br>
`TheolizerNonKeepStep<型>`クラスを用いています。

更に`TheolizerNonKeepStep<型>`クラスは下記のような２種類を定義しています。

- 部分特殊化　：　プリミティブ型(int型やstd::string型)
- プライマリ　：　その他のNon-keep-step全て(ポインタ、参照、非侵入型手動クラス）

@subsection HowToModifyClass43 4-3.侵入型半自動におけるバージョン・アップ／ダウン処理
-  旧バージョンのデータを保存する時の内部動作<br>
ターゲットのユーザ定義クラスから最新版のTheolizerVersionを生成後、次々とカスケードに１つ前のバージョンのTheolizerVersionをコンストラクトする際にメンバ変数を引き継ぎつつ、目的のバージョンのTheolizerVersionまで生成します。そして、そのバージョンのTheolizerVersionを使って保存処理を行います。<br>
この時、<b>１つ前のバージョンのTheolizerVersionを生成後、ユーザ定義のバージョン・ダウン処理（downVersion関数）が定義されていたら、それを呼び出します。</b><br>
<br>

-  旧バージョンのデータから回復する時の内部動作<br>
保存時と同様の流れで、目的のハージョンのTheolizerVersionまで生成します。目的のTheolizerVersionを使って回復処理を行い、生成時と逆の順序でTheolizerVersionをデストラクトしつつ、ターゲットのユーザ定義クラスへ値を戻します。<br>
この時、<b>１つ前バージョンのTheolizerVersionをデストラクトする時に、ユーザ定義のバージョン・アップ処理（upVersion関数）が定義されていたら、それを呼び出します。</b><br>
<br>

@subsection HowToModifyClass44 4-4.非侵入型手動におけるバージョン・アップ／ダウン処理
こちらは半自動型と異なり、カスケードにバージョン・アップ／ダウンを行うことはたいへん難しく、却ってあなたのプログラム開発の手間を増大させるため、カスケード処理は行いません。従って、各バージョン毎に保存処理と回復処理を記述する必要があります。<br>
しかし、対象クラスを変更する度にバージョン・アップが必要なわけではありません。対象クラスを回復する際に従来の保存データでは不足する場合にのみバージョン・アップが必要になります。<br>

また、バージョン変更の際にシリアライズ対象クラスと保存／回復処理間のI/Fに変化がなければ、旧バージョンの保存／回復処理の変更は不要です。しかし、もし、このI/Fを変更された場合は、それに合わせて旧バージョンの保存／回復処理の修正も必要となります。ですので、他のクラスとのI/Fは自由ですが、保存／回復処理とのI/Fはできるだけ変更しないことをお勧めします。<br>
<br>

@subsection HowToModifyClass45 4-5.バージョン・アップ時の注意事項
@ref Modifying にて記述した以下の点にご注意下さい。<br>

1. down/upVersion関数で変更可能な変数<br>
プリミティブ型とenum型変数のみです。<br>
クラス型メンバ変数にに含まれるプリミティブ型とenum型変数も変更可能です。更に、クラス型メンバ変数にに含まれるクラス型メンバ変数に含まれるプリミティブ型とenum型変数も変更可能です。（以下同様）<br>
<br>
2. それ以外のものは変更してはいけません。<br>
@ref Basic36 参照<br>
  - ポインタ、および、参照が指す先のインスタンス
  - 非侵入型手動クラスの基底クラス、および、メンバ変数
  - ポインタそれ自身<br>
<br>
3. upVersionで参照する変数は先に回復すること<br>
クラス分割により、異なるシリアライズ・データから回復される変数が存在します。<br>
それらをアクセスしてupVersion処理を行う場合、依存先のメンバ変数を先に回復する必要があります。<br>
見落とし易いのでご注意下さい。<br>

<br>
//############################################################################
@section HowToModifyClass5 5.バージョン番号を変えてクラスを修正する方法
//############################################################################

<br>
@subsection HowToModifyClass51 5-1.メンバ変数名の変更
リファクタリングの際にメンバ変数名を変更したい場合があると思います。<br>
単に変更するだけの場合、名前を変更した変数は削除して、別名の変数を追加された扱いとなり、変更前のプログラムで保存したシリアライズ・データから回復する際に、値を引き継ぐことができません。

メンバ変数名を変更するにはTHEOLIZER_ANNOTATE(FS:)の第一パラメータを用いて対応つけます。<br>
変更後のクラス定義で、変更前の変数名を指定します。

@code
型 メンバ変数名 THEOLIZER_ANNOTATE(FS:旧メンバ変数名);
@endcode

以下、名前対応クラス(ModifyClassName)と順序対応クラス(ModifyClassOrder)のmHalfAutoXMemberメンバ変数の変数名をmHalfAutoYMemberへ変更する例です。どちらのクラスの場合も同じです。<br>
なお、型変更には対応していません。変数名を変更して対応付ける各変数の型は同じにして下さい。<br>

なお、下記の例ではクラス名を変更していますが、クラス自体は同じものです。<br>
クラス名をModifyHalfAutoXからModifyHalfAutoYへ変更しています。クラス名の変更については次節で説明します。<br>

<br>
<b>変更前のソース例：(source/reference_and_test/ver1c/test_modify_class.h）</b><br>

@dontinclude ver1c/test_modify_class.h
@skip struct ModifyClassName :
@until 以下略
@skip THEOLIZER_INTRUSIVE
@until };

@dontinclude ver1c/test_modify_class.h
@skip struct ModifyClassOrder :
@until 以下略
@skip THEOLIZER_INTRUSIVE
@until };

<br>
<b>変更後のソース例：(source/reference_and_test/ver2a/test_modify_class.h）</b><br>

@dontinclude ver2a/test_modify_class.h
@skip struct ModifyClassName :
@until 以下略
@skip THEOLIZER_INTRUSIVE
@until };

@dontinclude ver2a/test_modify_class.h
@skip struct ModifyClassOrder :
@until 以下略
@skip THEOLIZER_INTRUSIVE
@until };

<br>
@subsection HowToModifyClass52 5-2.クラス名の変更
リファクタリングを行う時、クラス名を変更したい場合もあると思います。<br>
基本的には通常通り全てのソース・ファイルで使っているクラス名を変更します。<br>
その際、*.theolizer.hppで指定されているクラス名も変更します。ただし、旧バージョンのシリアライズ・データと対応付けるためのクラス名は変更してはいけません。<br>
前者（リファクタリング対象）は`(クラス名)`のように()で括られています。後者（変更しないクラス名）は`u8"クラス名"`のように文字列として定義されていますので、`(クラス名)`を一括修正することで*.theolizer.hppを変更できます。<br>

例えば、下記は「5-1.メンバ変数名の変更」で、クラス名をModifyHalfAutoXからModifyHalfAutoYへ変更する前の*.theolizer.hppファイル（複数あるので注意）の該当部分の一部です。<br>

<b>ModifyHalfAutoX</b>自動生成ソース部

<div style="padding: 5px; margin-bottom: 10px; border: 1px solid #c4cfe5; border-radius: 1px; background-color: #fbfcfd;">
#`ifdef  THEOLIZER_WRITE_CODE // ###### ModifyHalfAutoX ######`<br>
<br>
（中略）<br>
<br>
#`define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)`<br>
#`define THEOLIZER_GENERATED_CLASS_TYPE THEOLIZER_INTERNAL_UNPAREN`<b style="color: #ff0000;">(ModifyHalfAutoX)</b>``<br>
<br>
`//      ---<<< Version.1 >>>---`<br>
<br>
#`define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)`<br>
#`define THEOLIZER_GENERATED_CLASS_NAME()\`<br>
`　  THEOLIZER_INTERNAL_CLASS_NAME((u8"ModifyHalfAutoX"))`<br>
#`define THEOLIZER_GENERATED_ELEMENT_MAP emName`<br>
#`define THEOLIZER_GENERATED_ELEMENT_LIST()\`<br>
`　  THEOLIZER_INTERNAL_ELEMENT_N((mHalfAutoX),mHalfAutoX,etmDefault,\`<br>
`　      (theolizerD::All),\`<br>
`　      (std::string))<br>`
#`include <theolizer/internal/version_auto.inc><br>`
#`undef  THEOLIZER_GENERATED_VERSION_NO<br>`
<br>
#`endif //THEOLIZER_WRITE_CODE // ###### ModifyHalfAutoX ######<br>`
</div>

<b>ModifyClassName</b>自動生成ソース部

<div style="padding: 5px; margin-bottom: 10px; border: 1px solid #c4cfe5; border-radius: 1px; background-color: #fbfcfd;">
#`ifdef  THEOLIZER_WRITE_CODE // ###### ModifyClassName ######`<br>
<br>
#`define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)`<br>
#`define THEOLIZER_GENERATED_FULL_AUTO ModifyClassName`<br>
<br>
`//      ---<<< Version.1 >>>---`<br>
<br>
#`define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)`<br>
#`define THEOLIZER_GENERATED_CLASS_NAME()\`<br>
 　 `THEOLIZER_INTERNAL_CLASS_NAME((u8"ModifyClassName"))`<br>
#`define THEOLIZER_GENERATED_ELEMENT_MAP emName`<br>
#`define THEOLIZER_GENERATED_BASE_LIST()\`<br>
`　  THEOLIZER_INTERNAL_BASE_N(public,etmDefault,0,(ModifyManual),u8"ModifyManual")\`<br>
`　      THEOLIZER_GENERATED_SEP\`<br>
`　  THEOLIZER_INTERNAL_BASE_KN(public,etmDefault,1,(ModifyFullAuto),1,u8"ModifyFullAuto")\`<br>
 　      THEOLIZER_GENERATED_SEP\`<br>
`　  THEOLIZER_INTERNAL_BASE_KI(public,etmDefault,2,`<b style="color: #ff0000;">(ModifyHalfAutoX)</b>`,1,u8"ModifyHalfAutoX")`<br>
#`define THEOLIZER_GENERATED_ELEMENT_LIST()\`<br>
`　  THEOLIZER_INTERNAL_ELEMENT_N((mManualMember),mManualMember,etmDefault,\`<br>
`　      (theolizerD::All),\`<br>
`　      (ModifyManual))\`<br>
`　  THEOLIZER_INTERNAL_ELEMENT_KN((mFullAutoMember),mFullAutoMember,etmDefault,\`<br>
`　      (theolizerD::All),\`<br>
`　      (ModifyFullAuto),1)\`<br>
`　  THEOLIZER_INTERNAL_ELEMENT_KI((mHalfAutoXMember),mHalfAutoXMember,etmDefault,\`<br>
`　      (theolizerD::All),\`<br>
`　      `<b style="color: #ff0000;">(ModifyHalfAutoX)</b>`,1)`<br>
`　  THEOLIZER_INTERNAL_ELEMENT_N((mUnsigned),mUnsigned,etmDefault,\`<br>
`　      (theolizerD::All),\`<br>
`　      (unsigned int))\`<br>
`　  THEOLIZER_INTERNAL_ELEMENT_N((mShort),mShort,etmDefault,\`<br>
`　      (theolizerD::All),\`<br>
`　      (short))\`<br>
`　  THEOLIZER_INTERNAL_ELEMENT_N((mLong),mLong,etmDefault,\`<br>
`　      (theolizerD::All),\`<br>
`　      (long))`<br>
#`include <theolizer/internal/version_auto.inc>`<br>
#`undef  THEOLIZER_GENERATED_VERSION_NO`<br>
<br>
#`endif//THEOLIZER_WRITE_CODE // ###### ModifyClassName ######`<br>
</div>

ModifyHalfAutoXクラスの名前をModifyHalfAutoYへリファクタリングする際に*.theolizer.hppファイル内のu8"ModifyHalfAutoX"を除く(ModifyHalfAutoX)を(ModifyHalfAutoY)へ変更します。<br>

上記の場合、赤く記した３箇所の(ModifyHalfAutoX)を(ModifyHalfAutoY)へ変更することになります。<br>
なお、コメントに含まれるModifyHalfAutoXは変更してもしなくても影響はありません。

<br>
@subsection HowToModifyClass53 5-3.配列の次元数の変更
バージョン変更する際に配列の次元を増やしたり減らしたりすることができます。<br>
その際、特別な記述は不要です。<br>

<br>
<b>変更前のソース例：(source/reference_and_test/ver1c/test_modify_class.h）</b><br>

@dontinclude ver1c/test_modify_class.h
@skip struct ArrayTest
@until {
@skip kSize1=
@until 以下略
@skip };
@until };

<br>
<b>変更後のソース例：(source/reference_and_test/ver2a/test_modify_class.h）</b><br>

@dontinclude ver2a/test_modify_class.h
@skip struct ArrayTest
@until {
@skip kSize2=
@until 以下略
@skip };
@until };

<br>
@subsection HowToModifyClass54 5-4.バージョン・アップ／ダウン処理の記述方法

手順としては以下の通りです。

1. 通常通り、グローバル・バージョン番号テーブルを定義<br>
既に定義していたら、再定義は不要です。<br>
THEOLIZER_DEFINE_GLOBAL_VERSION_TABLE()マクロで定義します。<br>
詳細は@ref HowToMakeGlobalVersionNoTable を参照下さい。<br>
<br>
2. 対象のクラスが非侵入型完全自動クラスなら、侵入型半自動へ変更して、<b>一度ビルド</b><br>
通常はTHEOLIZER_INTRUSIVE()マクロを対象クラス内に記述することで「侵入型半自動」であることを指定します。<br>
詳細は@ref HalfAutoClass を参照下さい。<br>
ここでビルドしてver.1の自動生成ソースを生成します。<br>
ビルドしない場合、突然ver.2へ上げられたことになり、コンパイルできない場合があります。<br>
<br>
3. バージョン番号をインクリメント<br>
  - グローバル・バージョン番号<br>
@ref HowToMakeGlobalVersionNoTable を参照下さい。<br>
  - 対象クラスのローカル・バージョン番号<br>
@ref Basic321 、および、@ref HalfAutoClass を参照下さい。
<br>
<br>
4. down/upVersion関数雛形をコピー&ペースト<br>
後述します。<br>
<br>
5. 必要に応じてdown/upVersion関数の内容を記述<br>
後述します。<br>

@subsubsection HowToModifyClass541 5-4-1.down/upVersion関数雛形のコピー&ペースト
シリアライズ対象クラスのシリアライズ用のコードは*.theolizer.hppに自動生成されています。それと一緒にdown/upVersion関数の雛形も定義していますので、それをコピー&ペーストして下さい。

例えば、以下の通りです。

<br>
<b>対象クラスKeepStepTest：(source/reference_and_test/ver1c/test_modify_complex.h）</b><br>

@dontinclude ver1c/test_modify_complex.h
@skip struct KeepStepTest :
@until };

<br>
<b>KeepStepTestの自動生成ソース：(ビルド・フォルダ/reference_and_test/ver1c/test_modify_class.cpp.theolizer.hpp）</b><br>

@code

#ifdef  THEOLIZER_WRITE_CODE // ###### KeepStepTest ######

#if false // Sample of up/downVersion function.
template<class tTheolizerVersion, class tNextVersion>
struct KeepStepTest::TheolizerUserDefine<tTheolizerVersion, tNextVersion, 1>
{
    // Members version down.
    static void downVersion(tNextVersion const& iNextVersion, tTheolizerVersion& oNowVersion)
    {
    }

    // Members version up.
    static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)
    {
    }
};
#endif // Sample of up/downVersion function.

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE THEOLIZER_INTERNAL_UNPAREN(KeepStepTest)

//      ---<<< Version.1 >>>---

    中略

#endif//THEOLIZER_WRITE_CODE // ###### KeepStepTest ######

@endcode

この #`if` false ～ #`endif` までの間をコピーしてください。コピー先はKeepStepTestクラスの定義の後、自動生成ソースの前ならばどこでも良いです。KeepStepTestの定義直後をお勧めします。<br>

<b>コピー先ソース例：(source/reference_and_test/ver2a/test_modify_complex.h）</b><br>

@dontinclude ver2a/test_modify_complex.h
@skip struct KeepStepTest :
@until static void upVersion
@until {
@skip oNextVersion.mInt16=
@skip }
@until };

以上のTheolizerUserDefineクラス・テンプレートの最後のテンプレート引数の1がローカル・バージョン番号です。1の時はver.1とver.2の間、2の時はver.2とver.3の間のdown/upVersion処理を定義します。

バージョン・アップ／ダウン処理を記述する必要があるバージョンについてTheolizerUserDefineクラスを定義して下さい。

@subsubsection HowToModifyClass542 5-4-2.down/upVersion処理を記述する

<b>static void downVersion(tNextVersion const& iNextVersion, tTheolizerVersion& oNowVersion)関数</b><br>
iNextVersion引数は次バージョンです。<br>
oNowVersionは現バージョン(テンプレート引数で指定したバージョン)です。<br>
それぞれ@link HowToModifyClass41 TheolizerVersionクラス@endlink のインスタンスが渡ってきます。<br>
iNextVersionを参照しつつ、oNowVersionの@link Basic36 設定可能なメンバ@endlink を設定して下さい。<br>
<br>
<b>設定例：</b>
@code
oNowVersion.mData0=iNextVersion.mData0-100;
@endcode

なお、基底クラスは実装上の都合により、メンバ変数としてアクセスする必要が有ります。<br>
クラスAの基底クラスへアクセスする時は、クラス名にTheolizerBaseを連結したメンバ変数を使ってアクセスして下さい。<br>
例えば、次バージョンの基底クラス<b>FooClass</b>のmBarメンバへアクセスする場合、iNextVersion.<b>FooClass</b>TheolizerBase.mBar でアクセスして下さい。<br>
<br>

<b>static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)関数</b><br>
iNowVersionは現バージョン(テンプレート引数で指定したバージョン)です。<br>
tNextVersion引数は次バージョンです。<br>
それぞれ@link HowToModifyClass41 TheolizerVersionクラス@endlink のインスタンスが渡ってきます。<br>
iNextVersionを参照しつつ、oNowVersionの@link Basic36 設定可能なメンバ@endlink を設定して下さい。<br>
<br>
<b>設定例：</b>
@code
oNextVersion.mData0=iNowVersion.mData0+100;
@endcode

基底クラスのアクセス方法はdownVersion関数と同じです。<br>


upVersion()関数の設定可能なメンバ変数は、set()関数を持っています。これは例えば、変数の型を変更したような場合に用います。（型変更された変数の自動引継ぎをTheolizerはサポートしていませんので、手動で引継ぎが必要だからです。）<br>
例えば、ver.1ではint型であったmFooメンバ変数を、ver.2でlong型へ変更した場合、下記のような処理を記述して引継ぎして下さい。<br>
この時、set関数の第一パラメータの型は引継ぎ先の型となりますので、この例ではlong型です。<br>

@code
oNextVersion.mFoo.set(iNextVersion.mFoo, iNextVersion.mFoo.getDoSucceed());
@endcode

getDoSucceed()関数は、そのメンバ変数の引継ぎが必要かどうかを示します。通常はデシリアライズ時に回復された場合trueとなります。また、set(xxx, true)で設定された場合もtrueになります。最終的に最新版のTheolizerVersionでtrueだった場合、その変数の値がシリアライズ対象クラスの該当メンバへ設定されます。<br>
従って、上記の文により、mFooがデシリアライズされていた場合に限り、その値が次バージョンのmFoo変数へ引き継がれます。<br>

<br>
//############################################################################
@section HowToModifyClass6 6.網羅的な使用例（自動テスト）の説明
//############################################################################

<br>
@subsection HowToModifyClass61 6-1.クラス変更のテスト
クラス変更の基本的な網羅的なテストは @ref TestProgram で説明した各フォルダの下記2つのファイルにて実装しています。

- test_modify_class.h
- test_modify_class.cpp

名前対応クラス(ModifyClassName)、順序対応クラス(ModifyClassOrder)、配列メンバを持つクラス(ArrayTest)について下記の修正を行い、@ref TestProgram412 の各バージョン間で正しく回復できることを確認しています。<br>
それぞれのメンバ変数の型は、非侵入型完全自動クラス、侵入型半自動クラス、非侵入型手動クラス、および、プリミティブ型をテストしています。<br>

|バージョン|ModifyClassName|ModifyClassOrder|ArrayTest|
|----------|---------------|----------------|---------|
|ver1a→ver1b|基底クラスとメンバ変数について<br>　順序変更、追加、削除|メンバ変数の追加|要素数の増加と減少|
|ver1b→ver1c|基底クラス、メンバ変数(クラス、enum)について<br>　旧バージョンのみのシリアライズ|---|---|
|ver1c→ver2a|クラス名変更<br>クラス型メンバ変数名変更|クラス名変更<br>クラス型メンバ変数名変更|次元数増加|
|ver2a→ver3a|プリミティブ型メンバ変数名変更<br>クラス配列型メンバ変数追加|プリミティブ型メンバ変数名変更|次元数減少|
|ver3a→ver3b|基底クラスとメンバ変数について<br>　順序変更、追加、削除<br>クラス配列型メンバ変数削除|クラス型とプリミティブ型について<br>　メンバ変数削除|要素数上限|


<br>
@subsection HowToModifyClass62 6-2.クラス変更の複合テスト
クラス変更の複合的で網羅的なテストは @ref TestProgram で説明した各フォルダの下記2つのファイルにて実装しています。

- test_modify_complex.h
- test_modify_complex.cpp

ここでは、大きく下記の２種類についてテストしています。
- オブジェクト追跡している時のバージョン変更<br>
用いる主なクラスは、ポイントされる側のメンバ変数を持つクラス(PointeeInclude)、ポインタ型メンバ変数を持つクラス(PointerInclude)。<br>
ポイントされる変数について値だけでなく、アドレスも引き継がれることを確認しています。<br>
変数名を変更した時に値だけでなく、アドレスも引き継がれることを確認しています。<br>
<br>
- バージョン・アップ／ダウン処理<br>
用いる主なクラスは、主なdown/upVersionテスト用のVersionUpDownTestクラス、Keep-step変数のテストとNon-keep-step変数用の外側のupVersionテスト用のKeepStepTestクラスです。<br>
KeepStepTestはVersionUpDownTestを基底クラス、および、メンバ変数として含みます。<br>
「upVersionの全組合せテスト」では、下記の全組合せをテストします。<br>
  - データ回復した場合、しなかった場合
  - set()しなかった場合、DoSucceed=falseでset()した場合、DoSucceed=trueでset()した場合
  - 代入しなかった場合、代入した場合

|バージョン|オブジェクト追跡|バージョン・アップ／ダウン処理|
|----------|----------------|------------------------------|
|ver1a→ver1b|---|---|
|ver1b→ver1c|---|テスト準備|
|ver1c→ver2a|被ポインタとポインタ追加|Non-keep-step(enum型とプリミティブ型)について<br>　upVersionの全組合せテスト|
|ver2a→ver3a|変数名変更|型変更対応<br>外側のupVersion()優先|
|ver3a→ver3b|---|---|

*/
