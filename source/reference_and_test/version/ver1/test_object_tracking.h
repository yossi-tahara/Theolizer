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
//      組み合わせテスト共通
// ***************************************************************************

//----------------------------------------------------------------------------
//      ボイント先変数リスト
//          下記へ展開する
//              (1)ポイント先変数
//              (2)(1)へのポインタ変数
//              (3)(2)の保存／回復
//              (4)回復したポインタ変数のアドレス値のチェックとポイント先の値のチェック
//----------------------------------------------------------------------------

#ifndef SHRINK_FOR_MINGW64          // MinGWx64におけるFile too big回避用

#define DEFINE_MEMBERS()                                                                \
    /* 文字型 */                                                                        \
    DEFINE(char,                Char,           0,       10,      20)                   \
    DEFINE(signed char,         SChar,          0,      -11,      21)                   \
    DEFINE(unsigned char,       UChar,          0,       12,      22)                   \
    DEFINE(wchar_t,             Wchar,          0,       1100,    1200)                 \
    DEFINE(char16_t,            Char16,         0,       1101,    1201)                 \
    DEFINE(char32_t,            Char32,         0,       1102,    1202)                 \
    /* 整数型 */                                                                        \
    DEFINE(short,               Short,          0,      -2100,   -2200)                 \
    DEFINE(unsigned short,      UShort,         0,       2100,    2200)                 \
    DEFINE(int,                 Int,            0,      -3100,   -3200)                 \
    DEFINE(unsigned int,        UInt,           0U,      3100U,   3200U)                \
    DEFINE(long,                Long,           0L,     -4100L,  -4200L)                \
    DEFINE(unsigned long,       ULong,          0UL,     4100UL,  3200U)                \
    DEFINE(long long,           LongLong,       0LL,    -5100LL, -4200L)                \
    DEFINE(unsigned long long,  ULongLong,      0ULL,    5100ULL, 5200ULL)              \
    /* 浮動小数点型 */                                                                  \
    DEFINE(float,               Float,          0.0F,   2.23456F, 3.23456F)             \
    DEFINE(double,              Double,         0.0,    2.23456789012345,  3.23456789012345)\
    DEFINE(long double,         LongDouble,     0.0L,   2.23456789012345L, 3.23456789012345L)\
    /* 文字列型 */                                                                      \
    DEFINE(std::string,         String,         u8"",   u8"ＵＴＦ８a", u8"ＵＴＦ８b")   \
    DEFINE(std::wstring,        Wstring,        L"",    L"ＵＴＦ１６/３２a", L"ＵＴＦ１６/３２b")\
    DEFINE(std::u16string,      U16string,      u"",    u"ＵＴＦ１６a", u"ＵＴＦ１６b") \
    DEFINE(std::u32string,      U32string,      U"",    U"ＵＴＦ３２a", U"ＵＴＦ３２b") \
    /* enum型 */                                                                        \
    DEFINE(NormalEnum,  NormalEnum,     eneZero,          eneOne,          eneTwo)      \
    DEFINE(ScopedEnum,  ScopedEnum,     ScopedEnum::ZERO, ScopedEnum::ONE, ScopedEnum::TWO)\
    /* クラス */                                                                        \
    DEFINE(ClassBasicTest, ClassBasicTest,                                              \
           ClassBasicTest(), ClassBasicTest(1, "1", eneOne), ClassBasicTest(2, "2", eneTwo))\
    /* --- 配列 --- */                                                                  \
    /* 文字型 */                                                                        \
    ARRAY(char,                 Char,       5,  0,       30,      40)                   \
    ARRAY(signed char,          SChar,      6,  0,      -31,      41)                   \
    ARRAY(unsigned char,        UChar,      7,  0,       32,      42)                   \
    ARRAY(wchar_t,              Wchar,      8,  0,       1300,    1400)                 \
    ARRAY(char16_t,             Char16,     9,  0,       1301,    1401)                 \
    ARRAY(char32_t,             Char32,     10, 0,       1302,    1402)                 \
    /* 整数型 */                                                                        \
    ARRAY(short,                Short,      5,  0,      -2300,   -2400)                 \
    ARRAY(unsigned short,       UShort,     6,  0,       2300,    2400)                 \
    ARRAY(int,                  Int,        7,  0,      -3300,   -3400)                 \
    ARRAY(unsigned int,         UInt,       8,  0U,      3300U,   3400U)                \
    ARRAY(long,                 Long,       9,  0L,     -4300L,  -4400L)                \
    ARRAY(unsigned long,        ULong,      10, 0UL,     4300UL,  3400U)                \
    ARRAY(long long,            LongLong,   11, 0LL,    -5300LL, -4400L)                \
    ARRAY(unsigned long long,   ULongLong,  12, 0ULL,    5300ULL, 5400ULL)              \
    /* 浮動小数点型 */                                                                  \
    ARRAY(float,                Float,      5,  0.0F,   4.23456F, 5.23456F)             \
    ARRAY(double,               Double,     6,  0.0,    4.23456789012345,  5.23456789012345)\
    ARRAY(long double,          LongDouble, 7,  0.0L,   4.23456789012345L, 5.23456789012345L)\
    /* 文字列型 */                                                                      \
    ARRAY(std::string,          String,     5,  u8"",   u8"ＵＴＦ８c", u8"ＵＴＦd")     \
    ARRAY(std::wstring,         Wstring,    6,  L"",    L"ＵＴＦ１６/３２c", L"ＵＴＦ１６/３２d")\
    ARRAY(std::u16string,       U16string,  7,  u"",    u"ＵＴＦ１６c", u"ＵＴＦ１６d") \
    ARRAY(std::u32string,       U32string,  8,  U"",    U"ＵＴＦ３２c", U"ＵＴＦ３２d") \
    /* enum型 */                                                                        \
    ARRAY(NormalEnum,       NormalEnum,     5,  eneZero,         eneOne,         eneTwo)\
    ARRAY(ScopedEnum,       ScopedEnum,     6,  ScopedEnum::ZERO,ScopedEnum::ONE,ScopedEnum::TWO)\
    /* クラス */                                                                        \
    ARRAY(ClassBasicTest, ClassBasicTest,   5,                                          \
           ClassBasicTest(), ClassBasicTest(1, "1", eneOne), ClassBasicTest(2, "2", eneTwo))

