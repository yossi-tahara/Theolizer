//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          クラス変更の複合テスト
//              オブジェクト追跡
//              バージョン・アップ／ダウン処理（参照テスト含む）
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

#include "disable_test.h"
#ifndef DISABLE_MODIFY_COMPLEX_TEST

// ***************************************************************************
//      インクルード
// ***************************************************************************

// 標準ライブラリ
#include <iostream>
#include <fstream>
#include <string>

// theolizerライブラリ
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_json.h>
#include <theolizer/memory.h>

// プロジェクト・ヘッダ
#include "common.h"
#include "test_modify_complex.h"

// 自動生成ソース
#include "test_modify_complex.cpp.theolizer.hpp"

//############################################################################
//      組み合わせテスト
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

//! [saveModifyComplex]
template<class tSerializer>
void saveModifyComplex(tSerializer& iSerializer)
{
    std::cout << "    saveModifyComplex();\n";

//----------------------------------------------------------------------------
//      オブジェクト追跡
//----------------------------------------------------------------------------

    {
        iSerializer;
    }

//----------------------------------------------------------------------------
//      バージョン・アップ／ダウン処理（参照テスト含む）
//----------------------------------------------------------------------------

    {
    }
}
//! [saveModifyComplex]

INSTANTIATION_ALL(saveModifyComplex);

// ***************************************************************************
//      回復
// ***************************************************************************

//! [loadModifyComplex]
template<class tSerializer>
void loadModifyComplex(tSerializer& iSerializer)
{
//theolizer::DisplayPass aDisplayPass;
    std::cout << "    loadModifyComplex();\n";

//----------------------------------------------------------------------------
//      オブジェクト追跡
//----------------------------------------------------------------------------

    {
        iSerializer;
    }

//----------------------------------------------------------------------------
//      バージョン・アップ／ダウン処理（参照テスト含む）
//----------------------------------------------------------------------------

    {
    }
}
//! [loadModifyComplex]

INSTANTIATION_ALL(loadModifyComplex);

//############################################################################
//      保存先のテスト
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void saveDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB& iSerializerAB
)
{
    std::cout << "saveDestinations()" << std::endl;

    {
        iSerializerA;
        iSerializerB;
        iSerializerAB;
    }
}

INSTANTIATION_DESTINATIONS(saveDestinations);

// ***************************************************************************
//      回復
// ***************************************************************************

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void loadDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB& iSerializerAB
)
{
    std::cout << "loadDestinations()" << std::endl;

    {
        iSerializerA;
        iSerializerB;
        iSerializerAB;
    }
}

INSTANTIATION_DESTINATIONS(loadDestinations);

#endif  // DISABLE_MODIFY_COMPLEX_TEST
