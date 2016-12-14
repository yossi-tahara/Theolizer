//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          オブジェクト追跡
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

#if !defined(TEST_OBJECT_TRACKING_H)
#define TEST_OBJECT_TRACKING_H

#include <string>

// 基本のenum型とclassを使用する
#include "test_basic_process.h"

// ***************************************************************************
//      使い方の説明
// ***************************************************************************

//----------------------------------------------------------------------------
//      
//----------------------------------------------------------------------------

// ***************************************************************************
//      組み合わせテスト
// ***************************************************************************

//----------------------------------------------------------------------------
//      ボイント先変数リスト
//          下記へ展開する
//              (1)ポイント先変数
//              (2)(1)へのポインタ変数
//              (3)(2)の保存／回復
//              (4)回復したポインタ変数のアドレス値のチェックとポイント先の値のチェック
//----------------------------------------------------------------------------

#define DEFINE_MEMBERS()                                                                \
    /* 文字型 */                                                                        \
    DEFINE(char,                Char,           0,       110)                           \
    DEFINE(signed char,         SChar,          0,      -111)                           \
    DEFINE(unsigned char,       UChar,          0,       112)                           \
    DEFINE(wchar_t,             Wchar,          0,       1100)                          \
    DEFINE(char16_t,            Char16,         0,       1101)                          \
    DEFINE(char32_t,            Char32,         0,       1102)                          \
    /* 整数型 */                                                                        \
    DEFINE(short,               Short,          0,      -2100)                          \
    DEFINE(unsigned short,      UShort,         0,       2100)                          \
    DEFINE(int,                 Int,            0,      -3100)                          \
    DEFINE(unsigned int,        UInt,           0U,      3100U)                         \
    DEFINE(long,                Long,           0L,     -4100L)                         \
    DEFINE(unsigned long,       ULong,          0UL,     4100UL)                        \
    DEFINE(long long,           LongLong,       0LL,    -5100LL)                        \
    DEFINE(unsigned long long,  ULongLong,      0ULL,    5100ULL)                       \
    /* 浮動小数点型 */                                                                  \
    DEFINE(float,               Float,          0.0F,   2.23456F)                       \
    DEFINE(double,              Double,         0.0,    2.23456789012345)               \
    DEFINE(long double,         LongDouble,     0.0L,   2.23456789012345L)              \
    /* 文字列型 */                                                                      \
    DEFINE(std::string,         String,         u8"",   u8"ＵＴＦ－８a")                \
    DEFINE(std::wstring,        Wstring,        L"",    L"ＵＴＦ－１６／３２a")         \
    DEFINE(std::u16string,      U16string,      u"",    u"ＵＴＦ－１６a")               \
    DEFINE(std::u32string,      U32string,      U"",    U"ＵＴＦ－３２a")               \
    /* enum型 */                                                                        \
    DEFINE(NormalEnum,  NormalEnum,     eneZero,            eneOne)                     \
    DEFINE(ScopedEnum,  ScopedEnum,     ScopedEnum::ZERO,   ScopedEnum::TWO)            \
    /* クラス */                                                                        \
    DEFINE(ClassBasicTest, ClassBasicTest,                                              \
           ClassBasicTest(), ClassBasicTest(1, "1", eneOne))                            \
    /* --- 配列 --- */                                                                  \
    /* 文字型 */                                                                        \
    ARRAY(char,                 Char,       5,  0,       120)                           \
    ARRAY(signed char,          SChar,      6,  0,      -121)                           \
    ARRAY(unsigned char,        UChar,      7,  0,       112)                           \
    ARRAY(wchar_t,              Wchar,      8,  0,       1100)                          \
    ARRAY(char16_t,             Char16,     9,  0,       1101)                          \
    ARRAY(char32_t,             Char32,     10, 0,       1102)                          \
    /* 整数型 */                                                                        \
    ARRAY(short,                Short,      5,  0,      -2200)                          \
    ARRAY(unsigned short,       UShort,     6,  0,       2200)                          \
    ARRAY(int,                  Int,        7,  0,      -3200)                          \
    ARRAY(unsigned int,         UInt,       8,  0U,      3200U)                         \
    ARRAY(long,                 Long,       9,  0L,     -4200L)                         \
    ARRAY(unsigned long,        ULong,      10, 0UL,     4200UL)                        \
    ARRAY(long long,            LongLong,   11, 0LL,    -5200LL)                        \
    ARRAY(unsigned long long,   ULongLong,  12, 0ULL,    5200ULL)                       \
    /* 浮動小数点型 */                                                                  \
    ARRAY(float,                Float,      5,  0.0F,   3.23456F)                       \
    ARRAY(double,               Double,     6,  0.0,    3.23456789012345)               \
    ARRAY(long double,          LongDouble, 7,  0.0L,   3.23456789012345L)              \
    /* 文字列型 */                                                                      \
    ARRAY(std::string,          String,     5,  u8"",   u8"ＵＴＦ－８a")                \
    ARRAY(std::wstring,         Wstring,    6,  L"",    L"ＵＴＦ－１６／３２a")         \
    ARRAY(std::u16string,       U16string,  7,  u"",    u"ＵＴＦ－１６a")               \
    ARRAY(std::u32string,       U32string,  8,  U"",    U"ＵＴＦ－３２a")               \
    /* enum型 */                                                                        \
    ARRAY(NormalEnum,       NormalEnum,     5,  eneZero,            eneOne)             \
    ARRAY(ScopedEnum,       ScopedEnum,     6,  ScopedEnum::ZERO,   ScopedEnum::TWO)    \
    /* クラス */                                                                        \
    ARRAY(ClassBasicTest, ClassBasicTest,   5,                                          \
           ClassBasicTest(), ClassBasicTest(1, "1", eneOne))

