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
    // 変更無し
    eefaDefault,
    eefaValue1=10,
    eefaName10,
    eefaName20,
    eefaName41,
    eefaName42,
    eefaName30,
    eefaName51,
    eefaName52,
    eefaDeleted,
    eefaName11  THEOLIZER_ANNOTATE(ES:::0)
};
THEOLIZER_ENUM(EnumFullAuto, 2);

enum class ScopedEnumFullAuto : long
{
    // 変更無し
    Default,
    Value1=10,
    Name10,
    Name20,
    Name41,
    Name42,
    Name30,
    Name51,
    Name52,
    Deleted,
    Name11  THEOLIZER_ANNOTATE(ES:::0)
};
THEOLIZER_ENUM(ScopedEnumFullAuto, 2);

// ***************************************************************************
//      enum型(半自動型)
// ***************************************************************************

//----------------------------------------------------------------------------
//      非侵入型半自動（シンボル名→シンボル値対応）
//----------------------------------------------------------------------------

enum EnumSymName
{
    // 変更無し
    eesnDefault,
    eesnValue1=10,
    eesnName10,
    eesnName20,
    eesnName21, // シンボル名変更(値を変えないので特別な操作は不要)
    eesnName22, // シンボル名変更(値を変えないので特別な操作は不要)
    eesnName30,
    eesnName31, // シンボル名変更(値を変えないので特別な操作は不要)
    eesnName32, // シンボル名変更(値を変えないので特別な操作は不要)
    eesnDeleted THEOLIZER_ANNOTATE(ES:::18)  =99,   // 前バージョンは18
    eesnName11  THEOLIZER_ANNOTATE(ES::19:19)=70    // 変更前と前バージョンの両方が19
};
THEOLIZER_ENUM_VALUE(EnumSymName, 3);

enum class ScopedEnumSymName
{
    // 変更無し
    Default,
    Value1=10,
    Name10,
    Name20,
    Name21,     // シンボル名変更(値を変えないので特別な操作は不要)
    Name22,     // シンボル名変更(値を変えないので特別な操作は不要)
    Name30,
    Name31,     // シンボル名変更(値を変えないので特別な操作は不要)
    Name32,     // シンボル名変更(値を変えないので特別な操作は不要)
    Deleted     THEOLIZER_ANNOTATE(ES:::18)  =99,   // 前バージョンは18
    Name11      THEOLIZER_ANNOTATE(ES::19:19)=70    // 変更前と前バージョンの両方が19
};
THEOLIZER_ENUM_VALUE(ScopedEnumSymName, 3);

//----------------------------------------------------------------------------
//      非侵入型半自動（シンボル値→シンボル名対応）
//----------------------------------------------------------------------------

enum EnumSymVal
{
    // 変更無し
    eesvDefault,
    eesvName10,
    eesvValue10=10,
    eesvValue20=20,
    eesvValue21     THEOLIZER_ANNOTATE(ES:eesvValue41:)=41,
    eesvValue22     THEOLIZER_ANNOTATE(ES:eesvValue42:)=42,
    eesvValue30=30,
    eesvValue31     THEOLIZER_ANNOTATE(ES:eesvValue51:)=51,
    eesvValue32     THEOLIZER_ANNOTATE(ES:eesvValue52:)=52,
    eesvDeleted     THEOLIZER_ANNOTATE(ES:::99)        =18, // 値で前バージョン対応するので要指定
    eesvValue11     THEOLIZER_ANNOTATE(ES:::70)        =19  // 値で前バージョン対応するので要指定
};
THEOLIZER_ENUM(EnumSymVal, 3);

enum class ScopedEnumSymVal
{
    // 変更無し
    Default=100,
    Name10 =101,
    Value10=110,
    Value20=200,
    Value21         THEOLIZER_ANNOTATE(ES:Value41:)=410,
    Value22         THEOLIZER_ANNOTATE(ES:Value42:)=420,
    Value30=300,
    Value31         THEOLIZER_ANNOTATE(ES:Value51:)=510,
    Value32         THEOLIZER_ANNOTATE(ES:Value52:)=520,
    Deleted         THEOLIZER_ANNOTATE(ES:::999)   =18,     // 値で前バージョン対応するので要指定
    Value11         THEOLIZER_ANNOTATE(ES:::700)   =19      // 値で前バージョン対応するので要指定
};
THEOLIZER_ENUM(ScopedEnumSymVal, 3);

#endif  // TEST_MODIFY_ENUM_H
