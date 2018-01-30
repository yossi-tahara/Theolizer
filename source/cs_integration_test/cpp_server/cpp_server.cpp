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
#include "cpp_server.cpp.theolizer.hpp"

// ***************************************************************************
//      通知オブジェクト
// ***************************************************************************

NotifyData  gNotifyData;

//----------------------------------------------------------------------------
//      コンストラクタ／デストラクタ
//----------------------------------------------------------------------------

Notify::Notify(exchange::UserClassNotify* iInstance) : mTerminated(false)
{
    auto aIntegrator = theolizer::ThreadIntegrator::getIntegrator();
    mUserClassNotifyPointer = aIntegrator->getSharedPointer(iInstance);
}

Notify::~Notify()
{
    if (mThread.joinable())
    {
        mTerminated = true;
        mThread.join();
        mTerminated = false;
    }
}

//----------------------------------------------------------------------------
//      通知スレッド開始
//----------------------------------------------------------------------------

int Notify::startAsync()
{
    // UserClassNotifyが未登録ならエラー
    if (!mUserClassNotifyPointer)
return -2;

    // スレッド動作中ならエラー
    if (mThread.joinable())
return -1;

    // メイン・スレッドに登録されているインテグレータ取り出し
    //  ポインタの場合、ラムダ式内でアクセスする時、既にポインタは破棄されている
    //  そこで参照とする
    auto& aIntegrator = *theolizer::ThreadIntegrator::getIntegrator();

    // サブ・スレッド起動
    mThread = std::move
    (
        std::thread
        (
            [&]()
            {
                std::this_thread::sleep_for (std::chrono::seconds(3));
                if (!mTerminated)
                {
                    theolizer::ThreadIntegrator::setIntegrator(&aIntegrator);
                    auto& aUserClassNotify = mUserClassNotifyPointer.get();
                    ++aUserClassNotify.mCount;
                    aUserClassNotify.notify();
                    mThread.detach();
//gNotifyData.reset();
                }
            }
        )
    );
    return 1000;
}

// ***************************************************************************
//      user_main()
// ***************************************************************************

void user_main(theolizer::DllIntegrator& iDllIntegrator)
{
    DEBUG_PRINT("---------------- user_main()");

    iDllIntegrator.recieveLoop(1024, 4096);

    // 通知管理クラス開放
    gNotifyData.reset();
}
