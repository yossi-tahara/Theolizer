//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          全バージョン共通ヘッダ
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
    tType*      mData;
    AutoRelease(tType* iData) : mData(iData) { }
    ~AutoRelease() { delete mData; }
};

template<typename tType>
AutoRelease<tType> makeAutoRelease(tType* iData)
{
    return AutoRelease<tType>(iData);
}

//############################################################################
//      バージョン・アップ／ダウンのテスト用定義
//############################################################################

// ***************************************************************************
//      ArraySizeTestのmArray?Dのサイズ
// ***************************************************************************

static const unsigned   kDefSize =2;
static const unsigned   kVer3Size=160;

#endif  // ALL_COMMON_H
