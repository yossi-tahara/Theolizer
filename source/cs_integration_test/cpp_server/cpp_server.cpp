//############################################################################
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
#include "cpp_server.h"
#include "exchange.h"
#include "cpp_server.cpp.theolizer.hpp"

// ***************************************************************************
//      main()
// ***************************************************************************

int main()
{
    DEBUG_PRINT("---------------- main()");

    {
///        theolizer::JsonOSerializer<> jos(std::cout);  // シリアライザを生成
    }
    {
        exchange::func0Theolizer    afunc0Theolizer;
        afunc0Theolizer.mThis->mIntMain=123;
        afunc0Theolizer.miUserClassSub->mUIntSub=456;
        afunc0Theolizer.miUserClassSub->mStringSub="test";

        theolizer::JsonOSerializer<> jos(std::cout);
        THEOLIZER_PROCESS(jos, afunc0Theolizer);
    }


    auto&   aDllIntegrator  = theolizer::DllIntegrator::getInstance();
    aDllIntegrator.setSize(1024, 4096);

    auto&   aRequestStream  = aDllIntegrator.getRequestStream();
    auto&   aResponseStream = aDllIntegrator.getResponseStream();

    theolizer::JsonISerializer<> jis(aRequestStream);
    theolizer::JsonOSerializer<> debug(std::cout);
    theolizer::JsonOSerializer<> jos(aResponseStream);

    while (!aDllIntegrator.isTerminated())
    {
#if 0
        std::string aLine;
        getline(aRequestStream, aLine);
        aResponseStream << aLine << std::endl;
#else
        exchange::func0Theolizer    afunc0Theolizer;
        THEOLIZER_PROCESS(jis, afunc0Theolizer);
        THEOLIZER_PROCESS(debug, afunc0Theolizer);
        THEOLIZER_PROCESS(jos, afunc0Theolizer);
        aResponseStream.flush();
#endif
    }

    return 0;
}
