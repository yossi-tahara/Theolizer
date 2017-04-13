//############################################################################
//      各serializerのテスト
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
void TestOSerializerNormal
(
    string const& iFileName,
    std::ios::openmode iOpenMode,
    tArgs... iArgs
)
{
//  theolizer::DisplayPass aDisplayPass;

std::cout << "TestOSerializerNormal(" << iFileName << ", "
          << THEOLIZER_INTERNAL_TYPE_NAME(tOSerializer) << ")\n";

    std::ofstream   aStream(iFileName, iOpenMode);
    tOSerializer    aSerializer(aStream, iArgs...);

//      ---<<< Primitive >>>---

    THEOLIZER_PROCESS(aSerializer, -100);

    char    aChar=-101;
    THEOLIZER_PROCESS(aSerializer, aChar);

    short   aShort=-102;
    THEOLIZER_PROCESS(aSerializer, aShort);

    int aInt=-103;
    THEOLIZER_PROCESS(aSerializer, aInt);

    long     aLong=-104;
    THEOLIZER_PROCESS(aSerializer, aLong);

    long long   aLongLong=-105;
    THEOLIZER_PROCESS(aSerializer, aLongLong);

    unsigned char   aUChar=101;
    THEOLIZER_PROCESS(aSerializer, aUChar);

    unsigned short  aUShort=102;
    THEOLIZER_PROCESS(aSerializer, aUShort);

    unsigned int    aUInt=103;
    THEOLIZER_PROCESS(aSerializer, aUInt);

    unsigned long   aULong=104;
    THEOLIZER_PROCESS(aSerializer, aULong);

    unsigned long long  aULongLong=105;
    THEOLIZER_PROCESS(aSerializer, aULongLong);

    float       aFloat=1.23456F;
    THEOLIZER_PROCESS(aSerializer, aFloat);

    double      aDouble=1.23456789012345;
    THEOLIZER_PROCESS(aSerializer, aDouble);

    long double aLongDouble=1.23456789012345L;
    THEOLIZER_PROCESS(aSerializer, aLongDouble);

//      ---<<< enum型 >>>---

    THEOLIZER_PROCESS(aSerializer, EnumTest::one);

    EnumTest aEnum=EnumTest::zero;
    THEOLIZER_PROCESS(aSerializer, aEnum);

//      ---<<< 非侵入型 >>>---

    string  aString="Test";
    THEOLIZER_PROCESS(aSerializer, aString);

    NonIntrusiveDrived aNonIntrusiveDrived;
    aNonIntrusiveDrived.mLong=-100;
    aNonIntrusiveDrived.mInt=-101;
    THEOLIZER_PROCESS(aSerializer, aNonIntrusiveDrived);

    // 基底クラスの保存／回復
    IntrusiveDerived2 aIntrusiveDerived2;
    aIntrusiveDerived2.mLong=-200;
    aIntrusiveDerived2.mShort=-201;
    THEOLIZER_PROCESS(aSerializer, aIntrusiveDerived2);

//      ---<<< 侵入型(ノーマル) >>>---

    IntrusiveBase   aIntrusiveBase;
    aIntrusiveBase.setInt(201);
    aIntrusiveBase.mShort=202;
    aIntrusiveBase.mString=u8"203)テスト";
    THEOLIZER_PROCESS(aSerializer, aIntrusiveBase);

    // IntrusiveBase(値が化けていないことをチェックする)
    THEOLIZER_EQUAL(aIntrusiveBase.getInt(), 201);
    THEOLIZER_EQUAL(aIntrusiveBase.mShort, 202);
    THEOLIZER_EQUAL(aIntrusiveBase.mString, u8"203)テスト");

//      ---<<< 侵入型(基底クラス／包含クラス) >>>---

    IntrusiveDerived    aIntrusiveDerived;

    // IntrusiveBase
    aIntrusiveDerived.setInt(301);
    aIntrusiveDerived.mShort=302;
    aIntrusiveDerived.mString=u8"303)テスト";
#if 0   // IntrusiveBase2
    aIntrusiveDerived.mLong=401;
    aIntrusiveDerived.mLongLong=402;
    aIntrusiveDerived.mULong=403;
    aIntrusiveDerived.mULongLong=404;
#endif
    // IntrusiveDerived
    aIntrusiveDerived.mUInt=501;
    aIntrusiveDerived.mUShort=502;
    // IntrusiveDerived.IntrusiveBase
    aIntrusiveDerived.mIntrusiveBase.setInt(601);
    aIntrusiveDerived.mIntrusiveBase.mShort=602;
    aIntrusiveDerived.mIntrusiveBase.mString=u8"603)テスト";
    aIntrusiveDerived.mEnum=EnumTest::two;
    THEOLIZER_PROCESS(aSerializer, aIntrusiveDerived);

    // IntrusiveBase(値が化けていないことをチェックする)
    THEOLIZER_EQUAL(aIntrusiveDerived.getInt(), 301);
    THEOLIZER_EQUAL(aIntrusiveDerived.mShort, 302);
    THEOLIZER_EQUAL(aIntrusiveDerived.mString, u8"303)テスト");

    // IntrusiveDerived.IntrusiveBase(値が化けていないことをチェックする)
    THEOLIZER_EQUAL(aIntrusiveDerived.mIntrusiveBase.getInt(), 601);
    THEOLIZER_EQUAL(aIntrusiveDerived.mIntrusiveBase.mShort, 602);
    THEOLIZER_EQUAL(aIntrusiveDerived.mIntrusiveBase.mString, u8"603)テスト");

//      ---<<< 完全自動（ノーマル） >>>---

    FullAuto aFullAuto;
    aFullAuto.setInt(1);
    aFullAuto.setShort(-2);
    THEOLIZER_PROCESS(aSerializer, aFullAuto);

//      ---<<< 完全自動（テンプレート） >>>---

    FullAutoTemplate<unsigned, unsigned short> aFullAutoTemplate;
    aFullAutoTemplate.setType0(1);
    aFullAutoTemplate.setType1(2);
    THEOLIZER_PROCESS(aSerializer, aFullAutoTemplate);

    // 後始末
    aSerializer.clearTracking();
}

