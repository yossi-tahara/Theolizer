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
//          ヘッダを2箇所からincludeして問題ないことを確認する。
//############################################################################

#ifdef _MSC_VER
    #pragma warning(disable:4100 4127)
#endif

#include "test_serializer_base.h"
#include "test_serializer_base_version.h"

#include <sstream>
#include <fstream>
using namespace std;

#include <theolizer/test_tool.h>
using theolizer::u8string;

#include "test_serializer_base01b.cpp.theolizer.hpp"

//############################################################################
//      テストのサブ関数群－Ｂ
//############################################################################

// ***************************************************************************
//      文字列テスト
// ***************************************************************************

// MinGW向け特記事項
//  ソースをUTF-8エンコードしていると無印の文字列リテラルはUTF-8でエンコードされる。
//  しかし、Windows環境なのでマルチ・バイト文字処理が必要なので直接Shift-JISで
//  マルチ・バイト文字列を定義する。

#if defined(_WIN32)
    // CP932で"[こんにちは。]"
    const char aHelloCP932[] = "\x5B\x82\xB1\x82\xF1\x82\xC9\x82\xBF\x82\xCD\x81\x42\x5D";
#else
    // UTF-8で"[こんにちは。]"  非Windows用
    const char aHelloCP932[] = u8"[こんにちは。]";
#endif

void TestString(char const* iFileName)
{

// ---------------------------------------------------------------------------
//      シリアライズ
// ---------------------------------------------------------------------------

    {
        std::ofstream   aStream(iFileName);
        theolizer::JsonOSerializer<>    js(aStream);

        string      aString;
        wstring     aWstring;
        u16string   aU16string;
        u32string   aU32string;

//      ---<<< デフォルト >>>---

        aString = u8"デフォルトＵＴＦ－８";
        THEOLIZER_PROCESS(js, aString);

        aWstring = L"デフォルトＵＴＦ－１６／３２";
        THEOLIZER_PROCESS(js, aWstring);

        aU16string = u"デフォルトＵＴＦ－１６";
        THEOLIZER_PROCESS(js, aU16string);

        aU32string = U"デフォルトＵＴＦ－３２";
        THEOLIZER_PROCESS(js, aU32string);

//      ---<<< MultiByte指定 >>>---

        js.setCharIsMultiByte(true);

        aString = aHelloCP932;
        THEOLIZER_PROCESS(js, aString);

//      ---<<< UTF-8指定 >>>---

        js.setCharIsMultiByte(false);

        aString = u8"ユニコードＵＴＦ－８";
        THEOLIZER_PROCESS(js, aString);
    }

// ---------------------------------------------------------------------------
//      デシリアライズ
// ---------------------------------------------------------------------------

    {
        std::ifstream   aStream(iFileName);
        theolizer::JsonISerializer<>    js(aStream);


        string      aString;
        wstring     aWstring;
        u16string   aU16string;
        u32string   aU32string;

//      ---<<< デフォルト(UTF-8) >>>---

        aString = "";
        THEOLIZER_PROCESS(js, aString);
        THEOLIZER_EQUAL(aString, u8"デフォルトＵＴＦ－８");

        aWstring = L"";
        THEOLIZER_PROCESS(js, aWstring);
        THEOLIZER_EQUAL(aWstring, L"デフォルトＵＴＦ－１６／３２");

        aU16string = u"";
        THEOLIZER_PROCESS(js, aU16string);
        THEOLIZER_EQUAL(aU16string, u"デフォルトＵＴＦ－１６");

        aU32string = U"";
        THEOLIZER_PROCESS(js, aU32string);
        THEOLIZER_EQUAL(aU32string, U"デフォルトＵＴＦ－３２");

//      ---<<< MultiByte指定 >>>---

        js.setCharIsMultiByte(true);

        aString = "";
        THEOLIZER_PROCESS(js, aString);
        THEOLIZER_EQUAL(aString, aHelloCP932);

//      ---<<< UTF-8指定 >>>---

        js.setCharIsMultiByte(false);

        aString = "";
        THEOLIZER_PROCESS(js, aString);
        THEOLIZER_EQUAL(aString, u8"ユニコードＵＴＦ－８");
    }
}

// ***************************************************************************
//      保存先指定テスト(直接指定:Machine)
//          これは使い方の説明とテスト
//          →使い方の説明を独立させる方向とする
// ***************************************************************************

// ---------------------------------------------------------------------------
//      シリアライズ
// ---------------------------------------------------------------------------

