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

#include <theolizer/test_tool.h>
#include <theolizer/report.h>
using theolizer::u8string;

// ***************************************************************************
//      エラー・ログのパスを返却する関数
//          異常テストのため、２重定義している
// ***************************************************************************

THEOLIZER_ERRORLOG_FILE("ErrorLogFile%1%.log");

// ***************************************************************************
//      テストのメイン関数
//          多重登録により、main()起動前に終了する
//          main()が実行されないことを検証している
// ***************************************************************************

void dummy();

int main(int argc, char** argv)
{
//  theolizer::DisplayPass aDisplayPass;

//----------------------------------------------------------------------------
//      リンクできていることの確認用
//----------------------------------------------------------------------------

    dummy();

//----------------------------------------------------------------------------
//      異常終了した時、PASSするようCMakeLists.txtで指定しているので
//      パグがあったら正常終了してテストがFAILするようにする。
//----------------------------------------------------------------------------

    std::cout << "error: Test is fail, if this message is printed.\n";

    return 0;
}
