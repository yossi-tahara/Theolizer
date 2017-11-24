//############################################################################
//      Jsonシリアライザ
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

#include "../serializer_json.h"

//############################################################################
//      Begin
//############################################################################

namespace theolizer
{
namespace internal
{

//############################################################################
//      保存
//############################################################################

// ***************************************************************************
//      シリアライザ名
// ***************************************************************************

char const* const   JsonMidOSerializer::kSerializerName=
        "theolizer::internal::JsonMidOSerializer";

// ***************************************************************************
//      fstreamのオープン・モード
// ***************************************************************************

std::ios_base::openmode JsonMidOSerializer::kOpenMode=std::ios_base::openmode();

// ***************************************************************************
//      コンストラクタ
// ***************************************************************************

JsonMidOSerializer::JsonMidOSerializer
(
    std::ostream& iOStream,
    Destinations const& iDestinations,
    GlobalVersionNoTableBase const*const iGlobalVersionNoTable,
    unsigned iGlobalVersionNo,
    unsigned iLastGlobalVersionNo,
    CheckMode iCheckMode,
    bool iNoPrettyPrint,
    bool mNoThrowException
) : BaseSerializer
    (
        std::move(iDestinations),
        iGlobalVersionNoTable,
        iGlobalVersionNo,
        iLastGlobalVersionNo,
        iCheckMode,
        true,
        nullptr,
        mNoThrowException
    ),
    mOStream(iOStream),
    mNoPrettyPrint(iNoPrettyPrint),
    mWriteComma(false),
    mCharIsMultiByte(false)
{
    // エラー情報登録準備
    theolizer::internal::ApiBoundary aApiBoundary(&mAdditionalInfo);

    // 型情報取得中継クラス登録
    TypeFunctions<JsonMidOSerializer>   aTypeFunctions;

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

    mOStream << "\n";

    mWriteComma=false;
}

// ***************************************************************************
//      型情報保存
// ***************************************************************************

//----------------------------------------------------------------------------
//      ヘッダ保存
//----------------------------------------------------------------------------

void JsonMidOSerializer::writeHeader()
{
    AutoRestoreSave aAutoRestoreSave(*this, emName);

    // シリアライザ名出力
    writePreElement();
    saveElementName(emName, "SerialzierName");
    saveControl(kJsonSerializerName);

    // グローバル・バージョン番号出力
    writePreElement();
    saveElementName(emName, "GlobalVersionNo");
    saveControl(mGlobalVersionNo);

    // バージョン番号対応表生成
    createVersionNoTable();

    // 型情報出力
    writePreElement();
    saveElementName(emName, "TypeInfoList");
    BaseSerializer::writeHeaderTypeInfo();
}

// ***************************************************************************
//      内部処理
// ***************************************************************************

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
    void JsonMidOSerializer::savePrimitive(dType const& iPrimitive)         \
    {                                                                       \
        if (std::numeric_limits<dType>::is_signed) {                        \
            mOStream << static_cast<long long>(iPrimitive);                 \
        } else {                                                            \
            mOStream << static_cast<unsigned long long>(iPrimitive);        \
        }                                                                   \
        checkStreamError(mOStream.rdstate());                               \
    }

//      ---<<< 浮動小数点型 >>>---

#define THEOLIZER_INTERNAL_DEF_FLOATING_POINT(dType, dSimbol)               \
    void JsonMidOSerializer::savePrimitive(dType const& iPrimitive)         \
    {                                                                       \
        std::streamsize precision=mOStream.precision();                     \
        mOStream.precision(Decimal<dType>::kDigit);                         \
        mOStream << iPrimitive;                                             \
        mOStream.precision(precision);                                      \
        checkStreamError(mOStream.rdstate());                               \
    }

//      ---<<< 文字列型 >>>---

#define THEOLIZER_INTERNAL_DEF_NARROW_STRING(dType, dSimbol)                \
    void JsonMidOSerializer::savePrimitive(dType const& iPrimitive)         \
    {                                                                       \
        if (mCharIsMultiByte)                                               \
        {                                                                   \
            u8string temp(iPrimitive, MultiByte());                         \
            encodeJsonString(std::move(temp.str()));                        \
        }                                                                   \
        else                                                                \
        {                                                                   \
            encodeJsonString(iPrimitive);                                   \
        }                                                                   \
    }

#define THEOLIZER_INTERNAL_DEF_WIDE_STRING(dType, dSimbol)                  \
    void JsonMidOSerializer::savePrimitive(dType const& iPrimitive)         \
    {                                                                       \
        u8string temp(iPrimitive);                                          \
        encodeJsonString(temp.str());                                       \
    }

//      ---<<< 実体定義 >>>---

#include "primitive.inc"

//----------------------------------------------------------------------------
//      Element前処理
//----------------------------------------------------------------------------

void JsonMidOSerializer::writePreElement(bool iDoProcess)
{
    writeCommaIndent(mWriteComma);
}

// ***************************************************************************
//      ユーティリティ
// ***************************************************************************

//----------------------------------------------------------------------------
//      クラス(侵入型／非侵入型)処理
//----------------------------------------------------------------------------

//      ---<<< 開始処理 >>>---

void JsonMidOSerializer::saveGroupStart(bool iIsTop)
{
    mWriteComma=false;
    if (!iIsTop || (CheckMode::TypeCheck <= mCheckMode))
    {
        if (!mCancelPrettyPrint) mIndent++;
        switch (mElementsMapping)
        {
        case emName:
            mOStream << "{";
            break;

        case emOrder:
            mOStream << "[";
            break;
        }
    }
}

//      ---<<< 終了処理 >>>---

void JsonMidOSerializer::saveGroupEnd(bool iIsTop)
{
    mWriteComma=false;
    writeCommaIndent();
    if (!iIsTop || (CheckMode::TypeCheck <= mCheckMode))
    {
        switch (mElementsMapping)
        {
        case emName:
            mOStream << "}";
            break;

        case emOrder:
            mOStream << "]";
            break;
        }
    }
    if ((iIsTop) && (!mIndent))
        mOStream << "\n";
}

//----------------------------------------------------------------------------
//      各種構造処理
//----------------------------------------------------------------------------

//      ---<<< 開始処理 >>>---

void JsonMidOSerializer::saveStructureStart
(
    Structure,
    std::string&,
    std::size_t     iObjectId
)
{
    saveGroupStart();
    if (iObjectId != kInvalidSize)
    {
        writePreElement();
        saveControl(iObjectId);
    }
}

//      ---<<< 終了処理 >>>---

void JsonMidOSerializer::saveStructureEnd(Structure, std::string const&)
{
    saveGroupEnd();
}

//----------------------------------------------------------------------------
//      整形処理
//----------------------------------------------------------------------------

void JsonMidOSerializer::writeCommaIndent(bool iWriteComma)
{
    if (iWriteComma)
    {
        mOStream << ",";
    }

    if (!mNoPrettyPrint && !mCancelPrettyPrint)
    {
        mOStream << "\n";
        for (int i=0; i < mIndent; ++i)
            mOStream << "    ";
    }

    mWriteComma=true;
}

//----------------------------------------------------------------------------
//      JSON文字列へエンコードして保存
//----------------------------------------------------------------------------

void JsonMidOSerializer::encodeJsonString(std::string const& iString)
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

char const* const   JsonMidISerializer::kSerializerName=
        "theolizer::internal::JsonMidISerializer";

// ***************************************************************************
//      fstreamのオープン・モード
// ***************************************************************************

std::ios_base::openmode JsonMidISerializer::kOpenMode=std::ios_base::openmode();

// ***************************************************************************
//      コンストラクタ
// ***************************************************************************

JsonMidISerializer::JsonMidISerializer
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
        CheckMode::NoTypeCheck,
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
    TypeFunctions<JsonMidISerializer>   aTypeFunctions;

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
//      ヘッダ回復と型チェック
// ***************************************************************************

//----------------------------------------------------------------------------
//      ヘッダ情報回復
//----------------------------------------------------------------------------

void JsonMidISerializer::readHeader()
{
    AutoRestoreLoad aAutoRestoreLoad(*this, emName);

//      ---<<< 名前に従って回復 >>>---

    bool aExistSerializerName=false;
    bool aExistGlobalVersionNo=false;
    bool aExistTypeInfo=false;

    while(readPreElement())
    {
        std::string aInfoName=loadElementName(emName);

        if (aInfoName == "SerialzierName")
        {
            aExistSerializerName=true;
            std::string aSerialzierName;
            loadControl(aSerialzierName);
            if (aSerialzierName != kJsonSerializerName)
            {
                THEOLIZER_INTERNAL_DATA_ERROR
                    ("JsonMidISerializer : Unmatch serializer name(%1%).", aSerialzierName);
            }
        }
        else if (aInfoName == "GlobalVersionNo")
        {
            aExistGlobalVersionNo=true;
            loadControl(mGlobalVersionNo);

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
        else if (aInfoName == "TypeInfoList")
        {
            aExistTypeInfo=true;
            BaseSerializer::readHeaderTypeInfo();
//std::cout << "mSerializerVersionNo=" << mSerializerVersionNo << "\n";
        }
        else
        {
            disposeElement();
        }
    }

//      ---<<< 情報無しチェック >>>---

    if (!aExistSerializerName)
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"JsonMidISerializer : No Serializer name.");
    }

