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
#include <string>

// 固有ヘッダ
#include "exchange.h"
#include "cpp_server.h"
#include "exchange.cpp.theolizer.hpp"

// ***************************************************************************
//      メイン・クラス
// ***************************************************************************

namespace exchange
{

//----------------------------------------------------------------------------
//      メイン・クラス
//----------------------------------------------------------------------------

int UserClassMain::request(UserClassSub const& iUserClassSub, UserClassSub& ioUserClassSub2)
{
    std::cout << "request()\n"
              << "    mIntMain  =" << mIntMain << "\n"
              << "    mShortMain=" << mShortMain << "\n"
              << "    iUserClassSub.mUIntSub    =" << iUserClassSub.mUIntSub << "\n"
              << "    iUserClassSub.mStringSub  =" << iUserClassSub.mStringSub << "\n"
              << "    iUserClassSub.mShortSub   =" << iUserClassSub.mShortSub << "\n"
              << "    ioUserClassSub2.mUIntSub  =" << ioUserClassSub2.mUIntSub << "\n"
              << "    ioUserClassSub2.mStringSub=" << ioUserClassSub2.mStringSub << "\n"
              << "    ioUserClassSub2.mShortSub =" << ioUserClassSub2.mShortSub << "\n";
    mIntMain *= 2;
    ++mShortMain;
    ++ioUserClassSub2.mUIntSub;
    ++ioUserClassSub2.mShortSub;

    // 非同期処理起動
    return gNotify.startAsync(ioUserClassSub2);
}

}
