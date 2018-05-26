//############################################################################
//      Binaryシリアライザ
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
    #pragma warning(disable:4127)
#endif

//############################################################################
//      インクルード
//############################################################################

#include "avoid-trouble.h"
#include "internal.h"

#include <limits>

#include "../serializer_binary.h"

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
//      fstreamのオープン・モード
// ***************************************************************************

std::ios_base::openmode BinaryMidOSerializer::kOpenMode=std::ios_base::binary;

// ***************************************************************************
//      コンストラクタ
// ***************************************************************************

BinaryMidOSerializer::BinaryMidOSerializer
(
    std::ostream& iOStream,
    Destinations const& iDestinations,
    GlobalVersionNoTableBase const*const iGlobalVersionNoTable,
    unsigned iGlobalVersionNo,
    unsigned iLastGlobalVersionNo,
    CheckMode iCheckMode,
    bool mNoThrowException,
    TypeIndex iSerializerVersionTypeIndex
) : BaseSerializer
    (
        std::move(iDestinations),
        iGlobalVersionNoTable,
        iGlobalVersionNo,
        iLastGlobalVersionNo,
        iCheckMode,
        true,
        nullptr,
        mNoThrowException,
        iSerializerVersionTypeIndex
    ),
    mOStream(iOStream)
{
    // エラー情報登録準備
    theolizer::internal::ApiBoundarySerializer aApiBoundary(this, &mAdditionalInfo, true);

    // 型情報取得中継クラス登録
    TypeFunctions<BinaryMidOSerializer> aTypeFunctions;

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

    // ヘッダを追い出す(ピンポン通信時デッドロックさせないため)
    mOStream.flush();
}

// ***************************************************************************
//      型情報保存
// ***************************************************************************

//----------------------------------------------------------------------------
//      ヘッダ保存
//----------------------------------------------------------------------------

void BinaryMidOSerializer::writeHeader()
{
    AutoRestoreSave aAutoRestoreSave(*this, emName);

    // シリアライザ名出力
    writePreElement();
    saveElementName(emName, "SerialzierName");
    saveControl(SerializerVersion::kName);

    // グローバル・バージョン番号出力
    writePreElement();
    saveElementName(emName, "GlobalVersionNo");
    saveControl(mGlobalVersionNo);

    // 型情報出力
    writePreElement();
    saveElementName(emName, "TypeInfoList");
    BaseSerializer::writeHeaderTypeInfo();
}

// ***************************************************************************
//      内部処理
// ***************************************************************************

namespace
{
#if defined(_WIN32)
    typedef wchar_t DefChar;
#else
    typedef char    DefChar;
#endif
}   // namespace

//----------------------------------------------------------------------------
//      コントロール整数保存
//          値に応じて適切なサイズのPrimitiveで保存する
//----------------------------------------------------------------------------

//      ---<<< 符号付き >>>---

void BinaryMidOSerializer::saveSigned(long long iControl)
{
    if (iControl < 0)
    {
        saveUnsigned(static_cast<unsigned long long>(-iControl), BinaryTag::TagCode::MinusValue);
    }
    else
    {
        saveUnsigned(iControl);
    }
}

//      ---<<< 符号無し >>>---

void BinaryMidOSerializer::saveUnsigned(unsigned long long iControl, BinaryTag::TagCode iTagCode)
{
    unsigned aSize=0;
    if (iControl <= std::numeric_limits<uint8_t>::max())
    {
        aSize=1;
    }
    else if (iControl <= std::numeric_limits<uint16_t>::max())
    {
        aSize=2;
    }
    else if (iControl <= std::numeric_limits<uint32_t>::max())
    {
        aSize=4;
    }
    else
    {
        aSize=8;
    }
    writeByte(BinaryTag(iTagCode, aSize));
    // リトル・エンディアンで保存する
#if IS_BIG_ENDIAN == 1  // CPUがビッグ・エンディアンならエンディアン変換する
    for (int i=aSize-1; 0 <= i ; --i)
    {
        writeByte(static_cast<uint8_t>(iControl >> (8*i)));
    }
#else
    mOStream.write(reinterpret_cast<char const*>(&iControl), aSize);
    if (!mOStream.good())
    {
        THEOLIZER_INTERNAL_IO_ERROR(u8"Write Error(iControl=%llu).", iControl);
    }
#endif
}

//----------------------------------------------------------------------------
//      浮動小数点数のエンディアン変換保存
//----------------------------------------------------------------------------

