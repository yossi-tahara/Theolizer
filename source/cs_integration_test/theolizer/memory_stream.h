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
#include <theolizer/temp.h>

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

    THEOLIZER_INTERNAL_DLL theolizer::StreamStatus CppRead
    (
        theolizer::OMemoryStream* iOMemoryStream,
        uint8_t* buffer,
        int offset,
        int count,
        int* out_count
    );
}

//############################################################################
//      C++内部処理
//############################################################################

namespace theolizer
{
// ***************************************************************************
//      C# → C++用メモリ・ストリーム
// ***************************************************************************

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
    void disconnect()
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mDisconnected = true;
        mConditionVariable.notify_all();
    }

//      ---<<< istream用(C# → C++) >>>---

    int mLastChar;
    int mNextChar;
    int mBuffChar;

    // バッファ先頭文字獲得
    int underflow() override
    {
        DEBUG_PRINT("underflow() ", this);

        if (mNextChar != EOF)
        {
    return mNextChar;
        }
        if (mBuffChar != EOF)
        {
    return mBuffChar;
        }
        std::unique_lock<std::mutex> lock(mMutex);
        mConditionVariable.wait(lock, [&]{ return mDisconnected || (mCount != 0); });
        if (mDisconnected)
    return EOF;

        mBuffChar = *mTransBuff;
        return mBuffChar;
    }

    // バッファ先頭文字を獲得し、消費する
    int uflow() override
    {
        DEBUG_PRINT("uflow() ", this);

        if (mNextChar != EOF)
        {
            mLastChar = mNextChar;
            mNextChar = EOF;
            mBuffChar = EOF;
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

        return mLastChar;
    }

    // 1文字戻す
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
DEBUG_PRINT("---------------- IMemoryStreamBuf::write() : [", temp, "]");

        std::unique_lock<std::mutex> lock(mMutex);

        mTransBuff = buffer;
        mCount = count;
        mConditionVariable.notify_all();

        // データ転送が終わるのを待つ
        mConditionVariable.wait(lock, [&]{ return mDisconnected || (mCount == 0); });
DEBUG_PRINT("---------------- IMemoryStreamBuf::write() : ", mDisconnected, ", ", mCount);

        return (mDisconnected)?StreamStatus::Disconnected:StreamStatus::NoError;
    }

//      ---<<< C++環境向けAPI >>>---

public:
    IMemoryStreamBuf() :
        mDisconnected(false),
        mLastChar(EOF),
        mNextChar(EOF),
        mBuffChar(EOF),
        mTransBuff(nullptr),
        mCount(0)
    {
        setbuf(nullptr, 0);
        DEBUG_PRINT("IMemoryStreamBuf() ", this);
    }
    ~IMemoryStreamBuf()
    {
        DEBUG_PRINT("~IMemoryStreamBuf() ", this);
    }
};

//----------------------------------------------------------------------------
//      ストリーム
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
    void disconnect()
    {
        mIMemoryStreamBuf.disconnect();
    }

//      ---<<< C# I/F中継 >>>---

    StreamStatus write(uint8_t* buffer, int count)
    {
        return mIMemoryStreamBuf.write(buffer, count);
    }

public:
    IMemoryStream() : std::istream(&mIMemoryStreamBuf)
    {
    }

    ~IMemoryStream()
    {
    }
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
    void disconnect()
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mDisconnected = true;
        mConditionVariable.notify_all();
    }

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

    void setSize(std::size_t iSize)
    {
        std::lock_guard<std::mutex> lock(mMutex);

        if (mWriteMan.mCount+mReadMan.mCount)
    throw std::runtime_error("can not resize theolizer::OMemoryStreamBuf while Count != 0.");

        mSize = static_cast<std::streamsize>(iSize);
        mWriteMan.mRead = mWriteMan.mWrite = 0;
        mReadMan.mRead  = mReadMan.mWrite  = 0;
        mBuffer.reset(new uint8_t[mSize]);
    }

