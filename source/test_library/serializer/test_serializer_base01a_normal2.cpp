//############################################################################
//      各serializerのテスト
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

#define THEOLIZER_INTERNAL_EXCLUDE_VERSION_H

//############################################################################
//      通常のインクルード
//############################################################################

#ifdef _MSC_VER
    #pragma warning(disable:4100 4127)
#endif

#include "test_serializer_base.h"
#include "test_serializer_base_version.h"

#include <sstream>
#include <fstream>
#include <typeindex>
using namespace std;

#include <theolizer/test_tool.h>
#include <theolizer/vector.h>
#include <theolizer/list.h>
#include <theolizer/memory.h>
using theolizer::u8string;

#include "test_serializer_base01a_normal.cpp.theolizer.hpp"

//############################################################################
//      基本テスト
//############################################################################

// ***************************************************************************
//      通常テスト
// ***************************************************************************

// ---------------------------------------------------------------------------
//      シリアライズ
// ---------------------------------------------------------------------------

template<class tOSerializer, typename... tArgs>
void TestOSerializerNormal2
(
    string const& iFileName,
    std::ios::openmode iOpenMode,
    tArgs... iArgs
)
{
//  theolizer::DisplayPass aDisplayPass;

std::cout << "TestOSerializerNormal2(" << iFileName << ", "
          << THEOLIZER_INTERNAL_TYPE_NAME(tOSerializer) << ")\n";

    std::ofstream   aStream(iFileName, iOpenMode);
    tOSerializer    aSerializer(aStream, iArgs...);

//      ---<<< 生配列(トップレベル) >>>---

    int aArray[5][2];
    for (std::size_t i=0; i < std::extent<decltype(aArray), 0>::value; ++i)
        for (std::size_t j=0; j < std::extent<decltype(aArray), 1>::value; ++j)
            aArray[i][j]=static_cast<int>(10*i+j);
    THEOLIZER_PROCESS(aSerializer, aArray);

//      ---<<< 生配列(クラス内) >>>---

    ArrayClass aArrayClass;

    // プリミティブ配列
    typedef decltype(aArrayClass.mArrayInc) ArrayInc;
    for (std::size_t i=0; i < std::extent<ArrayInc, 0>::value; ++i)
    {
        for (std::size_t j=0; j < std::extent<ArrayInc, 1>::value; ++j)
        {
            for (std::size_t k=0; k < std::extent<ArrayInc, 2>::value; ++k)
            {
                aArrayClass.mArrayInc[i][j][k]=static_cast<short>(1000+i*100+j*10+k);
            }
        }
    }
    for (std::size_t i=0; i < std::extent<decltype(aArrayClass.mArrayDec), 0>::value; ++i)
    {
        aArrayClass.mArrayDec[i]=static_cast<unsigned>(2000+i);
    }

    // クラス配列
    typedef decltype(aArrayClass.mClassArrayInc)    ClassArrayInc;
    for (std::size_t i=0; i < std::extent<ClassArrayInc, 0>::value; ++i)
    {
        for (std::size_t j=0; j < std::extent<ClassArrayInc, 1>::value; ++j)
        {
            for (std::size_t k=0; k < std::extent<ClassArrayInc, 2>::value; ++k)
            {
                aArrayClass.mClassArrayInc[i][j][k].mShort=static_cast<short>(3000+i*100+j*10+k);
            }
        }
    }
    for (std::size_t i=0; i < std::extent<decltype(aArrayClass.mClassArrayDec), 0>::value; ++i)
    {
        aArrayClass.mClassArrayDec[i].mShort=static_cast<short>(4000+i);
    }

    // enum配列
    typedef decltype(aArrayClass.mEnumArrayInc) EnumArrayInc;
    for (std::size_t i=0; i < std::extent<EnumArrayInc, 0>::value; ++i)
    {
        for (std::size_t j=0; j < std::extent<EnumArrayInc, 1>::value; ++j)
        {
            for (std::size_t k=0; k < std::extent<EnumArrayInc, 2>::value; ++k)
            {
                aArrayClass.mEnumArrayInc[i][j][k]=static_cast<EnumTest>(k%3);
            }
        }
    }
    for (std::size_t i=0; i < std::extent<decltype(aArrayClass.mEnumArrayDec), 0>::value; ++i)
    {
        aArrayClass.mEnumArrayDec[i]=static_cast<EnumTest>(i%3);
    }
    THEOLIZER_PROCESS(aSerializer, aArrayClass);

//      ---<<< 生配列(クラス・テンプレート内) >>>---

    ArrayClassTemplate<2, 3> aArrayClassTemplate;
    typedef decltype(aArrayClassTemplate.mIntArray) IntArray;
    for (std::size_t i=0; i < std::extent<IntArray, 0>::value; ++i)
    {
        for (std::size_t j=0; j < std::extent<IntArray, 1>::value; ++j)
        {
                aArrayClassTemplate.mIntArray[i][j]=static_cast<int>(2000+i*10+j);
        }
    }
    THEOLIZER_PROCESS(aSerializer, aArrayClassTemplate);

//      ---<<< std::vector >>>---

    std::vector<FullAuto>   aVectorFullAuto=
    {
        { 1, 2},
        { 3, 4},
        { 5, 6}
    };
    THEOLIZER_PROCESS(aSerializer, aVectorFullAuto);

//      ---<<< std::list >>>---

    std::list<FullAuto>     aListFullAuto=
    {
        { 10, 20},
        { 30, 40},
        { 50, 60}
    };
    THEOLIZER_PROCESS(aSerializer, aListFullAuto);

//      ---<<< std::unique_ptr >>>---

    // 定義有り
    std::unique_ptr<FullAuto>   aUnique_ptrFullAuto(new FullAuto);
    aUnique_ptrFullAuto->setInt(10);
    aUnique_ptrFullAuto->setShort(20);
    THEOLIZER_PROCESS(aSerializer, aUnique_ptrFullAuto);

    // nullptr
    std::unique_ptr<FullAuto>   aUnique_ptrFullAuto2;
    THEOLIZER_PROCESS(aSerializer, aUnique_ptrFullAuto2);

    // 後始末
    aSerializer.clearTracking();
}

