//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          classの様々なバリエーション
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

#if !defined(TEST_CLASS_VARIATION_H)
#define TEST_CLASS_VARIATION_H

#include <string>

// 基本のenum型とclassを使用する
#include "test_basic_process.h"

// ***************************************************************************
//      使い方の説明
// ***************************************************************************

//----------------------------------------------------------------------------
//      非侵入型完全自動
//----------------------------------------------------------------------------

class FullAutoTutorial
{
    int     mFullAutoPrivate;
protected:
    int     mFullAutoProtected;
public:
    int     mFullAutoPublic;

    FullAutoTutorial() :
        mFullAutoPrivate(0),
        mFullAutoProtected(0),
        mFullAutoPublic(0)
    { }
    void setNonPublic(int iFullAutoPrivate, int iFullAutoProtected);
    void checkNonPublic(int iFullAutoPrivate, int iFullAutoProtected);
};

//----------------------------------------------------------------------------
//      侵入型半自動（名前対応）
//----------------------------------------------------------------------------

class HalfAutoNameTutorial
{
    int     mHalfAutoNamePrivate;
protected:
    int     mHalfAutoNameProtected;
public:
    int     mHalfAutoNamePublic;

    HalfAutoNameTutorial() :
        mHalfAutoNamePrivate(0),
        mHalfAutoNameProtected(0),
        mHalfAutoNamePublic(0)
    { }
    void setNonPublic(int iHalfAutoNamePrivate, int iHalfAutoNameProtected);
    void checkNonPublic(int iHalfAutoNamePrivate, int iHalfAutoNameProtected);

    // 侵入型半自動 指定
    THEOLIZER_INTRUSIVE(CS, (HalfAutoNameTutorial), 1);
};

//----------------------------------------------------------------------------
//      侵入型半自動（順序対応）
//----------------------------------------------------------------------------

class HalfAutoOrderTutorial
{
    int     mHalfAutoOrderPrivate;
protected:
    int     mHalfAutoOrderProtected;
public:
    int     mHalfAutoOrderPublic;

    HalfAutoOrderTutorial() :
        mHalfAutoOrderPrivate(0),
        mHalfAutoOrderProtected(0),
        mHalfAutoOrderPublic(0)
    { }
    void setNonPublic(int iHalfAutoOrderPrivate, int iHalfAutoOrderProtected);
    void checkNonPublic(int iHalfAutoOrderPrivate, int iHalfAutoOrderProtected);

    // 侵入型半自動 指定
    THEOLIZER_INTRUSIVE_ORDER(CS, (HalfAutoOrderTutorial), 1);
};

//----------------------------------------------------------------------------
//      非侵入型手動
//----------------------------------------------------------------------------

class ManualTutorial
{
public:
    int     mManualPublic;

    ManualTutorial() :
        mManualPublic(0)
    { }
};

//      ---<<< シリアライズ用の手動記述 >>>---

// 非侵入型手動クラスの指定
THEOLIZER_NON_INTRUSIVE_ORDER((ManualTutorial), 1);

// 保存処理／回復処理関数
template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<ManualTutorial>::
    TheolizerUserDefine<tBaseSerializer, tTheolizerVersion, 1>
{
    // Save members.
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        THEOLIZER_PROCESS(iSerializer, iInstance->mManualPublic);
    }

    // Load members.
    static void loadClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        if (!oInstance) oInstance=new typename tTheolizerVersion::TheolizerTarget();
        
        THEOLIZER_PROCESS(iSerializer, oInstance->mManualPublic);
    }
};

//----------------------------------------------------------------------------
//      派生／包含クラス
//----------------------------------------------------------------------------

class DerivedClass : public FullAutoTutorial, private HalfAutoNameTutorial
{
    HalfAutoOrderTutorial   mHalfAutoOrderTutorial;
public:
    ManualTutorial          mManualTutorial;

    DerivedClass() :
        FullAutoTutorial(),
        HalfAutoNameTutorial(),
        mHalfAutoOrderTutorial(),
        mManualTutorial()
    { }
    void setNonPublic
    (
        int iHalfAutoNamePrivate,
        int iHalfAutoNameProtected,
        int iHalfAutoOrderPrivate,
        int iHalfAutoOrderProtected
    );
    void checkNonPublic
    (
        int iHalfAutoNamePrivate,
        int iHalfAutoNameProtected,
        int iHalfAutoOrderPrivate,
        int iHalfAutoOrderProtected
    );
};

// ***************************************************************************
//      単独テスト
// ***************************************************************************

//----------------------------------------------------------------------------
//      メンバ変数リスト
//          下記へ展開する
//              メンバ変数定義
//              メンバ変数初期化
//              メンバ変数の値チェック
//----------------------------------------------------------------------------

#ifdef ENABLE_SINGLE_TEST

#ifndef SHRINK_FOR_MINGW            // MinGWにおけるFile too big回避用

#define DEFINE_MEMBERS()                                                                \
    /* 文字型 */                                                                        \
    DEFINE(char,                mChar,          0,       110)                           \
    DEFINE(signed char,         mSChar,         0,      -111)                           \
    DEFINE(unsigned char,       mUChar,         0U,      112U)                          \
    DEFINE(wchar_t,             mWchar,         0,       1100)                          \
    DEFINE(char16_t,            mChar16,        0U,      1101U)                         \
    DEFINE(char32_t,            mChar32,        0U,      1102U)                         \
    /* 整数型 */                                                                        \
    DEFINE(bool,                mBool,          false,   true)                          \
    DEFINE(short,               mShort,         0,      -2100)                          \
    DEFINE(unsigned short,      mUShort,        0U,      2100U)                         \
    DEFINE(int,                 mInt,           0,      -3100)                          \
    DEFINE(unsigned int,        mUInt,          0U,      3100U)                         \
    DEFINE(long,                mLong,          0L,     -4100L)                         \
    DEFINE(unsigned long,       mULong,         0UL,     4100UL)                        \
    DEFINE(long long,           mLongLong,      0LL,    -5100LL)                        \
    DEFINE(unsigned long long,  mULongLong,     0ULL,    5100ULL)                       \
    /* 浮動小数点型 */                                                                  \
    DEFINE(float,               mFloat,         0.0F,   2.23456F)                       \
    DEFINE(double,              mDouble,        0.0,    2.23456789012345)               \
    DEFINE(long double,         mLongDouble,    0.0L,   2.23456789012345678L)           \
    /* 文字列型 */                                                                      \
    DEFINE(std::string,         mString,        u8"",   u8"ＵＴＦ－８a")                \
    DEFINE(std::wstring,        mWstring,       L"",    L"ＵＴＦ－１６／３２a")         \
    DEFINE(std::u16string,      mU16string,     u"",    u"ＵＴＦ－１６a")               \
    DEFINE(std::u32string,      mU32string,     U"",    U"ＵＴＦ－３２a")               \
    /* enum型 */                                                                        \
    DEFINE(NormalEnum,  mNormalEnum,    eneZero,            eneOne)                     \
    DEFINE(ScopedEnum,  mScopedEnum,    ScopedEnum::ZERO,   ScopedEnum::TWO)            \
    /* クラス */                                                                        \
    DEFINE(ClassBasicTest, mClassBasicTest,                                             \
           ClassBasicTest(), ClassBasicTest(1, "1", eneOne))                            \
    /* --- 配列 --- */                                                                  \
    /* 文字型 */                                                                        \
    ARRAY(char,                 mChar,      5,  0,       120)                           \
    ARRAY(signed char,          mSChar,     6,  0,      -121)                           \
    ARRAY(unsigned char,        mUChar,     7,  0U,      112U)                          \
    ARRAY(wchar_t,              mWchar,     8,  0,       1100)                          \
    ARRAY(char16_t,             mChar16,    9,  0U,      1101U)                         \
    ARRAY(char32_t,             mChar32,    10, 0U,      1102U)                         \
    /* 整数型 */                                                                        \
    ARRAY(bool,                 mBool,      4,  false,   true)                          \
    ARRAY(short,                mShort,     5,  0,      -2200)                          \
    ARRAY(unsigned short,       mUShort,    6,  0U,      2200U)                         \
    ARRAY(int,                  mInt,       7,  0,      -3200)                          \
    ARRAY(unsigned int,         mUInt,      8,  0U,      3200U)                         \
    ARRAY(long,                 mLong,      9,  0L,     -4200L)                         \
    ARRAY(unsigned long,        mULong,     10, 0UL,     4200UL)                        \
    ARRAY(long long,            mLongLong,  11, 0LL,    -5200LL)                        \
    ARRAY(unsigned long long,   mULongLong, 12, 0ULL,    5200ULL)                       \
    /* 浮動小数点型 */                                                                  \
    ARRAY(float,                mFloat,     5,  0.0F,   3.23456F)                       \
    ARRAY(double,               mDouble,    6,  0.0,    3.23456789012345)               \
    ARRAY(long double,          mLongDouble,7,  0.0L,   3.23456789012345678L)           \
    /* 文字列型 */                                                                      \
    ARRAY(std::string,          mString,    5,  u8"",   u8"ＵＴＦ－８a")                \
    ARRAY(std::wstring,         mWstring,   6,  L"",    L"ＵＴＦ－１６／３２a")         \
    ARRAY(std::u16string,       mU16string, 7,  u"",    u"ＵＴＦ－１６a")               \
    ARRAY(std::u32string,       mU32string, 8,  U"",    U"ＵＴＦ－３２a")               \
    /* enum型 */                                                                        \
    ARRAY(NormalEnum,       mNormalEnum,    5,  eneZero,            eneOne)             \
    ARRAY(ScopedEnum,       mScopedEnum,    6,  ScopedEnum::ZERO,   ScopedEnum::TWO)    \
    /* クラス */                                                                        \
    ARRAY(ClassBasicTest, mClassBasicTest,  5,                                          \
           ClassBasicTest(), ClassBasicTest(1, "1", eneOne))