template<typename tType>
void BinaryMidOSerializer::saveFloat(tType iFloat)
{
    unsigned aSize=0;
    if (sizeof(tType) <= 1)
    {
        aSize=1;
    }
    else if (sizeof(tType) <= 2)
    {
        aSize=2;
    }
    else if (sizeof(tType) <= 4)
    {
        aSize=4;
    }
    else if (sizeof(tType) <= 8)
    {
        aSize=8;
    }
    // float80
    else if ((std::numeric_limits<tType>::is_signed == 1)
          && (std::numeric_limits<tType>::radix == 2)
          && (std::numeric_limits<tType>::digits == 64)
          && (std::numeric_limits<tType>::max_exponent == 16384))
    {
        aSize=10;
    }
    else
    {
        THEOLIZER_INTERNAL_ABORT
        (
            "saveFloat() : Unkown format(%s).", THEOLIZER_INTERNAL_TYPE_NAME(tType)
        );
    }
    writeByte(BinaryTag(BinaryTag::Primitive, aSize));

    // リトル・エンディアンで保存する
    char const* begin=reinterpret_cast<char const*>(&iFloat);
#if IS_BIG_ENDIAN == 1  // CPUがビッグ・エンディアンならエンディアン変換する
    char const* end  =begin + aSize;
    for (char const* p=end-1; begin <= p; --p)
    {
        writeByte(*p);
    }
#else
    mOStream.write(begin, aSize);
    if (!mOStream.good())
    {
        THEOLIZER_INTERNAL_IO_ERROR(u8"Write Error(iFloat=%f).", iFloat);
    }
#endif
}

template void BinaryMidOSerializer::saveFloat<float>(float iFloat);
template void BinaryMidOSerializer::saveFloat<double>(double iFloat);
template void BinaryMidOSerializer::saveFloat<long double>(long double iFloat);

//----------------------------------------------------------------------------
//      プリミティブ処理
//----------------------------------------------------------------------------

//      ---<<< 整数型 >>>---

#define THEOLIZER_INTERNAL_DEF_INTEGRAL(dType)                              \
    void BinaryMidOSerializer::savePrimitive(dType const& iPrimitive)       \
    {                                                                       \
        if (std::numeric_limits<dType>::is_signed)                          \
        {                                                                   \
            saveSigned(iPrimitive);                                         \
        }                                                                   \
        else                                                                \
        {                                                                   \
            saveUnsigned(iPrimitive);                                       \
        }                                                                   \
    }

//      ---<<< 浮動小数点型 >>>---

// 実マクロ
#define THEOLIZER_INTERNAL_DEF_FLOATING_POINT(dType)                        \
    void BinaryMidOSerializer::savePrimitive(dType const& iPrimitive)       \
    {                                                                       \
        saveFloat(iPrimitive);                                              \
    }

//      ---<<< 文字列型 >>>---

#define THEOLIZER_INTERNAL_DEF_NARROW_STRING(dType)                         \
    void BinaryMidOSerializer::savePrimitive(dType const& iPrimitive)       \
    {                                                                       \
        if (mCharIsMultiByte)                                               \
        {                                                                   \
            encodeString(Converter<DefChar, MultiByte>::conv(iPrimitive));  \
        }                                                                   \
        else                                                                \
        {                                                                   \
            encodeString(iPrimitive);                                       \
        }                                                                   \
    }

#define THEOLIZER_INTERNAL_DEF_WIDE_STRING(dType)                           \
    void BinaryMidOSerializer::savePrimitive(dType const& iPrimitive)       \
    {                                                                       \
        encodeString(iPrimitive);                                           \
    }

//      ---<<< 実体定義 >>>---

#include "primitive.inc"

// ***************************************************************************
//      ユーティリティ
// ***************************************************************************

//----------------------------------------------------------------------------
//      クラス(侵入型／非侵入型)処理
//----------------------------------------------------------------------------

//      ---<<< 開始処理 >>>---

void BinaryMidOSerializer::saveGroupStart(bool iIsTop)
{
    if (!iIsTop || (CheckMode::TypeCheckByIndex <= mCheckMode))
    {
        switch (mElementsMapping)
        {
        case emName:
            writeByte(BinaryTag::ClassStartName);
            break;

        case emOrder:
            writeByte(BinaryTag::ClassStartOrder);
            break;
        }
    }
}

//      ---<<< 終了処理 >>>---

void BinaryMidOSerializer::saveGroupEnd(bool iIsTop)
{
    if (!iIsTop || (CheckMode::TypeCheckByIndex <= mCheckMode))
    {
        writeByte(BinaryTag::ClassEnd);
    }
}

