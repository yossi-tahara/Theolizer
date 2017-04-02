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
//      変更テスト用クラス（名前対応）
//----------------------------------------------------------------------------

#ifndef DISABLE_MODIFY_CLASS_TEST_NAME
    {
        ModifyClassName aModifyClassName{true};
        THEOLIZER_PROCESS(iSerializer, aModifyClassName);
    }
#endif  // DISABLE_MODIFY_CLASS_TEST_NAME

//----------------------------------------------------------------------------
//      変更テスト用クラス（順序対応）
//----------------------------------------------------------------------------

#ifndef DISABLE_MODIFY_CLASS_TEST_ORDER
    {
        ModifyClassOrder    aModifyClassOrder{true};
        THEOLIZER_PROCESS(iSerializer, aModifyClassOrder);
    }
#endif  // DISABLE_MODIFY_CLASS_TEST_ORDER

//----------------------------------------------------------------------------
//      配列要素数の上限テスト
//----------------------------------------------------------------------------

#ifndef DISABLE_MODIFY_CLASS_TEST_ARRAY
    {
        auto    aArray=std::unique_ptr<ArraySizeTest>(new ArraySizeTest{true});
        THEOLIZER_PROCESS(iSerializer, aArray);

        // オブジェクトIDテーブルのクリア
        iSerializer.clearTracking();
    }
#endif  // DISABLE_MODIFY_CLASS_TEST_ARRAY
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

    VersionEnum aVersionEnum=gVersionList[gDataIndex].mVersionEnum;
    std::cout << "    loadModifyClass(" << aVersionEnum << ");\n";

//----------------------------------------------------------------------------
//      変更テスト用クラス（名前対応）
//----------------------------------------------------------------------------

#ifndef DISABLE_MODIFY_CLASS_TEST_NAME
    {
        ModifyClassName  aModifyClassName;
        THEOLIZER_PROCESS(iSerializer, aModifyClassName);
        aModifyClassName.check();
    }
#endif  // DISABLE_MODIFY_CLASS_TEST_NAME

//----------------------------------------------------------------------------
//      変更テスト用クラス（順序対応）
//----------------------------------------------------------------------------

#ifndef DISABLE_MODIFY_CLASS_TEST_ORDER
    {
        ModifyClassOrder    aModifyClassOrder;
        THEOLIZER_PROCESS(iSerializer, aModifyClassOrder);
        aModifyClassOrder.check();
    }
#endif  // DISABLE_MODIFY_CLASS_TEST_ORDER

//----------------------------------------------------------------------------
//      配列要素数の上限テスト
//----------------------------------------------------------------------------

#ifndef DISABLE_MODIFY_CLASS_TEST_ARRAY
    {
        auto    aArray=std::unique_ptr<ArraySizeTest>(new ArraySizeTest{});
        THEOLIZER_PROCESS(iSerializer, aArray);
        aArray->check();

        // オブジェクトIDテーブルのクリア
        iSerializer.clearTracking();
    }
#endif  // DISABLE_MODIFY_CLASS_TEST_ARRAY
}
//! [loadModifyClass]

INSTANTIATION_ALL(loadModifyClass);

#endif  // DISABLE_MODIFY_CLASS_TEST