#else

#define DEFINE_MEMBERS()                                                                \
    /* 文字型 */                                                                        \
    DEFINE(char,                mChar,          0,       110)                           \
    DEFINE(signed char,         mSChar,         0,      -111)                           \
    DEFINE(unsigned char,       mUChar,         0U,      112U)                          \
    /* 整数型 */                                                                        \
    DEFINE(short,               mShort,         0,      -2100)                          \
    DEFINE(unsigned short,      mUShort,        0U,      2100U)                         \
    DEFINE(int,                 mInt,           0,      -3100)                          \
    DEFINE(unsigned int,        mUInt,          0U,      3100U)                         \
    /* 浮動小数点型 */                                                                  \
    DEFINE(float,               mFloat,         0.0F,   2.23456F)                       \
    DEFINE(double,              mDouble,        0.0,    2.23456789012345)               \
    /* 文字列型 */                                                                      \
    DEFINE(std::string,         mString,        u8"",   u8"ＵＴＦ－８a")                \
    DEFINE(std::wstring,        mWstring,       L"",    L"ＵＴＦ－１６／３２a")         \
    /* enum型 */                                                                        \
    DEFINE(NormalEnum,  mNormalEnum,    eneZero,            eneOne)                     \
    /* クラス */                                                                        \
    DEFINE(ClassBasicTest, mClassBasicTest,                                             \
           ClassBasicTest(), ClassBasicTest(1, "1", eneOne))                            \
    /* --- 配列 --- */                                                                  \
    /* 文字型 */                                                                        \
    ARRAY(wchar_t,              mWchar,     8,  0,       1100)                          \
    ARRAY(char16_t,             mChar16,    9,  0U,      1101U)                         \
    ARRAY(char32_t,             mChar32,    10, 0U,      1102U)                         \
    /* 整数型 */                                                                        \
    ARRAY(long,                 mLong,      9,  0L,     -4200L)                         \
    ARRAY(unsigned long,        mULong,     10, 0UL,     4200UL)                        \
    ARRAY(long long,            mLongLong,  11, 0LL,    -5200LL)                        \
    ARRAY(unsigned long long,   mULongLong, 12, 0ULL,    5200ULL)                       \
    /* 浮動小数点型 */                                                                  \
    ARRAY(long double,          mLongDouble,7,  0.0L,   3.23456789012345678L)           \
    /* 文字列型 */                                                                      \
    ARRAY(std::u16string,       mU16string, 7,  u"",    u"ＵＴＦ－１６a")               \
    ARRAY(std::u32string,       mU32string, 8,  U"",    U"ＵＴＦ－３２a")               \
    /* enum型 */                                                                        \
    ARRAY(ScopedEnum,       mScopedEnum,    6,  ScopedEnum::ZERO,   ScopedEnum::TWO)    \
    /* クラス */                                                                        \
    ARRAY(ClassBasicTest, mClassBasicTest,  5,                                          \
           ClassBasicTest(), ClassBasicTest(1, "1", eneOne))

#endif

//----------------------------------------------------------------------------
//      非侵入型完全自動
//----------------------------------------------------------------------------

//      ---<<< 配列のみで参照される完全自動型 >>>---
//      専用処理が必要なのでテストを設ける

struct ArrayOnly0
{
    int     mInt;
    ArrayOnly0() : mInt(0) { }
};

//      ---<<< 本体 >>>---

class FullAuto
{
    // privateメンバ定義
    #define DEFINE(dType, dVar, dDef, dVal)     dType dVar##Private;
    #define ARRAY(dType, dVar, dNum, dDef, dVal)    \
        dType dVar##Array1Private[dNum];            \
        dType dVar##Array2Private[2][dNum];         \
        dType dVar##Array3Private[3][2][dNum];
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE

protected:
    // protectedメンバ定義
    #define DEFINE(dType, dVar, dDef, dVal) dType dVar##Protected;
    #define ARRAY(dType, dVar, dNum, dDef, dVal)    \
        dType dVar##Array1Protected[dNum];          \
        dType dVar##Array2Protected[2][dNum];       \
        dType dVar##Array3Protected[3][2][dNum];
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE

public:
    // publicメンバ定義
    #define DEFINE(dType, dVar, dDef, dVal) dType dVar;
    #define ARRAY(dType, dVar, dNum, dDef, dVal)    \
        dType dVar##Array1[dNum];                   \
        dType dVar##Array2[2][dNum];                \
        dType dVar##Array3[3][2][dNum];
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE

    // クラス配列
    static const std::size_t    kRow=2;
    static const std::size_t    kCol=2;
    static const int            kValue=123;
    ArrayOnly0  mArrayOnly[kRow][kCol];

    // デフォルト・コンストラクタ
    FullAuto() :
        #define DEFINE(dType, dVar, dDef, dVal) dVar##Private(dDef),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1Private{},                  \
            dVar##Array2Private{},                  \
            dVar##Array3Private{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        #define DEFINE(dType, dVar, dDef, dVal) dVar##Protected(dDef),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1Protected{},                \
            dVar##Array2Protected{},                \
            dVar##Array3Protected{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        #define DEFINE(dType, dVar, dDef, dVal) dVar(dDef),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1{},                         \
            dVar##Array2{},                         \
            dVar##Array3{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mArrayOnly{}
    { }

