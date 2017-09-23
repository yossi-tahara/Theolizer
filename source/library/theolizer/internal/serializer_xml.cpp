//############################################################################
//      XMLシリアライザ
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

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#if defined(_MSC_VER)
    #pragma warning(disable:4100)
#endif

//############################################################################
//      インクルード
//############################################################################

#include "avoid-trouble.h"
#include "internal.h"

#include <limits>
#include <sstream>

#include "../serializer_xml.h"

//############################################################################
//      Begin
//############################################################################

namespace theolizer
{
namespace internal
{
//############################################################################
//      保存／回復共通
//############################################################################

// ***************************************************************************
//      XMLタグ種別
// ***************************************************************************

std::ostream& operator<<(std::ostream& iOStream, TagKind iTagKind)
{
    switch(iTagKind)
    {
    case TagKind::Start:
        iOStream << "Start";
        break;

    case TagKind::End:
        iOStream << "End";
        break;

    case TagKind::StartEnd:
        iOStream << "StartEnd";
        break;

    default:
        THEOLIZER_INTERNAL_ABORT("Unknown TagKind in operator<<(TagKind)");
        break;
    }

    return iOStream;
}

// ***************************************************************************
//      構造名取得
// ***************************************************************************

char const* Attribute::getStructure() const
{
    switch(mStructure)
    {
    case Structure::None:
    case Structure::Class:
        break;

    case Structure::Array:
        return THEOLIZER_INTERNAL_XML_NAMESPACE ":Array";

    case Structure::Pointee:
        return THEOLIZER_INTERNAL_XML_NAMESPACE ":Pointee";

    case Structure::Pointer:
        return THEOLIZER_INTERNAL_XML_NAMESPACE ":Pointer";

    case Structure::OwnerPointer:
        return THEOLIZER_INTERNAL_XML_NAMESPACE ":OwnerPointer";

    case Structure::Reference:
        return THEOLIZER_INTERNAL_XML_NAMESPACE ":Reference";
    }
    return nullptr;
}

// ***************************************************************************
//      テンプレート型名変換
//          '<', '>'を'-'へ、','を'.'へ置き換え、xmlの要素名に使える文字へ変換する
// ***************************************************************************

void modifyTypeName(std::string& iTypeName)
{
    std::size_t pos = iTypeName.find('<');
    if (pos != std::string::npos)
    {
        iTypeName[pos]='-';
        for (auto it=iTypeName.begin()+pos+1, ed = iTypeName.end(); it != ed; ++it)
        {
            switch(*it)
            {
            case '<':
                *it = '-';
                break;

            case '>':
                *it = '-';
                break;

            case ',':
                *it = '.';
                break;
            }
        }
    }
}

//############################################################################
//      保存
//############################################################################

// ***************************************************************************
//      シリアライザ名
// ***************************************************************************

char const* const   XmlMidOSerializer::kSerializerName=
        "theolizer::internal::XmlMidOSerializer";

// ***************************************************************************
//      fstreamのオープン・モード
// ***************************************************************************

std::ios_base::openmode XmlMidOSerializer::kOpenMode=std::ios_base::openmode();

// ***************************************************************************
//      コンストラクタ
// ***************************************************************************

XmlMidOSerializer::XmlMidOSerializer
(
    std::ostream& iOStream,
    Destinations const& iDestinations,
    GlobalVersionNoTableBase const*const iGlobalVersionNoTable,
    unsigned iGlobalVersionNo,
    unsigned iLastGlobalVersionNo,
    bool iNoPrettyPrint,
    bool mNoThrowException
) : BaseSerializer
    (
        std::move(iDestinations),
        iGlobalVersionNoTable,
        iGlobalVersionNo,
        iLastGlobalVersionNo,
        CheckMode::TypeCheckInData,
        true,
        nullptr,
        mNoThrowException
    ),
    mOStream(iOStream),
    mNoPrettyPrint(iNoPrettyPrint),
    mCharIsMultiByte(false),
    mElementName(nullptr)
{
    // エラー情報登録準備
    theolizer::internal::ApiBoundary aApiBoundary(&mAdditionalInfo);

    // 型情報取得中継クラス登録
    TypeFunctions<XmlMidOSerializer>   aTypeFunctions;

    if (getNoThrowException())
    {
        try
        {
            // 通常ヘッダ保存
            writeHeader();
        }
        catch (ErrorInfo&)
        {
        }
    }
    else
    {
        // 通常ヘッダ保存
        writeHeader();
    }

//    mOStream << "\n";
}

// ***************************************************************************
//      デストラクタ
// ***************************************************************************

XmlMidOSerializer::~XmlMidOSerializer()
{
    try
    {
        mOStream << "\n";
        saveTag
        (
            TagKind::End,
            THEOLIZER_INTERNAL_XML_NAMESPACE ":" THEOLIZER_INTERNAL_XML_THEOLIZER_NAME
        );
        mOStream << "\n";
    }
    catch (ErrorInfo&)
    {
        // ErrorReporterに記録される
    }
}

// ***************************************************************************
//      型情報保存
// ***************************************************************************

//----------------------------------------------------------------------------
//      ヘッダ保存
//----------------------------------------------------------------------------

void XmlMidOSerializer::writeHeader()
{
//      ---<<< XMLヘッダ保存 >>>---

    mOStream << THEOLIZER_INTERNAL_XML_HEADER;

    Attribute   aAttribute;
    aAttribute.mXmlns=THEOLIZER_INTERNAL_XML_URI;
    aAttribute.mGlobalVersionNo=mGlobalVersionNo;
    saveTag
    (
        TagKind::Start,
        THEOLIZER_INTERNAL_XML_NAMESPACE ":" THEOLIZER_INTERNAL_XML_THEOLIZER_NAME,
        &aAttribute
    );

    // バージョン番号対応表生成
    createVersionNoTable();
}

// ***************************************************************************
//      内部処理
// ***************************************************************************

//----------------------------------------------------------------------------
//      型情報保存
//----------------------------------------------------------------------------

void XmlMidOSerializer::saveObjectId(std::size_t iObjectId, std::size_t iTypeIndex)
{
    Attribute   aAttribute;
    aAttribute.mStructure = Structure::Pointer;
    aAttribute.mObjectId = iObjectId;
    writeIndent();
    saveTag(TagKind::StartEnd, getTypeName(iTypeIndex), &aAttribute);
}

//----------------------------------------------------------------------------
//      要素名処理
//----------------------------------------------------------------------------

XmlMidOSerializer::AutoReleaseTagName::AutoReleaseTagName
(
    XmlMidOSerializer& iXmlMidOSerializer,
    std::size_t iTypeIndex
) : mXmlMidOSerializer(iXmlMidOSerializer),
    mTagName(mXmlMidOSerializer.getTypeName(iTypeIndex))
{
    mXmlMidOSerializer.writeIndent();
    mXmlMidOSerializer.saveTag(TagKind::Start, mTagName);
}

XmlMidOSerializer::AutoReleaseTagName::~AutoReleaseTagName()
{
    mXmlMidOSerializer.saveTag(TagKind::End, mTagName);
}

//----------------------------------------------------------------------------
//      10進数変換して精度が劣化しない桁数
//          https://ja.wikipedia.org/wiki/IEEE_754
//          必要な桁数は、1 + ceiling(p×log102)
//          ここで、pはその二進形式の仮数のビット数で、例えば binary32なら24ビットである。
//----------------------------------------------------------------------------

template<typename tType, class tEnable=void>
struct Decimal
{
    static const int    kDigit=9;
};

template<typename tType>
struct Decimal
<
    tType,
    EnableIf
    <
            (24 < std::numeric_limits<tType>::digits)
         && (std::numeric_limits<tType>::digits <= 53)
    >
>
{
    static const int    kDigit=17;
};

template<typename tType>
struct Decimal
<
    tType,
    EnableIf
    <
            (53 < std::numeric_limits<tType>::digits)
         && (std::numeric_limits<tType>::digits <= 64)
    >
>
{
    static const int    kDigit=21;
};

template<typename tType>
struct Decimal
<
    tType,
    EnableIf<(64 < std::numeric_limits<tType>::digits)>
>
{
    static_assert(Ignore<tType>::kFalse, "This is not supported floating point type.");
};

//----------------------------------------------------------------------------
//      プリミティブ処理
//----------------------------------------------------------------------------

//      ---<<< 整数型 >>>---

#define THEOLIZER_INTERNAL_DEF_INTEGRAL(dType, dSimbol)                     \
    void XmlMidOSerializer::savePrimitive(dType const& iPrimitive)          \
    {                                                                       \
        AutoReleaseTagName aAutoReleaseTagName(*this, getTypeIndex<dType>());\
        if (std::numeric_limits<dType>::is_signed) {                        \
            mOStream << static_cast<long long>(iPrimitive);                 \
        } else {                                                            \
            mOStream << static_cast<unsigned long long>(iPrimitive);        \
        }                                                                   \
        checkStreamError(mOStream.rdstate());                               \
    }

//      ---<<< 浮動小数点型 >>>---

#define THEOLIZER_INTERNAL_DEF_FLOATING_POINT(dType, dSimbol)               \
    void XmlMidOSerializer::savePrimitive(dType const& iPrimitive)          \
    {                                                                       \
        AutoReleaseTagName aAutoReleaseTagName(*this, getTypeIndex<dType>());\
        std::streamsize precision=mOStream.precision();                     \
        mOStream.precision(Decimal<dType>::kDigit);                         \
        mOStream << iPrimitive;                                             \
        mOStream.precision(precision);                                      \
        checkStreamError(mOStream.rdstate());                               \
    }

//      ---<<< 文字列型 >>>---

#define THEOLIZER_INTERNAL_DEF_NARROW_STRING(dType, dSimbol)                \
    void XmlMidOSerializer::savePrimitive(dType const& iPrimitive)          \
    {                                                                       \
        AutoReleaseTagName aAutoReleaseTagName(*this, getTypeIndex<dType>());\
        if (mCharIsMultiByte)                                               \
        {                                                                   \
            u8string temp(iPrimitive, MultiByte());                         \
            encodeXmlString(std::move(temp.str()));                         \
        }                                                                   \
        else                                                                \
        {                                                                   \
            encodeXmlString(iPrimitive);                                    \
        }                                                                   \
    }

#define THEOLIZER_INTERNAL_DEF_WIDE_STRING(dType, dSimbol)                  \
    void XmlMidOSerializer::savePrimitive(dType const& iPrimitive)          \
    {                                                                       \
        AutoReleaseTagName aAutoReleaseTagName(*this, getTypeIndex<dType>());\
        u8string temp(iPrimitive);                                          \
        encodeXmlString(temp.str());                                        \
    }

//      ---<<< 実体定義 >>>---

#include "primitive.inc"

// ***************************************************************************
//      ユーティリティ
// ***************************************************************************

//----------------------------------------------------------------------------
//      タグ保存
//----------------------------------------------------------------------------

char const* XmlMidOSerializer::saveTag
(
    TagKind             iTagKind,
    std::string const&  iName,
    Attribute const*    iAttribute
)
{
    char const* aTagName = nullptr;
    if (iAttribute)
    {
        aTagName = iAttribute->getStructure();
    }
    if (iTagKind != TagKind::End)
    {
        mOStream << "<" << ((aTagName)?aTagName:iName.c_str());
        if (mElementName)
        {
            mOStream << " " THEOLIZER_INTERNAL_XML_NAMESPACE ":MemberName="
                        "\"" << mElementName << "\"";
            mElementName = nullptr;
        }
        if (iAttribute)
        {
            if (iAttribute->mStructure >= Structure::Pointer)
            {
                mOStream << " " THEOLIZER_INTERNAL_XML_NAMESPACE ":Type=\""
                         << iName << '\"';
            }
            if (iAttribute->mObjectId != kInvalidSize)
            {
                mOStream << " " THEOLIZER_INTERNAL_XML_NAMESPACE ":ObjectId=\""
                         << iAttribute->mObjectId << '\"';
            }
            if (!iAttribute->mXmlns.empty())
            {
                mOStream << " xmlns:" THEOLIZER_INTERNAL_XML_NAMESPACE "="
                            "\"" THEOLIZER_INTERNAL_XML_URI "\"";
            }
            if (iAttribute->mGlobalVersionNo)
            {
                mOStream << " " THEOLIZER_INTERNAL_XML_NAMESPACE ":GlobalVersionNo=\""
                         << iAttribute->mGlobalVersionNo << '\"';
            }
        }
        if (iTagKind == TagKind::StartEnd)
        {
            mOStream << "/";
        }
        mOStream << ">";
    }
    else
    {
        mOStream << "</" << iName << ">";
    }

    return aTagName;
}

//----------------------------------------------------------------------------
//      各種構造処理
//----------------------------------------------------------------------------

//      ---<<< 開始処理 >>>---

void XmlMidOSerializer::saveStructureStart
(
    Structure       iStructure,
    std::string&    ioTypeName,
    std::size_t     iObjectId
)
{
std::cout << "saveStructureStart(" << ioTypeName << ") mNoPrettyPrint=" << mNoPrettyPrint << " mIndent=" << mIndent << "\n";
    writeIndent();
    mIndent++;
    modifyTypeName(ioTypeName);
    Attribute   aAttribute;
    aAttribute.mStructure = iStructure;
    aAttribute.mObjectId = iObjectId;
    char const* ret = saveTag(TagKind::Start, ioTypeName, &aAttribute);
    if (ret)
    {
        ioTypeName = ret;
    }
}

//      ---<<< 終了処理 >>>---

void XmlMidOSerializer::saveStructureEnd(Structure iStructure, std::string const& iTypeName)
{
    writeIndent();
    saveTag(TagKind::End, iTypeName);
}

//----------------------------------------------------------------------------
//      整形処理
//----------------------------------------------------------------------------

void XmlMidOSerializer::writeIndent()
{
    if (!mNoPrettyPrint)
    {
        mOStream << "\n";
        for (int i=0; i < mIndent; ++i)
            mOStream << "    ";
    }
}

//----------------------------------------------------------------------------
//      JSON文字列へエンコードして保存
//----------------------------------------------------------------------------

void XmlMidOSerializer::encodeXmlString(std::string const& iString)
{
    mOStream << "\"";
    for (auto ch : iString)
    {
        switch(ch)
        {
        case '\"':      mOStream << "\\\"";     break;
        case '\\':      mOStream << "\\\\";     break;
        case '/':       mOStream << "\\/";      break;
        case '\x08':    mOStream << "\\b";      break;
        case '\x0C':    mOStream << "\\f";      break;
        case '\n':      mOStream << "\\n";      break;
        case '\r':      mOStream << "\\r";      break;
        case '\t':      mOStream << "\\t";      break;
        default:        mOStream << ch;         break;
        }
    }
    mOStream << "\"";
    checkStreamError(mOStream.rdstate());
}

//############################################################################
//      回復
//############################################################################

// ***************************************************************************
//      シリアライザ名
// ***************************************************************************

char const* const   XmlMidISerializer::kSerializerName=
        "theolizer::internal::XmlMidISerializer";

// ***************************************************************************
//      fstreamのオープン・モード
// ***************************************************************************

std::ios_base::openmode XmlMidISerializer::kOpenMode=std::ios_base::openmode();

// ***************************************************************************
//      コンストラクタ
// ***************************************************************************

XmlMidISerializer::XmlMidISerializer
(
    std::istream& iIStream,
    Destinations const& iDestinations,
    GlobalVersionNoTableBase const*const iGlobalVersionNoTable,
    unsigned iLastGlobalVersionNo,
    std::ostream* iOStream,
    bool mNoThrowException
) : BaseSerializer
    (
        std::move(iDestinations),
        iGlobalVersionNoTable,
        0,
        iLastGlobalVersionNo,
        CheckMode::TypeCheckInData,
        false,
        iOStream,
        mNoThrowException
    ),
    mIStream(iIStream),
    mReadComma(false),
    mTerminated(false),
    mCharIsMultiByte(false)
{
    // エラー情報登録準備
    theolizer::internal::ApiBoundary aApiBoundary(&mAdditionalInfo, true);

    // 型情報取得中継クラス登録
    TypeFunctions<XmlMidISerializer>   aTypeFunctions;

    // ヘッダ処理
    if (getNoThrowException())
    {
        try
        {
            // 通常ヘッダ回復
            readHeader();
        }
        catch (ErrorInfo&)
        {
        }
    }
    else
    {
        // 通常ヘッダ回復
        readHeader();
    }
}

// ***************************************************************************
//      デストラクタ
// ***************************************************************************

XmlMidISerializer::~XmlMidISerializer()
{
    try
    {
        loadTag
        (
            TagKind::End,
            THEOLIZER_INTERNAL_XML_NAMESPACE ":" THEOLIZER_INTERNAL_XML_THEOLIZER_NAME
        );
    }
    catch (ErrorInfo&)
    {
        // ErrorReporterに記録される
    }
}

// ***************************************************************************
//      ヘッダ回復と型チェック
// ***************************************************************************

//----------------------------------------------------------------------------
//      ヘッダ情報回復
//----------------------------------------------------------------------------

void XmlMidISerializer::readHeader()
{
//      ---<<< XMLヘッダ回復 >>>---

    std::size_t size=sizeof(THEOLIZER_INTERNAL_XML_HEADER)-1;
    std::string header(size, 0);
    mIStream.read(&(*header.begin()), size);
    if (header != THEOLIZER_INTERNAL_XML_HEADER)
    {
        THEOLIZER_INTERNAL_DATA_ERROR("XML Header Error(%1%).", header);
    }

    Attribute   aAttribute;
    aAttribute.mXmlns = THEOLIZER_INTERNAL_XML_URI;
    loadTag
    (
        TagKind::Start,
        THEOLIZER_INTERNAL_XML_NAMESPACE ":" THEOLIZER_INTERNAL_XML_THEOLIZER_NAME,
        &aAttribute
    );
    if (aAttribute.mGlobalVersionNo == 0)
    {
        THEOLIZER_INTERNAL_DATA_ERROR
            ("XmlMidISerializer : No global version number.");
    }
    mGlobalVersionNo = aAttribute.mGlobalVersionNo;
#ifdef THEOLIZER_INTERNAL_ENABLE_META_SERIALIZER
    // メタ・データ回復時は型情報は存在しないので生成しない
    if (!mIsMetaMode)
#endif  // THEOLIZER_INTERNAL_ENABLE_META_SERIALIZER
    {
        // バージョン番号対応表生成
        createVersionNoTable();

        // 型名と型／バージョン番号対応表生成
        createTypeNameMap();
    }
//std::cout << "mGlobalVersionNo=" << mGlobalVersionNo << "\n";
}

//----------------------------------------------------------------------------
//      TypeIndex一致判定
//----------------------------------------------------------------------------

bool XmlMidISerializer::isMatchTypeIndex(size_t iSerializedTypeIndex,
                                          size_t iProgramTypeIndex)
{
    return BaseSerializer::isMatchTypeIndex(iSerializedTypeIndex, iProgramTypeIndex);
}

// ***************************************************************************
//      データ回復
// ***************************************************************************

//----------------------------------------------------------------------------
//      型情報保存
//----------------------------------------------------------------------------

void XmlMidISerializer::loadObjectId(std::size_t& oObjectId, std::size_t iTypeIndex)
{
    if (!mTagInfo.mValid)
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
    }
    mTagInfo.mValid = false;
    if (mTagInfo.mTypeName != getTypeName(iTypeIndex))
    {
        THEOLIZER_INTERNAL_DATA_ERROR
            ("XmlMidISerializer : Illegal type name(data:%1%, program:%2%).",
                mTagInfo.mTagName, getTypeName(iTypeIndex));
    }

