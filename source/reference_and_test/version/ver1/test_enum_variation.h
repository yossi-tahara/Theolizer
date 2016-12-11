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

#ifdef SHRINK_FOR_MINGW64           // MinGWx64におけるFile too big回避のため
#endif

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
        Enum##dName##Def,       /* デフォルト値 */                          \
        Enum##dName##Val        /* 設定値 */                                \
    };
DEFINE_ENUM()
#undef  DEFINE

//      ---<<< scoped enum型 >>>---

#define DEFINE(dName, dType)                                                \
    enum class ScopedEnum##dName dType                                      \
    {                                                                       \
        dName##Def,     /* デフォルト値 */                                  \
        dName##Val      /* 設定値 */                                        \
    };
DEFINE_ENUM()
#undef  DEFINE

//      ---<<< 処理関数 >>>---
//      手動(トップ・レベル)による保存／回復

// 保存
template<class tSerializer>
inline void saveEnumFullAuto(tSerializer& iSerializer)
{
    #define DEFINE(dName, dType)                                            \
        Enum##dName aEnum##dName=Enum##dName##Val;                          \
        THEOLIZER_PROCESS(iSerializer, aEnum##dName);
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        ScopedEnum##dName aScopedEnum##dName=ScopedEnum##dName::dName##Val; \
        THEOLIZER_PROCESS(iSerializer, aScopedEnum##dName);
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
        ScopedEnum##dName aScopedEnum##dName=ScopedEnum##dName::dName##Def; \
        THEOLIZER_PROCESS(iSerializer, aScopedEnum##dName);                 \
        THEOLIZER_EQUAL(aScopedEnum##dName, ScopedEnum##dName::dName##Val);
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
            mScopedEnum##dName(ScopedEnum##dName::dName##Def),
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
            mScopedEnum##dName(ScopedEnum##dName::dName##Val),
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
            THEOLIZER_EQUAL(mScopedEnum##dName, ScopedEnum##dName::dName##Val);
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
            mScopedEnum##dName(ScopedEnum##dName::dName##Def),
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
            mScopedEnum##dName(ScopedEnum##dName::dName##Val),
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
            THEOLIZER_EQUAL(mScopedEnum##dName, ScopedEnum##dName::dName##Val);
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
            THEOLIZER_PROCESS(iSerializer, iInstance->mEnum##dName);
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_PROCESS(iSerializer, iInstance->mScopedEnum##dName);
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
            THEOLIZER_PROCESS(iSerializer, oInstance->mEnum##dName);
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_PROCESS(iSerializer, oInstance->mScopedEnum##dName);
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
        EnumSymName##dName##Def,        /* デフォルト値 */                  \
        EnumSymName##dName##Val     /* 設定値 */                            \
    };                                                                      \
    THEOLIZER_ENUM(EnumSymName##dName, 1);
DEFINE_ENUM()
#undef  DEFINE

//      ---<<< scoped enum型 >>>---

#define DEFINE(dName, dType)                                                \
    enum class ScopedEnumSymName##dName dType                               \
    {                                                                       \
        dName##Def,     /* デフォルト値 */                                  \
        dName##Val      /* 設定値 */                                        \
    };                                                                      \
    THEOLIZER_ENUM(ScopedEnumSymName##dName, 1);
DEFINE_ENUM()
#undef  DEFINE

//      ---<<< 処理関数 >>>---
//      手動(トップ・レベル)による保存／回復

// 保存
template<class tSerializer>
inline void saveEnumSymName(tSerializer& iSerializer)
{
    #define DEFINE(dName, dType)                                            \
        Enum##dName aEnum##dName=Enum##dName##Val;                          \
        THEOLIZER_PROCESS(iSerializer, aEnum##dName);
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        ScopedEnum##dName aScopedEnum##dName=ScopedEnum##dName::dName##Val; \
        THEOLIZER_PROCESS(iSerializer, aScopedEnum##dName);
    DEFINE_ENUM()
    #undef  DEFINE
}

// 回復
template<class tSerializer>
inline void loadEnumSymName(tSerializer& iSerializer)
{
    #define DEFINE(dName, dType)                                            \
        Enum##dName aEnum##dName=Enum##dName##Def;                          \
        THEOLIZER_PROCESS(iSerializer, aEnum##dName);                       \
        THEOLIZER_EQUAL(aEnum##dName, Enum##dName##Val);
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        ScopedEnum##dName aScopedEnum##dName=ScopedEnum##dName::dName##Def; \
        THEOLIZER_PROCESS(iSerializer, aScopedEnum##dName);                 \
        THEOLIZER_EQUAL(aScopedEnum##dName, ScopedEnum##dName::dName##Val);
    DEFINE_ENUM()
    #undef  DEFINE
}

//      ---<<< 自動処理の検証 >>>---
//      自動による保存／回復

struct FullAutoClass4EnumSymName
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
    FullAutoClass4EnumSymName() :
        #define DEFINE(dName, dType)                                        \
            mEnum##dName(Enum##dName##Def),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnum##dName(ScopedEnum##dName::dName##Def),
        DEFINE_ENUM()
        #undef  DEFINE
        mDummy(0)
    { }

    // 初期値設定コンストラクタ(Val値で初期化)
    FullAutoClass4EnumSymName(bool) :
        #define DEFINE(dName, dType)                                        \
            mEnum##dName(Enum##dName##Val),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnum##dName(ScopedEnum##dName::dName##Val),
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
            THEOLIZER_EQUAL(mScopedEnum##dName, ScopedEnum##dName::dName##Val);
        DEFINE_ENUM()
        #undef  DEFINE
    }
};

//      ---<<< 手動処理の検証 >>>---
//      手動(非トップ・レベル)による保存／回復

struct ManualClass4EnumSymName
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
    ManualClass4EnumSymName() :
        #define DEFINE(dName, dType)                                        \
            mEnum##dName(Enum##dName##Def),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnum##dName(ScopedEnum##dName::dName##Def),
        DEFINE_ENUM()
        #undef  DEFINE
        mDummy(0)
    { }

    // 初期値設定コンストラクタ(Val値で初期化)
    ManualClass4EnumSymName(bool) :
        #define DEFINE(dName, dType)                                        \
            mEnum##dName(Enum##dName##Val),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnum##dName(ScopedEnum##dName::dName##Val),
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
            THEOLIZER_EQUAL(mScopedEnum##dName, ScopedEnum##dName::dName##Val);
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
            THEOLIZER_PROCESS(iSerializer, iInstance->mEnum##dName);
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_PROCESS(iSerializer, iInstance->mScopedEnum##dName);
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
            THEOLIZER_PROCESS(iSerializer, oInstance->mEnum##dName);
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_PROCESS(iSerializer, oInstance->mScopedEnum##dName);
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
        EnumSymVal##dName##Def,     /* デフォルト値 */                      \
        EnumSymVal##dName##Val      /* 設定値 */                            \
    };                                                                      \
    THEOLIZER_ENUM_VALUE(EnumSymVal##dName, 1);
DEFINE_ENUM()
#undef  DEFINE

//      ---<<< scoped enum型 >>>---

#define DEFINE(dName, dType)                                                \
    enum class ScopedEnumSymVal##dName dType                                \
    {                                                                       \
        dName##Def,     /* デフォルト値 */                                  \
        dName##Val      /* 設定値 */                                        \
    };                                                                      \
    THEOLIZER_ENUM_VALUE(ScopedEnumSymVal##dName, 1);
DEFINE_ENUM()
#undef  DEFINE

//      ---<<< 処理関数 >>>---
//      手動(トップ・レベル)による保存／回復

// 保存
template<class tSerializer>
inline void saveEnumSymVal(tSerializer& iSerializer)
{
    #define DEFINE(dName, dType)                                            \
        Enum##dName aEnum##dName=Enum##dName##Val;                          \
        THEOLIZER_PROCESS(iSerializer, aEnum##dName);
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        ScopedEnum##dName aScopedEnum##dName=ScopedEnum##dName::dName##Val; \
        THEOLIZER_PROCESS(iSerializer, aScopedEnum##dName);
    DEFINE_ENUM()
    #undef  DEFINE
}

// 回復
template<class tSerializer>
inline void loadEnumSymVal(tSerializer& iSerializer)
{
    #define DEFINE(dName, dType)                                            \
        Enum##dName aEnum##dName=Enum##dName##Def;                          \
        THEOLIZER_PROCESS(iSerializer, aEnum##dName);                       \
        THEOLIZER_EQUAL(aEnum##dName, Enum##dName##Val);
    DEFINE_ENUM()
    #undef  DEFINE
    #define DEFINE(dName, dType)                                            \
        ScopedEnum##dName aScopedEnum##dName=ScopedEnum##dName::dName##Def; \
        THEOLIZER_PROCESS(iSerializer, aScopedEnum##dName);                 \
        THEOLIZER_EQUAL(aScopedEnum##dName, ScopedEnum##dName::dName##Val);
    DEFINE_ENUM()
    #undef  DEFINE
}

//      ---<<< 自動処理の検証 >>>---
//      自動による保存／回復

struct FullAutoClass4EnumSymVal
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
    FullAutoClass4EnumSymVal() :
        #define DEFINE(dName, dType)                                        \
            mEnum##dName(Enum##dName##Def),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnum##dName(ScopedEnum##dName::dName##Def),
        DEFINE_ENUM()
        #undef  DEFINE
        mDummy(0)
    { }

    // 初期値設定コンストラクタ(Val値で初期化)
    FullAutoClass4EnumSymVal(bool) :
        #define DEFINE(dName, dType)                                        \
            mEnum##dName(Enum##dName##Val),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnum##dName(ScopedEnum##dName::dName##Val),
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
            THEOLIZER_EQUAL(mScopedEnum##dName, ScopedEnum##dName::dName##Val);
        DEFINE_ENUM()
        #undef  DEFINE
    }
};

//      ---<<< 手動処理の検証 >>>---
//      手動(非トップ・レベル)による保存／回復

struct ManualClass4EnumSymVal
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
    ManualClass4EnumSymVal() :
        #define DEFINE(dName, dType)                                        \
            mEnum##dName(Enum##dName##Def),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnum##dName(ScopedEnum##dName::dName##Def),
        DEFINE_ENUM()
        #undef  DEFINE
        mDummy(0)
    { }

    // 初期値設定コンストラクタ(Val値で初期化)
    ManualClass4EnumSymVal(bool) :
        #define DEFINE(dName, dType)                                        \
            mEnum##dName(Enum##dName##Val),
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            mScopedEnum##dName(ScopedEnum##dName::dName##Val),
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
            THEOLIZER_EQUAL(mScopedEnum##dName, ScopedEnum##dName::dName##Val);
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
            THEOLIZER_PROCESS(iSerializer, iInstance->mEnum##dName);
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_PROCESS(iSerializer, iInstance->mScopedEnum##dName);
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
            THEOLIZER_PROCESS(iSerializer, oInstance->mEnum##dName);
        DEFINE_ENUM()
        #undef  DEFINE
        #define DEFINE(dName, dType)                                        \
            THEOLIZER_PROCESS(iSerializer, oInstance->mScopedEnum##dName);
        DEFINE_ENUM()
        #undef  DEFINE
    }
};

#endif  // TEST_ENUM_VARIATION_H
