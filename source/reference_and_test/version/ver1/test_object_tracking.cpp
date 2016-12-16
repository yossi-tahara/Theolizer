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

//      ---<<< 処理関数 >>>---
//      手動(トップ・レベル)によるポインタの保存／回復

#if 1
    {
        // オブジェクト追跡前準備
        //  ここに来るまでにオブジェクト追跡テーブルに登録され、その領域が開放されるので必要。
        //  (オブジェクト追跡単位内でシリアライズした領域を開放してはいけない。)
        iSerializer.clearTracking();

        // ポイント先群
        PointeeList aPointeeList{true};

        // ポイント先保存前にポインタ保存
        std::cout << "        savePointer() : aPointerList" << std::endl;
        PointerList aPointerList(aPointeeList);
        savePointer(iSerializer, aPointerList);

        // ポイント先保存
        std::cout << "        THEOLIZER_PROCESS() : gPointeeList" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList);

        // ポイント先保存後にポインタ保存
        std::cout << "        savePointer() : aPointerList2" << std::endl;
        PointerList aPointerList2(aPointeeList);
        savePointer(iSerializer, aPointerList2);

        iSerializer.clearTracking();
    }
#endif
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

//      ---<<< 処理関数 >>>---
//      手動(トップ・レベル)によるポインタ（左辺値）の保存／回復

#if 1
    {
        // オブジェクト追跡前準備
        //  ここに来るまでにオブジェクト追跡テーブルに登録され、その領域が開放されるので必要。
        //  (オブジェクト追跡単位内でシリアライズした領域を開放してはいけない。)
        iSerializer.clearTracking();

        // 回復領域
        PointeeList aPointeeList;

        // 非ポインタ回復前にポインタ回復
        std::cout << "        loadPointer() : aPointerList" << std::endl;
        PointerList aPointerList;
        loadPointer(iSerializer, aPointerList);

        // 非ポインタ回復
        std::cout << "        THEOLIZER_PROCESS() : aPointeeList" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aPointeeList);
        aPointeeList.check();

        // 非ポインタ回復後にポインタ回復
        std::cout << "        loadPointer() : aPointerList2" << std::endl;
        PointerList aPointerList2;
        loadPointer(iSerializer, aPointerList2);

        // アドレス解決確認
        iSerializer.clearTracking();

        // 回復結果のチェック
        aPointerList.check(aPointeeList);
        aPointerList2.check(aPointeeList);
    }
#endif
}

INSTANTIATION_ALL(void, loadObjectTracking);

#endif  // DISABLE_OBJECT_TRACKING_TEST