void TestDestinationDirectSave(char const* iFileName)
{
    IntrusiveBase   aIntrusiveBase;
    aIntrusiveBase.setInt(101);
    aIntrusiveBase.mShort       =102;
    aIntrusiveBase.mString      ="103";

    IntrusiveBase2  aIntrusiveBase2;
    aIntrusiveBase2.mLong       =201;
    aIntrusiveBase2.mLongLong   =202;
    aIntrusiveBase2.mULong      =203;
    aIntrusiveBase2.mULongLong  =204;

    DestinationTest aDestinationTest;
    aDestinationTest.mIncluded1.mInt=301;
    aDestinationTest.mIncluded2.mShort=302;
    aDestinationTest.mFullAutoTemplate.setType0(303);
    aDestinationTest.mFullAutoTemplate.setType1(304);

    {
        std::ofstream   aStream(iFileName);
        theolizer::JsonOSerializer<theolizerD::Machine> js(aStream);

        THEOLIZER_PROCESS(js, aIntrusiveBase);
        THEOLIZER_PROCESS(js, aIntrusiveBase2);
        THEOLIZER_PROCESS(js, aDestinationTest);
    }
}

// ---------------------------------------------------------------------------
//      デシリアライズ
// ---------------------------------------------------------------------------

void TestDestinationDirectLoad(char const* iFileName)
{
    IntrusiveBase   aIntrusiveBase;
    IntrusiveBase2  aIntrusiveBase2;
    DestinationTest aDestinationTest;

    {
        std::ifstream   aStream(iFileName);
        theolizer::JsonISerializer<theolizerD::Machine> js(aStream);

        THEOLIZER_PROCESS(js, aIntrusiveBase);
        THEOLIZER_EQUAL(aIntrusiveBase.getInt(), 101);
        THEOLIZER_EQUAL(aIntrusiveBase.mShort, 102);
        THEOLIZER_EQUAL(aIntrusiveBase.mString, "");

        THEOLIZER_PROCESS(js, aIntrusiveBase2);
        THEOLIZER_EQUAL(aIntrusiveBase2.mLong, 201);
        THEOLIZER_EQUAL(aIntrusiveBase2.mLongLong, 202);
        THEOLIZER_EQUAL(aIntrusiveBase2.mULong, 0);
        THEOLIZER_EQUAL(aIntrusiveBase2.mULongLong, 0);

        THEOLIZER_PROCESS(js, aDestinationTest);
        THEOLIZER_EQUAL(aDestinationTest.mIncluded1.mInt, 301);
        THEOLIZER_EQUAL(aDestinationTest.mIncluded2.mShort, 0);
        THEOLIZER_EQUAL(aDestinationTest.mFullAutoTemplate.getType0(), 303);
        THEOLIZER_EQUAL(aDestinationTest.mFullAutoTemplate.getType1(), 304);
    }
}

// ***************************************************************************
//      保存先指定テスト(パラメータ指定:User)
//          これは使い方の説明とテスト
//          →使い方の説明を独立させる方向とする
// ***************************************************************************

template
<
    theolizer::Destination    uDefault=theolizerD::All,
    theolizer::Destination... uDestinations
