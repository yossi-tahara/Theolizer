//############################################################################
/*!
    @brief      ドキュメント・ファイル－使用方法（個別）
    @ingroup    Documents
    @file       7a.ClassVariation.h
    @author     Yoshinori Tahara
    @date       2016/12/02 Created
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
    @page ClassVariation クラスのバリエーション
ここでは、クラス（classとstruct）をシリアライズする時の様々なバリエーションについて説明します。

<br>
//############################################################################
@section FullAutoClass 1.非侵入型完全自動クラス
//############################################################################

これはシリアライズするためにクラス定義への前準備が不要な使い方です。<br>
使用している他のライブラリの「構造体」をシリアライズしたい時、および、プロトタイピングやデバッグ中に一時的にシリアライズしたい時に用います。<br>

使い方は簡単で、シリアライズしたい構造体のインスタンスをいきなりTHEOLIZER_PROCESS()マクロに与えるだけです。

<b>定義例</b> (source/reference_and_test/version/ver1/test_class_variation.h)<br>
非侵入型完全自動クラスの定義例です。

@dontinclude test_class_variation.h
@skip FullAutoTutorial
@until };

<b>保存処理例</b> (source/reference_and_test/version/ver1/test_class_variation.cpp の <b>tutoriseClassVariation()</b> 関数)<br>
上記のFullAutoTutorialを保存するコード例です。

@dontinclude test_class_variation.cpp
@skip void tutoriseClassVariation()
@skip "tutoriseClassVariation() start"
@skip {
@until THEOLIZER_PROCESS
@skip }
@until }

<b>保存結果</b> (`<ビルド・フォルダ>/Testing/tutorise_class_variation.json`)<br>
上記で保存されたシリアライズ・データの該当部分です。<br>
このファイルは、<b>zy2_long_test.sh</b>や<b>zz2_long_test.bat</b> にて自動生成されます。（「@ref HowToBuildLibrary122 」参照。）本節のビルド・フォルダ配下のファイルは全て同じです。<br>
privateメンバ以外のメンバが保存されています。
@code
{
    "mFullAutoProtected":456,
    "mFullAutoPublic":789
}
@endcode

<b>回復処理例</b> (source/reference_and_test/version/ver1/test_class_variation.cpp の <b>tutoriseClassVariation()</b> 関数)<br>
上記のFullAutoTutorialを回復して値をチェックするコード例です。

@skip {
@until THEOLIZER_EQUAL
@skip }
@until }

Theolizerドライバがシリアライズされていることを検出し、自動的に必要なメンバ・リストを生成して、シリアライズ可能な状態にします。<br>
privateメンバをシリアライズするためには、対象構造体の内部にシリアライズ関数を設ける必要が有ります。
それを自動的に行うと問題が多発してしまいます。（例えば「他のライブラリ」のソースを変更してしまう。）<br>
それは行うべきではありませんので完全自動クラスを「非侵入型」として実装しました。従って、privateメンバをシリアライズすることはできません。<br>
なお、対象クラスを派生して派生先クラスでシリアライズ関数を実装しているため、protectedメンバはシリアライズします。

各メンバに対して保存の有無、および、保存先の指定が可能です。「@ref Destinations 」を参照下さい。<br>
<br>
//############################################################################
@section HalfAutoClass 2.侵入型半自動クラス
//############################################################################

これはシリアライズするためにクラス定義への前準備が必要ですが、クラス定義のバージョン管理が可能になる使い方です。（バージョン管理については「@ref ChangingClass 」を参照下さい。）<br>

クラス定義のバージョン管理が不要な間は原則として使う必要はありません。<br>
例外は下記です。
  - privateメンバもシリアライズしたい時
  - メンバ変数を「順序対応」にしたい時

「@ref Basic121 」で説明したように、シリアライズ・データとプログラム間でメンバ変数を対応する方法として２種類サポートしています。

<b>名前対応の定義例</b> (source/reference_and_test/version/ver1/test_class_variation.h)<br>
侵入型半自動クラスを「名前対応」で定義する時はTHEOLIZER_INTRUSIVE()マクロを使います。

@dontinclude test_class_variation.h
@skip HalfAutoNameTutorial
@until };

<b>順序対応の定義例</b> (source/reference_and_test/version/ver1/test_class_variation.h)<br>
侵入型半自動クラスを「名前対応」で定義する時はTHEOLIZER_INTRUSIVE_ORDER()マクロを使います。

@skip HalfAutoOrderTutorial
@until };

THEOLIZER_INTRUSIVE()とTHEOLIZER_INTRUSIVE_ORDER()は、最後に<b>private:</b> で終わっています。ですので、クラス定義の最後に置くことをお勧めします。

これらのマクロは次のように使います。

  - <b>THEOLIZER_INTRUSIVE(dAnno, dClass, dLastVersionNo);</b>
  - <b>THEOLIZER_INTRUSIVE_ORDER(dAnno, dClass, dLastVersionNo);</b>

それぞれのパラメータは以下の通りです。

  - <b>dAnno</b><br>
    メンバ保存の指定方法のオプションです。詳しくは「@ref Destinations 」を参照下さい。<br>
    - CS : デフォルトで保存します。（通常はこちらを指定して下さい。）<br>
    - CN : デフォルトは保存しません。（保存するメンバを指定します）<br>
  - <b>dClass</b><br>
    自クラス名を指定します。C++はメンバ関数の外クラス定義内で自クラスにアクセスできないため、指定が必要なのです。クラス名の外側に必ず()を付けて下さい。<br>
  - <b>dLastVersionNo</b><br>
    最新版のバージョン番号は指定します。最初は1を指定して下さい。<br>
    「@ref ChangingClass 」で使い方を説明します。<br>
<br>

<b>保存処理／回復処理</b>は非侵入型完全自動と同じですので、省略します。

<b>保存結果</b> (`<ビルド・フォルダ>/Testing/tutorise_class_variation.json`)<br>
保存されたシリアライズ・データの該当部分です。<br>
privateメンバも含めて保存されています。
@code
{
    "mHalfAutoNamePrivate":-123,
    "mHalfAutoNameProtected":-456,
    "mHalfAutoNamePublic":-789
}
@endcode

<br>
//############################################################################
@section ManualClass 3.非侵入型手動クラス
//############################################################################

これは自動でシリアライズすることが困難なクラスに対応するために設けました。<br>
例えば、std::vector<>を自動的に適切なシリアライズをすることは現実的ではありません。そのような場合に保存／回復処理を手動で記述するための仕組みです。<br>
使い方が難しいため、どうしても必要な時だけお使い下さい。（クラス・テンプレートにも対応しているのですが、使い方の難易度が高いこと、および、十分なテストを行うには時間が掛かりそうですので、現時点ではクラス・テンプレート対応は非公開と致します。）

下記の手順で保存／回復処理を記述することを推奨します。<br>

1. @link DefineManualClass プロジェクトをビルド・エラーがでない状態にして下さい。@endlink<br>
    シリアライズ対象クラスの定義を行って下さい。そして、ビルド・エラーをなくすことをお勧めします。<br>
<br>

2. @link SpecifyManualClass 対象とするクラスをTHEOLIZER_NON_INTRUSIVE_ORDER()マクロで指定します。@endlink<br>
<br>

3. @link TemplateManualClass 一度ビルドします。この時、未定義エラーが出ます。@endlink<br>
    TheolizerNonIntrusive\<対象クラス名\>::%TheolizerUserDefine を含むエラーになります。<br>
    この部分はテンプレートを使っていることもあり、あまり適切なエラー・メッセージになりません。たいへん申し訳ないですが、注意深くご使用下さい。<br>
<br>

4. @link CopyManualClass 自動生成されたソース内にある未定義エラーのクラス定義雛形をコピーします。@endlink<br>
    これは保存／回復処理の記述先関数の枠組み定義です。<br>
    これをTHEOLIZER_NON_INTRUSIVE_ORDER()マクロの直後へコピーして下さい。<br>
<br>

5. @link WriteManualClass 上記のクラス内に保存／回復処理を記述して下さい。@endlink<br>
<br>

6. @link SaveLoadManualClass 後は通常通りTHEOLIZER_PROCESS()シリーズ・マクロでシリアライズ処理します。@endlink<br>

@subsection DefineManualClass 3-1.非侵入型手動クラスの定義例
サンプル・ソースは source/reference_and_test/version/ver1/test_class_variation.h です。<br>
非侵入型手動クラスの定義例です。

@skip ManualTutorial
@until };

@subsection SpecifyManualClass 3-2.非侵入型手動クラスのシリアライズ指定
サンプルですのでクラス定義に続けてシリアライズ指定していますが、実際にはシリアライズ指定はクラス定義より後であれば離れていてもよいです。

@skip ManualTutorial
@until THEOLIZER_NON_INTRUSIVE_ORDER

THEOLIZER_NON_INTRUSIVE_ORDER()マクロは次のように使います。

  - <b>THEOLIZER_NON_INTRUSIVE_ORDER(dClass, dLastVersionNo);</b><br>
    手動型は順序対応のみです。<br>

パラメータは以下の通りです。

  - <b>dClass</b><br>
    対象クラス名を指定します。クラス名の外側に必ず()を付けて下さい。<br>
  - <b>dLastVersionNo</b><br>
    最新版のバージョン番号は指定します。最初は1を指定して下さい。<br>
    「@ref ChangingClass 」で使い方を説明します。<br>
<br>

@subsection TemplateManualClass 3-3.自動生成される保存／回復処理用関数の雛形例 
サンプルは `<ビルド・フォルダ>/reference_and_test/version/ver1/test_class_variation.cpp.theolizer.hpp` に生成されます。<br>
ManualTutorialクラスに対する部分は下記です。<br>

@code
#ifdef  THEOLIZER_WRITE_CODE // ###### ManualTutorial ######

#if false // Sample of save/load function.
template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<ManualTutorial>::TheolizerUserDefine<tBaseSerializer, tTheolizerVersion, 1>
{
    // Save members.
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        
        
    }

    // Load members.
    static void loadClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        if (!oInstance) oInstance=new typename tTheolizerVersion::TheolizerTarget();
        
        
    }
};
#endif // Sample of save/load function.

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE ManualTutorial

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_CLASS_NAME((u8"ManualTutorial"))
#include <theolizer/internal/version_manual.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

#endif//THEOLIZER_WRITE_CODE // ###### ManualTutorial ######
@endcode

@subsection CopyManualClass 3-4.クラス定義雛形のコピー
上記雛形の <b>#%if false // Sample of save/load function.</b> の次の行から、 <b>#%endif // Sample of save/load function.</b> の前の行までを、THEOLIZER_NON_INTRUSIVE_ORDER((ManualTutorial), 1);の直後へコピーします。

@subsection WriteManualClass 3-5.保存／回復処理関数の記述例 
下記はTHEOLIZER_NON_INTRUSIVE_ORDER()マクロによるシリアライズ指定から、コピーした雛形に保存／回復処理を記述したサンプルです。

@dontinclude test_class_variation.h
@skip ManualTutorial
@skip <<<
@until };

<b>tTheolizerVersion::TheolizerTarget</b> は、指定した対象クラス型へ展開されます。<br>
この例ではManualTutorialです。constや参照がついていて分かりにくいですが、意味的には下記となります。

  - saveClassManual() 関数の<b>iInstance</b> : ManualTutorialへのポインタ
  - loadClassManual() 関数の<b>oInstance</b> : ManualTutorialへのポインタへの参照

oInstanceにnullptrが設定されていた場合は、領域を獲得して下さい。<br>
その際、一部の回復処理を先に行い、その値を使ってコンストラクタしてポインタを返却するため、ポインタへの参照にしています。

@subsection SaveLoadManualClass 3-6.保存／回復処理例 

<b>保存処理／回復処理</b>は非侵入型完全自動と同じですので、省略します。

<b>保存結果</b> (`<ビルド・フォルダ>/Testing/tutorise_class_variation.json`)<br>
保存されたシリアライズ・データの該当部分です。<br>
順序対応なのでメンバ変数名は出力されません。mManualPublicに100を設定して保存していますので、100のみが出力されます。
@code
[
    100
]
@endcode

<br>
//############################################################################
@section DerivedIncluded 4.それらを派生したり包含したりしたクラス
//############################################################################
各種クラスの派生、および、包含（メンバ変数として他のクラス型を用いる)は、通常通りです。

<b>各種クラスを派生／包含した例</b> (source/reference_and_test/version/ver1/test_class_variation.h)<br>
FullAutoTutorialをpublic継承、HalfAutoNameTutorialをprivate継承、HalfAutoOrderTutorialをprivateメンバ、ManualTutorialをpublicメンバとした非侵入型完全自動クラスです。<br>
protectedを定義していませんが、protected継承、protectedメンバの両方とも使えます。

@skip DerivedClass
@until };

<b>保存処理例</b> (source/reference_and_test/version/ver1/test_class_variation.cpp の <b>tutoriseClassVariation()</b> 関数)<br>
上記のDerivedClassを保存するコード例です。

@dontinclude test_class_variation.cpp
@skip void tutoriseClassVariation()
@skip "tutoriseClassVariation() start"
@skip {
@until JsonOSerializer
@skip 派生／包含クラス
@until }

<b>保存結果</b> (`<ビルド・フォルダ>/Testing/tutorise_class_variation.json`)<br>
上記で保存されたシリアライズ・データの該当部分です。<br>
名前対応ですので、基底クラスの定義順序変更に備え基底クラスは()付きでクラス名で保存しています。<br>
非侵入型ですのでprivate継承したクラス、およひ、privateメンバは保存されません。

@code
{
    "(FullAutoTutorial)":{
        "mFullAutoProtected":201,
        "mFullAutoPublic":202
    },
    "mManualTutorial":[
        207
    ]
}
@endcode

<b>回復処理例</b> (source/reference_and_test/version/ver1/test_class_variation.cpp の <b>tutoriseClassVariation()</b> 関数)<br>
上記のFullAutoTutorialを回復して値をチェックするコード例です。

@skip {
@until JsonISerializer
@skip 派生／包含クラス
@until }

private継承したクラス、およひ、privateメンバは保存されないため、デフォルト・コンストラクタで生成された値(0)のまま変化しませんので、0と一致することを確認しています。

<br>
//############################################################################
@section TestClassVariation 5.網羅的な使用例（自動テスト）の説明
//############################################################################

網羅的な自動テスト用のクラスは<b>source/reference_and_test/version/ver1/test_class_variation.h</b> ファイルの「単独テスト」以降で定義しています。<br>
また、テスト処理は<b>source/reference_and_test/version/ver1/test_class_variation.cpp</b> ファイルで定義しており、saveClassVariation()関数で保存処理、loadClassVariation()関数で回復処理と値の検証を行っています。

@subsection SingleTest 5-1.単独テスト
単独テストの先頭で<b>DEFINE_MEMBERS()マクロ</b>を定義しています。<br>

これは、全てのプリミティブ型、幾つかの基本的なenum型とクラスについて、単独、および、配列を定義するためのマクロです。このマクロを展開する直前でDEFINE()とARRAY()マクロを定義することで各々について、メンバ変数の定義、初期化、保存、回復、値チェックするコードを生成しています。

@dontinclude test_class_variation.h
@skip DEFINE_MEMBERS
@until ARRAY(ClassBasicTest
@until ClassBasicTest()

単独テストの内容は、以下の通りです。

1. 「@ref BasicProcess 」の網羅テストと同じプリミティブとenum型、クラス、配列のセットの定義<br>
    多数用いるのでDEFINE_MEMBERS()マクロで定義しています。<br>
<br>

2. 非侵入型完全自動クラス（FullAuto）<br>
    DEFINE_MEMBERS()マクロのメンバについて、private, protected, publicのそれぞれに定義しています。<br>
    ArrayOnly0（非侵入型完全自動クラス）クラスの配列メンバをpublicとして定義しています。<br>
    privateメンバ以外は保存／回復でき、privateメンバは初期値が維持されることを検証しています。<br>
<br>

3. 侵入型半自動－名前対応クラス（HalfAutoName）<br>
    DEFINE_MEMBERS()マクロのメンバについて、private, protected, publicのそれぞれに定義しています。<br>
    ArrayOnly1（非侵入型完全自動クラス）クラスの配列メンバをpublicとして定義しています。<br>
    全てのメンバが保存／回復できることを検証しています。<br>
<br>

4. 侵入型半自動－順序対応クラス（HalfAutoorder）<br>
    DEFINE_MEMBERS()マクロのメンバについて、private, protected, publicのそれぞれに定義しています。<br>
    ArrayOnly2（非侵入型完全自動クラス）クラスの配列メンバをpublicとして定義しています。<br>
    全てのメンバが保存／回復できることを検証しています。<br>
<br>

5. 非侵入型手動クラス（Manual）<br>
    DEFINE_MEMBERS()マクロのメンバについて、public定義しています。<br>
    ArrayOnly3（非侵入型完全自動クラス）クラスの配列メンバをpublicとして定義しています。<br>
    全てのメンバの保存／回復処理を実装しています。<br>
    全てのメンバが保存／回復できることを検証しています。<br>
    （非侵入型手動クラスで、private/protectedメンバをシリアライズするコードを記述するとコンパイルできないため、これらのメンバの実装とシリアライズ・テストは行なっていません。）

<br>

<b>ArrayOnly</b>について<br>
他のクラスメンバに配列としてのみ使用される非侵入型完全自動クラスのソース自動生成に不具合が発生したため、その再発防止用として実装しています。<br>

@subsection DoubleTest 5-2.二重組み合わせテスト

@subsubsection DoubleTestHelper 5-2-1.二重組み合わせテスト用のヘルパー・クラス
非侵入型完全自動(BaseFullAuto)、侵入型半自動(BaseHalfAuto)、非侵入型手動(BaseManual)のクラス・テンプレートを定義しています。<br>
これらは全てテンプレート・パラメータとしてint型の非型パラメータを1つ持ち、メンバ変数の初期値を決めてます。<br>

これらのBaseXxxクラス群を派生したクラスは更に別のクラスへ派生されます。その際、同じ基底クラスを複数の派生クラスを経由して１つの派生クラスに継承されることは許されないため、この非型パラメータを用いて別クラスにしています。<br>
例えば、下記のように定義すると、BaseFullAutoがDrivedFullAutoとDrivedHalfAutoを経由してTripledFullAutoへ継承されるためエラーになります。それを防ぐためにBaseFullAutoをクラス・テンプレートとし、DrivedFullAutoとDrivedHalfAutoへの継承は別実体としています。
@code
BaseFullAuto { };
DrivedFullAuto : public BaseFullAuto { };
DrivedHalfAuto : public BaseFullAuto { };
TripledFullAuto : public DrivedFullAuto, public DrivedHalfAuto { };
@endcode

BaseFullAutoは侵入型半自動クラスPrivateInheritanceをprivate継承しています。<br>
BaseManualは侵入型半自動クラスProtectedInheritanceをprotected継承しています。<br>
侵入型を非侵入型へprivate/protected継承するとMinGWでビルド・エラーになる不具合が発生したため、その再発防止用です。<br>

侵入型内部で定義しているstatic変数（kIsTheolizer）の有無で侵入型であることを判定していたのですが、private継承しているため非侵入型クラスにkIsTheolizerがprivateで存在していることになります。<br>
kIsTheolizerはアクセスできないのでSFINAEにより合致しないことを期待したのですが、「privateなのでアクセスできないエラー」となってしまいました。他の手段で判定し回避しました。

@subsubsection DoubleTestImpl 5-2-2.二重組み合わせテスト本体
内容は、以下の通りです。

1. 非侵入型完全自動クラス（DerivedFullAuto）<br>
    BaseFullAuto, BaseHalfAuto, BaseManualのそれぞれを、private, protected, public継承しています。合計9個のクラスを継承しています。<br>
    BaseFullAuto, BaseHalfAuto, BaseManualのそれぞれを、private, protected, publicメンバ変数として定義しています。<br>
    private継承したクラス、および、DerivedFullAutoのprivateメンバ変数は全て初期値が維持されること、それら以外のクラスについてはBaseFullAutoのprivateメンバ以外は全て保存／回復されること、BaseFullAutoのprivateメンバは初期値が維持されることを検証しています。<br>
<br>

2. 侵入型半自動クラス（DerivedHalfAuto）<br>
    BaseFullAuto, BaseHalfAuto, BaseManualのそれぞれを、private, protected, public継承しています。合計9個のクラスを継承しています。<br>
    BaseFullAuto, BaseHalfAuto, BaseManualのそれぞれを、private, protected, publicメンバ変数として定義しています。<br>
    全ての継承したクラス、および、DerivedHalfAutoのメンバ変数において、BaseFullAuto型のprivateメンバは全て初期値が維持されること、それら以外については全て保存／回復されることを検証しています。<br>
<br>

3. 非侵入型手動クラス（DerivedManual）<br>
    BaseFullAuto, BaseHalfAuto, BaseManualのそれぞれを、public継承しています。合計3個のクラスを継承しています。<br>
    BaseFullAuto, BaseHalfAuto, BaseManualのそれぞれを、publicメンバ変数として定義しています。<br>
    全ての基底クラスと全てのメンバ変数について保存／回復処理関数を実装しています。
    BaseFullAutoのprivateメンバは全て初期値が維持されること、それら以外については全て保存／回復されることを検証しています。<br>
<br>

@subsection TripledTest 5-3.三重組み合わせテスト
DerivedFullAuto, DerivedHalfAuto, DerivedManualを更に組み合わせて以下の自動テストを行います。


1. 非侵入型完全自動クラス（TripledFullAuto）<br>
    DrivedFullAuto, DrivedHalfAuto, DrivedManualのそれぞれを、public継承しています。合計3個のクラスを継承しています。<br>
    DrivedFullAuto, DrivedHalfAuto, DrivedManualのそれぞれを、private, protected, publicメンバ変数として定義しています。<br>
    TripledFullAutoクラスのprivateメンバ変数は全て初期値が維持されること、それら以外について、非侵入型完全自動クラスのprivateメンバは初期値が維持されること、それら以外については全て保存／回復されることを検証しています。<br>
<br>

2. 侵入型半自動クラス（TripledHalfAuto）<br>
    DrivedFullAuto, DrivedHalfAuto, DrivedManualのそれぞれを、public継承しています。合計3個のクラスを継承しています。<br>
    DrivedFullAuto, DrivedHalfAuto, DrivedManualのそれぞれを、private, protected, publicメンバ変数として定義しています。<br>
    非侵入型完全自動クラスのprivateメンバは全て初期値が維持されること、それら以外については全て保存／回復されることを検証しています。<br>
<br>

3. 非侵入型手動クラス（TripledManual）<br>
    DrivedFullAuto, DrivedHalfAuto, DrivedManualのそれぞれを、public継承しています。合計3個のクラスを継承しています。<br>
    DrivedFullAuto, DrivedHalfAuto, DrivedManualのそれぞれを、publicメンバ変数として定義しています。<br>
    全ての基底クラスと全てのメンバ変数について保存／回復処理関数を実装しています。<br>
    非侵入型完全自動クラスのprivateメンバは全て初期値が維持されること、それら以外については全て保存／回復されることを検証しています。<br>
<br>

*/
