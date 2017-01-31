//############################################################################
//      std::vector<>のシリアライズ
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

#if !defined(THEOLIZER_INTERNAL_VECTOR_H)
#define THEOLIZER_INTERNAL_VECTOR_H

//############################################################################
//      Begin
//############################################################################

#include <vector>
#include "serializer.h"
#include "internal/containers.h"

THEOLIZER_PROVIDED_BY("Theoride Technology");

//############################################################################
//      std::vector<>対応
//############################################################################

#ifndef THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//      シリアライズ指定
// ***************************************************************************

THEOLIZER_NON_INTRUSIVE_TEMPLATE_ORDER((template<class T, class Alloc>),
                                        (std::vector<T, Alloc>), 1,
                                        vectorTheolizer);

//----------------------------------------------------------------------------
//      ユーザ定義
//          回復処理の注意事項：
//              余分なデータの破棄、および、ClassType終了処理のため、
//              必ずiSerializer.readPreElement()がfalseを返却するまで
//              処理しておくこと。
//----------------------------------------------------------------------------

//      ---<<< Version.1 >>>---

template<class T, class Alloc>
template<class tMidSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<std::vector<T, Alloc>>::
    TheolizerUserDefine<tMidSerializer, tTheolizerVersion, 1>
{
    // 保存
    static void saveClassManual
    (
        tMidSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(Alloc);

        THEOLIZER_PROCESS(iSerializer, iInstance->size());
        for (auto& aElement : *iInstance)
        {
            THEOLIZER_PROCESS(iSerializer, aElement);
        }
    }

    // 回復
    static void loadClassManual
    (
        tMidSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(Alloc);

        // もし、nullptrなら、インスタンス生成
        if (!oInstance)   oInstance=new typename tTheolizerVersion::TheolizerTarget();

        std::size_t aSize;
        THEOLIZER_PROCESS(iSerializer, aSize);

        // 先に領域をvector内部に生成してから、そこへ回復する。
        //  これにより、要素のコピーやムーブが発生しないようにすることで、
        //  「親」へのポインタが壊れないようにしている。
        oInstance->resize(aSize);
        for (auto& aElement : *oInstance)
        {
            THEOLIZER_PROCESS(iSerializer, aElement);
            theolizer::internal::checkDataShort
            (
                !iSerializer.isTerminated(),
                "std::vector<T, Alloc>"
            );
        }
    }
};

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#ifdef  THEOLIZER_WRITE_CODE // ###### std::vector<T, Alloc> ######

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE std::vector<T, Alloc>
#define THEOLIZER_GENERATED_PARAMETER_LIST template<class T, class Alloc>
#define THEOLIZER_GENERATED_UNIQUE_NAME vectorTheolizer

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8"std::vector",T,Alloc))
#include <theolizer/internal/version_manual.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

#endif//THEOLIZER_WRITE_CODE // ###### std::vector<T, Alloc> ######

#endif  // THEOLIZER_INTERNAL_DOXYGEN

//############################################################################
//      要素をオブジェクト追跡するstd::vector<>
//############################################################################

// ***************************************************************************
//      theolizer::VectorPointee<>
//          std::vector<>の単純な派生クラス
// ***************************************************************************

namespace theolizer
{

template<class T, class Alloc=std::allocator<T> >
class THEOLIZER_ANNOTATE(CS) VectorPointee : public std::vector<T, Alloc>
{
public:
    using vector::vector;
};

}   // namespace theolizer

// ***************************************************************************
//      シリアライズ指定
// ***************************************************************************

THEOLIZER_NON_INTRUSIVE_TEMPLATE_ORDER((template<class T, class Alloc>),
                                        (theolizer::VectorPointee<T, Alloc>), 1,
                                        VectorTrackingPrimary);

//----------------------------------------------------------------------------
//      ユーザ定義
//          回復処理の注意事項：
//              余分なデータの破棄、および、ClassType終了処理のため、
//              必ずiSerializer.readPreElement()がfalseを返却するまで
//              処理しておくこと。
//----------------------------------------------------------------------------

//      ---<<< Version.1 >>>---

template<class T, class Alloc>
template<class tMidSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<theolizer::VectorPointee<T, Alloc>>::
    TheolizerUserDefine<tMidSerializer, tTheolizerVersion, 1>
{
    // 保存
    static void saveClassManual
    (
        tMidSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(Alloc);

        THEOLIZER_PROCESS(iSerializer, iInstance->size());
        for (auto& aElement : *iInstance)
        {
            THEOLIZER_PROCESS_POINTEE(iSerializer, aElement);
        }
    }

    // 回復
    static void loadClassManual
    (
        tMidSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(Alloc);

        // もし、nullptrなら、インスタンス生成
        if (!oInstance)   oInstance=new typename tTheolizerVersion::TheolizerTarget();

        std::size_t aSize;
        THEOLIZER_PROCESS(iSerializer, aSize);

        // 先に領域をvector内部に生成してから、そこへ回復する。
        //  これにより、要素のコピーやムーブが発生しないようにすることで、
        //  「親」へのポインタが壊れないようにしている。
        oInstance->resize(aSize);
        for (auto& aElement : *oInstance)
        {
            THEOLIZER_PROCESS_POINTEE(iSerializer, aElement);
            theolizer::internal::checkDataShort
            (
                !iSerializer.isTerminated(),
                "std::vector<T, Alloc>"
            );
        }
    }
};

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#ifdef  THEOLIZER_WRITE_CODE // ###### theolizer::VectorPointee<T, Alloc> ######

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE theolizer::VectorPointee<T, Alloc>
#define THEOLIZER_GENERATED_PARAMETER_LIST template<class T, class Alloc>
#define THEOLIZER_GENERATED_UNIQUE_NAME VectorTrackingPrimary

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8"theolizer::VectorPointee",T,Alloc))
#include <theolizer/internal/version_manual.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

#endif//THEOLIZER_WRITE_CODE // ###### theolizer::VectorPointee<T, Alloc> ######

//############################################################################
//      End
//############################################################################

#endif  // THEOLIZER_INTERNAL_VECTOR_H