>
void TestDestinationParameter(char const* iFileName)
{
//----------------------------------------------------------------------------
//      シリアライズ
//----------------------------------------------------------------------------

    {
        IntrusiveBase   aIntrusiveBase;
        aIntrusiveBase.setInt(101);
        aIntrusiveBase.mShort       =102;
        aIntrusiveBase.mString      ="103";

        IntrusiveBase2  aIntrusiveBase2;
        aIntrusiveBase2.mLong       =201;
        aIntrusiveBase2.mLongLong   =202;
        aIntrusiveBase2.mULong      =203;
        aIntrusiveBase2.mULongLong  =204;

        DestinationTest aDestinationTest;
        aDestinationTest.mIncluded1.mInt=301;
        aDestinationTest.mIncluded2.mShort=302;

        std::ofstream   aStream(iFileName);
        theolizer::JsonOSerializer<uDefault, uDestinations...> js(aStream);

        THEOLIZER_PROCESS(js, aIntrusiveBase);
        THEOLIZER_PROCESS(js, aIntrusiveBase2);
        THEOLIZER_PROCESS(js, aDestinationTest);
    }

//----------------------------------------------------------------------------
//      デシリアライズ
//----------------------------------------------------------------------------

    {
        IntrusiveBase   aIntrusiveBase;
        IntrusiveBase2  aIntrusiveBase2;
        DestinationTest aDestinationTest;

        std::ifstream   aStream(iFileName);
        theolizer::JsonISerializer<uDefault, uDestinations...> js(aStream);

        THEOLIZER_PROCESS(js, aIntrusiveBase);
        THEOLIZER_EQUAL(aIntrusiveBase.getInt(), 0);
        THEOLIZER_EQUAL(aIntrusiveBase.mShort, 0);
        THEOLIZER_EQUAL(aIntrusiveBase.mString, "103");

        THEOLIZER_PROCESS(js, aIntrusiveBase2);
        THEOLIZER_EQUAL(aIntrusiveBase2.mLong, 0);
        THEOLIZER_EQUAL(aIntrusiveBase2.mLongLong, 0);
        THEOLIZER_EQUAL(aIntrusiveBase2.mULong, 203);
        THEOLIZER_EQUAL(aIntrusiveBase2.mULongLong, 204);

        THEOLIZER_PROCESS(js, aDestinationTest);
        THEOLIZER_EQUAL(aDestinationTest.mIncluded1.mInt, 0);
        THEOLIZER_EQUAL(aDestinationTest.mIncluded2.mShort, 302);
    }
}

//----------------------------------------------------------------------------
//      実体化
//          実際に呼ぶもののみ実体化すれば十分
//          呼ばれないものまで実体化した時のデメリットは
//              アプリ起動時にトップ・レベルで保存／回復しているクラスの
//              登録処理が余分に走る。
//----------------------------------------------------------------------------

//template void TestDestinationParameter<theolizerD::Machine>(char const* iFileName);
template void TestDestinationParameter<theolizerD::User>(char const* iFileName);
//template void TestDestinationParameter<theolizerD::Document>(char const* iFileName);
//template void TestDestinationParameter<theolizerD::SpecialDocument>(char const* iFileName);
//template void TestDestinationParameter<theolizerD::TestDocument>(char const* iFileName);

// ***************************************************************************
//      中断ファイル・テスト用サブ関数
// ***************************************************************************

void TestEof(const theolizer::CheckMode iCheckMode)
{
    std::stringstream aStream;

    {
        theolizer::JsonOSerializer<theolizerD::Document> js(aStream, iCheckMode, false);

        int                   aInt=101;
        THEOLIZER_PROCESS(js, aInt);

        IntrusiveBase2  aIntrusiveBase2;
        aIntrusiveBase2.mLong     =201;
        aIntrusiveBase2.mLongLong =202;
        aIntrusiveBase2.mULong    =203;
        aIntrusiveBase2.mULongLong=204;
        THEOLIZER_PROCESS(js, aIntrusiveBase2);
    }

    std::cout << aStream.str() << std::endl;

    for (std::size_t i=1; i < aStream.str().size()-1; ++i)
    {
        // エラー発生
        std::stringstream ss(aStream.str().substr(0, i));
        bool aDoConstructing=true;
        std::streampos  aPos=0;

        theolizer::JsonISerializer<theolizerD::Document>    js(ss, true);
        if (!js.isError())
        {
            aDoConstructing=false;
            aPos=ss.tellg();            // リカバリ時の同期位置保存

            int                   aInt=0;
            THEOLIZER_PROCESS(js, aInt);

            IntrusiveBase2  aIntrusiveBase2{};
            THEOLIZER_PROCESS(js, aIntrusiveBase2);
        }

        // エラーが発生していない場合、FAIL
        THEOLIZER_CHECK(js.isError(), "No error");

        // リカバリ
        if (aDoConstructing)    // コンストラクト中のリカバリは不可能
    continue;

        ss.clear();
        ss.str(aStream.str());
        ss.seekg(aPos);
        js.resetError();

        int                   aInt=0;
        THEOLIZER_PROCESS(js, aInt);
        THEOLIZER_EQUAL(aInt, 101);

        IntrusiveBase2  aIntrusiveBase2{};
        THEOLIZER_PROCESS(js, aIntrusiveBase2);
        THEOLIZER_EQUAL(aIntrusiveBase2.mLong,      201);
        THEOLIZER_EQUAL(aIntrusiveBase2.mLongLong,  202);
        THEOLIZER_EQUAL(aIntrusiveBase2.mULong,     203);
        THEOLIZER_EQUAL(aIntrusiveBase2.mULongLong, 204);
    }
}
