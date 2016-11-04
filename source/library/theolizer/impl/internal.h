//############################################################################
//      Theolizer内部用のヘッダ
/*
    Copyright (c) 2016 Yohinori Tahara(Theoride Technology) - http://theolizer.com/

    商用ライセンス
        有効なTheolizer商用ライセンスを保持している人は、
        田原良則(Theoride Technology)と締結している商用ライセンス契約の
        使用条件に従って、このファイルを取り扱うことができます。

    General Public License Version 3(以下GPLv3)
        Free Software Foundationが公表するGPLv3の使用条件に従って、
        あなたはこのファイルを取り扱うことができます。
        GPLv3の内容を https://www.gnu.org/licenses/gpl.txt にて確認して下さい。
        GPLv3のコピーをLICENSE.TXTファイルにおいてます。
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

#endif  // THEOLIZER_INTERNAL_INTERNAL_H
