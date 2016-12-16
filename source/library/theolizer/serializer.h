//############################################################################
//      TheolizerライブラリのAPI部
//          下記をTheolizer(Theolizer Serializer)のAPI定義
//              標準派生Serializer
//              標準Serializableクラス
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

#if !defined(THEOLIZER_INTERNAL_SERIALIZER_H)
#define THEOLIZER_INTERNAL_SERIALIZER_H

#include "internal/core.h"

//############################################################################
//      Begin
//############################################################################

namespace theolizer
{

//############################################################################
//      トップ・レベルのシリアライズ／デシリアライズ要求
//############################################################################

// ***************************************************************************
//      シリアライズ／デシリアライズ処理
// ***************************************************************************

//----------------------------------------------------------------------------
//      シリアライズ／デシリアライズ共用マクロ
//----------------------------------------------------------------------------

//      ---<<< 通常 >>>---

#define THEOLIZER_PROCESS(dSerializer, dInstance)                           \
    theolizer::internal::process<theolizer::internal::etmDefault,tTheolizerVersion>\
        (dSerializer, dInstance, #dInstance, THEOLIZER_INTERNAL_FILE, __LINE__)

//      ---<<< オブジェクト追跡指定 >>>---
//          非ポインタ(静的定義領域)の追跡を指定する。
//          (なお、Theolizerは参照を静的定義領域として取り扱うので注意。)

#define THEOLIZER_PROCESS_POINTEE(dSerializer, dInstance)                   \
    do {                                                                    \
        typedef typename std::remove_reference<decltype(dInstance)>::type Type;\
        static_assert(!std::is_pointer<Type>::value,                        \
            "THEOLIZER_PROCESS_POINTEE() does not support a pointer.");     \
        theolizer::internal::process<theolizer::internal::etmPointee,tTheolizerVersion>\
            (dSerializer, dInstance, #dInstance, THEOLIZER_INTERNAL_FILE, __LINE__);\
    } while(0)

//      ---<<< オーナー指定 >>>---
//          ポインタの指す領域について、このポインタが所有権を持つことを指定する。
//          デシリアライズ時、nullptrや型が違う場合、
//              ポイントされている領域を開放し、正しい領域を獲得する。

#define THEOLIZER_PROCESS_OWNER(dSerializer, dInstance)                     \
    do {                                                                    \
        typedef typename std::remove_reference<decltype(dInstance)>::type Type;\
        static_assert(std::is_pointer<Type>::value,                         \
            "THEOLIZER_PROCESS_OWNER() support a pointer only.");           \
        theolizer::internal::process<theolizer::internal::etmOwner,tTheolizerVersion>\
            (dSerializer, dInstance, #dInstance, THEOLIZER_INTERNAL_FILE, __LINE__);\
    } while(0)

//############################################################################
//      クラスとenum型のシリアライズ／デシリアライズ定義
//############################################################################

// ***************************************************************************
//      クラスのシリアライズ指定
//          クラス・テンプレートのdClassは<,>問題を避けるため、()で括って渡す。
//          通常クラスの場合はその問題がないので()で括らないで渡す。
// ***************************************************************************

//----------------------------------------------------------------------------
//      侵入型(ノーマル)
//----------------------------------------------------------------------------

//      ---<<<< 対応方法=フィールド名) >>>---

#define THEOLIZER_INTRUSIVE(dAnno, dClass, dLastVersionNo)                  \
    private:                                                                \
    THEOLIZER_INTERNAL_SERIALIZABLE_AUTO(                                   \
        THEOLIZER_ANNOTATE(dAnno:), dClass, dClass, dLastVersionNo,         \
        (), theolizer::internal::emName, false, false)

//      ---<<<< 対応方法=順序) >>>---

#define THEOLIZER_INTRUSIVE_ORDER(dAnno, dClass, dLastVersionNo)            \
    private:                                                                \
    THEOLIZER_INTERNAL_SERIALIZABLE_AUTO(                                   \
        THEOLIZER_ANNOTATE(dAnno:), dClass, dClass, dLastVersionNo,         \
        (), theolizer::internal::emOrder, false, false)

//----------------------------------------------------------------------------
//      侵入型(テンプレート)
//----------------------------------------------------------------------------

//      ---<<<< 対応方法=フィールド名) >>>---

#define THEOLIZER_INTRUSIVE_TEMPLATE(dAnno, dList, dClass, dLastVersionNo, dUniqueClass)\
    private:                                                                \
        THEOLIZER_ANNOTATE(                                                 \
            TS:THEOLIZER_INTERNAL_UNPAREN dList;THEOLIZER_INTERNAL_UNPAREN dClass)\
    THEOLIZER_INTERNAL_SERIALIZABLE_AUTO(                                   \
        THEOLIZER_ANNOTATE(dAnno:), dClass, dClass, dLastVersionNo,         \
        (friend struct dUniqueClass;), theolizer::internal::emName, false, false)

//      ---<<<< 対応方法=順序) >>>---

#define THEOLIZER_INTRUSIVE_TEMPLATE_ORDER(dAnno, dList, dClass, dLastVersionNo, dUniqueClass)\
    private:                                                                \
        THEOLIZER_ANNOTATE(                                                 \
            TS:THEOLIZER_INTERNAL_UNPAREN dList;THEOLIZER_INTERNAL_UNPAREN dClass)\
    THEOLIZER_INTERNAL_SERIALIZABLE_AUTO(                                   \
        THEOLIZER_ANNOTATE(dAnno:), dClass, dClass, dLastVersionNo,         \
        (friend struct dUniqueClass;), theolizer::internal::emOrder, false, false)

//----------------------------------------------------------------------------
//      非侵入型(手動)
//----------------------------------------------------------------------------

//      ---<<<< 対応方法=順序(ノーマル) >>>---

#define THEOLIZER_NON_INTRUSIVE_ORDER(dClass, dLastVersionNo)               \
    template<>                                                              \
    struct TheolizerNonIntrusive<THEOLIZER_INTERNAL_UNPAREN dClass> :       \
                        public THEOLIZER_INTERNAL_UNPAREN dClass            \
    {                                                                       \
    private:                                                                \
        THEOLIZER_INTERNAL_SERIALIZABLE_MANUAL(dClass, dLastVersionNo,      \
            (), theolizer::internal::emOrder, true);                        \
    }

//      ---<<<< 対応方法=順序(テンプレート) >>>---

#define THEOLIZER_NON_INTRUSIVE_TEMPLATE_ORDER(dList, dClass, dLastVersionNo,\
                                              dUniqueClass)                 \
    THEOLIZER_INTERNAL_UNPAREN dList                                        \
    struct TheolizerNonIntrusive<THEOLIZER_INTERNAL_UNPAREN dClass> :       \
                                public THEOLIZER_INTERNAL_UNPAREN dClass    \
    {                                                                       \
    private:                                                                \
        THEOLIZER_ANNOTATE(                                                 \
            TS:THEOLIZER_INTERNAL_UNPAREN dList;THEOLIZER_INTERNAL_UNPAREN dClass)\
        THEOLIZER_INTERNAL_SERIALIZABLE_MANUAL(dClass, dLastVersionNo,      \
            (friend struct dUniqueClass;), theolizer::internal::emOrder, true);\
    }

// ***************************************************************************
//      enum型のシリアライズ指定
// ***************************************************************************

#define THEOLIZER_ENUM(dEnum, dLastVersionNo)                               \
    THEOLIZER_INTERNAL_ENUM(dEnum, dLastVersionNo, theolizer::internal::estName, false)

#define THEOLIZER_ENUM_VALUE(dEnum, dLastVersionNo)                         \
    THEOLIZER_INTERNAL_ENUM(dEnum, dLastVersionNo, theolizer::internal::estValue, false)

// ***************************************************************************
//      ポリモーフィズム派生クラス登録
// ***************************************************************************

#define THEOLIZER_REGISTER_CLASS(dClass)                                    \
    namespace theolizer{namespace internal{                                 \
    template                                                                \
    class RegisterToBaseClass<THEOLIZER_INTERNAL_UNPAREN dClass>;           \
    }}

// ***************************************************************************
//      アノテーション用マクロ
// ***************************************************************************

#ifdef THEOLIZER_INTERNAL_DRIVER
    #define THEOLIZER_ANNOTATE(...)                                         \
        __attribute__((annotate("THEORIDE:" THEOLIZER_INTERNAL_STRINGIZE(__VA_ARGS__))))
#else
    #define THEOLIZER_ANNOTATE(...)
#endif

//############################################################################
//      オプション機能指定
//          保存先
//          グローバル・バージョン番号
//############################################################################

// ***************************************************************************
//      ユーザ用保存先定義マクロ
//          dPrevSymbol : 定義済保存先の最後を指定する(これに続けて新規シンボルを定義する)
//          ...         : 新規シンボルを1個～8個の範囲で,で区切って指定する
//              8個以上定義したい時は、THEOLIZER_DESTINATIONS()を 繰り返し用いる。
//              dPrevSymbolは直前のTHEOLIZER_DESTINATIONS()で定義した
//              最後の新規シンボルを指定する。
// ***************************************************************************

/*!
    @todo   T.B.D.
*/
#define THEOLIZER_DESTINATIONS(dPrevSymbol, ...)                            \
    namespace theolizer{                                                    \
    namespace internal{namespace{namespace bits{                            \
    enum : unsigned {                                                       \
        THEOLIZER_INTERNAL_CAT(dPrevSymbol, Cont)=                          \
            static_cast<unsigned>(theolizerD::dPrevSymbol),                 \
        __VA_ARGS__                                                         \
    };                                                                      \
    }}} /* namespace internal */                                            \
    THEOLIZER_INTERNAL_FOR(THEOLIZER_INTERNAL_DESTINATION, __VA_ARGS__);\
    }   /* namespace theolizer */ 

// ***************************************************************************
//      グローバル・バージョン番号テーブル
// ***************************************************************************

//----------------------------------------------------------------------------
//      グローバル・バージョン・テーブル定義マクロ
//          グローバル名前空間で使用すること
//----------------------------------------------------------------------------

#ifdef THEOLIZER_WRITE_CODE
/*!
    @todo   T.B.D.
*/
#define THEOLIZER_DEFINE_GLOBAL_VERSION_TABLE(dName, dLastGlobalVersionNo)  \
    namespace theolizer{namespace internal{namespace global_table{          \
    class dName final : public internal::GlobalVersionNoTable<dLastGlobalVersionNo>\
    {                                                                       \
        static const unsigned kLastGlobalVersionNo=dLastGlobalVersionNo;    \
        dName();                                                            \
    public:                                                                 \
        static GlobalVersionNoTableBase* getInstance()                      \
        {                                                                   \
            static dName instance;                                          \
            return &instance;                                               \
        }                                                                   \
        char const* getTableName() const {return #dName;}                   \
    };                                                                      \
    }   /* namespace namespace */                                           \
    namespace {                                                             \
    GlobalVersionNoTableBase const*const sGlobalVersionNoTable=             \
        global_table::dName::getInstance();                                 \
    }}  /* namespace internal */                                            \
    namespace {                                                             \
    unsigned const kLastGlobalVersionNo=dLastGlobalVersionNo;               \
    }}  /* namespace theolizer */
#else
#define THEOLIZER_DEFINE_GLOBAL_VERSION_TABLE(dName, dLastGlobalVersionNo)
#endif

//############################################################################
//      End
//############################################################################

}   // namespace theolizer

// ***************************************************************************
//      バックアップ用(全てFastSerializerでコピーする。TODO:効率を上げたい)
//          FastSerialzier
//          copyData
// ***************************************************************************

//----------------------------------------------------------------------------
//      FastSerialzier
//----------------------------------------------------------------------------

#include "internal/serializer_fast.h"

//----------------------------------------------------------------------------
//      各種データ型のコピー
//----------------------------------------------------------------------------

namespace theolizer
{
namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

//  ---<<< コピーの実装 >>>---

// 通常
template<class tClass, THEOLIZER_INTERNAL_OVERLOAD((!IsTheolizerBase<tClass>::value))>
void copyDataImpl(tClass& oDestination, tClass const& iSource)
{
    MemoryStream    aBuff;
    backupSerializable(iSource, aBuff, true);
    aBuff.rewind();
    restoreSerializable(oDestination, aBuff, true);
}

// TheolizerBase<>
template<class tClass, THEOLIZER_INTERNAL_OVERLOAD((IsTheolizerBase<tClass>::value))>
void copyDataImpl(tClass& oDestination, tClass const& iSource)
{
    MemoryStream    aBuff;
    backupSerializable(iSource.get(), aBuff, true);
    aBuff.rewind();
    restoreSerializable(oDestination.get(), aBuff, true);
}

//  ---<<< コピー入り口 >>>---

template<typename tClass>
void copyData(tClass& oDestination, tClass const& iSource)
{
    copyDataImpl(oDestination, iSource);
}

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal
}   // namespace theolizer

#endif  // THEOLIZER_INTERNAL_SERIALIZER_H
