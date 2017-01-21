//############################################################################
/*!
    @brief      ドキュメント・ファイル－使用方法（個別）
    @ingroup    Documents
    @file       7.usage_individual.h
    @author     Yoshinori Tahara
    @date       2016/11/12 Created
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

@page UsageIndividual 使用方法（個別）

ここでは、Theolizerの以下の各機能について説明します。

@subpage BasicProcess \n
@subpage ClassVariation \n
@subpage EnumVariation \n
@subpage ObjectTracking \n
@subpage Destinations \n
@subpage UsageSTL \n
@subpage ChangingEnum \n
@subpage ChangingClass \n
@subpage ErrorReport \n

*/

/*!
    @page BasicProcess THEOLIZER_PROCESS()の使用例

ここでは、THEOLIZER_PROCESS()による保存／回復の網羅的な使用例（自動テスト）について説明します。<br>
クラスは単純なものを使います。クラスの詳細な使い方は「@ref ClassVariation 」で説明します。<br>
ここではポインタ型とオーナー・ポインタ型の説明と自動テストは行いません。これらについては「@ref ObjectTracking 」で行います。<br>

<br>
//############################################################################
@section DefinitionEnumClass 1.クラスとenum型の定義
//############################################################################

<b>source/reference_and_test/version/ver1/test_basic_process.h</b> で、次のようなクラスとenum型を定義しています。

enum型は従来のenum型と、C++11で追加されたscoped enum型の２つをテストします。

@dontinclude test_basic_process.h
@skip NormalEnum
@until };
@until };

クラス型はテストを容易にするため、非侵入型完全自動を用いています。<br>
同様にメンバ変数をpublicにしたいのでstructを用いています。<br>
（幾つか制約事項があります。「@ref ClassVariation 」にて説明します。）<br>
また、自動テストの記述を簡単化するため、下記を定義しています。
  - 初期化用のコンストラクタ
  - 比較演算子(operator==)
  - std::ostreamへのフレンド出力演算子(operator<<)

@skip struct ClassBasicTest
@until };

<br>
//############################################################################
@section TestBasicProcess 2.網羅的な使用例（自動テスト）の説明
//############################################################################
以下のアイテムを保存し、回復して保存した値と同じ値が回復できたことを、全てのシリアライザの全ての書式指定オプションに対して確認しています。（@ref TestProgram 参照）

  - C++言語がサポートする組み込み型全て<br>
    char, signed char, unsigned char, wchar_t, u16char, u32char,<br>
    short, int, long, long long,<br>
    unsigned short, unsigned int, unsigned long, unsigned long long,<br>
    float, double, long double<br>
<br>

  - C++11規格にて定義されている文字列型全て<br>
    std::string, std::wstring, std::u16string, std::u32string<br>
    C言語文字列（例："foo"）はchar型変数の配列として保存されます。<br>

    <b>１バイト文字列リテラルは要注意事項があります。</b><br>
    Visual Studioの１バイト文字列リテラルはShift-JIS等のNarrow文字列です。UTF-8ではありません。<br>
    Theolizerは１バイト文字列はUTF-8として取り扱いますので、u8"foo"のように必ずu8プリフィクスを付けて下さい。<br>

  - enum型<br>
    従来のenum型、scoped enum型<br>
<br>

  - クラス型<br>
    ここでは使い方を示すための1種類のみ。細かいテストは「@ref ClassVariation 」にて実施します。<br>
<br>

  - 右辺値（リテラルや式）が保存でき、同じ型の変数へ回復できること<br>
    int, long, long long,<br>
    unsigned int, unsigned long, unsigned long long,<br>
    float, double, long double,<br>
    std::string, std::wstring, std::u16string, std::u32string<br>
<br>

  - 配列
    上記全ての型に対する1次元、2次元、3次元配列についてテストします。
    saveTestArray(), loadTestArray()と言う関数テンプレトを使っています。
    型と最下位次元の要素数を指定すると、その型の1次元、2次元、3次元配列を生成して保存し、回復後の値をチェックします。
    設定する値とチェックする値は、それぞれの関数呼び出し時にラムダ式で与えています。

<b>保存処理（source/reference_and_test/version/ver1/test_basic_process.cpp）</b><br>
@snippet  test_basic_process.cpp saveBasicProcess

<b>多次元配列の保存処理（source/reference_and_test/version/ver1/common.h）</b><br>
@snippet  common.h saveTestArray

<b>回復処理（source/reference_and_test/version/ver1/test_basic_process.cpp）</b><br>
@snippet  test_basic_process.cpp loadBasicProcess

<b>多次元配列の回復処理（source/reference_and_test/version/ver1/common.h）</b><br>
@snippet  common.h loadTestArray

*/

/*!
    @page ChangingEnum enum型の変更方法、および、バージョン・アップ方法
# T.B.D.
*/

/*!
    @page UsageSTL 標準コンテナ（STL）の使い方
ここでは、標準コンテナ（STL）の使い方を説明します。
# T.B.D.
*/

/*!
    @page ChangingClass クラス定義の変更方法、および、バージョン・アップ方法
# T.B.D.
*/

/*!
    @page ErrorReport エラー処理

    エラー状態の解除は、下記のように、resetError()で行って下さい。<br>

@code
{
    std::ofstream   aStream("example.json");
    theolizer::JsonOSerializer<> js(aStream, theolizer::CheckMode::NoTypeCheck, false, true);

    THEOLIZER_PROCESS(js, ...);

    // エラー・チェック(例外を禁止している場合に必要)
    theolizer::ErrorInfo aErrorInfo=js.getError();
    if (aErrorInfo)
    {
        std::cout << aErrorInfo.getMessage() << std::endl;
        js.resetError();    // エラー状態を解除する
    }
}
@endcode

# T.B.D.
*/
