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
using theolizer::u8string;

#include "test_serializer_base01a_tracking.cpp.theolizer.hpp"

//############################################################################
//      オブジェクト追跡テスト
//############################################################################

// ***************************************************************************
//      様々なオブジェクト追跡のテスト
// ***************************************************************************

// ---------------------------------------------------------------------------
//      シリアライズ
// ---------------------------------------------------------------------------

template<class tOSerializer, typename... tArgs>
void TestOSerializerTracking
(
    string const& iFileName,
    std::ios::openmode iOpenMode,
    tArgs... iArgs
)
{
//  theolizer::DisplayPass aDisplayPass;

std::cout << "TestOSerializerTracking(" << iFileName << ", "
          << THEOLIZER_INTERNAL_TYPE_NAME(tOSerializer) << ")\n";

    std::ofstream   aStream(iFileName, iOpenMode);
    tOSerializer    aSerializer(aStream, iArgs...);

//      ---<<< 静的定義領域の追跡指定 >>>---

    unsigned    aUInt=100;
    THEOLIZER_PROCESS_POINTEE(aSerializer, aUInt);

    EnumTest aEnum=EnumTest::one;
    THEOLIZER_PROCESS_POINTEE(aSerializer, aEnum);

    wstring aWstring=L"テスト";
    THEOLIZER_PROCESS_POINTEE(aSerializer, aWstring);

    long    aLongArray[3][2];
    for (int i=0; i < 3; ++i)
        for (int j=0; j < 2; ++j)
            aLongArray[i][j]=200+10*i+j;
    THEOLIZER_PROCESS_POINTEE(aSerializer, aLongArray);

    Tracking   aTracking;
    aTracking.mInt              =301;
    aTracking.mEnum             =EnumTest::two;
    aTracking.mUInt             =302;
    aTracking.mIncluded2.mShort =303;
    aTracking.mString           ="304";
    THEOLIZER_PROCESS_POINTEE(aSerializer, aTracking);

//      ---<<< 静的定義領域のポインタ処理 >>>---

    // トップ・レベル
    unsigned*   aUIntPtr=&aUInt;
    THEOLIZER_PROCESS(aSerializer, aUIntPtr);

    EnumTest*   aEnumPtr=&aEnum;
    THEOLIZER_PROCESS(aSerializer, aEnumPtr);

    wstring* aWstringPtr=&aWstring;
    THEOLIZER_PROCESS(aSerializer, aWstringPtr);

    long*   aLongArrayElemPtr=&aLongArray[2][1];
    THEOLIZER_PROCESS(aSerializer, aLongArrayElemPtr);

    Tracking* aTrackingPtr=&aTracking;
    THEOLIZER_PROCESS(aSerializer, aTrackingPtr);

    // クラスの要素
    int*        aTrackingIntPtr=&aTracking.mInt;    // Included1::mInt
    THEOLIZER_PROCESS(aSerializer, aTrackingIntPtr);

    unsigned*   aTrackingUIntPtr=&aTracking.mUInt;
    THEOLIZER_PROCESS(aSerializer, aTrackingUIntPtr);

    EnumTest*   aTrackingEnumPtr=&aTracking.mEnum;
    THEOLIZER_PROCESS(aSerializer, aTrackingEnumPtr);

    Included2*  aTrackingIncluded2Ptr=&aTracking.mIncluded2;
    THEOLIZER_PROCESS(aSerializer, aTrackingIncluded2Ptr);

    string*     aTrackingStringPtr=&aTracking.mString;
    THEOLIZER_PROCESS(aSerializer, aTrackingStringPtr);

#ifndef NO_ARRAY
    unsigned*  aTrackingArrayElemPtr=&aTracking.mArray[1][2];
    THEOLIZER_PROCESS(aSerializer, aTrackingArrayElemPtr);
#endif

//      ---<<< 通常ポインタの追跡 >>>---

    Pointers    aPointers;
    aPointers.mUInt             =&aTracking.mUInt;
    aPointers.mEnum             =&aTracking.mEnum;
    aPointers.mUInt             =&aTracking.mUInt;
    aPointers.mIncluded2        =&aTracking.mIncluded2;
    aPointers.mString           =&aTracking.mString;
    THEOLIZER_PROCESS(aSerializer, aPointers);

//      ---<<< 動的生成領域のオーナー指定(基本) >>>---

    Dynamic*    aDynamic=new Dynamic();
    auto aAutoDeleter=theolizer::makeScopeExit([&]{delete aDynamic;});

    aDynamic->mUInt = new unsigned int(400);
    aDynamic->mEnum = new EnumTest(EnumTest::one);
    aDynamic->mIncluded1 = new Included1(402);
    aDynamic->mString = new string("403");
    aDynamic->mArray[1] = new int(404);
    aDynamic->mArrayPtr = reinterpret_cast<int(*)[2]>(new int[2]{405, 406});
    THEOLIZER_PROCESS_OWNER(aSerializer, aDynamic);

//      ---<<< 多重指定動的生成領域のポインタ処理(基本) >>>---

    unsigned*       mUInt       =aDynamic->mUInt;
    THEOLIZER_PROCESS_OWNER(aSerializer, mUInt);

    EnumTest*       mEnum       =aDynamic->mEnum;
    THEOLIZER_PROCESS_OWNER(aSerializer, mEnum);

    Included1*      mIncluded1  =aDynamic->mIncluded1;
    THEOLIZER_PROCESS_OWNER(aSerializer, mIncluded1);

    string*         mString     =aDynamic->mString;
    THEOLIZER_PROCESS_OWNER(aSerializer, mString);

#ifndef NO_ARRAY
    int*            mArrayElem  =aDynamic->mArray[1];
    THEOLIZER_PROCESS_OWNER(aSerializer, mArrayElem);
    int         (*mArrayPtr)[2] =aDynamic->mArrayPtr;
    THEOLIZER_PROCESS_OWNER(aSerializer, mArrayPtr);
#endif

//      ---<<< 動的生成領域のオーナー指定(別領域有り) >>>---

    Dynamic*    aDynamic2=new Dynamic();
    auto aAutoDeleter2=theolizer::makeScopeExit([&]{delete aDynamic2;});

    aDynamic2->mUInt = new unsigned int(500);
    aDynamic2->mEnum = new EnumTest(EnumTest::two);
    aDynamic2->mIncluded1 = new Included1(502);
    aDynamic2->mString = new string("503");
#ifndef NO_ARRAY
    aDynamic2->mArray[1] = new int(504);
    aDynamic2->mArrayPtr = reinterpret_cast<int(*)[2]>(new int[2]{505, 506});
#endif
    THEOLIZER_PROCESS_OWNER(aSerializer, aDynamic2);

//      ---<<< 多重指定動的生成領域のポインタ処理(非侵入型領域有り) >>>---

    string*         mString2    =aDynamic2->mString;
    THEOLIZER_PROCESS_OWNER(aSerializer, mString2);

//      ---<<< 多重指定動的生成領域のポインタ処理(非侵入型領域nullptr) >>>---

    Dynamic*    aDynamic3=new Dynamic();
    auto aAutoDeleter3=theolizer::makeScopeExit([&]{delete aDynamic3;});

    aDynamic3->mString = new string("603");
    THEOLIZER_PROCESS_OWNER(aSerializer, aDynamic3);

    string*         mString3    =aDynamic3->mString;
    THEOLIZER_PROCESS_OWNER(aSerializer, mString3);

    // 後始末
    aSerializer.clearTracking();
}

