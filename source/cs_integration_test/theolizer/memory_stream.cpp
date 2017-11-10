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

#define DLL_EXPORT
#include "memory_stream.h"

// ***************************************************************************
//      連携処理クラス
// ***************************************************************************

// ***************************************************************************
//      C# → C++
// ***************************************************************************

//----------------------------------------------------------------------------
//      書き込み
//----------------------------------------------------------------------------

theolizer::StreamStatus CppWrite
(
    theolizer::IMemoryStream* iIMemoryStream,
    uint8_t* buffer,
    int offset,
    int count
)
{
    std::string temp((char*)buffer+offset, count);
    DEBUG_PRINT("---------------- CppWrite() : ", temp.c_str());

    return iIMemoryStream->write(buffer+offset, count);
}

//----------------------------------------------------------------------------
//      フラッシュ
//----------------------------------------------------------------------------

theolizer::StreamStatus CppFlush(theolizer::IMemoryStream* iIMemoryStream)
{
    DEBUG_PRINT("---------------- CppFlush()");

    return theolizer::StreamStatus::NoError;
}

//----------------------------------------------------------------------------
//      読み出し
//----------------------------------------------------------------------------

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

#if 0
    int num=0;
    *buffer++ = 'A';    ++num;
    *buffer++ = 'n';    ++num;
    *buffer++ = 's';    ++num;
    *buffer++ = 'w';    ++num;
    *buffer++ = 'e';    ++num;
    *buffer++ = 'r';    ++num;
    *buffer++ = '\n';   ++num;
    *out_count=num;
    return theolizer::StreamStatus::NoError;
#else
    return iOMemoryStream->read(buffer+offset, count, out_count);
#endif
}
