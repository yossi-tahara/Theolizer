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

namespace
{

struct VersionList
{
    std::string mName;
    unsigned    mVersionNo; // 単調増加すること。また、0の時は変更テストを行わない
};
VersionList const gVersionList[]=
{
    {"basic", 0},
    {"ver1a", 1},
    {"ver1b", 1}
};

}

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

#endif  // ALL_COMMON_H
