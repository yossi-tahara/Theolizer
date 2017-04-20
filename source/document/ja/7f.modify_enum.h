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

### デフォルト値について

最初にデフォルト値について説明します。<br>
enumのシンボルを定義している先頭のシンボルの値をTheolizerは「デフォルト値」として用います。<br>
THEOLIZER_ANNOTATE(ES)による指定を間違った場合、enum値保存／回復やバージョン・アップ／ダウン時に対応するシンボルが見つからない場合があります。その時、警告を報告(例外にはなりません。)し、変換先の値を「デフォルト値」にします。

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
そのためにTHEOLIZER_ANNOTATE(ES:)の第１パラメータを使います。<br>
<br>
2. シンボル値保存の場合
  - シンボル名の変更<br>
シリライズ・データ中にシンボル名は記録されないため、シリアライズ処理とは無関係な変更です。<br>
従って、特にシリアライズ用に指定する必要はありません。<br>
  - シンボル値の変更<br>
変更前のシンボル値で保存されたシリアライズ・データを回復できるようにするためには、変更前のシンボル値がどのシンボルへ対応するのか、Theolizerへ教える必要があります。<br>
そのためにTHEOLIZER_ANNOTATE(ES:)の第２パラメータを使います。<br>
<br>
3. 共通
  - シンボルの追加<br>
古いシリアライズ・データに、新規追加したシンボル名／値が記録されることはありませんので、シリアライズ用の対応は何もありません。
  - シンボルの削除<br>
削除前のシンボル名／値に対応するシンボルをTHEOLIZER_ANNOTATE(ES:)で指定していない場合、削除されたシンボル名／値を回復する時に警告が報告されます。それを避けるためには、例えば「デフォルト値」へシンボル名／値が変更されたように指定して下さい。複数の旧シンボルを１つの現シンボルへ指定可能ですので複数のシンボルを削除した場合にも対応できます。<br>

注：シンボル値保存／シンボル名保存の指定方法については@ref EnumVariation を参照下さい。

THEOLIZER_ANNOTATE(ES:)の指定方法は以下の通りです。

@code
enumシンボル THEOLIZER_ANNOTATE(ES:シンボル名リスト:シンボル値リスト)=値,
    :
@endcode
「シンボル名リスト」はシンボル名保存時に指定します。<br>
「シンボル値リスト」はシンボル値保存時に指定します。
「=値」は通常のenum型と同じく省略可能です。

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
<b>THEOLIZER_ANNOTATE(ES:)を記述する位置が直感的な位置ではないですので、ご注意下さい。</b><br>
シンボルの<b>直後</b>です。この機能の実装にclangの属性指定(__attribute__())機能を使っていますが、enum型のシンボルの属性指定は、この位置のみ書けます。
</div>

<br>
@subsection HowToModifyEnum11 1-1.シンボル名保存の変更サンプル

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

<br>
@subsection HowToModifyEnum12 1-2.シンボル値保存の変更サンプル

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
@section HowToMakeGlobalVersionNoTable 2.グローバル・バージョン番号テーブル生成
//############################################################################

enum型解説の途中ですが、ここでバージョン・アップ前の準備について説明します。<br>
今までバージョン管理していなかった時にバージョン管理を始める時、@ref Basic32 で説明した「グローバル・バージョン番号テーブル」の生成が必要になります。

そのために必要な作業は以下の通りです。
- グローバル・バージョン番号テーブルの宣言
- グローバル・バージョン番号テーブル実体定義

<br>
@subsection HowToMakeGlobalVersionNoTable21 2-1.グローバル・バージョン番号テーブルの宣言

シリライズ処理するコンパイル単位全てがインクルードするヘッダファイルで宣言して下さい。<br>
具体的には下記書式で宣言します。

@code
THEOLIZER_DEFINE_GLOBAL_VERSION_TABLE(テーブル名, グローバル・バージョン番号);
@endcode

