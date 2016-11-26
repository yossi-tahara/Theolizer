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

// ***************************************************************************
//      クラス
// ***************************************************************************

// ---------------------------------------------------------------------------
//      基底クラス　非侵入型完全自動
// ---------------------------------------------------------------------------

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
//      非侵入型完全自動（基底クラス）
// ---------------------------------------------------------------------------

//      ---<<< 配列のみで参照される完全自動型 >>>---
//      専用処理が必要なのでテストを設ける

struct ArrayOnly0
{
    int     mInt;
    ArrayOnly0() : mInt(0) { }
};

//      ---<<< 本体 >>>---

class BaseFullAuto
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
    static const int            kValue=123;
    ArrayOnly0  mArrayOnly[kRow][kCol];

    // デフォルト・コンストラクタ
    BaseFullAuto() :
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
    BaseFullAuto(bool) :
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
//      侵入型半自動（基底クラス）
// ---------------------------------------------------------------------------

//      ---<<< 配列のみで参照される完全自動型 >>>---
//      専用処理が必要なのでテストを設ける

struct ArrayOnly1
{
    int     mInt;
    ArrayOnly1() : mInt(0) { }
};

//      ---<<< 本体 >>>---

class BaseHalfAuto
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
    static const int            kValue=321;
    ArrayOnly1  mArrayOnly[kRow][kCol];

    // デフォルト・コンストラクタ
    BaseHalfAuto() :
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
    BaseHalfAuto(bool) :
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

    // 侵入型半自動 指定
    THEOLIZER_INTRUSIVE(CS, (BaseHalfAuto), 1);
};

// ---------------------------------------------------------------------------
/*
    組み合わせ検討

        全てコンストラクタを２種類
            デフォルト
            値設定(パラメータは1つだけ)

        単独
            非侵入型完全自動Base
                プリミティブ全部
                enum型          非侵入型完全自動
                scoped enum型   非侵入型完全自動
                上記全ての配列  1次元、2次元、3次元
            侵入型半自動Base
                上記と同じ
            非侵入型手動Base
                上記と同じ（test_basic_processとほぼ同様)

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
