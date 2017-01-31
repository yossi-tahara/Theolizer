//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          STLサポート
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

#include "disable_test.h"
#ifndef DISABLE_SUPPORT_STL_TEST

namespace std
{
    template<class _Ty>
    struct default_delete;
}

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
#include "test_support_stl.h"

// 自動生成ソース
#include "test_support_stl.cpp.theolizer.hpp"

//############################################################################
//      使い方の説明
//############################################################################

// ***************************************************************************
//      動作確認
// ***************************************************************************

void tutoriseSupportStl()
{
    std::cout << "tutoriseSupportStl() start" << std::endl;

//----------------------------------------------------------------------------
//      保存
//----------------------------------------------------------------------------

    {
//      ---<<< シリアライズ対象変数定義 >>>---


//      ---<<< 保存処理 >>>---

        std::ofstream   aStream("tutorise_support_stl.json");
        theolizer::JsonOSerializer<> aSerializer(aStream);

        // オーナーとして保存／回復する

        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();
    }

//----------------------------------------------------------------------------
//      回復
//----------------------------------------------------------------------------

    {

//      ---<<< 回復先変数定義 >>>---


//      ---<<< 回復処理 >>>---

        std::ifstream   aStream("tutorise_support_stl.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        // オーナーとして保存／回復する

        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();

//      ---<<< 結果のチェック >>>---

    }

    std::cout << "tutoriseSupportStl() end\n" << std::endl;
}

//############################################################################
//      組み合わせテスト
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

template<class tSerializer>
void saveSupportStl(tSerializer& iSerializer)
{
iSerializer;

//----------------------------------------------------------------------------
//      スマート・ポインタ対応
//----------------------------------------------------------------------------

//      ---<<< 手動(トップ・レベル)による保存 >>>---

    {
        std::cout << "        saveSupportStl() : Manual(Top Level)" << std::endl;

        // 保存情報生成
        std::shared_ptr<SmartBase>  aSharedNull{};
        std::shared_ptr<SmartBase>  aForWeakNull{};
        std::shared_ptr<SmartBase>  aShared{new SmartDerived{100}};
        std::shared_ptr<SmartBase>  aForWeak{new SmartDerived{110}};

        // nullptrチェック用
        SmartTestAuto               aSmartTestAutoNull{0, aSharedNull, aForWeakNull};
        // nullptrへの回復チェック用
        SmartTestAuto               aSmartTestAuto0{120, aShared, aForWeak};
        // 同じ型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto1{130, aShared, aForWeak};
        // 異なる型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto2{140, aShared, aForWeak};

        // 保存
        THEOLIZER_PROCESS(iSerializer, aShared);
        THEOLIZER_PROCESS(iSerializer, aForWeak);
        processSmart(iSerializer, aSmartTestAutoNull);
        processSmart(iSerializer, aSmartTestAuto0);
        processSmart(iSerializer, aSmartTestAuto1);
        processSmart(iSerializer, aSmartTestAuto2);

        iSerializer.clearTracking();
    }

//      ---<<< 自動メンバ・リスト生成による保存 >>>---

    {
        std::cout << "        saveSupportStl() : Auto" << std::endl;

        // 保存情報生成
        std::shared_ptr<SmartBase>  aSharedNull{};
        std::shared_ptr<SmartBase>  aForWeakNull{};
        std::shared_ptr<SmartBase>  aShared{new SmartDerived{200}};
        std::shared_ptr<SmartBase>  aForWeak{new SmartDerived{210}};

        // nullptrチェック用
        SmartTestAuto               aSmartTestAutoNull{0, aSharedNull, aForWeakNull};
        // nullptrへの回復チェック用
        SmartTestAuto               aSmartTestAuto0{220, aShared, aForWeak};
        // 同じ型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto1{230, aShared, aForWeak};
        // 異なる型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto2{240, aShared, aForWeak};

        // 保存
        THEOLIZER_PROCESS(iSerializer, aShared);
        THEOLIZER_PROCESS(iSerializer, aForWeak);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAutoNull);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAuto0);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAuto1);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAuto2);

        iSerializer.clearTracking();
    }

//      ---<<< 手動(非トップ・レベル)による保存 >>>---

    {
        std::cout << "        saveSupportStl() : Manul(Non-Top)" << std::endl;

        // 保存情報生成
        std::shared_ptr<SmartBase>  aSharedNull{};
        std::shared_ptr<SmartBase>  aForWeakNull{};
        std::shared_ptr<SmartBase>  aShared{new SmartDerived{300}};
        std::shared_ptr<SmartBase>  aForWeak{new SmartDerived{310}};

        // nullptrチェック用
        SmartTestManual             aSmartTestManualNull{0, aSharedNull, aForWeakNull};
        // nullptrへの回復チェック用
        SmartTestManual             aSmartTestManual0{320, aShared, aForWeak};
        // 同じ型ポインタへの回復チェック用
        SmartTestManual             aSmartTestManual1{330, aShared, aForWeak};
        // 異なる型ポインタへの回復チェック用
        SmartTestManual             aSmartTestManual2{340, aShared, aForWeak};

        // 保存
        THEOLIZER_PROCESS(iSerializer, aShared);
        THEOLIZER_PROCESS(iSerializer, aForWeak);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManualNull);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManual0);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManual1);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManual2);

        iSerializer.clearTracking();
    }
}