// ---------------------------------------------------------------------------
//      デシリアライズ
// ---------------------------------------------------------------------------

template<class tISerializer, typename... tArgs>
void TestISerializerNormal
(
    string const& iFileName,
    std::ios::openmode iOpenMode,
    unsigned iGlovalVersionNo,
    tArgs... iArgs
)
{
std::cout << "TestISerializerNormal(" << iFileName << ", "
          << THEOLIZER_INTERNAL_TYPE_NAME(tISerializer) << ")\n";

    std::ifstream   aStream(iFileName, iOpenMode);
    tISerializer    aSerializer(aStream, iArgs...);

//      ---<<< Primitive >>>---

    int aTemp;
    THEOLIZER_PROCESS(aSerializer, aTemp);
    THEOLIZER_EQUAL(aTemp, -100);

    char    aChar;
    THEOLIZER_PROCESS(aSerializer, aChar);
    THEOLIZER_EQUAL(aChar, -101);

    short   aShort;
    THEOLIZER_PROCESS(aSerializer, aShort);
    THEOLIZER_EQUAL(aShort, -102);

    int aInt;
    THEOLIZER_PROCESS(aSerializer, aInt);
    THEOLIZER_EQUAL(aInt, -103);

    long    aLong;
    THEOLIZER_PROCESS(aSerializer, aLong);
    THEOLIZER_EQUAL(aLong, -104);

    long long   aLongLong;
    THEOLIZER_PROCESS(aSerializer, aLongLong);
    THEOLIZER_EQUAL(aLongLong, -105);

    unsigned char   aUChar;
    THEOLIZER_PROCESS(aSerializer, aUChar);
    THEOLIZER_EQUAL(aUChar, 101);

    unsigned short  aUShort;
    THEOLIZER_PROCESS(aSerializer, aUShort);
    THEOLIZER_EQUAL(aUShort, 102);

    unsigned int    aUInt;
    THEOLIZER_PROCESS(aSerializer, aUInt);
    THEOLIZER_EQUAL(aUInt, 103);

    unsigned long   aULong;
    THEOLIZER_PROCESS(aSerializer, aULong);
    THEOLIZER_EQUAL(aULong, 104);

    unsigned long long  aULongLong;
    THEOLIZER_PROCESS(aSerializer, aULongLong);
    THEOLIZER_EQUAL(aULongLong, 105);

    float       aFloat;
    THEOLIZER_PROCESS(aSerializer, aFloat);
    THEOLIZER_EQUAL(aFloat, 1.23456F);

    double      aDouble;
    THEOLIZER_PROCESS(aSerializer, aDouble);
    THEOLIZER_EQUAL(aDouble, 1.23456789012345);

    long double aLongDouble;
    THEOLIZER_PROCESS(aSerializer, aLongDouble);

    // TheolizerはFastSerializer以外はlong doubleをdoubleで処理するので、doubleで比較する
    if (tISerializer::kIsFastSerialzier)
    {
        THEOLIZER_EQUAL(aLongDouble, 1.23456789012345L);
    }
    else
    {
        double temp=static_cast<double>(aLongDouble);
        THEOLIZER_EQUAL(temp, 1.23456789012345);
    }

//      ---<<< enum型 >>>---

    EnumTest aEnum=EnumTest::zero;
    THEOLIZER_PROCESS(aSerializer, aEnum);
    THEOLIZER_EQUAL(aEnum, EnumTest::one);

    THEOLIZER_PROCESS(aSerializer, aEnum);
    THEOLIZER_EQUAL(aEnum, EnumTest::zero);

//      ---<<< 非侵入型 >>>---

    string  aString;
    THEOLIZER_PROCESS(aSerializer, aString);
    THEOLIZER_EQUAL(aString, "Test");

    NonIntrusiveDrived aNonIntrusiveDrived;
    THEOLIZER_PROCESS(aSerializer, aNonIntrusiveDrived);
    THEOLIZER_EQUAL(aNonIntrusiveDrived.mLong, -100);
    THEOLIZER_EQUAL(aNonIntrusiveDrived.mInt,  -101);

    // 基底クラスの保存／回復
    IntrusiveDerived2 aIntrusiveDerived2;
    THEOLIZER_PROCESS(aSerializer, aIntrusiveDerived2);
    THEOLIZER_EQUAL(aIntrusiveDerived2.mLong,  -200);
    THEOLIZER_EQUAL(aIntrusiveDerived2.mShort, -201);

//      ---<<< 侵入型(ノーマル) >>>---

    IntrusiveBase   aIntrusiveBase;
    THEOLIZER_PROCESS(aSerializer, aIntrusiveBase);

    THEOLIZER_EQUAL(aIntrusiveBase.getInt(), 201);
    THEOLIZER_EQUAL(aIntrusiveBase.mShort, 202);
    THEOLIZER_EQUAL(aIntrusiveBase.mString, u8"203)テスト");

//      ---<<< 侵入型(基底クラス／包含クラス) >>>---

    IntrusiveDerived    aIntrusiveDerived;
    THEOLIZER_PROCESS(aSerializer, aIntrusiveDerived);

    // IntrusiveBase
    THEOLIZER_EQUAL(aIntrusiveDerived.getInt(), 301);
    THEOLIZER_EQUAL(aIntrusiveDerived.mShort, 302);
    THEOLIZER_EQUAL(aIntrusiveDerived.mString, u8"303)テスト");
#if 0   // IntrusiveBase2
    THEOLIZER_EQUAL(aIntrusiveDerived.mLong, 401);
    THEOLIZER_EQUAL(aIntrusiveDerived.mLongLong, 402);
    THEOLIZER_EQUAL(aIntrusiveDerived.mULong, 403);
    THEOLIZER_EQUAL(aIntrusiveDerived.mULongLong, 404);
#endif
    // IntrusiveDerived
    if (3 <= iGlovalVersionNo)
    {
        THEOLIZER_EQUAL(aIntrusiveDerived.mUInt, 501);
        THEOLIZER_EQUAL(aIntrusiveDerived.mUShort, 502);
    }
    else
    {
        THEOLIZER_EQUAL(aIntrusiveDerived.mUInt, 0);
        THEOLIZER_EQUAL(aIntrusiveDerived.mUShort, 502+(3-iGlovalVersionNo));
    }
    // IntrusiveDerived.IntrusiveBase
    THEOLIZER_EQUAL(aIntrusiveDerived.mIntrusiveBase.getInt(), 601);
    THEOLIZER_EQUAL(aIntrusiveDerived.mIntrusiveBase.mShort, 602);
    THEOLIZER_EQUAL(aIntrusiveDerived.mIntrusiveBase.mString, u8"603)テスト");

    THEOLIZER_EQUAL(aIntrusiveDerived.mEnum, EnumTest::two);

//      ---<<< 完全自動（ノーマル） >>>---

    FullAuto aFullAuto;
    THEOLIZER_PROCESS(aSerializer, aFullAuto);
    THEOLIZER_EQUAL(aFullAuto.getInt(), 1);
    THEOLIZER_EQUAL(aFullAuto.getShort(), -2);

//      ---<<< 完全自動（テンプレート） >>>---

    FullAutoTemplate<unsigned, unsigned short> aFullAutoTemplate;
    THEOLIZER_PROCESS(aSerializer, aFullAutoTemplate);
    THEOLIZER_EQUAL(aFullAutoTemplate.getType0(), 1);
    THEOLIZER_EQUAL(aFullAutoTemplate.getType1(), 2);

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
void TestNormalImpl
(
    string const& iFileName,
    unsigned iGlobalVersionNo,
    theolizer::CheckMode iCheckMode,
    bool iNoPrettyPrint=false)
{
}

//      ---<<< Fast特殊化 >>>---

template<>
void TestNormalImpl<theolizer::FastOSerializer<>, theolizer::FastISerializer<> >
(
    string const& iFileName,
    unsigned iGlobalVersionNo,
    theolizer::CheckMode iCheckMode,
    bool iNoPrettyPrint
)
{
    TestOSerializerNormal<theolizer::FastOSerializer<> >
    (
        iFileName,
        std::ios::binary
    );

    TestISerializerNormal<theolizer::FastISerializer<> >
    (
        iFileName,
        std::ios::binary,
        iGlobalVersionNo
    );
}

//      ---<<< Json特殊化 >>>---

template<>
void TestNormalImpl<theolizer::JsonOSerializer<>, theolizer::JsonISerializer<> >
(
    string const& iFileName,
    unsigned iGlobalVersionNo,
    theolizer::CheckMode iCheckMode,
    bool iNoPrettyPrint
)
{
    TestOSerializerNormal<theolizer::JsonOSerializer<> >
    (
        iFileName,
        std::ios::openmode(),
        iGlobalVersionNo,
        iCheckMode,
        iNoPrettyPrint
    );

    TestISerializerNormal<theolizer::JsonISerializer<> >
    (
        iFileName,
        std::ios::openmode(),
        iGlobalVersionNo
    );
}

//      ---<<< Bainary特殊化 >>>---

template<>
void TestNormalImpl<theolizer::BinaryOSerializer<>, theolizer::BinaryISerializer<> >
(
    string const& iFileName,
    unsigned iGlobalVersionNo,
    theolizer::CheckMode iCheckMode,
    bool iNoPrettyPrint
)
{
    TestOSerializerNormal<theolizer::BinaryOSerializer<> >
    (
        iFileName,
        std::ios::binary,
        iGlobalVersionNo,
        iCheckMode
    );

    TestISerializerNormal<theolizer::BinaryISerializer<> >
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
void TestNormal(char const* iPreFix, bool iIsFast=false)
{
//      ---<<< InMemory形式 >>>---

    if (iIsFast)
    {
        TestNormalImpl<tOSerialzer, tISerializer>(string(iPreFix)+"_in_memory.log",
            4, theolizer::CheckMode::InMemory);
return;
    }

//      ---<<< 最新版(Version.4) >>>---

    TestNormalImpl<tOSerialzer, tISerializer>(string(iPreFix)+"_version4.log",
        4, theolizer::CheckMode::TypeCheck);

//      ---<<< Version.3 >>>---

    TestNormalImpl<tOSerialzer, tISerializer>(string(iPreFix)+"_version3.log",
        3, theolizer::CheckMode::TypeCheckByIndex);

//      ---<<< Version.2 >>>---

    TestNormalImpl<tOSerialzer, tISerializer>(string(iPreFix)+"_version2.log",
        2, theolizer::CheckMode::TypeCheck);

//      ---<<< Version.1 >>>---

    TestNormalImpl<tOSerialzer, tISerializer>(string(iPreFix)+"_version1.log",
        1, theolizer::CheckMode::TypeCheckByIndex);

//      ---<<< NoTypeCheck形式 >>>---

    TestNormalImpl<tOSerialzer, tISerializer>(string(iPreFix)+"_no_check.log",
        4, theolizer::CheckMode::NoTypeCheck);
}

// ***************************************************************************
//      メイン
// ***************************************************************************

void TestNormalMain()
{
//  theolizer::DisplayPass aDisplayPass;

    TestNormal<theolizer::FastOSerializer<>,  theolizer::FastISerializer<> >("test_fast", true);
    TestNormal<theolizer::JsonOSerializer<>,  theolizer::JsonISerializer<> >("test_json");
    TestNormal<theolizer::BinaryOSerializer<>,theolizer::BinaryISerializer<> >("test_binary");
}
