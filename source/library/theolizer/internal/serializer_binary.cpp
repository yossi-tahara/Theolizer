//############################################################################
//      Binaryシリアライザ
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
//          警告抑止
// ***************************************************************************

#if defined(_MSC_VER)
    #pragma warning(disable:4127)
#endif

//############################################################################
//      インクルード
//############################################################################

#include "avoid-trouble.h"

#include <limits>

#include "internal.h"
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
//      シリアライザ名
// ***************************************************************************

char const* const   BinaryMidOSerializer::kSerializerName=
        "theolizer::internal::BinaryMidOSerializer";

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
    mOStream(iOStream)
{
    // エラー情報登録準備
    theolizer::internal::ApiBoundary aApiBoundary(&mAdditionalInfo);

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
    saveControl(kBinarySerializerName);

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
    for (int i=aSize-1; 0 <= i ; --i)
    {
        writeByte(static_cast<uint8_t>(iControl >> (8*i)));
    }
}

//----------------------------------------------------------------------------
//      浮動小数点数のエンディアン変換保存
//----------------------------------------------------------------------------

namespace
{

bool isLittleEndian()
{
  int t=1;
  return (*reinterpret_cast<char*>(&t) == 1);
}

}   // namespace

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

    char const* begin=reinterpret_cast<char const*>(&iFloat);
    char const* end  =begin + aSize;
    if (isLittleEndian())
    {
        for (char const* p=end-1; begin <= p; --p)
        {
            writeByte(*p);
        }
    }
    else
    {
        for (char const* p=begin; p < end; ++p)
        {
            writeByte(*p);
        }
    }
}

template void BinaryMidOSerializer::saveFloat<float>(float iFloat);
template void BinaryMidOSerializer::saveFloat<double>(double iFloat);
template void BinaryMidOSerializer::saveFloat<long double>(long double iFloat);

//----------------------------------------------------------------------------
//      プリミティブ処理
//----------------------------------------------------------------------------

//      ---<<< 整数型 >>>---

#define THEOLIZER_INTERNAL_DEF_INTEGRAL(dType, dSimbol)                     \
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
#define THEOLIZER_INTERNAL_DEF_FLOATING_POINT(dType, dSimbol)               \
    void BinaryMidOSerializer::savePrimitive(dType const& iPrimitive)       \
    {                                                                       \
        saveFloat(iPrimitive);                                              \
    }

//      ---<<< 文字列型 >>>---

#define THEOLIZER_INTERNAL_DEF_NARROW_STRING(dType, dSimbol)                \
    void BinaryMidOSerializer::savePrimitive(dType const& iPrimitive)       \
    {                                                                       \
        saveByteString(iPrimitive);                                         \
    }

