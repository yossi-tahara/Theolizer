//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          classの様々なバリエーション
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

#if !defined(TEST_CLASS_VARIATION_H)
#define TEST_CLASS_VARIATION_H

#include <string>

// 基本のenum型とclassを使用する
#include "test_basic_process.h"

// 単独テストを無効にする
//#define DISABLE_SINGLE_TEST

// ***************************************************************************
//      使い方の説明
// ***************************************************************************

// ---------------------------------------------------------------------------
//      非侵入型完全自動
// ---------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------
//      侵入型半自動（名前対応）
// ---------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------
//      侵入型半自動（順序対応）
// ---------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------
//      非侵入型手動
// ---------------------------------------------------------------------------

class ManualTutorial
{
public:
    int     mManualPublic;

    ManualTutorial() :
        mManualPublic(0)
    { }
};

//      ---<<< シリアライズ用の手動記述 >>>---

THEOLIZER_NON_INTRUSIVE_ORDER((ManualTutorial), 1);

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

// ---------------------------------------------------------------------------
//      派生／包含クラス
// ---------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------
//      メンバ変数リスト
//          下記へ展開する
//              メンバ変数定義
//              メンバ変数初期化
//              メンバ変数の値チェック
// ---------------------------------------------------------------------------

#ifndef DISABLE_SINGLE_TEST

#define DEFINE_MEMBERS()                                                                \
    /* 文字型 */                                                                        \
    DEFINE(char,                mChar,          0,       110)                           \
    DEFINE(signed char,         mSChar,         0,      -111)                           \
    DEFINE(unsigned char,       mUChar,         0,       112)                           \
    DEFINE(wchar_t,             mWchar,         0,       1100)                          \
    DEFINE(char16_t,            mChar16,        0,       1101)                          \
    DEFINE(char32_t,            mChar32,        0,       1102)                          \
    /* 整数型 */                                                                        \
    DEFINE(short,               mShort,         0,      -2100)                          \
    DEFINE(unsigned short,      mUShort,        0,       2100)                          \
    DEFINE(int,                 mInt,           0,      -3100)                          \
    DEFINE(unsigned int,        mUInt,          0U,      3100U)                         \
    DEFINE(long,                mLong,          0L,     -4100L)                         \
    DEFINE(unsigned long,       mULong,         0UL,     4100UL)                        \
    DEFINE(long long,           mLongLong,      0LL,    -5100LL)                        \
    DEFINE(unsigned long long,  mULongLong,     0ULL,    5100ULL)                       \
    /* 浮動小数点型 */                                                                  \
    DEFINE(float,               mFloat,         0.0F,   2.23456F)                       \
    DEFINE(double,              mDouble,        0.0,    2.23456789012345)               \
    DEFINE(long double,         mLongDouble,    0.0L,   2.23456789012345L)              \
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
    ARRAY(unsigned char,        mUChar,     7,  0,       112)                           \
    ARRAY(wchar_t,              mWchar,     8,  0,       1100)                          \
    ARRAY(char16_t,             mChar16,    9,  0,       1101)                          \
    ARRAY(char32_t,             mChar32,    10, 0,       1102)                          \
    /* 整数型 */                                                                        \
    ARRAY(short,                mShort,     5,  0,      -2200)                          \
    ARRAY(unsigned short,       mUShort,    6,  0,       2200)                          \
    ARRAY(int,                  mInt,       7,  0,      -3200)                          \
    ARRAY(unsigned int,         mUInt,      8,  0U,      3200U)                         \
    ARRAY(long,                 mLong,      9,  0L,     -4200L)                         \
    ARRAY(unsigned long,        mULong,     10, 0UL,     4200UL)                        \
    ARRAY(long long,            mLongLong,  11, 0LL,    -5200LL)                        \
    ARRAY(unsigned long long,   mULongLong, 12, 0ULL,    5200ULL)                       \
    /* 浮動小数点型 */                                                                  \
    ARRAY(float,                mFloat,     5,  0.0F,   3.23456F)                       \
    ARRAY(double,               mDouble,    6,  0.0,    3.23456789012345)               \
    ARRAY(long double,          mLongDouble,7,  0.0L,   3.23456789012345L)              \
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

