//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          enum型変更の使い方
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

#if !defined(TEST_MODIFY_ENUM_H)
#define TEST_MODIFY_ENUM_H

#include <string>

// ***************************************************************************
//      enum型(完全自動型)
// ***************************************************************************

enum EnumFullAuto
{
    eefaDefault,

    // シンボル値を変更するもの
    eefaValue1=10,

    // シンボル名と値を変更するもの
    // 単にシンボル変更
    eefaName10  THEOLIZER_ANNOTATE(ES:eefaName11),

    // 3つのシンボルを先頭の1つへ統合する
    eefaName20  THEOLIZER_ANNOTATE(ES:eefaName21,eefaName22),
    eefaName41,
    eefaName42,

    // 3つのシンボルを別の1つへ統合する
    eefaName30  THEOLIZER_ANNOTATE(ES:eefaName31,eefaName32,eefaName33),
    eefaName51,
    eefaName52,

    // 削除されたものを指定し、回復できることを確認する
    eefaDeleted THEOLIZER_ANNOTATE(ES:eefaDelete)
};
THEOLIZER_ENUM(EnumFullAuto, 1);            // バージョン・アップに備え半自動型へ変更

enum class ScopedEnumFullAuto : long
{
    Default,

    // シンボル値を変更するもの
    Value1=10,

    // シンボル名と値を変更するもの
    // 単にシンボル変更
    Name10  THEOLIZER_ANNOTATE(ES:Name11),

    // 3つのシンボルを先頭の1つへ統合する
    Name20  THEOLIZER_ANNOTATE(ES:Name21,Name22),
    Name41,
    Name42,

    // 3つのシンボルを別の1つへ統合する
    Name30  THEOLIZER_ANNOTATE(ES:Name31,Name32,Name33),
    Name51,
    Name52,

    // 削除されたものを指定し、回復できることを確認する
    Deleted THEOLIZER_ANNOTATE(ES:Delete)
};
THEOLIZER_ENUM(ScopedEnumFullAuto, 1);  // バージョン・アップに備え半自動型へ変更

// ***************************************************************************
//      enum型(半自動型)
// ***************************************************************************

//----------------------------------------------------------------------------
//      非侵入型半自動（シンボル名対応）
//----------------------------------------------------------------------------

enum EnumSymName
{
    eesnDefault,

    // シンボル値を変更するもの
    eesnValue1=10,

    // シンボル名と値を変更するもの
    // 単にシンボル変更
    eesnName10  THEOLIZER_ANNOTATE(ES:eesnName11),

    // 3つのシンボルを先頭の1つへ統合する
    eesnName20  THEOLIZER_ANNOTATE(ES:eesnName21,eesnName22),
    eesnName41,
    eesnName42,

    // 3つのシンボルを別の1つへ統合する
    eesnName30  THEOLIZER_ANNOTATE(ES:eesnName31,eesnName32,eesnName33),
    eesnName51,
    eesnName52,

    // 削除されたものを指定し、回復できることを確認する
    eesnDeleted THEOLIZER_ANNOTATE(ES:eesnDelete)
};
THEOLIZER_ENUM(EnumSymName, 1);

enum class ScopedEnumSymName
{
    Default,

    // シンボル値を変更するもの
    Value1=10,

    // シンボル名と値を変更するもの
    // 単にシンボル変更
    Name10  THEOLIZER_ANNOTATE(ES:Name11),

    // 3つのシンボルを先頭の1つへ統合する
    Name20  THEOLIZER_ANNOTATE(ES:Name21,Name22),
    Name41,
    Name42,

    // 3つのシンボルを別の1つへ統合する
    Name30  THEOLIZER_ANNOTATE(ES:Name31,Name32,Name33),
    Name51,
    Name52,

    // 削除されたものを指定し、回復できることを確認する
    Deleted THEOLIZER_ANNOTATE(ES:Delete)
};
THEOLIZER_ENUM(ScopedEnumSymName, 1);

//----------------------------------------------------------------------------
//      非侵入型半自動（シンボル値対応）
//----------------------------------------------------------------------------

enum EnumSymVal
{
    eesvDefault,

    // シンボル名を変更するもの
    eesvName10,

    // シンボル名と値を変更するもの
    // 単にシンボル変更
    eesvValue10     THEOLIZER_ANNOTATE(ES::11)=10,

    // 3つのシンボルを先頭の1つへ統合する
    eesvValue20     THEOLIZER_ANNOTATE(ES::21,22)=20,
    eesvValue41=41,
    eesvValue42=42,

    // 3つのシンボルを別の1つへ統合する
    eesvValue30     THEOLIZER_ANNOTATE(ES::31,32,33)=30,
    eesvValue51=51,
    eesvValue52=52,

    // 削除されたものを指定し、回復できることを確認する
    eesvDeleted     THEOLIZER_ANNOTATE(ES::60)=99
};
THEOLIZER_ENUM_VALUE(EnumSymVal, 1);

enum class ScopedEnumSymVal
{
    Default=100,

    // シンボル名を変更するもの
    Name10 =101,

    // シンボル名と値を変更するもの
    // 単にシンボル変更
    Value10     THEOLIZER_ANNOTATE(ES::111)=110,

    // 3つのシンボルを先頭の1つへ統合する
    Value20     THEOLIZER_ANNOTATE(ES::210,220)=200,
    Value41=410,
    Value42=420,

    // 3つのシンボルを別の1つへ統合する
    Value30     THEOLIZER_ANNOTATE(ES::310,320,330)=300,
    Value51=510,
    Value52=520,

    // 削除されたものを指定し、回復できることを確認する
    Deleted     THEOLIZER_ANNOTATE(ES::600)=999
};
THEOLIZER_ENUM_VALUE(ScopedEnumSymVal, 1);

#endif  // TEST_MODIFY_ENUM_H