    if (!aExistGlobalVersionNo)
    {
        THEOLIZER_INTERNAL_DATA_ERROR
            ("JsonMidISerializer : No global version number.");
    }

    if (!aExistTypeInfo)
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"JsonMidISerializer : No types infomation.");
    }
}

//----------------------------------------------------------------------------
//      TypeIndex一致判定
//----------------------------------------------------------------------------

bool JsonMidISerializer::isMatchTypeIndex(size_t iSerializedTypeIndex,
                                          size_t iProgramTypeIndex)
{
    return BaseSerializer::isMatchTypeIndex(iSerializedTypeIndex, iProgramTypeIndex);
}

// ***************************************************************************
//      データ回復
// ***************************************************************************

//----------------------------------------------------------------------------
//      バッファ内の無効データを破棄する
//----------------------------------------------------------------------------

void JsonMidISerializer::flush()
{
    static const std::string    sSkipChars(" \t\n");

#if 1
char ch;
while (auto num=mIStream.readsome(&ch, 1))
{
    if (sSkipChars.find(ch) == std::string::npos)
    {
        mIStream.unget();
break;
    }
}
#else
    while(mIStream.gcount() && (sSkipChars.find(mIStream.peek()) != std::string::npos))
    {
        getChar();
    }
#endif
}