// ---------------------------------------------------------------------------
//      非侵入型完全自動
// ---------------------------------------------------------------------------

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
                    for (std::size_t k=0; k < 2; ++k)       \
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
    void checkPrivate(bool isValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar##Private, ((!isValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1Private[i], ((!isValued)?dDef:dVal));\
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2Private[j][i], ((!isValued)?dDef:dVal));\
                    for (std::size_t k=0; k < 2; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3Private[k][j][i], ((!isValued)?dDef:dVal));\
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // protectedメンバの値をチェック
    void checkProtected(bool isValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar##Protected, ((!isValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1Protected[i], ((!isValued)?dDef:dVal));\
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2Protected[j][i], ((!isValued)?dDef:dVal));\
                    for (std::size_t k=0; k < 2; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3Protected[k][j][i], ((!isValued)?dDef:dVal));\
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // publicメンバの値をチェック
    void checkPublic(bool isValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar, ((!isValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1[i], ((!isValued)?dDef:dVal));  \
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2[j][i], ((!isValued)?dDef:dVal));\
                    for (std::size_t k=0; k < 2; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3[k][j][i], ((!isValued)?dDef:dVal));\
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
                THEOLIZER_EQUAL(mArrayOnly[i][j].mInt, ((!isValued)?0:kValue));
            }
        }
    }
};

// ---------------------------------------------------------------------------
//      侵入型半自動－名前対応
// ---------------------------------------------------------------------------

//      ---<<< 配列のみで参照される完全自動型 >>>---
//      専用処理が必要なのでテストを設ける

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
    void checkPrivate(bool isValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar##Private, ((!isValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1Private[i], ((!isValued)?dDef:dVal));\
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2Private[j][i], ((!isValued)?dDef:dVal));\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3Private[k][j][i], ((!isValued)?dDef:dVal));\
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // protectedメンバの値をチェック
    void checkProtected(bool isValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar##Protected, ((!isValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1Protected[i], ((!isValued)?dDef:dVal));\
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2Protected[j][i], ((!isValued)?dDef:dVal));\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3Protected[k][j][i], ((!isValued)?dDef:dVal));\
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // publicメンバの値をチェック
    void checkPublic(bool isValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar, ((!isValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1[i], ((!isValued)?dDef:dVal));  \
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2[j][i], ((!isValued)?dDef:dVal));\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3[k][j][i], ((!isValued)?dDef:dVal));\
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
                THEOLIZER_EQUAL(mArrayOnly[i][j].mInt, ((!isValued)?0:kValue));
            }
        }
    }

    // 侵入型半自動 指定
    THEOLIZER_INTRUSIVE(CS, (HalfAutoName), 1);
};

// ---------------------------------------------------------------------------
//      侵入型半自動－順序対応
// ---------------------------------------------------------------------------

//      ---<<< 配列のみで参照される完全自動型 >>>---
//      専用処理が必要なのでテストを設ける

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
    void checkPrivate(bool isValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar##Private, ((!isValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1Private[i], ((!isValued)?dDef:dVal));\
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2Private[j][i], ((!isValued)?dDef:dVal));\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3Private[k][j][i], ((!isValued)?dDef:dVal));\
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // protectedメンバの値をチェック
    void checkProtected(bool isValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar##Protected, ((!isValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1Protected[i], ((!isValued)?dDef:dVal));\
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2Protected[j][i], ((!isValued)?dDef:dVal));\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3Protected[k][j][i], ((!isValued)?dDef:dVal));\
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // publicメンバの値をチェック
    void checkPublic(bool isValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar, ((!isValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1[i], ((!isValued)?dDef:dVal));  \
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2[j][i], ((!isValued)?dDef:dVal));\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3[k][j][i], ((!isValued)?dDef:dVal));\
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
                THEOLIZER_EQUAL(mArrayOnly[i][j].mInt, ((!isValued)?0:kValue));
            }
        }
    }

    // 侵入型半自動 指定
    THEOLIZER_INTRUSIVE_ORDER(CS, (HalfAutoOrder), 1);
};

