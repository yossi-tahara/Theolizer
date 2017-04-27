//############################################################################
/*!
    @brief      Jsonシリアライザ
    @ingroup    TheolizerLib
    @file       serializer_json.h
    @author     Yoshinori Tahara
    @date       2015/09/18 Created
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

#if !defined(THEOLIZER_INTERNAL_SERIALIZER_JSON_H)
#define THEOLIZER_INTERNAL_SERIALIZER_JSON_H

#include "serializer.h"

//############################################################################
//      Begin
//############################################################################

// ***************************************************************************
//          DLL用の警告禁止
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable:4251 4702)
#endif

namespace theolizer
{

//############################################################################
//      共通定義
//############################################################################

// ***************************************************************************
//      Jsonシリアライザのバージョン番号とシリアライザ名
//          注意事項：
//              このバージョンはサイズに「幅」がない。(厳密に一致することが必要)
//              余裕を持たせる場合、下記工夫が必要
//                  シリアライズ時は実型を表現できるシリアライズ型へ
//                  デシリアライズ時はシリアライズ型を表現できる実型へ
//              そこそこ手間が掛かりそうななので、必要が生じたら対応を検討する
// ***************************************************************************

namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

const static char kJsonSerializerName[]="JsonTheolizer";
const static unsigned kJsonSerializerVersionNo=1;

// ***************************************************************************
//      プロバティ返却
//          必要な場合直接見えた方が良いのでここで定義
// ***************************************************************************

inline bool hasPropertyJson(Property iProperty)
{
    bool ret=false;
    switch(iProperty)
    {
    case Property::EncodedString:
        ret=true;
        break;

    case Property::SupportModifying:
        ret=true;
        break;

    case Property::LongDoubleIsDouble:
        ret=true;
        break;

    default:
        break;
    }

    return ret;
}

// ***************************************************************************
//      プリミティブ(組み込み型)名生成
// ***************************************************************************

class JsonMidOSerializer;
class JsonMidISerializer;

#define THEOLIZER_INTERNAL_INTEGRAL(dSigned, dDigits, dName1)               \
    template<class tMidSerializer, typename tPrimitive>                     \
    struct PrimitiveName                                                    \
    <                                                                       \
        tMidSerializer,                                                     \
        tPrimitive,                                                         \
        EnableIf                                                            \
        <                                                                   \
               (std::is_same<tMidSerializer, JsonMidOSerializer>::value     \
             || std::is_same<tMidSerializer, JsonMidISerializer>::value)    \
            && (std::numeric_limits<tPrimitive>::is_signed == dSigned)      \
            && (std::numeric_limits<tPrimitive>::radix == 2)                \
            && (std::numeric_limits<tPrimitive>::digits == dDigits)         \
            && (std::numeric_limits<tPrimitive>::max_exponent == 0)         \
        >                                                                   \
    >                                                                       \
    {                                                                       \
        static char const* getPrimitiveName(unsigned iSerializerVersionNo)  \
        {                                                                   \
            switch(iSerializerVersionNo)                                    \
            {                                                               \
            case 1: return dName1;                                          \
            }                                                               \
            THEOLIZER_INTERNAL_ABORT("getPrimitiveName() : iVersionNo is illegal.");\
            return "";                                                      \
        }                                                                   \
    }

#define THEOLIZER_INTERNAL_FLOAT(dDigits, dMaxExponent, dName1)             \
    template<class tMidSerializer, typename tPrimitive>                     \
    struct PrimitiveName                                                    \
    <                                                                       \
        tMidSerializer,                                                     \
        tPrimitive,                                                         \
        EnableIf                                                            \
        <                                                                   \
               (std::is_same<tMidSerializer, JsonMidOSerializer>::value     \
             || std::is_same<tMidSerializer, JsonMidISerializer>::value)    \
            && (std::numeric_limits<tPrimitive>::is_signed == 1)            \
            && (std::numeric_limits<tPrimitive>::radix == 2)                \
            && (std::numeric_limits<tPrimitive>::digits == dDigits)         \
            && (std::numeric_limits<tPrimitive>::max_exponent == dMaxExponent)\
        >                                                                   \
    >                                                                       \
    {                                                                       \
        static char const* getPrimitiveName(unsigned iSerializerVersionNo)  \
        {                                                                   \
            switch(iSerializerVersionNo)                                    \
            {                                                               \
            case 1: return dName1;                                          \
            }                                                               \
            THEOLIZER_INTERNAL_ABORT("getPrimitiveName() : iVersionNo is illegal.");\
            return "";                                                      \
        }                                                                   \
    }

#define THEOLIZER_INTERNAL_STRING(dBytes, dName1)                           \
    template<class tMidSerializer, typename tPrimitive>                     \
    struct PrimitiveName                                                    \
    <                                                                       \
        tMidSerializer,                                                     \
        tPrimitive,                                                         \
        EnableIf                                                            \
        <                                                                   \
               (std::is_same<tMidSerializer, JsonMidOSerializer>::value     \
             || std::is_same<tMidSerializer, JsonMidISerializer>::value)    \
            && (IsString<tPrimitive>::value)                                \
            && (sizeof(typename tPrimitive::value_type) == dBytes)          \
        >                                                                   \
    >                                                                       \
    {                                                                       \
        static char const* getPrimitiveName(unsigned iSerializerVersionNo)  \
        {                                                                   \
            switch(iSerializerVersionNo)                                    \
            {                                                               \
            case 1: return dName1;                                          \
            }                                                               \
            THEOLIZER_INTERNAL_ABORT("getPrimitiveName() : iVersionNo is illegal.");\
            return "";                                                      \
        }                                                                   \
    }

THEOLIZER_INTERNAL_INTEGRAL(0,  1,  "bool");

THEOLIZER_INTERNAL_INTEGRAL(1,  7,  "int8");
THEOLIZER_INTERNAL_INTEGRAL(1, 15,  "int16");
THEOLIZER_INTERNAL_INTEGRAL(1, 31,  "int32");
THEOLIZER_INTERNAL_INTEGRAL(1, 63,  "int64");

THEOLIZER_INTERNAL_INTEGRAL(0,  8,  "unit8");
THEOLIZER_INTERNAL_INTEGRAL(0, 16,  "uint16");
THEOLIZER_INTERNAL_INTEGRAL(0, 32,  "uint32");
THEOLIZER_INTERNAL_INTEGRAL(0, 64,  "uint64");

THEOLIZER_INTERNAL_FLOAT(24,   128, "float32");
THEOLIZER_INTERNAL_FLOAT(53,  1024, "float64");
THEOLIZER_INTERNAL_FLOAT(64, 16384, "float80");

THEOLIZER_INTERNAL_STRING(1,        "String");
THEOLIZER_INTERNAL_STRING(2,        "String");
THEOLIZER_INTERNAL_STRING(4,        "String");

#undef  THEOLIZER_INTERNAL_INTEGRAL
#undef  THEOLIZER_INTERNAL_FLOAT
#undef  THEOLIZER_INTERNAL_STRING

//----------------------------------------------------------------------------
//      プリミティブ名からC++名へ変換
//----------------------------------------------------------------------------

THEOLIZER_INTERNAL_DLL
char const* getCppNameJson(std::string const& iPrimitiveName, unsigned iSerializerVersionNo);

//############################################################################
//      Json Serializer実装部
//
//          各文字列型が下記文字エンコードであるとして処理する
//              std::string      UTF-8
//              std::wstring     UTF-16/32
//              std::u16string   UTF-16
//              std::u32string   UTF-32
//          ただし、mCharIsMultiByteがtrueの時、
//              std::stringがMultiByteエンコードされているとして処理する。
//############################################################################

#endif  // THEOLIZER_INTERNAL_DOXYGEN
// ***************************************************************************
/*!
@brief      保存用中間JsonSerializer
@details    主な処理はここで行うが、インスタンスを作れないよう抽象クラスとする。
*/
// ***************************************************************************