// ---------------------------------------------------------------------------
//      デシリアライズ
// ---------------------------------------------------------------------------

template<class tISerializer, typename... tArgs>
void TestISerializerNormal2
(
    string const& iFileName,
    std::ios::openmode iOpenMode,
    unsigned iGlovalVersionNo,
    tArgs... iArgs
)
{
return;
std::cout << "TestISerializerNormal2(" << iFileName << ", "
          << THEOLIZER_INTERNAL_TYPE_NAME(tISerializer) << ")\n";

    std::ifstream   aStream(iFileName, iOpenMode);
    tISerializer    aSerializer(aStream, iArgs...);

//      ---<<< 生配列(トップレベル) >>>---

    int aArray[5][2];
    THEOLIZER_PROCESS(aSerializer, aArray);
    for (std::size_t i=0; i < std::extent<decltype(aArray), 0>::value; ++i)
        for (std::size_t j=0; j < std::extent<decltype(aArray), 1>::value; ++j)
            THEOLIZER_EQUAL(aArray[i][j], static_cast<int>(10*i+j), i, j);

//      ---<<< 生配列(クラス内) >>>---

    ArrayClass aArrayClass;
    THEOLIZER_PROCESS(aSerializer, aArrayClass);

    // 最新版のままの場合
#ifndef NO_ARRAY
    if (iGlovalVersionNo >= 3)
    {
        // プリミティブ配列
        typedef decltype(aArrayClass.mArrayInc) ArrayInc;
        for (std::size_t i=0; i < std::extent<ArrayInc, 0>::value; ++i)
        {
            for (std::size_t j=0; j < std::extent<ArrayInc, 1>::value; ++j)
            {
                for (std::size_t k=0; k < std::extent<ArrayInc, 2>::value; ++k)
                {
                    THEOLIZER_EQUAL
                    (
                        aArrayClass.mArrayInc[i][j][k],
                        static_cast<short>(1000+i*100+j*10+k),
                            i, j, k
                    );
                }
            }
        }
        for (std::size_t i=0; i < std::extent<decltype(aArrayClass.mArrayDec), 0>::value; ++i)
        {
            THEOLIZER_EQUAL(aArrayClass.mArrayDec[i], 2000+i, i);
        }

        // クラス配列
        typedef decltype(aArrayClass.mClassArrayInc)    ClassArrayInc;
        for (std::size_t i=0; i < std::extent<ClassArrayInc, 0>::value; ++i)
        {
            for (std::size_t j=0; j < std::extent<ClassArrayInc, 1>::value; ++j)
            {
                for (std::size_t k=0; k < std::extent<ClassArrayInc, 2>::value; ++k)
                {
                    THEOLIZER_EQUAL
                    (
                        aArrayClass.mClassArrayInc[i][j][k].mShort,
                        static_cast<short>(3000+i*100+j*10+k),
                            i, j, k
                    );
                }
            }
        }
        for (std::size_t i=0; i < std::extent<decltype(aArrayClass.mClassArrayDec), 0>::value; ++i)
        {
            THEOLIZER_EQUAL(aArrayClass.mClassArrayDec[i].mShort, static_cast<short>(4000+i), i);
        }

        // enum配列
        typedef decltype(aArrayClass.mEnumArrayInc) EnumArrayInc;
        for (std::size_t i=0; i < std::extent<EnumArrayInc, 0>::value; ++i)
        {
            for (std::size_t j=0; j < std::extent<EnumArrayInc, 1>::value; ++j)
            {
                for (std::size_t k=0; k < std::extent<EnumArrayInc, 2>::value; ++k)
                {
                    aArrayClass.mEnumArrayInc[i][j][k]=static_cast<EnumTest>(k%3);
                    THEOLIZER_EQUAL
                    (
                        aArrayClass.mEnumArrayInc[i][j][k],
                        static_cast<EnumTest>(k%3),
                            i, j, k
                    );
                }
            }
        }
        for (std::size_t i=0; i < std::extent<decltype(aArrayClass.mEnumArrayDec), 0>::value; ++i)
        {
            aArrayClass.mEnumArrayDec[i]=static_cast<EnumTest>(i%3);
            THEOLIZER_EQUAL
            (
                aArrayClass.mEnumArrayDec[i],
                static_cast<EnumTest>(i%3), i
            );
        }
    }

    // 旧バージョンへ一度落とした場合
    else
    {
        // プリミティブ配列
        typedef decltype(aArrayClass.mArrayInc) ArrayInc;
        for (std::size_t i=0; i < std::extent<ArrayInc, 0>::value; ++i)
        {
            for (std::size_t j=0; j < std::extent<ArrayInc, 1>::value; ++j)
            {
                for (std::size_t k=0; k < std::extent<ArrayInc, 2>::value; ++k)
                {
                    if ((i==0)&&(j==0)&&(k<2))
                    {
                        THEOLIZER_EQUAL
                        (
                            aArrayClass.mArrayInc[i][j][k],
                            static_cast<short>(1000+i*100+j*10+k),
                            i, j, k
                        );
                    }
                    else
                    {
                        THEOLIZER_EQUAL(aArrayClass.mArrayInc[i][j][k], static_cast<short>(99),
                            i, j, k);
                    }
                }
            }
        }
        for (std::size_t i=0; i < std::extent<decltype(aArrayClass.mArrayDec), 0>::value; ++i)
        {
            THEOLIZER_EQUAL(aArrayClass.mArrayDec[i], static_cast<unsigned>(2000+i), i);
        }

        // クラス配列
        typedef decltype(aArrayClass.mClassArrayInc)    ClassArrayInc;
        for (std::size_t i=0; i < std::extent<ClassArrayInc, 0>::value; ++i)
        {
            for (std::size_t j=0; j < std::extent<ClassArrayInc, 1>::value; ++j)
            {
                for (std::size_t k=0; k < std::extent<ClassArrayInc, 2>::value; ++k)
                {
                    if ((i==0)&&(j==0)&&(k<2))
                    {
                        THEOLIZER_EQUAL
                        (
                            aArrayClass.mClassArrayInc[i][j][k].mShort,
                            static_cast<short>(3000+i*100+j*10+k),
                            i, j, k
                        );
                    }
                    else
                    {
                        THEOLIZER_EQUAL
                        (
                            aArrayClass.mClassArrayInc[i][j][k].mShort,
                            static_cast<short>(999),
                            i, j, k
                        );
                    }
                }
            }
        }
        for (std::size_t i=0; i < std::extent<decltype(aArrayClass.mClassArrayDec), 0>::value; ++i)
        {
            THEOLIZER_EQUAL(aArrayClass.mClassArrayDec[i].mShort, static_cast<short>(4000+i), i);
        }

        // enum配列
        typedef decltype(aArrayClass.mEnumArrayInc) EnumArrayInc;
        for (std::size_t i=0; i < std::extent<EnumArrayInc, 0>::value; ++i)
        {
            for (std::size_t j=0; j < std::extent<EnumArrayInc, 1>::value; ++j)
            {
                for (std::size_t k=0; k < std::extent<EnumArrayInc, 2>::value; ++k)
                {
                    if ((i==0)&&(j==0)&&(k<2))
                    {
                        THEOLIZER_EQUAL
                        (
                            aArrayClass.mEnumArrayInc[i][j][k],
                            static_cast<EnumTest>(k%3),
                            i, j, k
                        );
                    }
                    else
                    {
                        THEOLIZER_EQUAL
                        (
                            aArrayClass.mEnumArrayInc[i][j][k],
                            static_cast<EnumTest>(1),
                            i, j, k
                        );
                    }
                }
            }
        }
        for (std::size_t i=0; i < std::extent<decltype(aArrayClass.mEnumArrayDec), 0>::value; ++i)
        {
            aArrayClass.mEnumArrayDec[i]=static_cast<EnumTest>(i%3);
            THEOLIZER_EQUAL
            (
                aArrayClass.mEnumArrayDec[i],
                static_cast<EnumTest>(i%3), i
            );
        }
    }
#endif

//      ---<<< 生配列(クラス・テンプレート内) >>>---

    ArrayClassTemplate<2, 3> aArrayClassTemplate;
    THEOLIZER_PROCESS(aSerializer, aArrayClassTemplate);
#ifndef NO_ARRAY
    typedef decltype(aArrayClassTemplate.mIntArray) IntArray;
    for (std::size_t i=0; i < std::extent<IntArray, 0>::value; ++i)
    {
        for (std::size_t j=0; j < std::extent<IntArray, 1>::value; ++j)
        {
            THEOLIZER_EQUAL(aArrayClassTemplate.mIntArray[i][j], static_cast<int>(2000+i*10+j));
        }
    }
#endif

//      ---<<< std::vector >>>---

    std::vector<FullAuto>   aVectorFullAuto;
    THEOLIZER_PROCESS(aSerializer, aVectorFullAuto);
    THEOLIZER_EQUAL(aVectorFullAuto[0].getInt(),   1);
    THEOLIZER_EQUAL(aVectorFullAuto[0].getShort(), 2);
    THEOLIZER_EQUAL(aVectorFullAuto[1].getInt(),   3);
    THEOLIZER_EQUAL(aVectorFullAuto[1].getShort(), 4);
    THEOLIZER_EQUAL(aVectorFullAuto[2].getInt(),   5);
    THEOLIZER_EQUAL(aVectorFullAuto[2].getShort(), 6);

//      ---<<< std::list >>>---

    std::list<FullAuto>     aListFullAuto;
    THEOLIZER_PROCESS(aSerializer, aListFullAuto);
    auto itr = aListFullAuto.begin();
    THEOLIZER_EQUAL(itr->getInt(),   10);
    THEOLIZER_EQUAL(itr->getShort(), 20);
    ++itr;
    THEOLIZER_EQUAL(itr->getInt(),   30);
    THEOLIZER_EQUAL(itr->getShort(), 40);
    ++itr;
    THEOLIZER_EQUAL(itr->getInt(),   50);
    THEOLIZER_EQUAL(itr->getShort(), 60);
    ++itr;

//      ---<<< std::unique_ptr >>>---

    // 定義有り
    std::unique_ptr<FullAuto>   aUnique_ptrFullAuto(new FullAuto);
    THEOLIZER_PROCESS(aSerializer, aUnique_ptrFullAuto);
    THEOLIZER_EQUAL(aUnique_ptrFullAuto->getInt(),   10);
    THEOLIZER_EQUAL(aUnique_ptrFullAuto->getShort(), 20);

    // nullptr
    std::unique_ptr<FullAuto>   aUnique_ptrFullAuto2;
    THEOLIZER_PROCESS(aSerializer, aUnique_ptrFullAuto2);
    THEOLIZER_CHECK(!aUnique_ptrFullAuto2, "not empty");

    // 後始末
    aSerializer.clearTracking();
}