    // 保存前の値設定
    FullAuto(bool) :
        #define DEFINE(dType, dVar, dDef, dVal) dVar##Private(dVal),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1Private{},                  \
            dVar##Array2Private{},                  \
            dVar##Array3Private{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        #define DEFINE(dType, dVar, dDef, dVal) dVar##Protected(dVal),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1Protected{},                \
            dVar##Array2Protected{},                \
            dVar##Array3Protected{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        #define DEFINE(dType, dVar, dDef, dVal) dVar(dVal),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1{},                         \
            dVar##Array2{},                         \
            dVar##Array3{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mArrayOnly{}
    {
        #define DEFINE(dType, dVar, dDef, dVal)
        #define ARRAY(dType, dVar, dNum, dDef, dVal)        \
            for (std::size_t i=0; i < dNum; ++i)            \
            {                                               \
                dVar##Array1Private[i]=dVal;                \
                dVar##Array1Protected[i]=dVal;              \
                dVar##Array1[i]=dVal;                       \
                for (std::size_t j=0; j < 2; ++j)           \
                {                                           \
                    dVar##Array2Private[j][i]=dVal;         \
                    dVar##Array2Protected[j][i]=dVal;       \
                    dVar##Array2[j][i]=dVal;                \
                    for (std::size_t k=0; k < 3; ++k)       \
                    {                                       \
                        dVar##Array3Private[k][j][i]=dVal;  \
                        dVar##Array3Protected[k][j][i]=dVal;\
                        dVar##Array3[k][j][i]=dVal;         \
                    }                                       \
                }                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE

        for (std::size_t i=0; i < kRow; ++i)
        {
            for (std::size_t j=0; j < kCol; ++j)
            {
                mArrayOnly[i][j].mInt=kValue;
            }
        }
    }

    // privateメンバの値をチェック
    void checkPrivate(bool iIsValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar##Private, ((!iIsValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1Private[i], ((!iIsValued)?dDef:dVal), i);\
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2Private[j][i], ((!iIsValued)?dDef:dVal), j, i);\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3Private[k][j][i], ((!iIsValued)?dDef:dVal),\
                                        k, j, i);                           \
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // protectedメンバの値をチェック
    void checkProtected(bool iIsValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar##Protected, ((!iIsValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1Protected[i], ((!iIsValued)?dDef:dVal), i);\
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2Protected[j][i], ((!iIsValued)?dDef:dVal), j, i);\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3Protected[k][j][i], ((!iIsValued)?dDef:dVal),\
                                        k, j, i);                           \
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // publicメンバの値をチェック
    void checkPublic(bool iIsValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar, ((!iIsValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1[i], ((!iIsValued)?dDef:dVal), i);\
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2[j][i], ((!iIsValued)?dDef:dVal), j, i);\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3[k][j][i], ((!iIsValued)?dDef:dVal),\
                                        k, j, i);                           \
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE

        for (std::size_t i=0; i < kRow; ++i)
        {
            for (std::size_t j=0; j < kCol; ++j)
            {
                THEOLIZER_EQUAL(mArrayOnly[i][j].mInt, ((!iIsValued)?0:kValue));
            }
        }
    }
};

//----------------------------------------------------------------------------
//      侵入型半自動－名前対応
//----------------------------------------------------------------------------

//      ---<<< 配列のみで参照される完全自動型 >>>---
//      ドライバがソース自動生成しない不具合が発生したのでその再発防止用

struct ArrayOnly1
{
    int     mInt;
    ArrayOnly1() : mInt(0) { }
};

//      ---<<< 本体 >>>---

class HalfAutoName
{
    // privateメンバ定義
    #define DEFINE(dType, dVar, dDef, dVal)     dType dVar##Private;
    #define ARRAY(dType, dVar, dNum, dDef, dVal)    \
        dType dVar##Array1Private[dNum];            \
        dType dVar##Array2Private[2][dNum];         \
        dType dVar##Array3Private[3][2][dNum];
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE

protected:
    // protectedメンバ定義
    #define DEFINE(dType, dVar, dDef, dVal) dType dVar##Protected;
    #define ARRAY(dType, dVar, dNum, dDef, dVal)    \
        dType dVar##Array1Protected[dNum];          \
        dType dVar##Array2Protected[2][dNum];       \
        dType dVar##Array3Protected[3][2][dNum];
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE

public:
    // publicメンバ定義
    #define DEFINE(dType, dVar, dDef, dVal) dType dVar;
    #define ARRAY(dType, dVar, dNum, dDef, dVal)    \
        dType dVar##Array1[dNum];                   \
        dType dVar##Array2[2][dNum];                \
        dType dVar##Array3[3][2][dNum];
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE

    // クラス配列
    static const std::size_t    kRow=2;
    static const std::size_t    kCol=3;
    static const int            kValue=456;
    ArrayOnly1  mArrayOnly[kRow][kCol];

    // デフォルト・コンストラクタ
    HalfAutoName() :
        #define DEFINE(dType, dVar, dDef, dVal) dVar##Private(dDef),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1Private{},                  \
            dVar##Array2Private{},                  \
            dVar##Array3Private{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        #define DEFINE(dType, dVar, dDef, dVal) dVar##Protected(dDef),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1Protected{},                \
            dVar##Array2Protected{},                \
            dVar##Array3Protected{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        #define DEFINE(dType, dVar, dDef, dVal) dVar(dDef),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1{},                         \
            dVar##Array2{},                         \
            dVar##Array3{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mArrayOnly{}
    { }

    // 保存前の値設定
    HalfAutoName(bool) :
        #define DEFINE(dType, dVar, dDef, dVal) dVar##Private(dVal),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1Private{},                  \
            dVar##Array2Private{},                  \
            dVar##Array3Private{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        #define DEFINE(dType, dVar, dDef, dVal) dVar##Protected(dVal),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1Protected{},                \
            dVar##Array2Protected{},                \
            dVar##Array3Protected{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        #define DEFINE(dType, dVar, dDef, dVal) dVar(dVal),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1{},                         \
            dVar##Array2{},                         \
            dVar##Array3{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mArrayOnly{}
    {
        #define DEFINE(dType, dVar, dDef, dVal)
        #define ARRAY(dType, dVar, dNum, dDef, dVal)        \
            for (std::size_t i=0; i < dNum; ++i)            \
            {                                               \
                dVar##Array1Private[i]=dVal;                \
                dVar##Array1Protected[i]=dVal;              \
                dVar##Array1[i]=dVal;                       \
                for (std::size_t j=0; j < 2; ++j)           \
                {                                           \
                    dVar##Array2Private[j][i]=dVal;         \
                    dVar##Array2Protected[j][i]=dVal;       \
                    dVar##Array2[j][i]=dVal;                \
                    for (std::size_t k=0; k < 3; ++k)       \
                    {                                       \
                        dVar##Array3Private[k][j][i]=dVal;  \
                        dVar##Array3Protected[k][j][i]=dVal;\
                        dVar##Array3[k][j][i]=dVal;         \
                    }                                       \
                }                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE

        for (std::size_t i=0; i < kRow; ++i)
        {
            for (std::size_t j=0; j < kCol; ++j)
            {
                mArrayOnly[i][j].mInt=kValue;
            }
        }
    }

    // privateメンバの値をチェック
    void checkPrivate(bool iIsValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar##Private, ((!iIsValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1Private[i], ((!iIsValued)?dDef:dVal), i);\
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2Private[j][i], ((!iIsValued)?dDef:dVal), j, i);\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3Private[k][j][i], ((!iIsValued)?dDef:dVal),\
                                        k, j, i);                           \
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // protectedメンバの値をチェック
    void checkProtected(bool iIsValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar##Protected, ((!iIsValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1Protected[i], ((!iIsValued)?dDef:dVal), i);\
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2Protected[j][i], ((!iIsValued)?dDef:dVal), j, i);\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3Protected[k][j][i], ((!iIsValued)?dDef:dVal),\
                                        k, j, i);                           \
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // publicメンバの値をチェック
    void checkPublic(bool iIsValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar, ((!iIsValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1[i], ((!iIsValued)?dDef:dVal), i);\
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2[j][i], ((!iIsValued)?dDef:dVal), j, i);\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3[k][j][i], ((!iIsValued)?dDef:dVal),\
                                        k, j, i);                           \
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE

        for (std::size_t i=0; i < kRow; ++i)
        {
            for (std::size_t j=0; j < kCol; ++j)
            {
                THEOLIZER_EQUAL(mArrayOnly[i][j].mInt, ((!iIsValued)?0:kValue));
            }
        }
    }

    // 侵入型半自動 指定
    THEOLIZER_INTRUSIVE(CS, (HalfAutoName), 1);
};

//----------------------------------------------------------------------------
//      侵入型半自動－順序対応
//----------------------------------------------------------------------------

//      ---<<< 配列のみで参照される完全自動型 >>>---

struct ArrayOnly2
{
    int     mInt;
    ArrayOnly2() : mInt(0) { }
};

//      ---<<< 本体 >>>---

