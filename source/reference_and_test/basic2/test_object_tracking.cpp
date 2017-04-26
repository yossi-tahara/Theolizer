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
#ifdef ENABLE_OBJECT_TRACKING_TEST

// ***************************************************************************
//      インクルード
// ***************************************************************************

// 標準ライブラリ
#include <iostream>
#include <fstream>
#include <ctime>

// theolizerライブラリ
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_fast.h>
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

// ***************************************************************************
//      動作確認
// ***************************************************************************

void tutoriseObjectTracking()
{
//theolizer::DisplayPass aDisplayPass;
    std::cout << "tutoriseObjectTracking() start" << std::endl;

//----------------------------------------------------------------------------
//      保存
//----------------------------------------------------------------------------

    {

//      ---<<< シリアライズ対象変数定義 >>>---

        // 静的定義領域(プリミティブ)
        long    aLong{100};

        // 静的定義領域(クラス)、動的生成領域(メンバ変数)
        ObjectTrackingClass     aObjectTrackingClass{200, 300};

        // 静的定義領域を指すポインタ(非オーナー)
        long*                   aLongPtr=&aLong;
        ObjectTrackingClass*    aObjectTrackingClassPtr=&aObjectTrackingClass;

        // 動的生成領域を指すオーナー・ポインタ
        long*                   aLongOwner=new long{101};
        ObjectTrackingClass*    aObjectTrackingClassOwner=new ObjectTrackingClass{201, 301};

        auto temp0=makeAutoRelease(aLongOwner);                 // 自動解放設定
        auto temp1=makeAutoRelease(aObjectTrackingClassOwner);  // 自動解放設定

        // 動的生成領域を指すポインタ(非オーナー)
        long*                   aLongPtr2=aLongOwner;
        ObjectTrackingClass*    aObjectTrackingClassPtr2=aObjectTrackingClassOwner;

        // THEOLIZER_ANNOTATE()でオブジェクト追跡指定された静的定義領域を指すポインタ(非オーナー)
        int*                    aIntPtr=aObjectTrackingClassOwner->getStatic();

        // THEOLIZER_ANNOTATE()でオーナー指定された動的生成領域を指すポインタ(非オーナー)
        short*                  aShortPtr=aObjectTrackingClassOwner->getDynamic();

//      ---<<< 保存処理 >>>---

        std::ofstream   aStream("tutorise_object_tracking.json");
        theolizer::JsonOSerializer<> aSerializer(aStream);

        // 静的定義領域を指すポインタ(非オーナー)を保存
        THEOLIZER_PROCESS(aSerializer, aLongPtr);
        THEOLIZER_PROCESS(aSerializer, aObjectTrackingClassPtr);

        // 静的定義領域(プリミティブ)を保存
        THEOLIZER_PROCESS_POINTEE(aSerializer, aLong);

        // 静的定義領域(クラス)、動的生成領域(メンバ変数mShort)を保存
        THEOLIZER_PROCESS_POINTEE(aSerializer, aObjectTrackingClass);

        // 動的生成領域を指すオーナー・ポインタを保存
        THEOLIZER_PROCESS_OWNER(aSerializer, aLongOwner);
        THEOLIZER_PROCESS_OWNER(aSerializer, aObjectTrackingClassOwner);

        // 動的生成領域を指すポインタ(非オーナー)を保存
        THEOLIZER_PROCESS(aSerializer, aLongPtr2);
        THEOLIZER_PROCESS(aSerializer, aObjectTrackingClassPtr2);

        // THEOLIZER_ANNOTATE()で指定したインスタンスを指すボインタ(非オーナー／オーナー)を保存
        THEOLIZER_PROCESS(aSerializer, aIntPtr);
        THEOLIZER_PROCESS(aSerializer, aShortPtr);

        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();
    }
    std::cout << "tutoriseObjectTracking() processing\n" << std::endl;

//----------------------------------------------------------------------------
//      回復
//----------------------------------------------------------------------------

    {

//      ---<<< 回復先変数定義 >>>---

        // 静的定義領域(プリミティブ)
        long    aLong{0};

        // 静的定義領域(クラス)、動的生成領域(メンバ変数)
        ObjectTrackingClass     aObjectTrackingClass;

        // 静的定義領域を指すポインタ(非オーナー)
        long*                   aLongPtr=nullptr;
        ObjectTrackingClass*    aObjectTrackingClassPtr=nullptr;

        // 動的生成領域を指すオーナー・ポインタ
        long*                   aLongOwner=nullptr;
        ObjectTrackingClass*    aObjectTrackingClassOwner=nullptr;

        auto temp0=makeAutoRelease(aLongOwner);                 // 自動解放設定
        auto temp1=makeAutoRelease(aObjectTrackingClassOwner);  // 自動解放設定

        // 動的生成領域を指すポインタ(非オーナー)
        long*                   aLongPtr2=nullptr;
        ObjectTrackingClass*    aObjectTrackingClassPtr2=nullptr;

        // THEOLIZER_ANNOTATE()でオブジェクト追跡指定された静的定義領域を指すポインタ(非オーナー)
        int*                    aIntPtr=nullptr;

        // THEOLIZER_ANNOTATE()でオーナー指定された動的生成領域を指すポインタ(非オーナー)
        short*                  aShortPtr=nullptr;

//      ---<<< 回復処理 >>>---

        std::ifstream   aStream("tutorise_object_tracking.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        // 静的定義領域を指すポインタ(非オーナー)を回復
        THEOLIZER_PROCESS(aSerializer, aLongPtr);
        THEOLIZER_PROCESS(aSerializer, aObjectTrackingClassPtr);

        // 静的定義領域(プリミティブ)を回復
        THEOLIZER_PROCESS_POINTEE(aSerializer, aLong);

        // 静的定義領域(クラス)、動的生成領域(メンバ変数mShort)を回復
        THEOLIZER_PROCESS_POINTEE(aSerializer, aObjectTrackingClass);

        // 動的生成領域を指すオーナー・ポインタを回復
        THEOLIZER_PROCESS_OWNER(aSerializer, aLongOwner);
        THEOLIZER_PROCESS_OWNER(aSerializer, aObjectTrackingClassOwner);

        // 動的生成領域を指すポインタ(非オーナー)を回復
        THEOLIZER_PROCESS(aSerializer, aLongPtr2);
        THEOLIZER_PROCESS(aSerializer, aObjectTrackingClassPtr2);

        // THEOLIZER_ANNOTATE()で指定したインスタンスを指すボインタ(非オーナー／オーナー)を回復
        THEOLIZER_PROCESS(aSerializer, aIntPtr);
        THEOLIZER_PROCESS(aSerializer, aShortPtr);

        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();

//      ---<<< 結果のチェック >>>---

        // 静的定義領域を指すポインタ(非オーナー)
        THEOLIZER_EQUAL_PTR(aLongPtr, &aLong);
        THEOLIZER_EQUAL_PTR(aObjectTrackingClassPtr, &aObjectTrackingClass);

        // 静的定義領域(プリミティブ)
        THEOLIZER_EQUAL(aLong, 100);

        // 静的定義領域(クラス)、動的生成領域(メンバ変数)
        aObjectTrackingClass.check(200, 300);

        // 動的生成領域を指すオーナー・ポインタ
        THEOLIZER_EQUAL(*aLongOwner, 101);
        aObjectTrackingClassOwner->check(201, 301);

        // 動的生成領域を指すポインタ(非オーナー)
        THEOLIZER_EQUAL_PTR(aLongPtr2, aLongOwner);
        THEOLIZER_EQUAL_PTR(aObjectTrackingClassPtr2, aObjectTrackingClassOwner);

        // THEOLIZER_ANNOTATE()で指定したインスタンスを指すボインタ(非オーナー／オーナー)
        THEOLIZER_EQUAL_PTR(aIntPtr,   aObjectTrackingClassOwner->getStatic());
        THEOLIZER_EQUAL_PTR(aShortPtr, aObjectTrackingClassOwner->getDynamic());
    }

//----------------------------------------------------------------------------
//      複数回シリアライズ・データの回復エラーテスト
//----------------------------------------------------------------------------

//      ---<<< 保存 >>>---

    {
        ObjectTrackingDerived   aObjectTrackingDerived{true};
        ObjectTrackingDerived&  aObjectTrackingDerivedRef(aObjectTrackingDerived);

        std::ofstream   aStream("tutorise_object_tracking_multi.json");
        theolizer::JsonOSerializer<> aSerializer(aStream);

        THEOLIZER_PROCESS_POINTEE(aSerializer, aObjectTrackingDerived);
        THEOLIZER_PROCESS_POINTEE(aSerializer, aObjectTrackingDerivedRef);

        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();
    }

//      ---<<< 正常系 >>>---

    {
        ObjectTrackingDerived   aObjectTrackingDerived;
        ObjectTrackingDerived&  aObjectTrackingDerivedRef(aObjectTrackingDerived);

        std::ifstream   aStream("tutorise_object_tracking_multi.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        THEOLIZER_PROCESS_POINTEE(aSerializer, aObjectTrackingDerived);
        aObjectTrackingDerived.check();

        THEOLIZER_PROCESS_POINTEE(aSerializer, aObjectTrackingDerivedRef);
        aObjectTrackingDerivedRef.check();

        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();
    }

//      ---<<< 異常系 >>>---

    {
        ObjectTrackingDerived   aObjectTrackingDerived;
        ObjectTrackingDerived   aObjectTrackingDerived2;

        std::ifstream   aStream("tutorise_object_tracking_multi.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        THEOLIZER_PROCESS_POINTEE(aSerializer, aObjectTrackingDerived);
        aObjectTrackingDerived.check();

        // 保存時と異なるアドレスへ回復するとエラー
        THEOLIZER_CHECK_EXCEPTION2(
            THEOLIZER_PROCESS_POINTEE(aSerializer, aObjectTrackingDerived2);,// dStatements
            theolizer::ErrorInfo& e,                                // dException
            e.getErrorKind() == theolizer::ErrorKind::WrongUsing,   // dJudge
            e.getMessage());                                        // dResult

        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();
    }

    std::cout << "tutoriseObjectTracking() end\n" << std::endl;
}

#endif  // ENABLE_OBJECT_TRACKING_TEST
