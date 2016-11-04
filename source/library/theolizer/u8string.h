//############################################################################
/*!
    @file
    @defgroup Unicode Unicode文字列補助機能

    @todo T.B.D.
*/
/*
@par ①u8string：下記文字エンコード間のコンバート機能を提供する
    char            UTF-8\n
    wchar_t         UTF-16/32\n
    char16_t        UTF-16\n
    char32_t        UTF-32\n

@par ②Windows用Multi Byte対応機能を提供する
    registerStream()にて登録したiostreamに対して、
    Windows APIを用いてu8stringのMulti Byteとのコンバートを行う。\n
    デフォルトにて、std::cint, std::cout, std::cerrを登録している。\n
    - std::istream : ulti Byte → UTF-8, UTF-16, UTF-32\n
    - std::ostream : UTF-8, UTF-16, UTF-32 → Multi Byte\n

@par ③コンバート・エラー発生時、ConvertError例外を投げる

@note
Visual Studioのchar型文字列リテラルは、デフォルトではMulti Byte文字エンコードである。\n
Visual Studio 2015以降ならば、u8"任意文字列"の書式にてUTF-8文字エンコードできる。\n

*/

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

#if !defined(THEOLIZER_INTERNAL_U8STRING_H)
#define THEOLIZER_INTERNAL_U8STRING_H

#include "impl/external.h"          // インクルード漏れチェックのため

#include <string>
#include <iostream>

//############################################################################
//      Begin
//############################################################################

// ***************************************************************************
//          DLL用の警告禁止
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable:4251)
#endif