class HalfAutoOrder
{
    // privateメンバ定義
    #define DEFINE(dType, dVar, dDef, dVal)     dType dVar##Private;
    #define ARRAY(dType, dVar, dNum, dDef, dVal)    \
        dType dVar##Array1Private[dNum];            \
        dType dVar##Array2Private[2][dNum];         \
        dType dVar##Array3Private[3][2][dNum];
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE

protected:
    // protectedメンバ定義
    #define DEFINE(dType, dVar, dDef, dVal) dType dVar##Protected;
    #define ARRAY(dType, dVar, dNum, dDef, dVal)    \
        dType dVar##Array1Protected[dNum];          \
        dType dVar##Array2Protected[2][dNum];       \
        dType dVar##Array3Protected[3][2][dNum];
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE

public:
    // publicメンバ定義
    #define DEFINE(dType, dVar, dDef, dVal) dType dVar;
    #define ARRAY(dType, dVar, dNum, dDef, dVal)    \
        dType dVar##Array1[dNum];                   \
        dType dVar##Array2[2][dNum];                \
        dType dVar##Array3[3][2][dNum];
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE

    // クラス配列
    static const std::size_t    kRow=3;
    static const std::size_t    kCol=2;
    static const int            kValue=-456;
    ArrayOnly1  mArrayOnly[kRow][kCol];

    // デフォルト・コンストラクタ
    HalfAutoOrder() :
        #define DEFINE(dType, dVar, dDef, dVal) dVar##Private(dDef),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1Private{},                  \
            dVar##Array2Private{},                  \
            dVar##Array3Private{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        #define DEFINE(dType, dVar, dDef, dVal) dVar##Protected(dDef),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1Protected{},                \
            dVar##Array2Protected{},                \
            dVar##Array3Protected{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        #define DEFINE(dType, dVar, dDef, dVal) dVar(dDef),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1{},                         \
            dVar##Array2{},                         \
            dVar##Array3{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mArrayOnly{}
    { }

    // 保存前の値設定
    HalfAutoOrder(bool) :
        #define DEFINE(dType, dVar, dDef, dVal) dVar##Private(dVal),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1Private{},                  \
            dVar##Array2Private{},                  \
            dVar##Array3Private{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        #define DEFINE(dType, dVar, dDef, dVal) dVar##Protected(dVal),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1Protected{},                \
            dVar##Array2Protected{},                \
            dVar##Array3Protected{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        #define DEFINE(dType, dVar, dDef, dVal) dVar(dVal),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1{},                         \
            dVar##Array2{},                         \
            dVar##Array3{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mArrayOnly{}
    {
        #define DEFINE(dType, dVar, dDef, dVal)
        #define ARRAY(dType, dVar, dNum, dDef, dVal)        \
            for (std::size_t i=0; i < dNum; ++i)            \
            {                                               \
                dVar##Array1Private[i]=dVal;                \
                dVar##Array1Protected[i]=dVal;              \
                dVar##Array1[i]=dVal;                       \
                for (std::size_t j=0; j < 2; ++j)           \
                {                                           \
                    dVar##Array2Private[j][i]=dVal;         \
                    dVar##Array2Protected[j][i]=dVal;       \
                    dVar##Array2[j][i]=dVal;                \
                    for (std::size_t k=0; k < 3; ++k)       \
                    {                                       \
                        dVar##Array3Private[k][j][i]=dVal;  \
                        dVar##Array3Protected[k][j][i]=dVal;\
                        dVar##Array3[k][j][i]=dVal;         \
                    }                                       \
                }                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE

        for (std::size_t i=0; i < kRow; ++i)
        {
            for (std::size_t j=0; j < kCol; ++j)
            {
                mArrayOnly[i][j].mInt=kValue;
            }
        }
    }

    // privateメンバの値をチェック
    void checkPrivate(bool iIsValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar##Private, ((!iIsValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1Private[i], ((!iIsValued)?dDef:dVal), i);\
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2Private[j][i], ((!iIsValued)?dDef:dVal), j, i);\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3Private[k][j][i], ((!iIsValued)?dDef:dVal),\
                                        k, j, i);                           \
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // protectedメンバの値をチェック
    void checkProtected(bool iIsValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar##Protected, ((!iIsValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1Protected[i], ((!iIsValued)?dDef:dVal), i);\
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2Protected[j][i], ((!iIsValued)?dDef:dVal), j, i);\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3Protected[k][j][i], ((!iIsValued)?dDef:dVal),\
                                        k, j, i);                           \
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // publicメンバの値をチェック
    void checkPublic(bool iIsValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar, ((!iIsValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1[i], ((!iIsValued)?dDef:dVal), i);\
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2[j][i], ((!iIsValued)?dDef:dVal), j, i);\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3[k][j][i], ((!iIsValued)?dDef:dVal),\
                                        k, j, i);                           \
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE

        for (std::size_t i=0; i < kRow; ++i)
        {
            for (std::size_t j=0; j < kCol; ++j)
            {
                THEOLIZER_EQUAL(mArrayOnly[i][j].mInt, ((!iIsValued)?0:kValue));
            }
        }
    }

    // 侵入型半自動 指定
    THEOLIZER_INTRUSIVE_ORDER(CS, (HalfAutoOrder), 1);
};

//----------------------------------------------------------------------------
//      非侵入型手動
//----------------------------------------------------------------------------

//      ---<<< 配列のみで参照される完全自動型 >>>---

struct ArrayOnly3
{
    int     mInt;
    ArrayOnly3() : mInt(0) { }
};

//      ---<<< 本体 >>>---

class Manual
{
public:
    // メンバ定義
    #define DEFINE(dType, dVar, dDef, dVal) dType dVar;
    #define ARRAY(dType, dVar, dNum, dDef, dVal)    \
        dType dVar##Array1[dNum];                   \
        dType dVar##Array2[2][dNum];                \
        dType dVar##Array3[3][2][dNum];
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE

    // クラス配列
    static const std::size_t    kRow=3;
    static const std::size_t    kCol=3;
    static const int            kValue=-123;
    ArrayOnly3  mArrayOnly[kRow][kCol];

    // デフォルト・コンストラクタ
    Manual() :
        #define DEFINE(dType, dVar, dDef, dVal) dVar(dDef),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1{},                         \
            dVar##Array2{},                         \
            dVar##Array3{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mArrayOnly{}
    { }

    // 保存前の値設定
    Manual(bool) :
        #undef  DEFINE
        #define DEFINE(dType, dVar, dDef, dVal) dVar(dVal),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            dVar##Array1{},                         \
            dVar##Array2{},                         \
            dVar##Array3{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mArrayOnly{}
    {
        #define DEFINE(dType, dVar, dDef, dVal)
        #define ARRAY(dType, dVar, dNum, dDef, dVal)        \
            for (std::size_t i=0; i < dNum; ++i)            \
            {                                               \
                dVar##Array1[i]=dVal;                       \
                for (std::size_t j=0; j < 2; ++j)           \
                {                                           \
                    dVar##Array2[j][i]=dVal;                \
                    for (std::size_t k=0; k < 3; ++k)       \
                    {                                       \
                        dVar##Array3[k][j][i]=dVal;         \
                    }                                       \
                }                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE

        for (std::size_t i=0; i < kRow; ++i)
        {
            for (std::size_t j=0; j < kCol; ++j)
            {
                mArrayOnly[i][j].mInt=kValue;
            }
        }
    }

    // publicメンバの値をチェック
    void checkPublic(bool iIsValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar, ((!iIsValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1[i], ((!iIsValued)?dDef:dVal));  \
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2[j][i], ((!iIsValued)?dDef:dVal));\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3[k][j][i], ((!iIsValued)?dDef:dVal));\
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE

        for (std::size_t i=0; i < kRow; ++i)
        {
            for (std::size_t j=0; j < kCol; ++j)
            {
                THEOLIZER_EQUAL(mArrayOnly[i][j].mInt, ((!iIsValued)?0:kValue));
            }
        }
    }
};

//      ---<<< シリアライズ用の手動記述 >>>---

THEOLIZER_NON_INTRUSIVE_ORDER((Manual), 1);

// 以下、test_class_variation.cpp.theolizer.hppから、
//  #ifdef  THEOLIZER_WRITE_CODE // ###### Manual ######の
//  #if false // Sample of save/load function.
//  から
//  #endif // Sample of save/load function.
//  までの間をコピーして、saveClassManual()/loadClassManual()の内容を記述。