// ---------------------------------------------------------------------------
//      デシリアライズ
// ---------------------------------------------------------------------------

template<class tISerializer, typename... tArgs>
void TestISerializerTracking
(
    string const& iFileName,
    std::ios::openmode iOpenMode,
    unsigned iGlovalVersionNo,
    tArgs... iArgs
)
{
std::cout << "TestISerializerTracking(" << iFileName << ", "
          << THEOLIZER_INTERNAL_TYPE_NAME(tISerializer) << ")\n";

    std::ifstream   aStream(iFileName, iOpenMode);
    tISerializer    aSerializer(aStream, iArgs...);

//      ---<<< 静的定義領域の追跡指定 >>>---

    unsigned    aUInt=0;
    THEOLIZER_PROCESS_POINTEE(aSerializer, aUInt);
    THEOLIZER_EQUAL(aUInt, 100);

    EnumTest aEnum=EnumTest::zero;
    THEOLIZER_PROCESS_POINTEE(aSerializer, aEnum);
    THEOLIZER_EQUAL(aEnum, EnumTest::one);

    wstring aWstring;
    THEOLIZER_PROCESS_POINTEE(aSerializer, aWstring);
    THEOLIZER_EQUAL(aWstring, L"テスト");

    long    aLongArray[3][2];
    THEOLIZER_PROCESS_POINTEE(aSerializer, aLongArray);
    for (int i=0; i < 3; ++i)
        for (int j=0; j < 2; ++j)
            THEOLIZER_EQUAL(aLongArray[i][j], 200+10*i+j);

    Tracking   aTracking;
    THEOLIZER_PROCESS_POINTEE(aSerializer, aTracking);
    THEOLIZER_EQUAL(aTracking.mInt, 301);
    THEOLIZER_EQUAL(aTracking.mEnum, EnumTest::two);
    THEOLIZER_EQUAL(aTracking.mUInt, 302);
    THEOLIZER_EQUAL(aTracking.mIncluded2.mShort, 303);
    THEOLIZER_EQUAL(aTracking.mString, "304");

//      ---<<< 静的定義領域のポインタ処理 >>>---

    // トップ・レベル
    unsigned*   aUIntPtr=nullptr;
    THEOLIZER_PROCESS(aSerializer, aUIntPtr);
    THEOLIZER_EQUAL(aUIntPtr, &aUInt);

    EnumTest*   aEnumPtr=nullptr;
    THEOLIZER_PROCESS(aSerializer, aEnumPtr);
    THEOLIZER_EQUAL(aEnumPtr, &aEnum);

    wstring* aWstringPtr=nullptr;
    THEOLIZER_PROCESS(aSerializer, aWstringPtr);
    THEOLIZER_EQUAL(aWstringPtr, &aWstring);

    long*   aLongArrayElemPtr=nullptr;
    THEOLIZER_PROCESS(aSerializer, aLongArrayElemPtr);
    THEOLIZER_EQUAL(aLongArrayElemPtr, &aLongArray[2][1]);

    Tracking* aTrackingPtr=nullptr;
    THEOLIZER_PROCESS(aSerializer, aTrackingPtr);
    THEOLIZER_EQUAL(aTrackingPtr, &aTracking);

    // クラスの要素
    int*        aTrackingIntPtr=nullptr;
    THEOLIZER_PROCESS(aSerializer, aTrackingIntPtr);
    THEOLIZER_EQUAL(aTrackingIntPtr, &aTracking.mInt);

    unsigned*   aTrackingUIntPtr=nullptr;
    THEOLIZER_PROCESS(aSerializer, aTrackingUIntPtr);
    THEOLIZER_EQUAL(aTrackingUIntPtr, &aTracking.mUInt);

    EnumTest*   aTrackingEnumPtr=nullptr;
    THEOLIZER_PROCESS(aSerializer, aTrackingEnumPtr);
    THEOLIZER_EQUAL(aTrackingEnumPtr, &aTracking.mEnum);

    Included2*  aTrackingIncluded2Ptr=nullptr;
    THEOLIZER_PROCESS(aSerializer, aTrackingIncluded2Ptr);
    THEOLIZER_EQUAL(aTrackingIncluded2Ptr, &aTracking.mIncluded2);

    string*     aTrackingStringPtr=nullptr;
    THEOLIZER_PROCESS(aSerializer, aTrackingStringPtr);
    THEOLIZER_EQUAL(aTrackingStringPtr, &aTracking.mString);

#ifndef NO_ARRAY
    unsigned*  aTrackingArrayElemPtr=nullptr;
    THEOLIZER_PROCESS(aSerializer, aTrackingArrayElemPtr);
    THEOLIZER_EQUAL(aTrackingArrayElemPtr, &aTracking.mArray[1][2]);
#endif

//      ---<<< 通常ポインタの追跡 >>>---

    Pointers    aPointers;
    THEOLIZER_PROCESS(aSerializer, aPointers);
    THEOLIZER_EQUAL(aPointers.mUInt,      &aTracking.mUInt);
    THEOLIZER_EQUAL(aPointers.mEnum,      &aTracking.mEnum);
    THEOLIZER_EQUAL(aPointers.mUInt,      &aTracking.mUInt);
    THEOLIZER_EQUAL(aPointers.mIncluded2, &aTracking.mIncluded2);
    THEOLIZER_EQUAL(aPointers.mString,    &aTracking.mString);

//      ---<<< 動的生成領域のオーナー指定(基本) >>>---

    Dynamic*    aDynamic=nullptr;
    auto aAutoDeleter=theolizer::makeScopeExit([&]{delete aDynamic;});

    THEOLIZER_PROCESS_OWNER(aSerializer, aDynamic);
    THEOLIZER_EQUAL(*(aDynamic->mUInt), 400);
    THEOLIZER_EQUAL(*aDynamic->mEnum, EnumTest::one);
    THEOLIZER_EQUAL( (aDynamic->mIncluded1->mInt), 402);
    THEOLIZER_EQUAL(*(aDynamic->mString), "403");
#ifndef NO_ARRAY
    THEOLIZER_EQUAL(*(aDynamic->mArray[1]), 404);
    THEOLIZER_EQUAL((*(aDynamic->mArrayPtr))[0], 405);
    THEOLIZER_EQUAL((*(aDynamic->mArrayPtr))[1], 406);
#endif

//      ---<<< 多重指定動的生成領域のポインタ処理(基本) >>>---

    unsigned*       mUInt       =new unsigned(0);
    THEOLIZER_PROCESS_OWNER(aSerializer, mUInt);
    THEOLIZER_EQUAL(mUInt, aDynamic->mUInt);

    EnumTest*       mEnum       =nullptr;
    THEOLIZER_PROCESS_OWNER(aSerializer, mEnum);
    THEOLIZER_EQUAL(mEnum, aDynamic->mEnum);

    Included1*      mIncluded1  =aDynamic->mIncluded1;
    THEOLIZER_PROCESS_OWNER(aSerializer, mIncluded1);
    THEOLIZER_EQUAL(mIncluded1, aDynamic->mIncluded1);

    string*         mString     =aDynamic->mString;
    THEOLIZER_PROCESS_OWNER(aSerializer, mString);
    THEOLIZER_EQUAL(mString, aDynamic->mString);

#ifndef NO_ARRAY
    int*            mArrayElem  =aDynamic->mArray[1];
    THEOLIZER_PROCESS_OWNER(aSerializer, mArrayElem);
    THEOLIZER_EQUAL(mArrayElem, aDynamic->mArray[1]);

    int         (*mArrayPtr)[2] =aDynamic->mArrayPtr;
    THEOLIZER_PROCESS_OWNER(aSerializer, mArrayPtr);
    THEOLIZER_EQUAL(mArrayPtr, aDynamic->mArrayPtr);
#endif

//      ---<<< 動的生成領域のオーナー指定(別領域有り) >>>---

    Dynamic*    aDynamic2=new Dynamic();
    auto aAutoDeleter2=theolizer::makeScopeExit([&]{delete aDynamic2;});

    aDynamic2->mUInt = new unsigned int(0);
    aDynamic2->mEnum = new EnumTest(EnumTest::zero);
    aDynamic2->mIncluded1 = new Included1(-502);
    aDynamic2->mString = new string("-503");
#ifndef NO_ARRAY
    aDynamic2->mArray[1] = new int(-504);
    aDynamic2->mArrayPtr = reinterpret_cast<int(*)[2]>(new int[2]{-505, -506});
#endif

    THEOLIZER_PROCESS_OWNER(aSerializer, aDynamic2);
    THEOLIZER_EQUAL(*(aDynamic2->mUInt), 500);
    THEOLIZER_EQUAL(*(aDynamic2->mEnum), EnumTest::two);
    THEOLIZER_EQUAL( (aDynamic2->mIncluded1->mInt), 502);
    THEOLIZER_EQUAL(*(aDynamic2->mString), "503");
#ifndef NO_ARRAY
    THEOLIZER_EQUAL(*(aDynamic2->mArray[1]) , 504);
    THEOLIZER_EQUAL((*(aDynamic2->mArrayPtr))[0], 505);
    THEOLIZER_EQUAL((*(aDynamic2->mArrayPtr))[1], 506);
#endif

//      ---<<< 多重指定動的生成領域のポインタ処理(非侵入型領域有り) >>>---

    string*         mString2     =new string();
    THEOLIZER_PROCESS_OWNER(aSerializer, mString2);
    THEOLIZER_EQUAL(mString2, aDynamic2->mString);

//      ---<<< 多重指定動的生成領域のポインタ処理(非侵入型領域nullptr) >>>---

    Dynamic*    aDynamic3=nullptr;
    auto aAutoDeleter3=theolizer::makeScopeExit([&]{delete aDynamic3;});

    THEOLIZER_PROCESS_OWNER(aSerializer, aDynamic3);
    THEOLIZER_EQUAL(*(aDynamic3->mString), "603");

    string*         mString3    =nullptr;
    THEOLIZER_PROCESS_OWNER(aSerializer, mString3);
    THEOLIZER_EQUAL(mString3, aDynamic3->mString);

    // 後始末
    aSerializer.clearTracking();
}

