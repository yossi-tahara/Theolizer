//############################################################################
//      report.*のテスト
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