namespace theolizer
{

//############################################################################
//      Multi Byte文字エンコード管理
//############################################################################

// ***************************************************************************
//      Multi Byteのコードページ管理
//          下記Windows APIで用いるコードページを管理する
//              MultiByteToWideChar
//              WideCharToMultiByte
//          デフォルトは::GetACP()で取得した値(システム・ロケールのコードページ)
//              非Windowsの場合、65001(UTF-8)固定
// ***************************************************************************

/*!
    @todo   T.B.D.
*/
extern const THEOLIZER_INTERNAL_DLL unsigned kCP_ACP;   // WindowsのCP_ACPと同じ値
/*!
    @todo   T.B.D.
*/
extern const THEOLIZER_INTERNAL_DLL unsigned kCP_UTF8;  // WindowsのCP_UTF8と同じ値

/*!
    @todo   T.B.D.
*/
unsigned THEOLIZER_INTERNAL_DLL getACP();
/*!
    @todo   T.B.D.
*/
bool THEOLIZER_INTERNAL_DLL setACP(unsigned iCodePage); // 失敗時false返却(非Windowsは常にtrue)

// ***************************************************************************
//      istream/ostream管理
//          Multi Byte変換するistream/ostreamを登録する。
// ***************************************************************************

//      ---<<< i/ostreamの登録 >>>---

/*!
    @ingroup Unicode
    @todo   T.B.D.
*/
bool THEOLIZER_INTERNAL_DLL registerStream(std::istream* iIStream);
/*!
    @ingroup Unicode
    @todo   T.B.D.
*/
bool THEOLIZER_INTERNAL_DLL registerStream(std::ostream* iOStream);
/*!
    @ingroup Unicode
    @todo   T.B.D.
*/
bool THEOLIZER_INTERNAL_DLL registerStream(std::iostream* iIOStream);

//      ---<<< i/ostreamの解除 >>>---

/*!
    @ingroup Unicode
    @todo   T.B.D.
*/
bool THEOLIZER_INTERNAL_DLL unregisterStream(std::istream* iIStream);
/*!
    @ingroup Unicode
    @todo   T.B.D.
*/
bool THEOLIZER_INTERNAL_DLL unregisterStream(std::ostream* iOStream);
/*!
    @ingroup Unicode
    @todo   T.B.D.
*/
bool THEOLIZER_INTERNAL_DLL unregisterStream(std::iostream* iIOStream);

//      ---<<< UTF-8でやりとりするi/ostream判定 >>>---

/*!
    @ingroup Unicode
    @todo   T.B.D.
*/
bool THEOLIZER_INTERNAL_DLL isInputByUtf8(std::istream* iIStream);
/*!
    @ingroup Unicode
    @todo   T.B.D.
*/
bool THEOLIZER_INTERNAL_DLL isOutputByUtf8(std::ostream* iOStream);

//############################################################################
//      文字列エンコード・コンバーター
//          tCharIn*を受け取り、文字エンコードを変換して
//          basic_string<tCharOut>として出力する。
//
//          tCharIn, tCharOutは、下記を使用可能。
//              char        UTF-8
//              wchar_t     UTF-16/32
//              char16_t    UTF-16
//              char32_t    UTF-32
//############################################################################

// Multi Byte(char)を指定するマーカー・クラス
/*!
    @ingroup Unicode
    @todo   T.B.D.
*/
struct MultiByte { };

// ***************************************************************************
//      UTF-8/UTF-16/32変換用クラス・テンプレート
// ***************************************************************************

namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

template<class tCharOut, class tCharIn>
struct Converter
{
    static std::basic_string<tCharOut> THEOLIZER_INTERNAL_DLL conv(tCharIn const* iIn) noexcept;
    static std::basic_string<tCharOut> conv(std::basic_string<tCharIn> const& iIn) noexcept
    {
        return conv(iIn.c_str());
    }
};

// ***************************************************************************
//      Multi Byte用特殊化
// ***************************************************************************

//      ---<<< Multi Byteを入力する >>>---

template<class tCharOut>
struct Converter<tCharOut, MultiByte>
{
    static std::basic_string<tCharOut> THEOLIZER_INTERNAL_DLL conv(char const* iIn) noexcept;
    static std::basic_string<tCharOut> conv(std::string const& iIn) noexcept
    {
        return conv(iIn.c_str());
    }
};

//      ---<<< Multi Byteを出力する >>>---

template<class tCharIn>
struct Converter<MultiByte, tCharIn>
{
    static std::string THEOLIZER_INTERNAL_DLL conv(tCharIn const* iIn) noexcept;
    static std::string conv(std::basic_string<tCharIn> const& iIn) noexcept
    {
        return conv(iIn.c_str());
    }
};

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

//############################################################################
//      Unicode記録用文字列クラスu8string
/*!
    @section u8string UTF-8文字列クラス

    @todo   T.B.D.
*/
/*
    std::stringを派生し、Unicode間のエンコード変換機能を追加
    また、Multi Byte文字エンコードとの変換も提供する
    テンプレート・パラメータとして、下記を使用可能。
        char        UTF-8
        wchar_t     UTF-16/32
        char16_t    UTF-16
        char32_t    UTF-32

    下記コンバート機能を追加する
        コンストラクタ
        代入演算子
        明示的basic_string\<他char型\>取出し
            std::string     get_string() const;
            std::wstring    get_wstring() const;
            std::u16string  get_u16string() const;
            std::u32string  get_u32string() const;
        basic_string\<他char型\>へのキャスト演算子
            operator std::string() const;
            operator std::wstring() const;
            operator std::u16string() const;
            operator std::u32string() const;
        Multi Byte用コンバータ
            basic_string<char>      getMultiByte() const;
            void setMultiByte(char const*);
        fstream用ファイル名返却
            ?string getXString() const;

スレッド・セーフ性：basic_string<>と同等
    const指定されている関数同士は別スレッドからアクセスしても問題ない。
    しかし、そうでない関数を別スレッドから呼び出してはいけない。

備忘録：
    各中継マクロは、mStringデータ・メンバのみの前提で作っている。

*/
//############################################################################

// ***************************************************************************
//          本体
// ***************************************************************************

class THEOLIZER_INTERNAL_DLL u8string
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN
//----------------------------------------------------------------------------
//      データ実体
//----------------------------------------------------------------------------

    std::string mString;

public:

//----------------------------------------------------------------------------
//      コンストラクタ
//----------------------------------------------------------------------------

//      ---<<< mStringのコンストラクタへ中継 >>>---

    u8string()                                  : mString() { }
    u8string(std::string const& str)            : mString(str) { }
    u8string(std::string const& str, std::size_t pos, std::size_t len = std::string::npos)
                                                : mString(str, pos, len) { }
    u8string(char const* s)                     : mString(s) { }
    u8string(char const* s, std::size_t n)      : mString(s, n) { }
    u8string(std::size_t n, char c)             : mString(n, c) { }
    template <class InputIterator>
    u8string(InputIterator first, InputIterator last)
                                                : mString(first, last) { }
    u8string(std::initializer_list<char> il)    : mString(il) { }
    u8string(std::string&& str) noexcept        : mString(str) { }

//      ---<<< コンバーター・コンストラクタ >>>---

    #define THEOLIZER_INTERNAL_CONSTRUCTOR(dFromChar)                       \
    u8string(const dFromChar* iIn) :                                        \
                mString(internal::Converter<char, dFromChar>::conv(iIn)) { }\
    u8string(const std::basic_string<dFromChar>& iIn) :                     \
                mString(internal::Converter<char, dFromChar>::conv(iIn)) { }

