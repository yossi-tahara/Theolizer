//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          オブジェクト追跡2
//              MinGWのFile too bigエラー対策のため分割。
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

#include "disable_test.h"
#ifndef DISABLE_OBJECT_TRACKING2_TEST

// ***************************************************************************
//      インクルード
// ***************************************************************************

// 標準ライブラリ
#include <iostream>
#include <fstream>
#include <ctime>

// theolizerライブラリ
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_json.h>
#include <theolizer/memory.h>

// プロジェクト・ヘッダ
#include "common.h"
#include "test_object_tracking.h"

// 自動生成ソース
#include "test_object_tracking2.cpp.theolizer.hpp"

//############################################################################
//      2番目以降の基底クラス・ポインタのオブジェクト追跡テスト
//      組み合わせテスト
//############################################################################

// ***************************************************************************
//      静的定義領域（グローバル変数）
// ***************************************************************************

//      ---<<< 保存側 >>>---

long            gLongSave{101};             // プリミティブ
ObjectTracking0 gObjectTracking0Save{201};  // Pointeeなしクラス
ObjectTracking1 gObjectTracking1Save{301};  // Pointeeありクラス

//      ---<<< 回復側 >>>---

long            gLongLoad;              // プリミティブ
ObjectTracking0 gObjectTracking0Load;   // Pointeeなしクラス
ObjectTracking1 gObjectTracking1Load;   // Pointeeありクラス

// ***************************************************************************
//      保存
// ***************************************************************************

