//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          メイン
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

#ifndef DISABLE_DESTINATIONS_TEST

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
#endif // DISABLE_DESTINATIONS_TEST

//----------------------------------------------------------------------------
//      標準コンテナ
//----------------------------------------------------------------------------

#ifndef DISABLE_SUPPORT_STL_TEST

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

#endif // DISABLE_SUPPORT_STL_TEST

// ***************************************************************************
//      各テスト呼び出し
// ***************************************************************************

//----------------------------------------------------------------------------
//      使い方説明サンプル呼び出し
//----------------------------------------------------------------------------

void callTutorial()
{
    std::cout << "callTutorial();\n";

    #ifndef DISABLE_CLASS_VARIATION_TEST
        tutoriseClassVariation();
    #endif
    #ifndef DISABLE_ENUM_VARIATION_TEST
        tutoriseEnumVariation();
    #endif
    #ifndef DISABLE_OBJECT_TRACKING_TEST
        tutoriseObjectTracking();
    #endif
    #ifndef DISABLE_POLYMORPHISM_TEST
        tutorisePolymorphism();
    #endif
    #ifndef DISABLE_DESTINATIONS_TEST
        tutoriseDestinations();
    #endif
    #ifndef DISABLE_SUPPORT_STL_TEST
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

#ifndef DISABLE_BASIC_PROCESS_TEST
    saveBasicProcess(iSerializer);
#endif
#ifndef DISABLE_CLASS_VARIATION_TEST
    saveClassVariation(iSerializer);
#endif
#ifndef DISABLE_ENUM_VARIATION_TEST
    saveEnumVariation(iSerializer);
#endif
#ifndef DISABLE_OBJECT_TRACKING2_TEST
    saveObjectTracking(iSerializer);
#endif
#ifndef DISABLE_OBJECT_TRACKING3_TEST
    saveObjectTracking3(iSerializer);
#endif
#ifndef DISABLE_POLYMORPHISM_TEST
    savePolymorphism(iSerializer);
#endif
#ifndef DISABLE_DESTINATIONS_TEST
    saveSpecifySaving(iSerializer);
#endif
#ifndef DISABLE_SUPPORT_STL_TEST
    saveSupportStl(iSerializer);
#endif
}

template<class tSerializer>
void loadBasic(tSerializer& iSerializer)
{
    std::cout << "loadBasic();\n";

#ifndef DISABLE_BASIC_PROCESS_TEST
    loadBasicProcess(iSerializer);
#endif
#ifndef DISABLE_CLASS_VARIATION_TEST
    loadClassVariation(iSerializer);
#endif
#ifndef DISABLE_ENUM_VARIATION_TEST
    loadEnumVariation(iSerializer);
#endif
#ifndef DISABLE_OBJECT_TRACKING2_TEST
    loadObjectTracking(iSerializer);
#endif
#ifndef DISABLE_OBJECT_TRACKING3_TEST
    loadObjectTracking3(iSerializer);
#endif
#ifndef DISABLE_POLYMORPHISM_TEST
    loadPolymorphism(iSerializer);
#endif
#ifndef DISABLE_DESTINATIONS_TEST
    loadSpecifySaving(iSerializer);
#endif
#ifndef DISABLE_SUPPORT_STL_TEST
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
#ifndef DISABLE_DESTINATIONS_TEST
    saveDestinations(iSerializerA, iSerializerB, iSerializerAB);
#endif
#ifndef DISABLE_SUPPORT_STL_TEST
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
#ifndef DISABLE_DESTINATIONS_TEST
    loadDestinations(iSerializerA, iSerializerB, iSerializerAB);
#endif
#ifndef DISABLE_SUPPORT_STL_TEST
    loadSupportStlDestinations(iSerializerA, iSerializerB, iSerializerAB);
#endif
}

//############################################################################
//      変更テストの共通部取り込み
//############################################################################

#include "main.inc"