// ---------------------------------------------------------------------------
//      非侵入型手動
// ---------------------------------------------------------------------------

//      ---<<< 配列のみで参照される完全自動型 >>>---
//      専用処理が必要なのでテストを設ける

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
    void checkPublic(bool isValued=false)
    {
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            THEOLIZER_EQUAL(dVar, ((!isValued)?dDef:dVal));
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(dVar##Array1[i], ((!isValued)?dDef:dVal));  \
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(dVar##Array2[j][i], ((!isValued)?dDef:dVal));\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(dVar##Array3[k][j][i], ((!isValued)?dDef:dVal));\
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
                THEOLIZER_EQUAL(mArrayOnly[i][j].mInt, ((!isValued)?0:kValue));
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
#endif

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

// ---------------------------------------------------------------------------
//      非侵入型完全自動　基底クラス
// ---------------------------------------------------------------------------

//      ---<<< 追加テスト >>>---
//      侵入型を非侵入型へprivate継承するとgccでビルド・エラーになる問題検出用

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

    // デフォルト・コンストラクタ
    BaseFullAuto() : mIntPrivate(0), mIntProtected(0), mIntPublic(0)
    { }

    // 保存前の値設定
    BaseFullAuto(bool) :
        mIntPrivate(kIntPrivate),
        mIntProtected(kIntProtected),
        mIntPublic(kIntPublic)
    { }

    // 一致チェック
    void checkPrivate(bool isValued=false)
    {
        THEOLIZER_EQUAL(mIntPrivate, ((!isValued)?0:kIntPrivate));
    }
    void checkProtected(bool isValued=false)
    {
        THEOLIZER_EQUAL(mIntProtected, ((!isValued)?0:kIntProtected));
    }
    void checkPublic(bool isValued=false)
    {
        THEOLIZER_EQUAL(mIntPublic, ((!isValued)?0:kIntPublic));
    }
};

// ---------------------------------------------------------------------------
//      侵入型半自動　基底クラス
// ---------------------------------------------------------------------------

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

    // 保存前の値設定
    BaseHalfAuto(bool) :
        mIntPrivate(kIntPrivate),
        mIntProtected(kIntProtected),
        mIntPublic(kIntPublic)
    { }

    // 一致チェック
    void checkPrivate(bool isValued=false)
    {
        THEOLIZER_EQUAL(mIntPrivate, ((!isValued)?0:kIntPrivate));
    }
    void checkProtected(bool isValued=false)
    {
        THEOLIZER_EQUAL(mIntProtected, ((!isValued)?0:kIntProtected));
    }
    void checkPublic(bool isValued=false)
    {
        THEOLIZER_EQUAL(mIntPublic, ((!isValued)?0:kIntPublic));
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

// ---------------------------------------------------------------------------
//      非侵入型手動　基底クラス
// ---------------------------------------------------------------------------

//      ---<<< 追加テスト >>>---
//      侵入型を非侵入型へprotected継承するとgccでビルド・エラーになる問題検出用

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

    // デフォルト・コンストラクタ
    BaseManual() : mIntPublic(0)
    { }

    // 保存前の値設定
    BaseManual(bool) :
        mIntPublic(kIntPublic)
    { }

    // 一致チェック
    void checkPublic(bool isValued=false)
    {
        THEOLIZER_EQUAL(mIntPublic, ((!isValued)?0:kIntPublic));
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
        if (!oInstance) oInstance=new typename tTheolizerVersion::TheolizerTarget();

        THEOLIZER_PROCESS(iSerializer, oInstance->mIntPublic);
    }
};

// ***************************************************************************
//      ２重組み合わせテスト
// ***************************************************************************

// ---------------------------------------------------------------------------
//      非侵入型完全自動
// ---------------------------------------------------------------------------

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

    DerivedFullAuto() :
        BaseFullAuto<1111>{},
        BaseHalfAuto<1121>{},
        BaseManual  <1131>{},
        BaseFullAuto<1211>{},
        BaseHalfAuto<1221>{},
        BaseManual  <1231>{},
        BaseFullAuto<1311>{},
        BaseHalfAuto<1321>{},
        BaseManual  <1331>{},
        mBaseFullAutoPrivate{},
        mBaseHalfAutoPrivate{},
        mBaseManualPrivate{},
        mBaseFullAutoProtected{},
        mBaseHalfAutoProtected{},
        mBaseManualProtected{},
        mBaseFullAutoPublic{},
        mBaseHalfAutoPublic{},
        mBaseManualPublic{}
    { }

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

    void check(bool iIsPrivate=false)
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
        BaseFullAuto<1211>::checkProtected(!iIsPrivate);
        BaseFullAuto<1211>::checkPublic(!iIsPrivate);
        BaseHalfAuto<1221>::checkPrivate(!iIsPrivate);
        BaseHalfAuto<1221>::checkProtected(!iIsPrivate);
        BaseHalfAuto<1221>::checkPublic(!iIsPrivate);
        BaseManual  <1231>::checkPublic(!iIsPrivate);

        // public継承
        BaseFullAuto<1311>::checkPrivate(false);            // 非侵入型のprivateは保存無し
        BaseFullAuto<1311>::checkProtected(!iIsPrivate);
        BaseFullAuto<1311>::checkPublic(!iIsPrivate);
        BaseHalfAuto<1321>::checkPrivate(!iIsPrivate);
        BaseHalfAuto<1321>::checkProtected(!iIsPrivate);
        BaseHalfAuto<1321>::checkPublic(!iIsPrivate);
        BaseManual  <1331>::checkPublic(!iIsPrivate);

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
        mBaseFullAutoProtected.checkProtected(!iIsPrivate);
        mBaseFullAutoProtected.checkPublic(!iIsPrivate);
        mBaseHalfAutoProtected.checkPrivate(!iIsPrivate);
        mBaseHalfAutoProtected.checkProtected(!iIsPrivate);
        mBaseHalfAutoProtected.checkPublic(!iIsPrivate);
        mBaseManualProtected.checkPublic(!iIsPrivate);

        // publicメンバ
        mBaseFullAutoPublic.checkPrivate(false);            // 非侵入型のprivateは保存無し
        mBaseFullAutoPublic.checkProtected(!iIsPrivate);
        mBaseFullAutoPublic.checkPublic(!iIsPrivate);
        mBaseHalfAutoPublic.checkPrivate(!iIsPrivate);
        mBaseHalfAutoPublic.checkProtected(!iIsPrivate);
        mBaseHalfAutoPublic.checkPublic(!iIsPrivate);
        mBaseManualPublic.checkPublic(!iIsPrivate);
    }
};

