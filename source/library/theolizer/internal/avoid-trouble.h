//############################################################################
//      コンパイラや外部ライブラリのトラブル回避措置
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

#if !defined(THEOLIZER_INTERNAL_AVOID_TROUBLE_H)
#define THEOLIZER_INTERNAL_AVOID_TROUBLE_H

// ***************************************************************************
//      MSVCのセキュリティ警告を止める
// ***************************************************************************

#if defined(_MSC_VER)   // disabling MSVC warnings
    #ifndef _SCL_SECURE_NO_WARNINGS
        #define _SCL_SECURE_NO_WARNINGS
    #endif
#endif

// ***************************************************************************
//      MinGW 5.2.0の不具合と思われるもの
// ***************************************************************************

#if defined(__MINGW32__)
  #if (__GNUC__ == 5) && (__GNUC_MINOR__ >= 2)

//----------------------------------------------------------------------------
//      localtime_r()とglobaltime_r()がundefinedになる問題対策
//----------------------------------------------------------------------------

      #define _POSIX_C_SOURCE 200809L

  #endif
#endif

// ***************************************************************************
//      MinGW/gccとboostの組み合わせで発生する問題
// ***************************************************************************

#define THEOLIZER_INTERNAL_PRAGMA(x)    _Pragma(#x)

//----------------------------------------------------------------------------
//      boostの一部(format.hpp等)をインクルードすると
//      boost、および、標準ライブラリのヘッダでauto_ptr警告が表示される
//      boostをインクルードする前に定義する
//----------------------------------------------------------------------------

#if defined(__GNUC__)
    #define THEOLIZER_INTERNAL_DISABLE_WARNING()                            \
        THEOLIZER_INTERNAL_PRAGMA(GCC diagnostic ignored "-Wdeprecated-declarations")\
        THEOLIZER_INTERNAL_PRAGMA(GCC diagnostic ignored "-Wunused-variable")\
        THEOLIZER_INTERNAL_PRAGMA(GCC diagnostic ignored "-Wunused-local-typedefs")
    #define THEOLIZER_INTERNAL_DISABLE_WARNING_SIGN_COMPARE()               \
        THEOLIZER_INTERNAL_PRAGMA(GCC diagnostic ignored "-Wsign-compare")
#else
    #define THEOLIZER_INTERNAL_DISABLE_WARNING()
    #define THEOLIZER_INTERNAL_DISABLE_WARNING_SIGN_COMPARE()
#endif

#endif  // THEOLIZER_INTERNAL_AVOID_TROUBLE_H