template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<Manual>::TheolizerUserDefine<tBaseSerializer, tTheolizerVersion,1>
{
    // Save members.
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        #define DEFINE(dType, dVar, dDef, dVal)                         \
            THEOLIZER_PROCESS(iSerializer, iInstance->dVar);
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                    \
            THEOLIZER_PROCESS(iSerializer, iInstance->dVar##Array1);    \
            THEOLIZER_PROCESS(iSerializer, iInstance->dVar##Array2);    \
            THEOLIZER_PROCESS(iSerializer, iInstance->dVar##Array3);
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        THEOLIZER_PROCESS(iSerializer, iInstance->mArrayOnly);
    }

    // Load members.
    static void loadClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        if (!oInstance) oInstance=new typename tTheolizerVersion::TheolizerTarget();

        #define DEFINE(dType, dVar, dDef, dVal)                         \
            THEOLIZER_PROCESS(iSerializer, oInstance->dVar);
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                    \
            THEOLIZER_PROCESS(iSerializer, oInstance->dVar##Array1);    \
            THEOLIZER_PROCESS(iSerializer, oInstance->dVar##Array2);    \
            THEOLIZER_PROCESS(iSerializer, oInstance->dVar##Array3);
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        THEOLIZER_PROCESS(iSerializer, oInstance->mArrayOnly);
    }
};

#endif  // ENABLE_SINGLE_TEST

// ***************************************************************************
//      組み合わせテスト用基底クラス
//          DrivedFullAuto, DrivedHalfAuto等を同じ基底クラスから派生すると
//          ３重テストで同じ基底クラスを別クラス経由で派生することになるため、
//          基底クラスを別クラスにする必要がある。
//          そこで、簡単のため、クラス・テンプレートを用いている。
//
//          なお、Theolizerはクラス・テンプレートに対応しているが、
//          現時点では非公開機能である。
// ***************************************************************************

#ifdef ENABLE_COMBINATION_TEST

//----------------------------------------------------------------------------
//      非侵入型完全自動　基底クラス
//----------------------------------------------------------------------------

//      ---<<< 追加テスト >>>---
//      侵入型を非侵入型へprivate継承するとMinGWでコンパイル・エラーになる不具合の再発防止用

struct PrivateInheritance
{
    THEOLIZER_INTRUSIVE(CS, (PrivateInheritance), 1);
};

//      ---<<< 本体 >>>---

template<int tInitialValue>
class BaseFullAuto : private PrivateInheritance
{
    static const int kIntPrivate  =tInitialValue;
    static const int kIntProtected=tInitialValue+1;
    static const int kIntPublic   =tInitialValue+2;

    int     mIntPrivate;

protected:
    int     mIntProtected;

public:
    int     mIntPublic;

    // コンストラクタ
    //  (デフォルト・コンストラクタは意図的に無し)
    BaseFullAuto(bool iIsValued) :
        mIntPrivate(  (iIsValued)?kIntPrivate:0),
        mIntProtected((iIsValued)?kIntProtected:0),
        mIntPublic(   (iIsValued)?kIntPublic:0)
    { }

    // 一致チェック
    void checkPrivate(bool iIsValued=false)
    {
        THEOLIZER_EQUAL(mIntPrivate, ((!iIsValued)?0:kIntPrivate));
    }
    void checkProtected(bool iIsValued=false)
    {
        THEOLIZER_EQUAL(mIntProtected, ((!iIsValued)?0:kIntProtected));
    }
    void checkPublic(bool iIsValued=false)
    {
        THEOLIZER_EQUAL(mIntPublic, ((!iIsValued)?0:kIntPublic));
    }
};

//----------------------------------------------------------------------------
//      侵入型半自動　基底クラス
//----------------------------------------------------------------------------

template<int tInitialValue>
class BaseHalfAuto
{
    static const int kIntPrivate  =tInitialValue;
    static const int kIntProtected=tInitialValue+1;
    static const int kIntPublic   =tInitialValue+2;

    int     mIntPrivate;

protected:
    int     mIntProtected;

public:
    int     mIntPublic;

    // デフォルト・コンストラクタ
    BaseHalfAuto() : mIntPrivate(0), mIntProtected(0), mIntPublic(0)
    { }

    // コンストラクタ
    //  (デフォルト・コンストラクタは意図的に無し)
    BaseHalfAuto(bool iIsValued) :
        mIntPrivate(  (iIsValued)?kIntPrivate:0),
        mIntProtected((iIsValued)?kIntProtected:0),
        mIntPublic(   (iIsValued)?kIntPublic:0)
    { }

    // 一致チェック
    void checkPrivate(bool iIsValued=false)
    {
        THEOLIZER_EQUAL(mIntPrivate, ((!iIsValued)?0:kIntPrivate));
    }
    void checkProtected(bool iIsValued=false)
    {
        THEOLIZER_EQUAL(mIntProtected, ((!iIsValued)?0:kIntProtected));
    }
    void checkPublic(bool iIsValued=false)
    {
        THEOLIZER_EQUAL(mIntPublic, ((!iIsValued)?0:kIntPublic));
    }

    // 侵入型半自動 指定
    THEOLIZER_INTRUSIVE_TEMPLATE
    (
        CS,
        (template<int tInitialValue>),
        (BaseHalfAuto<tInitialValue>),
        1,
        BaseHalfAutoPrimary
    );
};

//----------------------------------------------------------------------------
//      非侵入型手動　基底クラス
//----------------------------------------------------------------------------

//      ---<<< 追加テスト >>>---
//      侵入型を非侵入型へprotected継承するとMinGWでコンパイル・エラーになる不具合の再発防止用

struct ProtectedInheritance
{
    THEOLIZER_INTRUSIVE(CS, (ProtectedInheritance), 1);
};

//      ---<<< 本体 >>>---

template<int tInitialValue>
class BaseManual : protected ProtectedInheritance
{
    static const int kIntPublic   =tInitialValue+2;

public:
    int     mIntPublic;

    // コンストラクタ
    //  (デフォルト・コンストラクタは意図的に無し)
    BaseManual(bool iIsValued=false) :
        mIntPublic((iIsValued)?kIntPublic:0)
    { }

    // 一致チェック
    void checkPublic(bool iIsValued=false)
    {
        THEOLIZER_EQUAL(mIntPublic, ((!iIsValued)?0:kIntPublic));
    }
};

//      ---<<< シリアライズ用の手動記述 >>>---

//THEOLIZER_NON_INTRUSIVE_ORDER((BaseManual), 1);
THEOLIZER_NON_INTRUSIVE_TEMPLATE_ORDER
(
    (template<int tInitialValue>),
    (BaseManual<tInitialValue>),
    1,
    BaseManualPrimary
);

// 以下、test_class_variation.cpp.theolizer.hppから、
//  #ifdef  THEOLIZER_WRITE_CODE // ###### BaseManual ######の
//  #if false // Sample of save/load function.
//  から
//  #endif // Sample of save/load function.
//  までの間をコピーして、saveClassManual()/loadClassManual()の内容を記述。

template<int tInitialValue>
template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<BaseManual<tInitialValue>>::
    TheolizerUserDefine<tBaseSerializer, tTheolizerVersion, 1>
{
    // Save members.
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        THEOLIZER_PROCESS(iSerializer, iInstance->mIntPublic);
    }

    // Load members.
    static void loadClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        if (!oInstance) oInstance=new typename tTheolizerVersion::TheolizerTarget(false);

        THEOLIZER_PROCESS(iSerializer, oInstance->mIntPublic);
    }
};

// ***************************************************************************
//      ２重組み合わせテスト
// ***************************************************************************

//----------------------------------------------------------------------------
//      非侵入型完全自動
//----------------------------------------------------------------------------

class DerivedFullAuto :
    private     BaseFullAuto<1111>,
    private     BaseHalfAuto<1121>,
    private     BaseManual  <1131>,
    protected   BaseFullAuto<1211>,
    protected   BaseHalfAuto<1221>,
    protected   BaseManual  <1231>,
    public      BaseFullAuto<1311>,
    public      BaseHalfAuto<1321>,
    public      BaseManual  <1331>
{
    BaseFullAuto<1411>  mBaseFullAutoPrivate;
    BaseHalfAuto<1421>  mBaseHalfAutoPrivate;
    BaseManual  <1431>  mBaseManualPrivate;
protected:
    BaseFullAuto<1511>  mBaseFullAutoProtected;
    BaseHalfAuto<1521>  mBaseHalfAutoProtected;
    BaseManual  <1531>  mBaseManualProtected;
public:
    BaseFullAuto<1611>  mBaseFullAutoPublic;
    BaseHalfAuto<1621>  mBaseHalfAutoPublic;
    BaseManual  <1631>  mBaseManualPublic;

    // デフォルト・コンストラクタ
    DerivedFullAuto() :
        BaseFullAuto<1111>{false},
        BaseHalfAuto<1121>{false},
        BaseManual  <1131>{false},
        BaseFullAuto<1211>{false},
        BaseHalfAuto<1221>{false},
        BaseManual  <1231>{false},
        BaseFullAuto<1311>{false},
        BaseHalfAuto<1321>{false},
        BaseManual  <1331>{false},
        mBaseFullAutoPrivate{false},
        mBaseHalfAutoPrivate{false},
        mBaseManualPrivate{false},
        mBaseFullAutoProtected{false},
        mBaseHalfAutoProtected{false},
        mBaseManualProtected{false},
        mBaseFullAutoPublic{false},
        mBaseHalfAutoPublic{false},
        mBaseManualPublic{false}
    { }

    // 保存用コンストラクタ
    DerivedFullAuto(bool) :
        BaseFullAuto<1111>{true},
        BaseHalfAuto<1121>{true},
        BaseManual  <1131>{true},
        BaseFullAuto<1211>{true},
        BaseHalfAuto<1221>{true},
        BaseManual  <1231>{true},
        BaseFullAuto<1311>{true},
        BaseHalfAuto<1321>{true},
        BaseManual  <1331>{true},
        mBaseFullAutoPrivate{true},
        mBaseHalfAutoPrivate{true},
        mBaseManualPrivate{true},
        mBaseFullAutoProtected{true},
        mBaseHalfAutoProtected{true},
        mBaseManualProtected{true},
        mBaseFullAutoPublic{true},
        mBaseHalfAutoPublic{true},
        mBaseManualPublic{true}
    { }

    void check(bool doSave=true)
    {
        // private継承
        BaseFullAuto<1111>::checkPrivate(false);            // private継承は保存無し
        BaseFullAuto<1111>::checkProtected(false);          // private継承は保存無し
        BaseFullAuto<1111>::checkPublic(false);             // private継承は保存無し
        BaseHalfAuto<1121>::checkPrivate(false);            // private継承は保存無し
        BaseHalfAuto<1121>::checkProtected(false);          // private継承は保存無し
        BaseHalfAuto<1121>::checkPublic(false);             // private継承は保存無し
        BaseManual  <1131>::checkPublic(false);             // private継承は保存無し

        // protected継承
        BaseFullAuto<1211>::checkPrivate(false);            // 非侵入型のprivateは保存無し
        BaseFullAuto<1211>::checkProtected(doSave);
        BaseFullAuto<1211>::checkPublic(doSave);
        BaseHalfAuto<1221>::checkPrivate(doSave);
        BaseHalfAuto<1221>::checkProtected(doSave);
        BaseHalfAuto<1221>::checkPublic(doSave);
        BaseManual  <1231>::checkPublic(doSave);

        // public継承
        BaseFullAuto<1311>::checkPrivate(false);            // 非侵入型のprivateは保存無し
        BaseFullAuto<1311>::checkProtected(doSave);
        BaseFullAuto<1311>::checkPublic(doSave);
        BaseHalfAuto<1321>::checkPrivate(doSave);
        BaseHalfAuto<1321>::checkProtected(doSave);
        BaseHalfAuto<1321>::checkPublic(doSave);
        BaseManual  <1331>::checkPublic(doSave);

        // privateメンバ
        mBaseFullAutoPrivate.checkPrivate(false);           // privateメンバは保存無し
        mBaseFullAutoPrivate.checkProtected(false);         // privateメンバは保存無し
        mBaseFullAutoPrivate.checkPublic(false);            // privateメンバは保存無し
        mBaseHalfAutoPrivate.checkPrivate(false);           // privateメンバは保存無し
        mBaseHalfAutoPrivate.checkProtected(false);         // privateメンバは保存無し
        mBaseHalfAutoPrivate.checkPublic(false);            // privateメンバは保存無し
        mBaseManualPrivate.checkPublic(false);              // privateメンバは保存無し

        // protectedメンバ
        mBaseFullAutoProtected.checkPrivate(false);         // 非侵入型のprivateは保存無し
        mBaseFullAutoProtected.checkProtected(doSave);
        mBaseFullAutoProtected.checkPublic(doSave);
        mBaseHalfAutoProtected.checkPrivate(doSave);
        mBaseHalfAutoProtected.checkProtected(doSave);
        mBaseHalfAutoProtected.checkPublic(doSave);
        mBaseManualProtected.checkPublic(doSave);

        // publicメンバ
        mBaseFullAutoPublic.checkPrivate(false);            // 非侵入型のprivateは保存無し
        mBaseFullAutoPublic.checkProtected(doSave);
        mBaseFullAutoPublic.checkPublic(doSave);
        mBaseHalfAutoPublic.checkPrivate(doSave);
        mBaseHalfAutoPublic.checkProtected(doSave);
        mBaseHalfAutoPublic.checkPublic(doSave);
        mBaseManualPublic.checkPublic(doSave);
    }
};

//----------------------------------------------------------------------------
//      侵入型半自動
//----------------------------------------------------------------------------

class DerivedHalfAuto :
    private     BaseFullAuto<2111>,
    private     BaseHalfAuto<2121>,
    private     BaseManual  <2131>,
    protected   BaseFullAuto<2211>,
    protected   BaseHalfAuto<2221>,
    protected   BaseManual  <2231>,
    public      BaseFullAuto<2311>,
    public      BaseHalfAuto<2321>,
    public      BaseManual  <2331>
{
    BaseFullAuto<2411>  mBaseFullAutoPrivate;
    BaseHalfAuto<2421>  mBaseHalfAutoPrivate;
    BaseManual  <2431>  mBaseManualPrivate;
protected:
    BaseFullAuto<2511>  mBaseFullAutoProtected;
    BaseHalfAuto<2521>  mBaseHalfAutoProtected;
    BaseManual  <2531>  mBaseManualProtected;
public:
    BaseFullAuto<2611>  mBaseFullAutoPublic;
    BaseHalfAuto<2621>  mBaseHalfAutoPublic;
    BaseManual  <2631>  mBaseManualPublic;

    // デフォルト・コンストラクタ
    DerivedHalfAuto() :
        BaseFullAuto<2111>{false},
        BaseHalfAuto<2121>{false},
        BaseManual  <2131>{false},
        BaseFullAuto<2211>{false},
        BaseHalfAuto<2221>{false},
        BaseManual  <2231>{false},
        BaseFullAuto<2311>{false},
        BaseHalfAuto<2321>{false},
        BaseManual  <2331>{false},
        mBaseFullAutoPrivate{false},
        mBaseHalfAutoPrivate{false},
        mBaseManualPrivate{false},
        mBaseFullAutoProtected{false},
        mBaseHalfAutoProtected{false},
        mBaseManualProtected{false},
        mBaseFullAutoPublic{false},
        mBaseHalfAutoPublic{false},
        mBaseManualPublic{false}
    { }

    // 保存用コンストラクタ
    DerivedHalfAuto(bool) :
        BaseFullAuto<2111>{true},
        BaseHalfAuto<2121>{true},
        BaseManual  <2131>{true},
        BaseFullAuto<2211>{true},
        BaseHalfAuto<2221>{true},
        BaseManual  <2231>{true},
        BaseFullAuto<2311>{true},
        BaseHalfAuto<2321>{true},
        BaseManual  <2331>{true},
        mBaseFullAutoPrivate{true},
        mBaseHalfAutoPrivate{true},
        mBaseManualPrivate{true},
        mBaseFullAutoProtected{true},
        mBaseHalfAutoProtected{true},
        mBaseManualProtected{true},
        mBaseFullAutoPublic{true},
        mBaseHalfAutoPublic{true},
        mBaseManualPublic{true}
    { }

    void check(bool doSave=true)
    {
        // private継承
        BaseFullAuto<2111>::checkPrivate(false);            // 非侵入型のprivateは保存無し
        BaseFullAuto<2111>::checkProtected(doSave);
        BaseFullAuto<2111>::checkPublic(doSave);
        BaseHalfAuto<2121>::checkPrivate(doSave);
        BaseHalfAuto<2121>::checkProtected(doSave);
        BaseHalfAuto<2121>::checkPublic(doSave);
        BaseManual  <2131>::checkPublic(doSave);

        // protected継承
        BaseFullAuto<2211>::checkPrivate(false);            // 非侵入型のprivateは保存無し
        BaseFullAuto<2211>::checkProtected(doSave);
        BaseFullAuto<2211>::checkPublic(doSave);
        BaseHalfAuto<2221>::checkPrivate(doSave);
        BaseHalfAuto<2221>::checkProtected(doSave);
        BaseHalfAuto<2221>::checkPublic(doSave);
        BaseManual  <2231>::checkPublic(doSave);

        // public継承
        BaseFullAuto<2311>::checkPrivate(false);            // 非侵入型のprivateは保存無し
        BaseFullAuto<2311>::checkProtected(doSave);
        BaseFullAuto<2311>::checkPublic(doSave);
        BaseHalfAuto<2321>::checkPrivate(doSave);
        BaseHalfAuto<2321>::checkProtected(doSave);
        BaseHalfAuto<2321>::checkPublic(doSave);
        BaseManual  <2331>::checkPublic(doSave);

        // privateメンバ
        mBaseFullAutoPrivate.checkPrivate(false);           // 非侵入型のprivateは保存無し
        mBaseFullAutoPrivate.checkProtected(doSave);
        mBaseFullAutoPrivate.checkPublic(doSave);
        mBaseHalfAutoPrivate.checkPrivate(doSave);
        mBaseHalfAutoPrivate.checkProtected(doSave);
        mBaseHalfAutoPrivate.checkPublic(doSave);
        mBaseManualPrivate.checkPublic(doSave);

        // protectedメンバ
        mBaseFullAutoProtected.checkPrivate(false);         // 非侵入型のprivateは保存無し
        mBaseFullAutoProtected.checkProtected(doSave);
        mBaseFullAutoProtected.checkPublic(doSave);
        mBaseHalfAutoProtected.checkPrivate(doSave);
        mBaseHalfAutoProtected.checkProtected(doSave);
        mBaseHalfAutoProtected.checkPublic(doSave);
        mBaseManualProtected.checkPublic(doSave);

        // publicメンバ
        mBaseFullAutoPublic.checkPrivate(false);            // 非侵入型のprivateは保存無し
        mBaseFullAutoPublic.checkProtected(doSave);
        mBaseFullAutoPublic.checkPublic(doSave);
        mBaseHalfAutoPublic.checkPrivate(doSave);
        mBaseHalfAutoPublic.checkProtected(doSave);
        mBaseHalfAutoPublic.checkPublic(doSave);
        mBaseManualPublic.checkPublic(doSave);
    }

    // 侵入型半自動 指定
    THEOLIZER_INTRUSIVE(CS, (DerivedHalfAuto), 1);
};

//----------------------------------------------------------------------------
//      非侵入型手動
//----------------------------------------------------------------------------

class DerivedManual :
    public      BaseFullAuto<3311>,
    public      BaseHalfAuto<3321>,
    public      BaseManual  <3331>
{
public:
    BaseFullAuto<3611>  mBaseFullAutoPublic;
    BaseHalfAuto<3621>  mBaseHalfAutoPublic;
    BaseManual  <3631>  mBaseManualPublic;

    // デフォルト・コンストラクタ
    DerivedManual() :
        BaseFullAuto{false},
        BaseHalfAuto{false},
        BaseManual{false},
        mBaseFullAutoPublic{false},
        mBaseHalfAutoPublic{false},
        mBaseManualPublic{false}
    { }

    // 保存用コンストラクタ
    DerivedManual(bool) :
        BaseFullAuto{true},
        BaseHalfAuto{true},
        BaseManual{true},
        mBaseFullAutoPublic{true},
        mBaseHalfAutoPublic{true},
        mBaseManualPublic{true}
    { }

    void check(bool doSave=true)
    {
        // public継承
        BaseFullAuto::checkPrivate(false);              // 完全自動のprivateは保存無し
        BaseFullAuto::checkProtected(doSave);
        BaseFullAuto::checkPublic(doSave);
        BaseHalfAuto::checkPrivate(doSave);
        BaseHalfAuto::checkProtected(doSave);
        BaseHalfAuto::checkPublic(doSave);
        BaseManual::checkPublic(doSave);

        // publicメンバ
        mBaseFullAutoPublic.checkPrivate(false);        // 完全自動のprivateは保存無し
        mBaseFullAutoPublic.checkProtected(doSave);
        mBaseFullAutoPublic.checkPublic(doSave);
        mBaseHalfAutoPublic.checkPrivate(doSave);
        mBaseHalfAutoPublic.checkProtected(doSave);
        mBaseHalfAutoPublic.checkPublic(doSave);
        mBaseManualPublic.checkPublic(doSave);
    }
};

//      ---<<< シリアライズ用の手動記述 >>>---

THEOLIZER_NON_INTRUSIVE_ORDER((DerivedManual), 1);

template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<DerivedManual>::
    TheolizerUserDefine<tBaseSerializer, tTheolizerVersion,1>
{
    // Save members.
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        THEOLIZER_PROCESS_BASE(iSerializer, BaseFullAuto, iInstance);
        THEOLIZER_PROCESS_BASE(iSerializer, BaseHalfAuto, iInstance);
        THEOLIZER_PROCESS_BASE(iSerializer, BaseManual ,  iInstance);
        THEOLIZER_PROCESS(iSerializer, iInstance->mBaseFullAutoPublic);
        THEOLIZER_PROCESS(iSerializer, iInstance->mBaseHalfAutoPublic);
        THEOLIZER_PROCESS(iSerializer, iInstance->mBaseManualPublic);
    }

    // Load members.
    static void loadClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        if (!oInstance) oInstance=new typename tTheolizerVersion::TheolizerTarget();

        THEOLIZER_PROCESS_BASE(iSerializer, BaseFullAuto, oInstance);
        THEOLIZER_PROCESS_BASE(iSerializer, BaseHalfAuto, oInstance);
        THEOLIZER_PROCESS_BASE(iSerializer, BaseManual,   oInstance);
        THEOLIZER_PROCESS(iSerializer, oInstance->mBaseFullAutoPublic);
        THEOLIZER_PROCESS(iSerializer, oInstance->mBaseHalfAutoPublic);
        THEOLIZER_PROCESS(iSerializer, oInstance->mBaseManualPublic);
    }
};

