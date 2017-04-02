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

1. メンバ変数の場合は、通常通り保存指定<br>
2. THEOLIZER_PROCESS()マクロ<br>

<b>・オーナー・ポインタ型の指定（オーナー指定）</b>

1. メンバ変数の場合は、THEOLIZER_ANNOTATE(FS:...<...>Owner)<br>
このアノテーションはポインタに対してのみ指定できます。<br>
2. THEOLIZER_PROCESS_OWNER()マクロ<br>
このマクロはボインタのみ指定できます。<br>

また、回復時は初期値が必要です。nullptr、もしくは、適正なインスタンスへのポインタを設定して下さい。ポイント先インスタンスのクラスとシリアライズ・データ内のクラスが同じならば設定されていたインスタンスを維持したまま回復します。(これは保存先指定機能により複数のファイルに別々に保存されたメンバを統合するための機能です。) もし、クラスが異なる場合は解放し、シリアライズ・データと同じクラスをnewして回復します。nullptrの時も同様です。

<b>・オブジェクト追跡指定</b>

1. メンバ変数の場合は、THEOLIZER_ANNOTATE(FS:...<...>Pointee)
2. THEOLIZER_PROCESS_POINTEE()マクロ

@subsection HowToObjectTracking12 1-2.サンプル・ソース

<b>サンプル用のクラス定義：(source/reference_and_test/basic/test_object_tracking.h）</b><br>
（静的定義領域、動的生成領域については「@ref HowToObjectTracking33 」を参照下さい。）

@dontinclude basic/test_object_tracking.h
@skip ObjectTrackingClass
@until };

ObjectTrackingClass全体はオブジェクト追跡するクラスのサンプルです。また、メンバ変数はTHEOLIZER_ANNOTATE()マクロによるオブジェクト追跡関連指定のサンプルです。<br>
- mIntメンバ変数はオブジェクト追跡するよう指定されたメンバ変数です。
- mShortメンバ変数はオーナー指定されたメンバ変数です。

