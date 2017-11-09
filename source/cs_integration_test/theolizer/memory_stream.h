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
#include <sstream>

#include <theolizer/temp.h>


//############################################################################
//      C# I/F
//############################################################################

namespace theolizer
{
    class IMemoryStream;
    class OMemoryStream;
}

extern "C"
{
    THEOLIZER_INTERNAL_DLL  void CppWrite
    (
        theolizer::IMemoryStream* iIMemoryStream,
        uint8_t* buffer,
        int offset,
        int count
    );
    THEOLIZER_INTERNAL_DLL  void CppFlush(theolizer::IMemoryStream* iIMemoryStream);
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
        return n;
    }

    int underflow()
    {
        return EOF;
    }

public:
    MemoryStreamBuf()
    {
        setbuf(nullptr, 0);
    }
    ~MemoryStreamBuf()
    {
    }
};

// ***************************************************************************
//      C# → C++用メモリ・ストリーム
// ***************************************************************************

class IMemoryStream : public std::istream
{
public:
    IMemoryStream() : std::istream(new MemoryStreamBuf())
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
public:
    OMemoryStream() : std::ostream(new MemoryStreamBuf())
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