INSTANTIATION_ALL(saveSupportStl);

// ***************************************************************************
//      回復
// ***************************************************************************

template<class tSerializer>
void loadSupportStl(tSerializer& iSerializer)
{
iSerializer;

//----------------------------------------------------------------------------
//      スマート・ポインタ対応
//----------------------------------------------------------------------------

//      ---<<< 手動(トップ・レベル)による回復 >>>---

    {
        std::cout << "        loadSupportStl() : Manual(Top Level)" << std::endl;

        // 回復先生成
        std::shared_ptr<SmartBase>  aSharedNull{};
        std::shared_ptr<SmartBase>  aForWeakNull{};
        std::shared_ptr<SmartBase>  aShared{new SmartDerived{}};
        std::shared_ptr<SmartBase>  aForWeak{new SmartDerived{}};

        // nullptrチェック用
        SmartTestAuto               aSmartTestAutoNull{};
        // nullptrへの回復チェック用
        SmartTestAuto               aSmartTestAuto0{};
        // 同じ型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto1{};
        // 異なる型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto2{};

        // 回復
        THEOLIZER_PROCESS(iSerializer, aShared);
        THEOLIZER_PROCESS(iSerializer, aForWeak);
        processSmart(iSerializer, aSmartTestAutoNull);
        processSmart(iSerializer, aSmartTestAuto0);
        processSmart(iSerializer, aSmartTestAuto1);
        processSmart(iSerializer, aSmartTestAuto2);

        iSerializer.clearTracking();

        // チェック
        aShared.get()->check(100);
        aForWeak.get()->check(110);
        THEOLIZER_EQUAL(aSharedNull, false);
        THEOLIZER_EQUAL(aForWeakNull, false);
        aSmartTestAuto0.check(120, aShared, aForWeak);
        aSmartTestAuto1.check(130, aShared, aForWeak);
        aSmartTestAuto2.check(140, aShared, aForWeak);
    }

//      ---<<< 自動メンバ・リスト生成による回復 >>>---

    {
        std::cout << "        loadSupportStl() : Auto" << std::endl;

        // 回復先生成
        std::shared_ptr<SmartBase>  aSharedNull{};
        std::shared_ptr<SmartBase>  aForWeakNull{};
        std::shared_ptr<SmartBase>  aShared{new SmartDerived{}};
        std::shared_ptr<SmartBase>  aForWeak{new SmartDerived{}};

        // nullptrチェック用
        SmartTestAuto               aSmartTestAutoNull{};
        // nullptrへの回復チェック用
        SmartTestAuto               aSmartTestAuto0{};
        // 同じ型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto1{};
        // 異なる型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto2{};

        // 回復
        THEOLIZER_PROCESS(iSerializer, aShared);
        THEOLIZER_PROCESS(iSerializer, aForWeak);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAutoNull);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAuto0);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAuto1);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAuto2);

        iSerializer.clearTracking();

        // チェック
        aShared.get()->check(200);
        aForWeak.get()->check(210);
        THEOLIZER_EQUAL(aSharedNull, false);
        THEOLIZER_EQUAL(aForWeakNull, false);
        aSmartTestAuto0.check(220, aShared, aForWeak);
        aSmartTestAuto1.check(230, aShared, aForWeak);
        aSmartTestAuto2.check(240, aShared, aForWeak);
    }

//      ---<<< 手動(非トップ・レベル)による回復 >>>---

    {
        std::cout << "        loadSupportStl() : Manul(Non-Top)" << std::endl;

        // 回復先生成
        std::shared_ptr<SmartBase>  aSharedNull{};
        std::shared_ptr<SmartBase>  aForWeakNull{};
        std::shared_ptr<SmartBase>  aShared{new SmartDerived{}};
        std::shared_ptr<SmartBase>  aForWeak{new SmartDerived{}};

        // nullptrチェック用
        SmartTestManual             aSmartTestManualNull{};
        // nullptrへの回復チェック用
        SmartTestManual             aSmartTestManual0{};
        // 同じ型ポインタへの回復チェック用
        SmartTestManual             aSmartTestManual1{};
        // 異なる型ポインタへの回復チェック用
        SmartTestManual             aSmartTestManual2{};

        // 回復
        THEOLIZER_PROCESS(iSerializer, aShared);
        THEOLIZER_PROCESS(iSerializer, aForWeak);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManualNull);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManual0);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManual1);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManual2);

        iSerializer.clearTracking();

        // チェック
        aShared.get()->check(300);
        aForWeak.get()->check(310);
        THEOLIZER_EQUAL(aSharedNull, false);
        THEOLIZER_EQUAL(aForWeakNull, false);
        aSmartTestManual0.check(320, aShared, aForWeak);
        aSmartTestManual1.check(330, aShared, aForWeak);
        aSmartTestManual2.check(340, aShared, aForWeak);
    }
}

INSTANTIATION_ALL(loadSupportStl);

#endif  // DISABLE_SUPPORT_STL_TEST