#else

#define DEFINE_MEMBERS()                                                                \
    /* 文字型 */                                                                        \
    DEFINE(char,                Char,           0,       10,      20)                   \
    /* 整数型 */                                                                        \
    DEFINE(int,                 Int,            0,      -3100,   -3200)                 \
    /* 浮動小数点型 */                                                                  \
    DEFINE(double,              Double,         0.0,    2.23456789012345,  3.23456789012345)\
    /* 文字列型 */                                                                      \
    DEFINE(std::string,         String,         u8"",   u8"ＵＴＦ８a", u8"ＵＴＦ８b")   \
    /* enum型 */                                                                        \
    DEFINE(NormalEnum,  NormalEnum,     eneZero,          eneOne,          eneTwo)      \
    /* クラス */                                                                        \
    DEFINE(ClassBasicTest, ClassBasicTest,                                              \
           ClassBasicTest(), ClassBasicTest(1, "1", eneOne), ClassBasicTest(2, "2", eneTwo))\
    /* --- 配列 --- */                                                                  \
    /* 文字型 */                                                                        \
    ARRAY(wchar_t,              Wchar,      8,  0,       1300,    1400)                 \
    /* 整数型 */                                                                        \
    ARRAY(unsigned int,         UInt,       8,  0U,      3300U,   3400U)                \
    /* 浮動小数点型 */                                                                  \
    ARRAY(float,                Float,      5,  0.0F,   4.23456F, 5.23456F)             \
    /* 文字列型 */                                                                      \
    ARRAY(std::wstring,         Wstring,    6,  L"",    L"ＵＴＦ１６/３２c", L"ＵＴＦ１６/３２d")\
    /* enum型 */                                                                        \
    ARRAY(ScopedEnum,       ScopedEnum,     6,  ScopedEnum::ZERO,ScopedEnum::ONE,ScopedEnum::TWO)\
    /* クラス */                                                                        \
    ARRAY(ClassBasicTest, ClassBasicTest,   5,                                          \
           ClassBasicTest(), ClassBasicTest(1, "1", eneOne), ClassBasicTest(2, "2", eneTwo))