template<class tSerializer>
void saveObjectTracking(tSerializer& iSerializer)
{
    // オブジェクト追跡前準備
    //  ここに来るまでにオブジェクト追跡テーブルに登録され、その領域が開放されるので必要。
    //  (オブジェクト追跡単位内でシリアライズした領域を開放してはいけない。)
    iSerializer.clearTracking();

//----------------------------------------------------------------------------
//      各種メモリへのポインタのテスト
//          2番目以降の基底クラス・ポインタからのオブジェクト追跡テスト含む
//
//          shared_ptrの2番目以降の基底クラスによる保存／回復テストを含む。
//          ただし、shared_ptrの網羅的テストはコンテナの自動テストで行う。
//----------------------------------------------------------------------------

    {

//      ---<<< データ構造生成 >>>---

        // 静的定義領域（ローカル変数）
        long            aLong{102};             // プリミティブ
        ObjectTracking0 aObjectTracking0{202};  // Pointeeなしクラス
        ObjectTracking1 aObjectTracking1{302};  // Pointeeありクラス

        // 静的定義領域
        std::unique_ptr<StaticDefinition>   aStaticDefinition{new StaticDefinition{true}};

        // 動的定義領域(クラス側は2番目以降の基底クラス・ポインタのテスト含む)
        std::shared_ptr<long>                   aDynamicLong{new long{500}};
        std::shared_ptr<ObjectTrackingDerived>  aDynamicClass{new ObjectTrackingDerived{true}};

        // ポインタ設定(THEOLIZER_PROCESS用)
        //  グローバル変数ポイント用
        long*               aGlobalLongPtr              =&gLongSave;
        ObjectTracking0*    aGlobalObjectTracking0Ptr   =&gObjectTracking0Save;
        ObjectTracking1*    aGlobalObjectTracking1Ptr   =&gObjectTracking1Save;

        //  ローカル変数ポイント用
        long*               aLocalLongPtr               =&aLong;
        ObjectTracking0*    aLocalObjectTracking0Ptr    =&aObjectTracking0;
        ObjectTracking1*    aLocalObjectTracking1Ptr    =&aObjectTracking1;

        //  メンバ変数ポイント用
        long*               aMemberLongPtr              =&(aStaticDefinition->mLong);
        ObjectTracking0*    aMemberObjectTracking0Ptr   =&(aStaticDefinition->mObjectTracking0);
        ObjectTracking1*    aMemberObjectTracking1Ptr   =&(aStaticDefinition->mObjectTracking1);

        // ポインタ設定(自動コード生成用)
        Pointers    aPointers;
        aPointers.mGlobalLongPtr            =&gLongSave;
        aPointers.mGlobalObjectTracking0Ptr =&gObjectTracking0Save;
        aPointers.mGlobalObjectTracking1Ptr =&gObjectTracking1Save;
        aPointers.mLocalLongPtr             =&aLong;
        aPointers.mLocalObjectTracking0Ptr  =&aObjectTracking0;
        aPointers.mLocalObjectTracking1Ptr  =&aObjectTracking1;
        aPointers.mMemberLongPtr            =&(aStaticDefinition->mLong);
        aPointers.mMemberObjectTracking0Ptr =&(aStaticDefinition->mObjectTracking0);
        aPointers.mMemberObjectTracking1Ptr =&(aStaticDefinition->mObjectTracking1);
        aPointers.mDynamicLong              =aDynamicLong;
        aPointers.mDynamicClass             =aDynamicClass;

//      ---<<< 保存処理 >>>---

        // ポインタ(THEOLIZER_PROCESS用)保存
        THEOLIZER_PROCESS(iSerializer, aGlobalLongPtr);
        THEOLIZER_PROCESS(iSerializer, aGlobalObjectTracking0Ptr);
        THEOLIZER_PROCESS(iSerializer, aGlobalObjectTracking1Ptr);
        THEOLIZER_PROCESS(iSerializer, aLocalLongPtr);
        THEOLIZER_PROCESS(iSerializer, aLocalObjectTracking0Ptr);
        THEOLIZER_PROCESS(iSerializer, aLocalObjectTracking1Ptr);
        THEOLIZER_PROCESS(iSerializer, aMemberLongPtr);
        THEOLIZER_PROCESS(iSerializer, aMemberObjectTracking0Ptr);
        THEOLIZER_PROCESS(iSerializer, aMemberObjectTracking1Ptr);

        // 動的生成領域保存
        THEOLIZER_PROCESS(iSerializer, aDynamicLong);
        THEOLIZER_PROCESS(iSerializer, aDynamicClass);

        // インスタンス保存
        THEOLIZER_PROCESS_POINTEE(iSerializer, gLongSave);
        THEOLIZER_PROCESS(iSerializer, gObjectTracking0Save);
        THEOLIZER_PROCESS_POINTEE(iSerializer, gObjectTracking1Save);
        THEOLIZER_PROCESS_POINTEE(iSerializer, aLong);
        THEOLIZER_PROCESS(iSerializer, aObjectTracking0);
        THEOLIZER_PROCESS_POINTEE(iSerializer, aObjectTracking1);
        THEOLIZER_PROCESS(iSerializer, aStaticDefinition);

        // ポインタ(自動シリアライズ用)保存
        THEOLIZER_PROCESS(iSerializer, aPointers);

//      ---<<< 2番目以降の基底クラス・ポインタ(静的定義領域) >>>---

        ObjectTrackingDerived   aObjectTrackingDerived;
        THEOLIZER_PROCESS(iSerializer, aObjectTrackingDerived);

        ObjectTrackingBase1* aObjectTrackingBase1 = &aObjectTrackingDerived;
        THEOLIZER_PROCESS(iSerializer, aObjectTrackingBase1);

        iSerializer.clearTracking();
    }

//----------------------------------------------------------------------------
//      通常ポインタのテスト
//----------------------------------------------------------------------------

//      ---<<< 手動(トップ・レベル)によるポインタの保存 >>>---

    {
        // ポイント先群
        PointeeList         aPointeeList{true};
        PointeeList         aPointeeList2{2};
        PointeeListManual   aPointeeListManual{true};

        // ボインタ群
        PointerList aPointerListA{aPointeeList,  aPointeeList2};
        PointerList aPointerListB{aPointeeList,  aPointeeListManual};
        PointerList aPointerListC{aPointeeList2, aPointeeListManual};

        // ポイント先保存1：手動(トップ・レベル)
        std::cout << "        savePointee() : aPointeeList" << std::endl;
        savePointee(iSerializer, aPointeeList);

        // ポイント先保存前／後にポインタ保存
        std::cout << "        savePointer() : aPointerListA" << std::endl;
        savePointer(iSerializer, aPointerListA);

        // ポイント先保存2：自動メンバ・リスト生成
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList2" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList2);

        // ポイント先保存前／後にポインタ保存
        std::cout << "        savePointer() : aPointerListB" << std::endl;
        savePointer(iSerializer, aPointerListB);

        // ポイント先保存3：手動(非トップ・レベル)
        std::cout << "        THEOLIZER_PROCESS() : aPointeeListManual" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeListManual);

        // ポイント先保存後にポインタ保存
        std::cout << "        savePointer() : aPointerListC" << std::endl;
        savePointer(iSerializer, aPointerListC);

        iSerializer.clearTracking();
    }