    THEOLIZER_INTERNAL_CONSTRUCTOR(wchar_t);
    THEOLIZER_INTERNAL_CONSTRUCTOR(char16_t);
    THEOLIZER_INTERNAL_CONSTRUCTOR(char32_t);
    #undef THEOLIZER_INTERNAL_CONSTRUCTOR

    // Multi Byteからコンバート生成
    u8string(char const* iIn, MultiByte) :
                mString(internal::Converter<char, MultiByte>::conv(iIn)) { }
    u8string(std::string const& iIn, MultiByte) :
                mString(internal::Converter<char, MultiByte>::conv(iIn)) { }

//----------------------------------------------------------------------------
//      各種関数中継
//          メンバ  関数：パラメータが文字列１つだけのものについて対応。
//          フレンド関数：パラメータが文字列２つだけのものについて対応。
//----------------------------------------------------------------------------

//      ---<<< メンバ関数 >>>---

    #define THEOLIZER_INTERNAL_FUNCTION(dFunc)                              \
        u8string& dFunc(const u8string& iIn) {                              \
            mString.dFunc(iIn.mString);                                     \
            return *this;                                                   \
        }

    THEOLIZER_INTERNAL_FUNCTION(operator=);
    THEOLIZER_INTERNAL_FUNCTION(operator+=);
    THEOLIZER_INTERNAL_FUNCTION(append);
    THEOLIZER_INTERNAL_FUNCTION(assign);
    #undef THEOLIZER_INTERNAL_FUNCTION

//      ---<<< フレンド関数 >>>---

    friend u8string operator+(u8string const& lhs, u8string const& rhs)
    {
        return std::operator+(lhs.mString, rhs.mString);
    }
    friend u8string operator+(u8string const& lhs, char const* rhs)
    {
        return std::operator+(lhs.mString, rhs);
    }
    friend u8string operator+(char const* lhs, u8string const& rhs)
    {
        return std::operator+(lhs, rhs.mString);
    }

    #define THEOLIZER_INTERNAL_FRIEND_RELATIONAL_OPERATOR(dFunc)            \
        friend bool dFunc(const u8string& lhs, const u8string& rhs)         \
        {                                                                   \
            return std::dFunc(lhs.mString, rhs.mString);                    \
        }                                                                   \
        friend bool dFunc(const u8string& lhs, char const* rhs)             \
        {                                                                   \
            return std::dFunc(lhs.mString, rhs);                            \
        }                                                                   \
        friend bool dFunc(char const* lhs, const u8string& rhs)             \
        {                                                                   \
            return std::dFunc(lhs, rhs.mString);                            \
        }

    THEOLIZER_INTERNAL_FRIEND_RELATIONAL_OPERATOR(operator==);
    THEOLIZER_INTERNAL_FRIEND_RELATIONAL_OPERATOR(operator!=);
    THEOLIZER_INTERNAL_FRIEND_RELATIONAL_OPERATOR(operator<);
    THEOLIZER_INTERNAL_FRIEND_RELATIONAL_OPERATOR(operator<=);
    THEOLIZER_INTERNAL_FRIEND_RELATIONAL_OPERATOR(operator>);
    THEOLIZER_INTERNAL_FRIEND_RELATIONAL_OPERATOR(operator>=);
    #undef THEOLIZER_INTERNAL_FRIEND_RELATIONAL_OPERATOR

    friend void swap(u8string& lhs, u8string& rhs)
    {
        std::swap(lhs.mString, rhs.mString);
    }

    friend std::istream& operator>>(std::istream& iIStream, u8string& rhs)
    {
        if (theolizer::isInputByUtf8(&iIStream))
        {
            std::operator>>(iIStream, rhs.mString);
        }
        else
        {
            std::string aString;
            std::operator>>(iIStream, aString);
            rhs.mString=theolizer::internal::Converter<char, theolizer::MultiByte>::conv(aString);
        }
        return iIStream;
    }

    friend std::ostream& operator<<(std::ostream& iOStream, u8string const& rhs)
    {
        if (theolizer::isOutputByUtf8(&iOStream))
        {
            std::operator<<(iOStream, rhs.mString);
        }
        else
        {
            std::operator<<(iOStream,
                theolizer::internal::Converter<theolizer::MultiByte, char>::conv(rhs.mString));
        }
        return iOStream;
    }

//----------------------------------------------------------------------------
//      明示的string取り出し
//----------------------------------------------------------------------------

    std::string&       str()       {return mString;}
    std::string const& str() const {return mString;}