// ---------------------------------------------------------------------------
//      侵入型半自動
// ---------------------------------------------------------------------------

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

    DerivedHalfAuto() :
        BaseFullAuto<2111>{},
        BaseHalfAuto<2121>{},
        BaseManual  <2131>{},
        BaseFullAuto<2211>{},
        BaseHalfAuto<2221>{},
        BaseManual  <2231>{},
        BaseFullAuto<2311>{},
        BaseHalfAuto<2321>{},
        BaseManual  <2331>{},
        mBaseFullAutoPrivate{},
        mBaseHalfAutoPrivate{},
        mBaseManualPrivate{},
        mBaseFullAutoProtected{},
        mBaseHalfAutoProtected{},
        mBaseManualProtected{},
        mBaseFullAutoPublic{},
        mBaseHalfAutoPublic{},
        mBaseManualPublic{}
    { }

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

    void check(bool iIsPrivate=false)
    {
        // private継承
        BaseFullAuto<2111>::checkPrivate(false);            // 非侵入型のprivateは保存無し
        BaseFullAuto<2111>::checkProtected(!iIsPrivate);
        BaseFullAuto<2111>::checkPublic(!iIsPrivate);
        BaseHalfAuto<2121>::checkPrivate(!iIsPrivate);
        BaseHalfAuto<2121>::checkProtected(!iIsPrivate);
        BaseHalfAuto<2121>::checkPublic(!iIsPrivate);
        BaseManual  <2131>::checkPublic(!iIsPrivate);

        // protected継承
        BaseFullAuto<2211>::checkPrivate(false);            // 非侵入型のprivateは保存無し
        BaseFullAuto<2211>::checkProtected(!iIsPrivate);
        BaseFullAuto<2211>::checkPublic(!iIsPrivate);
        BaseHalfAuto<2221>::checkPrivate(!iIsPrivate);
        BaseHalfAuto<2221>::checkProtected(!iIsPrivate);
        BaseHalfAuto<2221>::checkPublic(!iIsPrivate);
        BaseManual  <2231>::checkPublic(!iIsPrivate);

        // public継承
        BaseFullAuto<2311>::checkPrivate(false);            // 非侵入型のprivateは保存無し
        BaseFullAuto<2311>::checkProtected(!iIsPrivate);
        BaseFullAuto<2311>::checkPublic(!iIsPrivate);
        BaseHalfAuto<2321>::checkPrivate(!iIsPrivate);
        BaseHalfAuto<2321>::checkProtected(!iIsPrivate);
        BaseHalfAuto<2321>::checkPublic(!iIsPrivate);
        BaseManual  <2331>::checkPublic(!iIsPrivate);

        // privateメンバ
        mBaseFullAutoPrivate.checkPrivate(false);           // 非侵入型のprivateは保存無し
        mBaseFullAutoPrivate.checkProtected(!iIsPrivate);
        mBaseFullAutoPrivate.checkPublic(!iIsPrivate);
        mBaseHalfAutoPrivate.checkPrivate(!iIsPrivate);
        mBaseHalfAutoPrivate.checkProtected(!iIsPrivate);
        mBaseHalfAutoPrivate.checkPublic(!iIsPrivate);
        mBaseManualPrivate.checkPublic(!iIsPrivate);

        // protectedメンバ
        mBaseFullAutoProtected.checkPrivate(false);         // 非侵入型のprivateは保存無し
        mBaseFullAutoProtected.checkProtected(!iIsPrivate);
        mBaseFullAutoProtected.checkPublic(!iIsPrivate);
        mBaseHalfAutoProtected.checkPrivate(!iIsPrivate);
        mBaseHalfAutoProtected.checkProtected(!iIsPrivate);
        mBaseHalfAutoProtected.checkPublic(!iIsPrivate);
        mBaseManualProtected.checkPublic(!iIsPrivate);

        // publicメンバ
        mBaseFullAutoPublic.checkPrivate(false);            // 非侵入型のprivateは保存無し
        mBaseFullAutoPublic.checkProtected(!iIsPrivate);
        mBaseFullAutoPublic.checkPublic(!iIsPrivate);
        mBaseHalfAutoPublic.checkPrivate(!iIsPrivate);
        mBaseHalfAutoPublic.checkProtected(!iIsPrivate);
        mBaseHalfAutoPublic.checkPublic(!iIsPrivate);
        mBaseManualPublic.checkPublic(!iIsPrivate);
    }

    // 侵入型半自動 指定
    THEOLIZER_INTRUSIVE(CS, (DerivedHalfAuto), 1);
};

