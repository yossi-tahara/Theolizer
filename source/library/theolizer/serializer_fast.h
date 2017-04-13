//############################################################################
//      Fastシリアライザ
//          同じプログラム内での最新版を保存／回復する。
//          最新版のみ対応。要素の追加／削除に対応していない。型チェック無し。
//          主にオブジェクトの保存指定要素のBackup/Restore/Copyに用いる。
//
//      注意事項：
//          異なる型に読ませてもエラーになるとは限らない。
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
    typedef FastMidOSerializer  MetaOSerializer;    // メタ・シリアライザ
    typedef FastMidISerializer  MetaISerializer;    // メタ・デシリアライザ
    static char const* const    kSerializerName;

public:
    //! 保存用なのでtrue
    static const bool       kIsSaver=true;

protected:
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

//      ---<<< プリミティブ保存 >>>---

    #define THEOLIZER_INTERNAL_DEF_SAVE
    #include "internal/primitive.inc"

//      ---<<< Element前処理 >>>---

    void writePreElement(bool iDoProcess=false) { }

//----------------------------------------------------------------------------
//      内部処理
//----------------------------------------------------------------------------

private:

//      ---<<< クラス(配列／侵入型／非侵入型)処理 >>>---

    void saveClassStart(bool iIsTop=false)
    { }
    void saveClassEnd(bool iIsTop=false)
    { }

//      ---<<< プリミティブ名返却 >>>---

    template<typename tType>
    static char const* getPrimitiveName(unsigned iSerializerVersionNo)
    {
        return "";
    }

//      ---<<< Element名保存 >>>---

    void saveElementName(ElementsMapping, const u8string&)
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

class THEOLIZER_INTERNAL_DLL FastMidISerializer : public BaseSerializer
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
    typedef FastMidOSerializer  MetaOSerializer;    // メタ・シリアライザ
    typedef FastMidISerializer  MetaISerializer;    // メタ・デシリアライザ
    static char const* const    kSerializerName;

public:
    //! 回復用なのでfalse
    static const bool       kIsSaver=false;

protected:
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

    void loadControl(int& iControl)                 {loadPrimitive(iControl);}
    void loadControl(long& iControl)                {loadPrimitive(iControl);}
    void loadControl(long long& iControl)           {loadPrimitive(iControl);}
    void loadControl(unsigned& iControl)            {loadPrimitive(iControl);}
    void loadControl(unsigned long& iControl)       {loadPrimitive(iControl);}
    void loadControl(unsigned long long& iControl)  {loadPrimitive(iControl);}
    void loadControl(std::string& iControl)         {loadString(iControl);}

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

//      ---<<< クラス(配列／侵入型／非侵入型)処理 >>>---

    void loadClassStart(bool)
    { }
    void loadClassEnd(bool)
    { }

//      ---<<< プリミティブ名返却 >>>---

    template<typename tType>
    static char const* getPrimitiveName(unsigned iSerializerVersionNo)
    {return "";}

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
class FastOSerializer : public internal::FastMidOSerializer
{
    THEOLIZER_INTERNAL_FRIENDS;

    void AbstructSerializer() { }       // インスタンス生成許可

    static const internal::Destinations     kDestinations;  // クラス外で定義

    // グローバル・バージョン番号テーブルへのポインタへのポインタ
    constexpr static internal::GlobalVersionNoTableBase const*const*const
        kPtrGlobalVersionNoTable=&internal::sGlobalVersionNoTable;

    typedef FastOSerializer                 DestOSerializer;
    typedef internal::FastMidOSerializer    MidSerializer;

public:
#ifndef THEOLIZER_INTERNAL_DOXYGEN
    static const bool                       kHasDestination=true;
#endif  // THEOLIZER_INTERNAL_DOXYGEN

    //! Fastシリアライザなのでtrue
    static const bool                       kIsFastSerialzier=true;

    //! コンストラクタ
    FastOSerializer(std::ostream& iOStream, bool iNoThrowException=false) :
        FastMidOSerializer
        (
            iOStream,
            kDestinations,
            internal::sGlobalVersionNoTable,
            kLastGlobalVersionNo,
            iNoThrowException
        )
    { }
};

#ifndef THEOLIZER_INTERNAL_DOXYGEN
template<Destination uDefault, Destination... uDestinations>
internal::Destinations const FastOSerializer<uDefault, uDestinations...>::
    kDestinations=internal::Destinations{uDefault, uDestinations...};
#endif  // THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//!     回復用FastSerializer
// ***************************************************************************

template<Destination uDefault=theolizerD::All, Destination... uDestinations>
class FastISerializer : public internal::FastMidISerializer
{
    THEOLIZER_INTERNAL_FRIENDS;

    void AbstructSerializer() { }       // インスタンス生成許可

    static const internal::Destinations     kDestinations;  // クラス外で定義

    // グローバル・バージョン番号テーブルへのポインタへのポインタ
    constexpr static internal::GlobalVersionNoTableBase const*const*const
        kPtrGlobalVersionNoTable=&internal::sGlobalVersionNoTable;

    typedef FastISerializer                 DestISerializer;
    typedef internal::FastMidISerializer    MidSerializer;

public:
#ifndef THEOLIZER_INTERNAL_DOXYGEN
    static const bool                       kHasDestination=true;
#endif  // THEOLIZER_INTERNAL_DOXYGEN

    //! Fastシリアライザなのでtrue
    static const bool                       kIsFastSerialzier=true;

    //! コンストラクタ
    FastISerializer(std::istream& iIStream, bool iNoThrowException=false) :
        FastMidISerializer
        (
            iIStream,
            kDestinations,
            internal::sGlobalVersionNoTable,
            kLastGlobalVersionNo,
            iNoThrowException
        )
    { }
};

#ifndef THEOLIZER_INTERNAL_DOXYGEN
template<Destination uDefault, Destination... uDestinations>
internal::Destinations const FastISerializer<uDefault, uDestinations...>::
    kDestinations=internal::Destinations{uDefault, uDestinations...};
#endif  // THEOLIZER_INTERNAL_DOXYGEN

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
