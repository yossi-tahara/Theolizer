//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          ポリモーフィズム
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

#include "disable_test.h"
#ifdef ENABLE_POLYMORPHISM_TEST

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

#include <theolizer/memory.h>   // std::unique_ptrのシリアライズ化
#include <theolizer/vector.h>   // std::vectorのシリアライズ化

// プロジェクト・ヘッダ
#include "common.h"
#include "test_polymorphism.h"

// 自動生成ソース
#include "test_polymorphism.cpp.theolizer.hpp"

//############################################################################
//      使い方の説明
//############################################################################

// ***************************************************************************
//      動作確認
// ***************************************************************************

void tutorisePolymorphism()
{
    std::cout << "tutorisePolymorphism() start" << std::endl;

//----------------------------------------------------------------------------
//      保存
//----------------------------------------------------------------------------

    {
//      ---<<< シリアライズ対象変数定義 >>>---

        PolyBase*   aPolyBase0=new PolyDerived0{true};
        PolyBase*   aPolyBase1=new PolyDerived1{true};

//      ---<<< 保存処理 >>>---

        std::ofstream   aStream("tutorise_polymorphism.json");
        theolizer::JsonOSerializer<> aSerializer(aStream);

        // オーナーとして保存／回復する
        THEOLIZER_PROCESS_OWNER(aSerializer, aPolyBase0);
        THEOLIZER_PROCESS_OWNER(aSerializer, aPolyBase1);

        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();
    }

//----------------------------------------------------------------------------
//      回復
//----------------------------------------------------------------------------

    {

//      ---<<< 回復先変数定義 >>>---

        PolyBase*   aPolyBase0=nullptr;
        PolyBase*   aPolyBase1=nullptr;

//      ---<<< 回復処理 >>>---

        std::ifstream   aStream("tutorise_polymorphism.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        // オーナーとして保存／回復する
        THEOLIZER_PROCESS_OWNER(aSerializer, aPolyBase0);
        THEOLIZER_PROCESS_OWNER(aSerializer, aPolyBase1);

        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();

//      ---<<< 結果のチェック >>>---

        aPolyBase0->check();
        aPolyBase1->check();
    }

//----------------------------------------------------------------------------
//      参照回復時のエラー・テスト
//          保存時と異なる型検出
//          nullptr参照(本来あり得ない)
//----------------------------------------------------------------------------

//      ---<<< 保存 >>>---

    {
        std::ofstream   aStream("tutorise_polymorphism_refer.json");
        theolizer::JsonOSerializer<> aSerializer(aStream);

        // 非侵入型完全自動
        PolyDerivedFullAuto aPolyDerivedFullAuto(true);
        PolyBaseFullAuto&   aPolyBaseFullAutoRef(aPolyDerivedFullAuto);
        THEOLIZER_PROCESS_POINTEE(aSerializer, aPolyBaseFullAutoRef);

        // 侵入型半自動
        PolyDerivedHalfAuto aPolyDerivedHalfAuto(true);
        PolyBaseHalfAuto&   aPolyBaseHalfAutoRef(aPolyDerivedHalfAuto);
        THEOLIZER_PROCESS_POINTEE(aSerializer, aPolyBaseHalfAutoRef);

        // 非侵入型手動
        PolyDerivedManual aPolyDerivedManual(true);
        PolyBaseManual&   aPolyBaseManualRef(aPolyDerivedManual);
        THEOLIZER_PROCESS_POINTEE(aSerializer, aPolyBaseManualRef);

        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();
    }

//      ---<<< 正常系 >>>---

    {
        std::ifstream   aStream("tutorise_polymorphism_refer.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        // 非侵入型完全自動
        PolyDerivedFullAuto aPolyDerivedFullAuto;
        PolyBaseFullAuto&   aPolyBaseFullAutoRef(aPolyDerivedFullAuto);
        THEOLIZER_PROCESS_POINTEE(aSerializer, aPolyBaseFullAutoRef);
        aPolyBaseFullAutoRef.check(true);

        // 侵入型半自動
        PolyDerivedHalfAuto aPolyDerivedHalfAuto;
        PolyBaseHalfAuto&   aPolyBaseHalfAutoRef(aPolyDerivedHalfAuto);
        THEOLIZER_PROCESS_POINTEE(aSerializer, aPolyBaseHalfAutoRef);
        aPolyBaseHalfAutoRef.check(true);

        // 非侵入型手動
        PolyDerivedManual aPolyDerivedManual;
        PolyBaseManual&   aPolyBaseManualRef(aPolyDerivedManual);
        THEOLIZER_PROCESS_POINTEE(aSerializer, aPolyBaseManualRef);
        aPolyBaseManualRef.check(true);

        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();
    }

//      ---<<< 異常系(型違い) >>>---

    {
        std::ifstream   aStream("tutorise_polymorphism_refer.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        // 保存時と異なる型へ回復しようとするとエラー

        // 非侵入型完全自動
        PolyDerivedHalfAuto aPolyDerivedHalfAuto;
        PolyBaseFullAuto&   aPolyBaseFullAutoRef(aPolyDerivedHalfAuto);
        THEOLIZER_CHECK_EXCEPTION2(
            THEOLIZER_PROCESS_POINTEE(aSerializer, aPolyBaseFullAutoRef);,// dStatements
            theolizer::ErrorInfo& e,                                // dException
            e.getErrorKind() == theolizer::ErrorKind::WrongUsing,   // dJudge
            e.getMessage());                                        // dResult

        // 侵入型半自動
        PolyDerivedManual   aPolyDerivedManual;
        PolyBaseHalfAuto&   aPolyBaseHalfAutoRef(aPolyDerivedManual);
        THEOLIZER_CHECK_EXCEPTION2(
            THEOLIZER_PROCESS_POINTEE(aSerializer, aPolyBaseHalfAutoRef);,// dStatements
            theolizer::ErrorInfo& e,                                // dException
            e.getErrorKind() == theolizer::ErrorKind::WrongUsing,   // dJudge
            e.getMessage());                                        // dResult

        // 非侵入型手動
        PolyDerivedFullAuto aPolyDerivedFullAuto;
        PolyBaseManual&     aPolyBaseManualRef(aPolyDerivedFullAuto);
        THEOLIZER_CHECK_EXCEPTION2(
            THEOLIZER_PROCESS_POINTEE(aSerializer, aPolyBaseManualRef);,// dStatements
            theolizer::ErrorInfo& e,                                // dException
            e.getErrorKind() == theolizer::ErrorKind::WrongUsing,   // dJudge
            e.getMessage());                                        // dResult

        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();
    }

//      ---<<< 異常系(nullptr) >>>---

#ifndef THEOLIZER_INTERNAL_DISABLE_NULL_REFERENCE
    {
        std::ifstream   aStream("tutorise_polymorphism_refer.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        // nullptrへ回復しようとするとエラー

        // 非侵入型完全自動
        PolyBaseFullAuto*   aPolyBaseFullAutoPtr(nullptr);
        THEOLIZER_CHECK_EXCEPTION2(
            THEOLIZER_PROCESS_POINTEE(aSerializer, *aPolyBaseFullAutoPtr);,// dStatements
            theolizer::ErrorInfo& e,                                // dException
            e.getErrorKind() == theolizer::ErrorKind::WrongUsing,   // dJudge
            e.getMessage());                                        // dResult

        // 侵入型半自動
        PolyBaseHalfAuto*   aPolyBaseHalfAutoPtr(nullptr);
        THEOLIZER_CHECK_EXCEPTION2(
            THEOLIZER_PROCESS_POINTEE(aSerializer, *aPolyBaseHalfAutoPtr);,// dStatements
            theolizer::ErrorInfo& e,                                // dException
            e.getErrorKind() == theolizer::ErrorKind::WrongUsing,   // dJudge
            e.getMessage());                                        // dResult

        // 非侵入型手動
        PolyBaseManual*     aPolyBaseManualPtr(nullptr);
        THEOLIZER_CHECK_EXCEPTION2(
            THEOLIZER_PROCESS_POINTEE(aSerializer, *aPolyBaseManualPtr);,// dStatements
            theolizer::ErrorInfo& e,                                // dException
            e.getErrorKind() == theolizer::ErrorKind::WrongUsing,   // dJudge
            e.getMessage());                                        // dResult

        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();
    }
#endif

    std::cout << "tutorisePolymorphism() end\n" << std::endl;
}

//############################################################################
//      組み合わせテスト
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

template<class tSerializer>
void savePolymorphism(tSerializer& iSerializer)
{
//----------------------------------------------------------------------------
//      非侵入型完全自動
//----------------------------------------------------------------------------

    {
        std::vector<std::unique_ptr<PolyBaseFullAuto> > aVector;
        aVector.emplace_back(new PolyDerivedFullAuto(true));
        aVector.emplace_back(new PolyDerivedHalfAuto(true));
        aVector.emplace_back(new PolyDerivedManual(true));
        THEOLIZER_PROCESS(iSerializer, aVector);

        iSerializer.clearTracking();
    }

//----------------------------------------------------------------------------
//      侵入型半自動
//----------------------------------------------------------------------------

    {
        std::vector<std::unique_ptr<PolyBaseHalfAuto> > aVector;
        aVector.emplace_back(new PolyDerivedFullAuto(true));
        aVector.emplace_back(new PolyDerivedHalfAuto(true));
        aVector.emplace_back(new PolyDerivedManual(true));
        THEOLIZER_PROCESS(iSerializer, aVector);

        iSerializer.clearTracking();
    }

//----------------------------------------------------------------------------
//      非侵入型手動
//----------------------------------------------------------------------------

    {
        std::vector<std::unique_ptr<PolyBaseManual> > aVector;
        aVector.emplace_back(new PolyDerivedFullAuto(true));
        aVector.emplace_back(new PolyDerivedHalfAuto(true));
        aVector.emplace_back(new PolyDerivedManual(true));
        THEOLIZER_PROCESS(iSerializer, aVector);

        iSerializer.clearTracking();
    }

//----------------------------------------------------------------------------
//      参照
//----------------------------------------------------------------------------

    {
        // 非侵入型完全自動
        PolyDerivedFullAuto aPolyDerivedFullAuto(true);
        PolyBaseFullAuto&   aPolyBaseFullAutoRef(aPolyDerivedFullAuto);
        THEOLIZER_PROCESS(iSerializer, aPolyBaseFullAutoRef);

        // 侵入型半自動
        PolyDerivedHalfAuto aPolyDerivedHalfAuto(true);
        PolyBaseHalfAuto&   aPolyBaseHalfAutoRef(aPolyDerivedHalfAuto);
        THEOLIZER_PROCESS(iSerializer, aPolyBaseHalfAutoRef);

        // 非侵入型手動
        PolyDerivedManual aPolyDerivedManual(true);
        PolyBaseManual&   aPolyBaseManualRef(aPolyDerivedManual);
        THEOLIZER_PROCESS(iSerializer, aPolyBaseManualRef);

        // オブジェクト追跡されていないことを確認する
        THEOLIZER_EQUAL(iSerializer.getRequireClearTracking(), false);
    }

    {
        // 非侵入型完全自動
        PolyDerivedFullAuto aPolyDerivedFullAuto(true);
        PolyBaseFullAuto&   aPolyBaseFullAutoRef(aPolyDerivedFullAuto);
        THEOLIZER_PROCESS_POINTEE(iSerializer, aPolyBaseFullAutoRef);

        // 侵入型半自動
        PolyDerivedHalfAuto aPolyDerivedHalfAuto(true);
        PolyBaseHalfAuto&   aPolyBaseHalfAutoRef(aPolyDerivedHalfAuto);
        THEOLIZER_PROCESS_POINTEE(iSerializer, aPolyBaseHalfAutoRef);

        // 非侵入型手動
        PolyDerivedManual aPolyDerivedManual(true);
        PolyBaseManual&   aPolyBaseManualRef(aPolyDerivedManual);
        THEOLIZER_PROCESS_POINTEE(iSerializer, aPolyBaseManualRef);

        // オブジェクト追跡されていることを確認する
        THEOLIZER_EQUAL(iSerializer.getRequireClearTracking(), true);

        // 基底クラスで派生クラスをシリアライズすると、オブジェクト追跡されるので要クリア
        iSerializer.clearTracking();
    }
}

INSTANTIATION_ALL(savePolymorphism);

// ***************************************************************************
//      回復
// ***************************************************************************

template<class tSerializer>
void loadPolymorphism(tSerializer& iSerializer)
{
//----------------------------------------------------------------------------
//      非侵入型完全自動
//----------------------------------------------------------------------------

    {
        std::vector<std::unique_ptr<PolyBaseFullAuto> > aVector;
        THEOLIZER_PROCESS(iSerializer, aVector);

        iSerializer.clearTracking();

        aVector[0]->check(true);
        THEOLIZER_EQUAL(typeid(*aVector[0].get()), typeid(PolyDerivedFullAuto));

        aVector[1]->check(true);
        THEOLIZER_EQUAL(typeid(*aVector[1].get()), typeid(PolyDerivedHalfAuto));

        aVector[2]->check(true);
        THEOLIZER_EQUAL(typeid(*aVector[2].get()), typeid(PolyDerivedManual));
    }

//----------------------------------------------------------------------------
//      侵入型半自動
//----------------------------------------------------------------------------

    {
        std::vector<std::unique_ptr<PolyBaseHalfAuto> > aVector;
        THEOLIZER_PROCESS(iSerializer, aVector);

        iSerializer.clearTracking();

        aVector[0]->check(true);
        THEOLIZER_EQUAL(typeid(*aVector[0].get()), typeid(PolyDerivedFullAuto));

        aVector[1]->check(true);
        THEOLIZER_EQUAL(typeid(*aVector[1].get()), typeid(PolyDerivedHalfAuto));

        aVector[2]->check(true);
        THEOLIZER_EQUAL(typeid(*aVector[2].get()), typeid(PolyDerivedManual));
    }

//----------------------------------------------------------------------------
//      非侵入型手動
//----------------------------------------------------------------------------

    {
        std::vector<std::unique_ptr<PolyBaseManual> > aVector;
        THEOLIZER_PROCESS(iSerializer, aVector);

        iSerializer.clearTracking();

        aVector[0]->check(true);
        THEOLIZER_EQUAL(typeid(*aVector[0].get()), typeid(PolyDerivedFullAuto));

        aVector[1]->check(true);
        THEOLIZER_EQUAL(typeid(*aVector[1].get()), typeid(PolyDerivedHalfAuto));

        aVector[2]->check(true);
        THEOLIZER_EQUAL(typeid(*aVector[2].get()), typeid(PolyDerivedManual));
    }

//----------------------------------------------------------------------------
//      参照
//----------------------------------------------------------------------------

    {
        PolyDerivedFullAuto aPolyDerivedFullAuto;
        PolyBaseFullAuto&   aPolyBaseFullAutoRef(aPolyDerivedFullAuto);
        THEOLIZER_PROCESS(iSerializer, aPolyBaseFullAutoRef);
        aPolyBaseFullAutoRef.check(true);

        // 侵入型半自動
        PolyDerivedHalfAuto aPolyDerivedHalfAuto;
        PolyBaseHalfAuto&   aPolyBaseHalfAutoRef(aPolyDerivedHalfAuto);
        THEOLIZER_PROCESS(iSerializer, aPolyBaseHalfAutoRef);
        aPolyBaseHalfAutoRef.check(true);

        // 非侵入型手動
        PolyDerivedManual aPolyDerivedManual;
        PolyBaseManual&   aPolyBaseManualRef(aPolyDerivedManual);
        THEOLIZER_PROCESS(iSerializer, aPolyBaseManualRef);
        aPolyBaseManualRef.check(true);

        // オブジェクト追跡されていないことを確認する
        THEOLIZER_EQUAL(iSerializer.getRequireClearTracking(), false);

        // 基底クラスで派生クラスをシリアライズすると、オブジェクト追跡されるので要クリア
        iSerializer.clearTracking();
    }

    {
        PolyDerivedFullAuto aPolyDerivedFullAuto;
        PolyBaseFullAuto&   aPolyBaseFullAutoRef(aPolyDerivedFullAuto);
        THEOLIZER_PROCESS_POINTEE(iSerializer, aPolyBaseFullAutoRef);
        aPolyBaseFullAutoRef.check(true);

        // 侵入型半自動
        PolyDerivedHalfAuto aPolyDerivedHalfAuto;
        PolyBaseHalfAuto&   aPolyBaseHalfAutoRef(aPolyDerivedHalfAuto);
        THEOLIZER_PROCESS_POINTEE(iSerializer, aPolyBaseHalfAutoRef);
        aPolyBaseHalfAutoRef.check(true);

        // 非侵入型手動
        PolyDerivedManual aPolyDerivedManual;
        PolyBaseManual&   aPolyBaseManualRef(aPolyDerivedManual);
        THEOLIZER_PROCESS_POINTEE(iSerializer, aPolyBaseManualRef);
        aPolyBaseManualRef.check(true);

        // オブジェクト追跡されていることを確認する
        THEOLIZER_EQUAL(iSerializer.getRequireClearTracking(), true);

        // 基底クラスで派生クラスをシリアライズすると、オブジェクト追跡されるので要クリア
        iSerializer.clearTracking();
    }
}

INSTANTIATION_ALL(loadPolymorphism);

#endif  // ENABLE_POLYMORPHISM_TEST
