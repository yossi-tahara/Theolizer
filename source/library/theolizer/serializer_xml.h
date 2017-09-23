//############################################################################
/*!
    @brief      XMLシリアライザ
    @ingroup    TheolizerLib
    @file       serializer_xml.h
    @author     Yoshinori Tahara
    @date       2017/09/08 Created
*/
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

#if !defined(THEOLIZER_INTERNAL_SERIALIZER_XML_H)
#define THEOLIZER_INTERNAL_SERIALIZER_XML_H

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

namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//      XMLシリアライザのバージョン番号とシリアライザ名
//          注意事項：
//              このバージョンはサイズに「幅」がない。(厳密に一致することが必要)
//              余裕を持たせる場合、下記工夫が必要
//                  シリアライズ時は実型を表現できるシリアライズ型へ
//                  デシリアライズ時はシリアライズ型を表現できる実型へ
//              そこそこ手間が掛かりそうななので、必要が生じたら対応を検討する
// ***************************************************************************

#define THEOLIZER_INTERNAL_XML_THEOLIZER_NAME   "XmlTheolizer"

const static char kXmlSerializerName[]=THEOLIZER_INTERNAL_XML_THEOLIZER_NAME;
const static unsigned kXmlSerializerVersionNo=1;

// ***************************************************************************
//      XMLヘッダ関係
// ***************************************************************************

#define THEOLIZER_INTERNAL_XML_HEADER       \
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n"
#define THEOLIZER_INTERNAL_XML_NAMESPACE    "th"
#define THEOLIZER_INTERNAL_XML_URI          "https://theolizer.com/theoride/xml-1"

// ***************************************************************************
//      XMLタグ／属性
// ***************************************************************************

enum class TagKind
{
    Start,      // <タグ名 アトリビュート・リスト>
    StartEnd,   // <タグ名 アトリビュート・リスト/>
    End         // </タグ名>
};
THEOLIZER_INTERNAL_DLL std::ostream& operator<<(std::ostream& iOStream, TagKind iTagKind);

// ***************************************************************************
//      XML属性
// ***************************************************************************

struct Attribute
{
    Structure       mStructure;
    std::size_t     mObjectId;
    std::string     mXmlns;             // xmlns:th用
    unsigned        mGlobalVersionNo;   // GlobalVersionNo

    Attribute() :
        mStructure(Structure::None),
        mObjectId(kInvalidSize),
        mXmlns(),
        mGlobalVersionNo(0)
    { }
    char const* getStructure() const;
};

// ***************************************************************************
//      プロバティ返却
//          必要な場合直接見えた方が良いのでここで定義
// ***************************************************************************

inline bool hasPropertyXml(Property iProperty, bool iIsSaver)
{
    bool ret=false;
    switch(iProperty)
    {
    case Property::IsSaver:
        ret=iIsSaver;
        break;

    case Property::EncodedString:
        ret=true;
        break;

    case Property::SupportModifying:
        ret=true;
        break;

    case Property::BinaryOpen:
        ret=false;
        break;
    }

    return ret;
}

// ***************************************************************************
//      プリミティブ(組み込み型)名生成
// ***************************************************************************

class XmlMidOSerializer;
class XmlMidISerializer;

#define THEOLIZER_INTERNAL_INTEGRAL(dSigned, dDigits, dName1)               \
    template<class tMidSerializer, typename tPrimitive>                     \
    struct PrimitiveName                                                    \
    <                                                                       \
        tMidSerializer,                                                     \
        tPrimitive,                                                         \
        EnableIf                                                            \
        <                                                                   \
               (std::is_same<tMidSerializer, XmlMidOSerializer>::value      \
             || std::is_same<tMidSerializer, XmlMidISerializer>::value)     \
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
               (std::is_same<tMidSerializer, XmlMidOSerializer>::value      \
             || std::is_same<tMidSerializer, XmlMidISerializer>::value)     \
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
               (std::is_same<tMidSerializer, XmlMidOSerializer>::value      \
             || std::is_same<tMidSerializer, XmlMidISerializer>::value)     \
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

THEOLIZER_INTERNAL_INTEGRAL(0,  1,  THEOLIZER_INTERNAL_XML_NAMESPACE ":bool");

