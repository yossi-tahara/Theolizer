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
//      ---<<< 保存処理 >>>---

        std::ofstream   aStream("tutorise_object_tracking.json");
        theolizer::JsonOSerializer<> aSerializer(aStream);


        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();
    }

//----------------------------------------------------------------------------
//      回復
//----------------------------------------------------------------------------

    {
//      ---<<< 回復処理 >>>---

        std::ifstream   aStream("tutorise_object_tracking.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();
    }

    std::cout << "tutoriseObjectTracking() end\n" << std::endl;
}

#endif  // DISABLE_OBJECT_TRACKING_TEST
