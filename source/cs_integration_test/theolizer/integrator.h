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

#if !defined(THEOLIZER_INTERNAL_INTEGRATOR_H)
#define THEOLIZER_INTERNAL_INTEGRATOR_H

#include <memory>
#include <thread>

#include "memory_stream.h"

//############################################################################
//      Begin
//############################################################################

// ***************************************************************************
//      C++DLL用連携処理統括クラス
//          DLLの場合、通常１つしかインスタンス不要なのでシングルトンとする
// ***************************************************************************

//----------------------------------------------------------------------------
//      サーバ初期化
//----------------------------------------------------------------------------

namespace theolizer
{
    struct Streams;
}   // theolizer

extern "C"  THEOLIZER_INTERNAL_DLL  void CppInitialize(theolizer::Streams* oStreams);

//----------------------------------------------------------------------------
//      C#への接続情報
//----------------------------------------------------------------------------

namespace theolizer
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

//----------------------------------------------------------------------------
//      本体
//----------------------------------------------------------------------------

class DllIntegrator
{
    friend  void ::CppInitialize(Streams*);

    std::thread*    mMainThread;

    // 生成／コピー／ムーブ不可
    DllIntegrator() : mMainThread(nullptr) { }
    DllIntegrator(DllIntegrator const&) = delete;
    DllIntegrator(DllIntegrator     &&) = delete;
    DllIntegrator& operator=(DllIntegrator const&) = delete;
    DllIntegrator& operator=(DllIntegrator     &&) = delete;

    template <class F, class ...Args>
    void startThread(F&& f, Args&&... args)
    {
        if (!mMainThread)
        {
            mMainThread = new std::thread(f, args...);
        }
    }

    //      ---<<< 管理領域 >>>---

    Streams mStreams;

public:
    //      ---<<< API >>>---

    static DllIntegrator& get()
    {
        static DllIntegrator instance;
        return instance;
    }
    ~DllIntegrator();

    Streams* getStreams() { return &mStreams; }
};

//############################################################################
//      End
//############################################################################

}   // namespace theolizer

#endif  // THEOLIZER_INTERNAL_INTEGRATOR_H
