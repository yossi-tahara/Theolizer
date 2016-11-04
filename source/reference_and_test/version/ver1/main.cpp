//############################################################################
//      Theolizer解説用サンプル・プログラム
//
//          簡単な家計簿用データ構造定義
//              データの保存と回復のサンプルなので、
//              ユーザ・インタフェースは実装しない
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

// ***************************************************************************
//      インクルード
// ***************************************************************************

// 標準ライブラリ
#include <iostream>
#include <fstream>

// 構造体定義
#include "main.h"

// Theolizerライブラリ（各種シリアライズ指示する時は構造体定義前に#include)
#include <theolizer/serializer_json.h>

// Theolizer自動生成先
#include "main.cpp.theolizer.hpp"

// ***************************************************************************
//      メイン
// ***************************************************************************

int main(int argc, char* argv[])
{
//----------------------------------------------------------------------------
//      保存
//----------------------------------------------------------------------------

    // 保存するデータの設定
    Trade  aTradeSave;
    aTradeSave.mDate.mYear  = 2015;
    aTradeSave.mDate.mMonth = 1;
    aTradeSave.mDate.mDay   = 1;
    aTradeSave.mItem        = u8"お米10Kg";
    aTradeSave.mAmount      = 4600;

    // 保存処理
    {
        std::ofstream   aStream("test.log");
        theolizer::JsonOSerializer<>  js(aStream);
        THEOLIZER_PROCESS(js, aTradeSave);
    }

//----------------------------------------------------------------------------
//      回復
//----------------------------------------------------------------------------

    // 回復先の領域
    Trade  aTradeLoad;

    // 回復処理
    {
        std::ifstream   aStream("test.log");
        theolizer::JsonISerializer<>  js(aStream);
        THEOLIZER_PROCESS(js, aTradeLoad);
    }

    // 結果表示
    std::cout << theolizer::print("%04d/%02d/%02d %s %u\n",
        aTradeLoad.mDate.mYear,
        aTradeLoad.mDate.mMonth+0,
        aTradeLoad.mDate.mDay+0,
        aTradeLoad.mItem, aTradeLoad.mAmount);

    return 0;
}
