//############################################################################
//      u8string.*のテスト
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

#define THEOLIZER_INTERNAL_EXCLUDE_VERSION_H

// ***************************************************************************
//      通常のインクルード
// ***************************************************************************

#if defined(_WIN32)
    #include <windows.h>
#endif

#include <exception>
#include <iostream>
#include <sstream>
using namespace std;

#define THEOLIZER_TEST_MAIN
#include <theolizer/test_tool.h>

// ***************************************************************************
//      テストのメイン関数
// ***************************************************************************

int TestMain(int argc, char** argv)
{
//  theolizer::AutoRestoreBool aBoolRestore(theolizer::gDoDisplayPass);

    using namespace theolizer;

    cout << "TestMain()\n";
#if defined(_MSC_VER)
    cout << "_MSC_VER = " << _MSC_VER << "\n";
    cout << "_MSC_FULL_VER = " << _MSC_FULL_VER << "\n";
#endif

//----------------------------------------------------------------------------
//      準備
//
//      注意事項①：
//          マルチ・バイト文字のテストはシステム・ロケール=日本のPCでのみ実施。
//      注意事項②：
//          Windowsにおいて、aHelloCP932と一致確認しているTHEOLIZER_CHECK()
//          の結果変数(dResult)は文字化けする。
//          operator<<はUTF-8を期待しているが、Multi Byteで出力されるため。
//          結果は正しく判定するので、これは許容する。
//----------------------------------------------------------------------------

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

//----------------------------------------------------------------------------
//      internal::Converterの動作確認(これは内部機能のテスト)
//----------------------------------------------------------------------------

    {
        string      aMultiByte;
        string      aString;
        wstring     aWstring;
#if !defined(THEOLIZER_INTERNAL_NO_U16_U32)
        u16string   aU16string;
        u32string   aU32string;
#endif

//      ---<<< from Multi Byte >>---

        aMultiByte = aHelloCP932;

        // Multi Byte to UTF-8
        aString = internal::Converter<char, MultiByte>::conv(aHelloCP932);
        THEOLIZER_EQUAL(aString, u8"[こんにちは。]");
        aString = internal::Converter<char, MultiByte>::conv(aMultiByte);
        THEOLIZER_EQUAL(aString, u8"[こんにちは。]");

        // Multi Byte to UTF-16/32
        aWstring = internal::Converter<wchar_t, MultiByte>::conv(aHelloCP932);
        THEOLIZER_EQUAL(aWstring, L"[こんにちは。]");
        aWstring = internal::Converter<wchar_t, MultiByte>::conv(aMultiByte);
        THEOLIZER_EQUAL(aWstring, L"[こんにちは。]");

#if !defined(THEOLIZER_INTERNAL_NO_U16_U32)
        // Multi Byte to UTF-16
        aU16string = internal::Converter<char16_t, MultiByte>::conv(aHelloCP932);
        THEOLIZER_EQUAL(aU16string, u"[こんにちは。]");
        aU16string = internal::Converter<char16_t, MultiByte>::conv(aMultiByte);
        THEOLIZER_EQUAL(aU16string, u"[こんにちは。]");

        // Multi Byte to UTF-32
        aU32string = internal::Converter<char32_t, MultiByte>::conv(aHelloCP932);
        THEOLIZER_EQUAL(aU32string, U"[こんにちは。]");
        aU32string = internal::Converter<char32_t, MultiByte>::conv(aMultiByte);
        THEOLIZER_EQUAL(aU32string, U"[こんにちは。]");
#endif

//      ---<<< from UTF-8 >>---

        aString = u8"[こんにちは。]";

        // UTF-8 to Multi Byte
        aMultiByte = internal::Converter<MultiByte, char>::conv(u8"[こんにちは。]");
        THEOLIZER_EQUAL(aMultiByte, aHelloCP932);
        aMultiByte = internal::Converter<MultiByte, char>::conv(aString);
        THEOLIZER_EQUAL(aMultiByte, aHelloCP932);

        // UTF-8 to UTF-16/32
        aWstring = internal::Converter<wchar_t, char>::conv(u8"[こんにちは。]");
        THEOLIZER_EQUAL(aWstring, L"[こんにちは。]");
        aWstring = internal::Converter<wchar_t, char>::conv(aString);
        THEOLIZER_EQUAL(aWstring, L"[こんにちは。]");

#if !defined(THEOLIZER_INTERNAL_NO_U16_U32)
        // UTF-8 to UTF-16
        aU16string = internal::Converter<char16_t, char>::conv(u8"[こんにちは。]");
        THEOLIZER_EQUAL(aU16string, u"[こんにちは。]");
        aU16string = internal::Converter<char16_t, char>::conv(aString);
        THEOLIZER_EQUAL(aU16string, u"[こんにちは。]");

        // UTF-8 to UTF-32
        aU32string = internal::Converter<char32_t, char>::conv(u8"[こんにちは。]");
        THEOLIZER_EQUAL(aU32string, U"[こんにちは。]");
        aU32string = internal::Converter<char32_t, char>::conv(aString);
        THEOLIZER_EQUAL(aU32string, U"[こんにちは。]");
#endif

//      ---<<< from UTF-16/32 >>---

        aWstring = L"[こんにちは。]";

        // UTF-16/32 to Multi Byte
        aMultiByte = internal::Converter<MultiByte, wchar_t>::conv(L"[こんにちは。]");
        THEOLIZER_EQUAL(aMultiByte, aHelloCP932);
        aMultiByte = internal::Converter<MultiByte, wchar_t>::conv(aWstring);
        THEOLIZER_EQUAL(aMultiByte, aHelloCP932);

        // UTF-16/32 to UTF-8
        aString = internal::Converter<char, wchar_t>::conv(L"[こんにちは。]");
        THEOLIZER_EQUAL(aString, u8"[こんにちは。]");
        aString = internal::Converter<char, wchar_t>::conv(aWstring);
        THEOLIZER_EQUAL(aString, u8"[こんにちは。]");

#if !defined(THEOLIZER_INTERNAL_NO_U16_U32)
        // UTF-16/32 to UTF-16
        aU16string = internal::Converter<char16_t, wchar_t>::conv(L"[こんにちは。]");
        THEOLIZER_EQUAL(aU16string, u"[こんにちは。]");
        aU16string = internal::Converter<char16_t, wchar_t>::conv(aWstring);
        THEOLIZER_EQUAL(aU16string, u"[こんにちは。]");

        // UTF-16/32 to UTF-32
        aU32string = internal::Converter<char32_t, wchar_t>::conv(L"[こんにちは。]");
        THEOLIZER_EQUAL(aU32string, U"[こんにちは。]");
        aU32string = internal::Converter<char32_t, wchar_t>::conv(aWstring);
        THEOLIZER_EQUAL(aU32string, U"[こんにちは。]");
#endif

//      ---<<< from UTF-16 >>---

#if !defined(THEOLIZER_INTERNAL_NO_U16_U32)
        aU16string = u"[こんにちは。]";

        // UTF-16 to Multi Byte
        aMultiByte = internal::Converter<MultiByte, char16_t>::conv(u"[こんにちは。]");
        THEOLIZER_EQUAL(aMultiByte, aHelloCP932);
        aMultiByte = internal::Converter<MultiByte, char16_t>::conv(aU16string);
        THEOLIZER_EQUAL(aMultiByte, aHelloCP932);

        // UTF-16 to UTF-8
        aString = internal::Converter<char, char16_t>::conv(u"[こんにちは。]");
        THEOLIZER_EQUAL(aString, u8"[こんにちは。]");
        aString = internal::Converter<char, char16_t>::conv(aU16string);
        THEOLIZER_EQUAL(aString, u8"[こんにちは。]");

        // UTF-16 to UTF-16/32
        aWstring = internal::Converter<wchar_t, char16_t>::conv(u"[こんにちは。]");
        THEOLIZER_EQUAL(aWstring, L"[こんにちは。]");
        aWstring = internal::Converter<wchar_t, char16_t>::conv(aU16string);
        THEOLIZER_EQUAL(aWstring, L"[こんにちは。]");

        // UTF-16 to UTF-32
        aU32string = internal::Converter<char32_t, char16_t>::conv(u"[こんにちは。]");
        THEOLIZER_EQUAL(aU32string, U"[こんにちは。]");
        aU32string = internal::Converter<char32_t, char16_t>::conv(aU16string);
        THEOLIZER_EQUAL(aU32string, U"[こんにちは。]");

//      ---<<< from UTF-32 >>---

        aU32string = U"[こんにちは。]";

        // UTF-32 to Multi Byte
        aMultiByte = internal::Converter<MultiByte, char32_t>::conv(U"[こんにちは。]");
        THEOLIZER_EQUAL(aMultiByte, aHelloCP932);
        aMultiByte = internal::Converter<MultiByte, char32_t>::conv(aU32string);
        THEOLIZER_EQUAL(aMultiByte, aHelloCP932);

        // UTF-32 to UTF-8
        aString = internal::Converter<char, char32_t>::conv(U"[こんにちは。]");
        THEOLIZER_EQUAL(aString, u8"[こんにちは。]");
        aString = internal::Converter<char, char32_t>::conv(aU32string);
        THEOLIZER_EQUAL(aString, u8"[こんにちは。]");

        // UTF-32 to UTF-16/32
        aWstring = internal::Converter<wchar_t, char32_t>::conv(U"[こんにちは。]");
        THEOLIZER_EQUAL(aWstring, L"[こんにちは。]");
        aWstring = internal::Converter<wchar_t, char32_t>::conv(aU32string);
        THEOLIZER_EQUAL(aWstring, L"[こんにちは。]");
#endif
    }

//----------------------------------------------------------------------------
//      u8stringのUTF動作確認
//----------------------------------------------------------------------------

    {
        // コンストラクタ
        u8string       aUtf8_u8(   u8"c: u8文字列です-1");
        THEOLIZER_EQUAL(aUtf8_u8, u8"c: u8文字列です-1");

        string           aString = u8"c: u8文字列です-1";
        u8string        aUtf8_str(aString);
        THEOLIZER_EQUAL(aUtf8_str, u8"c: u8文字列です-1");

        u8string        aUtf8_L(    L"c:  L文字列です-1");
        THEOLIZER_EQUAL( aUtf8_L, u8"c:  L文字列です-1");

        u8string        aUtf8_u(    u"c:  u文字列です-1");
        THEOLIZER_EQUAL( aUtf8_u, u8"c:  u文字列です-1");

        u8string        aUtf8_U(    U"c:  U文字列です-1");
        THEOLIZER_EQUAL( aUtf8_U, u8"c:  U文字列です-1");

        // 明示的basic_string<他char型>取出し
                                      aUtf8_u8 = u8"g: u8文字列です-4";
        THEOLIZER_EQUAL(aUtf8_u8,                u8"g: u8文字列です-4");
        THEOLIZER_EQUAL(aUtf8_u8.get_wstring(),   L"g: u8文字列です-4");
#if !defined(THEOLIZER_INTERNAL_NO_U16_U32)
        THEOLIZER_EQUAL(aUtf8_u8.get_u16string(), u"g: u8文字列です-4");
        THEOLIZER_EQUAL(aUtf8_u8.get_u32string(), U"g: u8文字列です-4");
#endif

        // 暗黙的キャスト取出し
                               aUtf8_u8=u8"c: u8文字列です-5";
        string aUtf8_string(aUtf8_u8.str());
        THEOLIZER_EQUAL(aUtf8_string,   u8"c: u8文字列です-5");
        wstring aUtf8_wstring(aUtf8_u8);
        THEOLIZER_EQUAL(aUtf8_wstring,   L"c: u8文字列です-5");
#if !defined(THEOLIZER_INTERNAL_NO_U16_U32)
        u16string aUtf8_u16string(aUtf8_u8);
        THEOLIZER_EQUAL(aUtf8_u16string, u"c: u8文字列です-5");
        u32string aUtf8_u32string(aUtf8_u8);
        THEOLIZER_EQUAL(aUtf8_u32string, U"c: u8文字列です-5");
#endif

        aUtf8_string="";aUtf8_string=aUtf8_u8.str();
        THEOLIZER_EQUAL(aUtf8_string,   u8"c: u8文字列です-5");
        aUtf8_wstring=L"";aUtf8_wstring=aUtf8_u8;
        THEOLIZER_EQUAL(aUtf8_wstring,   L"c: u8文字列です-5");
#if !defined(THEOLIZER_INTERNAL_NO_U16_U32)
        aUtf8_u16string=u"";aUtf8_u16string=aUtf8_u8;
        THEOLIZER_EQUAL(aUtf8_u16string, u"c: u8文字列です-5");
        aUtf8_u32string=U"";aUtf8_u32string=aUtf8_u8;
        THEOLIZER_EQUAL(aUtf8_u32string, U"c: u8文字列です-5");
#endif
    }

//----------------------------------------------------------------------------
//      BasicUstring<>のMulti Byte動作確認
//          Windowsの場合のみだが、システム・ロケールの設定で動作が変わる。
//          コードページ932(Shift-JIS)にてテストする。
//----------------------------------------------------------------------------

//      ---<<< set/getACP() >>>---

    {
        THEOLIZER_EQUAL(kCP_ACP, 0);
        THEOLIZER_EQUAL(kCP_UTF8, 65001);
        bool aBool;
        unsigned aCodePage;

#if defined(_WIN32)
        aBool = setACP(65002);
        THEOLIZER_EQUAL(aBool, false);
#endif

        aBool = setACP(0);
        THEOLIZER_EQUAL(aBool, true);

        aCodePage = getACP();
#if defined(_WIN32)
        THEOLIZER_EQUAL(aCodePage, 0);
#else
        THEOLIZER_EQUAL(aCodePage, 65001);
#endif

        // Theolizerを日本語環境へ切り替える(日本語以外のWindowsでもテストできるように)
        aBool = setACP(932);
        THEOLIZER_EQUAL(aBool, true);

        aCodePage = getACP();
#if defined(_WIN32)
        THEOLIZER_EQUAL(aCodePage, 932);
#else
        THEOLIZER_EQUAL(aCodePage, 65001);
#endif
    }

//      ---<<< MultiByte >>>---

    {
        // set/getMultiByte()
        u8string  aUtf8_mb(aHelloCP932, theolizer::MultiByte());
        THEOLIZER_EQUAL(aUtf8_mb, u8"[こんにちは。]");

        aUtf8_mb="";
        aUtf8_mb.setMultiByte(aHelloCP932);
        THEOLIZER_EQUAL(aUtf8_mb, u8"[こんにちは。]");

        u8string  aUtf8_u8 = u8"[こんにちは。]";
        THEOLIZER_EQUAL(aUtf8_u8.getMultiByte(), aHelloCP932);

        // 各char*取出し
        string aUtf8_string(aUtf8_u8.b_str());
        THEOLIZER_EQUAL(aUtf8_string, aHelloCP932);
        wstring aUtf8_wstring(aUtf8_u8.b_str());
        THEOLIZER_EQUAL(aUtf8_wstring,  L"[こんにちは。]");
#if !defined(THEOLIZER_INTERNAL_NO_U16_U32)
        u16string aUtf8_u16string(aUtf8_u8.b_str());
        THEOLIZER_EQUAL(aUtf8_u16string, u"[こんにちは。]");
        u32string aUtf8_u32string(aUtf8_u8.b_str());
        THEOLIZER_EQUAL(aUtf8_u32string, U"[こんにちは。]");
#endif
#if defined(_WIN32)
        u8string    aFileName(u8"ファイル名.txt");
        HANDLE hHandle = CreateFile(aFileName.b_str(), GENERIC_WRITE, 0, NULL,
                                    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        THEOLIZER_CHECK(hHandle != INVALID_HANDLE_VALUE, hHandle);
        CloseHandle(hHandle);
        THEOLIZER_CHECK(isExist(aFileName), isExist(aFileName));
        removeFile(aFileName);  // (後始末)
        THEOLIZER_CHECK(!isExist(aFileName), isExist(aFileName));
#endif

        // std::fstream対応
#if defined(_WIN32)
    #if defined(_MSC_VER)
        wstring aWstring = aUtf8_u8.get_fstring();
        THEOLIZER_EQUAL(aWstring, L"[こんにちは。]");
    #else
        string  aString = aUtf8_u8.get_fstring();
        THEOLIZER_EQUAL(aString, aHelloCP932);
    #endif
#else
        string  aString = aUtf8_u8.get_fstring();
        THEOLIZER_EQUAL(aString, u8"[こんにちは。]");
#endif

        // boost::filesystem::path対応
#if defined(_WIN32)
        wstring  aWString = aUtf8_u8.get_bstring();
        THEOLIZER_EQUAL(aWString, L"[こんにちは。]");
#else
        aString = aUtf8_u8.get_bstring();
        THEOLIZER_EQUAL(aString, u8"[こんにちは。]");
#endif
    }

//----------------------------------------------------------------------------
//      各種関数
//----------------------------------------------------------------------------

    {
        u8string aU8string;

        // 代入演算子(文字列)
                       aU8string =  u8"u8テスト";
        THEOLIZER_EQUAL(aU8string, u8"u8テスト");
                        aU8string =   L"Lテスト";
        THEOLIZER_EQUAL( aU8string, u8"Lテスト");
#if !defined(THEOLIZER_INTERNAL_NO_U16_U32)
                        aU8string =   u"uテスト";
        THEOLIZER_EQUAL( aU8string, u8"uテスト");
                        aU8string =   U"Uテスト";
        THEOLIZER_EQUAL( aU8string, u8"Uテスト");
#endif

        // +=演算子
        aU8string="";
                        aU8string +=u8"u8テスト";
        THEOLIZER_EQUAL(aU8string, u8"u8テスト");
                        aU8string +=  L"Lテスト";
        THEOLIZER_EQUAL(aU8string, u8"u8テストLテスト");
#if !defined(THEOLIZER_INTERNAL_NO_U16_U32)
                        aU8string +=  u"uテスト";
        THEOLIZER_EQUAL(aU8string, u8"u8テストLテストuテスト");
                        aU8string +=  U"Uテスト";
        THEOLIZER_EQUAL(aU8string, u8"u8テストLテストuテストUテスト");
#endif

        // append
        aU8string="";
                    aU8string.append(u8"u8テスト");
        THEOLIZER_EQUAL( aU8string, u8"u8テスト");
                      aU8string.append(L"Lテスト");
        THEOLIZER_EQUAL( aU8string, u8"u8テストLテスト");
#if !defined(THEOLIZER_INTERNAL_NO_U16_U32)
                      aU8string.append(u"uテスト");
        THEOLIZER_EQUAL( aU8string, u8"u8テストLテストuテスト");
                      aU8string.append(U"Uテスト");
        THEOLIZER_EQUAL( aU8string, u8"u8テストLテストuテストUテスト");
#endif

        // assign
                    aU8string.assign(u8"u8テスト");
        THEOLIZER_EQUAL( aU8string, u8"u8テスト");
                      aU8string.assign(L"Lテスト");
        THEOLIZER_EQUAL(  aU8string, u8"Lテスト");
#if !defined(THEOLIZER_INTERNAL_NO_U16_U32)
                      aU8string.assign(u"uテスト");
        THEOLIZER_EQUAL(  aU8string, u8"uテスト");
                      aU8string.assign(U"Uテスト");
        THEOLIZER_EQUAL(  aU8string, u8"Uテスト");
#endif

        // global operator+
        aU8string=u8string(u8"左") + u8string(L"右");
        THEOLIZER_EQUAL( aU8string, u8"左右");
        aU8string=u8string( u"左") + u8string(U"右");
        THEOLIZER_EQUAL( aU8string, u8"左右");

        aU8string=u8string(u8"左") + u8"右";
        THEOLIZER_EQUAL( aU8string, u8"左右");
        aU8string=u8string(u8"左") +  L"右";
        THEOLIZER_EQUAL( aU8string, u8"左右");
#if !defined(THEOLIZER_INTERNAL_NO_U16_U32)
        aU8string=u8string(u8"左") +  u"右";
        THEOLIZER_EQUAL( aU8string, u8"左右");
        aU8string=u8string(u8"左") +  U"右";
        THEOLIZER_EQUAL( aU8string, u8"左右");
#endif

        aU8string=u8"左" + u8string(u8"右");
        THEOLIZER_EQUAL( aU8string, u8"左右");
        aU8string=L"左"  + u8string(u8"右");
        THEOLIZER_EQUAL( aU8string, u8"左右");
#if !defined(THEOLIZER_INTERNAL_NO_U16_U32)
        aU8string=u"左"  + u8string(u8"右");
        THEOLIZER_EQUAL( aU8string, u8"左右");
        aU8string=U"左"  + u8string(u8"右");
        THEOLIZER_EQUAL( aU8string, u8"左右");
#endif

        // global operator==
        #define BOOL_CHECK(dJudge)  THEOLIZER_CHECK(dJudge, (dJudge));
        BOOL_CHECK(u8string(u8"テスト") == u8string(L"テスト"));
#if !defined(THEOLIZER_INTERNAL_NO_U16_U32)
        BOOL_CHECK( u8string(u"テスト") == u8string(U"テスト"));
#endif

        BOOL_CHECK(u8string(u8"テスト") == u8"テスト");
        BOOL_CHECK(u8string(u8"テスト") ==  L"テスト");
#if !defined(THEOLIZER_INTERNAL_NO_U16_U32)
        BOOL_CHECK(u8string(u8"テスト") ==  u"テスト");
        BOOL_CHECK(u8string(u8"テスト") ==  U"テスト");
#endif

        BOOL_CHECK(u8"テスト" == u8string(u8"テスト"));
        BOOL_CHECK( L"テスト" == u8string(u8"テスト"));
#if !defined(THEOLIZER_INTERNAL_NO_U16_U32)
        BOOL_CHECK( u"テスト" == u8string(u8"テスト"));
        BOOL_CHECK( U"テスト" == u8string(u8"テスト"));
#endif

        // global operator!=
        BOOL_CHECK(u8string(u8"テスタ") != u8string(L"テスト"));
        BOOL_CHECK(u8string(u8"テスタ") != u8"テスト");
        BOOL_CHECK(u8string(u8"テスタ") !=  L"テスト");
        BOOL_CHECK( u8"テスタ" != u8string(u8"テスト"));
        BOOL_CHECK(  L"テスタ" != u8string(u8"テスト"));

        // global operator<
        BOOL_CHECK(u8string(u8"テスタ") < u8string(L"テスト"));
        BOOL_CHECK(u8string(u8"テスタ") < u8"テスト");
        BOOL_CHECK(u8string(u8"テスタ") <  L"テスト");
        BOOL_CHECK( u8"テスタ" < u8string(u8"テスト"));
        BOOL_CHECK(  L"テスタ" < u8string(u8"テスト"));

        // global operator<=
        BOOL_CHECK(u8string(u8"テスタ") <= u8string(L"テスト"));
        BOOL_CHECK(u8string(u8"テスタ") <= u8"テスト");
        BOOL_CHECK(u8string(u8"テスタ") <=  L"テスト");
        BOOL_CHECK( u8"テスタ" <= u8string(u8"テスト"));
        BOOL_CHECK(  L"テスタ" <= u8string(u8"テスト"));

        // global operator>
        BOOL_CHECK(u8string(u8"テスト") > u8string(L"テスタ"));
        BOOL_CHECK(u8string(u8"テスト") > u8"テスタ");
        BOOL_CHECK(u8string(u8"テスト") >  L"テスタ");
        BOOL_CHECK( u8"テスト" > u8string(u8"テスタ"));
        BOOL_CHECK(  L"テスト" > u8string(u8"テスタ"));

        // global operator>=
        BOOL_CHECK(u8string(u8"テスト") >= u8string(L"テスタ"));
        BOOL_CHECK(u8string(u8"テスト") >= u8"テスタ");
        BOOL_CHECK(u8string(u8"テスト") >=  L"テスタ");
        BOOL_CHECK( u8"テスト" >= u8string(u8"テスタ"));
        BOOL_CHECK(  L"テスト" >= u8string(u8"テスタ"));
    }

//----------------------------------------------------------------------------
//      operator<<, >>の動作確認
//          http://stackoverflow.com/questions/5257509/freopen-equivalent-for-c-streams
//          freopenが便利
//          streambufをすげ替えるともっと便利
//----------------------------------------------------------------------------

    bool aDo;
    bool aIsUtf8;

//      ---<<< istream/ostreadの登録/解除 >>>---
//      (前節で定義したaHelloCP932を再利用する。)

    {

//      ---<<< 標準入出力デフォルト >>>---

        // デフォルト・チェック
        aIsUtf8 = isInputByUtf8(&cin);
        THEOLIZER_EQUAL(aIsUtf8, false);

        aIsUtf8 = isOutputByUtf8(&cout);
        THEOLIZER_EQUAL(aIsUtf8, false);

        aIsUtf8 = isOutputByUtf8(&cerr);
        THEOLIZER_EQUAL(aIsUtf8, false);

//      ---<<< istream登録/解除 >>>---

        // istream解除
        aDo=unregisterStream(&cin);
        THEOLIZER_EQUAL(aDo, true);

        // istream再解除
        aDo=unregisterStream(&cin);
        THEOLIZER_EQUAL(aDo, false);

        // istream解除チェック
        aIsUtf8 = isInputByUtf8(&cin);
        THEOLIZER_EQUAL(aIsUtf8, true);

        // istream登録
        aDo=registerStream(&cin);
        THEOLIZER_EQUAL(aDo, true);

        // istream再登録
        aDo=registerStream(&cin);
        THEOLIZER_EQUAL(aDo, false);

        // istream登録チェック
        aIsUtf8 = isInputByUtf8(&cin);
        THEOLIZER_EQUAL(aIsUtf8, false);

//      ---<<< ostream登録/解除 >>>---

        // ostream解除
        aDo=unregisterStream(&cout);
        THEOLIZER_EQUAL(aDo, true);

        // ostream再解除
        aDo=unregisterStream(&cout);
        THEOLIZER_EQUAL(aDo, false);

        // ostream解除チェック
        aIsUtf8 = isOutputByUtf8(&cout);
        THEOLIZER_EQUAL(aIsUtf8, true);

        // ostream登録
        aDo=registerStream(&cout);
        THEOLIZER_EQUAL(aDo, true);

        // ostream再登録
        aDo=registerStream(&cout);
        THEOLIZER_EQUAL(aDo, false);

        // ostream登録チェック
        aIsUtf8 = isOutputByUtf8(&cout);
        THEOLIZER_EQUAL(aIsUtf8, false);
    }

//      ---<<< 登録時の入出力テスト >>>---

    stringstream    ss;

#if 0
cout << "stringstream::goodbit = " << stringstream::goodbit << "\n";
cout << "stringstream::eofbit = " << stringstream::eofbit << "\n";
cout << "stringstream::failbit = " << stringstream::failbit << "\n";
cout << "stringstream::badbit = " << stringstream::badbit << "\n";
cout << "rdstate()=" << ss.rdstate() << "\n";
#endif

    {
        aDo=registerStream(&ss);
        THEOLIZER_EQUAL(aDo, true);

        aIsUtf8 = isInputByUtf8(&ss);
        THEOLIZER_EQUAL(aIsUtf8, false);

        aIsUtf8 = isOutputByUtf8(&ss);
        THEOLIZER_EQUAL(aIsUtf8, false);

        // UTF-8(リテラル)
        ss.str("");
        ss.clear();
        ss << u8"[こんにちは。]";
        THEOLIZER_EQUAL(ss.str(), aHelloCP932);
        string  aUtf8In;
        ss >> aUtf8In;
        THEOLIZER_EQUAL(aUtf8In, u8"[こんにちは。]");

        // UTF-8(string)
        ss.str("");
        ss.clear();
        string  aUtf8Out(u8"[こんにちは。]");
        ss << aUtf8Out;
        THEOLIZER_EQUAL(ss.str(), aHelloCP932);
        aUtf8In="";
        ss >> aUtf8In;
        THEOLIZER_EQUAL(aUtf8In, u8"[こんにちは。]");

        // UTF-8(u8string)
        ss.str("");
        ss.clear();
        u8string  aU8stringOut(u8"[こんにちは。]");
        ss << aU8stringOut;
        THEOLIZER_EQUAL(ss.str(), aHelloCP932);
        u8string  aU8stringIn;
        ss >> aU8stringIn;
        THEOLIZER_EQUAL(aU8stringIn, u8"[こんにちは。]");

        // UTF-16/32(リテラル)
        ss.str("");
        ss.clear();
        ss << L"[こんにちは。]";
        THEOLIZER_EQUAL(ss.str(), aHelloCP932);
        wstring aWstringIn;
        ss >> aWstringIn;
        THEOLIZER_EQUAL(aWstringIn, L"[こんにちは。]");

        // UTF-16/32(wstring)
        ss.str("");
        ss.clear();
        wstring  aWstringOut(L"[こんにちは。]");
        ss << aWstringOut;
        THEOLIZER_EQUAL(ss.str(), aHelloCP932);
        aWstringIn=L"";
        ss >> aWstringIn;
        THEOLIZER_EQUAL(aWstringIn, L"[こんにちは。]");

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)  // MSVC 2015 == 1900
        // UTF-16(リテラル)
        ss.str("");
        ss.clear();
        ss << u"[こんにちは。]";
        THEOLIZER_EQUAL(ss.str(), aHelloCP932);
        u16string aUtf16stringIn;
        ss >> aUtf16stringIn;
        THEOLIZER_EQUAL(aUtf16stringIn, u"[こんにちは。]");

        // UTF-16(u16string)
        ss.str("");
        ss.clear();
        u16string  aUtf16stringOut(u"[こんにちは。]");
        ss << aUtf16stringOut;
        THEOLIZER_EQUAL(ss.str(), aHelloCP932);
        aUtf16stringIn=u"";
        ss >> aUtf16stringIn;
        THEOLIZER_EQUAL(aUtf16stringIn, u"[こんにちは。]");

        // UTF-32(リテラル)
        ss.str("");
        ss.clear();
        ss << U"[こんにちは。]";
        THEOLIZER_EQUAL(ss.str(), aHelloCP932);
        u32string aUtf32stringIn;
        ss >> aUtf32stringIn;
        THEOLIZER_EQUAL(aUtf32stringIn, U"[こんにちは。]");

        // UTF-32(u32string)
        ss.str("");
        ss.clear();
        u32string  aUtf32stringOut(U"[こんにちは。]");
        ss << aUtf32stringOut;
        THEOLIZER_EQUAL(ss.str(), aHelloCP932);
        aUtf32stringIn=U"";
        ss >> aUtf32stringIn;
        THEOLIZER_EQUAL(aUtf32stringIn, U"[こんにちは。]");
#endif
    }

//      ---<<< 解除時の入出力テスト >>>---

    {
        aDo=unregisterStream(&ss);
        THEOLIZER_EQUAL(aDo, true);

        aIsUtf8 = isInputByUtf8(&ss);
        THEOLIZER_EQUAL(aIsUtf8, true);

        aIsUtf8 = isOutputByUtf8(&ss);
        THEOLIZER_EQUAL(aIsUtf8, true);

        // Multi Byte
        ss.str("");
        ss.clear();
        ss << aHelloCP932;
        THEOLIZER_EQUAL(ss.str(), aHelloCP932);
        string  aMultiByteIn;
        ss >> aMultiByteIn;
        THEOLIZER_EQUAL(aMultiByteIn, aHelloCP932);

        ss.str("");
        ss.clear();
        string  aMultiByteOut(aHelloCP932);
        ss << aMultiByteOut;
        THEOLIZER_EQUAL(ss.str(), aHelloCP932);
        aMultiByteIn="";
        ss >> aMultiByteIn;
        THEOLIZER_EQUAL(aMultiByteIn, aHelloCP932);

        // UTF-8(リテラル)
        ss.str("");
        ss.clear();
        ss << u8"[こんにちは。]";
        THEOLIZER_EQUAL(ss.str(), u8"[こんにちは。]");
        string  aUtf8In;
        ss >> aUtf8In;
        THEOLIZER_EQUAL(aUtf8In, u8"[こんにちは。]");

        // UTF-8(string)
        ss.str("");
        ss.clear();
        string  aUtf8Out(u8"[こんにちは。]");
        ss << aUtf8Out;
        THEOLIZER_EQUAL(ss.str(), u8"[こんにちは。]");
        aUtf8In="";
        ss >> aUtf8In;
        THEOLIZER_EQUAL(aUtf8In, u8"[こんにちは。]");

        // UTF-8(u8string)
        ss.str("");
        ss.clear();
        u8string  aU8stringOut(u8"[こんにちは。]");
        ss << aU8stringOut;
        THEOLIZER_EQUAL(ss.str(), u8"[こんにちは。]");
        u8string  aU8stringIn;
        ss >> aU8stringIn;
        THEOLIZER_EQUAL(aU8stringIn, u8"[こんにちは。]");

        // UTF-16/32(リテラル)
        ss.str("");
        ss.clear();
        ss << L"[こんにちは。]";
        THEOLIZER_EQUAL(ss.str(), u8"[こんにちは。]");
        wstring aWstringIn;
        ss >> aWstringIn;
        THEOLIZER_EQUAL(aWstringIn, L"[こんにちは。]");

        // UTF-16/32(wstring)
        ss.str("");
        ss.clear();
        wstring  aWstringOut(L"[こんにちは。]");
        ss << aWstringOut;
        THEOLIZER_EQUAL(ss.str(), u8"[こんにちは。]");
        aWstringIn=L"";
        ss >> aWstringIn;
        THEOLIZER_EQUAL(aWstringIn, L"[こんにちは。]");

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)  // MSVC 2015 == 1900
        // UTF-16(リテラル)
        ss.str("");
        ss.clear();
        ss << u"[こんにちは。]";
        THEOLIZER_EQUAL(ss.str(), u8"[こんにちは。]");
        u16string aUtf16stringIn;
        ss >> aUtf16stringIn;
        THEOLIZER_EQUAL(aUtf16stringIn, u"[こんにちは。]");

        // UTF-16(u16string)
        ss.str("");
        ss.clear();
        u16string  aUtf16stringOut(u"[こんにちは。]");
        ss << aUtf16stringOut;
        THEOLIZER_EQUAL(ss.str(), u8"[こんにちは。]");
        aUtf16stringIn=u"";
        ss >> aUtf16stringIn;
        THEOLIZER_EQUAL(aUtf16stringIn, u"[こんにちは。]");

        // UTF-32(リテラル)
        ss.str("");
        ss.clear();
        ss << U"[こんにちは。]";
        THEOLIZER_EQUAL(ss.str(), u8"[こんにちは。]");
        u32string aUtf32stringIn;
        ss >> aUtf32stringIn;
        THEOLIZER_EQUAL(aUtf32stringIn, U"[こんにちは。]");

        // UTF-32(u32string)
        ss.str("");
        ss.clear();
        u32string  aUtf32stringOut(U"[こんにちは。]");
        ss << aUtf32stringOut;
        THEOLIZER_EQUAL(ss.str(), u8"[こんにちは。]");
        aUtf32stringIn=U"";
        ss >> aUtf32stringIn;
        THEOLIZER_EQUAL(aUtf32stringIn, U"[こんにちは。]");
#endif
    }

    return theolizer::kExitSuccess;
}
