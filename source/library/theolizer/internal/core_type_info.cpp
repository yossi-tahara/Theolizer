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

std::size_t TypeInfoList::registerType(BaseTypeInfo* iTypeInfo)
{
    std::size_t ret=mList.size();
    mList.push_back(iTypeInfo);
    return ret;
}

TypeIndex TypeInfoList::registerType2(BaseTypeInfo* iTypeInfo)
{
    std::size_t ret=mList2.size();
    mList2.push_back(iTypeInfo);
    return ret;
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