// ***************************************************************************
//      複雑なポインタと参照のテスト
// ***************************************************************************

// ---------------------------------------------------------------------------
//      シリアライズ
// ---------------------------------------------------------------------------

template<class tOSerializer, typename... tArgs>
void TestOSerializerPointer(
    string const& iFileName,
    std::ios::openmode iOpenMode,
    tArgs... iArgs
)
{
std::cout << "TestOSerializerPointer(" << iFileName << ", "
          << THEOLIZER_INTERNAL_TYPE_NAME(tOSerializer) << ")\n";

    std::ofstream   aStream(iFileName, iOpenMode);
    tOSerializer    aSerializer(aStream, iArgs...);

//      ---<<< 静的定義領域／動的生成領域への前方／後方ポイント >>>---

    int*    aIntPtrD=new int(12345);
    auto aAutoDeleter=theolizer::makeScopeExit([&]{delete aIntPtrD;});

    int*    aIntPtrD0=aIntPtrD;
    THEOLIZER_PROCESS(aSerializer, aIntPtrD0);

    int     aInt=0;
    int*    aIntPtrS0=&aInt;
    THEOLIZER_PROCESS(aSerializer, aIntPtrS0);

    int*    aIntPtrD1=aIntPtrD;
    THEOLIZER_PROCESS(aSerializer, aIntPtrD1);

    int*    aIntPtrS1=&aInt;
    THEOLIZER_PROCESS(aSerializer, aIntPtrS1);

    aInt=56789;
    THEOLIZER_PROCESS_POINTEE(aSerializer, aInt);       // 静的定義領域追跡
    THEOLIZER_PROCESS_OWNER(aSerializer, aIntPtrD);     // 動的生成領域保存

    int*    aIntPtrS2=&aInt;
    THEOLIZER_PROCESS(aSerializer, aIntPtrS2);

    int*    aIntPtrD2=aIntPtrD;
    THEOLIZER_PROCESS(aSerializer, aIntPtrD2);

    int*    aIntPtrS3=&aInt;
    THEOLIZER_PROCESS(aSerializer, aIntPtrS3);

    int*    aIntPtrD3=aIntPtrD;
    THEOLIZER_PROCESS(aSerializer, aIntPtrD3);

    // nullptr
    short*  aShorPtr=nullptr;
    THEOLIZER_PROCESS_OWNER(aSerializer, aShorPtr);     // 動的生成領域保存

    // 2回目の追跡保存→2回目なので保存されない
    *aIntPtrD=-11;
    THEOLIZER_PROCESS_OWNER(aSerializer, aIntPtrD);     // 動的生成領域保存

#if 0   // ポインタのポインタはサボートしない
    int**       aIntPtr2=&aIntPtr;
    THEOLIZER_PROCESS(aSerializer, aIntPtr2);
#endif

//      ---<<< 参照 >>>---

    // 追跡する場合は、2回目の保存を検出するのでエラーになる
    int&    aIntRef=aInt;
    aIntRef=-22;
#if 0   // エラー・テスト用コードにてテストするべき
    THEOLIZER_PROCESS_POINTEE(aSerializer, aIntRef);    // 静的定義領域追跡
#endif

    // 追跡しない場合は、保存される
    THEOLIZER_PROCESS(aSerializer, aIntRef);

    aSerializer.clearTracking();

//      ---<<< clearTracking()後に同じ領域を再度保存 >>>---

    // 3回目の追跡保存→3回目だが、clearTracking()しているので保存される
    *aIntPtrD=-33;
    THEOLIZER_PROCESS_OWNER(aSerializer, aIntPtrD);     // 動的生成領域保存

    // 3回目の追跡保存→3回目だが、clearTracking()しているので保存される
    aIntRef=-44;
    THEOLIZER_PROCESS_POINTEE(aSerializer, aIntRef);    // 静的定義領域追跡

    // 後始末
    aSerializer.clearTracking();
}

