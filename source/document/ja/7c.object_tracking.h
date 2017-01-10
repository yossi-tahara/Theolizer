//############################################################################
/*!
    @brief      ドキュメント・ファイル－使用方法（個別）
    @ingroup    Documents
    @file       7c.object_tracking.h
    @author     Yoshinori Tahara
    @date       2017/01/06 Created
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
    @page ObjectTracking オブジェクト追跡について
ここでは、オブジェクト追跡について説明します。

<br>
//############################################################################
@section HowToObjectTracking 1.オブジェクト追跡の使い方
//############################################################################

<br>
@subsection HowToObjectTracking11 1-1.指定方法
<b>・ポインタ型の指定（通常通りのシリアライズ指定です）</b>

1. メンバ変数の場合は、通常通り保存指定
2. THEOLIZER_PROCESS()マクロ

<b>・オーナー・ポインタ型の指定（オーナー指定）</b>

1. メンバ変数の場合は、THEOLIZER_ANNOTATE(FS:...<...>Owner)
2. THEOLIZER_PROCESS_OWNER()マクロ

ボインタのみ指定できます。<br>
また、回復時は初期値が必要です。nullptr、もしくは、適正なインスタンスへのポインタを設定して下さい。ポイント先インスタンスのクラスとシリアライズ・データ内のクラスが同じならば設定されていたインスタンスを維持したまま回復します。(これは保存先指定機能により複数のファイルに別々に保存されたメンバを統合するための機能です。)<br>
もし、クラスが異なる場合は解放し、シリアライズ・データと同じクラスをnewして回復します。nullptrの時も同様です。

<b>・オブジェクト追跡指定</b>

1. メンバ変数の場合は、THEOLIZER_ANNOTATE(FS:...<...>Pointee)
2. THEOLIZER_PROCESS_POINTEE()マクロ

クラスに対してこれらの指定は不要ですが、指定しても問題はありません。

@subsection HowToObjectTracking12 1-2.サンプル・ソース

<b>サンプル用のクラス定義：(source/reference_and_test/version/ver1/test_object_tracking.h）</b>

@dontinclude test_object_tracking.h
@skip ObjectTrackingClass
@until };

ObjectTrackingClass全体はオブジェクト追跡するクラスのサンプルです。また、メンバ変数はTHEOLIZER_ANNOTATE()マクロによるオブジェクト追跡関連指定のサンプルです。<br>
- mIntメンバ変数はオブジェクト追跡するよう指定されたメンバ変数です。
- mShortメンバ変数はオーナー指定されたメンバ変数です。

<b>保存処理：(source/reference_and_test/version/ver1/test_object_tracking.cpp）</b>

@dontinclude test_object_tracking.cpp
@skip "tutoriseObjectTracking() start"
@skip {
@until aSerializer.clearTracking();
@until }

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
<b>makeAutoRelease()</b><br>
これはnewで獲得した領域を自動的にdeleteするためのヘルパー関数です。source/reference_and_test/version/ver1/common.hで定義しています。たいへん小さいですので、興味のある方は覗いてみて下さい。
</div>

これにより、下図のようなデータ構造が生成され、図の順序でデータが保存されます。

@image html object_tracking1.png

<b>tutorise_object_tracking.json</b>ファイルは下記となります。
（//以下は説明のために書き込みました。）

    {
        "SerialzierName":"JsonTheolizer",
        "GlobalVersionNo":1,
        "TypeInfoList":[1]
    }
    1                           // オブジェクトID=1へのポインタ(aLongPtr)
    2                           // オブジェクトID=2へのポインタ(aObjectTrackingClassPtr)
    [1,100]                     // オブジェクトID=1のインスタンス(aLong)
    [2,{                        // オブジェクトID=2のインスタンス(aObjectTrackingClass)
        "mInt":[3,200],         // オブジェクトID=3のインスタンス(mInt)
        "mShort":[4,300]        // オブジェクトID=4のインスタンス(mShort)
    }]
    [5,101]                     // オブジェクトID=5のインスタンス(aLongOwner)
    [6,"ObjectTrackingClass",{  // オブジェクトID=6のインスタンス(aObjectTrackingOwner)
        "mInt":[7,201],         // オブジェクトID=7のインスタンス(mInt)
        "mShort":[8,301]        // オブジェクトID=8のインスタンス(mShort)
    }]
    5                           // オブジェクトID=5へのポインタ(aLongPtr2)
    6                           // オブジェクトID=6へのポインタ(aObjectTrackingClassPtr2)
    7                           // オブジェクトID=7へのポインタ(aIntPtr)
    8                           // オブジェクトID=8へのポインタ(aShortPtr)

<b>回復処理：(source/reference_and_test/version/ver1/test_object_tracking.cpp）</b><br>
元のデータ構造を回復できていることをチェックしています。

@skip 回復
@skip {
@until aSerializer.clearTracking();
@until }


<br>
//############################################################################
@section Polymorphism 2.ポリモーフィズムの使い方
//############################################################################

@subsection Polymorphism31 3-1.使い方

ポリモーフィックな基底クラスへのポインタをオーナ・ポインタとしてシリアライズすることで、ポリモーフィックに回復されます。つまり、派生クラスのインスタンスを基底クラスへのポインタでポイントして保存して回復した場合、派生クラスのインスタンスとして回復されます。

そのように動作させるために、２つの注意点があります。

1. 基底クラスは仮想関数を最低1つ持つ必要が有ります。<br>
これがないと、基底クラスへのポインタが指す派生クラスのインスタンスの型を動的に判定できないためです。
2. 派生クラスはTHEOLIZER_REGISTER_CLASS()マクロで派生クラスであることを指定して下さい。<br>
基底クラスへのポインタ経由でシリアライズしたい派生クラスは必ず指定して下さい。<br>
指定漏れすると、シリアライズ処理する時に"Can not find the derived class for <基底クラス>."エラーになります。

また、「@ref Basic164 」も参照下さい。

<b>サンプル用のクラス定義：(source/reference_and_test/version/ver1/test_polymorphism.h）</b>

@dontinclude test_polymorphism.h
@skip //-
@until THEOLIZER_REGISTER_CLASS((PolyDerived1));

<b>THEOLIZER_REGISTER_CLASS()</b>による派生クラスの指定を忘れないようにお願いします。<br>
また、クラス名を()で囲って指定する必要が有りますのでご注意下さい。

<b>保存処理：(source/reference_and_test/version/ver1/test_polymorphism.cpp）</b>

@dontinclude test_polymorphism.cpp
@skip "tutorisePolymorphism() start"
@skip {
@until aSerializer.clearTracking();
@until }

<b>回復処理：(source/reference_and_test/version/ver1/test_polymorphism.cpp）</b><br>
元のデータ構造を回復できていることをチェックしています。

@skip 回復
@skip {
@until aSerializer.clearTracking();
@until }

<b>tutorise_object_tracking.json</b>ファイルは下記となります。

    {
        "SerialzierName":"JsonTheolizer",
        "GlobalVersionNo":1,
        "TypeInfoList":[1]
    }
    [1,"PolyDerived0",{
        "(PolyBase)":{
            "mInt":100
        },
        "mShort":200
    }]
    [2,"PolyDerived1",{
        "(PolyBase)":{
            "mInt":1000
        },
        "mString":"string"
    }]

メンバ変数名は"メンバ変数名"として記録されます。基底クラスにはメンバ変数名がないため、"(基底クラス名)"として記録しています。これにより、基底クラスの定義順序変更にも対応できます。

<br>
//############################################################################
@section ClearTracking 3.オブジェクト追跡の仕組み
//############################################################################

@subsection HowToObjectTracking31 3-1.ポインタをシリアライズする仕組み
Theolizerはboost::serializationと同様にポインタをファイルへ保存し、そのファイルを回復した時、元のポイント構造を回復できる仕組みを実装しています。

下記のようなオブジェクト追跡処理により実現しています。

<b>・ポインタの保存</b><br>
1. オブジェクトIDテーブル<br>
インスタンスを保存する時に、インスタンスの先頭アドレスに対してオブジェクトIDを割り当て、インスタンスの先頭アドレスとオブジェクトIDの対応表を生成します。<br>

2. ポインタを保存<br>
ポインタの指すアドレスに対応するオブジェクトIDをオブジェクトIDテーブルで求め、オブジェクトIDを保存します。

<b>・ポインタの回復</b><br>
1. オブジェクトIDテーブル<br>
インスタンスを回復する時に、オブジェクトIDに対応するインスタンスの先頭アドレスを記録します。

2. ポインタを回復<br>
オブジェクトIDを読み出し、それに対応するインスタンスの先頭アドレスをオブジェクトIDテーブルで求め、インスタンスの先頭アドレスをポインタに設定します。

インスタンスは任意の「型」のインスタンスですが、これが構造体の場合、構造体全体の先頭アドレスと構造体の先頭メンバの先頭アドレスは一致します。（クラスの場合も同様です。）これらを纏めて１つのオブジェクトIDとすると適切に回復できないため、インスタンスの先頭アドレスだけでなく「型」も一緒に記録し、アドレスと型に対してオブジェクトIDを割り当てています。

<br>
@subsection HowToObjectTracking32 3-2.オブジェクト追跡の課題
ポインタを保存する前に必ずそのポインタが指すインスタンスを保存できれば特に問題はないのですが、そうとは限りません。<br>

ポインタの指すインスタンスが例えばローカル変数で、それがポインタより後でシリアライズ指示される場合もあります。<br>
そのようなデータ構造を回復するためには、そのローカル変数の保存は最初にポインタをシリアライズ指示された時ではなく、ローカル変数のシリアライズ指示まで遅らせるべきです。そうしないと順序よく回復できないため回復手順が複雑で負荷が高いものになります。<br>

しかし、ポインタが指している領域の所有権を当該ポインタが持っている場合は、最初にポインタをシリアライズ指示された時にインスタンスを保存すればOKです。最初にポインタを回復する際にインスタンス領域を確保して回復すれば適切にポイント構造を回復できますから。(*1)<br>

従って、ポインタがポイント先の所有権を持っているかどうかを判定できれば適切に保存／回復できるのですが、残念ながらシリアライズしようとしているポインタが所有権のある領域を指しているのか、そうでない領域を指しているのか判定する仕組みはC++言語仕様にはありません。

そこで、下記２つの選択肢を検討しました。

1. boost::serializationのようにこのようなケースをエラーとする(*2)<br>
この場合ローカル変数のようなインスタンスはそれをポイントするポインタより先に保存する必要が有ります。できれば避けたい制約と思います。

2. ポインタの属性として所有権の有無をプログラマが指定する
保存順序の制約はなく、処理負荷も特に高くはなりません。その代わりプログラマに少し負担がかかります。<br>

さて、インスタンスの所有権を持つか持たないかをポインタの属性とすることはリソース・リークし難いプログラムになりますのでたいへん好ましいと考えます。実際、C++11のスマート・ポインタは正にそのような概念に基づいています。スマート・ポインタが管理する領域はスマート・ポインタが所有権を持ち、その他のポインタは所有権を持ちません。所有権が明確ですのでメモリ・リークのリスクを大きく低減できます。<br>

そこで、インスタンスの所有権の有無をポインタの属性とする仕組みを実装することで、この問題を回避することにしました。<br>

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
(*1)ポインタが指すオブジェクトをオブジェクト追跡単位の最後で保存する案も検討しました<br>
途中でボイントされているローカル変数がシリアライズされたら、その時点で保存すれば良いです。制約も少なくプログラマへの負荷も小さいように見えます。<br>
しかし、最後までポイント先のインスタンスがシリアライズされなかった時、ユーザ・プログラムにおけるシリアライズ漏れ不具合なのか、所有権のある領域なのかの判断がつきません。前者の不具合は発見が遅れるとかなり痛いですので、この案は棄却しました。
</div>

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
(*2)boostの場合<br>
boost::serializationはこのような使い方を許可していません。上記ケースではローカル変数を保存する時にpointer_conflict例外が投げられます。<br>
["Pointer Conflict" Errors in the Boost Serialization Library](http://www.bnikolic.co.uk/blog/cpp-boost-ser-conflict.html)<br>
[Boost serialization with pointers](http://stackoverflow.com/questions/37747596/boost-serialization-with-pointers)
</div>

<br>
@subsection HowToObjectTracking33 3-3.オブジェクト追跡で用いる用語
このために、少し用語を定義しました。

1. 静的定義領域<br>
ポインタが所有権を持つことができない領域です。下記があります。
  - グローバル変数
  - ローカル変数
  - 他のクラスのメンバ変数(非ボインタ)
    これは、例えばstruct Foo { int mInt; };のmIntです。ポインタ側からmIntを獲得／解放することができません。

2. 動的生成領域(*3)<br>
newやnew[]で獲得するインスタンスです。

3. ポインタ型<br>
通常のポインタです。これはポイント先のインスタンスの所有権を持ちません。<br>
静的定義領域、動的生成領域の両方をボイントすることができませ。<br>

4. オーナー・ポインタ型<br>
ポイント先のインスタンスの所有権を持っているポインタです。<br>
当然ですが動的生成領域のみポイント可能です。<br>

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
(*3)「静的定義領域」と言う名称<br>
以前、[teratailで良い名前がないか質問](https://teratail.com/questions/20206)してcatsforepawさんの回答を元に決定しました。<br>
遠いところからですが、catsforepawさん、その節はありがとうございました。
</div>

<br>
@subsection HowToObjectTracking34 3-4.オブジェクト追跡対象について
静的定義領域は、全てのシリアライズ対象の変数がオブジェクト追跡候補になります。しかし、実際にポインタでポイントされる変数はその内の一部だけですので、全てをオブジェクト追跡するのは無駄が多いです。<br>
そこで、「@ref Basic21 」で示した方法でオブジェクト追跡対象を絞り込んでいます。

<br>
@subsection HowToObjectTracking35 3-5.オブジェクト追跡単位について
「@ref Basic22 」に示したオブジェクト追跡単位について少し詳しく説明します。

これはオブジェクトIDテーブルの有効期間です。clearTracking()することで

1. オブジェクトIDテーブルにシリアライズされていないインスタンスが登録されていないか確認します
2. オブジェクトIDテーブルをクリアします

前者により、ユーザ・プログラムのバグ検出をサポートします。<br>
後者により、不要なオブジェクト追跡を解除できるのでバフォーマンスを改善できます。<br>

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
<b>注意事項：</b><br>
解放したインスタンスを追跡したままにしていると未定義メモリ・アクセスが発生する場合があります。<br>
ですので、clearTracking()するまで、もしくは、シリアライザ自身を破棄するまで下記インスタンスを破棄しないで下さい。<br>
・シリアライズしたインスタンス<br>
・シリアライズしたポインタが指すインスタンス<br>
なお、clearTracking()することで「シリアライズした」と言う記録が全て破棄されますので、必要であれば、cleartTracking()後、最初のシリアライズまでの間ならばインスタンスを安全に破棄できます。
</div>

<br>
//############################################################################
@section TestObjectTracking 4.オブジェクト追跡の網羅的な使用例（自動テスト）の説明
//############################################################################


<br>
//############################################################################
@section TestPolymorphism 5.ポリモーフィズムの網羅的な使用例（自動テスト）の説明
//############################################################################

*/