class THEOLIZER_INTERNAL_DLL JsonMidOSerializer : public BaseSerializer
{
private:
    friend class BaseSerializer;
    THEOLIZER_INTERNAL_FRIENDS_FOR_INTERNAL;

//----------------------------------------------------------------------------
//      基本
//----------------------------------------------------------------------------

    std::ostream&           mOStream;
    bool                    mNoPrettyPrint;
    bool                    mWriteComma;
    bool                    mCharIsMultiByte;

    static const unsigned   kLastVersionNo=kJsonSerializerVersionNo;

    typedef JsonMidOSerializer  MidSerializer;
    typedef JsonMidOSerializer  MetaOSerializer;    // メタ・シリアライザ
    typedef JsonMidISerializer  MetaISerializer;    // メタ・デシリアライザ
    static char const* const    kSerializerName;

public:
    //! 保存用なのでtrue
    static const bool       kIsSaver=true;

    //! @todo T.B.D.
    static bool hasPropertyStatic(Property iProperty)
    {
        return hasPropertyJson(iProperty);
    }

    //! @todo T.B.D.
    bool hasProperty(Property iProperty)
    {
        return hasPropertyStatic(iProperty);
    }

    //! std::stringをマルチ・パイト文字コードとして処理することを指定する
    void setCharIsMultiByte(bool iCharIsMultiByte)
    {
        mCharIsMultiByte=iCharIsMultiByte;
    }

protected:
    JsonMidOSerializer
    (
        std::ostream& iOStream,
        Destinations const& iDestinations,
        GlobalVersionNoTableBase const*const iGlobalVersionNoTable,
        unsigned iGlobalVersionNo,
        unsigned iLastGlobalVersionNo,
        CheckMode iCheckMode,
        bool iNoPrettyPrint,
        bool iNoThrowException
    );

//----------------------------------------------------------------------------
//      型情報保存
//----------------------------------------------------------------------------

private:

//      ---<<< ヘッダ保存 >>>---

