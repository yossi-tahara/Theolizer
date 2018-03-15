//############################################################################
//      Fastシリアライザ
//          同じプログラム内での最新版を保存／回復する。
//          最新版のみ対応。要素の追加／削除に対応していない。型チェック無し。
//          主にオブジェクトの保存指定要素のBackup/Restore/Copyに用いる。
//
//      注意事項：
//          異なる型に読ませてもエラーになるとは限らない。
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

#if !defined(THEOLIZER_INTERNAL_SERIALIZER_FAST_H)
#define THEOLIZER_INTERNAL_SERIALIZER_FAST_H

#include "internal/core.h"

//############################################################################
//      Begin
//############################################################################

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable:4100 4251)
#endif

namespace theolizer
{
namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

//############################################################################
//      共通定義
//############################################################################

// ***************************************************************************
//      Fastシリアライザのバージョン番号
// ***************************************************************************

const static unsigned kFastSerializerVersionNo=1;

// ***************************************************************************
//      プロバティ返却
//          必要な場合直接見えた方が良いのでここで定義
// ***************************************************************************

inline bool hasPropertyFast(Property iProperty, bool iIsSaver)
{
    bool ret=false;
    switch(iProperty)
    {
    case Property::IsSaver:
        ret=iIsSaver;
        break;

    case Property::EncodedString:
        ret=false;
        break;

    case Property::SupportModifying:
        ret=false;
        break;

    case Property::BinaryOpen:
        ret=true;
        break;
    }

    return ret;
}

//############################################################################
//      Fast Serializer実装部
//
//          各文字列型のエンコードは仮定しない。そのまま保存／回復する。
//############################################################################

class FastMidISerializer;

#endif  // THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
/*!
@brief      保存用中間FastSerializer
@details    主な処理はここで行うが、インスタンスを作れないよう抽象クラスとする。
*/
// ***************************************************************************

class THEOLIZER_INTERNAL_DLL FastMidOSerializer : public BaseSerializer
{
private:
    friend class BaseSerializer;
    THEOLIZER_INTERNAL_FRIENDS_FOR_INTERNAL;

//----------------------------------------------------------------------------
//      基本
//----------------------------------------------------------------------------

    std::ostream&           mOStream;

    static const unsigned   kLastVersionNo=kFastSerializerVersionNo;

    typedef FastMidOSerializer  MidSerializer;

protected:
    static char const* const        kSerializerName;
    static std::ios_base::openmode  kOpenMode;

    FastMidOSerializer
    (
        std::ostream& iOStream,
        Destinations const& iDestinations,
        GlobalVersionNoTableBase const*const iGlobalVersionNoTable,
        unsigned iLastGlobalVersionNo,
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
    void saveControl(std::string const& iControl)   {saveString(iControl);}
    void flush()                                    {mOStream.flush();}

//      ---<<< プリミティブ保存 >>>---

    #define THEOLIZER_INTERNAL_DEF_SAVE
    #include "internal/primitive.inc"

//      ---<<< Element前処理 >>>---

    void writePreElement(bool iDoProcess=false) { }

//----------------------------------------------------------------------------
//      内部処理
//----------------------------------------------------------------------------

private:

//      ---<<< グループ処理 >>>---

    void saveGroupStart(bool iIsTop=false)
    { }
    void saveGroupEnd(bool iIsTop=false)
    { }

//      ---<<< 各種構造処理 >>>---

    void saveStructureStart(Structure, std::string&, std::size_t iOjbectId)
    {
        if (iOjbectId != kInvalidSize) saveControl(iOjbectId);
    }
    void saveStructureEnd(Structure, std::string const& )
    { }

//      ---<<< Element名保存 >>>---

    void saveElementName(ElementsMapping,  char const*)
    { }


//      ---<<< 文字列保存 >>>---

    void saveString(std::string const& iString);
};

// ***************************************************************************
/*!
@brief      回復用中間FastSerializer
@details    主な処理はここで行うが、インスタンスを作れないよう抽象クラスとする。
*/
// ***************************************************************************

class THEOLIZER_INTERNAL_DLL FastMidISerializer : protected BaseSerializer
{
private:
    friend class BaseSerializer;
    THEOLIZER_INTERNAL_FRIENDS_FOR_INTERNAL;

//----------------------------------------------------------------------------
//      基本
//----------------------------------------------------------------------------

    std::istream&           mIStream;

    static const unsigned   kLastVersionNo=kFastSerializerVersionNo;

