//############################################################################
/*!
    @brief      ドキュメント・ファイル－使用方法（個別）
    @ingroup    Documents
    @file       7d.destinations.h
    @author     Yoshinori Tahara
    @date       2017/01/23 Created
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
    @page Destinations 保存先指定について
ここでは、保存先指定について説明します。

<br>
//############################################################################
@section HowToSpecifySaving 1.指定方法
//############################################################################

@subsection HowToSpecifySaving11 1-1.シリアライズの有無指定
クラスや構造体に含まれるメンバ変数をシリアライズするかしないか指定できます。
指定方法は２種類あります。
侵入型半自動クラスについて指定可能です。(*1)

1. デフォルト保存クラス：メンバ変数をデフォルトでシリアライズするクラス<br>
シリアライズ指定していないメンバ変数はシリアライズ対象となります。<br>

2. デフォルト非保存クラス：メンバ変数をデフォルトではシリアライズしないクラス
シリアライズ指定していないメンバ変数はシリアライズ対象としません。

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
<b>(*1)</b><br>
非侵入型完全自動クラスのメンバ変数への指定も可能ですが、クラス定義ソースの修正が必要であるため、現状では意味がありません（侵入する必要がある）ので、非サポートとします。
</div>

@subsection HowToSpecifySaving12 1-2.保存先指定
Theolizerは、クラスを複数のファイルや通信先に分割して保存／回復できます。<br>
そのためには<br>

1. 保存先を定義する<br>
特殊なenum型を定義するイメージです。

2. 侵入型半自動クラスのメンバ変数に保存先を指定する。
複数の保存先を指定できます。

3. シリアライザをコンストラクトする際に保存先を指定する。
通常は保存先を１つ指定しますが、複数指定することも可能です。

これにより、2と3で指定された保存先集合間に共通な保存先がある場合、2のメンバ変数が3で生成したシリアライザに渡したstreamへ保存、streamから回復されます。

なお、両者とも保存先を明示的に指定しなかった場合、「保存先無し」の意味ではなく「全ての保存先」の意味になります。2に保存先を明示的に指定していない場合、3でどの保存先を指定しても常にシリアライズされます。また、3で保存先を明示的に指定していない場合、2にどの保存先を指定していても常にシリアライズされます。

@subsection HowToSpecifySaving13 1-3.指定方法

@subsubsection HowToSpecifySaving131 1-3-1.保存先の定義方法
THEOLIZER_DESTINATIONS()マクロで定義します。<br>
通常のenum型の定義と同じく、保存先を用いる場所全てより前に定義して下さい。<br>
構文は次の通りです。<br>

THEOLIZER_DESTINATIONS(前THEOLIZER_DESTINATIONS()マクロ最後の保存先, 保存先0, 保存先1, ... );<br>

マクロ展開の都合上、THEOLIZER_DESTINATIONS()１つで最大8つの保存先を定義できます。8つを超える保存先を定義したい場合、THEOLIZER_DESTINATIONS()を続けて記述して下さい。例えは保存先を12個定義したい場合は、下記のようになります。

@code
THEOLIZER_DESTINATIONS(All,   Dest0, Dest1,  Dest2,  Dest3,  Dest4,  Dest5,  Dest6,  Dest7);
THEOLIZER_DESTINATIONS(Dest7, Dest8, Dest9,  Dest10, Dest11);
@endcode
シンボルはAll以外を自由に使って頂いて良いです。文字はenum型のシンボル名に使えるものを全て使えます。<br>
なお、Allは事前定義しています。「全ての保存先」の意味を持ち、内部的に使用しています。

また、各保存先はtheolizer::destination名前空間に配置されます。また、この名前空間にはtheolizerDの別名を定義しています。例えばDest1はtheolizerD::Dest1として指定します。

@subsubsection HowToSpecifySaving132 1-3-2.デフォルト保存／非保存クラスの指定方法
「@ref HalfAutoClass 」をTHEOLIZER_INTRUSIVE()マクロで指定しますが、その先頭パラメータで指定します。<br>
デフォルト保存クラス　：THEOLIZER_INTRUSIVE(<b>CS</b>, (クラス名), バージョン番号);<br>
デフォルト非保存クラス：THEOLIZER_INTRUSIVE(<b>CN</b>, (クラス名), バージョン番号);<br>

@subsubsection HowToSpecifySaving133 1-3-3.メンバ変数への指定方法
メンバ変数への指定にはTHEOLIZER_ANNOTATE()マクロをメンバ変数定義の最後、;の前に書きます。<br>
保存しない：THEOLIZER_ANNOTATE(<b>FN</b>)<br>
保存する　：THEOLIZER_ANNOTATE(<b>FS</b>)<br>
保存し、保存先も指定する：THEOLIZER_ANNOTATE(FS:`<保存先のリスト>`))<br>

@subsubsection HowToSpecifySaving134 1-3-4.シリアライザへの指定方法

`シリアライザ名<保存先のリスト> インスタンス名(パラメータ);`の構文で指定します。

@subsection HowToSpecifySaving14 1-4.サンプル・ソース

@subsubsection HowToSpecifySaving141 1-4-1.保存の有無指定

<br>
<b>デフォルト保存クラス例</b> (source/reference_and_test/basic/test_destinations.h）<br>

@snippet  basic/test_destinations.h DefaultSave

<b>説明：</b>
|メンバ変数|指定|説明|
|----------|----|----|
|mNoAnnotate|指定無し|デフォルト保存クラスなので、シリアライズされます|
|mAnnotateSave|保存する|シリアライズされます|
|mAnnotateNonSave|保存しない|シリアライズされません|


<b>デフォルト非保存クラス例</b> (source/reference_and_test/basic/test_destinations.h）<br>

@snippet  basic/test_destinations.h DefaultNonSave

<b>説明：</b>
|メンバ変数|指定|説明|
|----------|----|----|
|mNoAnnotate|指定無し|デフォルト非保存クラスなので、シリアライズされません|
|mAnnotateSave|保存する|シリアライズされます|
|mAnnotateNonSave|保存しない|シリアライズされません|

<br>
<b>保存と回復ソース：(source/reference_and_test/basic/test_destinations.cpp）</b><br>

@snippet  basic/test_destinations.cpp DefaultSave

以上の実行で保存されるファイルは次の通りです。
@code

{
    "SerialzierName":"JsonTheolizer",
    "GlobalVersionNo":1,
    "TypeInfoList":[1]
}
{
    "mNoAnnotate":100,
    "mAnnotateSave":200
}
{
    "mAnnotateSave":2000
}

@endcode

<br>
@subsubsection HowToSpecifySaving142 1-4-2.保存先指定

保存先を指定したクラスをポリモーフィックに保存／回復するサンプルを示します。保存先をマスター・ファイルと取引ファイルへ分割するケースを使います。できるだけ小さなサンプルにしていますので少し違和感があると思いますが、雰囲気は掴めると思います。

なお、このように基底クラスへのポインタから保存／回復することもできますし、普通にクラス・インスタンスを直接保存／回復することもできます。

<b>保存先の定義</b> (source/reference_and_test/basic/common.h）<br>

@dontinclude basic/common.h
@skip THEOLIZER_DESTINATIONS
@until );

<b>クラス例</b> (source/reference_and_test/basic/test_destinations.h）<br>

顧客管理を想定してます。BaseCustomerを基底クラスとし、法人の顧客をCorporateCustomer、個人の顧客をIndividualCustomerで管理する想定です。基底クラスで名前を記録し、派生クラスはそれぞれ、資本金と売掛金残高、誕生日と提供したポイント数を記録します。名前と資本金と誕生日はマスター・ファイル、売掛金とポイント数は取引ファイルへ保存します。

@snippet  basic/test_destinations.h DestinationPoly

<b>保存ソース：(source/reference_and_test/basic/test_destinations.cpp）</b><br>

@snippet  basic/test_destinations.cpp save DestinationPoly

これにて次のように保存されます。

<b>tutorise_destinationsMaster.json:</b><br>
@code

{
    "SerialzierName":"JsonTheolizer",
    "GlobalVersionNo":1,
    "TypeInfoList":[1]
}
[
    2,
    [
        [1,"CorporateCustomer",{
            "(BaseCustomer)":{
                "mName":"Theoride Technology"
            },
            "mCapitalStock":2000000
        }]
    ],
    [
        [2,"IndividualCustomer",{
            "(BaseCustomer)":{
                "mName":"Yossi Tahara"
            },
            "mBirthday":"1962\/01\/01"
        }]
    ]
]

@endcode

誕生日が`\`でエスケープされているのは、Jsonの仕様によるものです。

<b>tutorise_destinationsTrade.json:</b><br>
@code

{
    "SerialzierName":"JsonTheolizer",
    "GlobalVersionNo":1,
    "TypeInfoList":[1]
}
[
    2,
    [
        [1,"CorporateCustomer",{
            "(BaseCustomer)":{
            },
            "mTradeAccounts":500000
        }]
    ],
    [
        [2,"IndividualCustomer",{
            "(BaseCustomer)":{
            },
            "mPoint":12000
        }]
    ]
]

@endcode

<b>保存データのまとめ：</b><br>

|クラス|メンバ変数|内容|保存先|
|------|----------|----|------|
|CorporateCustomer|mName|"Theoride Technology"|マスター・ファイル|
||mCapitalStock|2,000,000|マスター・ファイル|
||mTradeAccounts|500,000|取引ファイル|
|IndividualCustomer|mName|"Yossi Tahara"|マスター・ファイル|
||mBirthday|1962/01/01|マスター・ファイル|
||mPoint|12,000|取引ファイル|

<br>
<b>回復ソース：(source/reference_and_test/basic/test_destinations.cpp）</b><br>

@snippet  basic/test_destinations.cpp load DestinationPoly

THEOLIZER_PROCESS(aSerializerMaster, aList);にてマスター・ファイルから回復してます。<br>
THEOLIZER_PROCESS(aSerializerTrade, aList);にて取引ファイルから同じインスタンスへ回復してます。<br>
これにより分割して保存したクラスを合成して回復します。<br>
以上により回復したデータは次のようになります。

@code

{
    "SerialzierName":"JsonTheolizer",
    "GlobalVersionNo":1,
    "TypeInfoList":[1]
}
[
    2,
    [
        [1,"CorporateCustomer",{
            "(BaseCustomer)":{
                "mName":"Theoride Technology"
            },
            "mCapitalStock":2000000,
            "mTradeAccounts":500000
        }]
    ],
    [
        [2,"IndividualCustomer",{
            "(BaseCustomer)":{
                "mName":"Yossi Tahara"
            },
            "mBirthday":"1962\/01\/01",
            "mPoint":12000
        }]
    ]
]

@endcode

<b>回復結果のまとめ：</b><br>

|クラス|メンバ変数|内容|
|------|----------|----|
|CorporateCustomer|mName|"Theoride Technology"|
||mCapitalStock|2,000,000|
||mTradeAccounts|500,000|
|IndividualCustomer|mName|"Yossi Tahara"|
||mBirthday|1962/01/01|
||mPoint|12,000|

<br>
@subsubsection HowToSpecifySaving143 1-4-3.間違い易い指定について
クラス・インスタンスをメンバ変数として持つクラスについて、注意点があります。他のクラスを含むクラスを親クラス、他のクラスに含まれるクラスを子クラスとします。<br>
この時、親クラス側で子クラス型のメンバ変数に保存先をAと指定し、子クラスのメンバ変数xに保存先をBとしていた場合、親クラスをBへ保存した時にxが保存されません。これは仕様ですがミスし易い部分です。<br>

以下具体例で説明します。<br>
親クラスをDestinationParent、子クラスをDestinationChildとします。<br>
親クラスはDestinationChild型のmDestinationChildメンバ変数を持っていて、これは保存先としてDestAを指定します。子クラスは２つのメンバ変数mAnnotateAとmAnnotateBを持ち、それぞれ保存先としてDestA、DestBを指定します。<br>

<b>保存先の定義</b> (source/reference_and_test/basic/common.h）<br>

@dontinclude basic/common.h
@skip THEOLIZER_DESTINATIONS
@skip );
@skip THEOLIZER_DESTINATIONS
@until );

<b>クラス例</b> (source/reference_and_test/basic/test_destinations.h）<br>

@snippet  basic/test_destinations.h DestinationChild/Parent

<b>保存ソース：(source/reference_and_test/basic/test_destinations.cpp）</b><br>

これにて次のように保存されます。

<b>tutorise_destinationsA.json:</b><br>
@code

{
    "SerialzierName":"JsonTheolizer",
    "GlobalVersionNo":1,
    "TypeInfoList":[1]
}
{
    "mAnnotateA":100,
    "mDestinationChild":{
        "mAnnotateA":300
    }
}

@endcode

mDestinationChildのmAnnotateBはDestBのみ保存なので、tutorise_destinationsA.jsonには保存されません。

<b>tutorise_destinationsB.json:</b><br>
@code

{
    "SerialzierName":"JsonTheolizer",
    "GlobalVersionNo":1,
    "TypeInfoList":[1]
}
{
    "mAnnotateB":200
}

@endcode

tutorise_destinationsB.jsonには、指示通りmDestinationChildが含まれません。
上記のどちらにもmDestinationChildのmAnnotateBが保存されないことに注意して下さい。

<b>回復ソース：(source/reference_and_test/basic/test_destinations.cpp）</b><br>

@snippet  basic/test_destinations.cpp load DestinationChild/Parent

以上により回復したデータは次のようになります。<b>mDestinationChild.mAnnotateBが0のままです。</b>
これはミスを誘発し易いですので、子クラスに保存先を指定する際は慎重に行って下さい。<br>
なお、基底クラスには親クラス側に保存先を指定できていため、同様のミスは発生しません。

@code

{
    "SerialzierName":"JsonTheolizer",
    "GlobalVersionNo":1,
    "TypeInfoList":[1]
}
{
    "mAnnotateA":100,
    "mAnnotateB":200,
    "mDestinationChild":{
        "mAnnotateA":300,
        "mAnnotateB":0
    }
}

@endcode

<br>
//############################################################################
@section TestSpecifySaving 2.保存先指定の網羅的な使用例（自動テスト）の説明
//############################################################################

@subsection TestSpecifySaving21 2-1.保存の有無指定のテスト
これは使い方説明で用いたDefaultSaveとDefaultNonSaveの保存／回復を、全てのシリアライザ、全ての書式でテストしています。

<b>source/reference_and_test/basic/test_destinations.cpp</b>でテスト関数を定義してます。<br>

1. 保存処理<br>
template<class tSerializer><br>
void saveSpecifySaving(tSerializer& iSerializer)の前2つ<br>
<br>

2. 回復処理<br>
template<class tSerializer><br>
void loadSpecifySaving(tSerializer& iSerializer)の前2つ<br>

<br>
@subsection TestSpecifySaving22 2-2.保存先指定のテスト
<b>source/reference_and_test/basic/test_destinations.h</b>でテスト用のクラスを定義してます。<br>

DestinationTestChildクラスとDestinationTestParentクラスを用います。両方ともデフォルト保存型です。<br>
前者はint型、後者はDestinationTestChild型のメンバ変数を下記のように持ちます。

|メンバ変数|保存先指定|
|----------|----------|
|mNoAnnotate|保存先指定無し|
|mAnnotateA|DestAへ保存|
|mAnnotateB|DestBへ保存|
|mAnnotateAB|DestAとDestBの両方へ保存|
|mAnnotateC|DestCへ保存|
|mAnnotateN|非保存指定|

<b>source/reference_and_test/basic/test_destinations.cpp</b>でテスト関数を定義してます。<br>
下記組み合わせでテストしています。

|シリアライザ|テストしている関数|
|------------|------------------|
|保存先指定無し|saveSpecifySaving()とloadSpecifySaving()内の最後の１つ|
|DestAのみ|saveDestinations()の先頭で保存し、loadDestinations()で単独回復|
|DestBのみ|saveDestinations()の２番目で保存し、loadDestinations()で合成回復|
|DestAとDestB|saveDestinations()とloadDestinations()の最後の１つ|

以上により、下記組み合わせのテストを行っています。

|シリアライザ|メンバ変数|保存先指定されたメンバ変数内のメンバ変数|
|------------|----------|----------------------------------------|
|指定無し|指定無し|指定無し(mNoAnnotate)|
||単独指定有り|親と同じ指定有り(mAnnotateA, mAnnotateB)|
|||親と一部同じ複数指定有り(mAnnotateAB)|
|||親と異なる指定有り(mAnnotateC)|
|||保存無し指定(mAnnotateN)|
||複数指定有り|上記と同じセット|
||保存無し指定|上記と同じセット|
|単独指定有り(DestA)|上記と同じセット|上記と同じセット|
|単独指定有りで合成回復(DestB)|上記と同じセット|上記と同じセット|
|複数指定(DestA,DestB)|上記と同じセット|上記と同じセット|

*/