    oObjectId = mTagInfo.mAttribute.mObjectId;
}

//----------------------------------------------------------------------------
//      要素名処理
//----------------------------------------------------------------------------

XmlMidISerializer::AutoReleaseTagName::AutoReleaseTagName
(
    XmlMidISerializer& iXmlMidISerializer,
    std::size_t iTypeIndex
) : mXmlMidISerializer(iXmlMidISerializer),
    mTagName(mXmlMidISerializer.getTypeName(iTypeIndex))
{
std::cout << "AutoReleaseTagName()\n";
    mXmlMidISerializer.loadTag(TagKind::Start, mTagName);
}

XmlMidISerializer::AutoReleaseTagName::~AutoReleaseTagName()
{
std::cout << "~AutoReleaseTagName()\n";
    mXmlMidISerializer.loadTag(TagKind::End, mTagName);
}

//----------------------------------------------------------------------------
//      プリミティブ処理
//----------------------------------------------------------------------------

//      ---<<< bool型 >>>---

#define THEOLIZER_INTERNAL_DEF_BOOL(dType, dSimbol)                         \
    void XmlMidISerializer::loadPrimitive(bool& oPrimitive)                 \
    {                                                                       \
        AutoReleaseTagName aAutoReleaseTagName(*this, getTypeIndex<dType>());\
        long long data(0);                                                  \
        mIStream >> data;                                                   \
        checkStreamError(mIStream.rdstate());                               \
        long long min = std::numeric_limits<bool>::min();                   \
        long long max = std::numeric_limits<bool>::max();                   \
        if ((data < min) || (max < data)) {                                 \
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Data overflow.");              \
        }                                                                   \
        oPrimitive = (data != 0);                                           \
    }

//      ---<<< 整数型 >>>---

#define THEOLIZER_INTERNAL_DEF_INTEGRAL(dType, dSimbol)                     \
    void XmlMidISerializer::loadPrimitive(dType& oPrimitive)                \
    {                                                                       \
        AutoReleaseTagName aAutoReleaseTagName(*this, getTypeIndex<dType>());\
        if (std::numeric_limits<dType>::is_signed)                          \
        {                                                                   \
            long long data(0);                                              \
            mIStream >> data;                                               \
            checkStreamError(mIStream.rdstate());                           \
            long long min = std::numeric_limits<dType>::min();              \
            long long max = std::numeric_limits<dType>::max();              \
            if ((data < min) || (max < data)) {                             \
                THEOLIZER_INTERNAL_DATA_ERROR(u8"Data overflow.");          \
            }                                                               \
            oPrimitive = static_cast<dType>(data);                          \
        }                                                                   \
        else                                                                \
        {                                                                   \
            unsigned long long data(0);                                     \
            mIStream >> data;                                               \
            checkStreamError(mIStream.rdstate());                           \
            unsigned long long max = std::numeric_limits<dType>::max();     \
            if (max < data) {                                               \
                THEOLIZER_INTERNAL_DATA_ERROR(u8"Data overflow.");          \
            }                                                               \
            oPrimitive = static_cast<dType>(data);                          \
        }                                                                   \
    }

//      ---<<< 浮動小数点型 >>>---

#define THEOLIZER_INTERNAL_DEF_FLOATING_POINT(dType, dSimbol)               \
    void XmlMidISerializer::loadPrimitive(dType& oPrimitive)                \
    {                                                                       \
        AutoReleaseTagName aAutoReleaseTagName(*this, getTypeIndex<dType>());\
        mIStream >> oPrimitive;                                             \
        checkStreamError(mIStream.rdstate());                               \
    }

//      ---<<< 文字列型 >>>---

#define THEOLIZER_INTERNAL_DEF_NARROW_STRING(dType, dSimbol)                \
    void XmlMidISerializer::loadPrimitive(dType& oPrimitive)                \
    {                                                                       \
        AutoReleaseTagName aAutoReleaseTagName(*this, getTypeIndex<dType>());\
        if (mCharIsMultiByte)                                               \
        {                                                                   \
            u8string temp;                                                  \
            decodeXmlString(temp.str());                                    \
            oPrimitive=std::move(temp.getMultiByte());                      \
        }                                                                   \
        else                                                                \
        {                                                                   \
            decodeXmlString(oPrimitive);                                    \
        }                                                                   \
    }

#define THEOLIZER_INTERNAL_DEF_WIDE_STRING(dType, dSimbol)                  \
    void XmlMidISerializer::loadPrimitive(dType& oPrimitive)                \
    {                                                                       \
        AutoReleaseTagName aAutoReleaseTagName(*this, getTypeIndex<dType>());\
        u8string temp;                                                      \
        decodeXmlString(temp.str());                                        \
        oPrimitive=temp;                                                    \
    }

//      ---<<< 実体定義 >>>---

#include "primitive.inc"

//----------------------------------------------------------------------------
//      Element前処理
//----------------------------------------------------------------------------

ReadStat XmlMidISerializer::readPreElement(bool iDoProcess)
{
    if (iDoProcess || mTagInfo.mValid)
return Continue;

std::cout << "(readPreElement)";
        loadTagInfo();
    return (mTagInfo.mTagKind != TagKind::End)?Continue:Terminated;
}

//----------------------------------------------------------------------------
//      Element名取出し
//----------------------------------------------------------------------------

std::string XmlMidISerializer::loadElementName(ElementsMapping iElementsMapping)
{
    if (!mTagInfo.mValid)
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
    }

    return std::move(mTagInfo.mMemberName);
}

// ***************************************************************************
//      要素破棄処理
// ***************************************************************************

void XmlMidISerializer::disposeElement()
{
    char in = find_not_of(" \t\n");

    // 次の処理に備えて、最後の文字を戻しておく
    mIStream.unget();

    switch(in)
    {
    case '\"':
        {
            std::string temp;
            decodeXmlString(temp);
        }
        break;

    case '{':
        disposeClass(emName);
        break;

    case '[':
        disposeClass(emOrder);
        break;

    default:
        {
            long double temp;
            mIStream >> temp;
        }
        break;
    }
}

// ***************************************************************************
//      ユーティリティ
// ***************************************************************************

//----------------------------------------------------------------------------
//      タグ回復と確認
//----------------------------------------------------------------------------

char const* XmlMidISerializer::loadTag
(
    TagKind             iTagKind,
    std::string const&  iName,
    Attribute*          iAttribute
)
{
std::cout << "<" << ((iTagKind==TagKind::End)?"/":"") << iName << ((iTagKind==TagKind::StartEnd)?"/":"") << ">";

    if (!mTagInfo.mValid)
    {
        loadTagInfo();
    }
    mTagInfo.mValid=false;

    // タグ種別チェック
    if (mTagInfo.mTagKind != iTagKind)
    {
        std::stringstream ss;
        ss << mTagInfo.mTagKind;
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.G(%1%)", ss.str());
    }

    // タグ名確認
    char const* aTagName = nullptr;
    if (iAttribute)
    {
        aTagName = iAttribute->getStructure();
    }
    if (!aTagName)
    {
        aTagName = iName.c_str();
    }
    if (mTagInfo.mTagName != aTagName)
    {
        THEOLIZER_INTERNAL_DATA_ERROR
            ("XmlMidISerializer : Illegal tag name(data:%1%, program:%2%).",
                mTagInfo.mTagName, iName);
    }

    // 属性返却(ObjectIdとGlobalVersionNo)
    if (iAttribute)
    {
        iAttribute->mObjectId        = mTagInfo.mAttribute.mObjectId;
        iAttribute->mGlobalVersionNo = mTagInfo.mAttribute.mGlobalVersionNo;
    }

    // 属性確認
    if (iAttribute && (iAttribute->mStructure >= Structure::Pointer))
    {
        if (iName != mTagInfo.mTypeName)
        {
            THEOLIZER_INTERNAL_DATA_ERROR
                ("XmlMidISerializer : Illegal type name(data:%1%, program:%2%).",
                mTagInfo.mTypeName, iName);
        }
    }
    if (mTagInfo.mAttribute.mXmlns.empty()
     != (!iAttribute || iAttribute->mXmlns.empty()))
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.(Xmlns)");
    }
    if (iAttribute && (mTagInfo.mAttribute.mXmlns != iAttribute->mXmlns))
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.(%1%)", mTagInfo.mAttribute.mXmlns);
    }

