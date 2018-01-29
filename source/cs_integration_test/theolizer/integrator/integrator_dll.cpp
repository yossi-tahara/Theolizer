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

#if defined(_WIN32)
    #define NOMINMAX
    #include <windows.h>
#endif

#include "integrator_dll.h"

//############################################################################
//      C# I/F
//############################################################################

//----------------------------------------------------------------------------
//      スレッド起動処理(ユーザ()呼び出し)
//----------------------------------------------------------------------------

void CppInitialize
(
    theolizer::internal::Streams* oStreams,
    theolizer::internal::DelegateNotifySharedObject iCallback,
    theolizer::SerializerType iSerializerType,
    bool iNotify
)
{
    auto& aDllIntegrator =
        theolizer::DllIntegrator::makeInstance(iCallback, iSerializerType, iNotify);
    *oStreams = *aDllIntegrator.getStreams();

    aDllIntegrator.startMainThread();
}

void CppFinalize()
{
    theolizer::DllIntegrator::disposeInstance();
}

namespace theolizer
{

//----------------------------------------------------------------------------
//      C#への接続情報
//----------------------------------------------------------------------------

namespace internal
{

Streams::Streams(bool iNotify) :
    mRequest(new IMemoryStream),
    mResponse(new OMemoryStream),
    mNotify(nullptr)
{
    if (iNotify)
    {
        mNotify = new OMemoryStream;
    }
}

Streams::~Streams()
{
    delete mNotify;
    delete mRequest;
    delete mResponse;
}

}   // namespace internal

//----------------------------------------------------------------------------
//      コンストラクタ
//----------------------------------------------------------------------------

DllIntegrator*  DllIntegrator::sDllIntegrator = nullptr;

#if defined(_WIN32)
FILE *fp;
#endif

DllIntegrator::DllIntegrator
(
    theolizer::internal::DelegateNotifySharedObject iCallback,
    theolizer::SerializerType iSerializerType,
    bool iNotify
) : BaseIntegrator(iCallback),
    mSerializerType(iSerializerType),
    mNotify(iNotify),
    mMainThread(nullptr),
    mStreams(iNotify),
    mRequestSerializer(nullptr),
    mResponseSerializer(nullptr),
    mNotifySerializer(nullptr)
{
#if defined(_WIN32)
AllocConsole();
freopen_s(&fp, "CON", "w", stdout);
#endif
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
    delete mNotifySerializer;
    delete mRequestSerializer;
    delete mResponseSerializer;

#if defined(_WIN32)
fclose(fp);
FreeConsole();
#endif
}

}   // namespace theolizer