    void writeHeader();

//----------------------------------------------------------------------------
//      データ保存
//----------------------------------------------------------------------------

//      ---<<< 制御情報保存 >>>---

    void saveControl(int iControl)                  {savePrimitive(iControl);}
    void saveControl(long iControl)                 {savePrimitive(iControl);}
    void saveControl(long long iControl)            {savePrimitive(iControl);}
    void saveControl(unsigned iControl)             {savePrimitive(iControl);}
    void saveControl(unsigned long iControl)        {savePrimitive(iControl);}
    void saveControl(unsigned long long iControl)   {savePrimitive(iControl);}
    void saveControl(std::string const& iControl)   {encodeJsonString(iControl);}

//      ---<<< プリミティブ保存 >>>---

    #define THEOLIZER_INTERNAL_DEF_SAVE
    #include "internal/primitive.inc"

//      ---<<< Element前処理 >>>---

    void writePreElement(bool iDoProcess=false);

//----------------------------------------------------------------------------
//      内部処理
//----------------------------------------------------------------------------

private:

//      ---<<< クラス(配列／侵入型／非侵入型)処理 >>>---

    void saveClassStart(bool iIsTop=false);
    void saveClassEnd(bool iIsTop=false);

//      ---<<< プリミティブ名返却 >>>---

    template<typename tType>
    static char const* getPrimitiveName(unsigned iSerializerVersionNo)
    {
        static_assert(Ignore<tType>::kFalse, "Unknown primitive name.");
        return "";
    }

//      ---<<< プリミティブ名からC++型名返却 >>>---

    static char const* getCppName(std::string const& iPrimitiveName, unsigned iSerializerVersionNo)
    {
        return getCppNameJson(iPrimitiveName, iSerializerVersionNo);
    }

//      ---<<< Element名保存 >>>---
//          名前対応時のみ保存する

    void saveElementName(ElementsMapping iElementsMapping,
                         u8string const& iElementName)
    {
        if (iElementsMapping == emName) {
            encodeJsonString(iElementName.str());
            mOStream << ":";
        }
    }

//      ---<<< 整形処理 >>>---

    void writeCommaIndent(bool iWriteComma=false);

//      ---<<< JSON文字列へエンコードして保存 >>>---

    void encodeJsonString(std::string const& iString);
};
#ifndef THEOLIZER_INTERNAL_DOXYGEN

//----------------------------------------------------------------------------
//      プリミティブ名返却
//----------------------------------------------------------------------------

#define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSymbol)                    \
    template<>                                                              \
    inline char const* JsonMidOSerializer::                                 \
        getPrimitiveName<dType>(unsigned iSerializerVersionNo)              \
    {                                                                       \
        return PrimitiveName<JsonMidOSerializer, dType>::getPrimitiveName(iSerializerVersionNo);\
    }
#include "internal/primitive.inc"