//      ---<<< 自動メンバ・リスト生成によるポインタの保存 >>>---

    {
        // ポイント先群
        PointeeList         aPointeeList{true};
        PointeeList         aPointeeList2{2};
        PointeeListManual   aPointeeListManual{true};

        // ボインタ群
        PointerList aPointerListA{aPointeeList,  aPointeeList2};
        PointerList aPointerListB{aPointeeList,  aPointeeListManual};
        PointerList aPointerListC{aPointeeList2, aPointeeListManual};

        // ポイント先保存1：手動(トップ・レベル)
        std::cout << "        savePointee() : aPointeeList" << std::endl;
        savePointee(iSerializer, aPointeeList);

        // ポイント先保存前／後にポインタ保存
        std::cout << "        THEOLIZER_PROCESS() : aPointerListA" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointerListA);

        // ポイント先保存2：自動メンバ・リスト生成
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList2" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList2);

        // ポイント先保存前／後にポインタ保存
        std::cout << "        THEOLIZER_PROCESS() : aPointerListB" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointerListB);

        // ポイント先保存3：手動(非トップ・レベル)
        std::cout << "        THEOLIZER_PROCESS() : aPointeeListManual" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeListManual);

        // ポイント先保存後にポインタ保存
        std::cout << "        THEOLIZER_PROCESS() : aPointerListC" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointerListC);

        iSerializer.clearTracking();
    }

//      ---<<< 手動(非トップ・レベル)によるポインタの保存 >>>---

    {
        // ポイント先群
        PointeeList         aPointeeList{true};
        PointeeList         aPointeeList2{2};
        PointeeListManual   aPointeeListManual{true};

        // ポインタ群
        ManualClass4PointerList aManualClass4PointerListA{aPointeeList,  aPointeeList2};
        ManualClass4PointerList aManualClass4PointerListB{aPointeeList,  aPointeeListManual};
        ManualClass4PointerList aManualClass4PointerListC{aPointeeList2, aPointeeListManual};

        // ポイント先保存1：手動(トップ・レベル)
        std::cout << "        savePointee() : aPointeeList" << std::endl;
        savePointee(iSerializer, aPointeeList);

        // ポイント先保存前／後にポインタ保存
        std::cout << "        THEOLIZER_PROCESS() : aManualClass4PointerListA" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aManualClass4PointerListA);

        // ポイント先保存2：自動メンバ・リスト生成
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList2" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList2);

        // ポイント先保存前／後にポインタ保存
        std::cout << "        THEOLIZER_PROCESS() : aManualClass4PointerListB" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aManualClass4PointerListB);

        // ポイント先保存3：手動(非トップ・レベル)
        std::cout << "        THEOLIZER_PROCESS() : aPointeeListManual" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeListManual);

        // ポイント先保存後にポインタ保存
        std::cout << "        THEOLIZER_PROCESS() : aManualClass4PointerListC" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aManualClass4PointerListC);

        iSerializer.clearTracking();
    }
}

INSTANTIATION_ALL(void, saveObjectTracking);

// ***************************************************************************
//      回復
// ***************************************************************************

