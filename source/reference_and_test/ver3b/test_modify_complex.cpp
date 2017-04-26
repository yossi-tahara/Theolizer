//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          クラス変更の複合テスト
//              オブジェクト追跡
//              バージョン・アップ／ダウン処理（参照テスト含む）
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
#ifdef ENABLE_MODIFY_COMPLEX_TEST

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
#include <string>

// theolizerライブラリ
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_fast.h>
#include <theolizer/serializer_json.h>
#include <theolizer/memory.h>

// プロジェクト・ヘッダ
#include "common.h"
#include "test_modify_complex.h"

// 自動生成ソース
#include "test_modify_complex.cpp.theolizer.hpp"

//############################################################################
//      組み合わせテスト
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

//! [saveModifyComplex]
template<class tSerializer>
void saveModifyComplex(tSerializer& iSerializer)
{
    std::cout << "    saveModifyComplex();\n";

//----------------------------------------------------------------------------
//      トップ・レベルの追加(ver2にて追加)
//----------------------------------------------------------------------------

    if (2 <= iSerializer.getGlobalVersionNo())
    {
        int     aAdditionalData=123;
        THEOLIZER_PROCESS(iSerializer, aAdditionalData);
    }

//----------------------------------------------------------------------------
//      オブジェクト追跡
//----------------------------------------------------------------------------

    {
        PointeeClass    aPointeeClass;
        PointeeInclude  aPointeeInclude(aPointeeClass, true);
        PointerInclude  aPointerInclude(aPointeeInclude);

        THEOLIZER_PROCESS(iSerializer, aPointeeClass);
        THEOLIZER_PROCESS(iSerializer, aPointeeInclude);
        THEOLIZER_PROCESS(iSerializer, aPointerInclude);

        iSerializer.clearTracking();
    }

//----------------------------------------------------------------------------
//      Keep-stepとNon-Keep-step
//----------------------------------------------------------------------------

    {
        VersionUpDownTest   aVersionUpDownTest(ClassKind::Kind1);
        KeepStepTest        aKeepStepTest(aVersionUpDownTest, true);
        THEOLIZER_PROCESS_POINTEE(iSerializer, aVersionUpDownTest);
        THEOLIZER_PROCESS(iSerializer, aKeepStepTest);

        iSerializer.clearTracking();
    }
}
//! [saveModifyComplex]

INSTANTIATION_ALL(saveModifyComplex);

// ***************************************************************************
//      回復
// ***************************************************************************

//! [loadModifyComplex]
template<class tSerializer>
void loadModifyComplex(tSerializer& iSerializer)
{
//theolizer::DisplayPass aDisplayPass;
    std::cout << "    loadModifyComplex();\n";

//----------------------------------------------------------------------------
//      トップ・レベルの追加(ver2にて追加)
//----------------------------------------------------------------------------

    if (2 <= iSerializer.getGlobalVersionNo())
    {
        int     aAdditionalData=0;
        THEOLIZER_PROCESS(iSerializer, aAdditionalData);
        THEOLIZER_EQUAL(aAdditionalData, 123);
    }

//----------------------------------------------------------------------------
//      オブジェクト追跡
//----------------------------------------------------------------------------

    {
//theolizer::DisplayPass aDisplayPass;
        PointeeClass    aPointeeClass;
        PointeeInclude  aPointeeInclude(aPointeeClass);
        PointerInclude  aPointerInclude;

        THEOLIZER_PROCESS(iSerializer, aPointeeClass);
        THEOLIZER_PROCESS(iSerializer, aPointeeInclude);
        THEOLIZER_PROCESS(iSerializer, aPointerInclude);

        aPointeeInclude.check();
        aPointerInclude.check(aPointeeInclude);

        iSerializer.clearTracking();
    }

//----------------------------------------------------------------------------
//      Keep-stepとNon-Keep-step
//----------------------------------------------------------------------------

    {
        VersionUpDownTest   aVersionUpDownTest;
        KeepStepTest        aKeepStepTest(aVersionUpDownTest);
        THEOLIZER_PROCESS_POINTEE(iSerializer, aVersionUpDownTest);
        THEOLIZER_PROCESS(iSerializer, aKeepStepTest);
        aVersionUpDownTest.check(ClassKind::Kind1);
        aKeepStepTest.check(aVersionUpDownTest);

        iSerializer.clearTracking();
    }
}
//! [loadModifyComplex]

INSTANTIATION_ALL(loadModifyComplex);

//############################################################################
//      保存先のテスト
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void saveDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB& iSerializerAB
)
{
    std::cout << "saveDestinations()" << std::endl;

//----------------------------------------------------------------------------
//      Keep-stepとNon-Keep-step
//----------------------------------------------------------------------------

    {
        VersionUpDownTest   aVersionUpDownTest(ClassKind::Kind1);
        KeepStepTest        aKeepStepTest(aVersionUpDownTest, true);
        THEOLIZER_PROCESS_POINTEE(iSerializerA, aVersionUpDownTest);
        THEOLIZER_PROCESS(iSerializerA, aKeepStepTest);
        iSerializerA.clearTracking();

        THEOLIZER_PROCESS_POINTEE(iSerializerB, aVersionUpDownTest);
        THEOLIZER_PROCESS(iSerializerB, aKeepStepTest);
        iSerializerB.clearTracking();
    }
}

INSTANTIATION_DESTINATIONS(saveDestinations);

// ***************************************************************************
//      回復
// ***************************************************************************

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void loadDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB& iSerializerAB
)
{
    std::cout << "loadDestinations()" << std::endl;

//----------------------------------------------------------------------------
//      Keep-stepとNon-Keep-step
//----------------------------------------------------------------------------

    {
        VersionUpDownTest   aVersionUpDownTest;
        KeepStepTest        aKeepStepTest(aVersionUpDownTest);
        THEOLIZER_PROCESS_POINTEE(iSerializerA, aVersionUpDownTest);
        THEOLIZER_PROCESS(iSerializerA, aKeepStepTest);
        iSerializerA.clearTracking();

        THEOLIZER_PROCESS_POINTEE(iSerializerB, aVersionUpDownTest);
        THEOLIZER_PROCESS(iSerializerB, aKeepStepTest);
        iSerializerB.clearTracking();

        aVersionUpDownTest.check(ClassKind::Kind1);
        aKeepStepTest.check(aVersionUpDownTest);
    }
}

INSTANTIATION_DESTINATIONS(loadDestinations);

#endif  // ENABLE_MODIFY_COMPLEX_TEST
