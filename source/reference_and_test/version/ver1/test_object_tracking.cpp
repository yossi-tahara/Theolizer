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

#include "disable_test.h"
#ifndef DISABLE_OBJECT_TRACKING_TEST

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
#include "test_object_tracking.cpp.theolizer.hpp"

//############################################################################
//      使い方の説明
//############################################################################

//----------------------------------------------------------------------------
//      静的定義領域（グローバル変数）
//----------------------------------------------------------------------------

//      ---<<< 保存側 >>>---

long            gLongSave{101};             // プリミティブ
ObjectTracking0 gObjectTracking0Save{201};  // Pointeeなしクラス
ObjectTracking1 gObjectTracking1Save{301};  // Pointeeありクラス

//      ---<<< 回復側 >>>---

long            gLongLoad;              // プリミティブ
ObjectTracking0 gObjectTracking0Load;   // Pointeeなしクラス
ObjectTracking1 gObjectTracking1Load;   // Pointeeありクラス

// ***************************************************************************
//      動作確認
// ***************************************************************************

void tutoriseObjectTracking()
{
    std::cout << "tutoriseObjectTracking() start" << std::endl;

//----------------------------------------------------------------------------
//      保存
//----------------------------------------------------------------------------

    {

//      ---<<< 静的定義領域（ローカル変数） >>>---

        long            aLong{102};             // プリミティブ
        ObjectTracking0 aObjectTracking0{202};  // Pointeeなしクラス
        ObjectTracking1 aObjectTracking1{302};  // Pointeeありクラス

//      ---<<< 静的定義領域（メンバ変数） >>>---

        std::unique_ptr<StaticDefinition>   aStaticDefinition{new StaticDefinition{true}};

//      ---<<< 動的定義領域 >>>---

        std::shared_ptr<long>   aDynamicLong{new long{500}};

//      ---<<< ポインタ設定(THEOLIZER_PROCESS用) >>>---

        // グローバル変数ポイント用
        long*               aGlobalLongPtr              =&gLongSave;
        ObjectTracking0*    aGlobalObjectTracking0Ptr   =&gObjectTracking0Save;
        ObjectTracking1*    aGlobalObjectTracking1Ptr   =&gObjectTracking1Save;

        // ローカル変数ポイント用
        long*               aLocalLongPtr               =&aLong;
        ObjectTracking0*    aLocalObjectTracking0Ptr    =&aObjectTracking0;
        ObjectTracking1*    aLocalObjectTracking1Ptr    =&aObjectTracking1;

        // メンバ変数ポイント用
        long*               aMemberLongPtr              =&(aStaticDefinition->mLong);
        ObjectTracking0*    aMemberObjectTracking0Ptr   =&(aStaticDefinition->mObjectTracking0);
        ObjectTracking1*    aMemberObjectTracking1Ptr   =&(aStaticDefinition->mObjectTracking1);

//      ---<<< ポインタ設定(自動シリアライズ用) >>>---

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
//        aPointers.mDynamicLong              =aDynamicLong;

//      ---<<< 保存処理 >>>---

        std::ofstream   aStream("tutorise_object_tracking.json");
        theolizer::JsonOSerializer<> aSerializer(aStream);

        // ポインタ(THEOLIZER_PROCESS用)保存
        THEOLIZER_PROCESS(aSerializer, aGlobalLongPtr);
        THEOLIZER_PROCESS(aSerializer, aGlobalObjectTracking0Ptr);
        THEOLIZER_PROCESS(aSerializer, aGlobalObjectTracking1Ptr);
        THEOLIZER_PROCESS(aSerializer, aLocalLongPtr);
        THEOLIZER_PROCESS(aSerializer, aLocalObjectTracking0Ptr);
        THEOLIZER_PROCESS(aSerializer, aLocalObjectTracking1Ptr);
        THEOLIZER_PROCESS(aSerializer, aMemberLongPtr);
        THEOLIZER_PROCESS(aSerializer, aMemberObjectTracking0Ptr);
        THEOLIZER_PROCESS(aSerializer, aMemberObjectTracking1Ptr);

        // 動的生成領域保存
        THEOLIZER_PROCESS(aSerializer, aDynamicLong);

        // インスタンス保存
        THEOLIZER_PROCESS_POINTEE(aSerializer, gLongSave);
        THEOLIZER_PROCESS(aSerializer, gObjectTracking0Save);
        THEOLIZER_PROCESS_POINTEE(aSerializer, gObjectTracking1Save);
        THEOLIZER_PROCESS_POINTEE(aSerializer, aLong);
        THEOLIZER_PROCESS(aSerializer, aObjectTracking0);
        THEOLIZER_PROCESS_POINTEE(aSerializer, aObjectTracking1);
        THEOLIZER_PROCESS(aSerializer, aStaticDefinition);

        // ポインタ(自動シリアライズ用)保存
        THEOLIZER_PROCESS(aSerializer, aPointers);

        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();
    }

//----------------------------------------------------------------------------
//      回復
//----------------------------------------------------------------------------

    {

//      ---<<< 静的定義領域（ローカル変数） >>>---

        long            aLong{0};               // プリミティブ
        ObjectTracking0 aObjectTracking0{};     // Pointeeなしクラス
        ObjectTracking1 aObjectTracking1{};     // Pointeeありクラス

//      ---<<< 静的定義領域（メンバ変数） >>>---

        std::unique_ptr<StaticDefinition>   aStaticDefinition{new StaticDefinition{}};

//      ---<<< 動的定義領域 >>>---

        std::shared_ptr<long>   aDynamicLong{};

//      ---<<< ポインタ設定(THEOLIZER_PROCESS用) >>>---

        // グローバル変数ポイント用
        long*               aGlobalLongPtr              =nullptr;
        ObjectTracking0*    aGlobalObjectTracking0Ptr   =nullptr;
        ObjectTracking1*    aGlobalObjectTracking1Ptr   =nullptr;

        // ローカル変数ポイント用
        long*               aLocalLongPtr               =nullptr;
        ObjectTracking0*    aLocalObjectTracking0Ptr    =nullptr;
        ObjectTracking1*    aLocalObjectTracking1Ptr    =nullptr;

        // メンバ変数ポイント用
        long*               aMemberLongPtr              =nullptr;
        ObjectTracking0*    aMemberObjectTracking0Ptr   =nullptr;
        ObjectTracking1*    aMemberObjectTracking1Ptr   =nullptr;

//      ---<<< ポインタ設定(自動シリアライズ用) >>>---

        Pointers    aPointers;

//      ---<<< 回復処理 >>>---

        std::ifstream   aStream("tutorise_object_tracking.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        // ポインタ(THEOLIZER_PROCESS用)回復
        THEOLIZER_PROCESS(aSerializer, aGlobalLongPtr);
        THEOLIZER_PROCESS(aSerializer, aGlobalObjectTracking0Ptr);
        THEOLIZER_PROCESS(aSerializer, aGlobalObjectTracking1Ptr);
        THEOLIZER_PROCESS(aSerializer, aLocalLongPtr);
        THEOLIZER_PROCESS(aSerializer, aLocalObjectTracking0Ptr);
        THEOLIZER_PROCESS(aSerializer, aLocalObjectTracking1Ptr);
        THEOLIZER_PROCESS(aSerializer, aMemberLongPtr);
        THEOLIZER_PROCESS(aSerializer, aMemberObjectTracking0Ptr);
        THEOLIZER_PROCESS(aSerializer, aMemberObjectTracking1Ptr);

        // 動的生成領域回復
        THEOLIZER_PROCESS(aSerializer, aDynamicLong);

        // インスタンス回復
        THEOLIZER_PROCESS_POINTEE(aSerializer, gLongLoad);
        THEOLIZER_PROCESS(aSerializer, gObjectTracking0Load);
        THEOLIZER_PROCESS_POINTEE(aSerializer, gObjectTracking1Load);
        THEOLIZER_PROCESS_POINTEE(aSerializer, aLong);
        THEOLIZER_PROCESS(aSerializer, aObjectTracking0);
        THEOLIZER_PROCESS_POINTEE(aSerializer, aObjectTracking1);
        THEOLIZER_PROCESS(aSerializer, aStaticDefinition);

        // ポインタ(自動シリアライズ用)回復
        THEOLIZER_PROCESS(aSerializer, aPointers);

        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();

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

//auto x=aPointers.mDynamicLong.get();
//auto y=aDynamicLong.get();
//        THEOLIZER_EQUAL_PTR(aPointers.mDynamicLong.get(),           aDynamicLong.get());
    }

    std::cout << "tutoriseObjectTracking() end\n" << std::endl;
}

#endif  // DISABLE_OBJECT_TRACKING_TEST
