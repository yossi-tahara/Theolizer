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

        String      = 0x30,             // 文字列
            CharString1     = 0x31,     // char列(長さフィールド= 1バイト)
            CharString2     = 0x32,     // char列(長さフィールド= 2バイト)
            CharString4     = 0x33,     // char列(長さフィールド= 4バイト)
            CharString8     = 0x34,     // char列(長さフィールド= 8バイト)
            CharString10    = 0x35,     // char列(長さフィールド=10バイト)

            U16String       = 0x35,     // char16_t文字列
            U16String1      = 0x36,     // char16_t列(長さフィールド= 1バイト)
            U16String2      = 0x37,     // char16_t列(長さフィールド= 2バイト)
            U16String4      = 0x38,     // char16_t列(長さフィールド= 4バイト)
            U16String8      = 0x39,     // char16_t列(長さフィールド= 8バイト)
            U16String10     = 0x3a,     // char16_t列(長さフィールド=10バイト)

            U32String       = 0x3a,     // char32_t文字列
            U32String1      = 0x3b,     // char32_t列(長さフィールド= 1バイト)
            U32String2      = 0x3c,     // char32_t列(長さフィールド= 2バイト)
            U32String4      = 0x3d,     // char32_t列(長さフィールド= 4バイト)
            U32String8      = 0x3e,     // char32_t列(長さフィールド= 8バイト)
            U32String10     = 0x3f,     // char32_t列(長さフィールド=10バイト)

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
        case  0: return static_cast<TagCode>(iTagKind + 0);
        case  1: return static_cast<TagCode>(iTagKind + 1);
        case  2: return static_cast<TagCode>(iTagKind + 2);
        case  4: return static_cast<TagCode>(iTagKind + 3);
        case  8: return static_cast<TagCode>(iTagKind + 4);
        case 10: return static_cast<TagCode>(iTagKind + 5);
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
    bool isString() const {return (mTagCode&KindMask) == String;}
    bool isCharString() const
    {
        return (String <= (mTagCode&KindMask)) && ((mTagCode&KindMask) <= CharString10);
    }
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
        uint8_t aSize = mTagCode & SizeMask;
        if ((mTagCode&KindMask) == String)
        {
            if (U16String < mTagCode)
            {
                aSize = mTagCode-U16String;
            }
            if (U32String < mTagCode)
            {
                aSize = mTagCode-U32String;
            }
        }
        switch(aSize)
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

    template<typename tChar>
    static TagCode getStringTag()
    {
        switch(sizeof(tChar))
        {
        default:
    return String;

        case 2:
    return U16String;

        case 4:
    return U32String;
        }
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

constexpr static char kBinarySerializerName[]="BinaryTheolizer";
constexpr static unsigned kBinarySerializerVersionNo=1;

// ***************************************************************************
//          バージョン管理
// ***************************************************************************

THEOLIZER_INTERNAL_REGISTER_SERIALIZER(BinarySerializerVersion, 1, "BinaryTheolizer");

// ***************************************************************************
//      プロバティ返却
//          必要な場合直接見えた方が良いのでここで定義
// ***************************************************************************

inline bool hasPropertyBinary(Property iProperty, bool iIsSaver)
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
        ret=true;
        break;
    }

    return ret;
}

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

protected:
    static char const* const        kSerializerName;
    static std::ios_base::openmode  kOpenMode;

    // 派生シリアライザのTypeIndex
    TypeIndex getSerializerTypeIndex() const
    {
        return AdditionalTypeInfo<BinarySerializerVersion>::getInstance().getTypeIndex();
    }

    // コンストラクタ
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
    void saveControl(std::string const& iControl)   {saveCharString(iControl);}
    void saveControl(TypeIndex iTypeIndex)          {saveUnsigned(iTypeIndex.getRaw());}
    void flush()                                    {mOStream.flush();}

//      ---<<< プリミティブ保存 >>>---

    template<typename tType>
    void saveFloat(tType iFloat);

    #define THEOLIZER_INTERNAL_DEF_SAVE
    #include "internal/primitive.inc"

//      ---<<< char列保存 >>>---

    void saveCharString(std::string const& iString);

//      ---<<< Element前処理 >>>---

    void writePreElement(bool iDoProcess=false) { }

//----------------------------------------------------------------------------
//      内部処理
//----------------------------------------------------------------------------

private:

//      ---<<< グループ処理 >>>---

    void saveGroupStart(bool iIsTop=false);
    void saveGroupEnd(bool iIsTop=false);

//      ---<<< 各種構造処理 >>>---

    void saveStructureStart(Structure iStructure, std::string& ioTypeName, std::size_t iOjbectId);
    void saveStructureEnd(Structure iStructure, std::string const& iTypeName);

//      ---<<< Element名保存 >>>---
//          名前対応時のみ保存する

    void saveElementName(ElementsMapping iElementsMapping,  char const* iElementName)
    {
        if (iElementsMapping == emName)
        {
            saveCharString(iElementName);
        }
    }

//      ---<<< 文字列保存 >>>---

    template<typename tString>
    void encodeString(tString const& iString);

//      ---<<< 1バイト書き込み >>>---

    void writeByte(uint8_t iByte);
};

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

