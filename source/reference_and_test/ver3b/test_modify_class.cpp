//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          クラス変更の使い方
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
#ifndef DISABLE_MODIFY_CLASS_TEST

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
#include "test_modify_class.h"

// 自動生成ソース
#include "test_modify_class.cpp.theolizer.hpp"

//############################################################################
//      組み合わせテスト
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

//! [saveModifyClass]
template<class tSerializer>
void saveModifyClass(tSerializer& iSerializer)
{
    std::cout << "    saveModifyClass();\n";

//----------------------------------------------------------------------------
//      配列要素数の上限テスト
//----------------------------------------------------------------------------

    {
        auto    aArray=std::unique_ptr<ArraySizeTest>(new ArraySizeTest{true});
        THEOLIZER_PROCESS(iSerializer, aArray);

        // オブジェクトIDテーブルのクリア
        iSerializer.clearTracking();
    }

//----------------------------------------------------------------------------
//      
//----------------------------------------------------------------------------

    {
    }
}
//! [saveModifyClass]

INSTANTIATION_ALL(saveModifyClass);

// ***************************************************************************
//      回復
// ***************************************************************************

//! [loadModifyClass]
template<class tSerializer>
void loadModifyClass(tSerializer& iSerializer)
{
//theolizer::DisplayPass aDisplayPass;

    std::size_t aIndex=gIndex;
    if (aIndex == kDefaultIndex)
    {
        aIndex=gProgramIndex;
    }

    VersionEnum aVersionEnum=gVersionList[aIndex].mVersionEnum;
    std::cout << "    loadModifyClass(" << aVersionEnum << ");\n";

//----------------------------------------------------------------------------
//      配列要素数の上限テスト
//----------------------------------------------------------------------------

    {
        auto    aArray=std::unique_ptr<ArraySizeTest>(new ArraySizeTest{});
        THEOLIZER_PROCESS(iSerializer, aArray);
        aArray->check(true, (aIndex < gMyIndex)?kDefSize:ArraySizeTest::kSize);

        // オブジェクトIDテーブルのクリア
        iSerializer.clearTracking();
    }

//----------------------------------------------------------------------------
//      
//----------------------------------------------------------------------------

    {
    }
}
//! [loadModifyClass]

INSTANTIATION_ALL(loadModifyClass);

#endif  // DISABLE_MODIFY_CLASS_TEST