    typedef FastMidISerializer  MidSerializer;

protected:
    static char const* const        kSerializerName;
    static std::ios_base::openmode  kOpenMode;

    FastMidISerializer
    (
        std::istream& iIStream,
        Destinations const& iDestinations,
        GlobalVersionNoTableBase const*const iGlobalVersionNoTable,
        unsigned iLastGlobalVersionNo,
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

    void loadControl(int& oControl)                 {loadPrimitive(oControl);}
    void loadControl(long& oControl)                {loadPrimitive(oControl);}
    void loadControl(long long& oControl)           {loadPrimitive(oControl);}
    void loadControl(unsigned& oControl)            {loadPrimitive(oControl);}
    void loadControl(unsigned long& oControl)       {loadPrimitive(oControl);}
    void loadControl(unsigned long long& oControl)  {loadPrimitive(oControl);}
    void loadControl(std::string& oControl)         {loadString(oControl);}

//      ---<<< プリミティブ回復 >>>---

    #define THEOLIZER_INTERNAL_DEF_LOAD
    #include "internal/primitive.inc"

//      ---<<< Element前処理 >>>---

    ReadStat readPreElement(bool iDoProcess=false) {return DontCare;}

//      ---<<< 要素破棄処理 >>>---

    void disposeElement()
    { }

//----------------------------------------------------------------------------
//      内部処理
//----------------------------------------------------------------------------

private:

//      ---<<< グループ処理 >>>---

    void loadGroupStart(bool)
    { }
    void loadGroupEnd(bool)
    { }

//      ---<<< 各種構造処理 >>>---

    void loadStructureStart(Structure, std::string&, std::size_t* oObjectId)
    {
        if (oObjectId) loadPrimitive(*oObjectId);
    }
    void loadStructureEnd(Structure, std::string const&)
    { }

//      ---<<< Element名回復 >>>---

    std::string loadElementName(ElementsMapping iElementsMapping)
    {return "";}

//      ---<<< 文字列回復 >>>---

    void loadString(std::string& iString);
};

//############################################################################
//      Fast Serializer I/Fクラス
//          保存先をテンプレートで指定する。
//
//      注意事項：
//          FastBaseI/OSerializerから派生してテンプレート化している。
//          FastBaseI/OSerializerを直接保存先のテンプレートとした場合、
//          使用した保存先全てに対して、コードが複製される。
//          それを防ぐことが目的である。
//############################################################################

#ifndef THEOLIZER_INTERNAL_DOXYGEN

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

// ***************************************************************************
//!     保存用FastSerializer
// ***************************************************************************

template<Destination uDefault=theolizerD::All, Destination... uDestinations>
class FastOSerializer : protected internal::FastMidOSerializer
{
    THEOLIZER_INTERNAL_FRIENDS;

    void AbstructSerializer() { }       // インスタンス生成許可

    static internal::Destinations const&    getDestinations()
    {
        static const internal::Destinations destinations{uDefault, uDestinations...};
        return destinations;
    }

    typedef FastOSerializer                 DestOSerializer;
    typedef internal::FastMidOSerializer    MidSerializer;

    // Switcherアクセス用
    using MidSerializer::kSerializerName;
    using BaseSerializer::mIsSaver;

public:
    //! ofstream用std::ios_base::openmode
    using MidSerializer::kOpenMode;

#ifndef THEOLIZER_INTERNAL_DOXYGEN
    static const bool                       kHasDestination=true;
#endif  // THEOLIZER_INTERNAL_DOXYGEN

    //! コンストラクタ
    FastOSerializer(std::ostream& iOStream, bool iNoThrowException=false) :
        FastMidOSerializer
        (
            iOStream,
            getDestinations(),
            internal::sGlobalVersionNoTable,
            kLastGlobalVersionNo,
            iNoThrowException
        )
    { }

    //! FastOSerializerのプロパティ返却（@ref Property 参照）
    static bool hasProperty(Property iProperty)
    {
        return internal::hasPropertyFast(iProperty, true);
    }

    using BaseSerializer::getGlobalVersionNo;
    using BaseSerializer::clearTracking;
    using BaseSerializer::getRequireClearTracking;
    using BaseSerializer::getCheckMode;
    using ErrorBase::getErrorInfo;
    using ErrorBase::isError;
    using ErrorBase::resetError;
};

// ***************************************************************************
//!     回復用FastSerializer
// ***************************************************************************

template<Destination uDefault=theolizerD::All, Destination... uDestinations>
class FastISerializer : protected internal::FastMidISerializer
{
    THEOLIZER_INTERNAL_FRIENDS;