テーブル名は任意の識別子です。これはtheolizer::internal::global_table名前空間内で必要なコードを定義する時の識別子の一部として用いられます。<br>
グローバル・バージョン番号は最初は必ず1として下さい。その後、バージョンを上げる度に1づつインクリメントして下さい。

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
グローバル・バージョン番号が１つ増える度にクラスやenum型のローカル・バージョン番号も最大1つ上げることができます。既に現在のグローバル・バージョン番号でクラスかenum型のローカル・バージョンを上げており、それを更に上げる場合はグローバル・バージョン番号も上げる必要があります。<br>
グローバル・バージョン番号が決まればクラスやenum型のローカル・バージョン番号も決まるようにする必要があるためです。</div>

<b>サンプルをver1cに用意してます。</b>(source/reference_and_test/ver1c/common.h)

@dontinclude ver1c/common.h
@skip THEOLIZER_DEFINE_GLOBAL_VERSION_TABLE
@until THEOLIZER_DEFINE_GLOBAL_VERSION_TABLE

<br>
@subsection HowToMakeGlobalVersionNoTable22 2-2.グローバル・バージョン番号テーブル実体定義

グローバル・バージョン番号テーブル実体は通常のグローバル変数(配列)として定義されますので、どれか1つのコンパイル単位でのみ実体定義します。

@code
#define THEOLIZER_GLOBAL_VERSION_TABLE
@endcode

そのコンパイル単位のソース・ファイル(.cpp)の先頭付近で下記を定義することで、Theolizerドライバがこのマクロ定義を認識し、実体をその.cppファイル用の*.theolizer.hpp内に自動生成します。

また、そのコンパイル単位をTheolizerドライバが解析する時に、シリアライズする全てのクラスとenum型の定義とそのシリアライズ指定が必要になります。そこで、当該コンパイル単位ではシリアライズする全ての型を定義したヘッダ・ファイルをインクルードして下さい。

<b>サンプルをver1cに用意してます。</b>(source/reference_and_test/ver1c/main.cpp)

@dontinclude ver1c/main.cpp
@skip theolizerライブラリ
@until main.cpp.theolizer.hpp

<br>
//############################################################################
@section HowToModifyEnum3 3.バージョン番号を変えてenum型を修正
//############################################################################

バージョン番号を更新して、enum型をバージョン・アップすることで次の変更ができます。

- 過去割り当てていたシンボルを別の意味で再割当て<br>
バージョン番号を変えない場合、古いプログラムで保存したシンボルを他のシンボルへ割り当てるため、古いシンボルも割り当てが存在するため変更できません。<br>
バージョン番号を更新することで、新しいバージョンでは古いシンボルは未定義なので、それを新たに割り当てしても混乱を生じません。<br>
<br>
- シンボル名保存とシンボル値保存の切り替え<br>
ローカル・バージョン番号を１つ上げて、シリアライズ指定マクロ名を変えるだけです。

@subsection HowToModifyEnum31 3-1.バージョン・アップ手順

enum型をバージョン・アップする時の手順は次の通りです。

1. まだ作っていないなら、グローバル・バージョン番号テーブルを作成しておきます。<br>
<br>
2. 対象enum型が非侵入型<b>完全自動</b>ならば非侵入型<b>半自動</b>へ変更します。<br>
完全自動のバージョン・アップに対応していません。<br>
<br>
3. ビルドし、Theolizerドライバにて自動生成ソースへ反映します。<br>
<br>
4. 必要な場合はグローバル・バージョン番号をインクリメントします。<br>
@ref HowToMakeGlobalVersionNoTable21 参照。<br>
<br>
5. 対象enum型のローカル・バージョン番号をインクリメントします。<br>
@ref HalfAutoEnumSymName 参照。<br>
この時、バージョン・アップ前のバージョンで付けていたTHEOLIZER_ANNOTATE()を一旦全て削除します。<br>
前バージョンで付けていたものは全て前バージョンへ指定ですので、次のバージョンでは指定内容が完全に異なります。<br>
<br>
6. 対象enum型の定義を変更し、前バージョンとの対応付けを行います。<br>
具体的には後述します。<br>
<br>
7. ビルドします。<br>