//      ---<<< ostream用(C++ → C#) >>>---

    void flush()
    {
        mWriteMan.mRead = mReadMan.mRead;
        mReadMan.mWrite = mWriteMan.mWrite;
        mReadMan.mCount += mWriteMan.mCount;
        mWriteMan.mCount = 0;
    }

    int waitSpace()
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mConditionVariable.wait
        (
            lock,
            [&]{ return mDisconnected || ((mReadMan.mCount+mWriteMan.mCount) < mSize); }
        );
        if (mDisconnected)
    return EOF;

        mIsFull = false;

        // flushすることで書き込み側へ「空き」を移転する
        flush();

        return 0;
    }

    bool    mSynchronized;
    int sync() override
    {
        DEBUG_PRINT("sync() mSynchronized=", mSynchronized, " ", this);

        if (!mSynchronized)
        {
            DEBUG_PRINT("process sync() ");
            mSynchronized = true;
            std::lock_guard<std::mutex> lock(mMutex);
            flush();
            mConditionVariable.notify_all();
        }
        return 0;
    }

    int overflow(int c = EOF) override
    {
        DEBUG_PRINT("overflow(...,", c , ") ", this);

        if (mDisconnected)
    return EOF;

        // Fullなら空きを待つ
        if (mIsFull)
        {
            if (waitSpace() == EOF)
    return EOF;
        }

        mSynchronized = false;

        // enqueueする
        *(mBuffer.get()+mWriteMan.mWrite) = static_cast<uint8_t>(c);
        ++mWriteMan.mCount;
        ++mWriteMan.mWrite;
        if (mWriteMan.mWrite == mSize)
        {
            mWriteMan.mWrite = 0;
        }
        if (mWriteMan.mWrite == mWriteMan.mRead)
        {
            mIsFull = true;
        }

        // フル、もしくは、十分に溜まっていたらflushする
        if (mIsFull || (128 <= mWriteMan.mCount))
        {
            DEBUG_PRINT("call sync(1) mIsFull=", mIsFull);
    return sync();
        }
        return 0;
    }

    std::streamsize xsputn(char const* buffer, std::streamsize count) override
    {
        DEBUG_PRINT("xsputn(...,", count, ") ", this);

        std::streamsize ret = 0;
        while (ret < count)
        {
            if (mDisconnected)
    return EOF;

            // Fullなら空きを待つ
            if (mIsFull)
            {
                if (waitSpace() == EOF)
    return EOF;
            }

            mSynchronized = false;

            // 書き込み可能な空きバッファ・サイズ
            std::streamsize aCount = 0;
            if (mWriteMan.mWrite < mWriteMan.mRead)
            {
                aCount = mWriteMan.mRead - mWriteMan.mWrite;
            }
            else
            {
                aCount = (mSize - mWriteMan.mWrite) + mWriteMan.mRead;
            }

            // コピーするバイト数
            if ((count-ret) < aCount)
            {
                aCount = count-ret;
            }

            // バッファの終わりをまたぐ時は、まずバッファの最後までenqueue
            if ((mSize-mWriteMan.mWrite) < aCount)
            {
                // バッファの最後までコピー
                std::streamsize aLast = mSize-mWriteMan.mWrite;
                std::copy
                (
                    buffer,
                    buffer+aLast,
                    THEOLIZER_INTERNAL_UNCHECKED(mBuffer.get()+mWriteMan.mWrite)
                );
                ret    += aLast;
                buffer += aLast;
                aCount -= aLast;
                mWriteMan.mCount += aLast;
                mWriteMan.mWrite = 0;
            }

            // 残りをコピー
            std::copy
            (
                buffer,
                buffer+aCount,
                THEOLIZER_INTERNAL_UNCHECKED(mBuffer.get()+mWriteMan.mWrite)
            );
            ret              += aCount;
            buffer           += aCount;
            mWriteMan.mCount += aCount;
            mWriteMan.mWrite += aCount;
            if (mWriteMan.mWrite == mSize)
            {
                mWriteMan.mWrite = 0;
            }
            if (mWriteMan.mWrite == mWriteMan.mRead)
            {
                mIsFull = true;
            }

            // フル、もしくは、十分に溜まっていたらflushすす
            if (mIsFull || (128 <= mWriteMan.mCount))
            {
                DEBUG_PRINT("call sync(2) mIsFull=", mIsFull);
                if (sync() == EOF)
    return EOF;
            }
        }

        return ret;
    }

