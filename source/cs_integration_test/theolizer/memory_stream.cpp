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

#define THEOLIZER_NO_ANALYZE

#include "memory_stream.h"

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(disable:4100)
#endif

//############################################################################
//      Export関数
//############################################################################

// ***************************************************************************
//      書き込み
// ***************************************************************************

theolizer::StreamStatus CppWrite
(
    theolizer::IMemoryStream* iIMemoryStream,
    uint8_t* buffer,
    int offset,
    int count
)
{
    return iIMemoryStream->write(buffer+offset, count);
}

// ***************************************************************************
//      フラッシュ
// ***************************************************************************

theolizer::StreamStatus CppFlush
(
    theolizer::IMemoryStream* iIMemoryStream
)
{
    DEBUG_PRINT("---------------- CppFlush()");

    return theolizer::StreamStatus::NoError;
}

// ***************************************************************************
//      読み出し
// ***************************************************************************

theolizer::StreamStatus CppRead
(
    theolizer::OMemoryStream* iOMemoryStream,
    uint8_t* buffer,
    int offset,
    int count,
    int* out_count
)
{
    DEBUG_PRINT("---------------- CppRead() : ", count);

    return iOMemoryStream->read(buffer+offset, count, out_count);
}

//############################################################################
//      C# → C++用メモリ・ストリーム
//############################################################################

