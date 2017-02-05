//############################################################################
//      
/*!
@brief      Theolizerライブラリの標準コンテナ・サポート(キー有り重複禁止用)
@ingroup    TheolizerLib
@file       container_key.inc
@author     Yoshinori Tahara(Theoride Technology)
@date       2017/02/05 Created
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
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(Compare);
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(Alloc);

        THEOLIZER_PROCESS(iSerializer, iInstance->size());
        auto itr=iInstance->begin();
        for (std::size_t i=0; i < iInstance->size(); ++i, ++itr)
        {
            THEOLIZER_PROCESS(iSerializer, *itr);
        }
    }

    // 回復
    static void loadClassManual
    (
        tMidSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(Compare);
        THEOLIZER_REGISTER_TEMPLATE_PARAMETER(Alloc);

        // もし、nullptrなら、インスタンス生成
        if (!oInstance)   oInstance=new typename tTheolizerVersion::TheolizerTarget();

        std::size_t aSize;
        THEOLIZER_PROCESS(iSerializer, aSize);

        for (std::size_t i=0; i < aSize; ++i)
        {
            typedef typename tTheolizerVersion::TheolizerTarget TheolizerTarget;
            typedef typename TheolizerTarget::value_type        ValueType;
            ValueType aElement;
            THEOLIZER_PROCESS(iSerializer, aElement);
            oInstance->insert(aElement);
        }
    }
};

//############################################################################
//      End
//############################################################################

#endif  // THEOLIZER_INTERNAL_DOXYGEN