    return aTagName;
}

//----------------------------------------------------------------------------
//      タグ回復
//----------------------------------------------------------------------------

void XmlMidISerializer::loadTagInfo()
{
    if (mTagInfo.mValid)
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"mTagInfo is valid.");
    }

    static const std::string sDelimChar="> \t\n";
    mTagInfo.mTypeName.clear();
    mTagInfo.mMemberName.clear();
    mTagInfo.mAttribute = Attribute();
    mTagInfo.mTagKind = TagKind::Start;

    // タグ名獲得
    char in = find_not_of(" \t\n");
    if (in != '<')
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.A(%1%)", in);
    }
    in = find_not_of(" \t\n");
    if (in == '/')
    {
        mTagInfo.mTagKind = TagKind::End;
    }
    else
    {
        mIStream.unget();
    }

    mTagInfo.mTagName.clear();
    while(1)
    {
        in = getChar();
        // エラーが発生していたら、終了する(無限ループ回避)
        if (ErrorReporter::isError())
return;
        std::string::size_type pos = sDelimChar.find(in);
        if (pos != std::string::npos)
        {
            mIStream.unget();
    break;
        }
        mTagInfo.mTagName.push_back(in);
    }

    while(1)
    {
        in = find_not_of(" \t\n");
        if (in == '/')
        {
            mTagInfo.mTagKind = TagKind::StartEnd;
            in = find_not_of(" \t\n");
            if (in != '>')
            {
                THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.C(%1%)", in);
            }
    break;
        }
        if (in == '>')
        {
    break;
        }

        mIStream.unget();
        std::string aAttributeName;
        std::getline(mIStream, aAttributeName, '=');
        in=getChar();
        if (in != '\"')
        {
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.D(%1%)", in);
        }
        std::string aAttributeValue;
        std::getline(mIStream, aAttributeValue, '\"');
        checkStreamError(mIStream.rdstate());

std::cout << "    " << aAttributeName << "=" << aAttributeValue << "\n";

        // エラーが発生していたら、終了する(無限ループ回避)
        if (ErrorReporter::isError())
return;

        if (aAttributeName == THEOLIZER_INTERNAL_XML_NAMESPACE ":Type")
        {
            mTagInfo.mTypeName = aAttributeValue;
        }
        else if (aAttributeName == THEOLIZER_INTERNAL_XML_NAMESPACE ":MemberName")
        {
            mTagInfo.mMemberName = aAttributeValue;
        }
        else if (aAttributeName == THEOLIZER_INTERNAL_XML_NAMESPACE ":ObjectId")
        {
            mTagInfo.mAttribute.mObjectId = std::stoull(aAttributeValue);
        }
        else if (aAttributeName == "xmlns:" THEOLIZER_INTERNAL_XML_NAMESPACE)
        {
            mTagInfo.mAttribute.mXmlns = aAttributeValue;
        }
        else if (aAttributeName == THEOLIZER_INTERNAL_XML_NAMESPACE ":GlobalVersionNo")
        {
            mTagInfo.mAttribute.mGlobalVersionNo=
                static_cast<unsigned>(std::stoul(aAttributeValue));
        }
        else
        {
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.F(%1%)", aAttributeName);
        }
    }
    mTagInfo.mValid=true;

