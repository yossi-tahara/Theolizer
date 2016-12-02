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

これは、シリアライズするためにクラス定義に対してシリアライズするための前準備が不要な使い方です。<br>
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

各メンバに対して保存の有無、および、保存先の指定が可能です。
通常は侵入型で指定しますので積極的な意味はないですが、便利のためにできるようにしています。

侵入型半自動<br>
クラス定義の内側にTHEOLIZER_INTRUSIVE()シリーズ・マクロを記述する。
このマクロの最後にprivate:をおいているので、クラス定義の最後に置くことを推奨する。

非侵入型手動<br>
THEOLIZER_NON_INTRUSIVE_ORDER()シリーズ・マクロでシリアライズ指定する。
その状態でビルドすると、未定義エラーとなる。*.hppにその関数の雛形があるので、コピーしてを必要な保存／回復処理を記述する。

*/
