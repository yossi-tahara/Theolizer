//############################################################################
//      Theolizer全体のベース部的な部分へ移動予定
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

#if !defined(THEOLIZER_INTERNAL_BASE_H)
#define THEOLIZER_INTERNAL_BASE_H

//############################################################################
//      Begin
//############################################################################

namespace theolizer
{

// ***************************************************************************
//          API定義用マクロ
// ***************************************************************************

#ifdef DLL_EXPORT
    #if defined(_MSC_VER)
        #define THEOLIZER_INTERNAL_DLL __declspec(dllexport)
    #elif defined(_WIN32)
        #define THEOLIZER_INTERNAL_DLL __declspec(dllexport)
    #else
        #define THEOLIZER_INTERNAL_DLL __attribute__((visibility ("default")))
    #endif
#else
    #if defined(THEOLIZER_DYN_LINK)
        #if defined(_MSC_VER)
            #define THEOLIZER_INTERNAL_DLL __declspec(dllimport)
        #else
            #define THEOLIZER_INTERNAL_DLL
        #endif
    #else
        #define THEOLIZER_INTERNAL_DLL
    #endif
#endif

// ***************************************************************************
//      簡易ロガー(OutputDebugStringA利用)
// ***************************************************************************

#if defined(_DEBUG) && defined(_WIN32)

#include <windows.h>

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

#define DEBUG_PRINT(...)    theolizer::print(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

//############################################################################
//      End
//############################################################################

}   // namespace theolizer

#endif  // THEOLIZER_INTERNAL_BASE_H