//      ---<<< C# I/F >>>---

    StreamStatus read(uint8_t* buffer, int count, int* out_count)
    {
DEBUG_PRINT("---------------- OMemoryStreamBuf::read() : mCount=", mReadMan.mCount);
uint8_t* debug_buf=buffer;

        std::streamsize ret=0;

        // ポインタ群をロックしてdequeue処理する
        {
            std::unique_lock<std::mutex> lock(mMutex);

            // 空ならデータ待ち
            if (mReadMan.mCount == 0)
            {
                mConditionVariable.wait(lock, [&]{return mDisconnected||(mReadMan.mCount != 0);});
            }

            // コピーするバイト数
            std::streamsize aCount = count;
            if (mReadMan.mCount < aCount)
            {
                aCount = mReadMan.mCount;
            }

            // バッファの終わりをまたぐ時は、まずバッファの最後までをdequeue
            if ((mSize-mReadMan.mRead) < aCount)
            {
                // バッファの最後までコピー
                std::copy
                (
                    mBuffer.get()+mReadMan.mRead,
                    mBuffer.get()+mSize,
                    THEOLIZER_INTERNAL_UNCHECKED(buffer)
                );
                std::streamsize aLast = mSize-mReadMan.mRead;
                buffer += aLast;
                ret    += aLast;
                aCount -= aLast;
                mReadMan.mCount -= aLast;
                mReadMan.mRead = 0;
            }

            // 残りをコピー
            std::copy
            (
                mBuffer.get()+mReadMan.mRead,
                mBuffer.get()+mReadMan.mRead+aCount,
                THEOLIZER_INTERNAL_UNCHECKED(buffer)
            );
            ret            += aCount;
            mReadMan.mCount-= aCount;
            mReadMan.mRead += aCount;
            if (mReadMan.mRead == mSize)
            {
                mReadMan.mRead = 0;
            }
        }

        // 空きが生じたことを通知する
        mConditionVariable.notify_all();

        *out_count = static_cast<int>(ret);

std::string temp((char*)debug_buf, *out_count);
DEBUG_PRINT("---------------- OMemoryStreamBuf::read() : ",
    mDisconnected, " out_count=", *out_count, " [", temp, "]");

        return (mDisconnected)?StreamStatus::Disconnected:StreamStatus::NoError;
    }

//      ---<<< C++環境向けAPI >>>---

public:
    OMemoryStreamBuf() :
        mDisconnected(false),
        mSize(1024),
        mIsFull(false),
        mBuffer(new uint8_t[mSize]),
        mSynchronized(false)
    {
        setbuf(nullptr, 0);
        DEBUG_PRINT("OMemoryStreamBuf() ", this);
    }
    ~OMemoryStreamBuf()
    {
        DEBUG_PRINT("~OMemoryStreamBuf() ", this);
    }
};

//----------------------------------------------------------------------------
//      ストリーム
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
    void disconnect()
    {
        mOMemoryStreamBuf.disconnect();
    }

    // FIFOサイズ設定
    void setSize(std::size_t iSize)
    {
        mOMemoryStreamBuf.setSize(iSize);
    }

//      ---<<< C# I/F中継 >>>---

    StreamStatus read(uint8_t* buffer, int count, int* out_count)
    {
        return mOMemoryStreamBuf.read(buffer, count, out_count);
    }
public:
    OMemoryStream() : std::ostream(&mOMemoryStreamBuf)
    {
    }

    ~OMemoryStream()
    {
    }
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
