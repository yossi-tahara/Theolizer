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

#if !defined(THEOLIZER_INTERNAL_SERIALIZER_BINARY_H)
#define THEOLIZER_INTERNAL_SERIALIZER_BINARY_H

#include "serializer.h"

//############################################################################
//      Begin
//############################################################################

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable:4100 4251 4702)
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
//      タグ・コード定義
// ***************************************************************************

class BinaryTag
{
public:
    enum TagCode : uint8_t
    {
        KindMask        = 0xf0,         // 種別マスク
        SizeMask        = 0x0f,         // サイズマスク

        None            = 0x00,         // 未設定
        Primitive       = 0x10,         // プリミティブ
            Primitive1      = 0x11,     //  1バイトのプリミティブ
            Primitive2      = 0x12,     //  2バイトのプリミティブ
            Primitive4      = 0x13,     //  4バイトのプリミティブ
            Primitive8      = 0x14,     //  8バイトのプリミティブ
            Primitive10     = 0x15,     // 10バイトのプリミティブ

        MinusValue      = 0x20,         // 負の整数の絶対値
            MinusValue1     = 0x21,     //  1バイトの負の整数の絶対値
            MinusValue2     = 0x22,     //  2バイトの負の整数の絶対値
            MinusValue4     = 0x23,     //  4バイトの負の整数の絶対値
            MinusValue8     = 0x24,     //  8バイトの負の整数の絶対値
            MinusValue10    = 0x25,     // 10バイトの負の整数の絶対値

        ByteString      = 0x30,         // バイト列
            ByteString1     = 0x31,     // バイト列(長さフィールド= 1バイト)
            ByteString2     = 0x32,     // バイト列(長さフィールド= 2バイト)
            ByteString4     = 0x33,     // バイト列(長さフィールド= 4バイト)
            ByteString8     = 0x34,     // バイト列(長さフィールド= 8バイト)
            ByteString10    = 0x35,     // バイト列(長さフィールド=10バイト)

        ClassEnd        = 0x40,         // クラス終了マーク
        ClassStartName  = 0x50,         // クラス開始マーク(名前対応)
        ClassStartOrder = 0x60          // クラス開始マーク(順序対応)
    };

private:
    TagCode     mTagCode;               // タグ・コード

    TagCode     getByteSize(TagCode iTagKind, unsigned iSize)
    {
        switch(iSize)
        {
        case  0: return static_cast<TagCode>(iTagKind | 0);
        case  1: return static_cast<TagCode>(iTagKind | 1);
        case  2: return static_cast<TagCode>(iTagKind | 2);
        case  4: return static_cast<TagCode>(iTagKind | 3);
        case  8: return static_cast<TagCode>(iTagKind | 4);
        case 10: return static_cast<TagCode>(iTagKind | 5);
        }
        THEOLIZER_INTERNAL_ABORT(u8"Illegal size in BinaryTag::getByteSize().");
    }

public:
    BinaryTag() : mTagCode(None) { }
    BinaryTag(TagCode iTagCode) : mTagCode(iTagCode) { }
    BinaryTag(TagCode iTagKind, unsigned iSize) :
        mTagCode(getByteSize(iTagKind, iSize))
    { }

    BinaryTag(uint8_t iByte) : mTagCode(static_cast<TagCode>(iByte)) { }
    operator TagCode() const {return mTagCode;}
    operator uint8_t() const {return mTagCode;}
    unsigned get()     const {return mTagCode;}
    BinaryTag& operator=(uint8_t iByte)
    {
        mTagCode=static_cast<TagCode>(iByte);
        return *this;
    }

    bool isPrimitive()  const
    {
        return ((mTagCode&KindMask)==Primitive)||(mTagCode&KindMask)==MinusValue;
    }
    bool isMinusValue() const {return (mTagCode&KindMask) == MinusValue;}
    bool isByteString() const {return (mTagCode&KindMask) == ByteString;}
    bool isClassEnd()   const {return (mTagCode == ClassEnd);}
    bool isClassStart() const {return (mTagCode==ClassStartName)||(mTagCode==ClassStartOrder);}
    bool isClassStartName()  const {return (mTagCode==ClassStartName);}
    bool isClassStartOrder() const {return (mTagCode==ClassStartOrder);}
    bool isTagCode(TagCode iTagCode) const {return (mTagCode == iTagCode);}

    TagCode getKind() const
    {
        return static_cast<TagCode>(mTagCode & KindMask);
    }

    unsigned getSize() const
    {
        switch(mTagCode & SizeMask)
        {
        case 0: return 0;
        case 1: return 1;
        case 2: return 2;
        case 3: return 4;
        case 4: return 8;
        case 5: return 10;
        }
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Illegal BinaryTag size(TagCode=0x%02x).", get());

        return 0;
    }
};

