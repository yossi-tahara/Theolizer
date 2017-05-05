//############################################################################
//      文字エンコード・コンバート
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
//      インクルード
//############################################################################

#include "avoid-trouble.h"

#include <cstdio>

#include <algorithm>            // for find

#if defined(_WIN32)
    #include <windows.h>
#endif

#define BOOST_ALL_NO_LIB        // Auto-linkを禁止する
                                //  #pragma comment(lib ...)があると
                                //  exeのリンク時にリンクされてしまう。
THEOLIZER_INTERNAL_DISABLE_WARNING()
#include <boost/locale.hpp>

#include "internal.h"
#include "../u8string.h"

//############################################################################
//      Begin
//############################################################################

namespace theolizer
{
    namespace boostLC=boost::locale::conv;

//############################################################################
//      Multi Byte文字エンコード管理
//############################################################################

// ***************************************************************************
//      Multi Byte変換するistream/ostreamの管理
//          デフォルトでstd::cin, std:cout, std:cerrを登録する。
// ***************************************************************************

namespace
{

class MultiByteManager
{
private:

//----------------------------------------------------------------------------
//      シングルトン・バターン
//----------------------------------------------------------------------------

    // コンストラクタ／デストラクタ
    MultiByteManager() : mCodePage(kCP_UTF8)
    {
#if defined(_WIN32)
        // コードページの初期値
        mCodePage = ::GetACP();
#endif
        // cin, cout, cerrは変換対象とする
        mIStreams.push_back(&std::cin);
        mOStreams.push_back(&std::cout);
        mOStreams.push_back(&std::cerr);
    }

    ~MultiByteManager() { }

    // 起動時に初期化させるため
    static MultiByteManager& mInstance;
    static void use(const MultiByteManager&)// {}
    { } // この中でcout, cerrへ出力すると、Windowsでは無限ループになるので要注意

public:
    // インスタンス獲得
    static MultiByteManager& getInstance()
    {
        static MultiByteManager instance;
        use(mInstance);
        return instance;
    }

    // コピー／ムーブ禁止(デストラクタ定義がないとis_trivially_copyableになるので注意)
    MultiByteManager(const MultiByteManager&) = delete;
    void operator=  (const MultiByteManager&) = delete;
    MultiByteManager(MultiByteManager&&) = delete;
    void operator=  (MultiByteManager&&) = delete;

//----------------------------------------------------------------------------
//      管理機能実体
//----------------------------------------------------------------------------

//      ---<<< コードページ管理 >>>---

    unsigned                            mCodePage;

//      ---<<< istream管理 >>>---

    typedef std::vector<std::istream*>  IStreams;
    IStreams                            mIStreams;
    // iIStreamsが登録されていたらtrue返却
    bool find(std::istream* iIStream, IStreams::iterator& oFound)
    {
        oFound = std::find(mIStreams.begin(), mIStreams.end(), iIStream);
        return (oFound != mIStreams.end()) && (*oFound == iIStream);
    }
    // iIStream登録
    bool registerStream(std::istream* iIStream)
    {
        IStreams::iterator found; 
        if (find(iIStream, found))
        {
    return false;
        }

        mIStreams.insert(found, iIStream);
        return true;
    }
    // iIStream解除
    bool unregisterStream(std::istream* iIStream)
    {
        IStreams::iterator found; 
        if (!find(iIStream, found))
        {
    return false;
        }

        mIStreams.erase(found);
        return true;
    }

//      ---<<< ostream管理 >>>---