//############################################################################
//      基本テスト呼び出し
//############################################################################

// ***************************************************************************
//      通常テスト用
// ***************************************************************************

// ---------------------------------------------------------------------------
//      各派生Serializer呼び出し
// ---------------------------------------------------------------------------

//      ---<<< プライマリー >>>---

template<class tOSerialzer, class tISerializer>
void TestNormalImpl2
(
    string const& iFileName,
    unsigned iGlobalVersionNo,
    theolizer::CheckMode iCheckMode,
    bool iNoPrettyPrint=false)
{
}

//      ---<<< Fast特殊化 >>>---

template<>
void TestNormalImpl2<theolizer::FastOSerializer<>, theolizer::FastISerializer<> >
(
    string const& iFileName,
    unsigned iGlobalVersionNo,
    theolizer::CheckMode iCheckMode,
    bool iNoPrettyPrint
)
{
    TestOSerializerNormal2<theolizer::FastOSerializer<> >
    (
        iFileName,
        std::ios::binary
    );

    TestISerializerNormal2<theolizer::FastISerializer<> >
    (
        iFileName,
        std::ios::binary,
        iGlobalVersionNo
    );
}

//      ---<<< Xml特殊化 >>>---

template<>
void TestNormalImpl2<theolizer::XmlOSerializer<>, theolizer::XmlISerializer<> >
(
    string const& iFileName,
    unsigned iGlobalVersionNo,
    theolizer::CheckMode iCheckMode,
    bool iNoPrettyPrint
)
{
    TestOSerializerNormal2<theolizer::XmlOSerializer<> >
    (
        iFileName,
        std::ios::openmode(),
        iGlobalVersionNo,
        iNoPrettyPrint
    );

    TestISerializerNormal2<theolizer::XmlISerializer<> >
    (
        iFileName,
        std::ios::openmode(),
        iGlobalVersionNo
    );
}

