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

#if !defined(THEOLIZER_INTERNAL_CONTAINERS_H)
#define THEOLIZER_INTERNAL_CONTAINERS_H

//############################################################################
//      Begin
//############################################################################
#ifndef THEOLIZER_INTERNAL_DOXYGEN

THEOLIZER_PROVIDED_BY("Theoride Technology");

#endif
// ***************************************************************************
//          DLL用の警告禁止
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(push)
//#pragma warning(disable:4100 4251)
#endif

//############################################################################
//      自動生成コードの雛形
//          これを各コンテナ用ヘッダ・ファイルの後半へコピーし、
//          THEOLIZER_PROVIDED_BY("Theoride Technology");をコメント・アウトして
//          ビルドすることで、Theolizerドライバにより適切な定義へ自動修正される。
//############################################################################

#if 0

// ***************************************************************************
//      自動生成コード
// ***************************************************************************

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

#endif

//############################################################################
//      グローバル名前空間
//############################################################################

#ifndef THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//      標準コンテナのデフォルト・パラメータ
// ***************************************************************************

//----------------------------------------------------------------------------
//      std::allocator<T>
//----------------------------------------------------------------------------

THEOLIZER_TEMPLATE_PARAMETER_TEMPLATE((template<class T>),
                                      std::allocator, (T),
                                      allocatorTheolizer);

//----------------------------------------------------------------------------
//      std::less<T>
//----------------------------------------------------------------------------

THEOLIZER_TEMPLATE_PARAMETER_TEMPLATE((template<class T>),
                                      std::less, (T),
                                      lessTheolizer);

//----------------------------------------------------------------------------
//      std::hash<Key>
//----------------------------------------------------------------------------

THEOLIZER_TEMPLATE_PARAMETER_TEMPLATE((template<class Key>),
                                      std::hash, (Key),
                                      hashTheolizer);

//----------------------------------------------------------------------------
//      std::equal_to<Key>
//----------------------------------------------------------------------------

THEOLIZER_TEMPLATE_PARAMETER_TEMPLATE((template<class Key>),
                                      std::equal_to, (Key),
                                      equal_toTheolizer);

//############################################################################
//      std::vector<bool>対応
//############################################################################

// ***************************************************************************
//      標準型
// ***************************************************************************

namespace theolizer
{
namespace internal
{

template<class tVector, typename tType, class tTheolizerVersion>
struct SupportVector
{
    template<class tSerializer>
    static void save(tSerializer& iSerializer, typename tVector::const_reference iValue)
    {
        THEOLIZER_PROCESS(iSerializer, iValue);
    }
    template<class tSerializer>
    static void load(tSerializer& iSerializer, typename tVector::reference oValue)
    {
        THEOLIZER_PROCESS(iSerializer, oValue);
    }
};

template<class tVector, class tTheolizerVersion>
struct SupportVector<tVector, bool, tTheolizerVersion>
{
    template<class tSerializer>
    static void save(tSerializer& iSerializer, typename tVector::const_reference iValue)
    {
        bool temp=iValue;
        THEOLIZER_PROCESS(iSerializer, temp);
    }
    template<class tSerializer>
    static void load(tSerializer& iSerializer, typename tVector::reference oValue)
    {
        bool temp;
        THEOLIZER_PROCESS(iSerializer, temp);
        oValue=temp;
    }
};

// ***************************************************************************
//      被ポインタ型
// ***************************************************************************

template<class tVector, typename tType, class tTheolizerVersion>
struct SupportVectorPointee
{
    template<class tSerializer>
    static void save(tSerializer& iSerializer, typename tVector::const_reference iValue)
    {
        THEOLIZER_PROCESS_POINTEE(iSerializer, iValue);
    }
    template<class tSerializer>
    static void load(tSerializer& iSerializer, typename tVector::reference oValue)
    {
        THEOLIZER_PROCESS_POINTEE(iSerializer, oValue);
    }
};

template<class tVector, class tTheolizerVersion>
struct SupportVectorPointee<tVector, bool, tTheolizerVersion>
{
    template<class tSerializer>
    static void save(tSerializer& iSerializer, typename tVector::const_reference iValue)
    {
        THEOLIZER_INTERNAL_ABORT("Can't support VectorPointee<bool>.");
    }
    template<class tSerializer>
    static void load(tSerializer& iSerializer, typename tVector::reference oValue)
    {
        THEOLIZER_INTERNAL_ABORT("Can't support VectorPointee<bool>.");
    }
};

}   // namespace internal
}   // namespace theolizer

//############################################################################
//      std::pair<>対応(std::mapシリーズ対応のため)
//############################################################################

// ***************************************************************************
//      シリアライズ指定
// ***************************************************************************

THEOLIZER_NON_INTRUSIVE_TEMPLATE_ORDER((template<class T1, class T2>),
                                        (std::pair<T1, T2>), 1,
                                        pairTheolizer);

//----------------------------------------------------------------------------
//      ユーザ定義
//----------------------------------------------------------------------------

//      ---<<< Version.1 >>>---

template<class T1, class T2>
template<class tMidSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<std::pair<T1, T2>>::
    TheolizerUserDefine<tMidSerializer, tTheolizerVersion, 1>
{
    // 保存
    static void saveClassManual
    (
        tMidSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        THEOLIZER_PROCESS(iSerializer, iInstance->first);
        THEOLIZER_PROCESS(iSerializer, iInstance->second);
    }

    // 回復
    static void loadClassManual
    (
        tMidSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        // もし、nullptrなら、インスタンス生成
        if (!oInstance)   oInstance=new typename tTheolizerVersion::TheolizerTarget();

        THEOLIZER_PROCESS(iSerializer, oInstance->first);
        THEOLIZER_PROCESS(iSerializer, oInstance->second);
    }
};

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#ifdef  THEOLIZER_WRITE_CODE // ###### std::pair<T1,T2> ######

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE THEOLIZER_INTERNAL_UNPAREN(std::pair<T1, T2>)
#define THEOLIZER_GENERATED_PARAMETER_LIST template<class T1, class T2>
#define THEOLIZER_GENERATED_UNIQUE_NAME pairTheolizer

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8"std::pair",T1,T2))
#include <theolizer/internal/version_manual.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

#endif//THEOLIZER_WRITE_CODE // ###### std::pair<T1,T2> ######

#endif  // THEOLIZER_INTERNAL_DOXYGEN

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
