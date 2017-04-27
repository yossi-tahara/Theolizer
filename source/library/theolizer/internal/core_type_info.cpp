//############################################################################
//      Theolizerライブラリのコア部
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

#define THEOLIZER_INTERNAL_EXCLUDE_VERSION_H

//############################################################################
//      インクルード
//############################################################################

#include "avoid-trouble.h"

#include <cstring>
#include <algorithm>

#include "internal.h"
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
//      型名リストの要素(型名→TypeIndex)
//----------------------------------------------------------------------------

TypeNameList::const_iterator findTypeName(const TypeNameList& iTypeNameList,
                                          char const* iTypeName)
{
    return lower_bound(iTypeNameList.begin(),
                       iTypeNameList.end(),
                       iTypeName,
                       [](const TypeName& iLhs, char const* iTypeName)
                       {
                          return strcmp(iLhs.mTypeName, iTypeName)<0;
                       });
}

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

    // プリミティブなら、そのTypeIndexを記録する
    //  上書きするので最後のものが有効
    if (iTypeInfo->mTypeCategory == etcPrimitiveType)
    {
        mPrimitiveTypeIndex=ret;
    }
    return ret;
}

//----------------------------------------------------------------------------
//      現在のグローバル・バージョン番号に対応したローカル・バージョン番号
//----------------------------------------------------------------------------

unsigned VersionNoList::at(std::size_t iTypeIndex) const
{
    // テーブルが生成されていない場合、最新版を返却する
    if (size() == 0)
    {
        auto& aTypeInfoListImpl=TypeInfoList::getInstance().getList();
return aTypeInfoListImpl[iTypeIndex]->getLastVersionNoV();
    }
    return std::vector<unsigned>::at(iTypeIndex);
}

}   // namespace internal
}   // namespace theolizer
