//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          クラス変更の使い方
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

#include "disable_test.h"
#ifdef ENABLE_MODIFY_CLASS_TEST

// ***************************************************************************
//      インクルード
// ***************************************************************************

// 標準ライブラリ
#include <iostream>
#include <fstream>
#include <string>

// theolizerライブラリ
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_fast.h>
#include <theolizer/serializer_json.h>
#include <theolizer/serializer_xml.h>
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
    (void)iSerializer;  // 警告回避

//----------------------------------------------------------------------------
//      変更テスト用クラス（名前対応）
//----------------------------------------------------------------------------

#ifdef ENABLE_MODIFY_CLASS_TEST_NAME
    {
        ModifyClassName aModifyClassName{true};
        THEOLIZER_PROCESS(iSerializer, aModifyClassName);
    }
#endif  // ENABLE_MODIFY_CLASS_TEST_NAME

//----------------------------------------------------------------------------
//      変更テスト用クラス（順序対応）
//----------------------------------------------------------------------------

#ifdef ENABLE_MODIFY_CLASS_TEST_ORDER
    {
        ModifyClassOrder    aModifyClassOrder{true};
        THEOLIZER_PROCESS(iSerializer, aModifyClassOrder);
    }
#endif  // ENABLE_MODIFY_CLASS_TEST_ORDER

//----------------------------------------------------------------------------
//      配列要素数の上限テスト
//----------------------------------------------------------------------------

#ifdef ENABLE_MODIFY_CLASS_TEST_ARRAY
    {
        auto    aArray=std::unique_ptr<ArrayTest>(new ArrayTest{true});
        THEOLIZER_PROCESS(iSerializer, aArray);

        // オブジェクトIDテーブルのクリア
        iSerializer.clearTracking();
    }
#endif  // ENABLE_MODIFY_CLASS_TEST_ARRAY
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
    (void)iSerializer;  // 警告回避

//----------------------------------------------------------------------------
//      変更テスト用クラス（名前対応）
//----------------------------------------------------------------------------

#ifdef ENABLE_MODIFY_CLASS_TEST_NAME
    {
        ModifyClassName  aModifyClassName;
        THEOLIZER_PROCESS(iSerializer, aModifyClassName);
        aModifyClassName.check();
    }
#endif  // ENABLE_MODIFY_CLASS_TEST_NAME

//----------------------------------------------------------------------------
//      変更テスト用クラス（順序対応）
//----------------------------------------------------------------------------

#ifdef ENABLE_MODIFY_CLASS_TEST_ORDER
    {
        ModifyClassOrder    aModifyClassOrder;
        THEOLIZER_PROCESS(iSerializer, aModifyClassOrder);
        aModifyClassOrder.check();
    }
#endif  // ENABLE_MODIFY_CLASS_TEST_ORDER

//----------------------------------------------------------------------------
//      配列要素数の上限テスト
//----------------------------------------------------------------------------

#ifdef ENABLE_MODIFY_CLASS_TEST_ARRAY
    {
        auto    aArray=std::unique_ptr<ArrayTest>(new ArrayTest{});
        THEOLIZER_PROCESS(iSerializer, aArray);
        aArray->check();

        // オブジェクトIDテーブルのクリア
        iSerializer.clearTracking();
    }
#endif  // ENABLE_MODIFY_CLASS_TEST_ARRAY
}
//! [loadModifyClass]

INSTANTIATION_ALL(loadModifyClass);

#endif  // ENABLE_MODIFY_CLASS_TEST
