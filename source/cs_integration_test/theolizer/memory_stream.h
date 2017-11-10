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
//      簡易循環キュー
//          排他制御は外部で行うこと
// ***************************************************************************

class Queue
{
    std::size_t                 mSize;
    std::size_t                 mCount;
    std::size_t                 mRead;
    std::size_t                 mWrite;
    std::unique_ptr<uint8_t[]>  mBuffer;
public:
    Queue(std::size_t iSize) :
        mSize(iSize),
        mCount(0),
        mRead(0),
        mWrite(0),
        mBuffer(new uint8_t[mSize])
    { }
    void resize(std::size_t iSize)
    {
        if (mCount)
    throw std::runtime_error("can not resize theolizer::Quque while Count != 0.");

        mSize = iSize;
        mRead = mWrite = 0;
        mBuffer.reset(new uint8_t[mSize]);
    }

    // 戻り値はdequeueできた要素数
    std::size_t dequeue(uint8_t* oData, std::size_t iCount)
    {
        DEBUG_PRINT("dequeue() mCount=", mCount, " iCount=", iCount);

        if (mCount == 0)
    return 0;

        // 残数より多くdequeueしようとした時は、有る分だけdequeue
        if (mCount < iCount)
    return dequeue(oData, mCount);

        // バッファの終わりをまたぐ時は、まずバッファの最後までをdequeue
        std::size_t ret=0;
        if ((mSize-mRead) < iCount)
        {
            // バッファの最後までコピー
            std::copy
            (
                mBuffer.get()+mRead,
                mBuffer.get()+mSize,
                THEOLIZER_INTERNAL_UNCHECKED(oData)
            );
            std::size_t aCopy = mSize-mRead;
            oData  += aCopy;
            ret    += aCopy;
            iCount -= aCopy;
            mCount -= aCopy;
            mRead = 0;
        }
{std::string temp((char*)mBuffer.get()+mRead, iCount);
DEBUG_PRINT("dequeue(2) ", temp);}

        // 残りをコピー
        std::copy
        (
            mBuffer.get()+mRead,
            mBuffer.get()+mRead+iCount,
            THEOLIZER_INTERNAL_UNCHECKED(oData)
        );
        ret    += iCount;
        mCount -= iCount;
        mRead  += iCount;
{std::string temp((char*)oData, iCount);
DEBUG_PRINT("dequeue(3) ", temp);}
    return ret;
    }

    // 戻り値はenqueueできた要素数
    std::size_t enqueue(uint8_t const* iData, std::size_t iCount)
    {
{std::string temp((char*)iData, iCount);
        DEBUG_PRINT("enqueue() mCount=", mCount, " iCount=", iCount, " mSize=", mSize, " temp=", temp);}

        if (mCount == mSize)
    return 0;

        // 空きより多くenqueしようとした時は、入る分だけenqueue
        if ((mSize - mCount) < iCount)
    return enqueue(iData, mSize - mCount);

        // バッファの最後をまたぐ時は、まずバッファの最後までenque
        //  (mWrite < mReadの場合、この条件は成立しないので条件省略)
        if ((mSize-mWrite) < iCount)
        {
            // バッファの最後までコピー
            std::size_t aCopy = mSize-mWrite;
            std::copy
            (
                iData,
                iData+aCopy,
                THEOLIZER_INTERNAL_UNCHECKED(mBuffer.get()+mWrite)
            );
            iCount -= aCopy;
            mCount += aCopy;
            iData  += aCopy;
            mWrite = 0;
        }

        std::copy
        (
            iData,
            iData+iCount,
            THEOLIZER_INTERNAL_UNCHECKED(mBuffer.get()+mWrite)
        );
        mCount += iCount;
        mWrite += iCount;
        DEBUG_PRINT("enqueue() return ", iCount);
{std::string temp((char*)mBuffer.get()+mRead, mCount);
        DEBUG_PRINT("enqueue() temp=", temp);}
        return iCount;
    }
};

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

    // バッファ先頭文字を獲得し、消費する
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
DEBUG_PRINT("---------------- IMemoryStreamBuf::write() : ", temp.c_str());

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

#if 1   // デバッグ用ostream
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

    Queue   mQueue;
    void setSize(std::size_t iSize)
    {
        mQueue.resize(iSize);
    }