// ***************************************************************************
//      ３重組み合わせテスト
// ***************************************************************************

#ifdef ENABLE_TRIPLED_TEST

//----------------------------------------------------------------------------
//      非侵入型完全自動
//----------------------------------------------------------------------------

class TripledFullAuto :
    public      DerivedFullAuto,
    public      DerivedHalfAuto,
    public      DerivedManual
{
    DerivedFullAuto mDerivedFullAutoPrivate;
    DerivedHalfAuto mDerivedHalfAutoPrivate;
    DerivedManual   mDerivedManualPrivate;
protected:
    DerivedFullAuto mDerivedFullAutoProtected;
    DerivedHalfAuto mDerivedHalfAutoProtected;
    DerivedManual   mDerivedManualProtected;
public:
    DerivedFullAuto mDerivedFullAutoPublic;
    DerivedHalfAuto mDerivedHalfAutoPublic;
    DerivedManual   mDerivedManualPublic;

    TripledFullAuto() :
        DerivedFullAuto{},
        DerivedHalfAuto{},
        DerivedManual{},
        mDerivedFullAutoPrivate{},
        mDerivedHalfAutoPrivate{},
        mDerivedManualPrivate{},
        mDerivedFullAutoProtected{},
        mDerivedHalfAutoProtected{},
        mDerivedManualProtected{},
        mDerivedFullAutoPublic{},
        mDerivedHalfAutoPublic{},
        mDerivedManualPublic{}
    { }

    TripledFullAuto(bool) :
        DerivedFullAuto{true},
        DerivedHalfAuto{true},
        DerivedManual{true},
        mDerivedFullAutoPrivate{true},
        mDerivedHalfAutoPrivate{true},
        mDerivedManualPrivate{true},
        mDerivedFullAutoProtected{true},
        mDerivedHalfAutoProtected{true},
        mDerivedManualProtected{true},
        mDerivedFullAutoPublic{true},
        mDerivedHalfAutoPublic{true},
        mDerivedManualPublic{true}
    { }

    void check()
    {
        // public継承（シリアライズ有りとして判定する）
        DerivedFullAuto::check();
        DerivedHalfAuto::check();
        DerivedManual::check();

        // privateメンバ（シリアライズ無しとして判定する）
        mDerivedFullAutoPrivate.check(false);
        mDerivedHalfAutoPrivate.check(false);
        mDerivedManualPrivate.check(false);

        // protectedメンバ（シリアライズ有りとして判定する）
        mDerivedFullAutoProtected.check();
        mDerivedHalfAutoProtected.check();
        mDerivedManualProtected.check();

        // publicメンバ（シリアライズ有りとして判定する）
        mDerivedFullAutoPublic.check();
        mDerivedHalfAutoPublic.check();
        mDerivedManualPublic.check();
    }
};

