//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          共通ヘッダ
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

#if !defined(COMMON_H)
#define COMMON_H

// ***************************************************************************
//      共通のインクルード
// ***************************************************************************

#include <string>
#include <theolizer/test_tool.h>

#include "all_common.h"

//############################################################################
//      ユーティリティ
//############################################################################

// ***************************************************************************
//      保存先指定の網羅テスト用
//          コンストラクトに必要な処理はこちらで行う
// ***************************************************************************

//----------------------------------------------------------------------------
//      保存先リスト（マスター・ファイルと取引ファイル）
//----------------------------------------------------------------------------

THEOLIZER_DESTINATIONS
(
    All,
    Master,
    Trade
);

//----------------------------------------------------------------------------
//      保存先リスト（組み合わせテスト用）
//----------------------------------------------------------------------------

THEOLIZER_DESTINATIONS
(
    Trade,
    DestA,
    DestB,
    DestC
);

//----------------------------------------------------------------------------
//      テスト関数の実体化用マクロ
//----------------------------------------------------------------------------

#define INSTANTIATION_DEST(dFunc, dSerializer)                              \
    template void dFunc                                                     \
    <                                                                       \
        dSerializer<theolizerD::DestA>,                                     \
        dSerializer<theolizerD::DestB>,                                     \
        dSerializer<theolizerD::DestA, theolizerD::DestB>                   \
    >                                                                       \
    (                                                                       \
        dSerializer<theolizerD::DestA>&,                                    \
        dSerializer<theolizerD::DestB>&,                                    \
        dSerializer<theolizerD::DestA, theolizerD::DestB>&                  \
    )

#define INSTANTIATION_DESTINATIONS(dFunc)                                   \
    INSTANTIATION_DEST(dFunc, theolizer::JsonOSerializer);                  \
    INSTANTIATION_DEST(dFunc, theolizer::JsonISerializer);                  \
    INSTANTIATION_DEST(dFunc, theolizer::BinaryOSerializer);                \
    INSTANTIATION_DEST(dFunc, theolizer::BinaryISerializer);                \
    INSTANTIATION_DEST(dFunc, theolizer::FastOSerializer);                  \
    INSTANTIATION_DEST(dFunc, theolizer::FastISerializer)

//############################################################################
//      配列テスト用関数テンプレート
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

//! [saveTestArray]
template<typename tType, std::size_t N, class tSerializer, typename tFunc>
void saveTestArray(tSerializer& iSerializer, tFunc iFunc)
{
    std::cout << "        saveTestArray<"
              << THEOLIZER_INTERNAL_TYPE_NAME(tType) << ", " << N << ">\n";

    // 1次元配列
    tType aArray1[N];
    for (std::size_t i=0; i < N; ++i)
    {
        theolizer::cutOptimize();
        aArray1[i]=iFunc(i);
    }
    THEOLIZER_PROCESS(iSerializer, aArray1);

    // 2次元配列
    tType aArray2[2][N];
    for (std::size_t j=0; j < 2; ++j)
    {
        for (std::size_t i=0; i < N; ++i)
        {
            theolizer::cutOptimize();
            aArray2[j][i]=iFunc(i+j);
        }
    }
    THEOLIZER_PROCESS(iSerializer, aArray2);

    // 3次元配列
    tType aArray3[3][2][N];
    for (std::size_t k=0; k < 3; ++k)
    {
        for (std::size_t j=0; j < 2; ++j)
        {
            for (std::size_t i=0; i < N; ++i)
            {
                theolizer::cutOptimize();
               aArray3[k][j][i]=iFunc(i+j+k);
            }
        }
    }
    THEOLIZER_PROCESS(iSerializer, aArray3);
}
//! [saveTestArray]

// ***************************************************************************
//      回復
// ***************************************************************************

//! [loadTestArray]
template<typename tType, std::size_t N, class tSerializer, typename tFunc>
void loadTestArray(tSerializer& iSerializer, tFunc iFunc)
{
    std::cout << "        loadTestArray<"
              << THEOLIZER_INTERNAL_TYPE_NAME(tType) << ", " << N << ">\n";

    // 1次元配列
    tType aArray1[N];
    THEOLIZER_PROCESS(iSerializer, aArray1);
    for (std::size_t i=0; i < N; ++i)
    {
        tType aTemp=iFunc(i);
        theolizer::cutOptimize();
        THEOLIZER_EQUAL(aArray1[i], aTemp, i);
    }

    // 2次元配列
    tType aArray2[2][N];
    THEOLIZER_PROCESS(iSerializer, aArray2);
    for (std::size_t j=0; j < 2; ++j)
    {
        for (std::size_t i=0; i < N; ++i)
        {
            tType aTemp=iFunc(i+j);
            theolizer::cutOptimize();
            THEOLIZER_EQUAL(aArray2[j][i], aTemp, i, j);
        }
    }

    // 3次元配列
    tType aArray3[3][2][N];
    THEOLIZER_PROCESS(iSerializer, aArray3);
    for (std::size_t k=0; k < 3; ++k)
    {
        for (std::size_t j=0; j < 2; ++j)
        {
            for (std::size_t i=0; i < N; ++i)
            {
                tType aTemp=iFunc(i+j+k);
                theolizer::cutOptimize();
                THEOLIZER_EQUAL(aArray3[k][j][i], aTemp, i, j, k);
            }
        }
    }
}
//! [loadTestArray]

#endif  // COMMON_H