// ---------------------------------------------------------------------------
//      デシリアライズ
// ---------------------------------------------------------------------------

template<class tISerializer, typename... tArgs>
void TestISerializerPointer
(
    string const& iFileName,
    std::ios::openmode iOpenMode,
    unsigned iGlovalVersionNo,
    tArgs... iArgs
)
{
std::cout << "TestISerializerPointer(" << iFileName << ", "
          << THEOLIZER_INTERNAL_TYPE_NAME(tISerializer) << ")\n";

    std::ifstream   aStream(iFileName, iOpenMode);
    tISerializer    aSerializer(aStream, iArgs...);

//      ---<<< 静的定義領域／動的生成領域への前方／後方ポイント >>>---

    int*    aIntPtrD0;
    THEOLIZER_PROCESS(aSerializer, aIntPtrD0);

    int*    aIntPtrS0;
    THEOLIZER_PROCESS(aSerializer, aIntPtrS0);

    int*    aIntPtrD1;
    THEOLIZER_PROCESS(aSerializer, aIntPtrD1);

    int*    aIntPtrS1;
    THEOLIZER_PROCESS(aSerializer, aIntPtrS1);

    int     aInt=0;
    THEOLIZER_PROCESS_POINTEE(aSerializer, aInt);       // 静的定義領域追跡
    THEOLIZER_EQUAL(aInt, 56789);

    int*    aIntPtrD=nullptr;
    auto aAutoDeleter=theolizer::makeScopeExit([&]{delete aIntPtrD;});
    THEOLIZER_PROCESS_OWNER(aSerializer, aIntPtrD);     // 動的生成領域回復
    THEOLIZER_EQUAL(*aIntPtrD, 12345);

    int*    aIntPtrS2;
    THEOLIZER_PROCESS(aSerializer, aIntPtrS2);

    int*    aIntPtrD2;
    THEOLIZER_PROCESS(aSerializer, aIntPtrD2);

    int*    aIntPtrS3;
    THEOLIZER_PROCESS(aSerializer, aIntPtrS3);

    int*    aIntPtrD3;
    THEOLIZER_PROCESS(aSerializer, aIntPtrD3);

    // nullptr
    short*  aShorPtr=nullptr;
    THEOLIZER_PROCESS_OWNER(aSerializer, aShorPtr);     // 動的生成領域回復
    THEOLIZER_CHECK(aShorPtr == nullptr, aShorPtr);

    // 2回目の追跡回復→2回目なので値が変化しない
    aIntPtrD=new int(100);
    THEOLIZER_PROCESS_OWNER(aSerializer, aIntPtrD);     // 動的生成領域回復
    THEOLIZER_EQUAL(*aIntPtrD, 12345);

#if 0   // ポインタのポインタはサボートしない
    int**       aIntPtr2;
    THEOLIZER_PROCESS(aSerializer, aIntPtr2);
#endif

//      ---<<< 参照 >>>---

    // 追跡する場合は、2回目の回復を検出するのでエラーになる
    int&    aIntRef=aInt;
#if 0   // エラー・テスト用コードにてテストするべきだが、保存できないので困難
    THEOLIZER_PROCESS_POINTEE(aSerializer, aIntRef);
    THEOLIZER_EQUAL(aIntRef, 56789);
#endif

    // 追跡しない場合は、回復される
    // 参照先も当然回復されるので、この例では回復処理中に値が変化する
    // このような使い方は推奨しない
    THEOLIZER_PROCESS(aSerializer, aIntRef);
    THEOLIZER_EQUAL(aIntRef, -22);
    THEOLIZER_EQUAL(aInt, -22);

    aSerializer.clearTracking();

    THEOLIZER_EQUAL(aIntPtrS0, &aInt);
    THEOLIZER_EQUAL(aIntPtrS1, &aInt);
    THEOLIZER_EQUAL(aIntPtrS2, &aInt);
    THEOLIZER_EQUAL(aIntPtrS3, &aInt);

    THEOLIZER_EQUAL(aIntPtrD0, aIntPtrD);
    THEOLIZER_EQUAL(aIntPtrD1, aIntPtrD);
    THEOLIZER_EQUAL(aIntPtrD2, aIntPtrD);
    THEOLIZER_EQUAL(aIntPtrD3, aIntPtrD);

//      ---<<< clearTracking()後に同じ領域を再度回復 >>>---

    // 3回目の追跡回復→3回目だが、clearTracking()しているので回復される
    THEOLIZER_PROCESS_OWNER(aSerializer, aIntPtrD);     // 動的生成領域回復
    THEOLIZER_EQUAL(*aIntPtrD, -33);

    // 3回目の追跡回復→3回目だが、clearTracking()しているので保存される
    THEOLIZER_PROCESS_POINTEE(aSerializer, aIntRef);    // 静的定義領域追跡
    THEOLIZER_EQUAL(aIntRef, -44);

    // 後始末
    aSerializer.clearTracking();
}