//----------------------------------------------------------------------------
//      プリミティブ処理
//----------------------------------------------------------------------------

//      ---<<< bool型 >>>---

#define THEOLIZER_INTERNAL_DEF_BOOL(dType, dSimbol)                         \
    void JsonMidISerializer::loadPrimitive(bool& oPrimitive)                \
    {                                                                       \
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
    void JsonMidISerializer::loadPrimitive(dType& oPrimitive)               \
    {                                                                       \
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
    void JsonMidISerializer::loadPrimitive(dType& oPrimitive)               \
    {                                                                       \
        mIStream >> oPrimitive;                                             \
        checkStreamError(mIStream.rdstate());                               \
    }

//      ---<<< 文字列型 >>>---

#define THEOLIZER_INTERNAL_DEF_NARROW_STRING(dType, dSimbol)                \
    void JsonMidISerializer::loadPrimitive(dType& oPrimitive)               \
    {                                                                       \
        if (mCharIsMultiByte)                                               \
        {                                                                   \
            u8string temp;                                                  \
            decodeJsonString(temp.str());                                   \
            oPrimitive=std::move(temp.getMultiByte());                      \
        }                                                                   \
        else                                                                \
        {                                                                   \
            decodeJsonString(oPrimitive);                                   \
        }                                                                   \
    }

#define THEOLIZER_INTERNAL_DEF_WIDE_STRING(dType, dSimbol)                  \
    void JsonMidISerializer::loadPrimitive(dType& oPrimitive)               \
    {                                                                       \
        u8string temp;                                                      \
        decodeJsonString(temp.str());                                       \
        oPrimitive=temp;                                                    \
    }

//      ---<<< 実体定義 >>>---

#include "primitive.inc"

//----------------------------------------------------------------------------
//      Element前処理
//----------------------------------------------------------------------------

ReadStat JsonMidISerializer::readPreElement(bool iDoProcess)
{
    bool aContinue=readComma(mReadComma);
    mReadComma=true;

    // isError()はErrorReporterを使う。(シリアライザへ伝達されていない場合があるため)
    return (aContinue && !ErrorReporter::isError())?Continue:Terminated;
}

//----------------------------------------------------------------------------
//      Element名取出し
//----------------------------------------------------------------------------

std::string JsonMidISerializer::loadElementName(ElementsMapping iElementsMapping)
{
    std::string aElementName;
    if (iElementsMapping == emName)
    {
        decodeJsonString(aElementName);
        char in = find_not_of(" \t\n");
        if (in != ':')
        {
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
        }
    }

    return aElementName;
}

// ***************************************************************************
//      要素破棄処理
// ***************************************************************************

void JsonMidISerializer::disposeElement()
{
    char in = find_not_of(" \t\n");

    // 次の処理に備えて、最後の文字を戻しておく
    mIStream.unget();

    switch(in)
    {
    case '\"':
        {
            std::string temp;
            decodeJsonString(temp);
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
//      グループ処理
//----------------------------------------------------------------------------

//      ---<<< 開始処理 >>>---

void JsonMidISerializer::loadGroupStart(bool iIsTop)
{
    mReadComma=false;
    if (!iIsTop || (CheckMode::TypeCheck <= mCheckMode))
    {
        char in = find_not_of(" \t\n");
        switch (mElementsMapping)
        {
        case emName:
            if (in != '{')
            {
                THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.(%1%)", in);
            }
            break;

        case emOrder:
            if (in != '[')
            {
                THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.(%1%)", in);
            }
            break;
        }
    }
//  mTerminated=false;  // 呼び出される前は必ずfalseなので設定不要
}

//      ---<<< 終了処理 >>>---

void JsonMidISerializer::loadGroupEnd(bool iIsTop)
{
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
    mTerminated=false;
    mReadComma=true;
}

//----------------------------------------------------------------------------
//      各種構造処理
//----------------------------------------------------------------------------

//      ---<<< 開始処理 >>>---

void JsonMidISerializer::loadStructureStart
(
    Structure,
    std::string&,
    std::size_t*    oObjectId
)
{
    loadGroupStart();
    if (oObjectId)
    {
        if (!readPreElement())
        {
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
        }
        loadControl(*oObjectId);
    }
}

//      ---<<< 終了処理 >>>---

void JsonMidISerializer::loadStructureEnd(Structure, std::string const&)
{
    loadGroupEnd();
}

//----------------------------------------------------------------------------
//      ,まで読み飛ばし
//----------------------------------------------------------------------------

bool JsonMidISerializer::readComma(bool iReadComma)
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

bool JsonMidISerializer::checkTerminal(char iIn)
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

void JsonMidISerializer::decodeJsonString(std::string& iString)
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

char JsonMidISerializer::getChar()
{
    char    in;
    mIStream.get(in);
    checkStreamError(mIStream.rdstate());

    return in;
}

//----------------------------------------------------------------------------
//      指定以外の文字まで読み飛ばし
//----------------------------------------------------------------------------

char JsonMidISerializer::find_not_of(std::string const& iSkipChars)
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
char const* getCppNameJson(std::string const& iPrimitiveName, unsigned iSerializerVersionNo)
{
#define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSymbol)                    \
    if (iPrimitiveName ==                                                   \
        PrimitiveName<JsonMidOSerializer, dType>::getPrimitiveName(iSerializerVersionNo))\
return #dType;
#include "primitive.inc"

    if (iPrimitiveName == "String")
return "std::string";

    return iPrimitiveName.c_str();
}
#endif  // THEOLIZER_INTERNAL_ENABLE_META_SERIALIZER

}   // namespace internal
}   // namespace theolizer