<br>
@subsection HowToModifyEnum32 3-2.非侵入型半自動への変更方法
単純にTHEOLIZER_ENUM()マクロで指定しするだけです。対象のenum型定義よりも後、*.theolizer.hppをインクルードする前にTHEOLIZER_ENUM()マクロを置いて下さい。なお、THEOLIZER_ENUM_VALUE()でシンボル値保存へ直接変更することはできません。シンボル値保存へ変更する場合は、ローカル・バージョン番号を更新する時に行って下さい。<br>

<b>サンプル</b><br>
test_modify_enum.hで定義しているEnumFullAutoとScopedEnumFullAutoはver1a, ver1bまでは非侵入型完全自動ですが、ver1cにて非侵入型半自動へ修正しています。<br>

<b>半自動型へ変更した(ver1c)のソース</b>(source/reference_and_test/ver1c/test_modify_enum.h)

@code

enum EnumFullAuto
{
    // 中略
};
THEOLIZER_ENUM(EnumFullAuto, 1);            // バージョン・アップに備え半自動型へ変更

enum class ScopedEnumFullAuto : long
{
    // 中略
};
THEOLIZER_ENUM(ScopedEnumFullAuto, 1);      // バージョン・アップに備え半自動型へ変更

@endcode

<br>
@subsection HowToModifyEnum33 3-3.バージョン・アップ時のシンボル対応
enum型はバージョン・アップする時、前バージョンと現バージョン間のシンボル値の対応を指定することができます。現バージョンのシンボルに対して、対応する前バージョンのシンボル値を指定します。<br>

この場合、THEOLIZER_ANNOTATE(ES:)の第３パラメータで指定します。

@code
enumシンボル THEOLIZER_ANNOTATE(ES:::対応する前バージョンのシンボル値)=値,
    :
@endcode

「対応する前バージョンのシンボル値」はenumシンボルではなく直接数値で指定して下さい。前バージョンのenum型そのものは存在しません。しかし、旧版のenum型シンホルの値を変更することも有りえませんので指定可能です。<br>
また、前バージョンとシンボル値が変化していない場合は「対応する前バージョンのシンボル値」の指定を省略できます。
<br>
@subsubsection HowToModifyEnum331 3-3-1.内部処理概要

- バージョン・アップ処理<br>
前バージョンのシンボル値に対して、次のバージョンの各シンボルを先頭から順に枚挙し、「対応する前バージョンのシンボル値」が一致するものを探します。最初に一致したシンボルへ変換します。<br>
複数のシンボルが一致する時は、先に定義されたシンポルへ変換されます。<br>
なお、一致するものが見つからなかった時は、デフォルト値を設定し、警告を報告します。<br>
<br>
- バージョン・ダウン処理<br>
前バージョンの各シンボルを先頭から順に枚挙し、現バージョンのシンボルの「対応する前バージョンのシンボル値」と一致するものを探します。最初に一致したシンボルへ変換します。<br>
複数のシンボルが一致する時は、先に定義されたシンポルへ変換されます。<br>
なお、一致するものが見つからなかった時は、デフォルト値を設定し、警告を報告します。<br>
<br>
@subsubsection HowToModifyEnum332 3-3-2.以前使用したシンボルの再割当て
これは、シンボルを名前変更したり削除(デフォルト値へ割り当て)したりした後、変更前に使っていたシンボルを他の意味で使いたいケースを想定しています。<br>
この場合、バージョン・アップすることで、そのシンボルを他のシンボルへの再割当てすることができます。その際、@ref HowToModifyEnum33 の方法で指定します。<br>

再割当てしたいシンボルを「enumシンボル」に追加します。<br>
この時、「対応する前バージョンのシンボル値」には前バージョンにおける「デフォルト値」を指定して下さい。<br>
<br>

なお、上記の指定方法で、バージョン・アップ前のバージョンの特定のシンボルを、以前使っていたシンボルへ戻すことも可能です。ただし、これはシンボルの意味の変更を伴いませんので、バージョン・アップしても問題ないですが、バージョン・アップする必要はありません。

下記の修正を行うサンプルです。

- 旧バージョンの各種指定を削除する
- 以前使っていたeesnName11を新しいバージョンのenum型へ新規追加する


<b>バージョン・アップ前(ver1c)のソース</b>(source/reference_and_test/ver1c/test_modify_enum.h)