//############################################################################
//      基本テスト呼び出し
//############################################################################

// ***************************************************************************
//      オブジェクト追跡テスト用
// ***************************************************************************

// ---------------------------------------------------------------------------
//      各派生Serializer呼び出し
// ---------------------------------------------------------------------------

//      ---<<< プライマリー >>>---

template<class tOSerialzer, class tISerializer>
void TestObjectTrackingImpl
(
    string const& iPreFix,
    unsigned iGlobalVersionNo,
    theolizer::CheckMode iCheckMode,
    bool iNoPrettyPrint=false
)
{
}

//      ---<<< Fast特殊化 >>>---

template<>
void TestObjectTrackingImpl<theolizer::FastOSerializer<>, theolizer::FastISerializer<> >
(
    string const& iPreFix,
    unsigned iGlobalVersionNo,
    theolizer::CheckMode iCheckMode,
    bool iNoPrettyPrint
)
{
//      ---<<< 様々なオブジェクト追跡のテスト >>>---

    string aFileName=iPreFix+"_tracking.log";
    TestOSerializerTracking<theolizer::FastOSerializer<> >
    (
        aFileName,
        std::ios::binary
    );
    TestISerializerTracking<theolizer::FastISerializer<> >
    (
        aFileName,
        std::ios::binary,
        iGlobalVersionNo
    );

//      ---<<< 複雑なポインタと参照のテスト >>>---

    aFileName=iPreFix+"_pointer.log";
    TestOSerializerPointer<theolizer::FastOSerializer<> >
    (
        aFileName,
        std::ios::binary
    );
    TestISerializerPointer<theolizer::FastISerializer<> >
    (
        aFileName,
        std::ios::binary,
        iGlobalVersionNo
    );
}