#define THEOLIZER_INTERNAL_DEF_WIDE_STRING(dType, dSimbol)                  \
    void BinaryMidOSerializer::savePrimitive(dType const& iPrimitive)       \
    {                                                                       \
        unsigned aDataSize=sizeof(dType::value_type);                       \
        std::size_t size=iPrimitive.size()*aDataSize;                       \
        saveUnsigned(size, BinaryTag::TagCode::ByteString);                 \
        for (auto data : iPrimitive)                                        \
        {                                                                   \
            for (int i=aDataSize-1; 0 <= i ; --i) {                         \
                writeByte(static_cast<uint8_t>(data >> (8*i)));             \
            }                                                               \
        }                                                                   \
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

void BinaryMidOSerializer::saveClassStart(bool iIsTop)
{
    if (!iIsTop || (CheckMode::TypeCheck <= mCheckMode))
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

void BinaryMidOSerializer::saveClassEnd(bool iIsTop)
{
    if (!iIsTop || (CheckMode::TypeCheck <= mCheckMode))
    {
        writeByte(BinaryTag::ClassEnd);
    }
}

//----------------------------------------------------------------------------
//      Byte列保存
//----------------------------------------------------------------------------

void BinaryMidOSerializer::saveByteString(std::string const& iString)
{
    std::size_t size=iString.size();
    saveUnsigned(size, BinaryTag::TagCode::ByteString);
    mOStream.write(iString.data(), size);
    if (!mOStream.good()) {
        THEOLIZER_INTERNAL_IO_ERROR(u8"Write Error(size=%1%).", size);
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
//      シリアライザ名
// ***************************************************************************

char const* const   BinaryMidISerializer::kSerializerName=
        "theolizer::internal::BinaryMidISerializer";

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
    mTerminated(false)
{
    // エラー情報登録準備
    theolizer::internal::ApiBoundary aApiBoundary(&mAdditionalInfo, true);

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
            if (aSerialzierName != kBinarySerializerName)
            {
                THEOLIZER_INTERNAL_DATA_ERROR
                    ("BinaryMidISerializer : シリアライザ名(%1%)が異なります。", aSerialzierName);
            }
        }
        else if (aInfoName == "GlobalVersionNo")
        {
            aExistGlobalVersionNo=true;
            loadControl(mGlobalVersionNo);

            // メタ・データ回復時は型情報は存在しないので生成しない
            if (!mIsMetaMode)
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

bool BinaryMidISerializer::isMatchTypeIndex(size_t iSerializedTypeIndex,
                                          size_t iProgramTypeIndex)
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
     || ((iTagCode == BinaryTag::TagCode::ByteString) && (!mBinaryTag.isByteString())))
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
    }

    unsigned long long ret=0;
    unsigned size=mBinaryTag.getSize();
    for (unsigned i=0; i < size; ++i)
    {
        ret <<= 8;
        ret |= readByte();
    }
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
    if (isLittleEndian())
    {
        for (char* p=end-1; begin <= p; --p)
        {
            *p=readByte();
        }
    }
    else
    {
        for (char* p=begin; p < end; ++p)
        {
            *p=readByte();
        }
    }
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

#define THEOLIZER_INTERNAL_DEF_BOOL(dType, dSimbol)                         \
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

#define THEOLIZER_INTERNAL_DEF_INTEGRAL(dType, dSimbol)                     \
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

#define THEOLIZER_INTERNAL_DEF_FLOATING_POINT(dType, dSimbol)               \
    void BinaryMidISerializer::loadPrimitive(dType& oPrimitive)             \
    {                                                                       \
        loadFloat(oPrimitive);                                              \
    }

//      ---<<< 文字列型 >>>---

#define THEOLIZER_INTERNAL_DEF_NARROW_STRING(dType, dSimbol)                \
    void BinaryMidISerializer::loadPrimitive(dType& oPrimitive)             \
    {                                                                       \
        loadByteString(oPrimitive);                                         \
    }

#define THEOLIZER_INTERNAL_DEF_WIDE_STRING(dType, dSimbol)                  \
    void BinaryMidISerializer::loadPrimitive(dType& oPrimitive)             \
    {                                                                       \
        if (!mBinaryTag.isByteString()) {                                   \
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");               \
        }                                                                   \
        unsigned aDataSize=sizeof(dType::value_type);                       \
        unsigned long long size=loadUnsigned(BinaryTag::TagCode::ByteString);\
        oPrimitive.resize(static_cast<std::size_t>(size/aDataSize));        \
        if (size)                                                           \
        {                                                                   \
            for (auto& data : oPrimitive)                                   \
            {                                                               \
                data=0;                                                     \
                for (unsigned i=0; i < aDataSize; ++i)                      \
                {                                                           \
                    data <<= 8;                                             \
                    data |= readByte();                                     \
                }                                                           \
            }                                                               \
        }                                                                   \
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

    return (aContinue && !ErrorReporter::getError())?Continue:Terminated;
}

//----------------------------------------------------------------------------
//      Element名取出し
//----------------------------------------------------------------------------

std::string BinaryMidISerializer::loadElementName(ElementsMapping iElementsMapping)
{
    std::string aElementName;
    if (iElementsMapping == emName)
    {
        loadByteString(aElementName);

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

    case BinaryTag::ByteString:
        {
            std::string temp;
            loadControl(temp);
        }
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

// ***************************************************************************
//      ユーティリティ
// ***************************************************************************

//----------------------------------------------------------------------------
//      クラス(侵入型／非侵入型)処理
//----------------------------------------------------------------------------

//      ---<<< 開始処理 >>>---

void BinaryMidISerializer::loadClassStart(bool iIsTop)
{
    if (iIsTop)
    {
        if (CheckMode::TypeCheck <= mCheckMode)
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

void BinaryMidISerializer::loadClassEnd(bool iIsTop)
{
    // まだ終了処理されてないなら、終了処理する
    if (!mTerminated)
    {
        while (1)
        {
            if (!iIsTop || (CheckMode::TypeCheck <= mCheckMode))
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
            if (ErrorReporter::getError())
        break;

            disposeElement();
        }
    }

    mTerminated=false;
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
//      Byte列回復
//----------------------------------------------------------------------------

void BinaryMidISerializer::loadByteString(std::string& iString)
{
    if (!mBinaryTag.isByteString()) {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
    }

    std::size_t size=static_cast<std::size_t>(loadUnsigned(BinaryTag::TagCode::ByteString));
    iString.resize(size);
    if (size) {
        mIStream.read(&(*iString.begin()), size);
    }
    if (!mIStream.good()) {
        THEOLIZER_INTERNAL_IO_ERROR(u8"Read Error.");
    }
}

//----------------------------------------------------------------------------
//      1バイト読み出し
//----------------------------------------------------------------------------

uint8_t BinaryMidISerializer::readByte()
{
    char    in;
    mIStream.get(in);
    if ((!mIStream.good()) || (mIStream.gcount() != 1)) {
        if ((mIStream.rdstate() & std::istream::eofbit)) {
            THEOLIZER_INTERNAL_DATA_ERROR(u8"EOF occured.");
        } else {
            THEOLIZER_INTERNAL_IO_ERROR(u8"I/O Error.");
        }
    }

    return static_cast<uint8_t>(in);
}

// ***************************************************************************
//      プリミティブ名からC++名へ変換実体
//          変換できなかったものはそのまま返却する
// ***************************************************************************

char const* getCppNameBinary(std::string const& iPrimitiveName, unsigned iSerializerVersionNo)
{
#define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSymbol)                  \
    if (iPrimitiveName ==                                                   \
        PrimitiveName<BinaryMidOSerializer, dType>::getPrimitiveName(iSerializerVersionNo))\
return #dType;
#include "primitive.inc"

    if (iPrimitiveName == "String")
return "std::string";

    return iPrimitiveName.c_str();
}

}   // namespace internal
}   // namespace theolizer
