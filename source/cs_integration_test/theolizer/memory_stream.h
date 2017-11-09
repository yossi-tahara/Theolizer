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

#include <theolizer/temp.h>

//############################################################################
//      C# I/F
//############################################################################

namespace theolizer
{
    class IMemoryStream;
    class OMemoryStream;

    enum class StreamStatus : int32_t
    {
        NoError,
        Disconnected
    };
}

extern "C"
{
    THEOLIZER_INTERNAL_DLL  theolizer::StreamStatus CppWrite
    (
        theolizer::IMemoryStream* iIMemoryStream,
        uint8_t* buffer,
        int offset,
        int count
    );
    THEOLIZER_INTERNAL_DLL  theolizer::StreamStatus CppFlush
    (
        theolizer::IMemoryStream* iIMemoryStream
    );
}

//############################################################################
//      C++内部処理
//############################################################################

namespace theolizer
{

// ***************************************************************************
//      ストリーム・バッファ
// ***************************************************************************

class MemoryStreamBuf : public std::streambuf
{
    friend  class IMemoryStream;
    friend  class OMemoryStream;

//----------------------------------------------------------------------------
//      排他制御用
//----------------------------------------------------------------------------

    std::mutex              mMutex;
    std::condition_variable mConditionVariable;

//----------------------------------------------------------------------------
//      切断要求
//----------------------------------------------------------------------------

    bool    mDisconnected;  // 切断
    void disconnect()
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mDisconnected = true;
        mConditionVariable.notify_all();
    }

//----------------------------------------------------------------------------
//      ostream用(C++ → C#)
//----------------------------------------------------------------------------

    std::streamsize xsputn(const char* s, std::streamsize n) override
    {
        DEBUG_PRINT("xsputn(...,", n , ") ", this);
        return n;
    }

//----------------------------------------------------------------------------
//      istream用(C# → C++)
//----------------------------------------------------------------------------

    int mLastChar;
    int mNextChar;
    int mBuffChar;

    int underflow() override
    {
        DEBUG_PRINT("underflow() ", this);

        if (mNextChar != EOF)
        {
DEBUG_PRINT("underflow(1) ", mNextChar);
    return mNextChar;
        }
        if (mBuffChar != EOF)
        {
DEBUG_PRINT("underflow(2) ", mBuffChar);
    return mBuffChar;
        }
        std::unique_lock<std::mutex> lock(mMutex);
        mConditionVariable.wait(lock, [&]{ return mDisconnected || (mCount != 0); });
        if (mDisconnected)
    return EOF;

        mBuffChar = *mTransBuff;
DEBUG_PRINT("underflow(3) ", mBuffChar);
        return mBuffChar;
    }

    int uflow() override
    {
        DEBUG_PRINT("uflow() ", this);

        if (mNextChar != EOF)
        {
            mLastChar = mNextChar;
            mNextChar = EOF;
            mBuffChar = EOF;
DEBUG_PRINT("uflow() ", mLastChar);
    return mLastChar;
        }

        std::unique_lock<std::mutex> lock(mMutex);
        mConditionVariable.wait(lock, [&]{ return mDisconnected || (mCount != 0); });
        if (mDisconnected)
    return EOF;

        mLastChar = *mTransBuff++;
        --mCount;
        mBuffChar = EOF;
        if (mCount == 0)
        {
            mConditionVariable.notify_all();
        }

DEBUG_PRINT("uflow() ", mLastChar);
        return mLastChar;
    }

    int pbackfail(int c = EOF) override
    {
        DEBUG_PRINT("pbackfail(", c ,") EOF=", EOF, " ", this);

        // 1回のみ戻せるものとする
        if (mNextChar != EOF)
    return EOF;

        mNextChar = mLastChar;
        return mNextChar;
    }


//      ---<<< C# I/F >>>---

    uint8_t*    mTransBuff;
    int         mCount;
    StreamStatus write(uint8_t* buffer, int count)
    {
std::string temp((char*)buffer, count);
DEBUG_PRINT("---------------- MemoryStreamBuf::write() : ", temp.c_str());

        std::unique_lock<std::mutex> lock(mMutex);

        mTransBuff = buffer;
        mCount = count;
        mConditionVariable.notify_all();

        // データ転送が終わるのを待つ
        mConditionVariable.wait(lock, [&]{ return mDisconnected || (mCount == 0); });
DEBUG_PRINT("---------------- MemoryStreamBuf::write() : ", mDisconnected, ", ", mCount);

        return (mDisconnected)?StreamStatus::Disconnected:StreamStatus::NoError;
    }

//----------------------------------------------------------------------------
//      C++環境向けAPI
//----------------------------------------------------------------------------

public:
    MemoryStreamBuf() :
        mDisconnected(false),
        mLastChar(EOF),
        mNextChar(EOF),
        mBuffChar(EOF),
        mTransBuff(nullptr),
        mCount(0)
    {
        setbuf(nullptr, 0);
        DEBUG_PRINT("MemoryStreamBuf() ", this);
    }
    ~MemoryStreamBuf()
    {
        DEBUG_PRINT("~MemoryStreamBuf() ", this);
    }
};

// ***************************************************************************
//      C# → C++用メモリ・ストリーム
// ***************************************************************************

class IMemoryStream : public std::istream
{
    friend enum class StreamStatus (::CppWrite)
    (
        IMemoryStream* iIMemoryStream,
        uint8_t* buffer,
        int offset,
        int count
    );

    friend  class DllIntegrator;

    // streambuf
    MemoryStreamBuf mMemoryStreamBuf;

    // 切断要求中継
    void disconnect()
    {
        mMemoryStreamBuf.disconnect();
    }

//      ---<<< C# I/F中継 >>>---

    StreamStatus write(uint8_t* buffer, int count)
    {
        return mMemoryStreamBuf.write(buffer, count);
    }

public:
    IMemoryStream() : std::istream(&mMemoryStreamBuf)
    {
    }

    ~IMemoryStream()
    {
    }
};

// ***************************************************************************
//      C++ → C#用メモリ・ストリーム
// ***************************************************************************

class OMemoryStream : public std::ostream
{
    MemoryStreamBuf mMemoryStreamBuf;
public:
    OMemoryStream() : std::ostream(&mMemoryStreamBuf)
    {
    }

    ~OMemoryStream()
    {
    }

    // 切断要求中継
    void disconnect()
    {
        mMemoryStreamBuf.disconnect();
    }
};

//############################################################################
//      End
//############################################################################

}   // namespace theolizer

#endif  // THEOLIZER_INTERNAL_MEMORY_STREAM_H