//      ---<<< Json特殊化 >>>---

template<>
void TestObjectTrackingImpl<theolizer::JsonOSerializer<>, theolizer::JsonISerializer<>>
(
    string const& iPreFix,
    unsigned iGlobalVersionNo,
    theolizer::CheckMode iCheckMode,
    bool iNoPrettyPrint
)
{
//      ---<<< 様々なオブジェクト追跡のテスト >>>---

    string aFileName=iPreFix+"_tracking.log";
    TestOSerializerTracking<theolizer::JsonOSerializer<> >
    (
        aFileName,
        std::ios::openmode(),
        iGlobalVersionNo,
        iCheckMode,
        iNoPrettyPrint
    );
    TestISerializerTracking<theolizer::JsonISerializer<> >
    (
        aFileName,
        std::ios::openmode(),
        iGlobalVersionNo
    );

//      ---<<< 複雑なポインタと参照のテスト >>>---

    aFileName=iPreFix+"_pointer.log";
    TestOSerializerPointer<theolizer::JsonOSerializer<> >
    (
        aFileName,
        std::ios::openmode(),
        iGlobalVersionNo,
        iCheckMode,
        iNoPrettyPrint
    );
    TestISerializerPointer<theolizer::JsonISerializer<> >
    (
        aFileName,
        std::ios::openmode(),
        iGlobalVersionNo
    );
}

