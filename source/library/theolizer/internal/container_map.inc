//############################################################################
//      
/*!
@brief      Theolizerライブラリの標準コンテナ・サポート(キー有り用)
@ingroup    TheolizerLib
@file       container_key.inc
@author     Yoshinori Tahara(Theoride Technology)
@date       2017/02/05 Created
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

//############################################################################
//      Begin
//############################################################################

#ifndef THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//      前準備
// ***************************************************************************

#define THEOLIZER_INTERNAL_FULL_NAME                                        \
    THEOLZIER_INTERNAL_CONTAINER_NAME<THEOLZIER_INTERNAL_CONTAINER_ARGUMENT>

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
        typedef typename tTheolizerVersion::TheolizerTarget TheolizerTarget;
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(typename TheolizerTarget::value_type);
#ifndef THEOLIZER_INTERNAL_IS_UNORDERED
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(Compare);
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(Alloc);
#else
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(Hash);
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(Pred);
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(Alloc);
#endif
        THEOLIZER_PROCESS(iSerializer, iInstance->size());
        auto itr=iInstance->begin();
        for (std::size_t i=0; i < iInstance->size(); ++i, ++itr)
        {
            THEOLIZER_PROCESS(iSerializer, itr->first);
#ifndef THEOLIZER_INTERNAL_POINTEE
            THEOLIZER_PROCESS(iSerializer, itr->second);
#else
            THEOLIZER_PROCESS_POINTEE(iSerializer, itr->second);
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
        typedef typename tTheolizerVersion::TheolizerTarget TheolizerTarget;
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(typename TheolizerTarget::value_type);
#ifndef THEOLIZER_INTERNAL_IS_UNORDERED
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(Compare);
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(Alloc);
#else
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(Hash);
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(Pred);
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(Alloc);
#endif

        // もし、nullptrなら、インスタンス生成
        if (!oInstance)   oInstance=new typename tTheolizerVersion::TheolizerTarget();

        std::size_t aSize;
        THEOLIZER_PROCESS(iSerializer, aSize);

#ifdef THEOLIZER_INTERNAL_IS_MULTI  // キーの重複を許す場合、一旦クリアする
        oInstance->clear();
#endif

        for (std::size_t i=0; i < aSize; ++i)
        {
            typename TheolizerTarget::key_type  aKey;
            THEOLIZER_PROCESS(iSerializer, aKey);
            auto found=oInstance->find(aKey);
            // 新規
            if (found == oInstance->end())
            {
                typename TheolizerTarget::mapped_type   aData;
                auto temp=oInstance->emplace(aKey, aData);
#ifdef THEOLIZER_INTERNAL_IS_MULTI
    #ifndef THEOLIZER_INTERNAL_POINTEE
                THEOLIZER_PROCESS(iSerializer, temp->second);
    #else
                #error "multiMap does not support Pointee type."
    #endif
#else
    #ifndef THEOLIZER_INTERNAL_POINTEE
                THEOLIZER_PROCESS(iSerializer, temp.first->second);
    #else
                THEOLIZER_PROCESS_POINTEE(iSerializer, temp.first->second);
    #endif
#endif
            }
            // 既存
            else
            {
#ifndef THEOLIZER_INTERNAL_POINTEE
                THEOLIZER_PROCESS(iSerializer, found->second);
#else
                THEOLIZER_PROCESS_POINTEE(iSerializer, found->second);
#endif
            }
        }
    }
};

//############################################################################
//      End
//############################################################################

#endif  // THEOLIZER_INTERNAL_DOXYGEN
