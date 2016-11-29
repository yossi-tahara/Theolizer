//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          classの様々なバリエーション
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
#include <string>

// theolizerライブラリ
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_json.h>

// プロジェクト・ヘッダ
#include "common.h"
#include "test_class_variation.h"

// 自動生成ソース
#include "test_class_variation.cpp.theolizer.hpp"

//############################################################################
//      プリミティブ、enum型、クラス、配列
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

//! [saveClassVariation]
template<class tSerializer>
void saveClassVariation(tSerializer& iSerializer)
{

//      ---<<< 非侵入型完全自動（基底クラス） >>>---

    BaseFullAuto    aBaseFullAuto{true};
    std::cout << "        saveClassVariation() : aBaseFullAuto\n";
    THEOLIZER_PROCESS(iSerializer, aBaseFullAuto);

//      ---<<< 侵入型半自動（基底クラス） >>>---

    BaseHalfAuto    aBaseHalfAuto{true};
    std::cout << "        saveClassVariation() : aBaseHalfAuto\n";
    THEOLIZER_PROCESS(iSerializer, aBaseHalfAuto);

//      ---<<< 非侵入型手動（基底クラス） >>>---

    BaseManual    aBaseManual{true};
    std::cout << "        saveClassVariation() : aBaseManual\n";
    THEOLIZER_PROCESS(iSerializer, aBaseManual);
}
//! [saveClassVariation]

INSTANTIATION_ALL(void, saveClassVariation);

// ***************************************************************************
//      回復
// ***************************************************************************

//! [loadClassVariation]
template<class tSerializer>
void loadClassVariation(tSerializer& iSerializer)
{

//      ---<<< 非侵入型完全自動（基底クラス） >>>---

    BaseFullAuto    aBaseFullAuto{};
    std::cout << "        loadClassVariation() : aBaseFullAuto\n";
    THEOLIZER_PROCESS(iSerializer, aBaseFullAuto);
    aBaseFullAuto.checkPrivate();
    aBaseFullAuto.checkProtected(true);
    aBaseFullAuto.checkPublic(true);

//      ---<<< 侵入型半自動（基底クラス） >>>---

    BaseHalfAuto    aBaseHalfAuto{};
    std::cout << "        loadClassVariation() : aBaseHalfAuto\n";
    THEOLIZER_PROCESS(iSerializer, aBaseHalfAuto);
    aBaseHalfAuto.checkPrivate(true);
    aBaseHalfAuto.checkProtected(true);
    aBaseHalfAuto.checkPublic(true);

//      ---<<< 非侵入型手動（基底クラス） >>>---

    BaseManual    aBaseManual{};
    std::cout << "        loadClassVariation() : aBaseManual\n";
    THEOLIZER_PROCESS(iSerializer, aBaseManual);
    aBaseManual.checkPublic(true);
}
//! [loadClassVariation]

INSTANTIATION_ALL(void, loadClassVariation);
