//############################################################################
//      各serializerのテスト
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

//############################################################################
//      通常のインクルード
//############################################################################

#include <sstream>
#include <fstream>
using namespace std;

#include <theolizer/test_tool.h>
#include <theolizer/serializer_json.h>
using theolizer::u8string;

//############################################################################
//      テストのメイン関数
//############################################################################

int main(int argc, char** argv)
{
//  theolizer::DisplayPass aDisplayPass;

// ***************************************************************************
//      メタ・デシリアライズ
// ***************************************************************************

    {
        std::ifstream   aStream("test_meta_data.log");
        std::ofstream   aClassH("class.h");
        theolizer::JsonISerializer<>   js(aStream, aClassH);
    }

    return 0;
}

//############################################################################
//      グローバル・バージョン番号テーブル用
//############################################################################

THEOLIZER_GENERATED_GLOBAL_TABLE();