// ---------------------------------------------------------------------------
//      非侵入型手動
// ---------------------------------------------------------------------------

class DerivedManual :
    public      BaseFullAuto<3311>,
    public      BaseHalfAuto<3321>,
    public      BaseManual  <3331>
{
public:
    BaseFullAuto<3611>  mBaseFullAutoPublic;
    BaseHalfAuto<3621>  mBaseHalfAutoPublic;
    BaseManual  <3631>  mBaseManualPublic;

    DerivedManual() :
        BaseFullAuto{},
        BaseHalfAuto{},
        BaseManual{},
        mBaseFullAutoPublic{},
        mBaseHalfAutoPublic{},
        mBaseManualPublic{}
    { }

    DerivedManual(bool) :
        BaseFullAuto{true},
        BaseHalfAuto{true},
        BaseManual{true},
        mBaseFullAutoPublic{true},
        mBaseHalfAutoPublic{true},
        mBaseManualPublic{true}
    { }

    void check(bool iIsPrivate=false)
    {
        // public継承
        BaseFullAuto::checkPrivate(false);              // 完全自動のprivateは保存無し
        BaseFullAuto::checkProtected(!iIsPrivate);
        BaseFullAuto::checkPublic(!iIsPrivate);
        BaseHalfAuto::checkPrivate(!iIsPrivate);
        BaseHalfAuto::checkProtected(!iIsPrivate);
        BaseHalfAuto::checkPublic(!iIsPrivate);
        BaseManual::checkPublic(!iIsPrivate);

        // publicメンバ
        mBaseFullAutoPublic.checkPrivate(false);        // 完全自動のprivateは保存無し
        mBaseFullAutoPublic.checkProtected(!iIsPrivate);
        mBaseFullAutoPublic.checkPublic(!iIsPrivate);
        mBaseHalfAutoPublic.checkPrivate(!iIsPrivate);
        mBaseHalfAutoPublic.checkProtected(!iIsPrivate);
        mBaseHalfAutoPublic.checkPublic(!iIsPrivate);
        mBaseManualPublic.checkPublic(!iIsPrivate);
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
        THEOLIZER_PROCESS(iSerializer, static_cast<BaseFullAuto const&>(*iInstance));
        THEOLIZER_PROCESS(iSerializer, static_cast<BaseHalfAuto const&>(*iInstance));
        THEOLIZER_PROCESS(iSerializer, static_cast<BaseManual const&>(*iInstance));
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

        THEOLIZER_PROCESS(iSerializer, static_cast<BaseFullAuto&>(*oInstance));
        THEOLIZER_PROCESS(iSerializer, static_cast<BaseHalfAuto&>(*oInstance));
        THEOLIZER_PROCESS(iSerializer, static_cast<BaseManual&>(*oInstance));
        THEOLIZER_PROCESS(iSerializer, oInstance->mBaseFullAutoPublic);
        THEOLIZER_PROCESS(iSerializer, oInstance->mBaseHalfAutoPublic);
        THEOLIZER_PROCESS(iSerializer, oInstance->mBaseManualPublic);
    }
};