template<class tSerializer>
void loadObjectTracking(tSerializer& iSerializer)
{
    // オブジェクト追跡前準備
    //  ここに来るまでにオブジェクト追跡テーブルに登録され、その領域が開放されるので必要。
    //  (オブジェクト追跡単位内でシリアライズした領域を開放してはいけない。)
    iSerializer.clearTracking();

//----------------------------------------------------------------------------
//      各種メモリへのポインタのテスト
//          2番目以降の基底クラス・ポインタからのオブジェクト追跡テスト含む
//
//          shared_ptrの2番目以降の基底クラスによる保存／回復テストを含む。
//          ただし、shared_ptrの網羅的テストはコンテナの自動テストで行う。
//----------------------------------------------------------------------------

    {

//      ---<<< 回復先領域準備 >>>---

        // 静的定義領域（ローカル変数）
        long            aLong{0};               // プリミティブ
        ObjectTracking0 aObjectTracking0{};     // Pointeeなしクラス
        ObjectTracking1 aObjectTracking1{};     // Pointeeありクラス

        // 静的定義領域（メンバ変数）
        std::unique_ptr<StaticDefinition>   aStaticDefinition{new StaticDefinition{}};

        // 動的定義領域
        std::shared_ptr<long>                   aDynamicLong{};
        std::shared_ptr<ObjectTrackingDerived>  aDynamicClass{};

        // ポインタ設定(THEOLIZER_PROCESS用)
        //  グローバル変数ポイント用
        long*               aGlobalLongPtr              =nullptr;
        ObjectTracking0*    aGlobalObjectTracking0Ptr   =nullptr;
        ObjectTracking1*    aGlobalObjectTracking1Ptr   =nullptr;

        //  ローカル変数ポイント用
        long*               aLocalLongPtr               =nullptr;
        ObjectTracking0*    aLocalObjectTracking0Ptr    =nullptr;
        ObjectTracking1*    aLocalObjectTracking1Ptr    =nullptr;

        //  メンバ変数ポイント用
        long*               aMemberLongPtr              =nullptr;
        ObjectTracking0*    aMemberObjectTracking0Ptr   =nullptr;
        ObjectTracking1*    aMemberObjectTracking1Ptr   =nullptr;

        // ポインタ領域(自動シリアライズ用)
        Pointers    aPointers;

//      ---<<< 回復処理 >>>---

        // ポインタ(THEOLIZER_PROCESS用)回復
        THEOLIZER_PROCESS(iSerializer, aGlobalLongPtr);
        THEOLIZER_PROCESS(iSerializer, aGlobalObjectTracking0Ptr);
        THEOLIZER_PROCESS(iSerializer, aGlobalObjectTracking1Ptr);
        THEOLIZER_PROCESS(iSerializer, aLocalLongPtr);
        THEOLIZER_PROCESS(iSerializer, aLocalObjectTracking0Ptr);
        THEOLIZER_PROCESS(iSerializer, aLocalObjectTracking1Ptr);
        THEOLIZER_PROCESS(iSerializer, aMemberLongPtr);
        THEOLIZER_PROCESS(iSerializer, aMemberObjectTracking0Ptr);
        THEOLIZER_PROCESS(iSerializer, aMemberObjectTracking1Ptr);

        // 動的生成領域回復
        THEOLIZER_PROCESS(iSerializer, aDynamicLong);
        THEOLIZER_PROCESS(iSerializer, aDynamicClass);

        // インスタンス回復
        THEOLIZER_PROCESS_POINTEE(iSerializer, gLongLoad);
        THEOLIZER_PROCESS(iSerializer, gObjectTracking0Load);
        THEOLIZER_PROCESS_POINTEE(iSerializer, gObjectTracking1Load);
        THEOLIZER_PROCESS_POINTEE(iSerializer, aLong);
        THEOLIZER_PROCESS(iSerializer, aObjectTracking0);
        THEOLIZER_PROCESS_POINTEE(iSerializer, aObjectTracking1);
        THEOLIZER_PROCESS(iSerializer, aStaticDefinition);

        // ポインタ(自動シリアライズ用)回復
        THEOLIZER_PROCESS(iSerializer, aPointers);

//      ---<<< チェック >>>---

        // ポインタ(THEOLIZER_PROCESS用)
        THEOLIZER_EQUAL_PTR(aGlobalLongPtr,             &gLongLoad);
        THEOLIZER_EQUAL_PTR(aGlobalObjectTracking0Ptr,  &gObjectTracking0Load);
        THEOLIZER_EQUAL_PTR(aGlobalObjectTracking1Ptr,  &gObjectTracking1Load);
        THEOLIZER_EQUAL_PTR(aLocalLongPtr,              &aLong);
        THEOLIZER_EQUAL_PTR(aLocalObjectTracking0Ptr,   &aObjectTracking0);
        THEOLIZER_EQUAL_PTR(aLocalObjectTracking1Ptr,   &aObjectTracking1);
        THEOLIZER_EQUAL_PTR(aMemberLongPtr,             &(aStaticDefinition->mLong));
        THEOLIZER_EQUAL_PTR(aMemberObjectTracking0Ptr,  &(aStaticDefinition->mObjectTracking0));
        THEOLIZER_EQUAL_PTR(aMemberObjectTracking1Ptr,  &(aStaticDefinition->mObjectTracking1));

        // 動的生成領域
        THEOLIZER_EQUAL(*(aDynamicLong.get()),          500);
        aDynamicClass->check();

        // インスタンス
        THEOLIZER_EQUAL(gLongLoad,                      101);
        THEOLIZER_EQUAL(gObjectTracking0Load.mInt,      201);
        THEOLIZER_EQUAL(gObjectTracking1Load.mShort,    301);
        THEOLIZER_EQUAL(aLong,                          102);
        THEOLIZER_EQUAL(aObjectTracking0.mInt,          202);
        THEOLIZER_EQUAL(aObjectTracking1.mShort,        302);
        aStaticDefinition->check(true);

        // ポインタ(自動シリアライズ用)
        THEOLIZER_EQUAL_PTR(aPointers.mGlobalLongPtr,               &gLongLoad);
        THEOLIZER_EQUAL_PTR(aPointers.mGlobalObjectTracking0Ptr,    &gObjectTracking0Load);
        THEOLIZER_EQUAL_PTR(aPointers.mGlobalObjectTracking1Ptr,    &gObjectTracking1Load);
        THEOLIZER_EQUAL_PTR(aPointers.mLocalLongPtr,                &aLong);
        THEOLIZER_EQUAL_PTR(aPointers.mLocalObjectTracking0Ptr,     &aObjectTracking0);
        THEOLIZER_EQUAL_PTR(aPointers.mLocalObjectTracking1Ptr,     &aObjectTracking1);
        THEOLIZER_EQUAL_PTR(aPointers.mMemberLongPtr,               &(aStaticDefinition->mLong));
        THEOLIZER_EQUAL_PTR(aPointers.mMemberObjectTracking0Ptr,
            &(aStaticDefinition->mObjectTracking0));
        THEOLIZER_EQUAL_PTR(aPointers.mMemberObjectTracking1Ptr,
            &(aStaticDefinition->mObjectTracking1));

        THEOLIZER_EQUAL_PTR(aPointers.mDynamicLong.get(),           aDynamicLong.get());
        THEOLIZER_EQUAL_PTR(aPointers.mDynamicClass.get(),          aDynamicClass.get());

//      ---<<< 2番目以降の基底クラス・ポインタ(静的定義領域) >>>---

        ObjectTrackingDerived   aObjectTrackingDerived;
        THEOLIZER_PROCESS(iSerializer, aObjectTrackingDerived);

        ObjectTrackingBase1* aObjectTrackingBase1 = nullptr;
        THEOLIZER_PROCESS(iSerializer, aObjectTrackingBase1);

        iSerializer.clearTracking();

        THEOLIZER_EQUAL_PTR(&aObjectTrackingDerived, aObjectTrackingBase1);
    }

//----------------------------------------------------------------------------
//      通常ポインタのテスト
//----------------------------------------------------------------------------

//      ---<<< 手動(トップ・レベル)によるポインタ（左辺値）の回復 >>>---

    {
        // ポイント先群
        PointeeList         aPointeeList;
        PointeeList         aPointeeList2;
        PointeeListManual   aPointeeListManual;

        // ポインタ群
        PointerList aPointerListA;
        PointerList aPointerListB;
        PointerList aPointerListC;

        // ポイント先回復1：手動(トップ・レベル)
        std::cout << "        loadPointee() : aPointeeList" << std::endl;
        loadPointee(iSerializer, aPointeeList);
        aPointeeList.check(true);

        // ポイント先回復前／後にポインタ回復
        std::cout << "        loadPointer() : aPointerListA" << std::endl;
        loadPointer(iSerializer, aPointerListA);

        // ポイント先回復2：自動メンバ・リスト生成
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList2" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList2);
        aPointeeList2.check(2);

        // ポイント先回復前／後にポインタ回復
        std::cout << "        loadPointer() : aPointerListB" << std::endl;
        loadPointer(iSerializer, aPointerListB);

        // ポイント先回復3：手動(非トップ・レベル)
        std::cout << "        THEOLIZER_PROCESS() : aPointeeListManual" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeListManual);
        aPointeeListManual.check(true);

        // ポイント先回復後にポインタ回復
        std::cout << "        loadPointer() : aPointerListC" << std::endl;
        loadPointer(iSerializer, aPointerListC);

        // アドレス解決確認
        iSerializer.clearTracking();

        // 回復結果のチェック
        aPointerListA.check(aPointeeList,  aPointeeList2);
        aPointerListB.check(aPointeeList,  aPointeeListManual);
        aPointerListC.check(aPointeeList2, aPointeeListManual);
    }

