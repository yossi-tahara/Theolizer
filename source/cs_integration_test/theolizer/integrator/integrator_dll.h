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

#if !defined(THEOLIZER_INTERNAL_INTEGRATOR_DLL_H)
#define THEOLIZER_INTERNAL_INTEGRATOR_DLL_H

#include <thread>
#include <utility>

// 対応する派生シリアライザを全てインクルードする
#include <theolizer/serializer_json.h>
#include <theolizer/serializer_binary.h>

#include "core_integrator.h"
#include "memory_stream.h"

//############################################################################
//      DLL用インテグレータ
//############################################################################

// ***************************************************************************
//      Friend指定用
// ***************************************************************************

namespace theolizer
{
namespace internal
{
    struct Streams;
}
}

extern "C"
{
    THEOLIZER_INTERNAL_EXPORT void CppInitialize(theolizer::internal::Streams* oStreams);
}

// ***************************************************************************
//      C++DLL用連携処理統括クラス
//          DLLの場合、通常１つしかインスタンス不要なのでシングルトンとする
// ***************************************************************************

namespace theolizer
{
//----------------------------------------------------------------------------
//      C#への接続情報
//----------------------------------------------------------------------------

namespace internal
{

struct Streams
{
    IMemoryStream*  mRequest;           // C#->Cpp要求用ストリーム
    OMemoryStream*  mResponse;          // Cpp->C#応答用ストリーム
    OMemoryStream*  mNotify;            // Cpp->C#通知用ストリーム

    // コンストラクタ
    Streams();

    // デストラクタ
    ~Streams();

private:
friend  void ::CppInitialize(Streams*);

    // コピー／ムーブ不可
    Streams(Streams const&) = delete;
    Streams(Streams     &&) = delete;
    Streams& operator=(Streams const&) = default;   // friendのみ許可
    Streams& operator=(Streams     &&) = delete;
};

}   // namespace internal

//----------------------------------------------------------------------------
//      本体
//----------------------------------------------------------------------------

class DllIntegrator : public internal::BaseIntegrator
{
    friend  void ::CppInitialize(internal::Streams*);

    std::thread*    mMainThread;        // メイン・スレッド

    // 生成／コピー／ムーブ不可
    DllIntegrator();
    DllIntegrator(DllIntegrator const&) = delete;
    DllIntegrator(DllIntegrator     &&) = delete;
    DllIntegrator& operator=(DllIntegrator const&) = delete;
    DllIntegrator& operator=(DllIntegrator     &&) = delete;

    class AutoTerminate
    {
        DllIntegrator&  mDllIntegrator;
    public:
        AutoTerminate(DllIntegrator& iDllIntegrator) : mDllIntegrator(iDllIntegrator) { }
        ~AutoTerminate()
        {
            mDllIntegrator.terminate();
        }
    };

    // メイン・スレッド起動と終了管理
    template<class F, class ...Args>
    void startThread(F&& f, Args&&... args)
    {
        if (!mMainThread)
        {
            mMainThread = new std::thread
                (
                    [&]()
                    {
                        AutoTerminate aAutoTerminate(*this);
                        f(std::forward<Args>(args)...);
                    }
                );
        }
    }

    // 終了
    void terminate()
    {
        if (!mTerminated)
        {
            mTerminated = true;
            if (mStreams.mRequest)  mStreams.mRequest->disconnect();
            if (mStreams.mResponse) mStreams.mResponse->disconnect();
            if (mStreams.mNotify)   mStreams.mNotify->disconnect();
        }
    }

    //      ---<<< 管理領域 >>>---

    internal::Streams                       mStreams;
    internal::Streams*                      getStreams() { return &mStreams; }
    theolizer::internal::BaseSerializer*    mRequestSerializer;
    theolizer::internal::BaseSerializer*    mResponseSerializer;
    theolizer::internal::BaseSerializer*    mNotirySerializer;

public:
    //      ---<<< API >>>---

    // コンストラクタ
    static DllIntegrator& getInstance()
    {
        static DllIntegrator instance;
        return instance;
    }
    // デストラクタ
    ~DllIntegrator();

    void setSize(std::size_t iResposeSize, std::size_t iNotifySize)
    {
        mStreams.mResponse->setSize(iResposeSize);
        mStreams.mNotify->setSize(iNotifySize);
    }

    // 受付処理開始
    //  派生シリアライザのコンストラクタへGlobalVersionNoTableを渡すためにヘッダで定義する。
    void start(SerializerType iSerializerType)
    {
        // 保存先指定は後日「連携先」へ変更する
        mRequestSerializer=makeISerializer<theolizerD::All>
            (
                iSerializerType, *mStreams.mRequest
            );

        mResponseSerializer=makeOSerializer<theolizerD::All>
            (
                iSerializerType, *mStreams.mResponse,
                mRequestSerializer->getGlobalVersionNo()
            );

        // C#からの受信処理
        while (!isTerminated())
        {
            callFunc(*mRequestSerializer, *mResponseSerializer);
        }
    }

    // 以下は削除予定
    std::istream& getRequestStream()  { return *(mStreams.mRequest); }
    std::ostream& getResponseStream() { return *(mStreams.mResponse); }
    std::ostream& getNotifyStream()   { return *(mStreams.mNotify); }
};

//############################################################################
//      End
//############################################################################

}   // namespace theolizer

#endif  // THEOLIZER_INTERNAL_INTEGRATOR_DLL_H
