//############################################################################
//      Theolizer内部用のヘッダ
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

#if !defined(THEOLIZER_INTERNAL_INTERNAL_H)
#define THEOLIZER_INTERNAL_INTERNAL_H

//############################################################################
//      Begin
//############################################################################

namespace theolizer
{

//############################################################################
//      マクロ定義
//############################################################################

//############################################################################
//      dll export関係
//############################################################################

#if defined(THEOLIZER_DYN_LINK)
    #if defined(_MSC_VER)
        #define THEOLIZER_INTERNAL_DLL __declspec(dllexport)
    #elif defined(_WIN32)
        #define THEOLIZER_INTERNAL_DLL __declspec(dllexport)
    #else
        #define THEOLIZER_INTERNAL_DLL __attribute__((visibility ("default")))
    #endif
#else
    #define THEOLIZER_INTERNAL_DLL
#endif

//############################################################################
//      End
//############################################################################

}   // namespace theolizer

// ***************************************************************************
//      基本ツールのインクルード
// ***************************************************************************

#include "../report.h"

// ***************************************************************************
//      BOOST_NO_EXCEPTIONSを定義し、throw_exception()をinline定義する
//          throw_exception()をユーザ側のそれと完全に切り離すため。
// ***************************************************************************

#define BOOST_NO_EXCEPTIONS
#define BOOST_NO_EXCEPTION  // boost 1.59.0対応
namespace boost
{
    static inline void throw_exception(std::exception const& e)
    {
        THEOLIZER_INTERNAL_ABORT(e.what());
    }
}

#endif  // THEOLIZER_INTERNAL_INTERNAL_H