#endif  // THEOLIZER_INTERNAL_DOXYGEN
// ***************************************************************************
/*!
@brief      回復用中間JsonSerializer
@details    主な処理はここで行うが、インスタンスを作れないよう抽象クラスとする。
*/
// ***************************************************************************

class THEOLIZER_INTERNAL_DLL JsonMidISerializer : public BaseSerializer
{
private:
    friend class BaseSerializer;
    THEOLIZER_INTERNAL_FRIENDS_FOR_INTERNAL;

//----------------------------------------------------------------------------
//      基本
//----------------------------------------------------------------------------

    std::istream&           mIStream;
    bool                    mReadComma;
    bool                    mCharIsMultiByte;
    bool                    mTerminated;

    static const unsigned   kLastVersionNo=kJsonSerializerVersionNo;

    typedef JsonMidISerializer  MidSerializer;
    typedef JsonMidOSerializer  MetaOSerializer;    // メタ・シリアライザ
    typedef JsonMidISerializer  MetaISerializer;    // メタ・デシリアライザ
    static char const* const    kSerializerName;

public:
    //! 回復用なのでfalse
    static const bool       kIsSaver=false;

    //! @todo T.B.D.
    static bool hasPropertyStatic(Property iProperty)
    {
        return hasPropertyJson(iProperty);
    }

    //! @todo T.B.D.
    bool hasProperty(Property iProperty)
    {
        return hasPropertyStatic(iProperty);
    }

    //! std::stringをマルチ・パイト文字コードとして処理することを指定する
    void setCharIsMultiByte(bool iCharIsMultiByte)
    {
        mCharIsMultiByte=iCharIsMultiByte;
    }

protected:
    JsonMidISerializer
    (
        std::istream& iIStream,
        Destinations const& iDestinations,
        GlobalVersionNoTableBase const*const iGlobalVersionNoTable,
        unsigned iLastGlobalVersionNo,
        std::ostream* iOStream,
        bool iNoThrowException
    );

//----------------------------------------------------------------------------
//      ヘッダ回復と型チェック
//----------------------------------------------------------------------------

private:

//      ---<<< ヘッダ情報回復 >>>---

    void readHeader();

//      ---<<<  TypeIndex一致判定 >>>---

    bool isMatchTypeIndex(size_t iSerializedTypeIndex,
                          size_t iProgramTypeIndex);

//----------------------------------------------------------------------------
//      データ回復
//----------------------------------------------------------------------------

private:

//      ---<<< 制御情報回復 >>>---

    void loadControl(int& iControl)                 {loadPrimitive(iControl);}
    void loadControl(long& iControl)                {loadPrimitive(iControl);}
    void loadControl(long long& iControl)           {loadPrimitive(iControl);}
    void loadControl(unsigned& iControl)            {loadPrimitive(iControl);}
    void loadControl(unsigned long& iControl)       {loadPrimitive(iControl);}
    void loadControl(unsigned long long& iControl)  {loadPrimitive(iControl);}
    void loadControl(std::string& iControl)         {decodeJsonString(iControl);}

//      ---<<< プリミティブ回復 >>>---

    #define THEOLIZER_INTERNAL_DEF_LOAD
    #include "internal/primitive.inc"

//      ---<<< Element前処理 >>>---
//          戻り値：
//              次がないなら、Terminated(mTerminated=true)
//              次があるなら、Continue  (mTerminated=false)
//              それ以外なら、例外

    ReadStat readPreElement(bool iDoProcess=false);

//      ---<<< 要素破棄処理 >>>---

    void disposeElement();

//      ---<<< ClassType終了状態返却 >>>---
//          処理中のClassTypeの読み出しが終了(mTerminated)していたら、
//          trueを返却する。その後、falseへ戻る。

public:
    //! @todo T.B.D.
    bool isTerminated() const {return mTerminated;}

//----------------------------------------------------------------------------
//      内部処理
//----------------------------------------------------------------------------

private:

//      ---<<< クラス(配列／侵入型／非侵入型)処理 >>>---
//          loadClassEnd()呼び出し以前に、readPreElement()呼び出しにより、
//          mTerminatedをtrueにしておくこと。

    void loadClassStart(bool iIsTop=false);
    void loadClassEnd(bool iIsTop=false);

//      ---<<< プリミティブ名返却 >>>---

