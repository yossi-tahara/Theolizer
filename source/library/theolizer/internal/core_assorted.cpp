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

#include <algorithm>

#include "core.h"

//############################################################################
//      Begin
//############################################################################

namespace theolizer
{
namespace internal
{

//############################################################################
//      エラー報告サービス群
//############################################################################

// ***************************************************************************
//      エラー・チェック用サーピス群
// ***************************************************************************

void checkDataShort(bool iIsPass, std::string const& iLoc)
{
    THEOLIZER_INTERNAL_ASSERT(iIsPass,
        theolizer::print("%1%:Data are insufficient.", iLoc));
}

void checkExtraData(bool iIsPass, std::string const& iLoc)
{
    THEOLIZER_INTERNAL_ASSERT(iIsPass,
        theolizer::print("%1%:There are too many data.", iLoc));
}

//############################################################################
//      保存先指定
//############################################################################

// ***************************************************************************
//      内部処理用アイテム群
// ***************************************************************************

//----------------------------------------------------------------------------
//      ビット列処理クラス
//----------------------------------------------------------------------------

//      ---<<< 必要なら記録領域追加 >>>---

void BitString::expandList(unsigned iIndex)
{
    // unsignedが不足していたら領域を追加する
    unsigned aNewCount = iIndex+1;
    if (mDataCount < aNewCount)
    {
        std::unique_ptr<unsigned[]> aNewList(new unsigned[aNewCount]);
        std::copy(&mBitList[0], &mBitList[mDataCount], &aNewList[0]);
        std::fill(&aNewList[mDataCount], &aNewList[aNewCount], 0);
        swap(mBitList, aNewList);
        mDataCount=aNewCount;
    }
}

//      ---<<< 指定bit追加 >>>---

void BitString::add(unsigned iBitNo)
{
    // unsignedが不足していたら領域を追加する
    expandList(iBitNo/kUnsignedSize);

    // 該当ビットを立てる
    unsigned aIndex = iBitNo/kUnsignedSize;
    unsigned aUnsignedNo = iBitNo - aIndex*kUnsignedSize;
    mBitList[aIndex] |= (1u << aUnsignedNo);
}

//      ---<<< 指定unsigned追加 >>>---

void BitString::add(unsigned iIndex, unsigned iData)
{
    // unsignedが不足していたら領域を追加する
    expandList(iIndex);

    mBitList[iIndex] |= iData;
}

//      ---<<< 指定bit判定 >>>---

bool BitString::isOne(unsigned iBitNo) const
{
    if (mDataCount == 0)
return false;

    THEOLIZER_INTERNAL_ASSERT(iBitNo < mDataCount*kUnsignedSize,
        theolizer::print("iBitNo(%1%) is too large in BitString::isOne().", iBitNo));

    unsigned aIndex = iBitNo/kUnsignedSize;
    unsigned aUnsignedNo = iBitNo - aIndex*kUnsignedSize;
    return !!(mBitList[aIndex] & (1u << aUnsignedNo));
}

//      ---<<< 文字列化 >>>---

std::string BitString::to_string(unsigned iStart, unsigned iEnd) const
{
    if (iEnd == kUnsignedMax)
    {
        iEnd=mDataCount*kUnsignedSize;
    }
    std::string ret;
    for (unsigned aBitNo=iStart; aBitNo < iEnd; ++aBitNo)
    {
        if (isOne(aBitNo))
        {
            ret.push_back('1');
        }
        else
        {
            ret.push_back('0');
        }
    }
    return ret;
}

//############################################################################
//      型管理
//############################################################################

// ***************************************************************************
//      型情報取得中継
// ***************************************************************************

//----------------------------------------------------------------------------
//      基底クラスのアクセス
//----------------------------------------------------------------------------

thread_local BaseTypeFunctions* xTypeFunctions=nullptr;

BaseTypeFunctions* getTypeFunctions()
{
    return xTypeFunctions;
}

void setTypeFunctions(BaseTypeFunctions* iTypeFunctions)
{
    xTypeFunctions=iTypeFunctions;
}

// ***************************************************************************
//      グローバル・バージョン番号管理
// ***************************************************************************

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
    // テーブルに登録されていない時は完全自動型なのでVer.1
    else if (size() <= iTypeIndex)
    {
return 1;
    }
    return std::vector<unsigned>::at(iTypeIndex);
}




//----------------------------------------------------------------------------
//      キー登録
//----------------------------------------------------------------------------

void addGlobalVersionKey
(
    GlobalVersionKey const& iKey,
    std::vector<GlobalVersionKey>& oKeyList
)
{
    auto found = lower_bound(oKeyList.begin(),
                             oKeyList.end(),
                             iKey,
                             [](GlobalVersionKey const& iLhs, GlobalVersionKey const& iRhs)
                             {
                                return iLhs.mTypeIndex < iRhs.mTypeIndex;
                             });
    THEOLIZER_INTERNAL_ASSERT((found == oKeyList.end())
                 || (found->mTypeIndex != iKey.mTypeIndex), "Multipule adding class");
    oKeyList.insert(found, iKey);
}

//----------------------------------------------------------------------------
//      キー検索
//          見つからなかった時、false返却
//----------------------------------------------------------------------------

bool findGlobalVersionKey
(
    std::vector<GlobalVersionKey> const& iKeyList,
    std::type_index iTypeIndex,
    std::size_t& oListIndex
)
{
    auto found = lower_bound(iKeyList.begin(),
                             iKeyList.end(),
                             iTypeIndex,
                             [](GlobalVersionKey const& iLhs, std::type_index iRhs)
                             {
                                return iLhs.mTypeIndex < iRhs;
                             });

    if ((found == iKeyList.end()) || (found->mTypeIndex != iTypeIndex))
return false;

    oListIndex=found->mListIndex;
    return true;
}

}   // namespace internal

// ***************************************************************************
//      型チェックのモード指定
// ***************************************************************************

std::ostream& operator<<(std::ostream& iOStream, CheckMode iCheckMode)
{
    switch(iCheckMode)
    {
    case CheckMode::InMemory:
        iOStream << "InMemory";
        break;

    case CheckMode::TypeCheckInData:
        iOStream << "TypeCheckInData";
        break;

    case CheckMode::NoTypeCheck:
        iOStream << "NoTypeCheck";
        break;

    case CheckMode::TypeCheck:
        iOStream << "TypeCheck";
        break;

    case CheckMode::TypeCheckByIndex:
        iOStream << "TypeCheckByIndex";
        break;

    case CheckMode::MetaMode:
        iOStream << "MetaMode";
        break;
    }

    return iOStream;
}

// ***************************************************************************
//      シリアライザが提供する機能(プロパティ）のリスト
// ***************************************************************************

std::ostream& operator<<(std::ostream& iOStream, Property iProperty)
{
    switch(iProperty)
    {
    case Property::IsSaver:
        iOStream << "IsSaver";
        break;

    case Property::EncodedString:
        iOStream << "EncodedString";
        break;

    case Property::SupportModifying:
        iOStream << "SupportModifying";
        break;

    case Property::BinaryOpen:
        iOStream << "BinaryOpen";
        break;
    }

    return iOStream;
}

}   // namespace theolizer
