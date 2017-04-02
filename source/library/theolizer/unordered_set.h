//############################################################################
//      std::unordered_set<>のシリアライズ
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

#if !defined(THEOLIZER_INTERNAL_UNORDERED_SET_H)
#define THEOLIZER_INTERNAL_UNORDERED_SET_H

#ifndef THEOLIZER_INTERNAL_DOXYGEN

//############################################################################
//      Begin
//############################################################################

#include <unordered_set>
#include "serializer.h"
#include "internal/containers.h"

THEOLIZER_PROVIDED_BY("Theoride Technology");

//############################################################################
//      std::unordered_set<>対応
//############################################################################

// ***************************************************************************
//      手動コード展開
// ***************************************************************************

#define THEOLZIER_INTERNAL_CONTAINER_PARAMETER          \
    template<class Key, class Hash, class Pred, class Alloc>
#define THEOLZIER_INTERNAL_CONTAINER_NAME               std::unordered_set
#define THEOLZIER_INTERNAL_CONTAINER_ARGUMENT           Key, Hash, Pred, Alloc
#define THEOLZIER_INTERNAL_CONTAINER_UNIQUE             unordered_setTheolizer
#define THEOLIZER_INTERNAL_IS_UNORDERED
#include "internal/container_set.inc"
#undef  THEOLIZER_INTERNAL_IS_UNORDERED

// ***************************************************************************
//      自動生成コード
// ***************************************************************************

#ifdef  THEOLIZER_WRITE_CODE

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE THEOLIZER_INTERNAL_UNPAREN(std::unordered_set<Key, Hash, Pred, Alloc>)
#define THEOLIZER_GENERATED_PARAMETER_LIST template<class Key, class Hash, class Pred, class Alloc>
#define THEOLIZER_GENERATED_UNIQUE_NAME unordered_setTheolizer

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8"std::unordered_set",Key,Hash,Pred,Alloc))
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
//      std::unordered_multiset<>対応
//############################################################################

// ***************************************************************************
//      手動コード展開
// ***************************************************************************

#define THEOLZIER_INTERNAL_CONTAINER_PARAMETER          \
    template<class Key, class Hash, class Pred, class Alloc>
#define THEOLZIER_INTERNAL_CONTAINER_NAME               std::unordered_multiset
#define THEOLZIER_INTERNAL_CONTAINER_ARGUMENT           Key, Hash, Pred, Alloc
#define THEOLZIER_INTERNAL_CONTAINER_UNIQUE             unordered_multisetTheolizer
#define THEOLIZER_INTERNAL_IS_UNORDERED
#define THEOLIZER_INTERNAL_IS_MULTI
#include "internal/container_set.inc"
#undef  THEOLIZER_INTERNAL_IS_MULTI
#undef  THEOLIZER_INTERNAL_IS_UNORDERED

// ***************************************************************************
//      自動生成コードの雛形
// ***************************************************************************

//----------------------------------------------------------------------------
//      通常用
//----------------------------------------------------------------------------

#ifdef  THEOLIZER_WRITE_CODE

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE THEOLIZER_INTERNAL_UNPAREN(std::unordered_multiset<Key, Hash, Pred, Alloc>)
#define THEOLIZER_GENERATED_PARAMETER_LIST template<class Key, class Hash, class Pred, class Alloc>
#define THEOLIZER_GENERATED_UNIQUE_NAME unordered_multisetTheolizer

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8"std::unordered_multiset",Key,Hash,Pred,Alloc))
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

#endif  // THEOLIZER_INTERNAL_UNORDERED_SET_H