//----------------------------------------------------------------------------
//      各種構造処理
//----------------------------------------------------------------------------

//      ---<<< 開始処理 >>>---

void BinaryMidOSerializer::saveStructureStart
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

void BinaryMidOSerializer::saveStructureEnd(Structure, std::string const&)
{
    saveGroupEnd();
}

//----------------------------------------------------------------------------
//      char列保存
//----------------------------------------------------------------------------

void BinaryMidOSerializer::saveCharString(std::string const& iString)
{
    std::size_t size=iString.size();
    saveUnsigned(size, BinaryTag::TagCode::String);
    mOStream.write(iString.data(), size);
    if (!mOStream.good())
    {
        THEOLIZER_INTERNAL_IO_ERROR(u8"Write Error(size=%1%).", size);
    }
}

//----------------------------------------------------------------------------
//      文字列をエンコードして保存
//----------------------------------------------------------------------------

template<typename tString>
void BinaryMidOSerializer::encodeString(tString const& iString)
{
    // UTF-16へ変換
    std::u16string  temp = Converter<char16_t, typename tString::value_type>::conv(iString);
    std::size_t size=temp.size()*sizeof(char16_t);

    // Tag
    saveUnsigned(size, BinaryTag::getStringTag<char16_t>());

    // 中身
#if IS_BIG_ENDIAN == 1
    for (auto data : temp)
    {
        writeByte(static_cast<uint8_t>(data >> 8));
        writeByte(static_cast<uint8_t>(data));
    }
#else
    mOStream.write(reinterpret_cast<char const*>(temp.data()), size);
#endif
    if (!mOStream.good())
    {
        THEOLIZER_INTERNAL_IO_ERROR
        (
            u8"Write Error(%s).",
            Converter<char, typename tString::value_type>::conv(iString)
        );
    }
}

//----------------------------------------------------------------------------
//      1バイト書き込み
//----------------------------------------------------------------------------

void BinaryMidOSerializer::writeByte(uint8_t iByte)
{
    mOStream.write(reinterpret_cast<char const*>(&iByte), 1);
    if (!mOStream.good())
    {
        THEOLIZER_INTERNAL_IO_ERROR(u8"Write Error(byte=0x%02x).", iByte);
    }
}

//############################################################################
//      回復
//############################################################################

// ***************************************************************************
//      fstreamのオープン・モード
// ***************************************************************************

std::ios_base::openmode BinaryMidISerializer::kOpenMode=std::ios_base::binary;

// ***************************************************************************
//      コンストラクタ
// ***************************************************************************

BinaryMidISerializer::BinaryMidISerializer
(
    std::istream& iIStream,
    Destinations const& iDestinations,
    GlobalVersionNoTableBase const*const iGlobalVersionNoTable,
    unsigned iLastGlobalVersionNo,
    std::ostream* iOStream,
    bool mNoThrowException,
    TypeIndex iSerializerVersionTypeIndex
) : BaseSerializer
    (
        std::move(iDestinations),
        iGlobalVersionNoTable,
        0,
        iLastGlobalVersionNo,
        CheckMode::NoTypeCheck,
        false,
        iOStream,
        mNoThrowException,
        iSerializerVersionTypeIndex
    ),
    mIStream(iIStream),
    mTerminated(false)
{
    // エラー情報登録準備
    theolizer::internal::ApiBoundarySerializer aApiBoundary(this, &mAdditionalInfo, true);

    // 型情報取得中継クラス登録
    TypeFunctions<BinaryMidISerializer> aTypeFunctions;

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

void BinaryMidISerializer::readHeader()
{
    mBinaryTag = readByte();
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
            if (aSerialzierName != SerializerVersion::kName)
            {
                THEOLIZER_INTERNAL_DATA_ERROR
                    ("BinaryMidISerializer : Unmatch serializer name(%1%).", aSerialzierName);
            }
        }
        else if (aInfoName == "GlobalVersionNo")
        {
            aExistGlobalVersionNo=true;
            loadControl(mGlobalVersionNo);

            // 型名と型／バージョン番号対応表生成
            createTypeNameMap();

//std::cout << "mGlobalVersionNo=" << mGlobalVersionNo << "\n";
        }
        else if (aInfoName == "TypeInfoList")
        {
            aExistTypeInfo=true;
            BaseSerializer::readHeaderTypeInfo();
        }
        else
        {
            disposeElement();
        }
    }