namespace theolizer
{

// ***************************************************************************
//      IMemoryStream用ストリーム・バッファ
// ***************************************************************************

//----------------------------------------------------------------------------
//      コンストラクタ／デストラクタ
//----------------------------------------------------------------------------

IMemoryStreamBuf::IMemoryStreamBuf() :
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

IMemoryStreamBuf::~IMemoryStreamBuf()
{
    DEBUG_PRINT("~IMemoryStreamBuf() ", this);
}

//----------------------------------------------------------------------------
//      切断
//----------------------------------------------------------------------------

void IMemoryStreamBuf::disconnect()
{
    std::lock_guard<std::mutex> lock(mMutex);
    mDisconnected = true;
    mConditionVariable.notify_all();
}

//----------------------------------------------------------------------------
//      istream用(C# → C++)
//----------------------------------------------------------------------------

//      ---<<< バッファ先頭文字獲得 >>>--

int IMemoryStreamBuf::underflow()
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

//      ---<<< バッファ先頭文字を獲得し、消費する >>>--

int IMemoryStreamBuf::uflow()
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

//      ---<<< 1文字戻す >>>--

int IMemoryStreamBuf::pbackfail(int c)
{
    DEBUG_PRINT("pbackfail(", c ,") EOF=", EOF, " ", this);

    // 1回のみ戻せるものとする
    if (mNextChar != EOF)
return EOF;

    mNextChar = mLastChar;
    return mNextChar;
}

//----------------------------------------------------------------------------
//      C#から受け取り
//----------------------------------------------------------------------------

uint8_t*    mTransBuff;
int         mCount;
StreamStatus IMemoryStreamBuf::write(uint8_t* buffer, int count)
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

// ***************************************************************************
//      IMemoryStream
// ***************************************************************************

//----------------------------------------------------------------------------
//      コンストラクタ／デストラクタ
//----------------------------------------------------------------------------

IMemoryStream::IMemoryStream() : std::istream(&mIMemoryStreamBuf)
{
}

IMemoryStream::~IMemoryStream()
{
}

//----------------------------------------------------------------------------
//      切断要求中継
//----------------------------------------------------------------------------

void IMemoryStream::disconnect()
{
    mIMemoryStreamBuf.disconnect();
}

//----------------------------------------------------------------------------
//      C# I/F中継
//----------------------------------------------------------------------------

StreamStatus IMemoryStream::write(uint8_t* buffer, int count)
{
    return mIMemoryStreamBuf.write(buffer, count);
}

//############################################################################
//      C++ → C#用メモリ・ストリーム
//############################################################################

// ***************************************************************************
//      OMemoryStream用ストリーム・バッファ
// ***************************************************************************

//----------------------------------------------------------------------------
//      コンストラクタ／デストラクタ
//----------------------------------------------------------------------------

OMemoryStreamBuf::OMemoryStreamBuf() :
    mDisconnected(false),
    mSize(1024),
    mIsFull(false),
    mBuffer(new uint8_t[mSize]),
    mSynchronized(false)
{
    setbuf(nullptr, 0);
    DEBUG_PRINT("OMemoryStreamBuf() ", this);
}

OMemoryStreamBuf::~OMemoryStreamBuf()
{
    DEBUG_PRINT("~OMemoryStreamBuf() ", this);
}

//----------------------------------------------------------------------------
//      切断要求
//----------------------------------------------------------------------------

void OMemoryStreamBuf::disconnect()
{
    std::lock_guard<std::mutex> lock(mMutex);
    mDisconnected = true;
    mConditionVariable.notify_all();
}

//----------------------------------------------------------------------------
//      Queue
//----------------------------------------------------------------------------

void OMemoryStreamBuf::setSize(std::size_t iSize)
{
    std::lock_guard<std::mutex> lock(mMutex);

    if (mWriteMan.mCount+mReadMan.mCount)
throw std::runtime_error("can not resize theolizer::OMemoryStreamBuf while Count != 0.");

    mSize = static_cast<std::streamsize>(iSize);
    mWriteMan.mRead = mWriteMan.mWrite = 0;
    mReadMan.mRead  = mReadMan.mWrite  = 0;
    mBuffer.reset(new uint8_t[mSize]);
}

//      ---<<< フラッシュ >>>---

void OMemoryStreamBuf::flush()
{
    mWriteMan.mRead = mReadMan.mRead;
    mReadMan.mWrite = mWriteMan.mWrite;
    mReadMan.mCount += mWriteMan.mCount;
    mWriteMan.mCount = 0;
}

//      ---<<< Queue空き待ち >>>---

int OMemoryStreamBuf::waitSpace()
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

//----------------------------------------------------------------------------
//      ostream用(C++ → C#)
//----------------------------------------------------------------------------

//      ---<<< フラッシュ実体 >>>---

int OMemoryStreamBuf::sync()
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

//      ---<<< 一文字出力 >>>---

int OMemoryStreamBuf::overflow(int c)
{
    DEBUG_PRINT("overflow(..., [", static_cast<char>(c) , "]) ", this);

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

//      ---<<< 複数文字出力 >>>---

std::streamsize OMemoryStreamBuf::xsputn(char const* buffer, std::streamsize count)
{
    DEBUG_PRINT("xsputn(...,", count, ") [", std::string(buffer, count), "] ", this);

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

        // フル、もしくは、十分に溜まっていたらflushする
        if (mIsFull || (128 <= mWriteMan.mCount))
        {
            DEBUG_PRINT("call sync(2) mIsFull=", mIsFull);
            if (sync() == EOF)
return EOF;
        }
    }

    return ret;
}

//----------------------------------------------------------------------------
//      C# I/F
//----------------------------------------------------------------------------

StreamStatus OMemoryStreamBuf::read(uint8_t* buffer, int count, int* out_count)
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

// ***************************************************************************
//      OMemoryStream
// ***************************************************************************

//----------------------------------------------------------------------------
//      コンストラクタ／デストラクタ
//----------------------------------------------------------------------------

OMemoryStream::OMemoryStream() : std::ostream(&mOMemoryStreamBuf)
{
}

OMemoryStream::~OMemoryStream()
{
}

//----------------------------------------------------------------------------
//      切断要求中継
//----------------------------------------------------------------------------

void OMemoryStream::disconnect()
{
    mOMemoryStreamBuf.disconnect();
}

//----------------------------------------------------------------------------
//      FIFOサイズ設定
//----------------------------------------------------------------------------

void OMemoryStream::setSize(std::size_t iSize)
{
    mOMemoryStreamBuf.setSize(iSize);
}

//----------------------------------------------------------------------------
//      C# I/F中継
//----------------------------------------------------------------------------

StreamStatus OMemoryStream::read(uint8_t* buffer, int count, int* out_count)
{
    return mOMemoryStreamBuf.read(buffer, count, out_count);
}

}   // namespace theolizer