std::cout << "[" << mTagInfo.mTagName << ":" << mTagInfo.mTagKind << "]" << ((mTagInfo.mTagKind!=TagKind::Start)?"\n":"");
}

//----------------------------------------------------------------------------
//      グループ処理
//----------------------------------------------------------------------------

//      ---<<< 開始処理 >>>---

void XmlMidISerializer::loadGroupStart(bool iIsTop)
{
    mReadComma=false;
}

//      ---<<< 終了処理 >>>---

void XmlMidISerializer::loadGroupEnd(bool iIsTop)
{
#if 0
    if (!iIsTop || (CheckMode::TypeCheck <= mCheckMode))
    {
        // まだ終了処理されてないなら、終了処理する
        if (!mTerminated)
        {
            while (readPreElement())
            {
                // エラーが発生していたら、抜ける
                if (ErrorReporter::isError())
            break;

                disposeElement();
            }
        }
    }
#endif
    mTerminated=false;
    mReadComma=true;
}

//----------------------------------------------------------------------------
//      各種構造処理
//----------------------------------------------------------------------------

//      ---<<< 開始処理 >>>---

void XmlMidISerializer::loadStructureStart
(
    Structure       iStructure,
    std::string&    ioTypeName,
    std::size_t*    oObjectId
)
{
std::cout << "loadStructureStart(" << ioTypeName << ")\n";
    modifyTypeName(ioTypeName);
    Attribute   aAttribute;
    aAttribute.mStructure = iStructure;
    char const* ret=loadTag(TagKind::Start, ioTypeName, &aAttribute);
    if (ret)
    {
        ioTypeName = ret;
    }
    if (oObjectId)
    {
        *oObjectId = aAttribute.mObjectId;
    }
}