    char const* c_str() const {return mString.c_str();}

//----------------------------------------------------------------------------
//      明示的basic_string<他char型>取出し
//----------------------------------------------------------------------------

    #define THEOLIZER_INTERNAL_GET_STRING(dToChar, dName)                   \
    std::dName get_##dName() const                                          \
    {                                                                       \
        return internal::Converter<dToChar, char>::conv(mString);           \
    }

    THEOLIZER_INTERNAL_GET_STRING(char,       string);
    THEOLIZER_INTERNAL_GET_STRING(wchar_t,    wstring);
    THEOLIZER_INTERNAL_GET_STRING(char16_t,   u16string);
    THEOLIZER_INTERNAL_GET_STRING(char32_t,   u32string);
    #undef THEOLIZER_INTERNAL_GET_STRING

//----------------------------------------------------------------------------
//      basic_string<>へのキャスト演算子
//----------------------------------------------------------------------------

    operator std::string()    const {return get_string();}
    operator std::wstring()   const {return get_wstring();}
    operator std::u16string() const {return get_u16string();}
    operator std::u32string() const {return get_u32string();}

//----------------------------------------------------------------------------
//      各char*型へのキャスト
//
//          注意事項：
//              b_str()から取り出した各ポインタは、その文の終わり(;)まで有効。
//              ;を超えて使うことは想定していない。
//----------------------------------------------------------------------------

    class b_string
    {
    private:
        u8string const& mString;
        std::string     mMBstring;
        std::wstring    mWstring;
        std::u16string  mU16string;
        std::u32string  mU32string;
    public:
        b_string(u8string const& iString) : mString(iString) { }

        operator char const*()     {mMBstring =mString.getMultiByte(); return mMBstring.c_str();}
        operator const wchar_t*()  {mWstring  =mString.get_wstring();  return mWstring.c_str();}
        operator const char16_t*() {mU16string=mString.get_u16string();return mU16string.c_str();}
        operator const char32_t*() {mU32string=mString.get_u32string();return mU32string.c_str();}
    };

    b_string b_str() {return b_string(*this);}

//----------------------------------------------------------------------------
//      Multi Byte変換機能
//
//      注意事項：
//          非Windowsについて、当ライブラリはMulti Byteをサポートしない。
//          当変換機能は、UTF-8文字列を出し入れする。
//----------------------------------------------------------------------------

//      ---<<< Multi Byte文字列 >>>---

    std::string getMultiByte() const
    {
        return internal::Converter<MultiByte, char>::conv(mString);
    }

    u8string& setMultiByte(char const* iIn)
    {
        *this = internal::Converter<char, MultiByte>::conv(iIn);
        return *this;
    }
    u8string& setMultiByte(std::string const& iIn)
    {
        *this = internal::Converter<char, MultiByte>::conv(iIn);
        return *this;
    }

//      ---<<< std::fstream対応 >>>---
//      MSVC    : stkd::wstring(UTF-16)返却
//      MinGW   : std::string(Multi Byte)返却
//      その他  : std::string(UTF-8)返却

#if defined(_WIN32)
    #if defined(_MSC_VER)
        std::wstring get_fstring() const {return get_wstring();}
    #else
        std::string  get_fstring() const {return getMultiByte();}
    #endif
#else
        std::string  get_fstring() const {return get_string();}
#endif

//      ---<<< boost::filesystem::path対応 >>>---
//      Windows : std::wstring(UTF-16)返却
//      その他  : std::string(UTF-8)返却

#if defined(_WIN32)
        std::wstring get_bstring() const {return get_wstring();}
#else
        std::string  get_bstring() const {return get_string();}
#endif

#endif  // THEOLIZER_INTERNAL_DOXYGEN
};

}   // theolizer ------------------- 注意。以降はグローバル名前空間

//############################################################################
//      istream, ostreamグローバル演算子／関数のオーバーロード
//          operator<<, operator>>, getlineについて下記をオーバーロードし、
//          登録されているistream/ostreamについては、Multi Byteに変換する。
//          登録されていないものについては、UTF-8に変換する。
//          なお、cin, cout, cerrはデフォルトで登録される。
//              operator<<のみ  3つとも
//              char*           std::string
//              wchar_t*        std::wstring
//              char16_t*       std::u16string
//              char32_t*       std::u32string
//
//          注意事項：
//              std空間で同様のoperatorが定義されている。
//              グローバル名前空間以外からこれらの演算子を呼び出すと、
//              ADLによりstd::ostream, std::istreamからstd空間の演算子が
//              呼びだされ、自動変換機能が働かない。
//              その場合、::operator<<(std::cout, 変数);のようにして呼び出す。
//              スマートに呼び出せる方法があれば、教えて欲しい。
//############################################################################

