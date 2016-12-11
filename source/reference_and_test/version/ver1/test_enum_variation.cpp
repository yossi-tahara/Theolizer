//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          enum型の様々なバリエーション
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

// ***************************************************************************
//      インクルード
// ***************************************************************************

// 標準ライブラリ
#include <iostream>
#include <fstream>
#include <ctime>

// theolizerライブラリ
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_json.h>

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

    saveEnumFullAuto(iSerializer);
    FullAutoClass4EnumFullAuto  aFullAutoClass4EnumFullAuto{true};
    THEOLIZER_PROCESS(iSerializer, aFullAutoClass4EnumFullAuto);

//      ---<<< 非侵入型半自動（シンボル名対応） >>>---

    saveEnumSymName(iSerializer);
    FullAutoClass4EnumSymName   aFullAutoClass4EnumSymName{true};
    THEOLIZER_PROCESS(iSerializer, aFullAutoClass4EnumSymName);

//      ---<<< 非侵入型半自動（シンボル値対応） >>>---

    saveEnumSymVal(iSerializer);
    FullAutoClass4EnumSymVal    aFullAutoClass4EnumSymVal{true};
    THEOLIZER_PROCESS(iSerializer, aFullAutoClass4EnumSymVal);
}

INSTANTIATION_ALL(void, saveEnumVariation);

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
    FullAutoClass4EnumFullAuto  aFullAutoClass4EnumFullAuto{};
    THEOLIZER_PROCESS(iSerializer, aFullAutoClass4EnumFullAuto);
    aFullAutoClass4EnumFullAuto.check();

//      ---<<< 非侵入型半自動（シンボル名対応） >>>---

    loadEnumSymName(iSerializer);
    FullAutoClass4EnumSymName   aFullAutoClass4EnumSymName{};
    THEOLIZER_PROCESS(iSerializer, aFullAutoClass4EnumSymName);
    aFullAutoClass4EnumSymName.check();

//      ---<<< 非侵入型半自動（シンボル値対応） >>>---

    loadEnumSymVal(iSerializer);
    FullAutoClass4EnumSymVal    aFullAutoClass4EnumSymVal{};
    THEOLIZER_PROCESS(iSerializer, aFullAutoClass4EnumSymVal);
    aFullAutoClass4EnumSymVal.check();

}

INSTANTIATION_ALL(void, loadEnumVariation);
