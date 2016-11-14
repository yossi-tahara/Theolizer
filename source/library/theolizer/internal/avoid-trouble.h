//############################################################################
//      コンパイラや外部ライブラリのトラブル回避措置
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
//      localtime_r()とlocaltime_r()がundefinedになる問題対策
//----------------------------------------------------------------------------

      #define _POSIX_C_SOURCE 200809L

  #endif
#endif

// ***************************************************************************
//      MinGW/gccとboostの組み合わせで発生する問題
// ***************************************************************************

#define THEOLIZER_INTERNAL_PRAGMA(x)    THEOLIZER_INTERNAL_PRAGMA_I(x)
#define THEOLIZER_INTERNAL_PRAGMA_I(x)  _Pragma(#x)

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