// ***************************************************************************
//      char型変数用operator<<, operator>>
//          Windowsでは登録されていたらMulti Byte変換処理する
//          非WindowsではMulti Byte非対応なので、不要。
// ***************************************************************************

#if defined(_WIN32)

//----------------------------------------------------------------------------
//      operator<<(char*)
//----------------------------------------------------------------------------

inline std::ostream& operator<<(std::ostream& iOStream, char const* iString)
{
    if (theolizer::isOutputByUtf8(&iOStream)) {
        std::operator<<(iOStream, iString);
    } else {
        std::operator<<(iOStream,
            theolizer::internal::Converter<theolizer::MultiByte, char>::conv(iString));
    }
    return iOStream;
}

//----------------------------------------------------------------------------
//      operator<<(std::string)
//----------------------------------------------------------------------------

inline std::ostream& operator<<(std::ostream& iOStream, std::string const& iString)
{
    iOStream << iString.c_str();
    return iOStream;
}

//----------------------------------------------------------------------------
//      operator>>(std::string)
//----------------------------------------------------------------------------

inline std::istream& operator>>(std::istream& iIStream, std::string& iString)
{
    if (theolizer::isInputByUtf8(&iIStream)) {
        std::operator>>(iIStream, iString);
    } else {
        std::string aString;
        std::operator>>(iIStream, aString);
        iString=theolizer::internal::Converter<char, theolizer::MultiByte>::conv(aString);
    }
    return iIStream;
}
#endif

// ***************************************************************************
//      wchar_t, char16_t, char32_t型変数用operator<<, operator>>
// ***************************************************************************

#ifndef THEOLIZER_INTERNAL_DOXYGEN

//----------------------------------------------------------------------------
//      operator<<  dChar*とbasic_string<dChar>用
//----------------------------------------------------------------------------

#define THEOLIZER_INTERNAL_LEFT_SHIFT_OPERATOR(dChar)                       \
    inline std::ostream& operator<<(std::ostream& iOStream, const dChar* iString)\
    {                                                                       \
        if (theolizer::isOutputByUtf8(&iOStream)) {                         \
            std::operator<<(iOStream,                                       \
                theolizer::internal::Converter<char, dChar>::conv(iString));\
        } else {                                                            \
            std::operator<<(iOStream,                                       \
                theolizer::internal::Converter<theolizer::MultiByte, dChar>::conv(iString));\
        }                                                                   \
        return iOStream;                                                    \
    }                                                                       \
    inline std::ostream& operator<<(std::ostream& iOStream,                 \
                                    const std::basic_string<dChar>& iString)\
    {                                                                       \
        iOStream << iString.c_str();                                        \
        return iOStream;                                                    \
    }

THEOLIZER_INTERNAL_LEFT_SHIFT_OPERATOR(wchar_t);
THEOLIZER_INTERNAL_LEFT_SHIFT_OPERATOR(char16_t);
THEOLIZER_INTERNAL_LEFT_SHIFT_OPERATOR(char32_t);
#undef THEOLIZER_INTERNAL_LEFT_SHIFT_OPERATOR

//----------------------------------------------------------------------------
//      operator>>  basic_string<dChar>用
//----------------------------------------------------------------------------

#define THEOLIZER_INTERNAL_RIGHT_SHIFT_OPERATOR(dChar)                      \
    inline std::istream& operator>>(std::istream& iIStream,std::basic_string<dChar>& iString)\
    {                                                                       \
        std::string aString;                                                \
        std::operator>>(iIStream, aString);                                 \
        if (theolizer::isInputByUtf8(&iIStream)) {                          \
            iString=theolizer::internal::Converter<dChar, char>::conv(aString);\
        } else {                                                            \
            iString=theolizer::internal::Converter<dChar,                   \
                                    theolizer::MultiByte>::conv(aString);   \
        }                                                                   \
        return iIStream;                                                    \
    }

THEOLIZER_INTERNAL_RIGHT_SHIFT_OPERATOR(wchar_t);
THEOLIZER_INTERNAL_RIGHT_SHIFT_OPERATOR(char16_t);
THEOLIZER_INTERNAL_RIGHT_SHIFT_OPERATOR(char32_t);
#undef THEOLIZER_INTERNAL_RIGHT_SHIFT_OPERATOR

#endif  // THEOLIZER_INTERNAL_DOXYGEN

//############################################################################
//      End
//############################################################################

// ***************************************************************************
//          DLL用の警告禁止解除
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#endif  // THEOLIZER_INTERNAL_U8STRING_H