//      ---<<< 終了処理 >>>---

void XmlMidISerializer::loadStructureEnd(Structure iStructure, std::string const& iTypeName)
{
std::cout << "loadStructureEnd(" << iTypeName << ")\n";
    loadTag(TagKind::End, iTypeName);
}

//      ---<<< 型名取得 >>>---

void XmlMidISerializer::loadTypeName(std::string& oTypeName)
{
    if (!mTagInfo.mValid)
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
    }

    oTypeName = mTagInfo.mTagName;
}

//----------------------------------------------------------------------------
//      ,まで読み飛ばし
//----------------------------------------------------------------------------

bool XmlMidISerializer::readComma(bool iReadComma)
{
    char in = find_not_of(" \t\n");
    if (in == ',')
    {
        if (iReadComma)
        {
return true;
        }
        else
        {
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
        }
    }

    // 終端マークなら、false返却
    if (checkTerminal(in))
return false;

    // 読みだした文字は要素の先頭なので、戻しておく
    mIStream.unget();

    return true;
}

//----------------------------------------------------------------------------
//      終了マーク確認
//----------------------------------------------------------------------------

bool XmlMidISerializer::checkTerminal(char iIn)
{
    switch (mElementsMapping)
    {
    case emName:
        if (iIn != '}')
return false;
        break;

    case emOrder:
        if (iIn != ']')
return false;
        break;
    }
    mTerminated=true;

    return true;
}