//      ---<<< Bainary特殊化 >>>---

template<>
void TestObjectTrackingImpl<theolizer::BinaryOSerializer<>, theolizer::BinaryISerializer<>>
(
    string const& iPreFix,
    unsigned iGlobalVersionNo,
    theolizer::CheckMode iCheckMode,
    bool iNoPrettyPrint
)
{
//      ---<<< 様々なオブジェクト追跡のテスト >>>---

    string aFileName=iPreFix+"_tracking.log";
    TestOSerializerTracking<theolizer::BinaryOSerializer<> >
    (
        aFileName,
        std::ios::binary,
        iGlobalVersionNo,
        iCheckMode
    );
    TestISerializerTracking<theolizer::BinaryISerializer<> >
    (
        aFileName,
        std::ios::binary,
        iGlobalVersionNo
    );

//      ---<<< 複雑なポインタと参照のテスト >>>---

    aFileName=iPreFix+"_pointer.log";
    TestOSerializerPointer<theolizer::BinaryOSerializer<> >
    (
        aFileName,
        std::ios::binary,
        iGlobalVersionNo,
        iCheckMode
    );
    TestISerializerPointer<theolizer::BinaryISerializer<> >
    (
        aFileName,
        std::ios::binary,
        iGlobalVersionNo
    );
}

