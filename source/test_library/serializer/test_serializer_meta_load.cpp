//############################################################################
//      各serializerのテスト
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

//############################################################################
//      通常のインクルード
//############################################################################

#include <sstream>
#include <fstream>
using namespace std;

#include <theolizer/test_tool.h>
#include <theolizer/serializer_json.h>
using theolizer::u8string;

#ifdef _MSC_VER
    #pragma warning(disable:4100 4127)
#endif

//############################################################################
//      テストのメイン関数
//############################################################################

int main(int argc, char** argv)
{
//  theolizer::DisplayPass aDisplayPass;

// ***************************************************************************
//      メタ・デシリアライズ
// ***************************************************************************

#ifdef THEOLIZER_INTERNAL_ENABLE_META_SERIALIZER
    {
        std::ifstream   aStream("test_meta_data.log");
        std::ofstream   aClassH("class.h");
        theolizer::JsonISerializer<>   js(aStream, aClassH);
    }
#endif  // THEOLIZER_INTERNAL_ENABLE_META_SERIALIZER

    return 0;
}

//############################################################################
//      グローバル・バージョン番号テーブル用
//############################################################################

THEOLIZER_GENERATED_GLOBAL_TABLE();