// ***************************************************************************
//      ３重組み合わせテスト
// ***************************************************************************

// ---------------------------------------------------------------------------
//      非侵入型完全自動
// ---------------------------------------------------------------------------

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
        // public継承
        DerivedFullAuto::check();
        DerivedHalfAuto::check();
        DerivedManual::check();

        // privateメンバ
        mDerivedFullAutoPrivate.check(true);
        mDerivedHalfAutoPrivate.check(true);
        mDerivedManualPrivate.check(true);

        // protectedメンバ
        mDerivedFullAutoProtected.check();
        mDerivedHalfAutoProtected.check();
        mDerivedManualProtected.check();

        // publicメンバ
        mDerivedFullAutoPublic.check();
        mDerivedHalfAutoPublic.check();
        mDerivedManualPublic.check();
    }
};

// ---------------------------------------------------------------------------
//      侵入型半自動
// ---------------------------------------------------------------------------

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
        // public継承
        DerivedFullAuto::check();
        DerivedHalfAuto::check();
        DerivedManual::check();

        // privateメンバ
        mDerivedFullAutoPrivate.check();
        mDerivedHalfAutoPrivate.check();
        mDerivedManualPrivate.check();   

        // protectedメンバ
        mDerivedFullAutoProtected.check();
        mDerivedHalfAutoProtected.check();
        mDerivedManualProtected.check();

        // publicメンバ
        mDerivedFullAutoPublic.check();
        mDerivedHalfAutoPublic.check();
        mDerivedManualPublic.check();
    }

    // 侵入型半自動 指定
    THEOLIZER_INTRUSIVE(CS, (TripledHalfAuto), 1);
};

