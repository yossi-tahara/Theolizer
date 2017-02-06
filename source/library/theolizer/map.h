//############################################################################
//      std::map<>のシリアライズ
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

#if !defined(THEOLIZER_INTERNAL_MAP_H)
#define THEOLIZER_INTERNAL_MAP_H

#ifndef THEOLIZER_INTERNAL_DOXYGEN

//############################################################################
//      Begin
//############################################################################

#include <map>
#include "serializer.h"
#include "internal/containers.h"

THEOLIZER_PROVIDED_BY("Theoride Technology");

//############################################################################
//      std::map<>対応
//############################################################################

// ***************************************************************************
//      手動コード展開
// ***************************************************************************

#define THEOLZIER_INTERNAL_CONTAINER_PARAMETER          \
    template<class Key, class T, class Compare, class Alloc>
#define THEOLZIER_INTERNAL_CONTAINER_NAME               std::map
#define THEOLZIER_INTERNAL_CONTAINER_ARGUMENT           Key, T, Compare, Alloc
#define THEOLZIER_INTERNAL_CONTAINER_UNIQUE             mapTheolizer
#define THEOLIZER_INTERNAL_IS_MAP
#include "internal/container_key.inc"
#undef  THEOLIZER_INTERNAL_IS_MAP

// ***************************************************************************
//      自動生成コードの雛形
// ***************************************************************************

//----------------------------------------------------------------------------
//      通常用
//----------------------------------------------------------------------------

#ifdef  THEOLIZER_WRITE_CODE

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE std::map<Key, T, Compare, Alloc>
#define THEOLIZER_GENERATED_PARAMETER_LIST template<class Key, class T, class Compare, class Alloc>
#define THEOLIZER_GENERATED_UNIQUE_NAME mapTheolizer

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8"std::map",Key,T,Compare,Alloc))
#include <theolizer/internal/version_manual.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

#endif//THEOLIZER_WRITE_CODE

// ***************************************************************************
//      定義したマクロの解放
// ***************************************************************************

#undef  THEOLZIER_INTERNAL_CONTAINER_PARAMETER
#undef  THEOLZIER_INTERNAL_CONTAINER_NAME
#undef  THEOLZIER_INTERNAL_CONTAINER_ARGUMENT
#undef  THEOLZIER_INTERNAL_CONTAINER_UNIQUE

#undef  THEOLIZER_INTERNAL_FULL_NAME

//############################################################################
//      std::multimap<>対応
//############################################################################

// ***************************************************************************
//      手動コード展開
// ***************************************************************************

#define THEOLZIER_INTERNAL_CONTAINER_PARAMETER          \
    template<class Key, class T, class Compare, class Alloc>
#define THEOLZIER_INTERNAL_CONTAINER_NAME               std::multimap
#define THEOLZIER_INTERNAL_CONTAINER_ARGUMENT           Key, T, Compare, Alloc
#define THEOLZIER_INTERNAL_CONTAINER_UNIQUE             multimapTheolizer
#define THEOLIZER_INTERNAL_IS_MAP
#define THEOLIZER_INTERNAL_IS_MULTI
#include "internal/container_key.inc"
#undef  THEOLIZER_INTERNAL_IS_MAP
#undef  THEOLIZER_INTERNAL_IS_MULTI

// ***************************************************************************
//      自動生成コードの雛形
// ***************************************************************************

//----------------------------------------------------------------------------
//      通常用
//----------------------------------------------------------------------------

#ifdef  THEOLIZER_WRITE_CODE

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE std::multimap<Key, T, Compare, Alloc>
#define THEOLIZER_GENERATED_PARAMETER_LIST template<class Key, class T, class Compare, class Alloc>
#define THEOLIZER_GENERATED_UNIQUE_NAME multimapTheolizer

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8"std::multimap",Key,T,Compare,Alloc))
#include <theolizer/internal/version_manual.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

#endif//THEOLIZER_WRITE_CODE

// ***************************************************************************
//      定義したマクロの解放
// ***************************************************************************

#undef  THEOLZIER_INTERNAL_CONTAINER_PARAMETER
#undef  THEOLZIER_INTERNAL_CONTAINER_NAME
#undef  THEOLZIER_INTERNAL_CONTAINER_ARGUMENT
#undef  THEOLZIER_INTERNAL_CONTAINER_UNIQUE

#undef  THEOLIZER_INTERNAL_FULL_NAME

//############################################################################
//      End
//############################################################################

#endif  // THEOLIZER_INTERNAL_DOXYGEN

#endif  // THEOLIZER_INTERNAL_MAP_H
