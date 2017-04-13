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
//#include <sstream>
#include <limits>

// theolizerライブラリ(グローバル・バージョン番号テーブル生成のためにも必要)
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_fast.h>
#include <theolizer/serializer_json.h>

// プロジェクト・ヘッダ
#include "common.h"

// グローバル・バージョン番号テーブル生成
#define THEOLIZER_GLOBAL_VERSION_TABLE
#include "test_modify_enum.h"
#include "test_modify_class.h"
#include "test_modify_complex.h"

// 自動生成ソース
#include "main.cpp.theolizer.hpp"

//############################################################################
//      テスト基本部
//############################################################################

// ***************************************************************************
//      各種テスト関数の宣言
// ***************************************************************************

//----------------------------------------------------------------------------
//      enum型変更テスト
//----------------------------------------------------------------------------

//      ---<<< 使い方のサンプル・コード >>>---

//void tutoriseModifyEnum();

//      ---<<< 自動テスト >>>---

template<class tSerializer>
void saveModifyEnum(tSerializer& iSerializer);

template<class tSerializer>
void loadModifyEnum(tSerializer& iSerializer);

//----------------------------------------------------------------------------
//      クラス変更テスト
//----------------------------------------------------------------------------

//      ---<<< 使い方のサンプル・コード >>>---

//void tutoriseModifyClass();

//      ---<<< 自動テスト >>>---

template<class tSerializer>
void saveModifyClass(tSerializer& iSerializer);

template<class tSerializer>
void loadModifyClass(tSerializer& iSerializer);

//----------------------------------------------------------------------------
//      クラス変更の複合テスト
//----------------------------------------------------------------------------

//      ---<<< 使い方のサンプル・コード >>>---

//void tutoriseModifyComplex();

//      ---<<< 自動テスト >>>---

template<class tSerializer>
void saveModifyComplex(tSerializer& iSerializer);

template<class tSerializer>
void loadModifyComplex(tSerializer& iSerializer);

//      ---<<< 保存先指定自動テスト >>>---

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

// ***************************************************************************
//      各テスト呼び出し
// ***************************************************************************

//----------------------------------------------------------------------------
//      使い方説明サンプル呼び出し
//----------------------------------------------------------------------------

void callTutorial()
{
    std::cout << "    callTutorial();\n";

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
    std::cout << "    saveBasic();\n";

#ifndef DISABLE_MODIFY_ENUM_TEST
    saveModifyEnum(iSerializer);
#endif

#ifndef DISABLE_MODIFY_CLASS_TEST
    saveModifyClass(iSerializer);
#endif

#ifndef DISABLE_MODIFY_COMPLEX_TEST
    saveModifyComplex(iSerializer);
#endif
}

template<class tSerializer>
void loadBasic(tSerializer& iSerializer)
{
    std::cout << "    loadBasic();\n";

#ifndef DISABLE_MODIFY_ENUM_TEST
    loadModifyEnum(iSerializer);
#endif

#ifndef DISABLE_MODIFY_CLASS_TEST
    loadModifyClass(iSerializer);
#endif

#ifndef DISABLE_MODIFY_COMPLEX_TEST
    loadModifyComplex(iSerializer);
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
    std::cout << "    callSaveDestinations();\n";

#ifndef DISABLE_MODIFY_COMPLEX_TEST
    saveDestinations(iSerializerA, iSerializerB, iSerializerAB);
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
    std::cout << "    callLoadDestinations();\n";

#ifndef DISABLE_MODIFY_COMPLEX_TEST
    loadDestinations(iSerializerA, iSerializerB, iSerializerAB);
#endif
}

//############################################################################
//      変更テストの共通部取り込み
//############################################################################

#include "main.inc"
