//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          THEOLIZER_PROCESS()の基本的な使い方
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
#ifdef ENABLE_BASIC_PROCESS_TEST

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

// プロジェクト・ヘッダ
#include "common.h"
#include "test_basic_process.h"

// 自動生成ソース
#include "test_basic_process.cpp.theolizer.hpp"

//############################################################################
//      プリミティブ、enum型、クラス、配列
//############################################################################

#if defined(_WIN32)
    // CP932で"[こんにちは。]"
    const char gHelloCP932[] = "\x5B\x82\xB1\x82\xF1\x82\xC9\x82\xBF\x82\xCD\x81\x42\x5D";
#else
    // UTF-8で"[こんにちは。]"  非Windows用
    const char gHelloCP932[] = u8"[こんにちは。]";
#endif

// ***************************************************************************
//      使い方サンプル・ソース
//          Shift-JISコード対応
//          文字列変数の互換
//          エラー・ログ
// ***************************************************************************

// エラー・ログ・ファイルの指定
THEOLIZER_ERRORLOG_FILE("ErrorLogSample%1%.log");

void tutoriseBasic()
{
    std::cout << "tutoriseBasic() start" << std::endl;

//----------------------------------------------------------------------------
//      保存
//----------------------------------------------------------------------------

    {
        std::ofstream   aStream("tutorise_basic.json");
        theolizer::JsonOSerializer<> aSerializer(aStream);

//      ---<<< Shift-JISコードのテスト >>>---

        // MultiByte指定
        aSerializer.setCharIsMultiByte(true);
        std::string aString = gHelloCP932;
        THEOLIZER_PROCESS(aSerializer, aString);

        // UTF-8へ戻す
        aSerializer.setCharIsMultiByte(false);
        aString = u8"ユニコードＵＴＦ－８";
        THEOLIZER_PROCESS(aSerializer, aString);

//      ---<<< 文字列互換テスト >>>---

        // charをwchar_t, char16_t, char32_tへ回復
        THEOLIZER_PROCESS(aSerializer, aString);
        THEOLIZER_PROCESS(aSerializer, aString);
        THEOLIZER_PROCESS(aSerializer, aString);

        // wchar_tをchar, char16_t, char32_tへ回復
        std::wstring aWString = L"ユニコードＵＴＦ－１６";
        THEOLIZER_PROCESS(aSerializer, aWString);
        THEOLIZER_PROCESS(aSerializer, aWString);
        THEOLIZER_PROCESS(aSerializer, aWString);

        // char16_tをchar, wchar_t, char32_tへ回復
        std::u16string aU16String = u"ユニコードＵＴＦ－１６";
        THEOLIZER_PROCESS(aSerializer, aU16String);
        THEOLIZER_PROCESS(aSerializer, aU16String);
        THEOLIZER_PROCESS(aSerializer, aU16String);

        // char32_tをchar, wchar_t, char16_tへ回復
        std::u32string aU32String = U"ユニコードＵＴＦ－３２";
        THEOLIZER_PROCESS(aSerializer, aU32String);
        THEOLIZER_PROCESS(aSerializer, aU32String);
        THEOLIZER_PROCESS(aSerializer, aU32String);
    }

//----------------------------------------------------------------------------
//      回復
//----------------------------------------------------------------------------

    {
        std::ifstream   aStream("tutorise_basic.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

//      ---<<< Shift-JISコードのテスト >>>---

        // MultiByte指定
        aSerializer.setCharIsMultiByte(true);
        std::string aString = "";
        THEOLIZER_PROCESS(aSerializer, aString);
        THEOLIZER_EQUAL(aString, gHelloCP932);

        // UTF-8へ戻す
        aSerializer.setCharIsMultiByte(false);
        aString = "";
        THEOLIZER_PROCESS(aSerializer, aString);
        THEOLIZER_EQUAL(aString, u8"ユニコードＵＴＦ－８");


//      ---<<< 文字列互換テスト >>>---

        std::wstring   aWString;
        std::u16string aU16String;
        std::u32string aU32String;

        // charをwchar_t, char16_t, char32_tへ回復
        THEOLIZER_PROCESS(aSerializer, aWString);
        THEOLIZER_EQUAL(aWString, L"ユニコードＵＴＦ－８");

        THEOLIZER_PROCESS(aSerializer, aU16String);
        THEOLIZER_EQUAL(aU16String, u"ユニコードＵＴＦ－８");

        THEOLIZER_PROCESS(aSerializer, aU32String);
        THEOLIZER_EQUAL(aU32String, U"ユニコードＵＴＦ－８");

        // wchar_tをchar, char16_t, char32_tへ回復
        aWString.clear();
        aU16String.clear();
        aU32String.clear();
        THEOLIZER_PROCESS(aSerializer, aWString);
        THEOLIZER_EQUAL(aWString, L"ユニコードＵＴＦ－１６");

        THEOLIZER_PROCESS(aSerializer, aU16String);
        THEOLIZER_EQUAL(aU16String, u"ユニコードＵＴＦ－１６");

        THEOLIZER_PROCESS(aSerializer, aU32String);
        THEOLIZER_EQUAL(aU32String, U"ユニコードＵＴＦ－１６");

        // char16_tをchar, wchar_t, char32_tへ回復
        aWString.clear();
        aU16String.clear();
        aU32String.clear();
        THEOLIZER_PROCESS(aSerializer, aWString);
        THEOLIZER_EQUAL(aWString, L"ユニコードＵＴＦ－１６");

        THEOLIZER_PROCESS(aSerializer, aU16String);
        THEOLIZER_EQUAL(aU16String, u"ユニコードＵＴＦ－１６");

        THEOLIZER_PROCESS(aSerializer, aU32String);
        THEOLIZER_EQUAL(aU32String, U"ユニコードＵＴＦ－１６");

        // char32_tをchar, wchar_t, char16_tへ回復
        aWString.clear();
        aU16String.clear();
        aU32String.clear();
        THEOLIZER_PROCESS(aSerializer, aWString);
        THEOLIZER_EQUAL(aWString, L"ユニコードＵＴＦ－３２");

        THEOLIZER_PROCESS(aSerializer, aU16String);
        THEOLIZER_EQUAL(aU16String, u"ユニコードＵＴＦ－３２");

        THEOLIZER_PROCESS(aSerializer, aU32String);
        THEOLIZER_EQUAL(aU32String, U"ユニコードＵＴＦ－３２");
    }

//----------------------------------------------------------------------------
//      エラー・ログ出力
//----------------------------------------------------------------------------

    theolizer::removeFile("ErrorLogSample0.log");
    theolizer::removeFile("ErrorLogSample1.log");

    {
        std::ifstream   aStream("tutorise_basic.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        int aInt;
        THEOLIZER_CHECK_EXCEPTION2(
            THEOLIZER_PROCESS(aSerializer, aInt);,                  // dStatements
            theolizer::ErrorInfo& e,                                // dException
            e.getErrorKind() == theolizer::ErrorKind::UnknownData,  // dJudge
            e.getMessage());                                        // dResult
        std::cout << "ErrorInfo.getString()=" << aSerializer.getErrorInfo().getString() << "\n";
    }

    theolizer::u8string aErrorLog = "ErrorLogSample0.log";
    std::cout << "------------- " << aErrorLog << " ------------- start\n";
    std::ifstream ifs(aErrorLog.get_fstring());
    while(ifs)
    {
        std::string aString;
        std::getline(ifs, aString);
        std::cout << aString << "\n";
    }
    std::cout << "------------- " << aErrorLog << " ------------- end\n";

//----------------------------------------------------------------------------
//      エラー通知の使い方
//----------------------------------------------------------------------------

//! [ErrorReport]

//      ---<<< 例外許可 >>>---

    try
    {
        std::ifstream   aStream("tutorise_basic.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        int aInt;
        THEOLIZER_PROCESS(aSerializer, aInt);   // Type mismatch発生
    }
    catch (theolizer::ErrorInfo& e)
    {
        std::cout << "exception    : " << e.getString() << std::endl;
    }

//      ---<<< 例外禁止 >>>---

    try
    {
        std::ifstream   aStream("tutorise_basic.json");
        theolizer::JsonISerializer<> aSerializer(aStream, true);

        short ashort;
        THEOLIZER_PROCESS(aSerializer, ashort); // Type mismatch発生

        theolizer::ErrorInfo aErrorInfo=aSerializer.getErrorInfo();
        if (aErrorInfo)
        {
            std::cout << "no-exception : " << aErrorInfo.getString() << std::endl;
            aSerializer.resetError();           // エラー状態を解除する
        }
    }
    catch (theolizer::ErrorInfo& e)
    {
        std::cout << "exception    : " << e.getString() << std::endl;
    }

//! [ErrorReport]

    std::cout << "tutoriseBasic() end" << std::endl;
}

// ***************************************************************************
//      保存
// ***************************************************************************

//! [saveBasicProcess]
template<class tSerializer>
void saveBasicProcess(tSerializer& iSerializer)
{

//      ---<<< 文字型 >>>---

    char            aChar = 100;
    THEOLIZER_PROCESS(iSerializer, aChar);
    signed char     aSChar=-101;
    THEOLIZER_PROCESS(iSerializer, aSChar);
    unsigned char   aUChar= 102;
    THEOLIZER_PROCESS(iSerializer, aUChar);

    wchar_t         aWchar  =1000;
    THEOLIZER_PROCESS(iSerializer, aWchar);

    char16_t        aChar16=1001;
    THEOLIZER_PROCESS(iSerializer, aChar16);

    char32_t        aChar32=1002;
    THEOLIZER_PROCESS(iSerializer, aChar32);

//      ---<<< 整数型 >>>---

    bool            aBool  =true;
    THEOLIZER_PROCESS(iSerializer, aBool);
    short           aShort =-2000;
    THEOLIZER_PROCESS(iSerializer, aShort);
    unsigned short  aUShort= 2000;
    THEOLIZER_PROCESS(iSerializer, aUShort);

    int             aInt =-3000;
    THEOLIZER_PROCESS(iSerializer, aInt);
    unsigned int    aUInt= 3000U;
    THEOLIZER_PROCESS(iSerializer, aUInt);

    long            aLong =-4000L;
    THEOLIZER_PROCESS(iSerializer, aLong);
    unsigned long   aULong= 4000UL;
    THEOLIZER_PROCESS(iSerializer, aULong);

    long long           aLongLong =-5000LL;
    THEOLIZER_PROCESS(iSerializer, aLongLong);
    unsigned long long  aULongLong= 5000ULL;
    THEOLIZER_PROCESS(iSerializer, aULongLong);

//      ---<<< 浮動小数点型 >>>---

    float       aFloat     =1.23456F;
    THEOLIZER_PROCESS(iSerializer, aFloat);

    double      aDouble    =1.23456789012345;
    THEOLIZER_PROCESS(iSerializer, aDouble);

    long double aLongDouble=1.23456789012345678L;
    THEOLIZER_PROCESS(iSerializer, aLongDouble);

//      ---<<< 文字列型 >>>---

    std::string aString=u8"ＵＴＦ－８";
    THEOLIZER_PROCESS(iSerializer, aString);

    std::wstring aWstring=L"ＵＴＦ－１６／３２";
    THEOLIZER_PROCESS(iSerializer, aWstring);

    std::u16string aU16string=u"ＵＴＦ－１６";
    THEOLIZER_PROCESS(iSerializer, aU16string);

    std::u32string aU32string=U"ＵＴＦ－３２";
    THEOLIZER_PROCESS(iSerializer, aU32string);

//      ---<<< enum型 >>>---

    NormalEnum aNormalEnum=eneOne;
    THEOLIZER_PROCESS(iSerializer, aNormalEnum);

    ScopedEnum aScopedEnum=ScopedEnum::TWO;
    THEOLIZER_PROCESS(iSerializer, aScopedEnum);

//      ---<<< クラス >>>---

    ClassBasicTest  aClassBasicTest(10000, "10000", eneTwo);
    THEOLIZER_PROCESS(iSerializer, aClassBasicTest);

//      ---<<< 右辺値保存 >>>---

    THEOLIZER_PROCESS(iSerializer, -3100);
    THEOLIZER_PROCESS(iSerializer,  3100U);
    THEOLIZER_PROCESS(iSerializer, -4100L);
    THEOLIZER_PROCESS(iSerializer,  4100UL);
    THEOLIZER_PROCESS(iSerializer, -5100LL);
    THEOLIZER_PROCESS(iSerializer,  5100ULL);

    THEOLIZER_PROCESS(iSerializer, 2.23456F);
    THEOLIZER_PROCESS(iSerializer, 2.23456789012345);
    THEOLIZER_PROCESS(iSerializer, 2.23456789012345678L);

    THEOLIZER_PROCESS(iSerializer, std::string(u8"rvalue:ＵＴＦ－８"));
    THEOLIZER_PROCESS(iSerializer, std::wstring(L"rvalue:ＵＴＦ－１６／３２"));
    THEOLIZER_PROCESS(iSerializer, std::u16string(u"rvalue:ＵＴＦ－１６"));
    THEOLIZER_PROCESS(iSerializer, std::u32string(U"rvalue:ＵＴＦ－３２"));

    THEOLIZER_PROCESS(iSerializer, eneTwo);
    THEOLIZER_PROCESS(iSerializer, ScopedEnum::ONE);

    THEOLIZER_PROCESS(iSerializer, ClassBasicTest(20000, "20000", eneOne));

//      ---<<< 配列保存 >>>---

    // 文字型
    saveTestArray<char, 5>
    (
        iSerializer,
        [](std::size_t i){return static_cast<char>(i);}
    );
    saveTestArray<signed char, 6>
    (
        iSerializer,
        [](std::size_t i){return static_cast<signed char>(i);}
    );
    saveTestArray<unsigned char, 7>
    (
        iSerializer,
        [](std::size_t i){return static_cast<unsigned char>(i);}
    );
    saveTestArray<wchar_t, 8>
    (
        iSerializer,
        [](std::size_t i){return static_cast<wchar_t>(i);}
    );
    saveTestArray<char16_t, 9>
    (
        iSerializer,
        [](std::size_t i){return static_cast<char16_t>(i);}
    );
    saveTestArray<char32_t, 10>
    (
        iSerializer,
        [](std::size_t i){return static_cast<char32_t>(i);}
    );

    // 整数型
    saveTestArray<bool, 4>
    (
        iSerializer,
        [](std::size_t){return static_cast<bool>(true);}
    );
    saveTestArray<short, 5>
    (
        iSerializer,
        [](std::size_t i){return static_cast<short>(i*10);}
    );
    saveTestArray<unsigned short, 6>
    (
        iSerializer,
        [](std::size_t i){return static_cast<unsigned short>(i*10);}
    );

    saveTestArray<int, 7>
    (
        iSerializer,
        [](std::size_t i){return static_cast<int>(i*100);}
    );
    saveTestArray<unsigned int, 8>
    (
        iSerializer,
        [](std::size_t i){return static_cast<unsigned int>(i*100);}
    );

    saveTestArray<long, 9>
    (
        iSerializer,
        [](std::size_t i){return static_cast<long>(i*1000);}
    );
    saveTestArray<unsigned long, 10>
    (
        iSerializer,
        [](std::size_t i){return static_cast<unsigned long>(i*1000);}
    );

    saveTestArray<long long, 11>
    (
        iSerializer,
        [](std::size_t i){return static_cast<long long>(i*10000);}
    );
    saveTestArray<unsigned long long, 12>
    (
        iSerializer,
        [](std::size_t i){return static_cast<unsigned long long>(i*10000);}
    );

    // 浮動小数点型
    saveTestArray<float, 5>
    (
        iSerializer,
        [](std::size_t i){return static_cast<float>(i+0.1);}
    );
    saveTestArray<double, 6>
    (
        iSerializer,
        [](std::size_t i){return i+0.2;}
    );
    saveTestArray<long double, 7>
    (
        iSerializer,
        [](std::size_t i){return static_cast<long double>(i+0.3);}
    );

    // 文字列型
    saveTestArray<std::string, 5>
    (
        iSerializer,
        [](std::size_t i){return std::to_string(i);}
    );
    saveTestArray<std::wstring, 6>
    (
        iSerializer,
        [](std::size_t i){return std::to_wstring(i);}
    );
    saveTestArray<std::u16string, 7>
    (
        iSerializer,
        [](std::size_t i){return theolizer::u8string(std::to_string(i)).get_u16string();}
    );
    saveTestArray<std::u32string, 8>
    (
        iSerializer,
        [](std::size_t i){return theolizer::u8string(std::to_string(i)).get_u32string();}
    );

    // enum型
    saveTestArray<NormalEnum, 5>
    (
        iSerializer,
        [](std::size_t i){return static_cast<NormalEnum>(i % 3);}
    );
    saveTestArray<ScopedEnum, 6>
    (
        iSerializer,
        [](std::size_t i){return static_cast<ScopedEnum>(i % 3);}
    );

    // クラス型
    saveTestArray<ClassBasicTest, 5>
    (
        iSerializer,
        [](std::size_t i)
        {
            return  ClassBasicTest
                    (
                        static_cast<int>(i),
                        std::to_string(i).c_str(),
                        static_cast<NormalEnum>(i % 3)
                    );
        }
    );
}
//! [saveBasicProcess]

INSTANTIATION_ALL(saveBasicProcess);

// ***************************************************************************
//      回復
// ***************************************************************************

//! [loadBasicProcess]
template<class tSerializer>
void loadBasicProcess(tSerializer& iSerializer)
{

//      ---<<< 文字型 >>>---

    char            aChar =0;
    THEOLIZER_PROCESS(iSerializer, aChar);
    THEOLIZER_EQUAL(aChar, 100);
    signed char     aSChar=0;
    THEOLIZER_PROCESS(iSerializer, aSChar);
    THEOLIZER_EQUAL(aSChar, -101);
    unsigned char   aUChar=0;
    THEOLIZER_PROCESS(iSerializer, aUChar);
    THEOLIZER_EQUAL(aUChar, 102);

    wchar_t         aWchar  =0;
    THEOLIZER_PROCESS(iSerializer, aWchar);
    THEOLIZER_EQUAL(aWchar, 1000);

    char16_t        aChar16=0;
    THEOLIZER_PROCESS(iSerializer, aChar16);
    THEOLIZER_EQUAL(aChar16, 1001);

    char32_t        aChar32=0;
    THEOLIZER_PROCESS(iSerializer, aChar32);
    THEOLIZER_EQUAL(aChar32, 1002);

//      ---<<< 整数型 >>>---

    bool            aBool=false;
    THEOLIZER_PROCESS(iSerializer, aBool);
    THEOLIZER_EQUAL(aBool, true);
    short           aShort=0;
    THEOLIZER_PROCESS(iSerializer, aShort);
    THEOLIZER_EQUAL(aShort, -2000);
    unsigned short  aUShort=0;
    THEOLIZER_PROCESS(iSerializer, aUShort);
    THEOLIZER_EQUAL(aUShort, 2000);

    int             aInt=0;
    THEOLIZER_PROCESS(iSerializer, aInt);
    THEOLIZER_EQUAL(aInt, -3000);
    unsigned int    aUInt=0;
    THEOLIZER_PROCESS(iSerializer, aUInt);
    THEOLIZER_EQUAL(aUInt, 3000U);

    long            aLong=0;
    THEOLIZER_PROCESS(iSerializer, aLong);
    THEOLIZER_EQUAL(aLong, -4000L);
    unsigned long   aULong=0;
    THEOLIZER_PROCESS(iSerializer, aULong);
    THEOLIZER_EQUAL(aULong, 4000UL);

    long long       aLongLong=0;
    THEOLIZER_PROCESS(iSerializer, aLongLong);
    THEOLIZER_EQUAL(aLongLong, -5000LL);
    unsigned long long  aULongLong=0;
    THEOLIZER_PROCESS(iSerializer, aULongLong);
    THEOLIZER_EQUAL(aULongLong, 5000ULL);

//      ---<<< 浮動小数点型 >>>---

    float       aFloat;
    THEOLIZER_PROCESS(iSerializer, aFloat);
    THEOLIZER_EQUAL(aFloat, 1.23456F);

    double      aDouble;
    THEOLIZER_PROCESS(iSerializer, aDouble);
    THEOLIZER_EQUAL(aDouble, 1.23456789012345);

    long double aLongDouble;
    THEOLIZER_PROCESS(iSerializer, aLongDouble);
    THEOLIZER_EQUAL(aLongDouble, 1.23456789012345678L);

//      ---<<< 文字列型 >>>---

    std::string aString;
    THEOLIZER_PROCESS(iSerializer, aString);
    THEOLIZER_EQUAL(aString, u8"ＵＴＦ－８");

    std::wstring aWstring;
    THEOLIZER_PROCESS(iSerializer, aWstring);
    THEOLIZER_EQUAL(aWstring, L"ＵＴＦ－１６／３２");

    std::u16string aU16string;
    THEOLIZER_PROCESS(iSerializer, aU16string);
    THEOLIZER_EQUAL(aU16string, u"ＵＴＦ－１６");

    std::u32string aU32string;
    THEOLIZER_PROCESS(iSerializer, aU32string);
    THEOLIZER_EQUAL(aU32string, U"ＵＴＦ－３２");

//      ---<<< enum型 >>>---

    NormalEnum aNormalEnum=eneZero;
    THEOLIZER_PROCESS(iSerializer, aNormalEnum);
    THEOLIZER_EQUAL(aNormalEnum, eneOne);

    ScopedEnum aScopedEnum=ScopedEnum::ZERO;
    THEOLIZER_PROCESS(iSerializer, aScopedEnum);
    THEOLIZER_EQUAL(aScopedEnum, ScopedEnum::TWO);

//      ---<<< クラス >>>---

    ClassBasicTest  aClassBasicTest;
    THEOLIZER_PROCESS(iSerializer, aClassBasicTest);
    THEOLIZER_EQUAL(aClassBasicTest.mInt, 10000);
    THEOLIZER_EQUAL(aClassBasicTest.mString, "10000");
    THEOLIZER_EQUAL(aClassBasicTest.mNormalEnum, eneTwo);

//      ---<<< 右辺値保存からの回復 >>>---

    THEOLIZER_PROCESS(iSerializer, aInt);
    THEOLIZER_EQUAL(aInt, -3100);
    THEOLIZER_PROCESS(iSerializer, aUInt);
    THEOLIZER_EQUAL(aUInt, 3100);

    THEOLIZER_PROCESS(iSerializer, aLong);
    THEOLIZER_EQUAL(aLong, -4100L);
    THEOLIZER_PROCESS(iSerializer, aULong);
    THEOLIZER_EQUAL(aULong, 4100UL);

    THEOLIZER_PROCESS(iSerializer, aLongLong);
    THEOLIZER_EQUAL(aLongLong, -5100LL);
    THEOLIZER_PROCESS(iSerializer, aULongLong);
    THEOLIZER_EQUAL(aULongLong, 5100ULL);

    THEOLIZER_PROCESS(iSerializer, aFloat);
    THEOLIZER_EQUAL(aFloat, 2.23456F);

    THEOLIZER_PROCESS(iSerializer, aDouble);
    THEOLIZER_EQUAL(aDouble, 2.23456789012345);

    THEOLIZER_PROCESS(iSerializer, aLongDouble);
    THEOLIZER_EQUAL(aLongDouble, 2.23456789012345678L);

    THEOLIZER_PROCESS(iSerializer, aString);
    THEOLIZER_EQUAL(aString, u8"rvalue:ＵＴＦ－８");

    THEOLIZER_PROCESS(iSerializer, aWstring);
    THEOLIZER_EQUAL(aWstring, L"rvalue:ＵＴＦ－１６／３２");

    THEOLIZER_PROCESS(iSerializer, aU16string);
    THEOLIZER_EQUAL(aU16string, u"rvalue:ＵＴＦ－１６");

    THEOLIZER_PROCESS(iSerializer, aU32string);
    THEOLIZER_EQUAL(aU32string, U"rvalue:ＵＴＦ－３２");

    THEOLIZER_PROCESS(iSerializer, aNormalEnum);
    THEOLIZER_EQUAL(aNormalEnum, eneTwo);

    THEOLIZER_PROCESS(iSerializer, aScopedEnum);
    THEOLIZER_EQUAL(aScopedEnum, ScopedEnum::ONE);

    THEOLIZER_PROCESS(iSerializer, aClassBasicTest);
    THEOLIZER_EQUAL(aClassBasicTest.mInt, 20000);
    THEOLIZER_EQUAL(aClassBasicTest.mString, "20000");
    THEOLIZER_EQUAL(aClassBasicTest.mNormalEnum, eneOne);

//      ---<<< 配列回復 >>>---

    // 文字型
    loadTestArray<char, 5>
    (
        iSerializer,
        [](std::size_t i){return static_cast<char>(i);}
    );
    loadTestArray<signed char, 6>
    (
        iSerializer,
        [](std::size_t i){return static_cast<signed char>(i);}
    );
    loadTestArray<unsigned char, 7>
    (
        iSerializer,
        [](std::size_t i){return static_cast<unsigned char>(i);}
    );
    loadTestArray<wchar_t, 8>
    (
        iSerializer,
        [](std::size_t i){return static_cast<wchar_t>(i);}
    );
    loadTestArray<char16_t, 9>
    (
        iSerializer,
        [](std::size_t i){return static_cast<char16_t>(i);}
    );
    loadTestArray<char32_t, 10>
    (
        iSerializer,
        [](std::size_t i){return static_cast<char32_t>(i);}
    );

    // 整数型
    loadTestArray<bool, 4>
    (
        iSerializer,
        [](std::size_t){return static_cast<bool>(true);}
    );
    loadTestArray<short, 5>
    (
        iSerializer,
        [](std::size_t i){return static_cast<short>(i*10);}
    );
    loadTestArray<unsigned short, 6>
    (
        iSerializer,
        [](std::size_t i){return static_cast<unsigned short>(i*10);}
    );

    loadTestArray<int, 7>
    (
        iSerializer,
        [](std::size_t i){return static_cast<int>(i*100);}
    );
    loadTestArray<unsigned int, 8>
    (
        iSerializer,
        [](std::size_t i){return static_cast<unsigned int>(i*100);}
    );

    loadTestArray<long, 9>
    (
        iSerializer,
        [](std::size_t i){return static_cast<long>(i*1000);}
    );
    loadTestArray<unsigned long, 10>
    (
        iSerializer,
        [](std::size_t i){return static_cast<unsigned long>(i*1000);}
    );

    loadTestArray<long long, 11>
    (
        iSerializer,
        [](std::size_t i){return static_cast<long long>(i*10000);}
    );
    loadTestArray<unsigned long long, 12>
    (
        iSerializer,
        [](std::size_t i){return static_cast<unsigned long long>(i*10000);}
    );

    // 浮動小数点型
    loadTestArray<float, 5>
    (
        iSerializer,
        [](std::size_t i){return static_cast<float>(i+0.1);}
    );
    loadTestArray<double, 6>
    (
        iSerializer,
        [](std::size_t i){return i+0.2;}
    );
    loadTestArray<long double, 7>
    (
        iSerializer,
        [](std::size_t i){return static_cast<long double>(i+0.3);}
    );

    // 文字列型
    loadTestArray<std::string, 5>
    (
        iSerializer,
        [](std::size_t i){return std::to_string(i);}
    );
    loadTestArray<std::wstring, 6>
    (
        iSerializer,
        [](std::size_t i){return std::to_wstring(i);}
    );
    loadTestArray<std::u16string, 7>
    (
        iSerializer,
        [](std::size_t i){return theolizer::u8string(std::to_string(i)).get_u16string();}
    );
    loadTestArray<std::u32string, 8>
    (
        iSerializer,
        [](std::size_t i){return theolizer::u8string(std::to_string(i)).get_u32string();}
    );

    // enum型
    loadTestArray<NormalEnum, 5>
    (
        iSerializer,
        [](std::size_t i){return static_cast<NormalEnum>(i % 3);}
    );
    loadTestArray<ScopedEnum, 6>
    (
        iSerializer,
        [](std::size_t i){return static_cast<ScopedEnum>(i % 3);}
    );

    // クラス型
    loadTestArray<ClassBasicTest, 5>
    (
        iSerializer,
        [](std::size_t i)
        {
            return  ClassBasicTest
                    (
                        static_cast<int>(i),
                        std::to_string(i).c_str(),
                        static_cast<NormalEnum>(i % 3)
                    );
        }
    );
}
//! [loadBasicProcess]

INSTANTIATION_ALL(loadBasicProcess);

#endif  // DISABLE_BASIC_PROCESS_TEST
