//############################################################################
//      Theolizerライブラリの連携処理クラス(C++側)
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

#define NOMINMAX
#include <windows.h>

#define DLL_EXPORT
#include "integrator.h"

// ***************************************************************************
//      C++DLL用連携処理統括クラス
//          DLLの場合、通常１つしかインスタンス不要なのでシングルトンとする
// ***************************************************************************

//----------------------------------------------------------------------------
//      スレッド起動処理(main()呼び出し)
//----------------------------------------------------------------------------

extern "C" int main();

void CppInitialize(theolizer::Streams* oStreams)
{
    auto&   aDllIntegrator = theolizer::DllIntegrator::getInstance();
DEBUG_PRINT("mRequest  = ", aDllIntegrator.getStreams()->mRequest);
DEBUG_PRINT("mResponse = ", aDllIntegrator.getStreams()->mResponse);
DEBUG_PRINT("mNotify   = ", aDllIntegrator.getStreams()->mNotify);
    *oStreams = *aDllIntegrator.getStreams();

    aDllIntegrator.startThread(main);
}

//----------------------------------------------------------------------------
//      C#への接続情報
//----------------------------------------------------------------------------

namespace theolizer
{

Streams::Streams() :
    mRequest(new IMemoryStream),
    mResponse(new OMemoryStream),
    mNotify(new OMemoryStream)
{ }

Streams::~Streams()
{
    delete mRequest;
    delete mResponse;
    delete mNotify;
}

//----------------------------------------------------------------------------
//      コンストラクタ
//----------------------------------------------------------------------------
FILE *fp;
DllIntegrator::DllIntegrator() : mMainThread(nullptr), mTerminated(false)
{
AllocConsole();
freopen_s(&fp, "CON", "w", stdout);
}

//----------------------------------------------------------------------------
//      デストラクタ(スレッド停止処理)
//----------------------------------------------------------------------------

DllIntegrator::~DllIntegrator()
{
    if (mMainThread)
    {
        mMainThread->join();
        delete mMainThread;
    }
fclose(fp);
FreeConsole();
}

}   // namespace theolizer