THEOLIZER_INTERNAL_INTEGRAL(1,  7,  THEOLIZER_INTERNAL_XML_NAMESPACE ":int8");
THEOLIZER_INTERNAL_INTEGRAL(1, 15,  THEOLIZER_INTERNAL_XML_NAMESPACE ":int16");
THEOLIZER_INTERNAL_INTEGRAL(1, 31,  THEOLIZER_INTERNAL_XML_NAMESPACE ":int32");
THEOLIZER_INTERNAL_INTEGRAL(1, 63,  THEOLIZER_INTERNAL_XML_NAMESPACE ":int64");

THEOLIZER_INTERNAL_INTEGRAL(0,  8,  THEOLIZER_INTERNAL_XML_NAMESPACE ":unit8");
THEOLIZER_INTERNAL_INTEGRAL(0, 16,  THEOLIZER_INTERNAL_XML_NAMESPACE ":uint16");
THEOLIZER_INTERNAL_INTEGRAL(0, 32,  THEOLIZER_INTERNAL_XML_NAMESPACE ":uint32");
THEOLIZER_INTERNAL_INTEGRAL(0, 64,  THEOLIZER_INTERNAL_XML_NAMESPACE ":uint64");

THEOLIZER_INTERNAL_FLOAT(24,   128, THEOLIZER_INTERNAL_XML_NAMESPACE ":float32");
THEOLIZER_INTERNAL_FLOAT(53,  1024, THEOLIZER_INTERNAL_XML_NAMESPACE ":float64");
THEOLIZER_INTERNAL_FLOAT(64, 16384, THEOLIZER_INTERNAL_XML_NAMESPACE ":float80");

THEOLIZER_INTERNAL_STRING(1,        THEOLIZER_INTERNAL_XML_NAMESPACE ":string");
THEOLIZER_INTERNAL_STRING(2,        THEOLIZER_INTERNAL_XML_NAMESPACE ":string");
THEOLIZER_INTERNAL_STRING(4,        THEOLIZER_INTERNAL_XML_NAMESPACE ":string");

#undef  THEOLIZER_INTERNAL_INTEGRAL
#undef  THEOLIZER_INTERNAL_FLOAT
#undef  THEOLIZER_INTERNAL_STRING

//----------------------------------------------------------------------------
//      プリミティブ名からC++名へ変換
//----------------------------------------------------------------------------

#ifdef THEOLIZER_INTERNAL_ENABLE_META_SERIALIZER
THEOLIZER_INTERNAL_DLL
char const* getCppNameXml(std::string const& iPrimitiveName, unsigned iSerializerVersionNo);
#endif  // THEOLIZER_INTERNAL_ENABLE_META_SERIALIZER

//############################################################################
//      Xml Serializer実装部
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
@brief      保存用中間XmlSerializer
@details    主な処理はここで行うが、インスタンスを作れないよう抽象クラスとする。
*/
// ***************************************************************************

class THEOLIZER_INTERNAL_DLL XmlMidOSerializer : public BaseSerializer
{
private:
    friend class BaseSerializer;
    THEOLIZER_INTERNAL_FRIENDS_FOR_INTERNAL;

//----------------------------------------------------------------------------
//      基本
//----------------------------------------------------------------------------

    std::ostream&           mOStream;
    bool                    mNoPrettyPrint;

    static const unsigned   kLastVersionNo=kXmlSerializerVersionNo;

    typedef XmlMidOSerializer  MidSerializer;
    typedef XmlMidOSerializer  MetaOSerializer;    // メタ・シリアライザ
    typedef XmlMidISerializer  MetaISerializer;    // メタ・デシリアライザ

protected:
    bool                            mCharIsMultiByte;
    static char const* const        kSerializerName;
    static std::ios_base::openmode  kOpenMode;

    XmlMidOSerializer
    (
        std::ostream& iOStream,
        Destinations const& iDestinations,
        GlobalVersionNoTableBase const*const iGlobalVersionNoTable,
        unsigned iGlobalVersionNo,
        unsigned iLastGlobalVersionNo,
        bool iNoPrettyPrint,
        bool iNoThrowException
    );
    ~XmlMidOSerializer();

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

    void saveObjectId(std::size_t iObjectId, std::size_t iTypeIndex);

