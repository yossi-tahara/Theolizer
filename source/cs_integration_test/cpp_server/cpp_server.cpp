//############################################################################
//      Theolizer Test Project for C# Integration
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

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#if defined(_MSC_VER)
//  #pragma warning(disable:4100)
#endif

// ***************************************************************************
//      インクルード
// ***************************************************************************

// 標準ライブラリ
#include <iostream>
#include <memory>
#include <thread>
#include <string>
#include <sstream>

// Windows
#include <windows.h>

// 固有ヘッダ
#define DLL_EXPORT
#include "cpp_server.h"

// ***************************************************************************
//      簡易ロガー(OutputDebugStringA利用)
// ***************************************************************************

#ifdef _DEBUG

namespace
{

inline void printImpl(std::stringstream& ioString)
{
}

template<typename tFirst, typename... tParams>
void printImpl(std::stringstream& ioString, tFirst iFirst, tParams... iParams)
{
    ioString << iFirst;
    printImpl(ioString, iParams...);
}

template<typename... tParams>
void print(tParams... iParams)
{
    std::stringstream ss;
    printImpl(ss, iParams...);
    std::string str(ss.str());
    if (str.back() != '\n') str.push_back('\n');
    OutputDebugStringA(str.c_str());
}

}   // namespace

#define DEBUG_PRINT(...)    print(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

#if 0   // デバッグ用ostream
class odstreambuf : public std::streambuf
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
    odstreambuf()
    {
        setbuf(nullptr, 0);
        AllocConsole();
    }
    ~odstreambuf()
    {
        FreeConsole();
    }
};

class odstream : public std::ostream
{
public:
    odstream() : std::ostream(new odstreambuf())
    {
    }

    ~odstream(void)
    {
    }
};
odstream ods;
#endif

// ***************************************************************************
//      メイン・スレッド
// ***************************************************************************

//----------------------------------------------------------------------------
//      メイン・スレッド管理（シングルトン）
//----------------------------------------------------------------------------

class MainThread
{
    std::thread*    mMainThread;

    // 生成／コピー／ムーブ不可
    MainThread() : mMainThread(nullptr) { }
    MainThread(MainThread const&) = delete;
    MainThread(MainThread     &&) = delete;
    MainThread& operator=(MainThread const&) = delete;
    MainThread& operator=(MainThread     &&) = delete;
public:
    static MainThread& get()
    {
        static MainThread instance;
        return instance;
    }
    ~MainThread()
    {
        mMainThread->join();
    }

    template <class F, class ...Args>
    void startThread(F&& f, Args&&... args)
    {
        mMainThread = new std::thread(f, args...);
    }
};

//----------------------------------------------------------------------------
//      スレッド起動処理(main()呼び出し)
//----------------------------------------------------------------------------

extern "C" int main();

void CppInitialize()
{
    MainThread::get().startThread(main);
}

// ***************************************************************************
//      連携処理クラス
// ***************************************************************************

// ***************************************************************************
//      C# → C++
// ***************************************************************************

//----------------------------------------------------------------------------
//      書き込み
//----------------------------------------------------------------------------

void CppWrite(uint8_t* buffer, int offset, int count)
{
    std::string temp((char*)buffer+offset, count);
    DEBUG_PRINT("---------------- CppWrite() : ", temp.c_str());
}

//----------------------------------------------------------------------------
//      フラッシュ
//----------------------------------------------------------------------------

void CppFlush()
{
    DEBUG_PRINT("---------------- CppFlush()");
}

// ***************************************************************************
//      main()
// ***************************************************************************

int main()
{
    DEBUG_PRINT("---------------- main()");

    return 0;
}