//----------------------------------------------------------------------------
//      侵入型半自動
//----------------------------------------------------------------------------

class TripledHalfAuto :
    public      DerivedFullAuto,
    public      DerivedHalfAuto,
    public      DerivedManual
{
    DerivedFullAuto mDerivedFullAutoPrivate;
    DerivedHalfAuto mDerivedHalfAutoPrivate;
    DerivedManual   mDerivedManualPrivate;
protected:
    DerivedFullAuto mDerivedFullAutoProtected;
    DerivedHalfAuto mDerivedHalfAutoProtected;
    DerivedManual   mDerivedManualProtected;
public:
    DerivedFullAuto mDerivedFullAutoPublic;
    DerivedHalfAuto mDerivedHalfAutoPublic;
    DerivedManual   mDerivedManualPublic;

    TripledHalfAuto() :
        DerivedFullAuto{},
        DerivedHalfAuto{},
        DerivedManual{},
        mDerivedFullAutoPrivate{},
        mDerivedHalfAutoPrivate{},
        mDerivedManualPrivate{},
        mDerivedFullAutoProtected{},
        mDerivedHalfAutoProtected{},
        mDerivedManualProtected{},
        mDerivedFullAutoPublic{},
        mDerivedHalfAutoPublic{},
        mDerivedManualPublic{}
    { }

    TripledHalfAuto(bool) :
        DerivedFullAuto{true},
        DerivedHalfAuto{true},
        DerivedManual{true},
        mDerivedFullAutoPrivate{true},
        mDerivedHalfAutoPrivate{true},
        mDerivedManualPrivate{true},
        mDerivedFullAutoProtected{true},
        mDerivedHalfAutoProtected{true},
        mDerivedManualProtected{true},
        mDerivedFullAutoPublic{true},
        mDerivedHalfAutoPublic{true},
        mDerivedManualPublic{true}
    { }

    void check()
    {
        // public継承（シリアライズ有りとして判定する）
        DerivedFullAuto::check();
        DerivedHalfAuto::check();
        DerivedManual::check();

        // privateメンバ（シリアライズ有りとして判定する）
        mDerivedFullAutoPrivate.check();
        mDerivedHalfAutoPrivate.check();
        mDerivedManualPrivate.check();   

        // protectedメンバ（シリアライズ有りとして判定する）
        mDerivedFullAutoProtected.check();
        mDerivedHalfAutoProtected.check();
        mDerivedManualProtected.check();

        // publicメンバ（シリアライズ有りとして判定する）
        mDerivedFullAutoPublic.check();
        mDerivedHalfAutoPublic.check();
        mDerivedManualPublic.check();
    }

    // 侵入型半自動 指定
    THEOLIZER_INTRUSIVE(CS, (TripledHalfAuto), 1);
};

