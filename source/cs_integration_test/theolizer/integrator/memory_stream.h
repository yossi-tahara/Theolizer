//############################################################################
//      Theolizerライブラリのメモリ・ストリーム(C++側)
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

#if !defined(THEOLIZER_INTERNAL_MEMORY_STREAM_H)
#define THEOLIZER_INTERNAL_MEMORY_STREAM_H

// 標準ライブラリ
#include <iostream>
#include <string>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <cstdint>

// theolizerライブラリ
#include "base_integrator.h"

//############################################################################
//      Begin
//############################################################################

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable:4100)
#endif

//############################################################################
//      C# I/F
//############################################################################

// ***************************************************************************
//      Friend指定用
// ***************************************************************************

namespace theolizer
{
    class IMemoryStream;
    class OMemoryStream;

    // ストリームの状態
    enum class StreamStatus : int32_t
    {
        NoError,
        Disconnected
    };
}

extern "C"
{
    THEOLIZER_INTERNAL_EXPORT theolizer::StreamStatus CppWrite
    (
        theolizer::IMemoryStream* iIMemoryStream,
        uint8_t* buffer,
        int offset,
        int count
    );
    THEOLIZER_INTERNAL_EXPORT theolizer::StreamStatus CppFlush
    (
        theolizer::IMemoryStream* iIMemoryStream
    );

    THEOLIZER_INTERNAL_EXPORT theolizer::StreamStatus CppRead
    (
        theolizer::OMemoryStream* iOMemoryStream,
        uint8_t* buffer,
        int offset,
        int count,
        int* out_count
    );
}

// ***************************************************************************
//      C# → C++用メモリ・ストリーム
// ***************************************************************************

namespace theolizer
{

//----------------------------------------------------------------------------
//      IMemoryStream用ストリーム・バッファ
//----------------------------------------------------------------------------

class IMemoryStreamBuf : public std::streambuf
{
    friend  class IMemoryStream;
    friend  class OMemoryStream;

//      ---<<< 排他制御用 >>>---

    std::mutex              mMutex;
    std::condition_variable mConditionVariable;

//      ---<<< 切断要求 >>>---

    bool    mDisconnected;  // 切断
    void disconnect();

//      ---<<< istream用(C# → C++) >>>---

    int mLastChar;
    int mNextChar;
    int mBuffChar;

    int underflow() override;
    int uflow() override;
    int pbackfail(int c = EOF) override;

//      ---<<< C# I/F >>>---

    uint8_t*    mTransBuff;
    int         mCount;
    StreamStatus write(uint8_t* buffer, int count);

//      ---<<< C++環境向けAPI >>>---

public:
    IMemoryStreamBuf();
    ~IMemoryStreamBuf();
};

//----------------------------------------------------------------------------
//      IMemoryStream
//----------------------------------------------------------------------------

class IMemoryStream : public std::istream
{
    friend StreamStatus (::CppWrite)
    (
        IMemoryStream* iIMemoryStream,
        uint8_t* buffer,
        int offset,
        int count
    );

    friend  class DllIntegrator;

    // streambuf
    IMemoryStreamBuf mIMemoryStreamBuf;

    // 切断要求中継
    void disconnect();

    // C# I/F中継
    StreamStatus write(uint8_t* buffer, int count);

public:
    IMemoryStream();
    ~IMemoryStream();
};

// ***************************************************************************
//      C++ → C#用メモリ・ストリーム
// ***************************************************************************

//----------------------------------------------------------------------------
//      OMemoryStream用ストリーム・バッファ
//----------------------------------------------------------------------------

class OMemoryStreamBuf : public std::streambuf
{
    friend  class OMemoryStream;

//      ---<<< 排他制御用 >>>---

    std::mutex              mMutex;
    std::condition_variable mConditionVariable;

//      ---<<< 切断要求 >>>---

    bool    mDisconnected;  // 切断
    void disconnect();

//      ---<<< Queue >>>---

    std::streamsize             mSize;          // バッファのバイト数
    bool                        mIsFull;        // フル
    struct Man
    {
        std::streamsize         mCount;         // 有効バイト数
        std::streamsize         mRead;          // リード位置
        std::streamsize         mWrite;         // ライト位置
        Man() : mCount(0), mRead(0), mWrite(0) { }
    } mWriteMan, mReadMan;
    std::unique_ptr<uint8_t[]>  mBuffer;

    void setSize(std::size_t iSize);
    void flush();
    int waitSpace();

//      ---<<< ostream用(C++ → C#) >>>---

    bool    mSynchronized;
    int sync() override;
    int overflow(int c = EOF) override;
    std::streamsize xsputn(char const* buffer, std::streamsize count) override;

//      ---<<< C# I/F >>>---

    StreamStatus read(uint8_t* buffer, int count, int* out_count);

//      ---<<< C++環境向けAPI >>>---

public:
    OMemoryStreamBuf();
    ~OMemoryStreamBuf();
};

//----------------------------------------------------------------------------
//      OMemoryStream
//----------------------------------------------------------------------------

class OMemoryStream : public std::ostream
{
    friend StreamStatus (::CppRead)
    (
        OMemoryStream* iOMemoryStream,
        uint8_t* buffer,
        int offset,
        int count,
        int* out_count
    );

    friend  class DllIntegrator;

    OMemoryStreamBuf mOMemoryStreamBuf;

    // 切断要求中継
    void disconnect();

    // FIFOサイズ設定
    void setSize(std::size_t iSize);

    // C# I/F中継
    StreamStatus read(uint8_t* buffer, int count, int* out_count);

public:
    OMemoryStream();
    ~OMemoryStream();
};

//############################################################################
//      End
//############################################################################

}   // namespace theolizer

// ***************************************************************************
//          警告抑止解除
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#endif  // THEOLIZER_INTERNAL_MEMORY_STREAM_H
