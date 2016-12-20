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

// プロジェクト・ヘッダ
#include "common.h"
#include "test_object_tracking.h"

// 自動生成ソース
#include "test_object_tracking.cpp.theolizer.hpp"

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

void tutoriseObjectTracking()
{
    std::cout << "tutoriseObjectTracking() start" << std::endl;

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

    std::cout << "tutoriseObjectTracking() end\n" << std::endl;
}

//############################################################################
//      組み合わせテスト
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

template<class tSerializer>
void saveObjectTracking(tSerializer& iSerializer)
{
//----------------------------------------------------------------------------
//      テスト
//----------------------------------------------------------------------------

//      ---<<< 手動(トップ・レベル)によるポインタの保存 >>>---

    {
        // オブジェクト追跡前準備
        //  ここに来るまでにオブジェクト追跡テーブルに登録され、その領域が開放されるので必要。
        //  (オブジェクト追跡単位内でシリアライズした領域を開放してはいけない。)
        iSerializer.clearTracking();

        // ポイント先群
        PointeeList aPointeeList{true};
        PointeeList aPointeeList2{2};

        // ポイント先1保存
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList);

        // ポイント先保存前／後にポインタ保存
        PointerList aPointerList(aPointeeList, aPointeeList2);
        std::cout << "        savePointer() : aPointerList" << std::endl;
        savePointer(iSerializer, aPointerList);

        // ポイント先保存2
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList2" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList2);

        // ポイント先保存後にポインタ保存
        PointerList aPointerList2(aPointeeList, aPointeeList2);
        std::cout << "        savePointer() : aPointerList2" << std::endl;
        savePointer(iSerializer, aPointerList2);

        iSerializer.clearTracking();
    }

//      ---<<< 自動メンバ・リスト生成によるポインタの保存 >>>---

    {
        // ポイント先群
        PointeeList aPointeeList{true};
        PointeeList aPointeeList2{2};

        // ポイント先1保存
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList);

        // ポイント先保存前／後にポインタ保存
        PointerList aPointerList(aPointeeList, aPointeeList2);
        std::cout << "        THEOLIZER_PROCESS() : aPointerList" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointerList);

        // ポイント先2保存
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList2);

        // ポイント先保存後にポインタ保存
        PointerList aPointerList2(aPointeeList, aPointeeList2);
        std::cout << "        THEOLIZER_PROCESS() : aPointerList2" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointerList2);

        iSerializer.clearTracking();
    }

//      ---<<< 手動(非トップ・レベル)によるポインタの保存 >>>---

    {
        // ポイント先群
        PointeeList aPointeeList{true};
        PointeeList aPointeeList2{2};

        // ポイント先1保存
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList);

        // ポイント先保存前／後にポインタ保存
        ManualClass4PointerList aManualClass4PointerList(aPointeeList);
        std::cout << "        THEOLIZER_PROCESS() : aManualClass4PointerList" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aManualClass4PointerList);

        // ポイント先2保存
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList2" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList2);

        // ポイント先保存後にポインタ保存
        ManualClass4PointerList aManualClass4PointerList2(aPointeeList);
        std::cout << "        THEOLIZER_PROCESS() : aManualClass4PointerList2" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aManualClass4PointerList2);

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
//----------------------------------------------------------------------------
//      テスト
//----------------------------------------------------------------------------

//      ---<<< 手動(トップ・レベル)によるポインタ（左辺値）の回復 >>>---

    {
        // オブジェクト追跡前準備
        //  ここに来るまでにオブジェクト追跡テーブルに登録され、その領域が開放されるので必要。
        //  (オブジェクト追跡単位内でシリアライズした領域を開放してはいけない。)
        iSerializer.clearTracking();

        // 回復領域
        PointeeList aPointeeList;
        PointeeList aPointeeList2;

        // ポイント先1回復
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList);
        aPointeeList.check(true);

        // ポイント先回復前／後にポインタ回復
        PointerList aPointerList;
        std::cout << "        loadPointer() : aPointerList" << std::endl;
        loadPointer(iSerializer, aPointerList);

        // ポイント先2回復
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList2" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList2);
        aPointeeList.check(true);

        // ポイント先回復後にポインタ回復
        PointerList aPointerList2;
        std::cout << "        loadPointer() : aPointerList2" << std::endl;
        loadPointer(iSerializer, aPointerList2);

        // アドレス解決確認
        iSerializer.clearTracking();

        // 回復結果のチェック
        aPointerList.check(aPointeeList, aPointeeList2);
        aPointerList2.check(aPointeeList, aPointeeList2);
    }

//      ---<<< 自動メンバ・リスト生成によるポインタの回復 >>>---

    {
        // 回復領域
        PointeeList aPointeeList;
        PointeeList aPointeeList2;

        // ポイント先1回復
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList);
        aPointeeList.check(true);

        // ポイント先回復前／後にポインタ回復
        PointerList aPointerList;
        std::cout << "        THEOLIZER_PROCESS() : aPointerList" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointerList);

        // ポイント先2回復
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList2" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList2);
        aPointeeList.check(true);

        // ポイント先回復後にポインタ回復
        PointerList aPointerList2;
        std::cout << "        THEOLIZER_PROCESS() : aPointerList2" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointerList2);

        // アドレス解決確認
        iSerializer.clearTracking();

        // 回復結果のチェック
        aPointerList.check(aPointeeList, aPointeeList2);
        aPointerList2.check(aPointeeList, aPointeeList2);
    }

//      ---<<< 手動(非トップ・レベル)によるポインタの回復 >>>---

    {
        // 回復領域
        PointeeList aPointeeList;
        PointeeList aPointeeList2;

        // ポイント先1回復
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList);
        aPointeeList.check(true);

        // ポイント先回復前／後にポインタ回復
        ManualClass4PointerList aManualClass4PointerList;
        std::cout << "        THEOLIZER_PROCESS() : aManualClass4PointerList" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aManualClass4PointerList);

        // ポイント先2回復
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList2" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList2);
        aPointeeList.check(true);

        // ポイント先回復後にポインタ回復
        ManualClass4PointerList aManualClass4PointerList2;
        std::cout << "        THEOLIZER_PROCESS() : aManualClass4PointerList2" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aManualClass4PointerList2);

        // アドレス解決確認
        iSerializer.clearTracking();

        // 回復結果のチェック
        aManualClass4PointerList.check(aPointeeList);
        aManualClass4PointerList2.check(aPointeeList);
    }
}

INSTANTIATION_ALL(void, loadObjectTracking);

#endif  // DISABLE_OBJECT_TRACKING_TEST