// ---------------------------------------------------------------------------
//      まとめ処理
// ---------------------------------------------------------------------------

template<class tOSerialzer, class tISerializer>
void TestObjectTracking(char const* iPreFix, bool iIsFast=false)
{
//      ---<<< InMemory形式 >>>---

    if (iIsFast)
    {
        string aFileName = string(iPreFix) + "_in_memory";
        TestObjectTrackingImpl<tOSerialzer, tISerializer>(aFileName,
            0, theolizer::CheckMode::InMemory);
return;
    }

//      ---<<< NoTypeCheck形式 >>>---

    {
        unsigned aVerNo=theolizer::kLastGlobalVersionNo;
        string aFileName=string(iPreFix)+"_no_check";
        aFileName += std::to_string(aVerNo);
        TestObjectTrackingImpl<tOSerialzer, tISerializer>(aFileName,
            aVerNo, theolizer::CheckMode::NoTypeCheck);
    }

//      ---<<< TypeCheck形式 >>>---

    {
        unsigned aVerNo=theolizer::kLastGlobalVersionNo;
        string aFileName=string(iPreFix)+"_check";
        aFileName += std::to_string(aVerNo);
        TestObjectTrackingImpl<tOSerialzer, tISerializer>(aFileName,
            aVerNo, theolizer::CheckMode::TypeCheck);
    }

//      ---<<< TypeCheckByIndex形式 >>>---

    {
        unsigned aVerNo=theolizer::kLastGlobalVersionNo;
        string aFileName=string(iPreFix)+"_check_index";
        aFileName += std::to_string(aVerNo);
        TestObjectTrackingImpl<tOSerialzer, tISerializer>(aFileName,
            aVerNo, theolizer::CheckMode::TypeCheckByIndex);
    }
}

// ***************************************************************************
//      メイン
// ***************************************************************************

void TestTrackingMain()
{
//  theolizer::DisplayPass aDisplayPass;

    TestObjectTracking<theolizer::FastOSerializer<>, theolizer::FastISerializer<>>
        ("test_fast", true);

    TestObjectTracking<theolizer::JsonOSerializer<>, theolizer::JsonISerializer<>>
        ("test_json");

    TestObjectTracking<theolizer::BinaryOSerializer<>,theolizer::BinaryISerializer<>>
        ("test_binary");
}
