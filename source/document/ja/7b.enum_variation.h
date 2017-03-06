//############################################################################
/*!
    @brief      ドキュメント・ファイル－使用方法（個別）
    @ingroup    Documents
    @file       7b.enum_variation.h
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
    @page EnumVariation enum型のバリエーション
Theolizerは下記のenum型をシリアライズするための機能を持っています。

<b>対応するenum型</b><br>
  - 従来のenum型
  - C++11で追加されたscoped enum型

<b>Theolizerのenum型サポート</b><br>
  - 非侵入型完全自動
  - 非侵入型半自動<br>
    非侵入型半自動は、下記オプションを指定します。
    - シンボル名保存（デフォルト）
    - シンボル値保存

ここでは、これらの使い方と自動テストについて説明します。<br>

<br>
//############################################################################
@section FullAutoEnum 1.非侵入型完全自動enum型
//############################################################################

完全自動方式の使い方については@ref BasicUsage で説明していますのでそちらを御覧ください。

<br>
//############################################################################
@section HalfAutoEnumSymName 2.非侵入型半自動enum型
//############################################################################

enum型をシリアライズ対象であることを明示的に指定する際に、下記マクロを用いることができます。

  - <b>THEOLIZER_ENUM(dEnum, dLastVersionNo);</b><br>
    シンボル名でシリアライズするよう指定します。
  - <b>THEOLIZER_ENUM_VALUE(dEnum, dLastVersionNo);</b>
    シンボル値でシリアライズするよう指定します。

それぞれのパラメータは以下の通りです。

  - <b>dEnum</b><br>
    シリアライズ対象のenum型名を指定します。<br>
  - <b>dLastVersionNo</b><br>
    そのバージョン番号を指定します。詳しくは@ref ChangingEnum を参照下さい。<br>
<br>

@dontinclude basic/test_enum_variation.h
@skip EnumSymName
@until };

例えば、上記ように定義されている時、次の構文でEnumSymNameをシリアライズ対象として指定します。

@skip THEOLIZER_ENUM
@until );

保存／回復処理は従来通りです。

@dontinclude basic/test_enum_variation.cpp
@skip tutoriseEnumVariation()
@skip //
@until THEOLIZER_EQUAL(aEnumSymVal, eesvVal);
@skip }
@until }

シリライズされた<b>tutorise_enum_variation.json</b> は以下の通りです。<br>

@code json
{
    "SerialzierName":"JsonTheolizer",
    "GlobalVersionNo":1,
    "TypeInfoList":[1]
}
"eesnVal"
3001
@endcode

<br>
//############################################################################
@section TestEnumVariation 3.網羅的な使用例（自動テスト）の説明
//############################################################################

網羅的な自動テスト用のクラスは<b>source/reference_and_test/basic/test_enum_variation.h</b> ファイルで定義しています。<br>
また、テスト処理は<b>source/reference_and_test/basic/test_enum_variation.cpp</b> ファイルで定義しており、saveEnumVariation()関数で保存処理、loadEnumVariation()関数で回復処理と値の検証を行っています。

<b>test_enum_variation.h</b> ファイル先頭で<b>DEFINE_ENUM()マクロ</b>を定義しています。<br>

@dontinclude basic/test_enum_variation.h
@skip DEFINE_ENUM
@until unsigned long long

C++11規格でenum型の値を保存する整数型を基底型として指定できるようになりました。その全ての基底型（指定していないものも含む）に対して自動テストを容易に展開できるようにするためのマクロです。<br>
このマクロを展開する直前でDEFINE()マクロを定義することで、enum型の定義、初期化、保存、回復、値チェックのためのコードを生成しています。

下記の３種類のTheolizer機能に分けて自動テストを実装しています。

  - 非侵入型完全自動
  - 非侵入型半自動（シンボル名対応）
  - 非侵入型半自動（シンボル値対応）

それぞれについて下記定義を行っています。

  1. 通常のenum型定義
  2. Scoped enum型定義
  3. トップ・レベルの保存（左辺値）／保存（右辺値）／回復関数定義
    - 非侵入型完全自動<br>
        saveLValueEnumFullAuto, saveRValueEnumFullAuto, loadEnumFullAuto
    - 非侵入型半自動（シンボル名対応）<br>
        saveLValueEnumSymName,  saveRValueEnumSymName,  loadEnumSymName
    - 非侵入型半自動（シンボル値対応）<br>
        saveLValueEnumFSymVal,  saveRValueEnumFSymVal,  loadEnumSymVal
  4. 非侵入型完全自動クラスのメンバとしての定義（メンバ・リスト自動生成対応の検証のため）
  5. 非侵入型手動クラスのメンバとしての定義（非トップ・レベルの保存／回復検証のため）

3.と5.については、THEOLIZER_PROCESS()で保存／回復しますが、THEOLIZER_PROCESS()は定数の保存も可能なので、左辺値と右辺値（定数)をテストします。定数への回復はできないので、回復は左辺値のみです。

*/
