//############################################################################
/*!
    @brief      ドキュメント・ファイル－使用方法（個別）
    @ingroup    Documents
    @file       7f.modify_enum.h
    @author     Yoshinori Tahara
    @date       2017/03/07 Created
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
    @page ChangingEnum enum型のアップデート／バージョン・アップ方法
ここでは、enum型を修正した時、古いプログラムが保存したデータを回復するための各種指定方法を説明します。<br>

<br>
//############################################################################
@section HowToModifyEnum1 1.バージョン番号を変えないでenum型を修正
//############################################################################

バージョン番号を変えない場合に可能なenum型の修正は以下のものがあります。

1. シンボル名保存の場合
  - シンボル値の変更<br>
シリライズ・データ中にシンボル値は記録されないため、シリアライズ処理とは無関係な変更です。<br>
従って、特にシリアライズ用に指定する必要はありません。<br>
  - シンボル名の変更<br>
変更前のシンボル名で保存されたシリアライズ・データを回復できるようにするためには、変更前のシンボル名がどのシンボルへ対応するのか、Theolizerへ教える必要があります。<br>
そのためにTHEOLIZER_ANNOTATE(ES)を使います。<br>
<br>
2. シンボル値保存の場合
  - シンボル名の変更<br>
シリライズ・データ中にシンボル名は記録されないため、シリアライズ処理とは無関係な変更です。<br>
従って、特にシリアライズ用に指定する必要はありません。<br>
  - シンボル値の変更<br>
変更前のシンボル値で保存されたシリアライズ・データを回復できるようにするためには、変更前のシンボル値がどのシンボルへ対応するのか、Theolizerへ教える必要があります。<br>
そのためにTHEOLIZER_ANNOTATE(ES)を使います。<br>
<br>
3. 共通
  - シンボルの追加<br>
古いシリアライズ・データに追加したシンボル名／値が記録されることはありませんので、シリアライズ用の対応は何もありません。
  - シンボルの削除<br>
削除前のシンボル名／値に対応するシンボルをTHEOLIZER_ANNOTATE(ES)で指定していない場合、削除されたシンボル名／値を回復する時に警告がでます。それを避けるためには、例えばデフォルトのシンボルを用意しておき、そこへシンボル名／値が変更された旨を指定して下さい。なお、多対１で指定可能ですので複数のシンボルを削除した場合にも対応できます。<br>

注：シンボル値保存／シンボル名保存の指定方法については@ref EnumVariation を参照下さい。

@subsection HowToModifyEnum11 1-1.バージョンを変えない場合のTHEOLIZER_ANNOTATE(ES)

指定方法は以下の通りです。

@code
enumシンボル THEOLIZER_ANNOTATE(ES:シンボル名リスト:シンボル値リスト)=値,
    :
@endcode
シンボル名リストは「シンボル名保存」時に指定します。
シンボル値リストは「シンボル値保存」時に指定します。

@subsubsection HowToModifyEnum111 1-1-1.シンボル名保存の変更サンプル

下記の修正を行うサンプルです。

- eesnName11をeesnName10へ変更
- eesnName20, eesnName21, eesnName22をeesnName20へ統合<br>
「eesnName21, eesnName22を削除して、eesnName20へ割り当てた」のと同じです。

<b>変更前(ver1a)のソース</b>(source/reference_and_test/ver1a/test_modify_enum.h)

@dontinclude ver1a/test_modify_enum.h
@skip EnumSymName
@until {
@skip eesnName11
@until eesnName22
(後略)

<b>変更後(ver1b)のソース</b>(source/reference_and_test/ver1b/test_modify_enum.h)

@dontinclude ver1b/test_modify_enum.h
@skip EnumSymName
@until {
@skip eesnName10
@until eesnName20
(後略)

@subsubsection HowToModifyEnum112 1-1-2.シンボル値保存の変更サンプル

修正内容はEnumSymNameの場合と同等です。

<b>変更前(ver1a)のソース</b>(source/reference_and_test/ver1a/test_modify_enum.h)

@dontinclude ver1a/test_modify_enum.h
@skip EnumSymVal
@until {
@skip eesvValue11
@until eesvValue22
(後略)

<b>変更後(ver1b)のソース</b>(source/reference_and_test/ver1b/test_modify_enum.h)

@dontinclude ver1b/test_modify_enum.h
@skip EnumSymVal
@until {
@skip eesvValue10
@until eesvValue20
(後略)

<br>
//############################################################################
@section HowToMakeGlobalVersionNoTable2 2.グローバル・バージョン番号テーブル生成
//############################################################################

enum型解説の途中ですが、ここでバージョン・アップするための手順を説明します。<br>
今までバージョン管理していなかった時にバージョン管理を始める時、@ref Basic32 で説明した「グローバル・バージョン番号テーブル」の生成が必要になります。

そのために必要な作業は以下の通りです。
- グローバル・バージョン番号テーブルの宣言
- グローバル・バージョン番号テーブル実体定義

@subsection HowToMakeGlobalVersionNoTable21 2-1.グローバル・バージョン番号テーブルの宣言

シリライズ処理するコンパイル単位全てがインクルードするヘッダファイルで宣言して下さい。<br>
具体的には下記書式で宣言します。

@code
THEOLIZER_DEFINE_GLOBAL_VERSION_TABLE(テーブル名, グローバル・バージョン番号);
@endcode

テーブル名は任意の識別子です。これはtheolizer::internal::global_table名前空間内で必要なコードを定義する時の識別子の一部として用いられます。<br>
グローバル・バージョン番号は最初は必ず1として下さい。その後、バージョンを上げる度に1づつインクリメントして下さい。

<b>サンプルをver1cに用意してます。</b>(source/reference_and_test/ver1c/common.h)

@dontinclude ver1c/common.h
@skip THEOLIZER_DEFINE_GLOBAL_VERSION_TABLE
@until THEOLIZER_DEFINE_GLOBAL_VERSION_TABLE


@subsection HowToMakeGlobalVersionNoTable22 2-2.グローバル・バージョン番号テーブル実体定義

グローバル・バージョン番号テーブル実体は通常のグローバル変数(配列)として定義されますので、どれか1つのコンパイル単位でのみ実体定義します。

@code
#define THEOLIZER_GLOBAL_VERSION_TABLE
@endcode

そのコンパイル単位のソース・ファイル(.cpp)の先頭付近で下記を定義することで、Theolizerドライバがこのマクロ定義を認識し、実体を*.theolizer.hpp内に自動生成します。

また、そのコンパイル単位をTheolizerドライバが解析する時に、シリアライズする全てのクラスとenum型の定義とそのシリアライズ指定が必要になります。そこで、当該コンパイル単位ではシリアライズする全ての型を定義したヘッダ・ファイルをインクルードして下さい。

<b>サンプルをver1cに用意してます。</b>(source/reference_and_test/ver1c/main.cpp)

@dontinclude ver1c/main.cpp
@skip theolizerライブラリ
@until main.cpp.theolizer.hpp


<br>
//############################################################################
@section HowToModifyEnum3 3.バージョン番号を変えてenum型を修正
//############################################################################

バージョン番号を更新して、enum型をバージョン・アップすることで以下の変更ができます。

1. シンボル名保存とシンボル値保存を切り替えることが可能<br>
ローカル・バージョン番号を１つ上げて、シリアライズ指定マクロ名を変えるだけです。

2. 過去割り当てていたシンボルを別の意味で再割当てできる<br>
バージョン番号を変えない場合、古いプログラムで保存したシンボルを他のシンボルへ割り当てることができますので、古いシンボルは新しいシンボルの意味と同義であり、変更できません。<br>
バージョン番号を更新することで、新しいバージョンでは古いシンボルは初めて出現するため、それを新たに割り当てしても混乱を生じないためです。




@subsection HowToModifyEnum12 1-2.バージョンを変えない場合の注意事項

古いプログラムで保存していたシンボル名やシンボル値を別の意味で用いることはできません。<br>



@subsection HowToModifyEnum21 2-1.標準コンテナを通常の使い方で保存／回復するサンプル・ソース

<b>サンプル・ソース（source/reference_and_test/basic/test_support_stl.cpp）</b><br>

@snippet  basic/test_support_stl.cpp ContaierNomal


*/