//----------------------------------------------------------------------------
//      非侵入型手動
//----------------------------------------------------------------------------

class TripledManual :
    public      DerivedFullAuto,
    public      DerivedHalfAuto,
    public      DerivedManual
{
public:
    DerivedFullAuto mDerivedFullAutoPublic;
    DerivedHalfAuto mDerivedHalfAutoPublic;
    DerivedManual   mDerivedManualPublic;

    TripledManual() :
        DerivedFullAuto{},
        DerivedHalfAuto{},
        DerivedManual{},
        mDerivedFullAutoPublic{},
        mDerivedHalfAutoPublic{},
        mDerivedManualPublic{}
    { }

    TripledManual(bool) :
        DerivedFullAuto{true},
        DerivedHalfAuto{true},
        DerivedManual{true},
        mDerivedFullAutoPublic{true},
        mDerivedHalfAutoPublic{true},
        mDerivedManualPublic{true}
    { }

    void check()
    {
        // public継承（シリアライズ有りとして判定する）
        DerivedFullAuto::check();
        DerivedHalfAuto::check();
        DerivedManual::check();

        // publicメンバ（シリアライズ有りとして判定する）
        mDerivedFullAutoPublic.check();
        mDerivedHalfAutoPublic.check();
        mDerivedManualPublic.check();
    }
};

//      ---<<< シリアライズ用の手動記述 >>>---

THEOLIZER_NON_INTRUSIVE_ORDER((TripledManual), 1);

template<class tDerivedSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<TripledManual>::
    TheolizerUserDefine<tDerivedSerializer, tTheolizerVersion,1>
{
    // Save members.
    static void saveClassManual
    (
        tDerivedSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        THEOLIZER_PROCESS_BASE(iSerializer, DerivedFullAuto, iInstance);
        THEOLIZER_PROCESS_BASE(iSerializer, DerivedHalfAuto, iInstance);
        THEOLIZER_PROCESS_BASE(iSerializer, DerivedManual,   iInstance);
        THEOLIZER_PROCESS(iSerializer, iInstance->mDerivedFullAutoPublic);
        THEOLIZER_PROCESS(iSerializer, iInstance->mDerivedHalfAutoPublic);
        THEOLIZER_PROCESS(iSerializer, iInstance->mDerivedManualPublic);
    }

    // Load members.
    static void loadClassManual
    (
        tDerivedSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        if (!oInstance) oInstance=new typename tTheolizerVersion::TheolizerTarget();

        THEOLIZER_PROCESS_BASE(iSerializer, DerivedFullAuto, oInstance);
        THEOLIZER_PROCESS_BASE(iSerializer, DerivedHalfAuto, oInstance);
        THEOLIZER_PROCESS_BASE(iSerializer, DerivedManual,   oInstance);
        THEOLIZER_PROCESS(iSerializer, oInstance->mDerivedFullAutoPublic);
        THEOLIZER_PROCESS(iSerializer, oInstance->mDerivedHalfAutoPublic);
        THEOLIZER_PROCESS(iSerializer, oInstance->mDerivedManualPublic);
    }
};

#endif  // ENABLE_TRIPLED_TEST
#endif  // ENABLE_COMBINATION_TEST

//----------------------------------------------------------------------------
//      シリアライズしていない完全自動型を含む半自動型のテスト
//          完全自動型はシリアライズしないとシリアライズ・コードが生成されない
//          しかし、完全自動型を含んでいる半自動型は、keep-step処理のため、
//          完全自動型のシリアライズ・コードを必要とする。
//          このようなケースでも完全自動型のシリアライズ・コードを生成するように
//          したのでそのテスト。（コンパイル・エラーでFAIL）
//----------------------------------------------------------------------------

struct FullAutoBase1
{
    int mInt;
};

struct FullAutoBase2
{
    int mInt;
};

struct HalfAutoDerived : public FullAutoBase1
{
    FullAutoBase2   mFullAutoBase2;
    THEOLIZER_INTRUSIVE(CS, (HalfAutoDerived), 1);
};

#endif  // TEST_CLASS_VARIATION_H
