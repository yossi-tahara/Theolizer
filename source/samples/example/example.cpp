//############################################################################
//      Theolizer紹介用サンプル・プログラム
//############################################################################

// ***************************************************************************
//      インクルード
// ***************************************************************************

#include <fstream>
#include "example.h"
#include "example.cpp.theolizer.hpp"                // Theolizer自動生成先

// ***************************************************************************
//      メイン
// ***************************************************************************

int main(int argc, char* argv[])
{
//----------------------------------------------------------------------------
//      保存
//----------------------------------------------------------------------------

    try
    {
        // データを生成する
        StructType aStructType;
        aStructType.mEnum   = EnumB;
        aStructType.mInt    = 1001;
        aStructType.mString = u8"ＵＴＦ－８　ｓｔｒｉｎｇ";

        std::ofstream   aStream("example.json");    // 保存先のファイルをオープンする
        theolizer::JsonOSerializer<> js(aStream);   // シリアライザを用意する
        THEOLIZER_PROCESS(js, aStructType);         // test.jsonファイルへ保存する
    }
    catch(theolizer::ErrorInfo& e)
    {
        std::cerr << e.getString() << "\n";
return 1;
    }

//----------------------------------------------------------------------------
//      回復
//----------------------------------------------------------------------------

    try
    {
        StructType aStructType;                     // データ領域を獲得する
        std::ifstream   aStream("example.json");    // 回復元のファイルをオープンする
        theolizer::JsonISerializer<> js(aStream);   // シリアライザを用意する
        THEOLIZER_PROCESS(js, aStructType);         // test.jsonファイルから回復する

        // 回復結果を表示する
        std::cout <<theolizer::print
                    (
                        u8"mEnum=%d mInt=%d mString=[%s]\n",
                        aStructType.mEnum,
                        aStructType.mInt,
                        aStructType.mString
                    );
    }
    catch(theolizer::ErrorInfo& e)
    {
        std::cerr << e.getString() << "\n";
return 2;
    }

    return 0;
}
