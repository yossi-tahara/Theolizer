//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          メイン
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

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#if defined(_MSC_VER)
    #pragma warning(disable:4100)
#endif

// ***************************************************************************
//      インクルード
// ***************************************************************************

// 標準ライブラリ
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

// theolizerライブラリ
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_fast.h>
#include <theolizer/serializer_json.h>

// プロジェクト・ヘッダ
#include "common.h"

// 自動生成ソース
#include "main.cpp.theolizer.hpp"

//############################################################################
//      テスト基本部
//############################################################################

// ***************************************************************************
//      各種テスト関数の宣言
// ***************************************************************************

//----------------------------------------------------------------------------
//      基本テスト
//----------------------------------------------------------------------------

void tutoriseBasic();

//----------------------------------------------------------------------------
//      THEOLIZER_PROCESS()の基本的な使い方
//----------------------------------------------------------------------------

template<class tSerializer>
void saveBasicProcess(tSerializer& iSerializer);

template<class tSerializer>
void loadBasicProcess(tSerializer& iSerializer);

//----------------------------------------------------------------------------
//      classの様々なバリエーション
//----------------------------------------------------------------------------

//      ---<<< 使い方のサンプル・コード >>>---

void tutoriseClassVariation();

//      ---<<< 自動テスト >>>---

template<class tSerializer>
void saveClassVariation(tSerializer& iSerializer);

template<class tSerializer>
void loadClassVariation(tSerializer& iSerializer);

//----------------------------------------------------------------------------
//      enum型の様々なバリエーション
//----------------------------------------------------------------------------

//      ---<<< 使い方のサンプル・コード >>>---

void tutoriseEnumVariation();

//      ---<<< 自動テスト >>>---

template<class tSerializer>
void saveEnumVariation(tSerializer& iSerializer);

template<class tSerializer>
void loadEnumVariation(tSerializer& iSerializer);

// ***************************************************************************
//      各テスト呼び出し
// ***************************************************************************

//----------------------------------------------------------------------------
//      使い方説明サンプル呼び出し
//----------------------------------------------------------------------------

void callTutorial()
{
    std::cout << "callTutorial();\n";

    #ifdef ENABLE_BASIC_PROCESS_TEST
        tutoriseBasic();
    #endif
    #ifdef ENABLE_CLASS_VARIATION_TEST
        tutoriseClassVariation();
    #endif
    #ifdef ENABLE_ENUM_VARIATION_TEST
        tutoriseEnumVariation();
    #endif

    // ここまでの中間結果表示
    theolizer::printResult("------------- Result of tutorial samples ----------");
    std::cerr << "\n\n";
}

//----------------------------------------------------------------------------
//      通常テスト
//----------------------------------------------------------------------------

template<class tSerializer>
void saveBasic(tSerializer& iSerializer)
{
    std::cout << "saveBasic();\n";

#ifdef ENABLE_BASIC_PROCESS_TEST
    saveBasicProcess(iSerializer);
#endif
#ifdef ENABLE_CLASS_VARIATION_TEST
    saveClassVariation(iSerializer);
#endif
#ifdef ENABLE_ENUM_VARIATION_TEST
    saveEnumVariation(iSerializer);
#endif
}

template<class tSerializer>
void loadBasic(tSerializer& iSerializer)
{
    std::cout << "loadBasic();\n";

#ifdef ENABLE_BASIC_PROCESS_TEST
    loadBasicProcess(iSerializer);
#endif
#ifdef ENABLE_CLASS_VARIATION_TEST
    loadClassVariation(iSerializer);
#endif
#ifdef ENABLE_ENUM_VARIATION_TEST
    loadEnumVariation(iSerializer);
#endif
}

//----------------------------------------------------------------------------
//      保存先指定の合成テスト
//----------------------------------------------------------------------------

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void callSaveDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB& iSerializerAB
)
{
}

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void callLoadDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB& iSerializerAB
)
{
}

//############################################################################
//      変更テストの共通部取り込み
//############################################################################

#include "main.inc"
