//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          enum型の様々なバリエーション
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
#ifdef ENABLE_ENUM_VARIATION_TEST

// ***************************************************************************
//      インクルード
// ***************************************************************************

// 標準ライブラリ
#include <iostream>
#include <fstream>
#include <ctime>

// theolizerライブラリ
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_fast.h>
#include <theolizer/serializer_json.h>
#include <theolizer/serializer_xml.h>

// プロジェクト・ヘッダ
#include "common.h"
#include "test_enum_variation.h"

// 自動生成ソース
#include "test_enum_variation.cpp.theolizer.hpp"

//############################################################################
//      使い方の説明
//############################################################################

// ***************************************************************************
//      動作確認
// ***************************************************************************

void tutoriseEnumVariation()
{
    std::cout << "tutoriseEnumVariation() start" << std::endl;

//----------------------------------------------------------------------------
//      保存
//----------------------------------------------------------------------------

    {
        std::ofstream   aStream("tutorise_enum_variation.json");
        theolizer::JsonOSerializer<> aSerializer(aStream);

        // 非侵入型半自動（シンボル名対応）
        EnumSymName aEnumSymName=eesnVal;
        THEOLIZER_PROCESS(aSerializer, aEnumSymName);

        // 非侵入型半自動（シンボル値対応）
        EnumSymVal  aEnumSymVal=eesvVal;
        THEOLIZER_PROCESS(aSerializer, aEnumSymVal);
    }

//----------------------------------------------------------------------------
//      回復
//----------------------------------------------------------------------------

    {
        std::ifstream   aStream("tutorise_enum_variation.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        // 非侵入型半自動（シンボル名対応）
        EnumSymName aEnumSymName=eesnDef;
        THEOLIZER_PROCESS(aSerializer, aEnumSymName);
        THEOLIZER_EQUAL(aEnumSymName, eesnVal);

        // 非侵入型半自動（シンボル値対応）
        EnumSymVal  aEnumSymVal=eesvDef;
        THEOLIZER_PROCESS(aSerializer, aEnumSymVal);
        THEOLIZER_EQUAL(aEnumSymVal, eesvVal);
    }

    std::cout << "tutoriseEnumVariation() end\n" << std::endl;
}

//############################################################################
//      組み合わせテスト
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

template<class tSerializer>
void saveEnumVariation(tSerializer& iSerializer)
{
//----------------------------------------------------------------------------
//      テスト
//----------------------------------------------------------------------------

//      ---<<< 非侵入型完全自動 >>>---

    saveLValueEnumFullAuto(iSerializer);
    saveRValueEnumFullAuto(iSerializer);
    FullAutoClass4EnumFullAuto  aFullAutoClass4EnumFullAuto{true};
    THEOLIZER_PROCESS(iSerializer, aFullAutoClass4EnumFullAuto);

//      ---<<< 非侵入型半自動（シンボル名対応） >>>---

    saveLValueEnumSymName(iSerializer);
    saveRValueEnumSymName(iSerializer);
    FullAutoClass4EnumSymName   aFullAutoClass4EnumSymName{true};
    THEOLIZER_PROCESS(iSerializer, aFullAutoClass4EnumSymName);

//      ---<<< 非侵入型半自動（シンボル値対応） >>>---

    saveLValueEnumSymVal(iSerializer);
    saveRValueEnumSymVal(iSerializer);
    FullAutoClass4EnumSymVal    aFullAutoClass4EnumSymVal{true};
    THEOLIZER_PROCESS(iSerializer, aFullAutoClass4EnumSymVal);
}

INSTANTIATION_ALL(saveEnumVariation);

// ***************************************************************************
//      回復
// ***************************************************************************

template<class tSerializer>
void loadEnumVariation(tSerializer& iSerializer)
{
//----------------------------------------------------------------------------
//      テスト
//----------------------------------------------------------------------------

//      ---<<< 非侵入型完全自動 >>>---

    loadEnumFullAuto(iSerializer);
    loadEnumFullAuto(iSerializer);
    FullAutoClass4EnumFullAuto  aFullAutoClass4EnumFullAuto{};
    THEOLIZER_PROCESS(iSerializer, aFullAutoClass4EnumFullAuto);
    aFullAutoClass4EnumFullAuto.check();

//      ---<<< 非侵入型半自動（シンボル名対応） >>>---

    loadEnumSymName(iSerializer);
    loadEnumSymName(iSerializer);
    FullAutoClass4EnumSymName   aFullAutoClass4EnumSymName{};
    THEOLIZER_PROCESS(iSerializer, aFullAutoClass4EnumSymName);
    aFullAutoClass4EnumSymName.check();

//      ---<<< 非侵入型半自動（シンボル値対応） >>>---

    loadEnumSymVal(iSerializer);
    loadEnumSymVal(iSerializer);
    FullAutoClass4EnumSymVal    aFullAutoClass4EnumSymVal{};
    THEOLIZER_PROCESS(iSerializer, aFullAutoClass4EnumSymVal);
    aFullAutoClass4EnumSymVal.check();

}

INSTANTIATION_ALL(loadEnumVariation);

#endif  // ENABLE_ENUM_VARIATION_TEST
