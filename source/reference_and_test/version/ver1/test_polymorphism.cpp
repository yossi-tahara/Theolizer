//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          ポリモーフィズム
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
#ifndef DISABLE_POLYMORPHISM_TEST

namespace std
{
    template<class _Ty>
    struct default_delete;
}

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

#include <theolizer/memory.h>   // std::unique_ptrのシリアライズ化
#include <theolizer/vector.h>   // std::vectorのシリアライス化

// プロジェクト・ヘッダ
#include "common.h"
#include "test_polymorphism.h"

// 自動生成ソース
#include "test_polymorphism.cpp.theolizer.hpp"

//############################################################################
//      使い方の説明
//############################################################################

// ***************************************************************************
//      クラス実体定義
// ***************************************************************************

//----------------------------------------------------------------------------
//      
//----------------------------------------------------------------------------

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
        std::ofstream   aStream("tutorise_object_tracking.json");
        theolizer::JsonOSerializer<> aSerializer(aStream);

        // 非侵入型完全自動

    }

//----------------------------------------------------------------------------
//      回復
//----------------------------------------------------------------------------

    {
        std::ifstream   aStream("tutorise_object_tracking.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        // 非侵入型完全自動
    }

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
}

INSTANTIATION_ALL(void, savePolymorphism);

// ***************************************************************************
//      回復
// ***************************************************************************

template<class tSerializer>
void loadPolymorphism(tSerializer& iSerializer)
{
std::cout << "loadPolymorphism() start\n";
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

theolizer::DisplayPass aDisplayPass;
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
}

INSTANTIATION_ALL(void, loadPolymorphism);

#endif  // DISABLE_POLYMORPHISM_TEST