//      ---<<< 情報無しチェック >>>---

    if (!aExistSerializerName)
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"BinaryMidISerializer : No Serializer name.");
    }

    if (!aExistGlobalVersionNo)
    {
        THEOLIZER_INTERNAL_DATA_ERROR
            ("BinaryMidISerializer : No global version number.");
    }

    if (!aExistTypeInfo)
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"BinaryMidISerializer : No types infomation.");
    }
}

//----------------------------------------------------------------------------
//      TypeIndex一致判定
//----------------------------------------------------------------------------

bool BinaryMidISerializer::isMatchTypeIndex(TypeIndex iSerializedTypeIndex,
                                            TypeIndex iProgramTypeIndex)
{
    return BaseSerializer::isMatchTypeIndex(iSerializedTypeIndex, iProgramTypeIndex);
}

// ***************************************************************************
//      データ回復
// ***************************************************************************

//----------------------------------------------------------------------------
//      コントロール整数回復
//          mBinatyTag値に応じて適切なサイズのPrimitiveで保存する
//----------------------------------------------------------------------------

//      ---<<< 符号付き >>>---

long long BinaryMidISerializer::loadSigned()
{
    if (mBinaryTag.isMinusValue())
    {
        return -static_cast<long long>(
            loadUnsigned
            (
                BinaryTag(BinaryTag::TagCode::Primitive, mBinaryTag.getSize())
            )
        );
    }
    else if (mBinaryTag.isPrimitive())
    {
        return loadUnsigned();
    }
    else
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
    }
    return 0;
}

//      ---<<< 符号無し >>>---

unsigned long long BinaryMidISerializer::loadUnsigned(BinaryTag::TagCode iTagCode)
{
    if (((iTagCode == BinaryTag::TagCode::Primitive)  && (!mBinaryTag.isPrimitive()))
     || ((iTagCode == BinaryTag::TagCode::String) && (!mBinaryTag.isString())))
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
    }

    unsigned long long ret=0;
    unsigned size=mBinaryTag.getSize();
#if IS_BIG_ENDIAN == 1  // CPUがビッグ・エンディアンならエンディアン変換する
    for (unsigned i=0; i < size; ++i)
    {
        ret <<= 8;
        ret |= readByte();
    }
#else
    mIStream.read(reinterpret_cast<char*>(&ret), size);
    checkStreamError(mIStream.rdstate());
#endif
    return ret;
}

//----------------------------------------------------------------------------
//      浮動小数点数のエンディアン変換回復
//----------------------------------------------------------------------------

template<typename tType>
void BinaryMidISerializer::loadFloat(tType& oFloat)
{
    if (!mBinaryTag.isPrimitive())
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
    }
    unsigned aDataLen=mBinaryTag.getSize();

    // float80
    if ((std::numeric_limits<tType>::is_signed == 1)
     && (std::numeric_limits<tType>::radix == 2)
     && (std::numeric_limits<tType>::digits == 64)
     && (std::numeric_limits<tType>::max_exponent == 16384))
    {
        THEOLIZER_INTERNAL_ASSERT
        (
            aDataLen == 10,
            "loadFloat() : Unkown format(%s).", THEOLIZER_INTERNAL_TYPE_NAME(tType)
        );
    }

    char* begin=reinterpret_cast<char*>(&oFloat);
    char* end  =begin + aDataLen;
    // リトル・エンディアンで回復する
#if IS_BIG_ENDIAN == 1  // CPUがビッグ・エンディアンならエンディアン変換する
    for (char* p=begin; p < end; ++p)
    {
        *p=readByte();
    }
#else
    mIStream.read(begin, aDataLen);
    checkStreamError(mIStream.rdstate());
#endif
    // パディング
    for (char* p=end; p < (begin+sizeof(tType)); ++p)
    {
        *p=0;
    }
}

template void BinaryMidISerializer::loadFloat<float>(float& oFloat);
template void BinaryMidISerializer::loadFloat<double>(double& oFloat);
template void BinaryMidISerializer::loadFloat<long double>(long double& oFloat);

//----------------------------------------------------------------------------
//      プリミティブ処理
//----------------------------------------------------------------------------

//      ---<<< bool型 >>>---

