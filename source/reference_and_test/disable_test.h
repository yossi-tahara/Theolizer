//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          テストをディセーブルするための定義
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

#if !defined(DISABLE_TEST_H)
#define DISABLE_TEST_H

// ***************************************************************************
//      basicテストの有効／無効
// ***************************************************************************

#define DISABLE_BASIC_PROCESS_TEST
#define DISABLE_CLASS_VARIATION_TEST
#define DISABLE_ENUM_VARIATION_TEST
#define DISABLE_OBJECT_TRACKING_TEST
#define DISABLE_OBJECT_TRACKING2_TEST
#define DISABLE_OBJECT_TRACKING3_TEST
#define DISABLE_POLYMORPHISM_TEST
#define DISABLE_DESTINATIONS_TEST
#define DISABLE_SUPPORT_STL_TEST

// ***************************************************************************
//      変更テストの有効／無効
// ***************************************************************************

#define DISABLE_MODIFY_ENUM_TEST
//#define DISABLE_MODIFY_CLASS_TEST
//#define DISABLE_MODIFY_CLASS_TEST_NAME
#define DISABLE_MODIFY_CLASS_TEST_ORDER
#define DISABLE_MODIFY_CLASS_TEST_ARRAY

#endif  // DISABLE_TEST_H
