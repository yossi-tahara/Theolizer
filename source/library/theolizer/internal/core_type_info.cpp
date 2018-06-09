//############################################################################
//      Theolizerライブラリのコア部
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

#define THEOLIZER_INTERNAL_EXCLUDE_VERSION_H

//############################################################################
//      インクルード
//############################################################################

#include "avoid-trouble.h"
#include "internal.h"

#include <cstring>
#include <algorithm>
#include <unordered_map>

#include "core.h"

//############################################################################
//      Begin
//############################################################################

namespace theolizer
{
namespace internal
{

//############################################################################
//      型管理
//############################################################################

// ***************************************************************************
//      型種別(メタ・シリアライズ用)
// ***************************************************************************

//----------------------------------------------------------------------------
//      文字列返却
//----------------------------------------------------------------------------

std::string TypeKindXXXX::to_string() const
{
    std::string ret;

    auto add=[&](char const* iRhs){ if (!ret.empty()) ret+="|"; ret += iRhs; };

    unsigned    aValue = mValue;
    switch(aValue & ~(AdditionalMask|TemplateFlag))
    {
    case Primitive:             ret="Primitive";            break;
    case Enum:                  ret="Enum";                 break;
    case IntrusiveAuto:         ret="IntrusiveAuto";        break;
    case NonIntrusiveAuto:      ret="NonIntrusiveAuto";     break;
    case IntrusiveManual:       ret="IntrusiveManual";      break;
    case NonIntrusiveManual:    ret="NonIntrusiveManual";   break;
    default:
        if (aValue & PrimitiveFlag)     add("PrimitiveFlag");
        if (aValue & EnumFlag)          add("EnumFlag");
        if (aValue & ClassFlag)         add("ClassFlag");
        if (aValue & NonIntrusiveFlag)  add("NonIntrusiveFlag");
        if (aValue & ManualFlag)        add("ManualFlag");
        break;
    }
    if (aValue & TemplateFlag)      add("TemplateFlag");
    if (aValue & ClassFlag)
    {
        if (aValue & Order)         add("Order");
    }
    else if (aValue & EnumFlag)
    {
        if (aValue & SaveValue)         add("SaveValue");
        if (aValue & Scoped)            add("Scoped");
        switch(aValue & UnderlyingMask)
        {
        case Bool:      add("Bool");    break;
        case Int8:      add("Int8");    break;
        case UInt8:     add("UInt8");   break;
        case Int16:     add("Int16");   break;
        case UInt16:    add("UInt16");  break;
        case Int32:     add("Int32");   break;
        case UInt32:    add("UInt32");  break;
        case Int64:     add("Int64");   break;
        case UInt64:    add("UInt64");  break;
        }
    }

    return ret;
}

// ***************************************************************************
//      型リスト(シングルトン)
// ***************************************************************************

//----------------------------------------------------------------------------
//      TypeInfoList実体定義
//      シングルトンとするため、ここでstatic定義する。
//          MinGWではヘッダで定義するとexeとdllで別インスタンスになってしまう。
//          おそらくgccも同じ。
//----------------------------------------------------------------------------

TypeInfoList& TypeInfoList::getInstance()
{
    static TypeInfoList instance;
    return instance;
}

//----------------------------------------------------------------------------
//      型をTypeInfoListへ登録する(TypeIndex返却)
//----------------------------------------------------------------------------

TypeIndex TypeInfoList::registerType(BaseTypeInfo* iTypeInfo)
{
    std::size_t ret=mTypeInfoList.size();
    THEOLIZER_INTERNAL_ASSERT(ret <= std::numeric_limits<unsigned>::max(),
        u8"too many Types!!");
    mTypeInfoList.push_back(iTypeInfo);

//std::cout << "registerType(" << iTypeInfo->getCName() << ") TypeIndex=" << ret << "e0\n";

    return static_cast<unsigned>(ret);
}

// ***************************************************************************
//      プリミティブ管理クラス(シングルトン)
//          プリミティブ型を型リストへ登録する
// ***************************************************************************

template<typename tPrimitiveType>
PrimitiveTypeInfo<tPrimitiveType>& PrimitiveTypeInfo<tPrimitiveType>::getInstance()
{
    static PrimitiveTypeInfo<tPrimitiveType> instance;
    return instance;
}

// 明示的実体化
#define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType)                             \
    template class PrimitiveTypeInfo<dType>;
#include "primitive.inc"

}   // namespace internal
}   // namespace theolizer
