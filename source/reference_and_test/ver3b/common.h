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

#include <theolizer/test_tool.h>

#include "all_common.h"

//############################################################################
//      グローバル・バージョン番号テーブル宣言
//############################################################################

THEOLIZER_DEFINE_GLOBAL_VERSION_TABLE(MyGVNT, 3);

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
    INSTANTIATION_DEST(dFunc, theolizer::XmlOSerializer);                   \
    INSTANTIATION_DEST(dFunc, theolizer::XmlISerializer);                   \
    INSTANTIATION_DEST(dFunc, theolizer::JsonOSerializer);                  \
    INSTANTIATION_DEST(dFunc, theolizer::JsonISerializer);                  \
    INSTANTIATION_DEST(dFunc, theolizer::BinaryOSerializer);                \
    INSTANTIATION_DEST(dFunc, theolizer::BinaryISerializer);                \
    INSTANTIATION_DEST(dFunc, theolizer::FastOSerializer);                  \
    INSTANTIATION_DEST(dFunc, theolizer::FastISerializer)

#endif  // COMMON_H