//      ---<<< 自動メンバ・リスト生成によるポインタの回復 >>>---

    {
        // ポイント先群
        PointeeList         aPointeeList;
        PointeeList         aPointeeList2;
        PointeeListManual   aPointeeListManual;

        // ポインタ群
        PointerList aPointerListA;
        PointerList aPointerListB;
        PointerList aPointerListC;

        // ポイント先回復1：手動(トップ・レベル)
        std::cout << "        loadPointee() : aPointeeList" << std::endl;
        loadPointee(iSerializer, aPointeeList);
        aPointeeList.check(true);

        // ポイント先回復前／後にポインタ回復
        std::cout << "        THEOLIZER_PROCESS() : aPointerListA" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointerListA);

        // ポイント先回復2：自動メンバ・リスト生成
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList2" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList2);
        aPointeeList2.check(2);

        // ポイント先回復前／後にポインタ回復
        std::cout << "        THEOLIZER_PROCESS() : aPointerListB" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointerListB);

        // ポイント先回復3：手動(非トップ・レベル)
        std::cout << "        THEOLIZER_PROCESS() : aPointeeListManual" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeListManual);
        aPointeeListManual.check(true);

        // ポイント先回復後にポインタ回復
        std::cout << "        THEOLIZER_PROCESS() : aPointerListC" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointerListC);

        // アドレス解決確認
        iSerializer.clearTracking();

        // 回復結果のチェック
        aPointerListA.check(aPointeeList,  aPointeeList2);
        aPointerListB.check(aPointeeList,  aPointeeListManual);
        aPointerListC.check(aPointeeList2, aPointeeListManual);
    }

