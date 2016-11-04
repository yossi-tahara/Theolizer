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
//      通常のインクルード
// ***************************************************************************

#include <iostream>
#include <string>

#include <theolizer/test_tool.h>
#include <theolizer/report.h>

// ***************************************************************************
//      ログ・ファイル生成トライのみ実行する
// ***************************************************************************

int main(int argc, char* argv[])
{
    if (argc < 2)
return -1;

    theolizer::WorkingLog aWorkingLog(argv[1]);

    return 0;
}