    void saveControl(int)                  {}
    void saveControl(long)                 {}
    void saveControl(long long)            {}
    void saveControl(unsigned)             {}
    void saveControl(unsigned long)        {}
    void saveControl(unsigned long long)   {}
    void saveControl(std::string const&)   {}

//      ---<<< 要素名処理 >>>---

    class AutoReleaseTagName
    {
        XmlMidOSerializer&  mXmlMidOSerializer;
        std::string         mTagName;
    public:
        AutoReleaseTagName(XmlMidOSerializer& iXmlMidOSerializer, std::size_t iTypeIndex);
        ~AutoReleaseTagName();
    };

//      ---<<< プリミティブ保存 >>>---

    #define THEOLIZER_INTERNAL_DEF_SAVE
    #include "internal/primitive.inc"

//      ---<<< Element前処理 >>>---

    void writePreElement(bool) { };

//----------------------------------------------------------------------------
//      内部処理
//----------------------------------------------------------------------------

private:

//      ---<<< タグ保存 >>>---

    char const* saveTag
    (
        TagKind             iTagKind,
        std::string const&  iName,
        Attribute const*    iAttribute=nullptr
    );

//      ---<<< グループ処理 >>>---

    void saveGroupStart(bool) { }
    void saveGroupEnd(bool)   { }

//      ---<<< 各種構造処理 >>>---

    void saveStructureStart(Structure iStructure, std::string& ioTypeName, std::size_t iOjbectId);
    void saveStructureEnd(Structure iStructure, std::string const& iTypeName);

//      ---<<< プリミティブ名返却 >>>---

    template<typename tType>
    static char const* getPrimitiveName(unsigned iSerializerVersionNo)
    {
        static_assert(Ignore<tType>::kFalse, "Unknown primitive name.");
        return "";
    }

//      ---<<< Element名保存 >>>---
//          名前対応時のみ保存する

    char const* mElementName;
    void saveElementName(ElementsMapping iElementsMapping, char const* iElementName)
    {
        if (iElementsMapping == emName)
        {
            mElementName = iElementName;
        }
    }

//      ---<<< 整形処理 >>>---

    void writeIndent();

//      ---<<< XML文字列へエンコードして保存 >>>---

    void encodeXmlString(std::string const& iString);

//      ---<<< プリミティブ名からC++型名返却 >>>---

#ifdef THEOLIZER_INTERNAL_ENABLE_META_SERIALIZER
    static char const* getCppName(std::string const& iPrimitiveName, unsigned iSerializerVersionNo)
    {
        return getCppNameXml(iPrimitiveName, iSerializerVersionNo);
    }
#endif  // THEOLIZER_INTERNAL_ENABLE_META_SERIALIZER
};
#ifndef THEOLIZER_INTERNAL_DOXYGEN

//----------------------------------------------------------------------------
//      プリミティブ名返却
//----------------------------------------------------------------------------

#define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSymbol)                    \
    template<>                                                              \
    inline char const* XmlMidOSerializer::                                  \
        getPrimitiveName<dType>(unsigned iSerializerVersionNo)              \
    {                                                                       \
        return PrimitiveName<XmlMidOSerializer, dType>::getPrimitiveName(iSerializerVersionNo);\
    }
#include "internal/primitive.inc"

#endif  // THEOLIZER_INTERNAL_DOXYGEN
// ***************************************************************************
/*!
@brief      回復用中間XmlSerializer
@details    主な処理はここで行うが、インスタンスを作れないよう抽象クラスとする。
*/
// ***************************************************************************

class THEOLIZER_INTERNAL_DLL XmlMidISerializer : public BaseSerializer
{
private:
    friend class BaseSerializer;
    THEOLIZER_INTERNAL_FRIENDS_FOR_INTERNAL;

//----------------------------------------------------------------------------
//      基本
//----------------------------------------------------------------------------

    std::istream&           mIStream;
    bool                    mReadComma;
    bool                    mTerminated;

    static const unsigned   kLastVersionNo=kXmlSerializerVersionNo;

    typedef XmlMidISerializer  MidSerializer;
    typedef XmlMidOSerializer  MetaOSerializer;    // メタ・シリアライザ
    typedef XmlMidISerializer  MetaISerializer;    // メタ・デシリアライザ

protected:
    bool                            mCharIsMultiByte;
    static char const* const        kSerializerName;
    static std::ios_base::openmode  kOpenMode;

