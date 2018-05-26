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

#include "test_serializer_base03.h"

#ifdef _MSC_VER
    #pragma warning(disable:4100 4127)
#endif

#include <sstream>
#include <fstream>
#include <typeindex>
using namespace std;

#include <theolizer/test_tool.h>
#include <theolizer/serializer_json.h>
using theolizer::u8string;

// 再インクルード・テスト
#include <theolizer/test_tool.h>
#include <theolizer/serializer_json.h>

//############################################################################
//      ボリモーフィズム・テスト
//############################################################################

template<class tSerializer>
void TestPoly(tSerializer &iSerializer)
{
    BaseClass* aBaseClass=nullptr;

    // テスト中のメモリ・リーク回避
    auto aAutoDeleter=theolizer::makeScopeExit([&]{delete aBaseClass;});

//----------------------------------------------------------------------------
//      シリアライズ用準備
//----------------------------------------------------------------------------

    if (tSerializer::hasProperty(theolizer::Property::IsSaver))
    {
        Derived2Class* aDerived2Class=new Derived2Class;
        aDerived2Class->mInt =10;
        aDerived2Class->mUInt=20;
        aDerived2Class->mLong=30;
        aBaseClass=aDerived2Class;
    }

//----------------------------------------------------------------------------
//      シリアライズ or デシリアライズ処理
//----------------------------------------------------------------------------

    THEOLIZER_PROCESS_OWNER(iSerializer, aBaseClass);
    iSerializer.clearTracking();

//----------------------------------------------------------------------------
//      デシリアライズ結果検証
//----------------------------------------------------------------------------

    if (!tSerializer::hasProperty(theolizer::Property::IsSaver))
    {
        Derived2Class* aDerived2Class=dynamic_cast<Derived2Class*>(aBaseClass);
        THEOLIZER_REQUIRE(aDerived2Class != nullptr, aDerived2Class);
        THEOLIZER_EQUAL(aDerived2Class->mInt, 10);
        THEOLIZER_EQUAL(aDerived2Class->mUInt, 20);
        THEOLIZER_EQUAL(aDerived2Class->mLong, 30);
    }
}

//############################################################################
//      テストのメイン関数
//          ・ポリモーフィズム
//          ・namespace
//          ・参照
//############################################################################

int main(int argc, char** argv)
{
//  theolizer::DisplayPass aDisplayPass;

    try
    {

//----------------------------------------------------------------------------
//      型チェック無し
//----------------------------------------------------------------------------

        {
            std::ofstream   aStream("test_poly_no_check.log");
            theolizer::JsonOSerializer<>   js(aStream);
            TestPoly(js);
        }
        {
            std::ifstream   aStream("test_poly_no_check.log");
            theolizer::JsonISerializer<>   js(aStream);
            TestPoly(js);
        }

//----------------------------------------------------------------------------
//      TypeIndexで型チェック
//----------------------------------------------------------------------------

        {
            std::ofstream   aStream("test_poly_type_check.log");
            theolizer::JsonOSerializer<>   js(aStream, theolizer::CheckMode::TypeCheck);
            TestPoly(js);
        }
        {
            std::ifstream   aStream("test_poly_type_check.log");
            theolizer::JsonISerializer<>   js(aStream);
            TestPoly(js);
        }

//----------------------------------------------------------------------------
//      MetaModeで型チェック
//----------------------------------------------------------------------------

        {
            std::ofstream   aStream("test_poly_meta.log");
            theolizer::JsonOSerializer<>   js(aStream, theolizer::CheckMode::MetaMode);
            TestPoly(js);
        }
        {
            std::ifstream   aStream("test_poly_meta.log");
            theolizer::JsonISerializer<>   js(aStream);
            TestPoly(js);
        }

//----------------------------------------------------------------------------
//      namespaceテスト
//----------------------------------------------------------------------------

#if defined(NAMESPACE_TEST)
        {
            std::ofstream   aStream("test_namespace.log");
            theolizer::JsonOSerializer<>   js(aStream, theolizer::CheckMode::TypeCheck);

            user::NamespaceClass aBaseClass;
            aBaseClass.mUInt=123;
            THEOLIZER_PROCESS(js, aBaseClass);
        }
        {
            std::ifstream   aStream("test_namespace.log");
            theolizer::JsonISerializer<>   js(aStream);
            user::NamespaceClass aBaseClass;
            THEOLIZER_PROCESS(js, aBaseClass);
            THEOLIZER_EQUAL(aBaseClass.mUInt, 123);
        }
#endif

//----------------------------------------------------------------------------
//      参照テスト
//----------------------------------------------------------------------------

#ifdef REFERENCE_TEST
        {
            std::ofstream   aStream("test_reference.log");
            theolizer::JsonOSerializer<>   js(aStream, theolizer::CheckMode::TypeCheck);

            unsigned    aUInt=1;
            BaseClass   aBaseClass(2);
            int         aIntArray[2]={3, 4};

            ReferenceClass aReferenceClass(aUInt, aBaseClass, aIntArray);
            THEOLIZER_PROCESS(js, aReferenceClass);
        }

        {
            std::ifstream   aStream("test_reference.log");
            theolizer::JsonISerializer<>   js(aStream);

            unsigned    aUInt=0;
            BaseClass   aBaseClass(0);
            int         aIntArray[2]={0, 0};

            ReferenceClass aReferenceClass(aUInt, aBaseClass, aIntArray);
            THEOLIZER_PROCESS(js, aReferenceClass);
            THEOLIZER_EQUAL(aUInt, 1);
            THEOLIZER_EQUAL(aBaseClass.mInt, 2);
            THEOLIZER_EQUAL(aIntArray[0], 3);
            THEOLIZER_EQUAL(aIntArray[1], 4);
        }
#endif
    }
    catch(theolizer::ErrorInfo& e)
    {
        std::cout << e.getMessage() << std::endl;
    }
    catch(...)
    {
        std::cout << "unknown exception!!" << std::endl;
throw;
    }

    return 0;
}