@dontinclude ver1c/test_modify_enum.h
@skip  EnumSymName
@until eesnDefault
@skip  eesnValue1
@until eesnValue1
@skip  eesnName10
@until eesnName10
@skip  eesnName20
@until eesnName42
@skip  eesnName30
@until eesnName52
@skip  eesnDeleted
@until };

<b>バージョン・アップ後(ver2a)のソース</b>(source/reference_and_test/ver2a/test_modify_enum.h)

@dontinclude ver2a/test_modify_enum.h
@skip EnumSymName
@until };

<br>
@subsubsection HowToModifyEnum34 3-4.シンボル名保存とシンボル値保存の切り替え
開発当初は変更対応しやすいシンボル名保存が使い勝手が良いです。（`C#`での経験ですが、シンボル値を気にしないで良いのはたいへんありがたかったです。)<br>
ある程度枯れてきたら、シンボル名では効率が悪いのでシンボル値へ変更したくなります。<br>
そして、再度、大改造する際に一旦シンボル名へ戻したい時もあるでしょう。<br>
このようなシナリオに対応することを想定しています。

変更は@ref HowToModifyEnum31 の手順に従います。この6.の手順にて下記マクロ名を切り替えます。<br>

- THEOLIZER_ENUM
- THEOLIZER_ENUM_VALUE

下記の修正を行うサンプルです。

- 旧バージョンのシンボル名保存enum型をシンボル値保存enum型へ変更(EnumSymName)


<b>バージョン・アップ前(ver2a)のソース</b>(source/reference_and_test/ver2a/test_modify_enum.h)

@dontinclude ver2a/test_modify_enum.h
@skip  EnumSymName
@skip  THEOLIZER_ENUM
@until THEOLIZER_ENUM

<b>バージョン・アップ後(ver3a)のソース</b>(source/reference_and_test/ver3a/test_modify_enum.h)

@dontinclude ver3a/test_modify_enum.h
@skip  EnumSymName
@skip  THEOLIZER_ENUM_VALUE
@until THEOLIZER_ENUM_VALUE
<br>
//############################################################################
@section HowToModifyEnum4 4.網羅的な使用例（自動テスト）の説明
//############################################################################
enum型の修正に関する網羅的なテストは @ref TestProgram で説明した各フォルダの下記2つのファイルにて実装しています。

- test_modify_enum.h
- test_modify_enum.cpp

unscoped enum型とscoped enum型について下記の修正を行い、@ref TestProgram412 の各バージョン間で正しく回復できることを確認しています。

|バージョン|修正点|特記事項|
|----------|------|--------|
|ver1a→ver1b|1.シンボル値変更<br>2.シンボル名変更(a→b)<br>3.シンボル名変更(a,b,c→a)<br>4.シンボル名変更(a,b,c→d)<br>5.シンボル名削除(指定ミス)|<br>単純変更<br>3つから先頭の1つへ対応<br>3つから別の1つへ対応<br>指定ミスは対応先指定漏れ警告が出ることを確認|
|ver1b→ver1c|1.グローバル・バージョン番号テーブル作成<br>2.完全自動型を半自動型へ変更<br>3.シンボル名削除指定ミスの修正|警告が消えることを確認|
|ver1c→ver2a|1.バージョン・アップ<br>2.以前使っていたシンボルの再割当て|   |
|ver2a→ver3a|1.バージョン名保存→バージョン値保存<br>2.バージョン値保存→バージョン名保存<br>3.前バージョンとの対応指定ミス|バージョン・アップ処理と回復処理で警告が出ることを確認<br>(元完全自動型はバージョン・アップ無し)|
|ver3a→ver3b|指定ミスの修正|警告が消えることを確認<br>(元完全自動型はバージョン・アップ無し)|

ver3a/test_modify_enum.cppのtutoriseModifyEnum()関数にて対応シンボルが無い時のバージョン・ダウン処理、および、保存処理に於ける警告検出をjson形式を用いてテストしています。

ver3b/test_modify_enum.cppのtutoriseModifyEnum()関数にて上記警告が消えることをテストしています。

*/
