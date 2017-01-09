//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          enum型の様々なバリエーション
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

#if !defined(TEST_ENUM_VARIATION_H)
#define TEST_ENUM_VARIATION_H

#include <string>

// 基本のenum型とclassを使用する
#include "test_basic_process.h"

// ***************************************************************************
//      使い方の説明
// ***************************************************************************

//----------------------------------------------------------------------------
//      非侵入型半自動（シンボル名対応）
//----------------------------------------------------------------------------

enum EnumSymName
{
    eesnDef=1000,
    eesnVal
};
THEOLIZER_ENUM(EnumSymName, 1);

enum class ScopedEnumSymName
{
    Def=2000,
    Val
};
THEOLIZER_ENUM(ScopedEnumSymName, 1);

//----------------------------------------------------------------------------
//      非侵入型半自動（シンボル値対応）
//----------------------------------------------------------------------------

enum EnumSymVal
{
    eesvDef=3000,
    eesvVal
};
THEOLIZER_ENUM_VALUE(EnumSymVal, 1);

enum class ScopedEnumSymVal
{
    Def=4000,
    Val
};
THEOLIZER_ENUM_VALUE(ScopedEnumSymVal, 1);

// ***************************************************************************
//      自動テスト
// ***************************************************************************

//----------------------------------------------------------------------------
//      基底型のバリエーション
//----------------------------------------------------------------------------

#define DEFINE_ENUM()                                                       \
    /* デフォルト */                                                        \
    DEFINE(Default,                        )                                \
    /* 文字型 */                                                            \
    DEFINE(Char,       : char              )                                \
    DEFINE(SChar,      : signed char       )                                \
    DEFINE(UChar,      : unsigned char     )                                \
    DEFINE(Wchar,      : wchar_t           )                                \
    DEFINE(Char16,     : char16_t          )                                \
    DEFINE(Char32,     : char32_t          )                                \
    /* 整数型 */                                                            \
    DEFINE(Short,      : short             )                                \
    DEFINE(UShort,     : unsigned short    )                                \
    DEFINE(Int,        : int               )                                \
    DEFINE(UInt,       : unsigned int      )                                \
    DEFINE(Long,       : long              )                                \
    DEFINE(ULong,      : unsigned long     )                                \
    DEFINE(LongLong,   : long long         )                                \
    DEFINE(ULongLong,  : unsigned long long)

//----------------------------------------------------------------------------
//      非侵入型完全自動定義
//----------------------------------------------------------------------------

//      ---<<< 通常型 >>>---

#define DEFINE(dName, dType)                                                \
    enum Enum##dName dType                                                  \
    {                                                                       \
        Enum##dName##Def=10,    /* デフォルト値 */                          \
        Enum##dName##Val        /* 設定値 */                                \
    };
DEFINE_ENUM()
#undef  DEFINE

//      ---<<< scoped enum型 >>>---

#define DEFINE(dName, dType)                                                \
    enum class ScopedEnum##dName dType                                      \
    {                                                                       \
        Def=20,             /* デフォルト値 */                              \
        Val                 /* 設定値 */                                    \
    };
DEFINE_ENUM()
#undef  DEFINE

//      ---<<< 処理関数 >>>---
//      手動(トップ・レベル)による保存／回復