#define THEOLIZER_INTERNAL_DEF_BOOL(dType)                                  \
    void BinaryMidISerializer::loadPrimitive(dType& oPrimitive)             \
    {                                                                       \
        long long data=loadSigned();                                        \
        long long min = std::numeric_limits<dType>::min();                  \
        long long max = std::numeric_limits<dType>::max();                  \
        if ((data < min) || (max < data)) {                                 \
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Data overflow.");              \
        }                                                                   \
        oPrimitive = (data != 0);                                           \
    }

//      ---<<< 整数型 >>>---

#define THEOLIZER_INTERNAL_DEF_INTEGRAL(dType)                              \
    void BinaryMidISerializer::loadPrimitive(dType& oPrimitive)             \
    {                                                                       \
        if (std::numeric_limits<dType>::is_signed)                          \
        {                                                                   \
            long long data=loadSigned();                                    \
            long long min = std::numeric_limits<dType>::min();              \
            long long max = std::numeric_limits<dType>::max();              \
            if ((data < min) || (max < data)) {                             \
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Data overflow.");              \
            }                                                               \
            oPrimitive = static_cast<dType>(data);                          \
        }                                                                   \
        else                                                                \
        {                                                                   \
            unsigned long long data=loadUnsigned();                         \
            unsigned long long max = std::numeric_limits<dType>::max();     \
            if (max < data) {                                               \
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Data overflow.");              \
            }                                                               \
            oPrimitive = static_cast<dType>(data);                          \
        }                                                                   \
    }

//      ---<<< 浮動小数点型 >>>---

#define THEOLIZER_INTERNAL_DEF_FLOATING_POINT(dType)                        \
    void BinaryMidISerializer::loadPrimitive(dType& oPrimitive)             \
    {                                                                       \
        loadFloat(oPrimitive);                                              \
    }

//      ---<<< 文字列型 >>>---

#define THEOLIZER_INTERNAL_DEF_NARROW_STRING(dType)                         \
    void BinaryMidISerializer::loadPrimitive(dType& oPrimitive)             \
    {                                                                       \
        if (mCharIsMultiByte)                                               \
        {                                                                   \
            u8string temp;                                                  \
            decodeString(temp.str());                                       \
            oPrimitive=std::move(temp.getMultiByte());                      \
        }                                                                   \
        else                                                                \
        {                                                                   \
            decodeString(oPrimitive);                                       \
        }                                                                   \
    }

#define THEOLIZER_INTERNAL_DEF_WIDE_STRING(dType)                           \
    void BinaryMidISerializer::loadPrimitive(dType& oPrimitive)             \
    {                                                                       \
        decodeString(oPrimitive);                                           \
    }

//      ---<<< 実体定義 >>>---

#include "primitive.inc"

//----------------------------------------------------------------------------
//      Element前処理
//----------------------------------------------------------------------------

ReadStat BinaryMidISerializer::readPreElement(bool iDoProcess)
{
    bool aContinue=readNext();

    // 継続する時は再処理のため戻しておく
    if (aContinue && iDoProcess)
    {
        mIStream.unget();
    }

    // isError()はErrorReporterを使う。(シリアライザへ伝達されていない場合があるため)
    return (aContinue && !ErrorReporter::isError())?Continue:Terminated;
}

//----------------------------------------------------------------------------
//      Element名取出し
//----------------------------------------------------------------------------

std::string BinaryMidISerializer::loadElementName(ElementsMapping iElementsMapping)
{
    std::string aElementName;
    if (iElementsMapping == emName)
    {
        loadCharString(aElementName);

        // 次の準備
        mBinaryTag = readByte();
        if (mBinaryTag.isClassEnd())
        {
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
        }
    }

    return aElementName;
}

// ***************************************************************************
//      要素破棄処理
// ***************************************************************************

void BinaryMidISerializer::disposeElement()
{
    switch(mBinaryTag.getKind())
    {
    case BinaryTag::Primitive:
        {
            unsigned long long temp;
            loadControl(temp);
        }
        break;

    case BinaryTag::String:
        disposeString();
        break;

    case BinaryTag::ClassStartName:
        disposeClass(emName);
        break;

    case BinaryTag::ClassStartOrder:
        disposeClass(emOrder);
        break;

    default:
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
        break;
    }
}

void BinaryMidISerializer::disposeString()
{
    // 中身
    auto size=loadUnsigned(BinaryTag::getStringTag<char16_t>());
    THEOLIZER_INTERNAL_ASSERT(size <= std::numeric_limits<std::size_t>::max(),
        u8"BinaryMidISerializer::disposeString : too large value!!");
    if (size)
    {
        unsigned aDataSize=sizeof(char16_t);
        for (std::size_t i=0; i < static_cast<std::size_t>(size)/aDataSize; ++i)
        {
            char16_t    dummy;
            mIStream.read(reinterpret_cast<char*>(&dummy), aDataSize);
        }
    }
}

// ***************************************************************************
//      ユーティリティ
// ***************************************************************************

//----------------------------------------------------------------------------
//      グループ処理
//----------------------------------------------------------------------------

//      ---<<< 開始処理 >>>---

void BinaryMidISerializer::loadGroupStart(bool iIsTop)
{
    if (iIsTop)
    {
        if (CheckMode::TypeCheckByIndex <= mCheckMode)
        {
            mBinaryTag = readByte();
        }
        else
        {
            mBinaryTag = BinaryTag::TagCode::ClassStartOrder;
        }
    }

    switch (mElementsMapping)
    {
    case emName:
        if (!mBinaryTag.isClassStartName())
        {
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
        }
        break;

    case emOrder:
        if (!mBinaryTag.isClassStartOrder())
        {
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
        }
        break;
    }
//  mTerminated=false;  // 呼び出される前は必ずfalseなので設定不要
}

//      ---<<< 終了処理 >>>---

void BinaryMidISerializer::loadGroupEnd(bool iIsTop)
{
    // まだ終了処理されてないなら、終了処理する
    if (!mTerminated)
    {
        while (1)
        {
            if (!iIsTop || (CheckMode::TypeCheckByIndex <= mCheckMode))
            {
                mBinaryTag = readByte();
                if (mBinaryTag.isClassEnd())
        break;
            }
            else
            {
        break;
            }

            // エラーが発生していたら、抜ける
            if (ErrorReporter::isError())
        break;

            disposeElement();
        }
    }

    mTerminated=false;
}

//----------------------------------------------------------------------------
//      各種構造処理
//----------------------------------------------------------------------------

//      ---<<< 開始処理 >>>---

void BinaryMidISerializer::loadStructureStart
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

void BinaryMidISerializer::loadStructureEnd(Structure, std::string const&)
{
    loadGroupEnd();
}

//----------------------------------------------------------------------------
//      次のバイト獲得
//----------------------------------------------------------------------------

bool BinaryMidISerializer::readNext()
{
    mBinaryTag = readByte();
    if (mBinaryTag.isClassEnd())
    {
        mTerminated=true;
return false;
    }

    return true;
}

//----------------------------------------------------------------------------
//      char文字列回復
//----------------------------------------------------------------------------

void BinaryMidISerializer::loadCharString(std::string& iString)
{
    if (!mBinaryTag.isCharString())
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
    }

    auto size = loadUnsigned(BinaryTag::TagCode::String);
    THEOLIZER_INTERNAL_ASSERT(size <= std::numeric_limits<std::size_t>::max(),
        u8"BinaryMidISerializer::loadCharString()() : too large value!!");
    iString.resize(static_cast<std::size_t>(size));
    if (size)
    {
        mIStream.read(&(*iString.begin()), static_cast<std::size_t>(size));
    }
    if (!mIStream.good())
    {
        THEOLIZER_INTERNAL_IO_ERROR(u8"Read Error.");
    }
}