//      ---<<< 手動(非トップ・レベル)によるポインタの回復 >>>---

    {
        // ポイント先群
        PointeeList         aPointeeList;
        PointeeList         aPointeeList2;
        PointeeListManual   aPointeeListManual;

        // ポインタ
        ManualClass4PointerList aManualClass4PointerListA;
        ManualClass4PointerList aManualClass4PointerListB;
        ManualClass4PointerList aManualClass4PointerListC;

        // ポイント先回復1：手動(トップ・レベル)
        std::cout << "        loadPointee() : aPointeeList" << std::endl;
        loadPointee(iSerializer, aPointeeList);
        aPointeeList.check(true);

        // ポイント先回復前／後にポインタ回復
        std::cout << "        THEOLIZER_PROCESS() : aManualClass4PointerListA" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aManualClass4PointerListA);

        // ポイント先回復2：自動メンバ・リスト生成
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList2" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList2);
        aPointeeList2.check(2);

        // ポイント先回復前／後にポインタ回復
        std::cout << "        THEOLIZER_PROCESS() : aManualClass4PointerListB" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aManualClass4PointerListB);

        // ポイント先回復3：手動(非トップ・レベル)
        std::cout << "        THEOLIZER_PROCESS() : aPointeeListManual" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeListManual);
        aPointeeListManual.check(true);

        // ポイント先回復後にポインタ回復
        std::cout << "        THEOLIZER_PROCESS() : aManualClass4PointerListC" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aManualClass4PointerListC);

        // アドレス解決確認
        iSerializer.clearTracking();

        // 回復結果のチェック
        aManualClass4PointerListA.check(aPointeeList,  aPointeeList2);
        aManualClass4PointerListB.check(aPointeeList,  aPointeeListManual);
        aManualClass4PointerListC.check(aPointeeList2, aPointeeListManual);
    }
}

INSTANTIATION_ALL(void, loadObjectTracking);

#endif  // DISABLE_OBJECT_TRACKING2_TEST