#endif

#define DEFAULT_PTR(dType)  reinterpret_cast<dType*>(0xffff)

// ***************************************************************************
//      通常のポインタのテスト
// ***************************************************************************

//----------------------------------------------------------------------------
//      ポイント先の定義
//          テストの実装を手軽に行うために構造体に纏めた
//----------------------------------------------------------------------------

//      ---<<< 定義本体 >>>---

struct PointeeList
{
    #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                        \
        dType m##dVar THEOLIZER_ANNOTATE(FS:<>Pointee);
    #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)                   \
        dType m##dVar##Array1[dNum] THEOLIZER_ANNOTATE(FS:<>Pointee);       \
        dType m##dVar##Array2[2][dNum] THEOLIZER_ANNOTATE(FS:<>Pointee);    \
        dType m##dVar##Array3[3][2][dNum] THEOLIZER_ANNOTATE(FS:<>Pointee);
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE

    int mDummy;

    // デフォルト・コンストラクタ
    PointeeList() :
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                    \
            m##dVar{dVal0},
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            m##dVar##Array1{},                                              \
            m##dVar##Array2{},                                              \
            m##dVar##Array3{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mDummy(0)
    { }

    // 保存前の値設定1
    PointeeList(bool) :
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                    \
            m##dVar{dVal1},
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            m##dVar##Array1{},                                              \
            m##dVar##Array2{},                                              \
            m##dVar##Array3{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mDummy(0)
    {
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                m##dVar##Array1[i]=dVal1;                                   \
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    m##dVar##Array2[j][i]=dVal1;                            \
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        m##dVar##Array3[k][j][i]=dVal1;                     \
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // 保存前の値設定2
    PointeeList(int) :
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                    \
            m##dVar{dVal2},
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            m##dVar##Array1{},                                              \
            m##dVar##Array2{},                                              \
            m##dVar##Array3{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mDummy(0)
    {
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                m##dVar##Array1[i]=dVal2;                                   \
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    m##dVar##Array2[j][i]=dVal2;                            \
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        m##dVar##Array3[k][j][i]=dVal2;                     \
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // 値確認（dVal1側と等しいことをチェックする)
    void check(bool)
    {
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                    \
            THEOLIZER_EQUAL(m##dVar, dVal1);
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(m##dVar##Array1[i], dVal1);                 \
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(m##dVar##Array2[j][i], dVal1);          \
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(m##dVar##Array3[k][j][i], dVal1);   \
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // 値確認（dVal2側と等しいことをチェックする)
    void check(int)
    {
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                    \
            THEOLIZER_EQUAL(m##dVar, dVal2);
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL(m##dVar##Array1[i], dVal2);                 \
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL(m##dVar##Array2[j][i], dVal2);          \
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL(m##dVar##Array3[k][j][i], dVal2);   \
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }
};

//      ---<<< ポイント先の手動（トップ・レベル）処理 >>>---

// 保存
template<class tSerializer>
inline void savePointee(tSerializer& iSerializer, PointeeList& iPointeeList)
{
    // ポインタ変数を保存する
    #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                                \
      THEOLIZER_PROCESS_POINTEE(iSerializer, iPointeeList.m##dVar);
    #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)                           \
        THEOLIZER_PROCESS_POINTEE(iSerializer, iPointeeList.m##dVar##Array1);       \
        THEOLIZER_PROCESS_POINTEE(iSerializer, iPointeeList.m##dVar##Array2);       \
        THEOLIZER_PROCESS_POINTEE(iSerializer, iPointeeList.m##dVar##Array3);
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE
}

// 回復
template<class tSerializer>
inline void loadPointee(tSerializer& iSerializer, PointeeList& iPointeeList)
{
    // ポインタ変数を回復する
    #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                                \
        THEOLIZER_PROCESS_POINTEE(iSerializer, iPointeeList.m##dVar);
    #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)                           \
        THEOLIZER_PROCESS_POINTEE(iSerializer, iPointeeList.m##dVar##Array1);       \
        THEOLIZER_PROCESS_POINTEE(iSerializer, iPointeeList.m##dVar##Array2);       \
        THEOLIZER_PROCESS_POINTEE(iSerializer, iPointeeList.m##dVar##Array3);
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE
}

//      ---<<< ポイント先の手動（非トップ・レベル）処理 >>>---
//      PointeeListを派生して、派生先を手動型とする

struct PointeeListManual : public PointeeList
{
   using PointeeList::PointeeList;
};


THEOLIZER_NON_INTRUSIVE_ORDER((PointeeListManual), 1);

template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<PointeeListManual>::
    TheolizerUserDefine<tBaseSerializer, tTheolizerVersion, 1>
{
    // Save members.
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        // PointeeListのメンバ変数を保存する
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                            \
          THEOLIZER_PROCESS_POINTEE(iSerializer, iInstance->m##dVar);
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)                       \
            THEOLIZER_PROCESS_POINTEE(iSerializer, iInstance->m##dVar##Array1);     \
            THEOLIZER_PROCESS_POINTEE(iSerializer, iInstance->m##dVar##Array2);     \
            THEOLIZER_PROCESS_POINTEE(iSerializer, iInstance->m##dVar##Array3);
        DEFINE_MEMBERS()
        #undef  ARRAY
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

        // PointeeListのメンバ変数を回復する
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                            \
            THEOLIZER_PROCESS_POINTEE(iSerializer, oInstance->m##dVar);
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)                       \
            THEOLIZER_PROCESS_POINTEE(iSerializer, oInstance->m##dVar##Array1);     \
            THEOLIZER_PROCESS_POINTEE(iSerializer, oInstance->m##dVar##Array2);     \
            THEOLIZER_PROCESS_POINTEE(iSerializer, oInstance->m##dVar##Array3);
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }
};

//----------------------------------------------------------------------------
//      ポインタ側１
//          手動（トップ・レベル）  メンバ1つ1つをTHEOLIZER_PROCESS()する
//          自動メンバ・リスト生成  PointerList全体をTHEOLIZER_PROCESS()する
//----------------------------------------------------------------------------

struct PointerList
{
    #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                        \
        dType*          m##dVar;                                            \
        dType const*    m##dVar##Const;
    #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)                   \
        dType*          m##dVar##Array1[dNum];                              \
        dType*          m##dVar##Array2[2][dNum];                           \
        dType*          m##dVar##Array3[3][2][dNum];                        \
        dType const*    m##dVar##Array1##Const[dNum];                       \
        dType const*    m##dVar##Array2##Const[2][dNum];                    \
        dType const*    m##dVar##Array3##Const[3][2][dNum];
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE

    int mDummy;

    // デフォルト・コンストラクタ
    PointerList() :
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                    \
            m##dVar{DEFAULT_PTR(dType)},                                    \
            m##dVar##Const{DEFAULT_PTR(dType)},
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            m##dVar##Array1{},                                              \
            m##dVar##Array2{},                                              \
            m##dVar##Array3{},                                              \
            m##dVar##Array1##Const{},                                       \
            m##dVar##Array2##Const{},                                       \
            m##dVar##Array3##Const{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mDummy(0)
    {
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                m##dVar##Array1[i]=DEFAULT_PTR(dType);                      \
                m##dVar##Array1##Const[i]=DEFAULT_PTR(dType);               \
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    m##dVar##Array2[j][i]=DEFAULT_PTR(dType);               \
                    m##dVar##Array2##Const[j][i]=DEFAULT_PTR(dType);        \
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        m##dVar##Array3[k][j][i]=DEFAULT_PTR(dType);        \
                        m##dVar##Array3##Const[k][j][i]=DEFAULT_PTR(dType); \
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // 保存前の値設定
    PointerList(PointeeList& iPointeeList, PointeeList const& iPointeeList2) :
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                    \
            m##dVar{&iPointeeList.m##dVar},                                 \
            m##dVar##Const{&iPointeeList2.m##dVar},
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            m##dVar##Array1{},                                              \
            m##dVar##Array2{},                                              \
            m##dVar##Array3{},                                              \
            m##dVar##Array1##Const{},                                       \
            m##dVar##Array2##Const{},                                       \
            m##dVar##Array3##Const{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mDummy(0)
    {
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                m##dVar##Array1[i]=&iPointeeList.m##dVar##Array1[i];        \
                m##dVar##Array1##Const[i]=&iPointeeList2.m##dVar##Array1[i];\
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    m##dVar##Array2[j][i]=&iPointeeList.m##dVar##Array2[j][i];\
                    m##dVar##Array2##Const[j][i]=&iPointeeList2.m##dVar##Array2[j][i];\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        m##dVar##Array3[k][j][i]=&iPointeeList.m##dVar##Array3[k][j][i];\
                        m##dVar##Array3##Const[k][j][i]=&iPointeeList2.m##dVar##Array3[k][j][i];\
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // 値確認（dVal1側と等しいことをチェックする)
    void check(PointeeList const& iPointeeList, PointeeList const& iPointeeList2)
    {
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                    \
            THEOLIZER_EQUAL_PTR(m##dVar, &iPointeeList.m##dVar);            \
            THEOLIZER_EQUAL_PTR(m##dVar##Const, &iPointeeList2.m##dVar);
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                THEOLIZER_EQUAL_PTR(m##dVar##Array1[i],                     \
                    &iPointeeList.m##dVar##Array1[i]);                      \
                THEOLIZER_EQUAL_PTR(m##dVar##Array1##Const[i],              \
                    &iPointeeList2.m##dVar##Array1[i]);                     \
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    THEOLIZER_EQUAL_PTR(m##dVar##Array2[j][i],              \
                        &iPointeeList.m##dVar##Array2[j][i]);               \
                    THEOLIZER_EQUAL_PTR(m##dVar##Array2##Const[j][i],       \
                        &iPointeeList2.m##dVar##Array2[j][i]);              \
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        THEOLIZER_EQUAL_PTR(m##dVar##Array3[k][j][i],       \
                            &iPointeeList.m##dVar##Array3[k][j][i]);        \
                        THEOLIZER_EQUAL_PTR(m##dVar##Array3##Const[k][j][i],\
                            &iPointeeList2.m##dVar##Array3[k][j][i]);       \
                    }                                                       \
                }                                                           \
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
    #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                        \
      THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar);                 \
      THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Const);
    #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)                   \
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Array1);       \
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Array2);       \
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Array3);       \
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Array1##Const);\
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Array2##Const);\
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Array3##Const);
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE
}

// 回復
template<class tSerializer>
inline void loadPointer(tSerializer& iSerializer, PointerList& iPointerList)
{
    // ポインタ変数を回復する
    #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                        \
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar);               \
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Const);
    #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)                   \
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Array1);       \
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Array2);       \
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Array3);       \
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Array1##Const);\
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Array2##Const);\
        THEOLIZER_PROCESS(iSerializer, iPointerList.m##dVar##Array3##Const);
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE
}

//----------------------------------------------------------------------------
//      ポインタ側２
//          手動（非トップ・レベル）メンバ1つ1つをTHEOLIZER_PROCESS()する
//----------------------------------------------------------------------------

struct ManualClass4PointerList : public PointerList
{
    using   PointerList::PointerList;
};

THEOLIZER_NON_INTRUSIVE_ORDER((ManualClass4PointerList), 1);

template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<ManualClass4PointerList>::
    TheolizerUserDefine<tBaseSerializer, tTheolizerVersion, 1>
{
    // Save members.
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        // ポインタ変数を保存する
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                    \
          THEOLIZER_PROCESS(iSerializer, iInstance->m##dVar);               \
          THEOLIZER_PROCESS(iSerializer, iInstance->m##dVar##Const);
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            THEOLIZER_PROCESS(iSerializer, iInstance->m##dVar##Array1);     \
            THEOLIZER_PROCESS(iSerializer, iInstance->m##dVar##Array2);     \
            THEOLIZER_PROCESS(iSerializer, iInstance->m##dVar##Array3);     \
            THEOLIZER_PROCESS(iSerializer, iInstance->m##dVar##Array1##Const);\
            THEOLIZER_PROCESS(iSerializer, iInstance->m##dVar##Array2##Const);\
            THEOLIZER_PROCESS(iSerializer, iInstance->m##dVar##Array3##Const);
        DEFINE_MEMBERS()
        #undef  ARRAY
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

        // ポインタ変数を回復する
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                    \
            THEOLIZER_PROCESS(iSerializer, oInstance->m##dVar);             \
            THEOLIZER_PROCESS(iSerializer, oInstance->m##dVar##Const);
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            THEOLIZER_PROCESS(iSerializer, oInstance->m##dVar##Array1);     \
            THEOLIZER_PROCESS(iSerializer, oInstance->m##dVar##Array2);     \
            THEOLIZER_PROCESS(iSerializer, oInstance->m##dVar##Array3);     \
            THEOLIZER_PROCESS(iSerializer, oInstance->m##dVar##Array1##Const);\
            THEOLIZER_PROCESS(iSerializer, oInstance->m##dVar##Array2##Const);\
            THEOLIZER_PROCESS(iSerializer, oInstance->m##dVar##Array3##Const);
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }
};

// ***************************************************************************
//      オーナー指定ポインタのテスト
// ***************************************************************************

//----------------------------------------------------------------------------
//      配列の領域獲得
//          dTypeは基本型
//          dSizesは例えば[3][2][2]のように多次元配列指定
//          newが返却する型はdType(*)[3][2][2]ではなく、
//          dType(*)[2][2]になってしまうため、
//          reinterpret_cast<>して正しい型へ戻す。
//----------------------------------------------------------------------------

#define NEW_ARRAY(dType, dSizes)                                            \
    reinterpret_cast<dType(*)dSizes>(new dType##dSizes)

//----------------------------------------------------------------------------
//      オーナー・ポインタ１
//          手動（トップ・レベル）  メンバ1つ1つをTHEOLIZER_PROCESS_OWNER()する
//          自動メンバ・リスト生成
//              各メンバにTHEOLIZER_ANNOTATE()でオーナー指定しておき、
//              OwnerList全体をTHEOLIZER_PROCESS()する
//----------------------------------------------------------------------------

struct OwnerList
{
    #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                        \
        dType*  m##dVar                     THEOLIZER_ANNOTATE(FS:<>Owner);
    #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)                   \
        dType*  m##dVar##Array1[dNum]       THEOLIZER_ANNOTATE(FS:<>Owner); \
        dType*  m##dVar##Array2[2][dNum]    THEOLIZER_ANNOTATE(FS:<>Owner); \
        dType*  m##dVar##Array3[3][2][dNum] THEOLIZER_ANNOTATE(FS:<>Owner);
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE

    int mDummy;

    // デフォルト・コンストラクタ
    OwnerList() :
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                    \
            m##dVar{nullptr},
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            m##dVar##Array1{},                                              \
            m##dVar##Array2{},                                              \
            m##dVar##Array3{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mDummy(0)
    {
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                m##dVar##Array1[i]=nullptr;                                 \
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    m##dVar##Array2[j][i]=nullptr;                          \
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        m##dVar##Array3[k][j][i]=nullptr;                   \
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // 保存前の値設定1
    OwnerList(bool) :
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                    \
            m##dVar{new dType{dVal1}},
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            m##dVar##Array1{},                                              \
            m##dVar##Array2{},                                              \
            m##dVar##Array3{},
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
        mDummy(0)
    {
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                m##dVar##Array1[i]=new dType{dVal1};                        \
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    m##dVar##Array2[j][i]=new dType{dVal1};                 \
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        m##dVar##Array3[k][j][i]=new dType{dVal1};          \
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // デストラクタ(領域のオーナーなので解放が必要）
    ~OwnerList()
    {
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                    \
            delete m##dVar;
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                delete m##dVar##Array1[i];                                  \
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    delete m##dVar##Array2[j][i];                           \
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        delete m##dVar##Array3[k][j][i];                    \
                    }                                                       \
                }                                                           \
            }
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }

    // 値確認（nullptr or dVal1と等しいことをチェックする)
    void check(bool iIsVal1=false)
    {
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                    \
            if (iIsVal1)                                                    \
                THEOLIZER_EQUAL(*m##dVar, dVal1);                           \
            else                                                            \
                THEOLIZER_EQUAL_PTR(m##dVar, nullptr);
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            for (std::size_t i=0; i < dNum; ++i)                            \
            {                                                               \
                if (iIsVal1)                                                \
                    THEOLIZER_EQUAL(*m##dVar##Array1[i], dVal1);            \
                else                                                        \
                    THEOLIZER_EQUAL_PTR(m##dVar##Array1[i], nullptr);       \
                for (std::size_t j=0; j < 2; ++j)                           \
                {                                                           \
                    if (iIsVal1)                                            \
                        THEOLIZER_EQUAL(*m##dVar##Array2[j][i], dVal1);     \
                    else                                                    \
                        THEOLIZER_EQUAL_PTR(m##dVar##Array2[j][i], nullptr);\
                    for (std::size_t k=0; k < 3; ++k)                       \
                    {                                                       \
                        if (iIsVal1)                                        \
                            THEOLIZER_EQUAL(*m##dVar##Array3[k][j][i], dVal1);\
                        else                                                \
                            THEOLIZER_EQUAL_PTR(m##dVar##Array3[k][j][i], nullptr);\
                    }                                                       \
                }                                                           \
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
inline void saveOwner(tSerializer& iSerializer, OwnerList& iOwnerList)
{
    // ポインタ変数を保存する
    #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                        \
        THEOLIZER_PROCESS_OWNER(iSerializer, iOwnerList.m##dVar);
    #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)                   \
        THEOLIZER_PROCESS_OWNER(iSerializer, iOwnerList.m##dVar##Array1);   \
        THEOLIZER_PROCESS_OWNER(iSerializer, iOwnerList.m##dVar##Array2);   \
        THEOLIZER_PROCESS_OWNER(iSerializer, iOwnerList.m##dVar##Array3);
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE
}

// 回復
template<class tSerializer>
inline void loadOwner(tSerializer& iSerializer, OwnerList& iOwnerList)
{
    // ポインタ変数を回復する
    #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                        \
        THEOLIZER_PROCESS_OWNER(iSerializer, iOwnerList.m##dVar);
    #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)                   \
        THEOLIZER_PROCESS_OWNER(iSerializer, iOwnerList.m##dVar##Array1);   \
        THEOLIZER_PROCESS_OWNER(iSerializer, iOwnerList.m##dVar##Array2);   \
        THEOLIZER_PROCESS_OWNER(iSerializer, iOwnerList.m##dVar##Array3);
    DEFINE_MEMBERS()
    #undef  ARRAY
    #undef  DEFINE
}

//----------------------------------------------------------------------------
//      オーナー・ポインタ２
//          手動（非トップ・レベル）メンバ1つ1つをTHEOLIZER_PROCESS_OWNER()する
//----------------------------------------------------------------------------

struct ManualClass4OwnerList : public OwnerList
{
    using   OwnerList::OwnerList;
};

THEOLIZER_NON_INTRUSIVE_ORDER((ManualClass4OwnerList), 1);

template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<ManualClass4OwnerList>::
    TheolizerUserDefine<tBaseSerializer, tTheolizerVersion, 1>
{
    // Save members.
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        // ポインタ変数を保存する
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                    \
          THEOLIZER_PROCESS_OWNER(iSerializer, iInstance->m##dVar);
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            THEOLIZER_PROCESS_OWNER(iSerializer, iInstance->m##dVar##Array1);\
            THEOLIZER_PROCESS_OWNER(iSerializer, iInstance->m##dVar##Array2);\
            THEOLIZER_PROCESS_OWNER(iSerializer, iInstance->m##dVar##Array3);
        DEFINE_MEMBERS()
        #undef  ARRAY
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

        // ポインタ変数を回復する
        #define DEFINE(dType, dVar, dVal0, dVal1, dVal2)                    \
            THEOLIZER_PROCESS_OWNER(iSerializer, oInstance->m##dVar);
        #define ARRAY(dType, dVar, dNum, dVal0, dVal1, dVal2)               \
            THEOLIZER_PROCESS_OWNER(iSerializer, oInstance->m##dVar##Array1);\
            THEOLIZER_PROCESS_OWNER(iSerializer, oInstance->m##dVar##Array2);\
            THEOLIZER_PROCESS_OWNER(iSerializer, oInstance->m##dVar##Array3);
        DEFINE_MEMBERS()
        #undef  ARRAY
        #undef  DEFINE
    }
};

#endif  // TEST_OBJECT_TRACKING_H
