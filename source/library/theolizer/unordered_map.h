//############################################################################
/*!
    @brief      std::unordered_map<>のシリアライズ
    @ingroup    SerializationStl
    @file       unordered_map.h
    @author     Yoshinori Tahara(Theoride Technology)
    @date       2017/02/07 Created
*/
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

#if !defined(THEOLIZER_INTERNAL_UNORDERED_MAP_H)
#define THEOLIZER_INTERNAL_UNORDERED_MAP_H

#ifndef THEOLIZER_INTERNAL_DOXYGEN

//############################################################################
//      Begin
//############################################################################

#include <unordered_map>
#include "serializer.h"
#include "internal/containers.h"

THEOLIZER_PROVIDED_BY("Theoride Technology");

//############################################################################
//      std::unordered_map<>対応
//############################################################################

// ***************************************************************************
//      手動コード展開
// ***************************************************************************

#define THEOLZIER_INTERNAL_CONTAINER_PARAMETER          \
    template<class Key, class T, class Hash, class Pred, class Alloc>
#define THEOLZIER_INTERNAL_CONTAINER_NAME               std::unordered_map
#define THEOLZIER_INTERNAL_CONTAINER_ARGUMENT           Key, T, Hash, Pred, Alloc
#define THEOLZIER_INTERNAL_CONTAINER_UNIQUE             unordered_mapTheolizer
#define THEOLIZER_INTERNAL_IS_UNORDERED
#include "internal/container_map.inc"
#undef  THEOLIZER_INTERNAL_IS_UNORDERED

// ***************************************************************************
//      自動生成コード
// ***************************************************************************

#ifdef  THEOLIZER_WRITE_CODE

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE THEOLIZER_INTERNAL_UNPAREN(std::unordered_map<Key, T, Hash, Pred, Alloc>)
#define THEOLIZER_GENERATED_PARAMETER_LIST template<class Key, class T, class Hash, class Pred, class Alloc>
#define THEOLIZER_GENERATED_UNIQUE_NAME unordered_mapTheolizer

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8"std::unordered_map",Key,T,Hash,Pred,Alloc))
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
//      被ポインタ用(theolizer::MapPointee<>)
//############################################################################

// ***************************************************************************
//      theolizer::UnorderedMapPointee<>
//          std::unordered_map<>の単純な派生クラス
//          要素をPointeeとして処理する
// ***************************************************************************

namespace theolizer
{

template
<
    class Key,
    class T,
    class Hash=std::hash<Key>,
    class Pred=std::equal_to<Key>,
    class Alloc=std::allocator<std::pair<const Key,T> >
>
class THEOLIZER_ANNOTATE(CS) UnorderedMapPointee : 
    public std::unordered_map<Key, T, Hash, Pred, Alloc>
{
public:
    using std::unordered_map<Key, T, Hash, Pred, Alloc>::unordered_map;
};

}   // namespace theolizer

// ***************************************************************************
//      手動コード展開
// ***************************************************************************

#define THEOLZIER_INTERNAL_CONTAINER_PARAMETER          \
    template<class Key, class T, class Hash, class Pred, class Alloc>
#define THEOLZIER_INTERNAL_CONTAINER_NAME               theolizer::UnorderedMapPointee
#define THEOLZIER_INTERNAL_CONTAINER_ARGUMENT           Key, T, Hash, Pred, Alloc
#define THEOLZIER_INTERNAL_CONTAINER_UNIQUE             UnorderedMapPointeeTheolizer
#define THEOLIZER_INTERNAL_IS_UNORDERED
#define THEOLIZER_INTERNAL_POINTEE
#include "internal/container_map.inc"
#undef  THEOLIZER_INTERNAL_POINTEE
#undef  THEOLIZER_INTERNAL_IS_UNORDERED

// ***************************************************************************
//      自動生成コード
// ***************************************************************************

#ifdef  THEOLIZER_WRITE_CODE

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE THEOLIZER_INTERNAL_UNPAREN(theolizer::UnorderedMapPointee<Key, T, Hash, Pred, Alloc>)
#define THEOLIZER_GENERATED_PARAMETER_LIST template<class Key, class T, class Hash, class Pred, class Alloc>
#define THEOLIZER_GENERATED_UNIQUE_NAME UnorderedMapPointeeTheolizer

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8"theolizer::UnorderedMapPointee",Key,T,Hash,Pred,Alloc))
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
//      std::unordered_multimap<>対応
//############################################################################

// ***************************************************************************
//      手動コード展開
// ***************************************************************************

#define THEOLZIER_INTERNAL_CONTAINER_PARAMETER          \
    template<class Key, class T, class Hash, class Pred, class Alloc>
#define THEOLZIER_INTERNAL_CONTAINER_NAME               std::unordered_multimap
#define THEOLZIER_INTERNAL_CONTAINER_ARGUMENT           Key, T, Hash, Pred, Alloc
#define THEOLZIER_INTERNAL_CONTAINER_UNIQUE             unordered_multimapTheolizer
#define THEOLIZER_INTERNAL_IS_UNORDERED
#define THEOLIZER_INTERNAL_IS_MULTI
#include "internal/container_map.inc"
#undef  THEOLIZER_INTERNAL_IS_MULTI
#undef  THEOLIZER_INTERNAL_IS_UNORDERED

// ***************************************************************************
//      自動生成コード
// ***************************************************************************

#ifdef  THEOLIZER_WRITE_CODE

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE THEOLIZER_INTERNAL_UNPAREN(std::unordered_multimap<Key, T, Hash, Pred, Alloc>)
#define THEOLIZER_GENERATED_PARAMETER_LIST template<class Key, class T, class Hash, class Pred, class Alloc>
#define THEOLIZER_GENERATED_UNIQUE_NAME unordered_multimapTheolizer

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8"std::unordered_multimap",Key,T,Hash,Pred,Alloc))
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

#endif  // THEOLIZER_INTERNAL_UNORDERED_MAP_H