//----------------------------------------------------------------------------
//      回復してデコード
//----------------------------------------------------------------------------

template<typename tString>
void BinaryMidISerializer::decodeString(tString& oString)
{
    // Tag
    if (!mBinaryTag.isString())
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
    }

    // 中身
    auto size=loadUnsigned(BinaryTag::getStringTag<char16_t>());
    THEOLIZER_INTERNAL_ASSERT(size <= std::numeric_limits<std::size_t>::max(),
        u8"BinaryMidISerializer::decodeString : too large value!!");
    std::u16string  temp;
    if (size)
    {
        unsigned aDataSize=sizeof(char16_t);
        temp.resize(static_cast<std::size_t>(size)/aDataSize);
#if IS_BIG_ENDIAN == 1
        for (auto& data : temp)
        {
            data = (static_cast<char16_t>(readByte()) << 8) | readByte();
        }
#else
        mIStream.read(reinterpret_cast<char*>(&(*temp.begin())), static_cast<std::size_t>(size));
#endif
    }
    oString=Converter<typename tString::value_type, char16_t>::conv(temp);
}

//----------------------------------------------------------------------------
//      1バイト読み出し
//----------------------------------------------------------------------------

uint8_t BinaryMidISerializer::readByte()
{
    char    in=0;
    mIStream.get(in);
    checkStreamError(mIStream.rdstate());

    return static_cast<uint8_t>(in);
}

}   // namespace internal
}   // namespace theolizer