    typedef std::vector<std::ostream*> OStreams;
    OStreams                            mOStreams;
    // iOStreamが登録されていたらtrue返却
    bool find(std::ostream* iOStream, OStreams::iterator& oFound)
    {
        oFound = std::find(mOStreams.begin(), mOStreams.end(), iOStream);
        return (oFound != mOStreams.end()) && (*oFound == iOStream);
    }
    // iOStream登録
    bool registerStream(std::ostream* iOStream)
    {
        OStreams::iterator found; 
        if (find(iOStream, found))
        {
    return false;
        }

        mOStreams.insert(found, iOStream);
        return true;
    }
    // iOStream解除
    bool unregisterStream(std::ostream* iOStream)
    {
        OStreams::iterator found; 
        if (!find(iOStream, found))
        {
    return false;
        }

        mOStreams.erase(found);
        return true;
    }
};

// 起動時生成
MultiByteManager& MultiByteManager::mInstance = MultiByteManager::getInstance();

}   // namespace

// ***************************************************************************
//      Multi Byteのコードページ管理
// ***************************************************************************

#if defined(_WIN32)
    const unsigned kCP_ACP = CP_ACP;
    const unsigned kCP_UTF8 = CP_UTF8;
#else
    const unsigned kCP_ACP = 0;         // Don't care
    const unsigned kCP_UTF8 = 65001;
#endif

unsigned getACP()
{
    return MultiByteManager::getInstance().mCodePage;
}

bool setACP(unsigned iCodePage)
{
#if defined(_WIN32)
    const int len = ::MultiByteToWideChar(iCodePage,
                                          0, "Dummy", -1, NULL, 0);
    if (len == 0)
    {
return false;
    }
    MultiByteManager::getInstance().mCodePage = iCodePage;
#endif
    return true;
}

// ***************************************************************************
//      istream/ostream管理I/F
// ***************************************************************************

//      ---<<< iostream管理 >>>---

//      ---<<< i/ostreamの登録 >>>---

bool registerStream(std::istream* iIStream)
{
    return MultiByteManager::getInstance().registerStream(iIStream);
}

bool registerStream(std::ostream* iOStream)
{
    return MultiByteManager::getInstance().registerStream(iOStream);
}
// iIOStream登録
bool registerStream(std::iostream* iIOStream)
{
    bool ret0 = MultiByteManager::getInstance().registerStream(
                                static_cast<std::istream*>(iIOStream));
    bool ret1 = MultiByteManager::getInstance().registerStream(
                                static_cast<std::ostream*>(iIOStream));
    return ret0||ret1;
}

//      ---<<< i/ostreamの解除 >>>---

bool unregisterStream(std::istream* iIStream)
{
    return MultiByteManager::getInstance().unregisterStream(iIStream);
}

bool unregisterStream(std::ostream* iOStream)
{
    return MultiByteManager::getInstance().unregisterStream(iOStream);
}

// iIOStream解除
bool unregisterStream(std::iostream* iIOStream)
{
    bool ret0 = MultiByteManager::getInstance().unregisterStream(
                                static_cast<std::istream*>(iIOStream));
    bool ret1 = MultiByteManager::getInstance().unregisterStream(
                                static_cast<std::ostream*>(iIOStream));
    return ret0||ret1;
}

//      ---<<< UTF-8でやりとりするi/ostream判定 >>>---

bool isInputByUtf8(std::istream* iIStream)
{
    MultiByteManager::IStreams::iterator found; 
    return !MultiByteManager::getInstance().find(iIStream, found);
}

bool isOutputByUtf8(std::ostream* iOStream)
{
    MultiByteManager::OStreams::iterator found; 
    return !MultiByteManager::getInstance().find(iOStream, found);
}

//############################################################################
//      文字列エンコード・コンバーター
//############################################################################

namespace internal
{

// ***************************************************************************
//      UTF-8/UTF-16/32用クラス・テンプレート
// ***************************************************************************

//      ---<<< 関数定義 >>>---

template<class tCharOut, class tCharIn>
std::basic_string<tCharOut> Converter<tCharOut, tCharIn>::conv(const tCharIn* iIn) noexcept
{
    return boostLC::utf_to_utf<tCharOut, tCharIn>(iIn);
}

//      ---<<< 実体化 >>>---

//      --- from char* ---

template std::string    Converter<char,     char>::conv(char const* iIn) noexcept;
template std::wstring   Converter<wchar_t,  char>::conv(char const* iIn) noexcept;
template std::u16string Converter<char16_t, char>::conv(char const* iIn) noexcept;
template std::u32string Converter<char32_t, char>::conv(char const* iIn) noexcept;

//      --- from wchar_t* ---

template std::string    Converter<char,     wchar_t>::conv(wchar_t const* iIn) noexcept;
template std::wstring   Converter<wchar_t,  wchar_t>::conv(wchar_t const* iIn) noexcept;    //--
template std::u16string Converter<char16_t, wchar_t>::conv(wchar_t const* iIn) noexcept;    //xx
template std::u32string Converter<char32_t, wchar_t>::conv(wchar_t const* iIn) noexcept;    //xx

//      --- from char16_t* ---

template std::string    Converter<char,     char16_t>::conv(char16_t const* iIn) noexcept;
template std::wstring   Converter<wchar_t,  char16_t>::conv(char16_t const* iIn) noexcept;  //xx
template std::u16string Converter<char16_t, char16_t>::conv(char16_t const* iIn) noexcept;  //--
template std::u32string Converter<char32_t, char16_t>::conv(char16_t const* iIn) noexcept;  //xx

//      --- from char32_t* ---

template std::string    Converter<char,     char32_t>::conv(char32_t const* iIn) noexcept;
template std::wstring   Converter<wchar_t,  char32_t>::conv(char32_t const* iIn) noexcept;  //xx
template std::u16string Converter<char16_t, char32_t>::conv(char32_t const* iIn) noexcept;  //xx
template std::u32string Converter<char32_t, char32_t>::conv(char32_t const* iIn) noexcept;  //--

// ***************************************************************************
//      Multi Byte用特殊化(Multi Byteを入力する)
// ***************************************************************************

//      ---<<< 関数定義 >>>---

template<class tCharOut>
std::basic_string<tCharOut> Converter<tCharOut, MultiByte>::conv(char const* iIn) noexcept
{
#if defined(_WIN32)
    const int len = ::MultiByteToWideChar(MultiByteManager::getInstance().mCodePage,
                                          0, iIn, -1, NULL, 0);
    std::wstring aUtf16;
    aUtf16.resize(len);
    ::MultiByteToWideChar(MultiByteManager::getInstance().mCodePage,
                          0, iIn, -1, &*aUtf16.begin(), len);
    aUtf16.resize(len-1);
    return boostLC::utf_to_utf<tCharOut, wchar_t>(aUtf16);
#else
    return boostLC::utf_to_utf<tCharOut, char>(iIn);
#endif
}

//      ---<<< 実体化 >>>---

template std::string    Converter<char,     MultiByte>::conv(char const* iIn) noexcept;
template std::wstring   Converter<wchar_t,  MultiByte>::conv(char const* iIn) noexcept;
template std::u16string Converter<char16_t, MultiByte>::conv(char const* iIn) noexcept;
template std::u32string Converter<char32_t, MultiByte>::conv(char const* iIn) noexcept;

// ***************************************************************************
//      Multi Byte用特殊化(Multi Byteを出力する)
// ***************************************************************************

//      ---<<< 関数定義 >>>---

template<class tCharIn>
std::string Converter<MultiByte, tCharIn>::conv(const tCharIn* iIn) noexcept
{
#if defined(_WIN32)
    std::wstring aWstring = boostLC::utf_to_utf<wchar_t, tCharIn>(iIn);
    const int len=::WideCharToMultiByte(MultiByteManager::getInstance().mCodePage,
                                        0, aWstring.c_str(), -1, NULL, 0, NULL, NULL);
    std::string aMultiByte;
    aMultiByte.resize(len);
    ::WideCharToMultiByte(MultiByteManager::getInstance().mCodePage,
                          0, aWstring.c_str(), -1, &*aMultiByte.begin(), len, NULL, NULL);
    aMultiByte.resize(len-1);
    return aMultiByte;
#else
    return boostLC::utf_to_utf<char, tCharIn>(iIn);;
#endif
}

//      ---<<< 実体化 >>>---

template std::string Converter<MultiByte, char    >::conv(char     const* iIn) noexcept;
template std::string Converter<MultiByte, wchar_t >::conv(wchar_t  const* iIn) noexcept;    //xx
template std::string Converter<MultiByte, char16_t>::conv(char16_t const* iIn) noexcept;    //xx
template std::string Converter<MultiByte, char32_t>::conv(char32_t const* iIn) noexcept;    //xx

//############################################################################
//      End
//############################################################################

}   //namespace internal
}   // namespace theolizer
