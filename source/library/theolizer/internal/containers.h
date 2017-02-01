//############################################################################
//      
/*!
@brief      Theolizerライブラリの標準コンテナ・サポート
@ingroup    TheolizerLib
@file       containers.h
@author     Yoshinori Tahara(Theoride Technology)
@date       2017/02/01 Created
*/
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

#if !defined(THEOLIZER_INTERNAL_CONTAINERS_H)
#define THEOLIZER_INTERNAL_CONTAINERS_H


//############################################################################
//      Begin
//############################################################################

// ***************************************************************************
//          DLL用の警告禁止
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(push)
//#pragma warning(disable:4100 4251)
#endif

//############################################################################
//      グローバル名前空間
//############################################################################

// ***************************************************************************
//      標準コンテナのデフォルト・パラメータ
// ***************************************************************************

//----------------------------------------------------------------------------
//      allocator<T>
//----------------------------------------------------------------------------

THEOLIZER_TEMPLATE_PARAMETER_TEMPLATE((template<class T>),
                                      std::allocator, (T),
                                      allocatorTheolizer);

#if 0

// ***************************************************************************
//      自動生成コードの雛形
// ***************************************************************************

//----------------------------------------------------------------------------
//      通常用
//----------------------------------------------------------------------------

#ifdef  THEOLIZER_WRITE_CODE

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE      THEOLIZER_INTERNAL_FULL_NAME
#define THEOLIZER_GENERATED_PARAMETER_LIST  THEOLZIER_INTERNAL_CONTAINER_PARAMETER
#define THEOLIZER_GENERATED_UNIQUE_NAME     THEOLZIER_INTERNAL_CONTAINER_UNIQUE

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO  THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()    THEOLIZER_INTERNAL_TEMPLATE_NAME(\
    (u8"" THEOLIZER_INTERNAL_STRINGIZE(THEOLZIER_INTERNAL_CONTAINER_NAME),  \
    THEOLZIER_INTERNAL_CONTAINER_ARGUMENT))
#include <theolizer/internal/version_manual.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

#endif//THEOLIZER_WRITE_CODE

//----------------------------------------------------------------------------
//      被ポインタ用
//----------------------------------------------------------------------------

#ifdef  THEOLIZER_WRITE_CODE

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE theolizer::VectorPointee<T, Alloc>
#define THEOLIZER_GENERATED_PARAMETER_LIST template<class T, class Alloc>
#define THEOLIZER_GENERATED_UNIQUE_NAME VectorPointeeTheolizer

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8"theolizer::VectorPointee",T,Alloc))
#include <theolizer/internal/version_manual.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

#endif//THEOLIZER_WRITE_CODE

#endif

//############################################################################
//      End
//############################################################################

// ***************************************************************************
//          警告抑止解除
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#endif  // THEOLIZER_INTERNAL_CONTAINERS_H
