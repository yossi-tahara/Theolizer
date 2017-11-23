//############################################################################
//      Theolizerライブラリの連携処理クラス(C++側dll用)
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

#define THEOLIZER_NO_ANALYZE

#define NOMINMAX
#include <windows.h>

#include "integrator_dll.h"

//############################################################################
//      C# I/F
//############################################################################

//----------------------------------------------------------------------------
//      スレッド起動処理(main()呼び出し)
//----------------------------------------------------------------------------

int main(theolizer::DllIntegrator& iDllIntegrator);

void CppInitialize(theolizer::internal::Streams* oStreams)
{
    auto&   aDllIntegrator = theolizer::DllIntegrator::getInstance();
    *oStreams = *aDllIntegrator.getStreams();

    aDllIntegrator.startThread(main, aDllIntegrator);
}

namespace theolizer
{

//----------------------------------------------------------------------------
//      C#への接続情報
//----------------------------------------------------------------------------

namespace internal
{

Streams::Streams() :
    mRequest(new IMemoryStream),
    mResponse(new OMemoryStream),
    mNotify(new OMemoryStream)
{ }

Streams::~Streams()
{
    delete mResponse;
    delete mRequest;
    delete mNotify;
}

}   // namespace internal

//----------------------------------------------------------------------------
//      コンストラクタ
//----------------------------------------------------------------------------

FILE *fp;
DllIntegrator::DllIntegrator() :
    mMainThread(nullptr),
    mStreams(),
    mRequestSerializer(nullptr),
    mResponseSerializer(nullptr),
    mNotirySerializer(nullptr)
{
AllocConsole();
freopen_s(&fp, "CON", "w", stdout);
}

//----------------------------------------------------------------------------
//      デストラクタ(スレッド停止処理)
//----------------------------------------------------------------------------

DllIntegrator::~DllIntegrator()
{
    terminate();
    if (mMainThread)
    {
        mMainThread->join();
        delete mMainThread;
    }
    deleteSerializer(mResponseSerializer);
    deleteSerializer(mRequestSerializer);
    deleteSerializer(mNotirySerializer);

fclose(fp);
FreeConsole();
}

}   // namespace theolizer