    void AbstructSerializer() { }       // インスタンス生成許可

    static internal::Destinations const&    getDestinations()
    {
        static const internal::Destinations destinations{uDefault, uDestinations...};
        return destinations;
    }

    typedef FastISerializer                 DestISerializer;
    typedef internal::FastMidISerializer    MidSerializer;

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
    FastISerializer(std::istream& iIStream, bool iNoThrowException=false) :
        FastMidISerializer
        (
            iIStream,
            getDestinations(),
            internal::sGlobalVersionNoTable,
            kLastGlobalVersionNo,
            iNoThrowException
        )
    { }

    //! FastISerializerのプロパティ返却（@ref Property 参照）
    static bool hasProperty(Property iProperty)
    {
        return internal::hasPropertyFast(iProperty, false);
    }

    using BaseSerializer::getGlobalVersionNo;
    using BaseSerializer::clearTracking;
    using BaseSerializer::getRequireClearTracking;
    using BaseSerializer::getCheckMode;
    using ErrorBase::getErrorInfo;
    using ErrorBase::isError;
    using ErrorBase::resetError;
};

//############################################################################
//      FastSerializerを用いたユーティリティ
//############################################################################

namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//      バックアップ
// ***************************************************************************

// 通常用
template<class tClass, THEOLIZER_INTERNAL_OVERLOAD((!IsTheolizerVersion<tClass>::value))>
void backupSerializable(tClass const& iInstance, std::ostream& oStream, bool iIsBackup=false)
{
    FastOSerializer<>  aSerializer(oStream);
    {
        THEOLIZER_PROCESS(aSerializer, iInstance);
    }
    aSerializer.clearTracking();
}

// TheolizerVersion用
template<class tClass, THEOLIZER_INTERNAL_OVERLOAD((IsTheolizerVersion<tClass>::value))>
void backupSerializable(tClass const& iInstance, std::ostream& oStream, bool iIsBackup=false)
{
    // const外し
    typedef typename RemoveCV<tClass>::type  Type;

    // 保存
    FastOSerializer<>  aSerializer(oStream);
    {
        BaseSerializer& aBaseSerializer=aSerializer;
        THEOLIZER_INTERNAL_SAVE(aBaseSerializer, const_cast<Type&>(iInstance), etmDefault);
    }
    aSerializer.clearTracking();
}

// ***************************************************************************
//      リストア
// ***************************************************************************

// 通常用
template<class tClass, THEOLIZER_INTERNAL_OVERLOAD((!IsTheolizerVersion<tClass>::value))>
void restoreSerializable(tClass& oInstance, std::istream& iStream, bool iIsBackup=false)
{
    FastISerializer<>  aSerializer(iStream);
    {
        THEOLIZER_PROCESS(aSerializer, oInstance);
    }
    aSerializer.clearTracking();
}

// TheolizerVersion用
template<class tClass, THEOLIZER_INTERNAL_OVERLOAD((IsTheolizerVersion<tClass>::value))>
void restoreSerializable(tClass& oInstance, std::istream& iStream, bool iIsBackup=false)
{
    // 回復
    FastISerializer<>  aSerializer(iStream);
    {
        BaseSerializer& aBaseSerializer=aSerializer;
        THEOLIZER_INTERNAL_LOAD(aBaseSerializer, oInstance, etmDefault);
    }
    aSerializer.clearTracking();
}

// ***************************************************************************
//      メモリ・ストリーム(バックアップ／リストア／コピー用のバッファ)
// ***************************************************************************

class THEOLIZER_INTERNAL_DLL MemoryStream
{
private:
    struct                  Impl;
    std::unique_ptr<Impl>   mImpl;

public:
    MemoryStream() noexcept;
    ~MemoryStream() noexcept;

    std::iostream& get() noexcept;      // iostream返却
    operator std::iostream&() noexcept
    {
        return get();
    }
    void clear();                       // 内容と状態のクリア
    void rewind();                      // 読出ポインタを先頭へ巻き戻す
};

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

// ***************************************************************************
//!     シアライズ対象メンバをコピーする
// ***************************************************************************

template<typename tType>
void copySerializable(tType const& iSource, tType& oDestination)
{
    internal::MemoryStream    aBuff;
    backupSerializable(iSource, aBuff);
    aBuff.rewind();
    restoreSerializable(oDestination, aBuff);
}

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

#endif  // THEOLIZER_INTERNAL_SERIALIZER_FAST_H