// ***************************************************************************
//      Binaryシリアライザのバージョン番号とシリアライザ名
//          注意事項：
//              このバージョンはサイズに「幅」がない。(厳密に一致することが必要)
//              余裕を持たせる場合、下記工夫が必要
//                  シリアライズ時は実型を表現できるシリアライズ型へ
//                  デシリアライズ時はシリアライズ型を表現できる実型へ
//              そこそこ手間が掛かりそうななので、必要が生じたら対応を検討する
//          long doubleはdouble形式で保存する
// ***************************************************************************

const static char kBinarySerializerName[]="BinaryTheolizer";
const static unsigned kBinarySerializerVersionNo=1;

// ***************************************************************************
//      プロバティ返却
//          必要な場合直接見えた方が良いのでここで定義
// ***************************************************************************

inline bool hasPropertyBinary(Property iProperty)
{
    bool ret=false;
    switch(iProperty)
    {
    case Property::EncodedString:
        ret=false;
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

class BinaryMidOSerializer;
class BinaryMidISerializer;

#define THEOLIZER_INTERNAL_INTEGRAL(dSigned, dDigits, dName1)               \
    template<class tMidSerializer, typename tPrimitive>                     \
    struct PrimitiveName                                                    \
    <                                                                       \
        tMidSerializer,                                                     \
        tPrimitive,                                                         \
        EnableIf                                                            \
        <                                                                   \
               (std::is_same<tMidSerializer, BinaryMidOSerializer>::value   \
             || std::is_same<tMidSerializer, BinaryMidISerializer>::value)  \
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
               (std::is_same<tMidSerializer, BinaryMidOSerializer>::value   \
             || std::is_same<tMidSerializer, BinaryMidISerializer>::value)  \
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
               (std::is_same<tMidSerializer, BinaryMidOSerializer>::value   \
             || std::is_same<tMidSerializer, BinaryMidISerializer>::value)  \
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
THEOLIZER_INTERNAL_STRING(2,        "U16string");
THEOLIZER_INTERNAL_STRING(4,        "U32string");

#undef  THEOLIZER_INTERNAL_INTEGRAL
#undef  THEOLIZER_INTERNAL_FLOAT
#undef  THEOLIZER_INTERNAL_STRING

//----------------------------------------------------------------------------
//      プリミティブ名からC++名へ変換
//----------------------------------------------------------------------------

THEOLIZER_INTERNAL_DLL
char const* getCppNameBinary(std::string const& iPrimitiveName, unsigned iSerializerVersionNo);

#endif  // THEOLIZER_INTERNAL_DOXYGEN
//############################################################################
//      Binary Serializer実装部
//          プリミティブの注意事項
//              エンディアンの相違は吸収する(並び順は整数型で決まるものとする)
//              符号付き整数型の負数はタグで負数を示し、絶対値表現
//              浮動小数点型はIEEE754形式
//              整数型のサイズは1, 2, 4, 8Bytes
//              浮動小数点型のサイズは4, 8Bytes
//
//              内部表現も上記の通りと仮定する
//############################################################################

// ***************************************************************************
/*!
@brief      保存用中間BinarySerializer
@details    主な処理はここで行うが、インスタンスを作れないよう抽象クラスとする。
*/
// ***************************************************************************

class THEOLIZER_INTERNAL_DLL BinaryMidOSerializer : public BaseSerializer
{
private:
    friend class BaseSerializer;
    THEOLIZER_INTERNAL_FRIENDS_FOR_INTERNAL;

//----------------------------------------------------------------------------
//      基本
//----------------------------------------------------------------------------

    std::ostream&           mOStream;

    static const unsigned   kLastVersionNo=kBinarySerializerVersionNo;

    typedef BinaryMidOSerializer  MidSerializer;
    typedef BinaryMidOSerializer  MetaOSerializer;   // メタ・シリアライザ
    typedef BinaryMidISerializer  MetaISerializer;   // メタ・デシリアライザ
    static char const* const    kSerializerName;

public:
    //! 保存用なのでtrue
    static const bool       kIsSaver=true;

    //! @todo T.B.D.
    static bool hasPropertyStatic(Property iProperty)
    {
        return hasPropertyBinary(iProperty);
    }

    //! @todo T.B.D.
    bool hasProperty(Property iProperty)
    {
        return hasPropertyStatic(iProperty);
    }

protected:
    BinaryMidOSerializer
    (
        std::ostream& iOStream,
        Destinations const& iDestinations,
        GlobalVersionNoTableBase const*const iGlobalVersionNoTable,
        unsigned iGlobalVersionNo,
        unsigned iLastGlobalVersionNo,
        CheckMode iCheckMode,
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

    void saveSigned(long long iControl);
    void saveUnsigned
    (
        unsigned long long iControl,
        BinaryTag::TagCode iTagCode=BinaryTag::TagCode::Primitive
    );

    void saveControl(int iControl)                  {saveSigned(iControl);}
    void saveControl(long iControl)                 {saveSigned(iControl);}
    void saveControl(long long iControl)            {saveSigned(iControl);}
    void saveControl(unsigned iControl)             {saveUnsigned(iControl);}
    void saveControl(unsigned long iControl)        {saveUnsigned(iControl);}
    void saveControl(unsigned long long iControl)   {saveUnsigned(iControl);}
    void saveControl(std::string const& iControl)   {saveByteString(iControl);}

//      ---<<< プリミティブ保存 >>>---

    template<typename tType>
    void saveFloat(tType iFloat);

    #define THEOLIZER_INTERNAL_DEF_SAVE
    #include "internal/primitive.inc"

//      ---<<< バイト列保存 >>>---

    void saveByteString(std::string const& iString);

//      ---<<< Element前処理 >>>---

    void writePreElement(bool iDoProcess=false) { }

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
        return getCppNameBinary(iPrimitiveName, iSerializerVersionNo);
    }

//      ---<<< Element名保存 >>>---
//          名前対応時のみ保存する

    void saveElementName(ElementsMapping iElementsMapping,
                         u8string const& iElementName)
    {
        if (iElementsMapping == emName)
        {
            saveByteString(iElementName.str());
        }
    }

//      ---<<< 1バイト書き込み >>>---

    void writeByte(uint8_t iByte);
};
#ifndef THEOLIZER_INTERNAL_DOXYGEN

//----------------------------------------------------------------------------
//      プリミティブ名返却
//----------------------------------------------------------------------------

#define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSymbol)                    \
    template<>                                                              \
    inline char const* BinaryMidOSerializer::                               \
        getPrimitiveName<dType>(unsigned iSerializerVersionNo)              \
    {                                                                       \
        return PrimitiveName<BinaryMidOSerializer, dType>::getPrimitiveName(iSerializerVersionNo);\
    }
#include "internal/primitive.inc"

#endif  // THEOLIZER_INTERNAL_DOXYGEN
// ***************************************************************************
/*!
@brief      回復用中間BinarySerializer
@details    主な処理はここで行うが、インスタンスを作れないよう抽象クラスとする。
*/
// ***************************************************************************

class THEOLIZER_INTERNAL_DLL BinaryMidISerializer : public BaseSerializer
{
private:
    friend class BaseSerializer;
    THEOLIZER_INTERNAL_FRIENDS_FOR_INTERNAL;

//----------------------------------------------------------------------------
//      基本
//----------------------------------------------------------------------------

    std::istream&           mIStream;
    bool                    mTerminated;
    BinaryTag               mBinaryTag;

    static const unsigned   kLastVersionNo=kBinarySerializerVersionNo;

    typedef BinaryMidISerializer  MidSerializer;
    typedef BinaryMidOSerializer  MetaOSerializer;   // メタ・シリアライザ
    typedef BinaryMidISerializer  MetaISerializer;   // メタ・デシリアライザ
    static char const* const  kSerializerName;

public:
    //! 回復用なのでfalse
    static const bool       kIsSaver=false;

    //! @todo T.B.D.
    static bool hasPropertyStatic(Property iProperty)
    {
        return hasPropertyBinary(iProperty);
    }

    //! @todo T.B.D.
    bool hasProperty(Property iProperty)
    {
        return hasPropertyStatic(iProperty);
    }

protected:
    BinaryMidISerializer
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

//      ---<<< 制御情報回復 >>>---

    long long loadSigned();
    unsigned long long loadUnsigned(BinaryTag::TagCode iTagCode=BinaryTag::TagCode::Primitive);

    void loadControl(int& iControl)                {iControl=static_cast<int>(loadSigned());}
    void loadControl(long& iControl)               {iControl=static_cast<long>(loadSigned());}
    void loadControl(long long& iControl)          {iControl=loadSigned();}
    void loadControl(unsigned& iControl)           {iControl=static_cast<unsigned>(loadSigned());}
    void loadControl(unsigned long& iControl)  {iControl=static_cast<unsigned long>(loadSigned());}
    void loadControl(unsigned long long& iControl) {iControl=loadSigned();}
    void loadControl(std::string& iControl)        {loadByteString(iControl);}

//      ---<<< プリミティブ回復 >>>---

    template<typename tType>
    void loadFloat(tType& oFloat);

    #define THEOLIZER_INTERNAL_DEF_LOAD
    #include "internal/primitive.inc"

//      ---<<< バイト列回復 >>>---

    void loadByteString(std::string& iString);

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
        return getCppNameBinary(iPrimitiveName, iSerializerVersionNo);
    }

//      ---<<< Element名回復 >>>---
//          ヘッダが有る時はElementsMappingの値に関係なく名前対応可能である。
//              そのような派生Serializerに対応するためのI/Fである。
//              なお、BinaryMid?Serializerはこの機能に対応していない。
//              ヘッダはデバッグ用オプションなので、ヘッダの有無で動作を変えないため。
//          戻り値：
//              名前対応するなら、名前返却
//              順序対応するなら、empty返却
//          注意事項：
//              同じTheolizerVersion<>に対しては名前対応と順序対応を変えてはいけない

    std::string loadElementName(ElementsMapping iElementsMapping);

//      ---<<< 次のバイト獲得 >>>---
//      クラス終了ならfalse

    bool readNext();

//      ---<<< 1バイト読み出し >>>---

    uint8_t readByte();
};
#ifndef THEOLIZER_INTERNAL_DOXYGEN

//----------------------------------------------------------------------------
//      プリミティブ名返却
//----------------------------------------------------------------------------

#define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSymbol)                    \
    template<>                                                              \
    inline char const* BinaryMidISerializer::                               \
        getPrimitiveName<dType>(unsigned iSerializerVersionNo)              \
    {                                                                       \
        return PrimitiveName<BinaryMidISerializer, dType>::getPrimitiveName(iSerializerVersionNo);\
    }
#include "internal/primitive.inc"

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

//############################################################################
//      Binary Serializer I/Fクラス
//          保存先をテンプレートで指定する。
//
//      注意事項：
//          BinaryBaseI/OSerializerから派生してテンプレート化している。
//          BinaryBaseI/OSerializerを直接保存先のテンプレートとした場合、
//          使用した保存先全てに対して、コードが複製される。
//          それを防ぐことが目的である。
//############################################################################

// ***************************************************************************
//!     保存用BinarySerializer
// ***************************************************************************

template<Destination uDefault=theolizerD::All, Destination... uDestinations>
class BinaryOSerializer : public internal::BinaryMidOSerializer
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

    typedef BinaryOSerializer               DestOSerializer;
    typedef internal::BinaryMidOSerializer  MidSerializer;

public:
#ifndef THEOLIZER_INTERNAL_DOXYGEN
    static const bool                       kHasDestination=true;
#endif  // THEOLIZER_INTERNAL_DOXYGEN

    //! 通常のコンストラクタ
    BinaryOSerializer
    (
        std::ostream& iOStream,
        unsigned iGlobalVersionNo=kLastGlobalVersionNo,
        CheckMode iCheckMode=CheckMode::NoTypeCheck,
        bool iNoThrowException=false
    ) : BinaryMidOSerializer
        (
            iOStream,
            getDestinations(),
            internal::sGlobalVersionNoTable,
            iGlobalVersionNo,
            kLastGlobalVersionNo,
            iCheckMode,
            iNoThrowException
        )
    { }

    //! GlobalVersionNo以外のオプションを指定するコンストラクタ
    BinaryOSerializer
    (
        std::ostream& iOStream,
        CheckMode iCheckMode,
        bool iNoThrowException=false
    ) : BinaryMidOSerializer
        (
            iOStream,
            getDestinations(),
            internal::sGlobalVersionNoTable,
            kLastGlobalVersionNo,
            kLastGlobalVersionNo,
            iCheckMode,
            iNoThrowException
        )
    { }
};

