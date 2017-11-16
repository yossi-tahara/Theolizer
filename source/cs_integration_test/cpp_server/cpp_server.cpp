﻿//############################################################################
//      Theolizer Test Project for C# Integration
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

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#if defined(_MSC_VER)
//  #pragma warning(disable:4100)
#endif

// ***************************************************************************
//      インクルード
// ***************************************************************************

// 標準ライブラリ
#include <iostream>
#include <string>
#include <sstream>

// 固有ヘッダ
#define DLL_EXPORT
#include "cpp_server.h"

// ***************************************************************************
//      main()
// ***************************************************************************

int main()
{
    DEBUG_PRINT("---------------- main()");

    auto&   aDllIntegrator  = theolizer::DllIntegrator::get();
    aDllIntegrator.setSize(20, 4096);

    auto&   aRequestStream  = aDllIntegrator.getRequestStream();
    auto&   aResponseStream = aDllIntegrator.getResponseStream();

    while (!aDllIntegrator.isTerminated())
    {
        int     aInt=123456789;
        DEBUG_PRINT("----------------(1) aInt=", aInt, " rdstate()=", aRequestStream.rdstate());
        aRequestStream >> aInt;
        DEBUG_PRINT("----------------(2) aInt=", aInt, " rdstate()=", aRequestStream.rdstate());

#if 0   // エラー発生テスト用(xsputnで戻り値が小さい時など)
        if (!aRequestStream.good())
        {
            aRequestStream.clear();
            std::string dummy;
            aRequestStream >> dummy;
        }
#endif
#if 0   // unget()デバッグ用
        aRequestStream.unget();
        aRequestStream >> aInt;
        DEBUG_PRINT("----------------(3) aInt=", aInt, " rdstate()=", aRequestStream.rdstate());
#endif

        // 改行の読み捨て
        std::string temp;
        std::getline(aRequestStream, temp);

        // 要求データを全て受け取ってから応答を返却するべき
        aResponseStream << aInt*2 << " tes" << "ted " << "ABC" << std::endl;
    }

    return 0;
}