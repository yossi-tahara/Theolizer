//############################################################################
//      std::array<>のシリアライズ
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

#if !defined(THEOLIZER_INTERNAL_ARRAY_H)
#define THEOLIZER_INTERNAL_ARRAY_H

#ifndef THEOLIZER_INTERNAL_DOXYGEN

//############################################################################
//      Begin
//############################################################################

#include <array>
#include "serializer.h"
#include "internal/containers.h"

THEOLIZER_PROVIDED_BY("Theoride Technology");

//############################################################################
//      std::array<>対応
//############################################################################

// ***************************************************************************
//      theolizer::ArrayPointee<>
//          std::array<>の単純な派生クラス
//          要素をPointeeとして処理する
// ***************************************************************************

namespace theolizer
{

template<class T, std::size_t N>
class THEOLIZER_ANNOTATE(CS) ArrayPointee : public std::array<T, N>
{
public:
    using std::array<T, N>::array;
};

}   // namespace theolizer

// ***************************************************************************
//      手動コード展開
// ***************************************************************************

#define THEOLZIER_INTERNAL_CONTAINER_PARAMETER          template<class T, std::size_t N>
#define THEOLZIER_INTERNAL_CONTAINER_NAME               std::array
#define THEOLZIER_INTERNAL_CONTAINER_NAME_POINTEE       theolizer::ArrayPointee
#define THEOLZIER_INTERNAL_CONTAINER_ARGUMENT           T, N
#define THEOLZIER_INTERNAL_CONTAINER_UNIQUE             arrayTheolizer
#define THEOLZIER_INTERNAL_CONTAINER_UNIQUE_POINTEE     ArrayPointeeTheolizer
#include "internal/container_fixed.inc"

// ***************************************************************************
//      自動生成コードの雛形
// ***************************************************************************

//----------------------------------------------------------------------------
//      通常用
//----------------------------------------------------------------------------

#ifdef  THEOLIZER_WRITE_CODE

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE std::array<T, N>
#define THEOLIZER_GENERATED_PARAMETER_LIST template<class T, std::size_t N>
#define THEOLIZER_GENERATED_UNIQUE_NAME arrayTheolizer

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8"std::array",T,\
        theolizer::internal::NonType<std::size_t,N>))
#include <theolizer/internal/version_manual.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

#endif//THEOLIZER_WRITE_CODE

//----------------------------------------------------------------------------
//      被ポインタ用
//----------------------------------------------------------------------------

#ifdef  THEOLIZER_WRITE_CODE

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE theolizer::ArrayPointee<T, N>
#define THEOLIZER_GENERATED_PARAMETER_LIST template<class T, std::size_t N>
#define THEOLIZER_GENERATED_UNIQUE_NAME ArrayPointeeTheolizer

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8"theolizer::ArrayPointee",T,\
        theolizer::internal::NonType<std::size_t,N>))
#include <theolizer/internal/version_manual.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

#endif//THEOLIZER_WRITE_CODE

// ***************************************************************************
//      定義したマクロの解放
// ***************************************************************************

#undef  THEOLZIER_INTERNAL_CONTAINER_PARAMETER
#undef  THEOLZIER_INTERNAL_CONTAINER_NAME
#undef  THEOLZIER_INTERNAL_CONTAINER_NAME_POINTEE
#undef  THEOLZIER_INTERNAL_CONTAINER_ARGUMENT
#undef  THEOLZIER_INTERNAL_CONTAINER_UNIQUE
#undef  THEOLZIER_INTERNAL_CONTAINER_UNIQUE_POINTEE

#undef  THEOLIZER_INTERNAL_FULL_NAME
#undef  THEOLIZER_INTERNAL_FULL_NAME_POINTEE

//############################################################################
//      End
//############################################################################

#endif  // THEOLIZER_INTERNAL_DOXYGEN

#endif  // THEOLIZER_INTERNAL_ARRAY_H
