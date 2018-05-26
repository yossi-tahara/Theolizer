//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          クラス変更の複合テスト
//              オブジェクト追跡
//              バージョン・アップ／ダウン処理（参照テスト含む）
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
#include <theolizer/serializer_xml.h>
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
//      エラー・リカバリ
// ***************************************************************************

template<class tSerializerSave, class tSerializerLoad>
void testModifyComplex(unsigned iGlobalVersionNo, theolizer::CheckMode iCheckMode)
{
    std::cout << "testModifyComplex(" << iGlobalVersionNo << ", " << iCheckMode << ");\n";

    std::stringstream aStream;

//----------------------------------------------------------------------------
//      データ生成
//----------------------------------------------------------------------------

    {
        tSerializerSave aSerializer(aStream, iGlobalVersionNo, iCheckMode);

        TestRecovery    aTestRecovery{true};
        THEOLIZER_PROCESS(aSerializer, aTestRecovery);
    }

    if (tSerializerSave::hasProperty(theolizer::Property::EncodedString))
    {
        std::cout << aStream.str() << std::endl;
    }

//----------------------------------------------------------------------------
//      エラー発生とリカバリ（例外禁止）
//----------------------------------------------------------------------------

    for (std::size_t i=1; i < aStream.str().size()-1; ++i)
    {
        // エラー発生
        std::stringstream ss(aStream.str().substr(0, i));
        bool aDoConstructing=true;
        std::streampos  aPos=0;

        tSerializerLoad aSerializer(ss, true);
        if (!aSerializer.isError())
        {
            aDoConstructing=false;
            aPos=ss.tellg();            // リカバリ時の同期位置保存

            TestRecovery    aTestRecovery;
            THEOLIZER_PROCESS(aSerializer, aTestRecovery);
        }

        // エラーが発生していない場合、FAIL
        THEOLIZER_CHECK(aSerializer.isError(), "No error");

        // リカバリ
        if (aDoConstructing)    // コンストラクト中のリカバリは不可能
    continue;

        ss.clear();
        ss.str(aStream.str());
        ss.seekg(aPos);
        aSerializer.resetError();

        TestRecovery    aTestRecovery;
        THEOLIZER_PROCESS(aSerializer, aTestRecovery);
        aTestRecovery.check(iGlobalVersionNo);
    }

//----------------------------------------------------------------------------
//      エラー発生とリカバリ（例外許可）
//----------------------------------------------------------------------------

    for (std::size_t i=1; i < aStream.str().size()-1; ++i)
    {
        // コンストラクト中のエラー・キャッチ
        try
        {
            std::stringstream ss(aStream.str().substr(0, i));
            std::streampos  aPos=0;
            tSerializerLoad aSerializer(ss);
            aPos=ss.tellg();            // リカバリ時の同期位置保存

            // 回復処理中のエラー・キャッチ
            try
            {
                TestRecovery    aTestRecovery;
                THEOLIZER_PROCESS(aSerializer, aTestRecovery);

                // エラーが発生していない場合、FAIL
                THEOLIZER_CHECK(aSerializer.isError(), "No error");
            }
            catch (theolizer::ErrorInfo&)
            {
                // リカバリ
                ss.clear();
                ss.str(aStream.str());
                ss.seekg(aPos);
                aSerializer.resetError();

                TestRecovery    aTestRecovery;
                THEOLIZER_PROCESS(aSerializer, aTestRecovery);
                aTestRecovery.check(iGlobalVersionNo);
            }
        }
        catch (theolizer::ErrorInfo&)
        {
        }
    }
}

//----------------------------------------------------------------------------
//      複数のシリアライザにて検証テスト呼び出し
//----------------------------------------------------------------------------

void tutoriseModifyComplex()
{
    std::cout << "tutoriseModifyComplex() start" << std::endl;

//      ---<<< Json-ver1c >>>---

    testModifyComplex
    <
        theolizer::JsonOSerializer<theolizerD::Master>,
        theolizer::JsonISerializer<theolizerD::Master>
    >(1, theolizer::CheckMode::NoTypeCheck);

    testModifyComplex
    <
        theolizer::JsonOSerializer<theolizerD::Master>,
        theolizer::JsonISerializer<theolizerD::Master>
    >(1, theolizer::CheckMode::TypeCheckByIndex);

    testModifyComplex
    <
        theolizer::JsonOSerializer<theolizerD::Master>,
        theolizer::JsonISerializer<theolizerD::Master>
    >(1, theolizer::CheckMode::MetaMode);

//      ---<<< Json-ver2a >>>---

    testModifyComplex
    <
        theolizer::JsonOSerializer<theolizerD::Master>,
        theolizer::JsonISerializer<theolizerD::Master>
    >(2, theolizer::CheckMode::NoTypeCheck);

    testModifyComplex
    <
        theolizer::JsonOSerializer<theolizerD::Master>,
        theolizer::JsonISerializer<theolizerD::Master>
    >(2, theolizer::CheckMode::TypeCheckByIndex);

    testModifyComplex
    <
        theolizer::JsonOSerializer<theolizerD::Master>,
        theolizer::JsonISerializer<theolizerD::Master>
    >(2, theolizer::CheckMode::MetaMode);

//      ---<<< Binary-ver1c >>>---

    testModifyComplex
    <
        theolizer::BinaryOSerializer<theolizerD::Master>,
        theolizer::BinaryISerializer<theolizerD::Master>
    >(1, theolizer::CheckMode::NoTypeCheck);

    testModifyComplex
    <
        theolizer::BinaryOSerializer<theolizerD::Master>,
        theolizer::BinaryISerializer<theolizerD::Master>
    >(1, theolizer::CheckMode::TypeCheckByIndex);

    testModifyComplex
    <
        theolizer::BinaryOSerializer<theolizerD::Master>,
        theolizer::BinaryISerializer<theolizerD::Master>
    >(1, theolizer::CheckMode::MetaMode);

//      ---<<< Binary-ver2a >>>---

    testModifyComplex
    <
        theolizer::BinaryOSerializer<theolizerD::Master>,
        theolizer::BinaryISerializer<theolizerD::Master>
    >(2, theolizer::CheckMode::NoTypeCheck);

    testModifyComplex
    <
        theolizer::BinaryOSerializer<theolizerD::Master>,
        theolizer::BinaryISerializer<theolizerD::Master>
    >(2, theolizer::CheckMode::TypeCheckByIndex);

    testModifyComplex
    <
        theolizer::BinaryOSerializer<theolizerD::Master>,
        theolizer::BinaryISerializer<theolizerD::Master>
    >(2, theolizer::CheckMode::MetaMode);

    std::cout << "tutoriseModifyComplex() end" << std::endl;
}

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