#ifndef THEOLIZER_INTERNAL_DOXYGEN
// MidSerializer登録(トライバ処理用)
THEOLIZER_INTERNAL_REGISTER_MID_SERIALIZER(BinaryOSerializer);
#endif  // THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//!     回復用BinarySerializer
// ***************************************************************************

template<Destination uDefault=theolizerD::All, Destination... uDestinations>
class BinaryISerializer : public internal::BinaryMidISerializer
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

    typedef BinaryISerializer               DestISerializer;
    typedef internal::BinaryMidISerializer  MidSerializer;

public:
#ifndef THEOLIZER_INTERNAL_DOXYGEN
    static const bool                       kHasDestination=true;
#endif  // THEOLIZER_INTERNAL_DOXYGEN

    //! 通常のコンストラクタ
    BinaryISerializer
    (
        std::istream& iIStream,
        bool iNoThrowException=false
    ) : BinaryMidISerializer
        (
            iIStream,
            getDestinations(),
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
    BinaryISerializer
    (
        std::istream& iIStream,
        std::ostream& iOStream,
        bool iNoThrowException=false
    ) : BinaryMidISerializer
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
THEOLIZER_INTERNAL_REGISTER_MID_SERIALIZER(BinaryISerializer);
#endif  // THEOLIZER_INTERNAL_DOXYGEN

//############################################################################
//      End
//############################################################################

}   // namespace theolizer

// ***************************************************************************
//          警告抑止解除
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#endif  // THEOLIZER_INTERNAL_SERIALIZER_BINARY_H