    XmlMidISerializer
    (
        std::istream& iIStream,
        Destinations const& iDestinations,
        GlobalVersionNoTableBase const*const iGlobalVersionNoTable,
        unsigned iLastGlobalVersionNo,
        std::ostream* iOStream,
        bool iNoThrowException
    );
    ~XmlMidISerializer();

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

    void loadObjectId(std::size_t& oObjectId, std::size_t iTypeIndex);

    void loadControl(int&)                 {}
    void loadControl(long&)                {}
    void loadControl(long long&)           {}
    void loadControl(unsigned&)            {}
    void loadControl(unsigned long&)       {}
    void loadControl(unsigned long long&)  {}
    void loadControl(std::string&)         {}

//      ---<<< 要素名処理 >>>---

    class AutoReleaseTagName
    {
        XmlMidISerializer&  mXmlMidISerializer;
        std::string         mTagName;
    public:
        AutoReleaseTagName(XmlMidISerializer& iXmlMidISerializer, std::size_t iTypeIndex);
        ~AutoReleaseTagName();
    };

//      ---<<< プリミティブ回復 >>>---

    #define THEOLIZER_INTERNAL_DEF_LOAD
    #include "internal/primitive.inc"

//      ---<<< Element前処理 >>>---
//          戻り値：
//              次がないなら、Terminated(mTerminated=true)
//              次があるなら、Continue  (mTerminated=false)
//              それ以外なら、例外

    // readPreElementで回復し、loadTagで確認
    ReadStat readPreElement(bool iDoProcess=false);

//      ---<<< 要素破棄処理 >>>---

    void disposeElement();

//----------------------------------------------------------------------------
//      内部処理
//----------------------------------------------------------------------------

private:

//      ---<<< タグ回復と確認 >>>---

    // mTagInfoが有効ならmTagInfoと一致チェック、そうでないなら回復してチェック
    char const* loadTag(TagKind iTagKind, std::string const& iName, Attribute* iAttribute=nullptr);

    // タグ情報回復
    struct TagInfo
    {
        bool        mValid;
        TagKind     mTagKind;
        std::string mTagName;
        std::string mTypeName;
        std::string mMemberName;
        Attribute   mAttribute;
        TagInfo() : mValid(false), mTagKind(TagKind::Start) { }
    } mTagInfo;
    void loadTagInfo();

//      ---<<< グループ処理 >>>---
//          loadGroupEnd()呼び出し以前に、readPreElement()呼び出しにより、
//          mTerminatedをtrueにしておくこと。

    void loadGroupStart(bool iIsTop=false);
    void loadGroupEnd(bool iIsTop=false);

//      ---<<< 各種構造処理 >>>---

    void loadStructureStart(Structure iStructure, std::string& ioTypeName, std::size_t* oObjectId);
    void loadStructureEnd(Structure iStructure, std::string const& iTypeName);
    void loadTypeName(std::string& oTypeName);

//      ---<<< プリミティブ名返却 >>>---

    template<typename tType>
    static char const* getPrimitiveName(unsigned iSerializerVersionNo)
    {
        static_assert(Ignore<tType>::kFalse, "Unknown primitive name.");
        return "";
    }

//      ---<<< Element名回復 >>>---
//          ヘッダが有る時はElementsMappingの値に関係なく名前対応可能である。
//              そのような派生Serializerに対応するためのI/Fである。
//              なお、XmlMid?Serializerはこの機能に対応していない。
//              ヘッダはデバッグ用オプションなので、ヘッダの有無で動作を変えないため。
//          戻り値：
//              名前対応するなら、名前返却
//              順序対応するなら、empty返却
//          注意事項：
//              同じTheolizerVersion<>に対しては名前対応と順序対応を変えてはいけない

    std::string loadElementName(ElementsMapping iElementsMapping);

//      ---<<< XML文字列をデコードして文字列回復 >>>---

    void decodeXmlString(std::string& iString);

//      ---<<< 1文字読み出し >>>---

    char getChar();

//      ---<<< 指定以外の文字まで読み飛ばし >>>---
//          指定文字列に含まれない文字が出て来るまで、ストリームから読み捨てる。
//          戻り値は最後に読み出した文字