//      ---<<< Json特殊化 >>>---

template<>
void TestNormalImpl2<theolizer::JsonOSerializer<>, theolizer::JsonISerializer<> >
(
    string const& iFileName,
    unsigned iGlobalVersionNo,
    theolizer::CheckMode iCheckMode,
    bool iNoPrettyPrint
)
{
    TestOSerializerNormal2<theolizer::JsonOSerializer<> >
    (
        iFileName,
        std::ios::openmode(),
        iGlobalVersionNo,
        iCheckMode,
        iNoPrettyPrint
    );

    TestISerializerNormal2<theolizer::JsonISerializer<> >
    (
        iFileName,
        std::ios::openmode(),
        iGlobalVersionNo
    );
}

//      ---<<< Bainary特殊化 >>>---

template<>
void TestNormalImpl2<theolizer::BinaryOSerializer<>, theolizer::BinaryISerializer<> >
(
    string const& iFileName,
    unsigned iGlobalVersionNo,
    theolizer::CheckMode iCheckMode,
    bool iNoPrettyPrint
)
{
    TestOSerializerNormal2<theolizer::BinaryOSerializer<> >
    (
        iFileName,
        std::ios::binary,
        iGlobalVersionNo,
        iCheckMode
    );

    TestISerializerNormal2<theolizer::BinaryISerializer<> >
    (
        iFileName,
        std::ios::binary,
        iGlobalVersionNo
    );
}

