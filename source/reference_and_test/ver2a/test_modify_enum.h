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

    // 「次のバージョン・アップで名前変更ミス」のテストを行う
    eefaDeleted,

    // 以前使ったシンボルを追加(前バージョンのeefaDefaultに対応させる)
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

    // 以前使ったシンボルを追加(前バージョンのeefaDefaultに対応させる)
    Name11  THEOLIZER_ANNOTATE(ES:::0)
};
THEOLIZER_ENUM(ScopedEnumFullAuto, 2);

// ***************************************************************************
//      enum型(半自動型)
// ***************************************************************************

//----------------------------------------------------------------------------
//      非侵入型半自動（シンボル名対応）
//----------------------------------------------------------------------------

enum EnumSymName
{
    // 変更無し
    eesnDefault,
    eesnValue1=10,
    eesnName10,
    eesnName20,
    eesnName41,
    eesnName42,
    eesnName30,
    eesnName51,
    eesnName52,
    eesnDeleted,

    // 追加(前バージョンのeesnDefaultに対応させる)
    eesnName11  THEOLIZER_ANNOTATE(ES:::0)
};
THEOLIZER_ENUM(EnumSymName, 2);

enum class ScopedEnumSymName
{
    // 変更無し
    Default,
    Value1=10,
    Name10,
    // 変更無し
    Name20,
    Name41,
    Name42,
    Name30,
    Name51,
    Name52,
    Deleted,

    // 追加(前バージョンのDefaultに対応させる)
    Name11  THEOLIZER_ANNOTATE(ES:::0)
};
THEOLIZER_ENUM(ScopedEnumSymName, 2);

//----------------------------------------------------------------------------
//      非侵入型半自動（シンボル値対応）
//----------------------------------------------------------------------------

enum EnumSymVal
{
    // 変更無し
    eesvDefault,
    eesvName10,
    eesvValue10=10,
    eesvValue20=20,
    eesvValue41=41,
    eesvValue42=42,
    eesvValue30=30,
    eesvValue51=51,
    eesvValue52=52,
    eesvDeleted=99,

    // 追加(前バージョンのeesnDefaultに対応させる)
    eesvValue11 THEOLIZER_ANNOTATE(ES:::0)=70
};
THEOLIZER_ENUM_VALUE(EnumSymVal, 2);

enum class ScopedEnumSymVal
{
    // 変更無し
    Default=100,
    Name10 =101,
    Value10=110,
    Value20=200,
    Value41=410,
    Value42=420,
    Value30=300,
    Value51=510,
    Value52=520,
    Deleted=999,

    // 追加(前バージョンのeesnDefaultに対応させる)
    Value11 THEOLIZER_ANNOTATE(ES:::0)=700
};
THEOLIZER_ENUM_VALUE(ScopedEnumSymVal, 2);

#endif  // TEST_MODIFY_ENUM_H