    char find_not_of(std::string const& iSkipChars);

//      ---<<< プリミティブ名からC++型名返却 >>>---

#ifdef THEOLIZER_INTERNAL_ENABLE_META_SERIALIZER
    static char const* getCppName(std::string const& iPrimitiveName, unsigned iSerializerVersionNo)
    {
        return getCppNameXml(iPrimitiveName, iSerializerVersionNo);
    }
#endif  // THEOLIZER_INTERNAL_ENABLE_META_SERIALIZER
};
#ifndef THEOLIZER_INTERNAL_DOXYGEN

//----------------------------------------------------------------------------
//      プリミティブ名返却
//----------------------------------------------------------------------------

#define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSymbol)                    \
    template<>                                                              \
    inline char const* XmlMidISerializer::                                  \
        getPrimitiveName<dType>(unsigned iSerializerVersionNo)              \
    {                                                                       \
        return PrimitiveName<XmlMidISerializer, dType>::getPrimitiveName(iSerializerVersionNo);\
    }
#include "internal/primitive.inc"

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

//############################################################################
//      Xml Serializer I/Fクラス
//          保存先をテンプレートで指定する。
//
//      注意事項：
//          XmlBaseI/OSerializerから派生してテンプレート化している。
//          XmlBaseI/OSerializerを直接保存先のテンプレートとした場合、
//          使用した保存先全てに対して、コードが複製される。
//          それを防ぐことが目的である。
//############################################################################

// ***************************************************************************
//!     保存用XmlSerializer
// ***************************************************************************

//      ---<<< 通常用 >>>---

template<Destination uDefault=theolizerD::All, Destination... uDestinations>
class XmlOSerializer : protected internal::XmlMidOSerializer
{
    THEOLIZER_INTERNAL_FRIENDS;
    void AbstructSerializer() { }       // インスタンス生成許可

    static internal::Destinations const&    getDestinations()
    {
        static const internal::Destinations destinations{uDefault, uDestinations...};
        return destinations;
    }

    // グローバル・バージョン番号テーブルへのポインタへのポインタ
    //  グローバル・バージョン番号テーブルへのポインタの値は初期化時、
    //  確定していない可能性があるが、ポインタ自身のアドレスは確定している。
    //  なお、sGlobalVersionNoTableは外部リンケージを持たないものであり、
    //  Theolizerのクライアントをコンパイルする時に使われるsGlobalVersionNoTableを
    //  取込む必要があるため、テンプレートであるトップ・シリアライザで処理する。
    constexpr static internal::GlobalVersionNoTableBase const*const*const
        kPtrGlobalVersionNoTable=&internal::sGlobalVersionNoTable;

    typedef XmlOSerializer                 DestOSerializer;
    typedef internal::XmlMidOSerializer    MidSerializer;

    // Switcherアクセス用
    using MidSerializer::kSerializerName;
    using BaseSerializer::mIsSaver;

public:
    //! ofstream用std::ios_base::openmode
    using MidSerializer::kOpenMode;

#ifndef THEOLIZER_INTERNAL_DOXYGEN
    static const bool                       kHasDestination=true;
#endif  // THEOLIZER_INTERNAL_DOXYGEN

    //! 通常のコンストラクタ
    XmlOSerializer
    (
        std::ostream& iOStream,
        unsigned iGlobalVersionNo=kLastGlobalVersionNo,
        bool iNoPrettyPrint=false,
        bool iNoThrowException=false
    ) : XmlMidOSerializer
        (
            iOStream,
            getDestinations(),
            internal::sGlobalVersionNoTable,
            iGlobalVersionNo,
            kLastGlobalVersionNo,
            iNoPrettyPrint,
            iNoThrowException
        )
    { }

    //! GlobalVersionNo以外のオプションを指定するコンストラクタ
    XmlOSerializer
    (
        std::ostream& iOStream,
        bool iNoPrettyPrint,
        bool iNoThrowException=false
    ) : XmlMidOSerializer
        (
            iOStream,
            getDestinations(),
            internal::sGlobalVersionNoTable,
            kLastGlobalVersionNo,
            kLastGlobalVersionNo,
            iNoPrettyPrint,
            iNoThrowException
        )
    { }

    //! XmlOSerializerのプロパティ返却（@ref Property 参照）
    static bool hasProperty(Property iProperty)
    {
        return internal::hasPropertyXml(iProperty, true);
    }