    template<typename tType>
    static char const* getPrimitiveName(unsigned iSerializerVersionNo)
    {
        static_assert(Ignore<tType>::kFalse, "Unknown primitive name.");
        return "";
    }

//      ---<<< プリミティブ名からC++型名返却 >>>---

    static char const* getCppName(std::string const& iPrimitiveName, unsigned iSerializerVersionNo)
    {
        return getCppNameJson(iPrimitiveName, iSerializerVersionNo);
    }

//      ---<<< Element名回復 >>>---
//          ヘッダが有る時はElementsMappingの値に関係なく名前対応可能である。
//              そのような派生Serializerに対応するためのI/Fである。
//              なお、JsonMid?Serializerはこの機能に対応していない。
//              ヘッダはデバッグ用オプションなので、ヘッダの有無で動作を変えないため。
//          戻り値：
//              名前対応するなら、名前返却
//              順序対応するなら、empty返却
//          注意事項：
//              同じTheolizerVersion<>に対しては名前対応と順序対応を変えてはいけない

    std::string loadElementName(ElementsMapping iElementsMapping);

//      ---<<< ,まで読み飛ばし >>>---
//          戻り値：
//              ,なら、true
//              終端なら、false
//              それ以外なら、例外

    bool readComma(bool iReadComma=false);

//      ---<<< 終了マーク確認 >>>---
//          終了マークなら、true

    bool checkTerminal(char iIn);

//      ---<<< JSON文字列をデコードして文字列回復 >>>---

    void decodeJsonString(std::string& iString);

//      ---<<< 1文字読み出し >>>---

    char getChar();

//      ---<<< 指定以外の文字まで読み飛ばし >>>---
//          指定文字列に含まれない文字が出て来るまで、ストリームから読み捨てる。
//          戻り値は最後に読み出した文字

    char find_not_of(std::string const& iSkipChars);
};
#ifndef THEOLIZER_INTERNAL_DOXYGEN

//----------------------------------------------------------------------------
//      プリミティブ名返却
//----------------------------------------------------------------------------

#define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSymbol)                    \
    template<>                                                              \
    inline char const* JsonMidISerializer::                                 \
        getPrimitiveName<dType>(unsigned iSerializerVersionNo)              \
    {                                                                       \
        return PrimitiveName<JsonMidISerializer, dType>::getPrimitiveName(iSerializerVersionNo);\
    }
#include "internal/primitive.inc"

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

//############################################################################
//      Json Serializer I/Fクラス
//          保存先をテンプレートで指定する。
//
//      注意事項：
//          JsonBaseI/OSerializerから派生してテンプレート化している。
//          JsonBaseI/OSerializerを直接保存先のテンプレートとした場合、
//          使用した保存先全てに対して、コードが複製される。
//          それを防ぐことが目的である。
//############################################################################

// ***************************************************************************
//!     保存用JsonSerializer
// ***************************************************************************

//      ---<<< 通常用 >>>---

template<Destination uDefault=theolizerD::All, Destination... uDestinations>
class JsonOSerializer : public internal::JsonMidOSerializer
{
    THEOLIZER_INTERNAL_FRIENDS;
    void AbstructSerializer() { }       // インスタンス生成許可

    static const internal::Destinations     kDestinations;  // クラス外で定義

    // グローバル・バージョン番号テーブルへのポインタへのポインタ
    //  グローバル・バージョン番号テーブルへのポインタの値は初期化時、
    //  確定していない可能性があるが、ポインタ自身のアドレスは確定している。
    //  なお、sGlobalVersionNoTableは外部リンケージを持たないものであり、
    //  Theolizerのクライアントをコンパイルする時に使われるsGlobalVersionNoTableを
    //  取込む必要があるため、テンプレートであるトップ・シリアライザで処理する。
    constexpr static internal::GlobalVersionNoTableBase const*const*const
        kPtrGlobalVersionNoTable=&internal::sGlobalVersionNoTable;

    typedef JsonOSerializer                 DestOSerializer;
    typedef internal::JsonMidOSerializer    MidSerializer;

public:
#ifndef THEOLIZER_INTERNAL_DOXYGEN
    static const bool                       kHasDestination=true;
#endif  // THEOLIZER_INTERNAL_DOXYGEN

