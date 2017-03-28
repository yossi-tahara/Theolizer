//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          保存先
/*
    Copyright (c) 2017 Yohinori Tahara(Theoride Technology) - http://theolizer.com/

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
#ifndef DISABLE_DESTINATIONS_TEST

// ***************************************************************************
//      インクルード
// ***************************************************************************

// 標準ライブラリ
#include <iostream>
#include <fstream>

// theolizerライブラリ
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_json.h>

#include <theolizer/memory.h>   // std::unique_ptrのシリアライズ化
#include <theolizer/list.h>     // std::listのシリアライス化

// プロジェクト・ヘッダ
#include "common.h"
#include "test_destinations.h"

// 自動生成ソース
#include "test_destinations.cpp.theolizer.hpp"

//############################################################################
//      使い方の説明
//############################################################################

// ***************************************************************************
//      動作確認
// ***************************************************************************

void tutoriseDestinations()
{
    std::cout << "tutoriseDestinations() start" << std::endl;

//----------------------------------------------------------------------------
//      保存の有無指定
//----------------------------------------------------------------------------

//![DefaultSave]

//      ---<<< 保存 >>>---

    {
        std::ofstream   aStream("tutorise_destinations.json");
        theolizer::JsonOSerializer<> aSerializer(aStream);

        // デフォルト保存
        DefaultSave aDefaultSave(true);
        THEOLIZER_PROCESS(aSerializer, aDefaultSave);

        // デフォルト非保存
        DefaultNonSave aDefaultNonSave(true);
        THEOLIZER_PROCESS(aSerializer, aDefaultNonSave);
    }

//      ---<<< 回復 >>>---

    {
        std::ifstream   aStream("tutorise_destinations.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        // デフォルト保存
        DefaultSave aDefaultSave;
        THEOLIZER_PROCESS(aSerializer, aDefaultSave);
        aDefaultSave.check();

        // デフォルト非保存
        DefaultNonSave aDefaultNonSave;
        THEOLIZER_PROCESS(aSerializer, aDefaultNonSave);
        aDefaultNonSave.check();
    }

//![DefaultSave]

//----------------------------------------------------------------------------
//      保存先指定（マスター・ファイルと取引ファイル）
//----------------------------------------------------------------------------

//      ---<<< 保存 >>>---

//![save DestinationPoly]

    {
        // データ作成
        std::list<std::unique_ptr<BaseCustomer> >   aList;
        aList.emplace_back(new CorporateCustomer("Theoride Technology", 2000000, 500000));
        aList.emplace_back(new IndividualCustomer("Yossi Tahara", "1962/01/01", 12000));

        // Master保存
        std::ofstream   aStreamMaster("tutorise_destinationsMaster.json");
        theolizer::JsonOSerializer<theolizerD::Master> aSerializerMaster(aStreamMaster);
        THEOLIZER_PROCESS(aSerializerMaster, aList);
        aSerializerMaster.clearTracking();

        // Trade保存
        std::ofstream   aStreamTrade("tutorise_destinationsTrade.json");
        theolizer::JsonOSerializer<theolizerD::Trade> aSerializerTrade(aStreamTrade);
        THEOLIZER_PROCESS(aSerializerTrade, aList);
        aSerializerTrade.clearTracking();
    }

//![save DestinationPoly]

//      ---<<< 回復 >>>---

//![load DestinationPoly]

    {
        std::list<std::unique_ptr<BaseCustomer> >   aList;

        // Master回復
        std::ifstream   aStreamMaster("tutorise_destinationsMaster.json");
        theolizer::JsonISerializer<theolizerD::Master> aSerializerMaster(aStreamMaster);
        THEOLIZER_PROCESS(aSerializerMaster, aList);
        aSerializerMaster.clearTracking();

        // Tradeを合成回復
        std::ifstream   aStreamTrade("tutorise_destinationsTrade.json");
        theolizer::JsonISerializer<theolizerD::Trade> aSerializerTrade(aStreamTrade);
        THEOLIZER_PROCESS(aSerializerTrade, aList);
        aSerializerTrade.clearTracking();

        // 合成結果検証
        auto it = aList.begin();
        CorporateCustomer* aCorporateCustomer=dynamic_cast<CorporateCustomer*>(it->get());
        THEOLIZER_EQUAL(aCorporateCustomer->mName,          "Theoride Technology");
        THEOLIZER_EQUAL(aCorporateCustomer->mCapitalStock,  2000000);
        THEOLIZER_EQUAL(aCorporateCustomer->mTradeAccounts, 500000);
        ++it;
        IndividualCustomer* aIndividualCustomer=dynamic_cast<IndividualCustomer*>(it->get());
        THEOLIZER_EQUAL(aIndividualCustomer->mName,         "Yossi Tahara");
        THEOLIZER_EQUAL(aIndividualCustomer->mBirthday,     "1962/01/01");
        THEOLIZER_EQUAL(aIndividualCustomer->mPoint,        12000);

        // 結果表示
        theolizer::JsonOSerializer<> aSerializer(std::cout);
        THEOLIZER_PROCESS(aSerializer, aList);
        aSerializer.clearTracking();
    }

//![load DestinationPoly]

//----------------------------------------------------------------------------
//      保存先指定（注意点）
//----------------------------------------------------------------------------

//      ---<<< 保存 >>>---

//![save DestinationChild/Parent]

    {
        DestinationParent aDestinationParent(100);

        // DestA保存
        std::ofstream   aStreamA("tutorise_destinationsA.json");
        theolizer::JsonOSerializer<theolizerD::DestA> aSerializerA(aStreamA);
        THEOLIZER_PROCESS(aSerializerA, aDestinationParent);

        // DestB保存
        std::ofstream   aStreamB("tutorise_destinationsB.json");
        theolizer::JsonOSerializer<theolizerD::DestB> aSerializerB(aStreamB);
        THEOLIZER_PROCESS(aSerializerB, aDestinationParent);
    }

//![save DestinationChild/Parent]

//      ---<<< 回復 >>>---

//![load DestinationChild/Parent]

    {
        DestinationParent aDestinationParent;

        // DestA回復
        std::ifstream   aStreamA("tutorise_destinationsA.json");
        theolizer::JsonISerializer<theolizerD::DestA> aSerializerA(aStreamA);
        THEOLIZER_PROCESS(aSerializerA, aDestinationParent);

        // DestBを合成回復
        std::ifstream   aStreamB("tutorise_destinationsB.json");
        theolizer::JsonISerializer<theolizerD::DestB> aSerializerB(aStreamB);
        THEOLIZER_PROCESS(aSerializerB, aDestinationParent);

        // 合成結果検証
        THEOLIZER_EQUAL(aDestinationParent.mAnnotateA, 100);
        THEOLIZER_EQUAL(aDestinationParent.mAnnotateB, 200);
        THEOLIZER_EQUAL(aDestinationParent.mDestinationChild.mAnnotateA, 300);
        THEOLIZER_EQUAL(aDestinationParent.mDestinationChild.mAnnotateB, 0);

        // 結果表示
        theolizer::JsonOSerializer<> aSerializer(std::cout);
        THEOLIZER_PROCESS(aSerializer, aDestinationParent);
    }

//![load DestinationChild/Parent]

    std::cout << "tutoriseDestinations() end\n" << std::endl;
}

//############################################################################
//      保存有無のテスト
//          保存先指定=All
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

template<class tSerializer>
void saveSpecifySaving(tSerializer& iSerializer)
{
    std::cout << "saveSpecifySaving()" << std::endl;

    {
        DefaultSave aDefaultSave(true);
        THEOLIZER_PROCESS(iSerializer, aDefaultSave);

        DefaultNonSave  aDefaultNonSave(true);
        THEOLIZER_PROCESS(iSerializer, aDefaultNonSave);

        DestinationTestParent   aDestinationParent(true);
        THEOLIZER_PROCESS(iSerializer, aDestinationParent);
    }
}

INSTANTIATION_ALL(saveSpecifySaving);

// ***************************************************************************
//      回復
// ***************************************************************************

template<class tSerializer>
void loadSpecifySaving(tSerializer& iSerializer)
{
    std::cout << "loadSpecifySaving()" << std::endl;

//theolizer::DisplayPass aDisplayPass;
    {
        DefaultSave aDefaultSave;
        THEOLIZER_PROCESS(iSerializer, aDefaultSave);
        aDefaultSave.check();

        DefaultNonSave  aDefaultNonSave;
        THEOLIZER_PROCESS(iSerializer, aDefaultNonSave);
        aDefaultNonSave.check();

        DestinationTestParent   aDestinationParent;
        THEOLIZER_PROCESS(iSerializer, aDestinationParent);
        aDestinationParent.check({theolizerD::All});
    }
}

INSTANTIATION_ALL(loadSpecifySaving);

//############################################################################
//      保存先のテスト
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void saveDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB& iSerializerAB
)
{
    std::cout << "saveDestinations()" << std::endl;

    {
        DestinationTestParent   aDestinationParent(true);

        THEOLIZER_PROCESS(iSerializerA, aDestinationParent);
        THEOLIZER_PROCESS(iSerializerB, aDestinationParent);
        THEOLIZER_PROCESS(iSerializerAB, aDestinationParent);
    }
}

INSTANTIATION_DESTINATIONS(saveDestinations);

// ***************************************************************************
//      回復
// ***************************************************************************

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void loadDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB& iSerializerAB
)
{
    std::cout << "loadDestinations()" << std::endl;

    {
        DestinationTestParent   aDestinationParent;

        // １つ目のファイルからの回復テスト
        THEOLIZER_PROCESS(iSerializerA, aDestinationParent);
        aDestinationParent.check({theolizerD::DestA});

        // 合成テスト
        THEOLIZER_PROCESS(iSerializerB, aDestinationParent);
        aDestinationParent.check();

        // 合成結果の表示
        theolizer::JsonOSerializer<> jos(std::cout);
        THEOLIZER_PROCESS(jos, aDestinationParent);

        // 複数保存先テスト
        DestinationTestParent   aDestinationParentAB;
        THEOLIZER_PROCESS(iSerializerAB, aDestinationParentAB);
        aDestinationParentAB.check({theolizerD::DestA, theolizerD::DestB});
    }
}

INSTANTIATION_DESTINATIONS(loadDestinations);

#endif  // DISABLE_DESTINATIONS_TEST