// ---------------------------------------------------------------------------
//      非侵入型手動
// ---------------------------------------------------------------------------

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
        // public継承
        DerivedFullAuto::check();
        DerivedHalfAuto::check();
        DerivedManual::check();

        // publicメンバ
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
        THEOLIZER_PROCESS(iSerializer, static_cast<DerivedFullAuto const&>(*iInstance));
        THEOLIZER_PROCESS(iSerializer, static_cast<DerivedHalfAuto const&>(*iInstance));
        THEOLIZER_PROCESS(iSerializer, static_cast<DerivedManual const&>(*iInstance));
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

        THEOLIZER_PROCESS(iSerializer, static_cast<DerivedFullAuto&>(*oInstance));
        THEOLIZER_PROCESS(iSerializer, static_cast<DerivedHalfAuto&>(*oInstance));
        THEOLIZER_PROCESS(iSerializer, static_cast<DerivedManual&>(*oInstance));
        THEOLIZER_PROCESS(iSerializer, oInstance->mDerivedFullAutoPublic);
        THEOLIZER_PROCESS(iSerializer, oInstance->mDerivedHalfAutoPublic);
        THEOLIZER_PROCESS(iSerializer, oInstance->mDerivedManualPublic);
    }
};

// ---------------------------------------------------------------------------
/*
    組み合わせ検討

        全てコンストラクタを２種類
            デフォルト
            値設定(パラメータは1つだけ)

        単独
            非侵入型完全自動
                プリミティブ全部
                enum型          非侵入型完全自動
                scoped enum型   非侵入型完全自動
                上記全ての配列  1次元、2次元、3次元
            侵入型半自動
                上記と同じ      名前対応
                上記と同じ      順序対応
            非侵入型手動
                上記と同じ（test_basic_processとほぼ同様)

        基底クラス
            非侵入型完全自動Base
            侵入型半自動Base
            非侵入型手動Base

        2重組み合わせ
            非侵入型完全自動Derived
                非侵入型完全自動Base
                    基底クラス
                    包含クラス
                侵入型半自動Base
                    基底クラス
                    包含クラス
                非侵入型手動Base
                    基底クラス
                    包含クラス
            侵入型半自動Derived
                非侵入型完全自動Base
                    基底クラス
                    包含クラス
                侵入型半自動Base
                    基底クラス
                    包含クラス
                非侵入型手動Base
                    基底クラス
                    包含クラス
            非侵入型手動Derived
                非侵入型完全自動Base
                    基底クラス
                    包含クラス
                侵入型半自動Base
                    基底クラス
                    包含クラス
                非侵入型手動Base
                    基底クラス
                    包含クラス

        3重組み合わせ
            非侵入型完全自動Tripled
                非侵入型完全自動Derived
                    基底クラス
                    包含クラス
                侵入型半自動Derived
                    基底クラス
                    包含クラス
                非侵入型手動Derived
                    基底クラス
                    包含クラス
            侵入型半自動Tripled
                非侵入型完全自動Derived
                    基底クラス
                    包含クラス
                侵入型半自動Derived
                    基底クラス
                    包含クラス
                非侵入型手動Derived
                    基底クラス
                    包含クラス
            非侵入型手動Tripled
                非侵入型完全自動Derived
                    基底クラス
                    包含クラス
                侵入型半自動Derived
                    基底クラス
                    包含クラス
                非侵入型手動Derived
                    基底クラス
                    包含クラス
*/


#endif  // TEST_CLASS_VARIATION_H
