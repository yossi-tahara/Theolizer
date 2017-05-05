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
//      オブジェクト追跡
//----------------------------------------------------------------------------

//      ---<<< 使い方のサンプル・コード >>>---

void tutoriseObjectTracking();

//      ---<<< 自動テスト >>>---

template<class tSerializer>
void saveObjectTracking(tSerializer& iSerializer);

template<class tSerializer>
void loadObjectTracking(tSerializer& iSerializer);

template<class tSerializer>
void saveObjectTracking3(tSerializer& iSerializer);

template<class tSerializer>
void loadObjectTracking3(tSerializer& iSerializer);

//----------------------------------------------------------------------------
//      ポリモーフィズム
//----------------------------------------------------------------------------

//      ---<<< 使い方のサンプル・コード >>>---

void tutorisePolymorphism();

//      ---<<< 自動テスト >>>---

template<class tSerializer>
void savePolymorphism(tSerializer& iSerializer);

template<class tSerializer>
void loadPolymorphism(tSerializer& iSerializer);

//----------------------------------------------------------------------------
//      保存の有無と保存先
//----------------------------------------------------------------------------

#ifdef ENABLE_DESTINATIONS_TEST

//      ---<<< 使い方のサンプル・コード >>>---

void tutoriseDestinations();

//      ---<<< 自動テスト >>>---

template<class tSerializer>
void saveSpecifySaving(tSerializer& iSerializer);

template<class tSerializer>
void loadSpecifySaving(tSerializer& iSerializer);

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void saveDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB& iSerializerAB
);

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void loadDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB& iSerializerAB
);
#endif // ENABLE_DESTINATIONS_TEST

//----------------------------------------------------------------------------
//      標準コンテナ
//----------------------------------------------------------------------------

#ifdef ENABLE_SUPPORT_STL_TEST

//      ---<<< 使い方のサンプル・コード >>>---

void tutoriseSupportStl();

//      ---<<< 自動テスト >>>---

template<class tSerializer>
void saveSupportStl(tSerializer& iSerializer);

template<class tSerializer>
void loadSupportStl(tSerializer& iSerializer);

//      ---<<< 自動テスト(保存先指定の合成テスト) >>>---

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void saveSupportStlDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB& iSerializerAB
);

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void loadSupportStlDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB& iSerializerAB
);

#endif // ENABLE_SUPPORT_STL_TEST

// ***************************************************************************
//      各テスト呼び出し
// ***************************************************************************

//----------------------------------------------------------------------------
//      使い方説明サンプル呼び出し
//----------------------------------------------------------------------------

void callTutorial()
{
    std::cout << "callTutorial();\n";

    #ifdef ENABLE_OBJECT_TRACKING_TEST
        tutoriseObjectTracking();
    #endif
    #ifdef ENABLE_POLYMORPHISM_TEST
        tutorisePolymorphism();
    #endif
    #ifdef ENABLE_DESTINATIONS_TEST
        tutoriseDestinations();
    #endif
    #ifdef ENABLE_SUPPORT_STL_TEST
        tutoriseSupportStl();
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

#ifdef ENABLE_OBJECT_TRACKING2_TEST
    saveObjectTracking(iSerializer);
#endif
#ifdef ENABLE_OBJECT_TRACKING3_TEST
    saveObjectTracking3(iSerializer);
#endif
#ifdef ENABLE_POLYMORPHISM_TEST
    savePolymorphism(iSerializer);
#endif
#ifdef ENABLE_DESTINATIONS_TEST
    saveSpecifySaving(iSerializer);
#endif
#ifdef ENABLE_SUPPORT_STL_TEST
    saveSupportStl(iSerializer);
#endif
}

template<class tSerializer>
void loadBasic(tSerializer& iSerializer)
{
    std::cout << "loadBasic();\n";

#ifdef ENABLE_OBJECT_TRACKING2_TEST
    loadObjectTracking(iSerializer);
#endif
#ifdef ENABLE_OBJECT_TRACKING3_TEST
    loadObjectTracking3(iSerializer);
#endif
#ifdef ENABLE_POLYMORPHISM_TEST
    loadPolymorphism(iSerializer);
#endif
#ifdef ENABLE_DESTINATIONS_TEST
    loadSpecifySaving(iSerializer);
#endif
#ifdef ENABLE_SUPPORT_STL_TEST
    loadSupportStl(iSerializer);
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
#ifdef ENABLE_DESTINATIONS_TEST
    saveDestinations(iSerializerA, iSerializerB, iSerializerAB);
#endif
#ifdef ENABLE_SUPPORT_STL_TEST
    saveSupportStlDestinations(iSerializerA, iSerializerB, iSerializerAB);
#endif
}

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void callLoadDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB& iSerializerAB
)
{
#ifdef ENABLE_DESTINATIONS_TEST
    loadDestinations(iSerializerA, iSerializerB, iSerializerAB);
#endif
#ifdef ENABLE_SUPPORT_STL_TEST
    loadSupportStlDestinations(iSerializerA, iSerializerB, iSerializerAB);
#endif
}

//############################################################################
//      変更テストの共通部取り込み
//############################################################################

#include "main.inc"