protected:
    static char const* const        kSerializerName;
    static std::ios_base::openmode  kOpenMode;

    // 派生シリアライザのTypeIndex
    TypeIndex getSerializerTypeIndex() const
    {
        return AdditionalTypeInfo<BinarySerializerVersion>::getInstance().getTypeIndex();
    }

    // コンストラクタ
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

    bool isMatchTypeIndex(TypeIndex iSerializedTypeIndex, TypeIndex iProgramTypeIndex);

//----------------------------------------------------------------------------
//      データ回復
//----------------------------------------------------------------------------

//      ---<<< 制御情報回復 >>>---

    long long loadSigned();
    unsigned long long loadUnsigned(BinaryTag::TagCode iTagCode=BinaryTag::TagCode::Primitive);

    void loadControl(int& oControl)                {oControl=static_cast<int>(loadSigned());}
    void loadControl(long& oControl)               {oControl=static_cast<long>(loadSigned());}
    void loadControl(long long& oControl)          {oControl=loadSigned();}
    void loadControl(unsigned& oControl)           {oControl=static_cast<unsigned>(loadSigned());}
    void loadControl(unsigned long& oControl)  {oControl=static_cast<unsigned long>(loadSigned());}
    void loadControl(unsigned long long& oControl) {oControl=loadSigned();}
    void loadControl(std::string& oControl)        {loadCharString(oControl);}
    void loadControl(TypeIndex& oTypeIndex)    {oTypeIndex=static_cast<unsigned>(loadSigned());}

//      ---<<< プリミティブ回復 >>>---

    template<typename tType>
    void loadFloat(tType& oFloat);

    #define THEOLIZER_INTERNAL_DEF_LOAD
    #include "internal/primitive.inc"

//      ---<<< char列回復 >>>---

    void loadCharString(std::string& iString);

//      ---<<< Element前処理 >>>---
//          戻り値：
//              次がないなら、Terminated(mTerminated=true)
//              次があるなら、Continue  (mTerminated=false)
//              それ以外なら、例外

    ReadStat readPreElement(bool iDoProcess=false);

//      ---<<< 要素破棄処理 >>>---

    void disposeElement();
    void disposeString();

//----------------------------------------------------------------------------
//      内部処理
//----------------------------------------------------------------------------

private:

//      ---<<< グループ処理 >>>---
//          loadGroupEnd()呼び出し以前に、readPreElement()呼び出しにより、
//          mTerminatedをtrueにしておくこと。

    void loadGroupStart(bool iIsTop=false);
    void loadGroupEnd(bool iIsTop=false);

//      ---<<< 各種構造処理 >>>---

    void loadStructureStart(Structure iStructure, std::string& ioTypeName, std::size_t* oObjectId);
    void loadStructureEnd(Structure iStructure, std::string const& iTypeName);

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

//      ---<<< 文字列回復 >>>---

    template<typename tString>
    void decodeString(tString& oString);

//      ---<<< 次のバイト獲得 >>>---
//      クラス終了ならfalse

    bool readNext();

//      ---<<< 1バイト読み出し >>>---

    uint8_t readByte();
};

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
class BinaryOSerializer : protected internal::BinaryMidOSerializer
{
    THEOLIZER_INTERNAL_FRIENDS;

    void AbstructSerializer() { }       // インスタンス生成許可

    static internal::Destinations const&    getDestinations()
    {
        static const internal::Destinations destinations{uDefault, uDestinations...};
        return destinations;
    }

    typedef BinaryOSerializer               DestOSerializer;
    typedef internal::BinaryMidOSerializer  MidSerializer;

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

    //! BinaryOSerializerのプロパティ返却（@ref Property 参照）
    static bool hasProperty(Property iProperty)
    {
        return internal::hasPropertyBinary(iProperty, true);
    }

    using BaseSerializer::getGlobalVersionNo;
    using BaseSerializer::clearTracking;
    using BaseSerializer::getRequireClearTracking;
    using ErrorBase::getErrorInfo;
    using ErrorBase::isError;
    using ErrorBase::resetError;
};

#ifndef THEOLIZER_INTERNAL_DOXYGEN
// MidSerializer登録(トライバ処理用)
THEOLIZER_INTERNAL_REGISTER_MID_SERIALIZER(BinaryOSerializer);
#endif  // THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//!     回復用BinarySerializer
// ***************************************************************************

template<Destination uDefault=theolizerD::All, Destination... uDestinations>
class BinaryISerializer : protected internal::BinaryMidISerializer
{
    THEOLIZER_INTERNAL_FRIENDS;

    void AbstructSerializer() { }       // インスタンス生成許可

    static internal::Destinations const&    getDestinations()
    {
        static const internal::Destinations destinations{uDefault, uDestinations...};
        return destinations;
    }

    typedef BinaryISerializer               DestISerializer;
    typedef internal::BinaryMidISerializer  MidSerializer;

    // Switcherアクセス用
    using MidSerializer::kSerializerName;
    using BaseSerializer::mIsSaver;

public:
    //! ifstream用std::ios_base::openmode
    using MidSerializer::kOpenMode;

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

    //! BinaryISerializerのプロパティ返却（@ref Property 参照）
    static bool hasProperty(Property iProperty)
    {
        return internal::hasPropertyBinary(iProperty, false);
    }

    // 継承メンバー関数群
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