//      ---<<< ostream用(C++ → C#) >>>---

    int overflow(int c = EOF) override
    {
        DEBUG_PRINT("overflow(...,", c , ") ", this);

        uint8_t  buff = static_cast<uint8_t>(c);
        {
            std::unique_lock<std::mutex> lock(mMutex);
            mConditionVariable.wait
            (
                lock,
                [&]{ return (mDisconnected || (mQueue.enqueue(&buff, 1) != 0)); }
            );
        }
        mConditionVariable.notify_all();
        return 0;
    }

    std::streamsize xsputn(const char* s, std::streamsize n) override
    {
        DEBUG_PRINT("xsputn(...,", n , ") ", this);

        std::streamsize ret=0;
        {
            std::unique_lock<std::mutex> lock(mMutex);
            mConditionVariable.wait
            (
                lock,
                [&]
                {
                    if (mDisconnected)
                return true;
                    ret += mQueue.enqueue(reinterpret_cast<uint8_t const*>(s)+ret, n-ret);
DEBUG_PRINT("mConditionVariable.wait(1) return ", (ret >= n));
                    return (ret >= n);
                }
            );
        }
        mConditionVariable.notify_all();
        return ret;
    }

//      ---<<< C# I/F >>>---

    StreamStatus read(uint8_t* buffer, int count, int* out_cout)
    {
DEBUG_PRINT("---------------- OMemoryStreamBuf::read() : ");

        {
            std::unique_lock<std::mutex> lock(mMutex);
            mConditionVariable.wait
            (
                lock,
                [&]
                {
                    if (mDisconnected)
                return true;
                    *out_cout = static_cast<int>(mQueue.dequeue(buffer, count));
DEBUG_PRINT("mConditionVariable.wait(2) return ", *out_cout);
                    return (*out_cout != 0);
                }
            );
        }

        mConditionVariable.notify_all();
std::string temp((char*)buffer, *out_cout);
DEBUG_PRINT("---------------- OMemoryStreamBuf::read() : ", mDisconnected, " ", temp.c_str());
        return (mDisconnected)?StreamStatus::Disconnected:StreamStatus::NoError;
    }

//      ---<<< C++環境向けAPI >>>---

public:
    OMemoryStreamBuf() :
        mDisconnected(false),
        mQueue(1024)
    {
        setbuf(nullptr, 0);
        DEBUG_PRINT("OMemoryStreamBuf() ", this);
    }
    ~OMemoryStreamBuf()
    {
        DEBUG_PRINT("~OMemoryStreamBuf() ", this);
    }
};

#else
#include <windows.h>
class OMemoryStreamBuf : public std::streambuf
{
    std::string     mInternalBuff;

    std::streampos seekoff
    ( 
        std::streamoff off, 
        std::ios::seek_dir dir, 
        int nMode = std::ios::in | std::ios::out
    )
    {
        return EOF;
    }
    std::streampos seekpos
    ( 
        std::streampos pos, 
        int nMode = std::ios::in | std::ios::out
    )
    {
        return EOF;
    }

#if 0
    int overflow( int nCh = EOF ) override
    {
        char buffer[2];
        buffer[0]=nCh;
        DWORD size;
        WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),buffer,1,&size,NULL);
        return 0;
    }
#endif

    std::streamsize xsputn(const char* s, std::streamsize n) override
    {
#if 1
        DWORD size;
        WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),s,n,&size,NULL);
        return size;
#else
        std::streamsize start=0;
        for (std::streamsize i=0; i < n; ++i)
        {
            if (s[i] == '\n')
            {
                mInternalBuff.append(s+start, n-start);
                OutputDebugStringA(mInternalBuff.c_str());
                start=i+1;
                mInternalBuff.clear();
            }
        }
        mInternalBuff.assign(s+start, n-start);
        return n;
#endif
    }

    int underflow()
    {
        return EOF;
    }

public:
    OMemoryStreamBuf()
    {
        setbuf(nullptr, 0);
        AllocConsole();
    }
    ~OMemoryStreamBuf()
    {
        FreeConsole();
    }


    StreamStatus read(uint8_t* buffer, int count, int* out_cout)
    {
        return StreamStatus::NoError;
    }
    void disconnect()
    {
    }
    void setSize(std::size_t iSize)
    {
    }
};
#endif
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
        int* out_cout
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

#endif  // THEOLIZER_INTERNAL_MEMORY_STREAM_H