    //! 通常のコンストラクタ
    JsonOSerializer
    (
        std::ostream& iOStream,
        unsigned iGlobalVersionNo=kLastGlobalVersionNo,
        CheckMode iCheckMode=CheckMode::NoTypeCheck,
        bool iNoPrettyPrint=false,
        bool iNoThrowException=false
    ) : JsonMidOSerializer
        (
            iOStream,
            kDestinations,
            internal::sGlobalVersionNoTable,
            iGlobalVersionNo,
            kLastGlobalVersionNo,
            iCheckMode,
            iNoPrettyPrint,
            iNoThrowException
        )
    { }

    //! GlobalVersionNo以外のオプションを指定するコンストラクタ
    JsonOSerializer
    (
        std::ostream& iOStream,
        CheckMode iCheckMode,
        bool iNoPrettyPrint=false,
        bool iNoThrowException=false
    ) : JsonMidOSerializer
        (
            iOStream,
            kDestinations,
            internal::sGlobalVersionNoTable,
            kLastGlobalVersionNo,
            kLastGlobalVersionNo,
            iCheckMode,
            iNoPrettyPrint,
            iNoThrowException
        )
    { }
};

#ifndef THEOLIZER_INTERNAL_DOXYGEN
template<Destination uDefault, Destination... uDestinations>
internal::Destinations const JsonOSerializer<uDefault, uDestinations...>::
    kDestinations=internal::Destinations{uDefault, uDestinations...};

// MidSerializer登録(トライバ処理用)
THEOLIZER_INTERNAL_REGISTER_MID_SERIALIZER(JsonOSerializer);
#endif  // THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//!     回復用JsonSerializer
// ***************************************************************************

//      ---<<< 通常用 >>>---

template<Destination uDefault=theolizerD::All, Destination... uDestinations>
class JsonISerializer : public internal::JsonMidISerializer
{
    THEOLIZER_INTERNAL_FRIENDS;
    void AbstructSerializer() { }       // インスタンス生成許可

    static const internal::Destinations     kDestinations;  // クラス外で定義

    // グローバル・バージョン番号テーブルへのポインタへのポインタ
    constexpr static internal::GlobalVersionNoTableBase const*const*const
        kPtrGlobalVersionNoTable=&internal::sGlobalVersionNoTable;

    typedef JsonISerializer                 DestISerializer;
    typedef internal::JsonMidISerializer    MidSerializer;

public:
#ifndef THEOLIZER_INTERNAL_DOXYGEN
    static const bool                       kHasDestination=true;
#endif  // THEOLIZER_INTERNAL_DOXYGEN

    //! コンストラクタ
    JsonISerializer
    (
        std::istream& iIStream,
        bool iNoThrowException=false
    ) : JsonMidISerializer
        (
            iIStream,
            kDestinations,
            internal::sGlobalVersionNoTable,
            kLastGlobalVersionNo,
            nullptr,
            iNoThrowException
        )
    { }

#ifndef THEOLIZER_INTERNAL_DOXYGEN
    // メタ・デシリアライズ用
    /*
        メタ・シリアライズ処理検討後決定
        現時点でメタ・シリアライズは実験的な実装なので、実用性がない。
        それを製品版に含めるのはどうかと思う。
        できるだけ、別ファイルへ分離することを検討する。
    */
    JsonISerializer
    (
        std::istream& iIStream,
        std::ostream& iOStream,
        bool iNoThrowException=false
    ) : JsonMidISerializer
        (
            iIStream,
            kDestinations,
            internal::sGlobalVersionNoTable,
            kLastGlobalVersionNo,
            &iOStream,
            iNoThrowException
        )
    { }
#endif // THEOLIZER_INTERNAL_DOXYGEN
};

#ifndef THEOLIZER_INTERNAL_DOXYGEN
template<Destination uDefault, Destination... uDestinations>
internal::Destinations const JsonISerializer<uDefault, uDestinations...>::
    kDestinations=internal::Destinations{uDefault, uDestinations...};

// MidSerializer登録(トライバ処理用)
THEOLIZER_INTERNAL_REGISTER_MID_SERIALIZER(JsonISerializer);
#endif  // THEOLIZER_INTERNAL_DOXYGEN

//############################################################################
//      End
//############################################################################

}   // namespace theolizer

// ***************************************************************************
//          DLL用の警告禁止解除
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#endif  // THEOLIZER_INTERNAL_SERIALIZER_JSON_H
