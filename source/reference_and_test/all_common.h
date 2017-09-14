//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          全バージョン共通ヘッダ
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

#if !defined(ALL_COMMON_H)
#define ALL_COMMON_H

// ***************************************************************************
//      共通のインクルード
// ***************************************************************************

#include <string>

#include "disable_test.h"

//############################################################################
//      テスト用の全バージョンで共通な定義
//############################################################################

// ***************************************************************************
//      バージョン名のリスト
// ***************************************************************************

//----------------------------------------------------------------------------
//      定義
//          VERSION_DEF(バージョン名, グローバル・バージョン番号)
//          グローバル・バージョン番号は、
//              単調増加すること。
//              0の時は変更テストを行わない。
//----------------------------------------------------------------------------

#define VERSION_INDEX()                                                     \
    VERSION_DEF(basic, 0),                                                  \
    VERSION_DEF(basic2,0),                                                  \
    VERSION_DEF(ver1a, 1),                                                  \
    VERSION_DEF(ver1b, 1),                                                  \
    VERSION_DEF(ver1c, 1),                                                  \
    VERSION_DEF(ver2a, 2),                                                  \
    VERSION_DEF(ver3a, 3),                                                  \
    VERSION_DEF(ver3b, 3)

//----------------------------------------------------------------------------
//      実体
//----------------------------------------------------------------------------

enum class VersionEnum
{
    #define VERSION_DEF(dName, dVersionNo)  dName
    VERSION_INDEX()
    #undef  VERSION_DEF
};

struct VersionList
{
    std::string     mName;
    VersionEnum     mVersionEnum;
    unsigned        mVersionNo;
};

namespace
{

VersionList const gVersionList[]=
{
    #define VERSION_DEF(dName, dVersionNo)  {#dName, VersionEnum::dName, dVersionNo}
    VERSION_INDEX()
    #undef  VERSION_DEF
};

}

// ***************************************************************************
//      処理中データのバージョン・テーブル・インデックス
// ***************************************************************************

// デフォルト指定用インデックス番号
const std::size_t   kDefaultIndex=std::numeric_limits<std::size_t>::max();

extern std::size_t gMyIndex;        // 自分自身のバージョン・インデックス
extern std::size_t gIndex;          // データのバージョン・インデックス(指定値)
extern std::size_t gDataIndex;      // データのバージョン・インデックス(実値)
extern std::size_t gProgramIndex;   // 保存したプログラムのバージョン・インデックス

// ***************************************************************************
//      テスト用補助ツール
// ***************************************************************************

//----------------------------------------------------------------------------
//      テスト用関数テンプレート実体化マクロ
//----------------------------------------------------------------------------

#define INSTANTIATION(dFunc, dSerializer)                                   \
    template void dFunc<dSerializer>(dSerializer&)

#define INSTANTIATION_ALL(dFunc)                                            \
    INSTANTIATION(dFunc, theolizer::XmlOSerializer<>);                      \
    INSTANTIATION(dFunc, theolizer::XmlISerializer<>);                      \
    INSTANTIATION(dFunc, theolizer::JsonOSerializer<>);                     \
    INSTANTIATION(dFunc, theolizer::JsonISerializer<>);                     \
    INSTANTIATION(dFunc, theolizer::BinaryOSerializer<>);                   \
    INSTANTIATION(dFunc, theolizer::BinaryISerializer<>);                   \
    INSTANTIATION(dFunc, theolizer::FastOSerializer<>);                     \
    INSTANTIATION(dFunc, theolizer::FastISerializer<>)

//----------------------------------------------------------------------------
//      領域の自動解放
//----------------------------------------------------------------------------

template<typename tType>
struct AutoRelease
{
    tType*&     mData;
    AutoRelease(tType*& iData) : mData(iData) { }
    ~AutoRelease() { delete mData; }
};

template<typename tType>
AutoRelease<tType> makeAutoRelease(tType*& iData)
{
    return AutoRelease<tType>(iData);
}

//############################################################################
//      バージョン・アップ／ダウンのテスト用定義
//############################################################################

// ***************************************************************************
//      ArrayTestのmArray?Dの最大サイズ
// ***************************************************************************

static const unsigned   kArrayMax=160;

#endif  // ALL_COMMON_H
