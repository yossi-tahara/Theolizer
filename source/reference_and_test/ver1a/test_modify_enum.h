//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          enum型変更の使い方
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

#if !defined(DISABLE_MODIFY_ENUM_TEST)
#define DISABLE_MODIFY_ENUM_TEST

#include <string>

// ***************************************************************************
//      enum型(完全自動型)
// ***************************************************************************

enum EnumFullAuto
{
    eefaDefault,

    // シンボル値を変更するもの
    eefaValue1,

    // シンボル名と値を変更するもの
    eefaName11,             // 単にシンボル変更
    eefaName20,             // 3つのシンボルを先頭の1つへ統合する
    eefaName21,
    eefaName22,
    eefaName31,             // 3つのシンボルを別の1つへ統合する
    eefaName32,
    eefaName33,
};

enum class ScopedEnumFullAuto : long
{
    Default,

    // シンボル値を変更するもの
    Value1,

    // シンボル名と値を変更するもの
    Name11,                 // 単にシンボル変更
    Name20,                 // 3つのシンボルを先頭の1つへ統合する
    Name21,
    Name22,
    Name31,                 // 3つのシンボルを別の1つへ統合する
    Name32,
    Name33,
};

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
    eesnValue1,

    // シンボル名と値を変更するもの
    eesnName11,             // 単にシンボル変更
    eesnName20,             // 3つのシンボルを先頭の1つへ統合する
    eesnName21,
    eesnName22,
    eesnName31,             // 3つのシンボルを別の1つへ統合する
    eesnName32,
    eesnName33,
};
THEOLIZER_ENUM(EnumSymName, 1);

enum class ScopedEnumSymName
{
    Default,

    // シンボル値を変更するもの
    Value1,

    // シンボル名と値を変更するもの
    Name11,                 // 単にシンボル変更
    Name20,                 // 3つのシンボルを先頭の1つへ統合する
    Name21,
    Name22,
    Name31,                 // 3つのシンボルを別の1つへ統合する
    Name32,
    Name33,
};
THEOLIZER_ENUM(ScopedEnumSymName, 1);

//----------------------------------------------------------------------------
//      非侵入型半自動（シンボル値対応）
//----------------------------------------------------------------------------

enum EnumSymVal
{
    eesvDefault,

    // シンボル名を変更するもの
    eesvName1,

    // シンボル名と値を変更するもの
    eesvValue11=11,         // 単にシンボル変更
    eesvValue20=20,         // 3つのシンボルを先頭の1つへ統合する
    eesvValue21=21,
    eesvValue22=22,
    eesvValue31=31,         // 3つのシンボルを別の1つへ統合する
    eesvValue32=32,
    eesvValue33=33
};
THEOLIZER_ENUM_VALUE(EnumSymVal, 1);

enum class ScopedEnumSymVal
{
    Default=100,

    // シンボル名を変更するもの
    Name1  =101,

    // シンボル値を変更するもの
    Value11=110,                // 単にシンボル変更
    Value20=200,                // 3つのシンボルを先頭の1つへ統合する
    Value21=210,
    Value22=220,
    Value31=310,                // 3つのシンボルを別の1つへ統合する
    Value32=320,
    Value33=330
};
THEOLIZER_ENUM_VALUE(ScopedEnumSymVal, 1);

#endif  // DISABLE_MODIFY_ENUM_TEST
