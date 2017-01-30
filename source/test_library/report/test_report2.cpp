//############################################################################
//      report.*のテスト
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

#define THEOLIZER_INTERNAL_EXCLUDE_VERSION_H

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#if defined(_MSC_VER)
    #pragma warning(disable:4100)
#endif

// ***************************************************************************
//      通常のインクルード
// ***************************************************************************

#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
using namespace std;

#include <theolizer/test_tool.h>
#include <theolizer/report.h>
using theolizer::u8string;

// ***************************************************************************
//      テストのメイン関数
// ***************************************************************************

int main(int argc, char** argv)
{
//  theolizer::DisplayPass aDisplayPass;

//----------------------------------------------------------------------------
//      WorkingLogクラス・エラー・ログの異常系テスト
//
//          概要
//              下記を確認する
//                  THEOLIZER_ERRORLOG_FILEがない時にfalseが返却されること
//----------------------------------------------------------------------------

    {
//      ---<<< 例外チェック >>>---

        THEOLIZER_EQUAL(THEOLIZER_INTERNAL_WARNING(u8"Error Message"), false);
    }

    return 0;
}
