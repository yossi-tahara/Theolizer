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
// ---------------------------------------------------------------------------
//      単独テスト
// ---------------------------------------------------------------------------

//      ---<<< 非侵入型完全自動 >>>---

    {
        FullAuto    aFullAuto{true};
        std::cout << "        saveClassVariation() : aFullAuto" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aFullAuto);
    }

//      ---<<< 侵入型半自動－名前対応 >>>---

    {
        HalfAutoName    aHalfAutoName{true};
        std::cout << "        saveClassVariation() : aHalfAutoName" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aHalfAutoName);
    }

//      ---<<< 侵入型半自動－順序対応 >>>---

    {
        HalfAutoOrder   aHalfAutoOrder{true};
        std::cout << "        saveClassVariation() : aHalfAutoOrder" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aHalfAutoOrder);
    }

//      ---<<< 非侵入型手動 >>>---

    {
        Manual    aManual{true};
        std::cout << "        saveClassVariation() : aManual" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aManual);
    }

// ---------------------------------------------------------------------------
//      ２重組み合わせテスト
// ---------------------------------------------------------------------------

//      ---<<< 非侵入型完全自動 >>>---

    {
        DerivedFullAuto aDerivedFullAuto{true};
        std::cout << "        saveClassVariation() : aDerivedFullAuto" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aDerivedFullAuto);
    }

//      ---<<< 侵入型半自動 >>>---

    {
        DerivedHalfAuto aDerivedHalfAuto{true};
        std::cout << "        saveClassVariation() : aDerivedHalfAuto" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aDerivedHalfAuto);
    }

//      ---<<< 非侵入型手動 >>>---

    {
        DerivedManual   aDerivedManual{true};
        std::cout << "        saveClassVariation() : aDerivedManual" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aDerivedManual);
    }
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
// ---------------------------------------------------------------------------
//      単独テスト
// ---------------------------------------------------------------------------

//      ---<<< 非侵入型完全自動 >>>---

    {
        FullAuto    aFullAuto{};
        std::cout << "        loadClassVariation() : aFullAuto" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aFullAuto);
        aFullAuto.checkPrivate();
        aFullAuto.checkProtected(true);
        aFullAuto.checkPublic(true);
    }

//      ---<<< 侵入型半自動－名前対応 >>>---

    {
        HalfAutoName    aHalfAutoName{};
        std::cout << "        loadClassVariation() : aHalfAutoName" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aHalfAutoName);
        aHalfAutoName.checkPrivate(true);
        aHalfAutoName.checkProtected(true);
        aHalfAutoName.checkPublic(true);
    }

//      ---<<< 侵入型半自動－順序対応 >>>---

    {
        HalfAutoOrder   aHalfAutoOrder{true};
        std::cout << "        loadClassVariation() : aHalfAutoOrder" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aHalfAutoOrder);
        aHalfAutoOrder.checkPrivate(true);
        aHalfAutoOrder.checkProtected(true);
        aHalfAutoOrder.checkPublic(true);
    }

//      ---<<< 非侵入型手動 >>>---

    {
        Manual      aManual{};
        std::cout << "        loadClassVariation() : aManual" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aManual);
        aManual.checkPublic(true);
    }

// ---------------------------------------------------------------------------
//      ２重組み合わせテスト
// ---------------------------------------------------------------------------

//      ---<<< 非侵入型完全自動 >>>---

    {
        DerivedFullAuto aDerivedFullAuto{};
        std::cout << "        loadClassVariation() : aDerivedFullAuto" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aDerivedFullAuto);
        aDerivedFullAuto.check();
    }

//      ---<<< 侵入型半自動 >>>---

    {
        DerivedHalfAuto aDerivedHalfAuto{};
        std::cout << "        loadClassVariation() : aDerivedHalfAuto" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aDerivedHalfAuto);
        aDerivedHalfAuto.check();
    }

//      ---<<< 非侵入型手動 >>>---

    {
        DerivedManual   aDerivedManual{};
        std::cout << "        loadClassVariation() : aDerivedManual" << std::endl;
        THEOLIZER_PROCESS(iSerializer, aDerivedManual);
        aDerivedManual.check();
    }
}
//! [loadClassVariation]

INSTANTIATION_ALL(void, loadClassVariation);