// 保存（左辺値）
template<class tSerializer>
inline void saveLValueEnumFullAuto(tSerializer& iSerializer)
{
    #define DEFINE(dName, dType)                                            \
        Enum##dName aEnum##dName=Enum##dName##Val;                          \
        THEOLIZER_PROCESS(iSerializer, aEnum##dName);
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        ScopedEnum##dName aScopedEnum##dName=ScopedEnum##dName::Val;        \
        THEOLIZER_PROCESS(iSerializer, aScopedEnum##dName);
    DEFINE_ENUM()
    #undef  DEFINE
}

// 保存（右辺値）
template<class tSerializer>
inline void saveRValueEnumFullAuto(tSerializer& iSerializer)
{
    #define DEFINE(dName, dType)                                            \
        THEOLIZER_PROCESS(iSerializer, Enum##dName##Val);
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        THEOLIZER_PROCESS(iSerializer, ScopedEnum##dName::Val);
    DEFINE_ENUM()
    #undef  DEFINE
}

// 回復
template<class tSerializer>
inline void loadEnumFullAuto(tSerializer& iSerializer)
{
    #define DEFINE(dName, dType)                                            \
        Enum##dName aEnum##dName=Enum##dName##Def;                          \
        THEOLIZER_PROCESS(iSerializer, aEnum##dName);                       \
        THEOLIZER_EQUAL(aEnum##dName, Enum##dName##Val);
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        ScopedEnum##dName aScopedEnum##dName=ScopedEnum##dName::Def;        \
        THEOLIZER_PROCESS(iSerializer, aScopedEnum##dName);                 \
        THEOLIZER_EQUAL(aScopedEnum##dName, ScopedEnum##dName::Val);
    DEFINE_ENUM()
    #undef  DEFINE
}

//      ---<<< 自動処理の検証 >>>---
//      自動による保存／回復

struct FullAutoClass4EnumFullAuto
{
    #define DEFINE(dName, dType)                                            \
        Enum##dName mEnum##dName;
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        ScopedEnum##dName mScopedEnum##dName;
    DEFINE_ENUM()
    #undef  DEFINE

    int     mDummy;

    // デフォルト・コンストラクタ(Def値で初期化)
    FullAutoClass4EnumFullAuto() :
        #define DEFINE(dName, dType)                                        \
            mEnum##dName(Enum##dName##Def),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnum##dName(ScopedEnum##dName::Def),
        DEFINE_ENUM()
        #undef  DEFINE
        mDummy(0)
    { }

    // 初期値設定コンストラクタ(Val値で初期化)
    FullAutoClass4EnumFullAuto(bool) :
        #define DEFINE(dName, dType)                                        \
            mEnum##dName(Enum##dName##Val),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnum##dName(ScopedEnum##dName::Val),
        DEFINE_ENUM()
        #undef  DEFINE
        mDummy(0)
    { }

    // 結果チェック
    void check()
    {
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_EQUAL(mEnum##dName, Enum##dName##Val);
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_EQUAL(mScopedEnum##dName, ScopedEnum##dName::Val);
        DEFINE_ENUM()
        #undef  DEFINE
    }
};

//      ---<<< 手動処理の検証 >>>---
//      手動(非トップ・レベル)による保存／回復

struct ManualClass4EnumFullAuto
{
    #define DEFINE(dName, dType)                                            \
        Enum##dName mEnum##dName;
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        ScopedEnum##dName mScopedEnum##dName;
    DEFINE_ENUM()
    #undef  DEFINE

    int     mDummy;

    // デフォルト・コンストラクタ(Def値で初期化)
    ManualClass4EnumFullAuto() :
        #define DEFINE(dName, dType)                                        \
            mEnum##dName(Enum##dName##Def),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnum##dName(ScopedEnum##dName::Def),
        DEFINE_ENUM()
        #undef  DEFINE
        mDummy(0)
    { }

    // 初期値設定コンストラクタ(Val値で初期化)
    ManualClass4EnumFullAuto(bool) :
        #define DEFINE(dName, dType)                                        \
            mEnum##dName(Enum##dName##Val),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnum##dName(ScopedEnum##dName::Val),
        DEFINE_ENUM()
        #undef  DEFINE
        mDummy(0)
    { }

    // 結果チェック
    void check()
    {
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_EQUAL(mEnum##dName, Enum##dName##Val);
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_EQUAL(mScopedEnum##dName, ScopedEnum##dName::Val);
        DEFINE_ENUM()
        #undef  DEFINE
    }
};

THEOLIZER_NON_INTRUSIVE_ORDER((ManualClass4EnumFullAuto), 1);

template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<ManualClass4EnumFullAuto>::
    TheolizerUserDefine<tBaseSerializer, tTheolizerVersion, 1>
{
    // Save members.
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_PROCESS(iSerializer, iInstance->mEnum##dName);        \
            THEOLIZER_PROCESS(iSerializer, Enum##dName##Val);
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_PROCESS(iSerializer, iInstance->mScopedEnum##dName);  \
            THEOLIZER_PROCESS(iSerializer, ScopedEnum##dName::Val);
        DEFINE_ENUM()
        #undef  DEFINE
    }

    // Load members.
    static void loadClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        if (!oInstance) oInstance=new typename tTheolizerVersion::TheolizerTarget();
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_PROCESS(iSerializer, oInstance->mEnum##dName);        \
            Enum##dName aEnum##dName;                                       \
            THEOLIZER_PROCESS(iSerializer, aEnum##dName);                   \
            THEOLIZER_EQUAL(aEnum##dName, Enum##dName##Val);
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_PROCESS(iSerializer, oInstance->mScopedEnum##dName);  \
            ScopedEnum##dName aScopedEnum##dName;                           \
            THEOLIZER_PROCESS(iSerializer, aScopedEnum##dName);             \
            THEOLIZER_EQUAL(aScopedEnum##dName, ScopedEnum##dName::Val);
        DEFINE_ENUM()
        #undef  DEFINE
    }
};

//----------------------------------------------------------------------------
//      非侵入型半自動（シンボル名対応）
//----------------------------------------------------------------------------

//      ---<<< 通常型 >>>---

#define DEFINE(dName, dType)                                                \
    enum EnumSymName##dName dType                                           \
    {                                                                       \
        EnumSymName##dName##Def=30,     /* デフォルト値 */                  \
        EnumSymName##dName##Val         /* 設定値 */                        \
    };                                                                      \
    THEOLIZER_ENUM(EnumSymName##dName, 1);
DEFINE_ENUM()
#undef  DEFINE

//      ---<<< scoped enum型 >>>---

#define DEFINE(dName, dType)                                                \
    enum class ScopedEnumSymName##dName dType                               \
    {                                                                       \
        Def=40,     /* デフォルト値 */                                      \
        Val         /* 設定値 */                                            \
    };                                                                      \
    THEOLIZER_ENUM(ScopedEnumSymName##dName, 1);
DEFINE_ENUM()
#undef  DEFINE

//      ---<<< 処理関数 >>>---
//      手動(トップ・レベル)による保存／回復

// 保存（左辺値）
template<class tSerializer>
inline void saveLValueEnumSymName(tSerializer& iSerializer)
{
    #define DEFINE(dName, dType)                                            \
        EnumSymName##dName aEnum##dName=EnumSymName##dName##Val;            \
        THEOLIZER_PROCESS(iSerializer, aEnum##dName);
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        ScopedEnumSymName##dName aScopedEnum##dName=ScopedEnumSymName##dName::Val;\
        THEOLIZER_PROCESS(iSerializer, aScopedEnum##dName);
    DEFINE_ENUM()
    #undef  DEFINE
}

// 保存（右辺値）
template<class tSerializer>
inline void saveRValueEnumSymName(tSerializer& iSerializer)
{
    #define DEFINE(dName, dType)                                            \
        THEOLIZER_PROCESS(iSerializer, EnumSymName##dName##Val);
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        THEOLIZER_PROCESS(iSerializer, ScopedEnumSymName##dName::Val);
    DEFINE_ENUM()
    #undef  DEFINE
}

// 回復
template<class tSerializer>
inline void loadEnumSymName(tSerializer& iSerializer)
{
    #define DEFINE(dName, dType)                                            \
        EnumSymName##dName aEnumSymName##dName=EnumSymName##dName##Def;     \
        THEOLIZER_PROCESS(iSerializer, aEnumSymName##dName);                \
        THEOLIZER_EQUAL(aEnumSymName##dName, EnumSymName##dName##Val);
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        ScopedEnumSymName##dName aScopedEnumSymName##dName=ScopedEnumSymName##dName::Def;\
        THEOLIZER_PROCESS(iSerializer, aScopedEnumSymName##dName);          \
        THEOLIZER_EQUAL(aScopedEnumSymName##dName, ScopedEnumSymName##dName::Val);
    DEFINE_ENUM()
    #undef  DEFINE
}

//      ---<<< 自動処理の検証 >>>---
//      自動による保存／回復

struct FullAutoClass4EnumSymName
{
    #define DEFINE(dName, dType)                                            \
        EnumSymName##dName mEnumSymName##dName;
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        ScopedEnumSymName##dName mScopedEnumSymName##dName;
    DEFINE_ENUM()
    #undef  DEFINE

    int     mDummy;

    // デフォルト・コンストラクタ(Def値で初期化)
    FullAutoClass4EnumSymName() :
        #define DEFINE(dName, dType)                                        \
            mEnumSymName##dName(EnumSymName##dName##Def),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnumSymName##dName(ScopedEnumSymName##dName::Def),
        DEFINE_ENUM()
        #undef  DEFINE
        mDummy(0)
    { }

    // 初期値設定コンストラクタ(Val値で初期化)
    FullAutoClass4EnumSymName(bool) :
        #define DEFINE(dName, dType)                                        \
            mEnumSymName##dName(EnumSymName##dName##Val),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnumSymName##dName(ScopedEnumSymName##dName::Val),
        DEFINE_ENUM()
        #undef  DEFINE
        mDummy(0)
    { }

    // 結果チェック
    void check()
    {
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_EQUAL(mEnumSymName##dName, EnumSymName##dName##Val);
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_EQUAL(mScopedEnumSymName##dName, ScopedEnumSymName##dName::Val);
        DEFINE_ENUM()
        #undef  DEFINE
    }
};

//      ---<<< 手動処理の検証 >>>---
//      手動(非トップ・レベル)による保存／回復

struct ManualClass4EnumSymName
{
    #define DEFINE(dName, dType)                                            \
        EnumSymName##dName mEnumSymName##dName;
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        ScopedEnumSymName##dName mScopedEnumSymName##dName;
    DEFINE_ENUM()
    #undef  DEFINE

    int     mDummy;

    // デフォルト・コンストラクタ(Def値で初期化)
    ManualClass4EnumSymName() :
        #define DEFINE(dName, dType)                                        \
            mEnumSymName##dName(EnumSymName##dName##Def),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnumSymName##dName(ScopedEnumSymName##dName::Def),
        DEFINE_ENUM()
        #undef  DEFINE
        mDummy(0)
    { }

    // 初期値設定コンストラクタ(Val値で初期化)
    ManualClass4EnumSymName(bool) :
        #define DEFINE(dName, dType)                                        \
            mEnumSymName##dName(EnumSymName##dName##Val),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnumSymName##dName(ScopedEnumSymName##dName::Val),
        DEFINE_ENUM()
        #undef  DEFINE
        mDummy(0)
    { }

    // 結果チェック
    void check()
    {
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_EQUAL(mEnumSymName##dName, EnumSymName##dName##Val);
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_EQUAL(mScopedEnumSymName##dName, ScopedEnumSymName##dName::Val);
        DEFINE_ENUM()
        #undef  DEFINE
    }
};

THEOLIZER_NON_INTRUSIVE_ORDER((ManualClass4EnumSymName), 1);

template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<ManualClass4EnumSymName>::
    TheolizerUserDefine<tBaseSerializer, tTheolizerVersion, 1>
{
    // Save members.
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_PROCESS(iSerializer, iInstance->mEnumSymName##dName); \
            THEOLIZER_PROCESS(iSerializer, EnumSymName##dName##Val);
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_PROCESS(iSerializer, iInstance->mScopedEnumSymName##dName);\
            THEOLIZER_PROCESS(iSerializer, ScopedEnumSymName##dName::Val);
        DEFINE_ENUM()
        #undef  DEFINE
    }

    // Load members.
    static void loadClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        if (!oInstance) oInstance=new typename tTheolizerVersion::TheolizerTarget();
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_PROCESS(iSerializer, oInstance->mEnumSymName##dName); \
            EnumSymName##dName aEnumSymName##dName;                         \
            THEOLIZER_PROCESS(iSerializer, aEnumSymName##dName);            \
            THEOLIZER_EQUAL(aEnumSymName##dName, EnumSymName##dName##Val);
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_PROCESS(iSerializer, oInstance->mScopedEnumSymName##dName);\
            ScopedEnumSymName##dName aScopedEnumSymName##dName;             \
            THEOLIZER_PROCESS(iSerializer, aScopedEnumSymName##dName);      \
            THEOLIZER_EQUAL(aScopedEnumSymName##dName, ScopedEnumSymName##dName::Val);
        DEFINE_ENUM()
        #undef  DEFINE
    }
};

//----------------------------------------------------------------------------
//      非侵入型半自動（シンボル値対応）
//----------------------------------------------------------------------------

//      ---<<< 通常型 >>>---

#define DEFINE(dName, dType)                                                \
    enum EnumSymVal##dName dType                                            \
    {                                                                       \
        EnumSymVal##dName##Def=50,  /* デフォルト値 */                      \
        EnumSymVal##dName##Val      /* 設定値 */                            \
    };                                                                      \
    THEOLIZER_ENUM_VALUE(EnumSymVal##dName, 1);
DEFINE_ENUM()
#undef  DEFINE

//      ---<<< scoped enum型 >>>---

#define DEFINE(dName, dType)                                                \
    enum class ScopedEnumSymVal##dName dType                                \
    {                                                                       \
        Def=60,  /* デフォルト値 */                                         \
        Val      /* 設定値 */                                               \
    };                                                                      \
    THEOLIZER_ENUM_VALUE(ScopedEnumSymVal##dName, 1);
DEFINE_ENUM()
#undef  DEFINE

//      ---<<< 処理関数 >>>---
//      手動(トップ・レベル)による保存／回復

// 保存（左辺値）
template<class tSerializer>
inline void saveLValueEnumSymVal(tSerializer& iSerializer)
{
    #define DEFINE(dName, dType)                                            \
        EnumSymVal##dName aEnumSymVal##dName=EnumSymVal##dName##Val;        \
        THEOLIZER_PROCESS(iSerializer, aEnumSymVal##dName);
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        ScopedEnum##dName aScopedEnum##dName=ScopedEnum##dName::Val;        \
        THEOLIZER_PROCESS(iSerializer, aScopedEnum##dName);
    DEFINE_ENUM()
    #undef  DEFINE
}

// 保存（右辺値）
template<class tSerializer>
inline void saveRValueEnumSymVal(tSerializer& iSerializer)
{
    #define DEFINE(dName, dType)                                            \
        THEOLIZER_PROCESS(iSerializer, EnumSymVal##dName##Val);
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        THEOLIZER_PROCESS(iSerializer, ScopedEnum##dName::Val);
    DEFINE_ENUM()
    #undef  DEFINE
}

// 回復
template<class tSerializer>
inline void loadEnumSymVal(tSerializer& iSerializer)
{
    #define DEFINE(dName, dType)                                            \
        EnumSymVal##dName aEnumSymVal##dName=EnumSymVal##dName##Def;  \
        THEOLIZER_PROCESS(iSerializer, aEnumSymVal##dName);                 \
        THEOLIZER_EQUAL(aEnumSymVal##dName, EnumSymVal##dName##Val);
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        ScopedEnum##dName aScopedEnum##dName=ScopedEnum##dName::Def;        \
        THEOLIZER_PROCESS(iSerializer, aScopedEnum##dName);                 \
        THEOLIZER_EQUAL(aScopedEnum##dName, ScopedEnum##dName::Val);
    DEFINE_ENUM()
    #undef  DEFINE
}

//      ---<<< 自動処理の検証 >>>---
//      自動による保存／回復

struct FullAutoClass4EnumSymVal
{
    #define DEFINE(dName, dType)                                            \
        EnumSymVal##dName mEnumSymVal##dName;
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        ScopedEnum##dName mScopedEnum##dName;
    DEFINE_ENUM()
    #undef  DEFINE

    int     mDummy;

    // デフォルト・コンストラクタ(Def値で初期化)
    FullAutoClass4EnumSymVal() :
        #define DEFINE(dName, dType)                                        \
            mEnumSymVal##dName(EnumSymVal##dName##Def),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnum##dName(ScopedEnum##dName::Def),
        DEFINE_ENUM()
        #undef  DEFINE
        mDummy(0)
    { }

    // 初期値設定コンストラクタ(Val値で初期化)
    FullAutoClass4EnumSymVal(bool) :
        #define DEFINE(dName, dType)                                        \
            mEnumSymVal##dName(EnumSymVal##dName##Val),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnum##dName(ScopedEnum##dName::Val),
        DEFINE_ENUM()
        #undef  DEFINE
        mDummy(0)
    { }

    // 結果チェック
    void check()
    {
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_EQUAL(mEnumSymVal##dName, EnumSymVal##dName##Val);
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_EQUAL(mScopedEnum##dName, ScopedEnum##dName::Val);
        DEFINE_ENUM()
        #undef  DEFINE
    }
};

//      ---<<< 手動処理の検証 >>>---
//      手動(非トップ・レベル)による保存／回復

struct ManualClass4EnumSymVal
{
    #define DEFINE(dName, dType)                                            \
        EnumSymVal##dName mEnumSymVal##dName;
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        ScopedEnum##dName mScopedEnum##dName;
    DEFINE_ENUM()
    #undef  DEFINE

    int     mDummy;

    // デフォルト・コンストラクタ(Def値で初期化)
    ManualClass4EnumSymVal() :
        #define DEFINE(dName, dType)                                        \
            mEnumSymVal##dName(EnumSymVal##dName##Def),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnum##dName(ScopedEnum##dName::Def),
        DEFINE_ENUM()
        #undef  DEFINE
        mDummy(0)
    { }

    // 初期値設定コンストラクタ(Val値で初期化)
    ManualClass4EnumSymVal(bool) :
        #define DEFINE(dName, dType)                                        \
            mEnumSymVal##dName(EnumSymVal##dName##Val),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnum##dName(ScopedEnum##dName::Val),
        DEFINE_ENUM()
        #undef  DEFINE
        mDummy(0)
    { }

    // 結果チェック
    void check()
    {
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_EQUAL(mEnumSymVal##dName, EnumSymVal##dName##Val);
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_EQUAL(mScopedEnum##dName, ScopedEnum##dName::Val);
        DEFINE_ENUM()
        #undef  DEFINE
    }
};

THEOLIZER_NON_INTRUSIVE_ORDER((ManualClass4EnumSymVal), 1);

template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<ManualClass4EnumSymVal>::
    TheolizerUserDefine<tBaseSerializer, tTheolizerVersion, 1>
{
    // Save members.
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_PROCESS(iSerializer, iInstance->mEnumSymVal##dName);  \
            THEOLIZER_PROCESS(iSerializer, EnumSymVal##dName##Val);
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_PROCESS(iSerializer, iInstance->mScopedEnum##dName);  \
            THEOLIZER_PROCESS(iSerializer, ScopedEnum##dName::Val);
        DEFINE_ENUM()
        #undef  DEFINE
    }

    // Load members.
    static void loadClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        if (!oInstance) oInstance=new typename tTheolizerVersion::TheolizerTarget();
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_PROCESS(iSerializer, oInstance->mEnumSymVal##dName);  \
            EnumSymVal##dName aEnumSymVal##dName;                           \
            THEOLIZER_PROCESS(iSerializer, aEnumSymVal##dName);             \
            THEOLIZER_EQUAL(aEnumSymVal##dName, EnumSymVal##dName##Val);
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_PROCESS(iSerializer, oInstance->mScopedEnum##dName);  \
            ScopedEnum##dName aScopedEnum##dName;                           \
            THEOLIZER_PROCESS(iSerializer, aScopedEnum##dName);             \
            THEOLIZER_EQUAL(aScopedEnum##dName, ScopedEnum##dName::Val);
        DEFINE_ENUM()
        #undef  DEFINE
    }
};

#endif  // TEST_ENUM_VARIATION_H