<b>保存処理：(source/reference_and_test/basic/test_object_tracking.cpp）</b>

@dontinclude basic/test_object_tracking.cpp
@skip "tutoriseObjectTracking() start"
@skip {
@until aSerializer.clearTracking();
@until }

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
<b>makeAutoRelease()</b><br>
これはnewで獲得した領域を自動的にdeleteするためのヘルパー関数です。戻り値のインスタンスが解放されるタイミングでdeleteします。source/reference_and_test/basic/common.hで定義しています。たいへん小さいですので、興味のある方は覗いてみて下さい。
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

<b>回復処理：(source/reference_and_test/basic/test_object_tracking.cpp）</b><br>
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
これがないと、C++の仕様上、基底クラスへのポインタが指す派生クラスのインスタンスの型を動的に判定できないためです。<br>
<br>

2. 派生クラスはTHEOLIZER_REGISTER_CLASS()マクロで派生クラスであることを指定して下さい。<br>
基底クラスへのポインタ経由でシリアライズしたい派生クラスは必ず指定して下さい。<br>
指定漏れすると、シリアライズ処理する時に"Can not find the derived class for <基底クラス>."エラーになります。<br>

また、「@ref Basic164 」も参照下さい。

<b>サンプル用のクラス定義：(source/reference_and_test/basic/test_polymorphism.h）</b>

@dontinclude basic/test_polymorphism.h
@skip //-
@until THEOLIZER_REGISTER_CLASS((PolyDerived1));

<b>THEOLIZER_REGISTER_CLASS()</b>による派生クラスの指定を忘れないようにお願いします。<br>
また、クラス名を()で囲って指定する必要が有りますのでご注意下さい。

<b>保存処理：(source/reference_and_test/basic/test_polymorphism.cpp）</b>

@dontinclude basic/test_polymorphism.cpp
@skip "tutorisePolymorphism() start"
@skip {
@until aSerializer.clearTracking();
@until }

<b>回復処理：(source/reference_and_test/basic/test_polymorphism.cpp）</b><br>
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

メンバ変数名は"メンバ変数名"として記録されます。基底クラスにはメンバ変数名がないため、"{基底クラス名}"として記録しています。これにより、基底クラスの定義順序変更にも対応できます。

@subsection Polymorphism32 3-2.参照を経由する場合

ポリモーフィックな基底クラスへの参照をシリアライズすることで、ポリモーフィックに回復されます。つまり、派生クラスのインスタンスを基底クラスへのポインタでポイントして保存して回復した場合、派生クラスのインスタンスとして回復されます。

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
ポインタが所有権（獲得／解放する権限）を持つことができない領域です。下記があります。
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
以前、teratailで「[インスタンス生成方法の呼び方について](https://teratail.com/questions/20206)」質問し、catsforepawさんの回答を元に決定しました。<br>
遠いところからですが、catsforepawさん、その節はありがとうございました。
</div>

<br>
@subsection HowToObjectTracking34 3-4.オブジェクト追跡対象について
静的定義領域は、全てのシリアライズ対象の変数がオブジェクト追跡候補になります。しかし、実際にポインタでポイントされる変数はその内の一部だけですので、全てをオブジェクト追跡するのは無駄が多いです。そこで、「@ref Basic21 」で示した方法でオブジェクト追跡対象を絞り込んでいます。

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
解放したインスタンスを追跡したままにしていると未定義メモリ・アクセスが発生する場合があります。ですので、clearTracking()するまで、もしくは、シリアライザ自身を破棄するまで下記インスタンスを破棄しないで下さい。<br>
・シリアライズしたインスタンス<br>
・シリアライズしたポインタが指すインスタンス<br>
なお、clearTracking()することで「シリアライズした」と言う記録が全て破棄されますので、必要であれば、cleartTracking()後、最初のシリアライズまでの間ならばインスタンスを安全に破棄できます。
</div>

<br>
//############################################################################
@section TestObjectTracking 4.オブジェクト追跡の網羅的な使用例（自動テスト）の説明
//############################################################################

@subsection TestObjectTracking41 4-1.各種メモリへのポインタのテスト
@subsubsection TestObjectTracking411 4-1-1.概要

ここでは、C++で用いられる少し異なるメモリ上のインスタンスへのポインタが適切に回復されることをテストしています。具体的には下記の通りです。

1. グローバル変数<br>
ここには静的定義領域のみ存在できますので、ここへのポインタをオーナー指定できません。<br>

2. ローカル変数（スタック変数）<br>
ここも静的定義領域のみ存在できますので、ここへのポインタをオーナー指定できません。<br>

3. ヒープ変数<br>
ここには動的生成領域と静的定義領域の両方が存在できます。newで獲得したクラスのインスタンス全体は動的生成領域です。そのクラスの各メンバ変数は静的定義領域となります。基底クラスは少し複雑です。<br>

  - ポリモーフィックな（仮想関数がある）クラスの場合<br>
  基底クラスへのポインタ経由でdeleteできますので、基底クラスへのポインタをオーナー指定できます。<br>
<br>

  - 非ポリモーフィックな（仮想関数がない）クラスの場合<br>
  C++仕様上、基底クラスへのポインタ経由でdeleteすると派生クラスのデストラクタは呼ばれず基底クラスとしてデストラクトされます。従って、非ポリモーフィックなクラスの場合、その基底クラスへのポインタをオーナー指定してはいけません。<br>

下記組み合わせのインスタンスとポインタ（非オーナー）について保存／回復テストを行います。<br>

    静的定義領域(グローバル変数, ローカル変数, メンバ変数)　＋　動的生成領域(プリミティブ, クラス)
                                ×
    Pointee指定プリミティブ型(long)　＋　Pointee指定なしクラス型（ObjectTracking0）　＋　Pointee指定クラス型（ObjectTracking1）

また、2番目以降の基底クラス・ポインタは一般にインスタンス先頭ではないアドレスを指しますが、このケースにも対応できていることを確認します。
そのために、派生クラスのインスタンス、および、その２番目の基底クラスへのポインタをシリアライズし、ポインタが回復したインスンタスをポイントすることを確認しています。

更に、動的生成領域(クラス)へのポインタ(非オーナー)のテストを実施する際に、std::shared_ptr<>を非侵入型手動対応しました。その時、[std::shared_ptr<>対応は意外に難しい](https://github.com/yossi-tahara/Theolizer/issues/18)ことが分かり、いくつか複雑な対応を行いましたので、ここでその一部のテストも行っています。(後日、標準コンテナ対応する予定です。その時、網羅的な自動テストを実装します。)

@subsubsection TestObjectTracking412 4-1-2.ソース・コード

<b>source/reference_and_test/basic/test_object_tracking.h</b>でテスト用のクラスを定義してます。<br>

1. ２番目以降の基底クラスへのポインタの回復テスト用
  - ObjectTrackingBase0             １番目の基底クラス
  - ObjectTrackingBase1             ２番目の基底クラス
  - ObjectTrackingDerived           上記２つを継承したクラス<br>
<br>

2. 各種メモリへのポインタのテスト用
  - ObjectTracking0                 被ポイント指定(Pointee)なし
  - ObjectTracking1                 被ポイント指定(Pointee)あり
  - StaticDefinition                メンバ変数静的定義領域用
  - Pointers                        自動シリアライズするポインタの定義<br>
<br>

3. 同じインスタンスを複数回シリアライズした時のテスト用
上記で定義したObjectTrackingDerivedを用います。

<b>source/reference_and_test/basic/test_object_tracking2.cpp</b>でテスト関数を定義してます。<br>

1. 保存処理<br>
template<class tSerializer><br>
void saveObjectTracking(tSerializer& iSerializer)の前半<br>
<br>

2. 回復処理<br>
template<class tSerializer><br>
void loadObjectTracking(tSerializer& iSerializer)の前半<br>


@subsection TestObjectTracking42 4-2.ポインタ(非オーナー)のテスト
@subsubsection TestObjectTracking421 4-2-1.概要
ポインタのシリアライズ、および、ポイント先のインスタンスのシリアライズについて、指定方法が手動(トップ・レベル)、自動シリアライズ（自動型のメンバ変数）、手動(非トップ・レベル)の３種類あります。それぞれについて各種の型、および、ポインタ→インスタンスとインスタンス→ポインタの順序おのおのの組み合わせのテストを行います。

    ポインタ：
    　手動(トップ・レベル)　＋　自動シリアライズ（自動型のメンバ変数）　＋　手動(非トップ・レベル)
                ×
    インスタンス：
    　手動(トップ・レベル)　＋　自動シリアライズ（自動型のメンバ変数）　＋　手動(非トップ・レベル)
                ×
    全てのプリミティブ　＋　enum型　＋　scoped num型　＋　クラス　＋　各配列型
                ×
    ポインタを先に保存　＋　インスタンスを先に保存

@subsubsection TestObjectTracking422 4-2-2.ソース・コード

<b>source/reference_and_test/basic/test_object_tracking.h</b>でテスト用のクラスとマクロを定義してます。<br>

1. DEFINE_MEMBERS()マクロ<br>
各種の型に対応する、変数宣言や初期化を定義するためのマクロです。「@ref SingleTest 」と同じ名前ですが、少し異なるマクロです。定義している変数の型は同じですが、初期化値が異なります。<br>
<br>

2. PointeeListクラス<br>
静的定義領域の定義です。各型のインスタンスについて、自動シリアライズと手動(トップ・レベル)を担います。前者は非侵入型完全自動として処理し、後者はsavePointee()とloadPointee()関数で行います。<br>
<br>

3. PointeeListManualクラス<br>
静的定義領域の定義です。各型のインスタンスについて、手動(非トップ・レベル)を担います。<br>
<br>

4. PointerListクラス<br>
ポインタ(非オーナー)の定義です。各型のポインタについて、自動シリアライズと手動(トップ・レベル)を担います。前者は非侵入型完全自動として処理し、後者はsavePointer()とloadPointer()関数で行います。トップ・レベル、および、自動型については、例えばデバッグやログ用にconst領域へのポインタの保存を想定して、constポインタも定義しています。<br>
<br>

5. PointerListManual<br>
ポインタ(非オーナー)の定義です。各型のポインタについて、手動(非トップ・レベル)を担います。回復処理を行わない手動型の実装は想定不要と考えますので、constポインタを定義していません。<br>
<br>

<b>source/reference_and_test/basic/test_object_tracking2.cpp</b>でテスト関数を定義してます。<br>

1. 保存処理<br>
template<class tSerializer><br>
void saveObjectTracking(tSerializer& iSerializer)の後半<br>
<br>

2. 回復処理<br>
template<class tSerializer><br>
void loadObjectTracking(tSerializer& iSerializer)の後半<br>


@subsection TestObjectTracking43 4-3.オーナー・ポインタのテスト
@subsubsection TestObjectTracking431 4-3-1.概要
オーナー・ポインタのシリアライズについて、指定方法が手動(トップ・レベル)、自動シリアライズ（自動型のメンバ変数）、手動(非トップ・レベル)の３種類あります。それぞれについて各種の型の組み合わせのテストを行います。また、nullptrの回復もテストします。<br>
（ポインタ(非オーナー)と異なり、インスタンス側はポインタが管理するため組み合わせが発生しません。また、インスタンスの保存は先に出現した方が先ですので、保存順序の組み合わせも発生しません。）

    オーナー・ポインタ：
    　手動(トップ・レベル)　＋　自動シリアライズ（自動型のメンバ変数）　＋　手動(非トップ・レベル)
                ×
    全てのプリミティブ　＋　enum型　＋　scoped num型　＋　クラス　＋　各配列型

@subsubsection TestObjectTracking432 4-3-2.ソース・コード

<b>source/reference_and_test/basic/test_object_tracking.h</b>でテスト用のクラスとマクロを定義してます。<br>

1. DEFINE_MEMBERS()マクロ<br>
ポインタ(非オーナー)と共通です。<br>
<br>

2. NEW_ARRAY()マクロ<br>
C++は、配列型(Type[N])をnewした結果は残念なことに配列型へのポインタ(Type(*)[N])にならず要素へのポインタ(Type*)になります。そのため、配列型へのポインタ(Type(*)[N])変数へそのままでは代入できません。これをreinterpret_cast<Type(*)[N]>()して(*4)代入できるようにするマクロです。<br>
<br>

3. OwnerListクラス<br>
オーナー・ポインタの定義です。各型のポインタについて、自動シリアライズと手動(トップ・レベル)を担います。前者は非侵入型完全自動として処理し、後者はsavePointer()とloadPointer()関数で行います。インスタンスを回復しないオーナー指定ポインタは意味がないので、constポインタは定義していません。<br>
<br>

4. OwnerListManualクラス<br>
オーナー・ポインタの定義です。各型のポインタについて、手動(非トップ・レベル)を担います。OwnerListクラスと同様constポインタを定義していません。<br>
<br>

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
(*4)「配列型へのポインタ」のサポート<br>
以前、teratailで「[配列型(サイズ含む)へのポインタをnewで生成したい](https://teratail.com/questions/34266)」旨を質問してraccyさんの回答をヒントに対応できました。<br>
遠いところからですが、raccyさん、ありがとうございました。
</div>

<b>source/reference_and_test/basic/test_object_tracking3.cpp</b>でテスト関数を定義してます。<br>

1. 保存処理<br>
template<class tSerializer><br>
void saveObjectTracking3(tSerializer& iSerializer)の前半<br>
<br>

2. 回復処理<br>
template<class tSerializer><br>
void loadObjectTracking3(tSerializer& iSerializer)の前半<br>

@subsection TestObjectTracking44 4-4.同じインスタンスを複数回シリアライズするテスト

ObjectTrackingDerivedのインスタンスに対して、被ポインタ指定したものとしていないものについて保存／回復テストを行います。

<b>source/reference_and_test/basic/test_object_tracking3.cpp</b>でテスト関数を定義してます。<br>

1. 保存処理<br>
template<class tSerializer><br>
void saveObjectTracking3(tSerializer& iSerializer)の後半<br>
<br>

2. 回復処理<br>
template<class tSerializer><br>
void loadObjectTracking3(tSerializer& iSerializer)の後半<br>

また、同じインスタンスを複数回保存し、それを異なるインスタンスへ回復しようとした時、WrongUsing例外が発生することのテストを行います。

<b>source/reference_and_test/basic/test_object_tracking.cpp</b>でテスト関数を定義してます。<br>

tutoriseObjectTracking()関数の最後の方、「複数回シリアライズ・データの回復エラーテスト」で行っています。<br>

<br>
//############################################################################
@section TestPolymorphism 5.ポリモーフィズムの網羅的な使用例（自動テスト）の説明
//############################################################################

ここでは、基底クラスへのポインタでポイントされる異なる派生クラスを適切に回復できることを確認します。<br>
非侵入型完全自動、侵入型半自動、非侵入型手動の３種類の基底クラスと派生クラスを用意し、派生クラスは３種類の基底クラスを全て継承しました。

<b>source/reference_and_test/basic/test_polymorphism.h</b>でテスト用のクラスを定義してます。<br>

1. 基底クラス
  - PolyBaseFullAuto        非侵入型完全自動
  - PolyBaseHalfAuto        侵入型半自動
  - PolyBaseManual          非侵入型手動<br>
<br>

2. 派生クラス
  - PolyDerivedFullAuto     非侵入型完全自動
  - PolyDerivedHalfAuto     侵入型半自動
  - PolyDerivedManual       非侵入型手動

<b>source/reference_and_test/basic/test_polymorphism.cpp</b>でテスト関数を定義してます。<br>

1. 保存処理<br>
template<class tSerializer><br>
void savePolymorphism(tSerializer& iSerializer)<br>
<br>

2. 回復処理<br>
template<class tSerializer><br>
void loadPolymorphism(tSerializer& iSerializer)<br>

基底クラスのインスタンスをstd::unique_ptr<>で保持し、そのstd::unique_ptr<>のリストをstd::vector<>で保持しています。（std::vector<>とstd::unique_ptr<>を非侵入型手動クラスとして仮にシリアライズ対応しています。）<br>
３種類の基底クラス毎にstd::vector<>を用意しているので、std::vector<>も下記の３種類あります。

- 非侵入型完全自動
- 侵入型半自動
- 非侵入型手動

各std::vector<>には３種類の派生クラスのインスタンスを登録して、std::vector<>を保存します。そして、全てのポインタをnullptr設定して回復し、保存した時の派生クラスを回復できたことを確認しています。<br>

*/
