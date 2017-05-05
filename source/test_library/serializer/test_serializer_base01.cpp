//############################################################################
//      各serializerのテスト
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

#define THEOLIZER_INTERNAL_EXCLUDE_VERSION_H

//############################################################################
//      通常のインクルード
//############################################################################

#include <sstream>
#include <fstream>
#include <typeindex>
using namespace std;

#include <theolizer/test_tool.h>

#ifdef _MSC_VER
    #pragma warning(disable:4100 4127)
#endif

#include "test_serializer_base.h"
#include "test_serializer_base_version.h"
#include "test_serializer_base01.cpp.theolizer.hpp"

//############################################################################
//      テストのメイン関数
//############################################################################

void TestNormalMain();
void TestNormalMain2();
void TestTrackingMain();
void TestString(char const* iFileName);
void TestDestinationDirectSave(char const* iFileName);
void TestDestinationDirectLoad(char const* iFileName);
template
<
    theolizer::Destination    uDefault=theolizerD::All,
    theolizer::Destination... uDestinations
>
void TestDestinationParameter(char const* iFileName);

void TestEof(const theolizer::CheckMode iCheckMode);

// ***************************************************************************
//      メイン
// ***************************************************************************

int main(int argc, char** argv)
{
    int ret=0;
//  theolizer::DisplayPass aDisplayPass;

    try
    {

// ***************************************************************************
//      メタ・シリアライズ
// ***************************************************************************

#ifdef THEOLIZER_INTERNAL_ENABLE_META_SERIALIZER
        {
            std::ofstream   aStream("test_meta_data.log");
            theolizer::JsonOSerializer<>   aSerializer(aStream, theolizer::CheckMode::MetaMode);
        }
#endif  // THEOLIZER_INTERNAL_ENABLE_META_SERIALIZER

// ***************************************************************************
//      基本的テスト
// ***************************************************************************

        TestNormalMain();
        TestNormalMain2();
        TestTrackingMain();

// ***************************************************************************
//      文字列テスト
// ***************************************************************************

        TestString("test_string.log");

// ***************************************************************************
//      保存先指定テスト(直接指定)
// ***************************************************************************

        TestDestinationDirectSave("test_destination_Machine.log");
        TestDestinationDirectLoad("test_destination_Machine.log");

// ***************************************************************************
//      保存先指定テスト(パラメータ指定)
// ***************************************************************************

        TestDestinationParameter<theolizerD::User>("test_destination_User.log");

// ***************************************************************************
//      fastシリアライザの特別テスト
// ***************************************************************************

// ---------------------------------------------------------------------------
//      未保存アドレスの回復
// ---------------------------------------------------------------------------

//      ---<<< シリアライズ >>>---

        Tracking   aTracking;
        {
            std::ofstream   aStream("test_fast_cant_track.log", std::ios::binary);
            theolizer::FastOSerializer<>    aSerializer(aStream);

            Pointers    aPointers;
            aPointers.mUInt             =&aTracking.mUInt;
            aPointers.mEnum             =&aTracking.mEnum;
            aPointers.mUInt             =&aTracking.mUInt;
            aPointers.mIncluded2        =&aTracking.mIncluded2;
            aPointers.mString           =&aTracking.mString;
            THEOLIZER_PROCESS(aSerializer, aPointers);

            // ポインタ保存
            aSerializer.clearTracking();
        }

//      ---<<< デシリアライズ >>>---

        {
            std::ifstream   aStream("test_fast_cant_track.log", std::ios::binary);
            theolizer::FastISerializer<>    aSerializer(aStream);

            Pointers    aPointers;
            THEOLIZER_PROCESS(aSerializer, aPointers);

            // ポインタ回復
            aSerializer.clearTracking();

            THEOLIZER_EQUAL(aPointers.mUInt,      &aTracking.mUInt);
            THEOLIZER_EQUAL(aPointers.mEnum,      &aTracking.mEnum);
            THEOLIZER_EQUAL(aPointers.mUInt,      &aTracking.mUInt);
            THEOLIZER_EQUAL(aPointers.mIncluded2, &aTracking.mIncluded2);
            THEOLIZER_EQUAL(aPointers.mString,    &aTracking.mString);
        }

// ---------------------------------------------------------------------------
//      コピーツールのテスト
// ---------------------------------------------------------------------------

        {
            IntrusiveBase   aDestination;

            {
                IntrusiveBase   aSource;
                aSource.setInt(101);
                aSource.mShort=102;
                aSource.mString=u8"103)テスト";
                theolizer::copySerializable(aSource, aDestination);
            }

            THEOLIZER_EQUAL(aDestination.getInt(), 101);
            THEOLIZER_EQUAL(aDestination.mShort, 102);
            THEOLIZER_EQUAL(aDestination.mString, u8"103)テスト");
        }

// ***************************************************************************
//      定義変更テスト
// ***************************************************************************

// ---------------------------------------------------------------------------
//      シリアライズ
// ---------------------------------------------------------------------------

        {
            std::ofstream   aStream("test_change_same_version.log");
            theolizer::JsonOSerializer<>   aSerializer(aStream);

//      ---<<< 定義変更テスト(EnumType) >>>---

            THEOLIZER_PROCESS(aSerializer, another::EnumTest::TWO);

//      ---<<< 定義変更テスト(ClassType) >>>---

            DerivedChange  aDerivedChange;

            // IntrusiveBase
            aDerivedChange.mShort  =1001;
            aDerivedChange.setInt(1002);
            aDerivedChange.mString =u8"1003)テスト";
            // IntrusiveBase2
            aDerivedChange.mLong       =1101;
            aDerivedChange.mLongLong   =1102;
            aDerivedChange.mULong      =1103;
            aDerivedChange.mULongLong  =1104;

            // DerivedChange
            aDerivedChange.mUInt       =1201;
            aDerivedChange.mUShort     =1202;
            // DerivedChange.IntrusiveBase
            aDerivedChange.mIntrusiveBase.mShort   =1301;
            aDerivedChange.mIntrusiveBase.setInt(1302);
            aDerivedChange.mIntrusiveBase.mString  =u8"1303)テスト";
            // DerivedChange.IntrusiveBase2
            aDerivedChange.mIntrusiveBase2.mLong       =1401;
            aDerivedChange.mIntrusiveBase2.mLongLong   =1402;
            aDerivedChange.mIntrusiveBase2.mULong      =1403;
            aDerivedChange.mIntrusiveBase2.mULongLong  =1404;

            // 追加／順序変更テスト用
            THEOLIZER_PROCESS(aSerializer, aDerivedChange);

            // 削除テスト用
            THEOLIZER_PROCESS(aSerializer, aDerivedChange);
    }

// ---------------------------------------------------------------------------
//      デシリアライズ
// ---------------------------------------------------------------------------

        {
            std::ifstream   aStream("test_change_same_version.log");
            theolizer::JsonISerializer<>   aSerializer(aStream);

//      ---<<< 定義変更テスト(EnumType) >>>---

            EnumTest aEnum=EnumTest::zero;
            THEOLIZER_PROCESS(aSerializer, aEnum);
            THEOLIZER_EQUAL(aEnum, EnumTest::two);

//      ---<<< 定義変更テスト(ClassTypeの追加／順序変更) >>>---

            change_order::dummy::DerivedChange     aDerivedChgOrder;
            aDerivedChgOrder.mUChar=1;
            THEOLIZER_PROCESS(aSerializer, aDerivedChgOrder);

            // IntrusiveBase
            THEOLIZER_EQUAL(aDerivedChgOrder.mShort, 1001);
            THEOLIZER_EQUAL(aDerivedChgOrder.getInt(), 1002);
            THEOLIZER_EQUAL(aDerivedChgOrder.mString, u8"1003)テスト");
            // IntrusiveBase2
            THEOLIZER_EQUAL(aDerivedChgOrder.mLong, 1101);
            THEOLIZER_EQUAL(aDerivedChgOrder.mLongLong, 1102);
            THEOLIZER_EQUAL(aDerivedChgOrder.mULong, 1103);
            THEOLIZER_EQUAL(aDerivedChgOrder.mULongLong, 1104);

            // DerivedChange
            THEOLIZER_EQUAL(aDerivedChgOrder.mUInt, 1201);
            THEOLIZER_EQUAL(aDerivedChgOrder.mUShort, 1202);
            THEOLIZER_EQUAL(aDerivedChgOrder.mUChar, 1);

            // DerivedChange.IntrusiveBase
            THEOLIZER_EQUAL(aDerivedChgOrder.mIntrusiveBase.mShort, 1301);
            THEOLIZER_EQUAL(aDerivedChgOrder.mIntrusiveBase.getInt(), 1302);
            THEOLIZER_EQUAL(aDerivedChgOrder.mIntrusiveBase.mString, u8"1303)テスト");
            // DerivedChange.IntrusiveBase2
            THEOLIZER_EQUAL(aDerivedChgOrder.mIntrusiveBase2.mLong, 1401);
            THEOLIZER_EQUAL(aDerivedChgOrder.mIntrusiveBase2.mLongLong, 1402);
            THEOLIZER_EQUAL(aDerivedChgOrder.mIntrusiveBase2.mULong, 1403);
            THEOLIZER_EQUAL(aDerivedChgOrder.mIntrusiveBase2.mULongLong, 1404);

//      ---<<< 定義変更テスト(ClassTypeの削除) >>>---

            DerivedDelete        aDerivedDelete;
            THEOLIZER_PROCESS(aSerializer, aDerivedDelete);

            // IntrusiveBase
            THEOLIZER_EQUAL(aDerivedDelete.mShort, 1001);
            THEOLIZER_EQUAL(aDerivedDelete.getInt(), 1002);
            THEOLIZER_EQUAL(aDerivedDelete.mString, u8"1003)テスト");
            // DerivedChange
            THEOLIZER_EQUAL(aDerivedDelete.mUInt, 1201);
            // DerivedChange.IntrusiveBase2
            THEOLIZER_EQUAL(aDerivedDelete.mIntrusiveBase2.mLong, 1401);
            THEOLIZER_EQUAL(aDerivedDelete.mIntrusiveBase2.mLongLong, 1402);
            THEOLIZER_EQUAL(aDerivedDelete.mIntrusiveBase2.mULong, 1403);
            THEOLIZER_EQUAL(aDerivedDelete.mIntrusiveBase2.mULongLong, 1404);
        }

// ***************************************************************************
//      トップ・レベルの型チェック・エラー・テスト
// ***************************************************************************

        u8string aMessage(u8"Unmatch type.");

// ---------------------------------------------------------------------------
//      TypeCheck
// ---------------------------------------------------------------------------

//      ---<<< シリアライズ >>>---

        {
            std::ofstream   aStream("test_change_check0.log");
            theolizer::JsonOSerializer<theolizerD::SpecialDocument>
                aSerializer(aStream, theolizer::CheckMode::TypeCheck);

            int             aInt=101;
            THEOLIZER_PROCESS(aSerializer, aInt);
        }

//      ---<<< デシリアライズ(例外有り) >>>---

        {
            std::ifstream   aStream("test_change_check0.log");
            theolizer::JsonISerializer<>   aSerializer(aStream);

            unsigned int    aUInt;
            THEOLIZER_CHECK_EXCEPTION(
                THEOLIZER_PROCESS(aSerializer, aUInt); ,
                theolizer::ErrorInfo);
        }

//      ---<<< デシリアライズ(例外無し) >>>---

        {
            std::ifstream   aStream("test_change_check0.log");
            theolizer::JsonISerializer<>   aSerializer(aStream, true);

            unsigned int    aUInt;
            THEOLIZER_PROCESS(aSerializer, aUInt); unsigned line=__LINE__;
            u8string aAdditionalInfo=string("aUInt{test_serializer_base01.cpp(")
                                    +std::to_string(line)+")}";
            auto aError=aSerializer.getErrorInfo();
            THEOLIZER_CHECK(aError, aError.getErrorType());
            THEOLIZER_EQUAL(aError.getMessage().str().substr(0, aMessage.str().size()), aMessage);
            THEOLIZER_EQUAL(aError.getAdditionalInfo(), aAdditionalInfo);
            THEOLIZER_EQUAL(string(aError.getFileName()), "core_serializer.cpp");
            aSerializer.resetError();
        }

// ---------------------------------------------------------------------------
//      TypeCheckByIndex
// ---------------------------------------------------------------------------

//      ---<<< シリアライズ >>>---

        {
            std::ofstream   aStream("test_change_check_index0.log");
            theolizer::JsonOSerializer<theolizerD::SpecialDocument>
                aSerializer(aStream, theolizer::CheckMode::TypeCheckByIndex);

            int                  aInt=301;
            THEOLIZER_PROCESS(aSerializer, aInt);
        }

//      ---<<< デシリアライズ(例外有り) >>>---

        {
            std::ifstream   aStream("test_change_check_index0.log");
            theolizer::JsonISerializer<>   aSerializer(aStream);

            unsigned int             aUInt;
            THEOLIZER_CHECK_EXCEPTION(
                THEOLIZER_PROCESS(aSerializer, aUInt); ,
                theolizer::ErrorInfo);
        }

//      ---<<< デシリアライズ(例外無し) >>>---

        {
            std::ifstream   aStream("test_change_check_index0.log");
            theolizer::JsonISerializer<>   aSerializer(aStream, true);

            unsigned int         aUInt;
            THEOLIZER_PROCESS(aSerializer, aUInt); unsigned line=__LINE__;
            u8string aAdditionalInfo=string("aUInt{test_serializer_base01.cpp(")
                                    +std::to_string(line)+")}";
            auto aError=aSerializer.getErrorInfo();
            THEOLIZER_CHECK(aError, aError.getErrorType());
            THEOLIZER_EQUAL(aError.getMessage(), aMessage);
            THEOLIZER_EQUAL(aError.getAdditionalInfo(), aAdditionalInfo);
            THEOLIZER_EQUAL(string(aError.getFileName()), "core_serializer.cpp");
            aSerializer.resetError();
        }

// ***************************************************************************
//      ヘッダの型チェック・エラー・テスト用シリアライズ
//          デシリアライズとテストはtest_serializer_base03.*にて実装
// ***************************************************************************

// ---------------------------------------------------------------------------
//      TypeCheck
// ---------------------------------------------------------------------------

        {
            std::ofstream   aStream("test_change_check1.log");
            theolizer::JsonOSerializer<theolizerD::Document>
                aSerializer(aStream, theolizer::CheckMode::TypeCheck);

            IntrusiveBase2  mIntrusiveBase2;
            mIntrusiveBase2.mLong     =201;
            mIntrusiveBase2.mLongLong =202;
            mIntrusiveBase2.mULong    =203;
            mIntrusiveBase2.mULongLong=204;
            THEOLIZER_PROCESS(aSerializer, mIntrusiveBase2);
        }

// ---------------------------------------------------------------------------
//      TypeCheckByIndex
// ---------------------------------------------------------------------------

//      ---<<< ヘッダ >>>---

        {
            std::ofstream   aStream("test_change_check_index1.log");
            theolizer::JsonOSerializer<theolizerD::Document>
                aSerializer(aStream, theolizer::CheckMode::TypeCheckByIndex);

            IntrusiveBase2  mIntrusiveBase2;
            mIntrusiveBase2.mLong     =301;
            mIntrusiveBase2.mLongLong =302;
            mIntrusiveBase2.mULong    =303;
            mIntrusiveBase2.mULongLong=304;
            THEOLIZER_PROCESS(aSerializer, mIntrusiveBase2);
        }


// ***************************************************************************
//      フォーマット異常(データ途切れ)テスト
// ***************************************************************************

#if 1
        TestEof(theolizer::CheckMode::NoTypeCheck);
        TestEof(theolizer::CheckMode::TypeCheck);
        TestEof(theolizer::CheckMode::TypeCheckByIndex);
#endif

    }
    catch(theolizer::ErrorInfo& e)
    {
        std::cout << e.getMessage() << std::endl;
        ret=1;
    }

// ***************************************************************************
//      次のテストに備えてエラー・ログ削除
// ***************************************************************************

    theolizer::removeFile("ErrorLogFile0.log");
    theolizer::removeFile("ErrorLogFile1.log");

    return ret;
}
