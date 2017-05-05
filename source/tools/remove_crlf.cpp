//############################################################################
//      CR, LFを統一してargv[1]ファイルへ出力する
//          改行コードがOS毎に異なるため、ソースのハッシュ値計算用に統一する。
//          LF   → LF  (Unix)
//          CRLF → LF  (Windows)
//          CR   → LF  (Mac)
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

#include <iostream>
#include <fstream>
#include <string>

// ***************************************************************************
//      メイン
// ***************************************************************************

#define CR  0x0d
#define LF  0x0a

int main(int iArgc, const char **iArgv)
{
    if (iArgc < 3)
    {
        std::cout << "usage : remove_crlf <output file> -- <input files> ...\n";
return 1;
    }

    // ミス防止
    if (std::string(iArgv[2]) != "--")
    {
        std::cout << "usage : remove_crlf <output file> -- <input files> ...\n";
return 2;
    }

    std::ofstream ofs(iArgv[1], std::ios_base::binary);
    for (int i=3; i < iArgc; ++i)
    {
        std::ifstream ifs(iArgv[i], std::ios_base::binary);
        if (!ifs)
return 3;

        char ch;
        bool aLastIsCr=false;
        while(ifs.get(ch))
        {
            if (aLastIsCr && (ch == LF))
            {
                aLastIsCr=false;
        continue;
            }
            if (ch == CR)
            {
                aLastIsCr=true;
                ofs.put(LF);
            }
            else
            {
                aLastIsCr=false;
                ofs.put(ch);
            }
        }
    }

    return 0;
}