    //! std::stringをマルチ・パイト文字コードとして処理するかどうか指定する
    void setCharIsMultiByte(bool iCharIsMultiByte)
    {
        mCharIsMultiByte=iCharIsMultiByte;
    }

    using BaseSerializer::getGlobalVersionNo;
    using BaseSerializer::clearTracking;
    using BaseSerializer::getRequireClearTracking;
    using BaseSerializer::getCheckMode;
    using ErrorBase::getErrorInfo;
    using ErrorBase::isError;
    using ErrorBase::resetError;
};

#ifndef THEOLIZER_INTERNAL_DOXYGEN
// MidSerializer登録(トライバ処理用)
THEOLIZER_INTERNAL_REGISTER_MID_SERIALIZER(XmlOSerializer);
#endif  // THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//!     回復用XmlSerializer
// ***************************************************************************

//      ---<<< 通常用 >>>---

template<Destination uDefault=theolizerD::All, Destination... uDestinations>
class XmlISerializer : protected internal::XmlMidISerializer
{
    THEOLIZER_INTERNAL_FRIENDS;
    void AbstructSerializer() { }       // インスタンス生成許可

    static internal::Destinations const&    getDestinations()
    {
        static const internal::Destinations destinations{uDefault, uDestinations...};
        return destinations;
    }

    // グローバル・バージョン番号テーブルへのポインタへのポインタ
    constexpr static internal::GlobalVersionNoTableBase const*const*const
        kPtrGlobalVersionNoTable=&internal::sGlobalVersionNoTable;

    typedef XmlISerializer                 DestISerializer;
    typedef internal::XmlMidISerializer    MidSerializer;

    // Switcherアクセス用
    using MidSerializer::kSerializerName;
    using BaseSerializer::mIsSaver;

public:
    //! ifstream用std::ios_base::openmode
    using MidSerializer::kOpenMode;

#ifndef THEOLIZER_INTERNAL_DOXYGEN
    static const bool                       kHasDestination=true;
#endif  // THEOLIZER_INTERNAL_DOXYGEN

    //! コンストラクタ
    XmlISerializer
    (
        std::istream& iIStream,
        bool iNoThrowException=false
    ) : XmlMidISerializer
        (
            iIStream,
            getDestinations(),
            internal::sGlobalVersionNoTable,
            kLastGlobalVersionNo,
            nullptr,
            iNoThrowException
        )
    { }

    //! XmlISerializerのプロパティ返却（@ref Property 参照）
    static bool hasProperty(Property iProperty)
    {
        return internal::hasPropertyXml(iProperty, false);
    }

    //! std::stringをマルチ・パイト文字コードとして処理することを指定する
    void setCharIsMultiByte(bool iCharIsMultiByte)
    {
        mCharIsMultiByte=iCharIsMultiByte;
    }

    using BaseSerializer::getGlobalVersionNo;
    using BaseSerializer::clearTracking;
    using BaseSerializer::getRequireClearTracking;
    using BaseSerializer::getCheckMode;
    using ErrorBase::getErrorInfo;
    using ErrorBase::isError;
    using ErrorBase::resetError;

#ifndef THEOLIZER_INTERNAL_DOXYGEN
    // メタ・デシリアライズ用
    /*
        メタ・シリアライズ処理検討後決定
        現時点でメタ・シリアライズは実験的な実装なので、実用性がない。
        それを製品版に含めるのはどうかと思う。
        できるだけ、別ファイルへ分離することを検討する。
    */
    XmlISerializer
    (
        std::istream& iIStream,
        std::ostream& iOStream,
        bool iNoThrowException=false
    ) : XmlMidISerializer
        (
            iIStream,
            getDestinations(),
            internal::sGlobalVersionNoTable,
            kLastGlobalVersionNo,
            &iOStream,
            iNoThrowException
        )
    { }
#endif // THEOLIZER_INTERNAL_DOXYGEN
};

#ifndef THEOLIZER_INTERNAL_DOXYGEN
// MidSerializer登録(トライバ処理用)
THEOLIZER_INTERNAL_REGISTER_MID_SERIALIZER(XmlISerializer);
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

#endif  // THEOLIZER_INTERNAL_SERIALIZER_XML_H
