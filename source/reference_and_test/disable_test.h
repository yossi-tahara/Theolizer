//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          テストをディセーブルするための定義
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

#if !defined(DISABLE_TEST_H)
#define DISABLE_TEST_H

// ***************************************************************************
//      一部機能のディセーブル制御
// ***************************************************************************

#if defined(ERROR1) || defined(ERROR2)
    #define BUILD_ERROR_TEST
#endif

// ***************************************************************************
//      テストの有効／無効
// ***************************************************************************

#ifndef BUILD_ERROR_TEST
  #if 1
    #define ENABLE_XML_BINARY_TEST

    #define ENABLE_BASIC_PROCESS_TEST
    #define ENABLE_CLASS_VARIATION_TEST
        #define ENABLE_SINGLE_TEST
        #define ENABLE_COMBINATION_TEST
        #define ENABLE_TRIPLED_TEST
    #define ENABLE_ENUM_VARIATION_TEST
    #define ENABLE_OBJECT_TRACKING_TEST
    #define ENABLE_OBJECT_TRACKING2_TEST
    #define ENABLE_OBJECT_TRACKING3_TEST
    #define ENABLE_POLYMORPHISM_TEST
    #define ENABLE_DESTINATIONS_TEST
    #define ENABLE_SUPPORT_STL_TEST

    #define ENABLE_MODIFY_ENUM_TEST
    #define ENABLE_MODIFY_CLASS_TEST
        #define ENABLE_MODIFY_CLASS_TEST_NAME
        #define ENABLE_MODIFY_CLASS_TEST_ORDER
        #define ENABLE_MODIFY_CLASS_TEST_ARRAY
  #endif
#endif

#define ENABLE_MODIFY_COMPLEX_TEST

#endif  // DISABLE_TEST_H
