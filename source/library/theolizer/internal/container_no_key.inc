//############################################################################
//      
/*!
@brief      Theolizerライブラリの標準コンテナ・サポート(キー無しresize有り用)
@ingroup    TheolizerLib
@file       container_no_key.inc
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

//############################################################################
//      Begin
//############################################################################

#ifndef THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//      前準備
// ***************************************************************************

#define THEOLIZER_INTERNAL_FULL_NAME                                        \
    THEOLZIER_INTERNAL_CONTAINER_NAME<THEOLZIER_INTERNAL_CONTAINER_ARGUMENT>

#define THEOLIZER_INTERNAL_FULL_NAME_POINTEE                                \
    THEOLZIER_INTERNAL_CONTAINER_NAME_POINTEE<THEOLZIER_INTERNAL_CONTAINER_ARGUMENT>

//############################################################################
//      標準型
//############################################################################

// ***************************************************************************
//      シリアライズ指定
// ***************************************************************************

THEOLIZER_NON_INTRUSIVE_TEMPLATE_ORDER((THEOLZIER_INTERNAL_CONTAINER_PARAMETER),
                                        (THEOLIZER_INTERNAL_FULL_NAME), 1,
                                        THEOLZIER_INTERNAL_CONTAINER_UNIQUE);

//----------------------------------------------------------------------------
//      ユーザ定義
//          回復処理の注意事項：
//              余分なデータの破棄、および、ClassType終了処理のため、
//              必ずiSerializer.readPreElement()がfalseを返却するまで
//              処理しておくこと。
//----------------------------------------------------------------------------

//      ---<<< Version.1 >>>---

THEOLZIER_INTERNAL_CONTAINER_PARAMETER
template<class tMidSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<THEOLIZER_INTERNAL_FULL_NAME>::
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
        auto itr=iInstance->begin();
        for (std::size_t i=0; i < iInstance->size(); ++i, ++itr)
        {
#ifndef THEOLIZER_INTERNAL_IS_VECTOR
            THEOLIZER_PROCESS(iSerializer, *itr);
#else
            typedef typename tTheolizerVersion::TheolizerTarget TheolizerTarget;
            typedef typename TheolizerTarget::value_type        ValueType;
            theolizer::internal::SupportVector<TheolizerTarget,ValueType, tTheolizerVersion>::
                save(iSerializer,*itr);
#endif
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
        if (oInstance->size() < aSize) oInstance->resize(aSize);
        auto itr=oInstance->begin();
        for (std::size_t i=0; i < aSize; ++i, ++itr)
        {
#ifndef THEOLIZER_INTERNAL_IS_VECTOR
            THEOLIZER_PROCESS(iSerializer, *itr);
#else
            typedef typename tTheolizerVersion::TheolizerTarget TheolizerTarget;
            typedef typename TheolizerTarget::value_type        ValueType;
            theolizer::internal::SupportVector<TheolizerTarget,ValueType, tTheolizerVersion>::
                load(iSerializer,*itr);
#endif
        }
    }
};

//############################################################################
//      被ポインタ型
//############################################################################

// ***************************************************************************
//      シリアライズ指定
// ***************************************************************************

THEOLIZER_NON_INTRUSIVE_TEMPLATE_ORDER((THEOLZIER_INTERNAL_CONTAINER_PARAMETER),
                                        (THEOLIZER_INTERNAL_FULL_NAME_POINTEE), 1,
                                        THEOLZIER_INTERNAL_CONTAINER_UNIQUE_POINTEE);

//----------------------------------------------------------------------------
//      ユーザ定義
//          回復処理の注意事項：
//              余分なデータの破棄、および、ClassType終了処理のため、
//              必ずiSerializer.readPreElement()がfalseを返却するまで
//              処理しておくこと。
//----------------------------------------------------------------------------

//      ---<<< Version.1 >>>---

THEOLZIER_INTERNAL_CONTAINER_PARAMETER
template<class tMidSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<THEOLIZER_INTERNAL_FULL_NAME_POINTEE>::
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
        auto itr=iInstance->begin();
        for (std::size_t i=0; i < iInstance->size(); ++i, ++itr)
        {
#ifndef THEOLIZER_INTERNAL_IS_VECTOR
            THEOLIZER_PROCESS_POINTEE(iSerializer, *itr);
#else
            typedef typename tTheolizerVersion::TheolizerTarget TheolizerTarget;
            typedef typename TheolizerTarget::value_type        ValueType;
            theolizer::internal::SupportVectorPointee
            <
                TheolizerTarget,
                ValueType,
                tTheolizerVersion
            >::save(iSerializer, *itr);
#endif
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
        if (oInstance->size() < aSize) oInstance->resize(aSize);
        auto itr=oInstance->begin();
        for (std::size_t i=0; i < aSize; ++i, ++itr)
        {
#ifndef THEOLIZER_INTERNAL_IS_VECTOR
            THEOLIZER_PROCESS_POINTEE(iSerializer, *itr);
#else
            typedef typename tTheolizerVersion::TheolizerTarget TheolizerTarget;
            typedef typename TheolizerTarget::value_type        ValueType;
            theolizer::internal::SupportVectorPointee
            <
                TheolizerTarget,
                ValueType,
                tTheolizerVersion
            >::load(iSerializer, *itr);
#endif
        }
    }
};

//############################################################################
//      End
//############################################################################

#endif  // THEOLIZER_INTERNAL_DOXYGEN