//----------------------------------------------------------------------------
//              通常のポインタ
//----------------------------------------------------------------------------

//      ---<<< ポイント先の定義 >>>---
//      テストの実装を手軽に行うために構造体に纏めた

struct PointeeList
{
    #define DEFINE(dType, dVar, dDef, dVal)                                 \
        dType m##dVar THEOLIZER_ANNOTATE(FS:<>Pointee);
    #define ARRAY(dType, dVar, dNum, dDef, dVal)                            \
        dType m##dVar##Array1[dNum] THEOLIZER_ANNOTATE(FS:<>Pointee);       \
        dType m##dVar##Array2[2][dNum] THEOLIZER_ANNOTATE(FS:<>Pointee);    \
        dType m##dVar##Array3[3][2][dNum] THEOLIZER_ANNOTATE(FS:<>Pointee);
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE

    int mDummy;

    // デフォルト・コンストラクタ
    PointeeList() :
        #define DEFINE(dType, dVar, dDef, dVal) m##dVar(dDef),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            m##dVar##Array1{},                      \
            m##dVar##Array2{},                      \
            m##dVar##Array3{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mDummy(0)
    { }

    // 保存前の値設定
    PointeeList(bool) :
        #define DEFINE(dType, dVar, dDef, dVal) m##dVar(dVal),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)\
            m##dVar##Array1{},                      \
            m##dVar##Array2{},                      \
            m##dVar##Array3{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mDummy(0)
    {
        #define DEFINE(dType, dVar, dDef, dVal)
        #define ARRAY(dType, dVar, dNum, dDef, dVal)        \
            for (std::size_t i=0; i < dNum; ++i)            \
            {                                               \
                m##dVar##Array1[i]=dVal;                    \
                for (std::size_t j=0; j < 2; ++j)           \
                {                                           \
                    m##dVar##Array2[j][i]=dVal;             \
                    for (std::size_t k=0; k < 3; ++k)       \
                    {                                       \
                        m##dVar##Array3[k][j][i]=dVal;      \
                    }                                       \
                }                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // 値確認（dVal側と等しいことをチェックする)
    void check()
    {
    #define DEFINE(dType, dVar, dDef, dVal)                                 \
        THEOLIZER_EQUAL(m##dVar, dVal);
    #define ARRAY(dType, dVar, dNum, dDef, dVal)                            \
        for (std::size_t i=0; i < dNum; ++i)                                \
        {                                                                   \
            THEOLIZER_EQUAL(m##dVar##Array1[i], dVal);                      \
            for (std::size_t j=0; j < 2; ++j)                               \
            {                                                               \
                THEOLIZER_EQUAL(m##dVar##Array2[j][i], dVal);               \
                for (std::size_t k=0; k < 3; ++k)                           \
                {                                                           \
                    THEOLIZER_EQUAL(m##dVar##Array3[k][j][i], dVal);        \
                }                                                           \
            }                                                               \
        }
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE
    }
};

//      ---<<< ポインタの定義 >>>---
//      テストの実装を手軽に行うために構造体に纏めた

struct PointerList
{
    #define DEFINE(dType, dVar, dDef, dVal)                                 \
        dType const* m##dVar;
    #define ARRAY(dType, dVar, dNum, dDef, dVal)                            \
        dType const* m##dVar##Array1[dNum];                                 \
        dType const* m##dVar##Array2[2][dNum];                              \
        dType const* m##dVar##Array3[3][2][dNum];
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE

    int mDummy;

    // デフォルト・コンストラクタ
    PointerList() :
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            m##dVar(reinterpret_cast<dType*>(0xffff)),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            m##dVar##Array1{},                                              \
            m##dVar##Array2{},                                              \
            m##dVar##Array3{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mDummy(0)
    {
        #define DEFINE(dType, dVar, dDef, dVal)
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                m##dVar##Array1[i]=reinterpret_cast<dType*>(0xffff);        \
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    m##dVar##Array2[j][i]=reinterpret_cast<dType*>(0xffff); \
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        m##dVar##Array3[k][j][i]=reinterpret_cast<dType*>(0xffff);\
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // 保存前の値設定
    PointerList(PointeeList const& iPointeeList) :
        #define DEFINE(dType, dVar, dDef, dVal)                             \
            m##dVar(&iPointeeList.m##dVar),
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            m##dVar##Array1{},                                              \
            m##dVar##Array2{},                                              \
            m##dVar##Array3{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mDummy(0)
    {
        #define DEFINE(dType, dVar, dDef, dVal)
        #define ARRAY(dType, dVar, dNum, dDef, dVal)                        \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                m##dVar##Array1[i]=&iPointeeList.m##dVar##Array1[i];        \
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    m##dVar##Array2[j][i]=&iPointeeList.m##dVar##Array2[j][i];\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        m##dVar##Array3[k][j][i]=&iPointeeList.m##dVar##Array3[k][j][i];\
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // 値確認（dVal側と等しいことをチェックする)
    void check(PointeeList const& iPointeeList)
    {
    #define CHECK_EQUAL(dLhs, dRhs)                                         \
        THEOLIZER_EQUAL_PTR(dLhs, dRhs);                                    \
        if (dLhs)                                                           \
        {                                                                   \
            THEOLIZER_EQUAL(*dLhs,  *dRhs);                                 \
        }

    #define DEFINE(dType, dVar, dDef, dVal)                                 \
        CHECK_EQUAL(m##dVar, &iPointeeList.m##dVar);
    #define ARRAY(dType, dVar, dNum, dDef, dVal)                            \
        for (std::size_t i=0; i < dNum; ++i)                                \
        {                                                                   \
            CHECK_EQUAL(m##dVar##Array1[i], &iPointeeList.m##dVar##Array1[i]);\
            for (std::size_t j=0; j < 2; ++j)                               \
            {                                                               \
                CHECK_EQUAL(m##dVar##Array2[j][i], &iPointeeList.m##dVar##Array2[j][i]);\
                for (std::size_t k=0; k < 3; ++k)                           \
                {                                                           \
                    CHECK_EQUAL(m##dVar##Array3[k][j][i], &iPointeeList.m##dVar##Array3[k][j][i]);\
                }                                                           \
            }                                                               \
        }
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE
    }
};

//      ---<<< 処理関数 >>>---
//      手動(トップ・レベル)によるポインタの保存／回復

// 保存
template<class tSerializer>
inline void savePointer(tSerializer& iSerializer, PointerList& iPointerList)
{
    // ポインタ変数を保存する
    #define DEFINE(dType, dVar, dDef, dVal)                                 \
      THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar);
    #define ARRAY(dType, dVar, dNum, dDef, dVal)                            \
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Array1);       \
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Array2);       \
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Array3);
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE
}

// 回復
template<class tSerializer>
inline void loadPointer(tSerializer& iSerializer, PointerList& iPointerList)
{
    // ポインタ変数を回復する
    #define DEFINE(dType, dVar, dDef, dVal)                                 \
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar);
    #define ARRAY(dType, dVar, dNum, dDef, dVal)                            \
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Array1);       \
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Array2);       \
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Array3);
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE
}

//      ---<<< 自動処理の検証 >>>---
//      自動による保存／回復

#if 0
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
#endif

//----------------------------------------------------------------------------
//              オーナー指定ポインタ
//----------------------------------------------------------------------------

#endif  // TEST_OBJECT_TRACKING_H