// ---------------------------------------------------------------------------
//      まとめ処理
// ---------------------------------------------------------------------------

template<class tOSerialzer, class tISerializer>
void TestNormal2(char const* iPreFix)
{
//      ---<<< InMemory形式 >>>---

    if (!tOSerialzer::hasProperty(theolizer::Property::SupportModifying))
    {
        TestNormalImpl2<tOSerialzer, tISerializer>(string(iPreFix)+"_in_memory.log",
            4, theolizer::CheckMode::InMemory);
return;
    }

//      ---<<< 最新版(Version.4) >>>---

    TestNormalImpl2<tOSerialzer, tISerializer>(string(iPreFix)+"_version4.log",
        4, theolizer::CheckMode::TypeCheck);

//      ---<<< Version.3 >>>---

    TestNormalImpl2<tOSerialzer, tISerializer>(string(iPreFix)+"_version3.log",
        3, theolizer::CheckMode::TypeCheckByIndex);

//      ---<<< Version.2 >>>---

    TestNormalImpl2<tOSerialzer, tISerializer>(string(iPreFix)+"_version2.log",
        2, theolizer::CheckMode::TypeCheck);

//      ---<<< Version.1 >>>---

    TestNormalImpl2<tOSerialzer, tISerializer>(string(iPreFix)+"_version1.log",
        1, theolizer::CheckMode::TypeCheckByIndex);

//      ---<<< NoTypeCheck形式 >>>---

    TestNormalImpl2<tOSerialzer, tISerializer>(string(iPreFix)+"_no_check.log",
        4, theolizer::CheckMode::NoTypeCheck);
}

// ***************************************************************************
//      メイン
// ***************************************************************************

void TestNormalMain2()
{
//  theolizer::DisplayPass aDisplayPass;

//    TestNormal2<theolizer::FastOSerializer<>,  theolizer::FastISerializer<> >("test_fast2");
//    TestNormal2<theolizer::XmlOSerializer<>,   theolizer::XmlISerializer<> >("test_xml2");
    TestNormal2<theolizer::JsonOSerializer<>,  theolizer::JsonISerializer<> >("test_json2");
//    TestNormal2<theolizer::BinaryOSerializer<>,theolizer::BinaryISerializer<> >("test_binary2");
}