//----------------------------------------------------------------------------
//      JSON文字列を回復しつつ、デコード
//----------------------------------------------------------------------------

void XmlMidISerializer::decodeXmlString(std::string& iString)
{
//      ---<<< "までスキップ >>>---

    char in = find_not_of(" \t\n");
    if (in != '\"')
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
    }

//      ---<<< "の直前までを追加する >>>---

    iString.clear();
    while(1)
    {
        in=getChar();

        // Escape文字
        if (in == '\\')
        {
            in=getChar();
            switch (in)
            {
            case '\"':              break;
            case '\\':              break;
            case '/':               break;
            case 'b':   in='\x08';  break;
            case 'f':   in='\x0C';  break;
            case 'n':   in='\n';    break;
            case 'r':   in='\r';    break;
            case 't':   in='\t';    break;
            default:
                THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
                break;
            }
        // "(終了文字)
        }
        else if (in == '\"')
        {
    break;
        }
        iString += in;
    }
}

//----------------------------------------------------------------------------
//      1文字読み出し
//----------------------------------------------------------------------------

char XmlMidISerializer::getChar()
{
    char    in;
    mIStream.get(in);
    checkStreamError(mIStream.rdstate());

    return in;
}

//----------------------------------------------------------------------------
//      指定以外の文字まで読み飛ばし
//----------------------------------------------------------------------------

char XmlMidISerializer::find_not_of(std::string const& iSkipChars)
{
    char    in;
    while(1)
    {
        in=getChar();
        // エラーが発生していたら、終了する
        if (ErrorReporter::isError())
return 0;

        std::string::size_type pos = iSkipChars.find(in);
        if (pos == std::string::npos)
    break;
    }

    return in;
}

// ***************************************************************************
//      プリミティブ名からC++名へ変換実体
//          変換できなかったものはそのまま返却する
// ***************************************************************************

#ifdef THEOLIZER_INTERNAL_ENABLE_META_SERIALIZER
char const* getCppNameXml(std::string const& iPrimitiveName, unsigned iSerializerVersionNo)
{
#define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSymbol)                    \
    if (iPrimitiveName ==                                                   \
        PrimitiveName<XmlMidOSerializer, dType>::getPrimitiveName(iSerializerVersionNo))\
return #dType;
#include "primitive.inc"

    if (iPrimitiveName == "String")
return "std::string";

    return iPrimitiveName.c_str();
}
#endif  // THEOLIZER_INTERNAL_ENABLE_META_SERIALIZER

}   // namespace internal
}   // namespace theolizer
