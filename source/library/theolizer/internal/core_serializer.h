//############################################################################
//      Theolizerライブラリのコア部(基底シリアライザ)
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

#if !defined(THEOLIZER_INTERNAL_CORE_SERIALIZER_H)
#define THEOLIZER_INTERNAL_CORE_SERIALIZER_H

#include "core_type_info.h"
#include "version_array.h"

//############################################################################
//      Begin
//############################################################################

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable:4100 4127 4251)
#endif

namespace theolizer
{
namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

//############################################################################
//      シリアライズ用内部処理マクロ
//############################################################################

// ***************************************************************************
//      非侵入型(完全自動)のノーマル >>>---
//          対応方法=フィールド名
// ***************************************************************************

#define THEOLIZER_INTERNAL_NON_INTRUSIVE(dClass)                            \
    template<>                                                              \
    struct TheolizerNonIntrusive<dClass> : public dClass                    \
    {                                                                       \
    private:                                                                \
        THEOLIZER_INTERNAL_SERIALIZABLE_AUTO(,                              \
            (TheolizerNonIntrusive<dClass>), (dClass), 1,                   \
            (), theolizer::internal::emName, true, true);                   \
    }

//      ---<<<< 非侵入型(完全自動)のテンプレート >>>---
//          対応方法=フィールド名

#define THEOLIZER_INTERNAL_NON_INTRUSIVE_TEMPLATE(dList, dClass, dUniqueClass)\
    THEOLIZER_INTERNAL_UNPAREN dList                                        \
    struct TheolizerNonIntrusive<THEOLIZER_INTERNAL_UNPAREN dClass> :       \
        public THEOLIZER_INTERNAL_UNPAREN dClass                            \
    {                                                                       \
    private:                                                                \
        THEOLIZER_ANNOTATE(                                                 \
            TS:THEOLIZER_INTERNAL_UNPAREN dList;THEOLIZER_INTERNAL_UNPAREN dClass)\
        THEOLIZER_INTERNAL_SERIALIZABLE_AUTO(,                              \
            (TheolizerNonIntrusive<THEOLIZER_INTERNAL_UNPAREN dClass>), dClass, 1,\
            (friend struct dUniqueClass;), theolizer::internal::emName, true, true);\
    }

//----------------------------------------------------------------------------
//      サブマクロ
//          クラス・テンプレートにおいて
//          Theilizerライブラリでは、struct DefineUniqueName;にて
//              グローバル・バージョン番号テーブルと自クラスをバインドする
//          Theilizerドライバでは、friend struct dUniqueClass;にて
//              TheolizerVersion<>と自クラスをバインドする
//          自クラスとは、
//              侵入型は、  ターゲット・クラス
//              非侵入型は  TheolizerNonIntrusive<ターゲット・クラス>
//----------------------------------------------------------------------------

#define THEOLIZER_INTERNAL_SERIALIZABLE_AUTO(dAttribute, dClass, dTarget, dLastVersionNo,\
                                                dDefineUnique, dMap, dIsNonIntrusive, dIsFullAuto)\
    typedef THEOLIZER_INTERNAL_UNPAREN dTarget TheolizerTarget;             \
    dAttribute                                                              \
    typedef THEOLIZER_INTERNAL_UNPAREN dClass TheolizerClass;               \
    struct Theolizer                                                        \
    {                                                                       \
        THEOLIZER_INTERNAL_UNPAREN dDefineUnique                            \
        static const bool kIsFullAuto=dIsFullAuto;                          \
        static const unsigned kLastVersionNo=dLastVersionNo;                \
        static const theolizer::internal::ElementsMapping kElementsMapping=dMap;\
        struct DefineUniqueName;                                            \
                                                                            \
        static const bool kIsNonIntrusive=dIsNonIntrusive;                  \
        static const bool kIsAuto=true;                                     \
        static const bool kIsVersion=false;                                 \
                                                                            \
        static std::string getClassName(                                    \
            theolizer::internal::VersionNoList const&, unsigned);           \
        static char const* getUniqueName();                                 \
        static theolizer::internal::ElementsMapping getElementsMapping(unsigned);\
        static theolizer::internal::ElementRange getElementRange(unsigned); \
        static unsigned getTypeFlags(unsigned);                             \
        static void registerToBaseClass(theolizer::internal::BaseTypeInfo*);\
                                                                            \
        static void saveClass(theolizer::internal::BaseSerializer&,         \
            TheolizerClass*&, unsigned);                                    \
        static void loadClass(theolizer::internal::BaseSerializer&,         \
            TheolizerClass*&, unsigned);                                    \
    };                                                                      \
    template<class tTheolizerVersion, class tNextVersion, unsigned tVersionNo>\
    struct TheolizerUserDefine                                              \
    {                                                                       \
        static const unsigned kTheolizerVersionNo=tVersionNo;               \
        static void initialize(tTheolizerVersion&) {}                       \
        static void downVersion(tNextVersion const&, tTheolizerVersion&) { }\
        static void upVersion(tTheolizerVersion const&, tNextVersion&) { }  \
    };                                                                      \
    template<class tBaseSerializer, unsigned tVersionNo>                    \
    struct TheolizerSpecials { };                                           \
    template<class tBaseSerializer, unsigned tVersionNo>                    \
    struct TheolizerBackups { };                                            \
    THEOLIZER_INTERNAL_SERIALIZABLE(theolizer::internal::emOrder)

#define THEOLIZER_INTERNAL_SERIALIZABLE_MANUAL(dClass, dLastVersionNo,      \
                                                  dDefineUnique, dMap, dIsNonIntrusive)\
    typedef THEOLIZER_INTERNAL_UNPAREN dClass TheolizerTarget;              \
    typedef TheolizerNonIntrusive<THEOLIZER_INTERNAL_UNPAREN dClass> TheolizerClass;\
    struct Theolizer                                                        \
    {                                                                       \
        THEOLIZER_INTERNAL_UNPAREN dDefineUnique                            \
        static const bool kIsFullAuto=false;                                \
        static const unsigned kLastVersionNo=dLastVersionNo;                \
        static const theolizer::internal::ElementsMapping kElementsMapping=dMap;\
        struct DefineUniqueName;                                            \
                                                                            \
        static const bool kIsNonIntrusive=dIsNonIntrusive;                  \
        static const bool kIsAuto=false;                                    \
        static const bool kIsVersion=false;                                 \
                                                                            \
        static std::string getClassName(                                    \
            theolizer::internal::VersionNoList const&, unsigned);           \
        static char const* getUniqueName();                                 \
        static theolizer::internal::ElementsMapping getElementsMapping(unsigned);\
        static theolizer::internal::ElementRange getElementRange(unsigned); \
        static unsigned getTypeFlags(unsigned) {return 0;}                  \
        static void registerToBaseClass(theolizer::internal::BaseTypeInfo*);\
                                                                            \
        static void saveClass(theolizer::internal::BaseSerializer&,         \
            TheolizerClass*&, unsigned);                                    \
        static void loadClass(theolizer::internal::BaseSerializer&,         \
            TheolizerClass*&, unsigned);                                    \
    };                                                                      \
    template<class tBaseSerializer, class tTheolizerVersion, unsigned tVersionNo>\
    struct TheolizerUserDefine                                              \
    {                                                                       \
        static const unsigned kTheolizerVersionNo=tVersionNo;               \
        static void saveClassManual                                         \
            (tBaseSerializer&, typename tTheolizerVersion::TheolizerTarget const*const&)\
        {                                                                   \
            static_assert(theolizer::internal::Ignore<tBaseSerializer>::kFalse,\
                          "saveClassManual() is undefined");                \
        }                                                                   \
        static void loadClassManual                                         \
            (tBaseSerializer&, typename tTheolizerVersion::TheolizerClass*&)\
        {                                                                   \
            static_assert(theolizer::internal::Ignore<tBaseSerializer>::kFalse,\
                          "loadClassManual() is undefined");                \
        }                                                                   \
    };                                                                      \
    THEOLIZER_INTERNAL_SERIALIZABLE(theolizer::internal::emOrder)

#define THEOLIZER_INTERNAL_SERIALIZABLE(dMap)                               \
public:                                                                     \
    template<class tBaseSerializer, unsigned tVersionNo>                    \
    struct TheolizerVersion { };                                            \
                                                                            \
private:                                                                    \
    static const bool kIsTheolizer=true;                                    \
    template<class>         friend struct theolizer::internal::IsNonIntrusive;\
    template<class, class>  friend struct theolizer::internal::IsNonIntrusiveImpl;\
    template<class, typename, bool, theolizer::internal::TrackingMode, class>\
        friend struct theolizer::internal::Switcher;                        \
    template<typename, bool> friend struct theolizer::internal::RegisterToBaseClassEntrance;\
    template<class> friend class theolizer::internal::ClassTypeInfo

#define THEOLIZER_INTERNAL_TEMPLATE_PARAMETER(dClass, dName, dParam, dUniqueClass)\
    typedef void NoGenerate; /* ドライバへの自動生成無し指示 */             \
    typedef THEOLIZER_INTERNAL_UNPAREN dClass TheolizerTarget;              \
    typedef TheolizerNonIntrusive<THEOLIZER_INTERNAL_UNPAREN dClass> TheolizerClass;\
    struct Theolizer                                                        \
    {                                                                       \
        friend struct dUniqueClass;                                         \
        static const bool kIsFullAuto=false;                                \
        static const unsigned kLastVersionNo=1;                             \
        static const theolizer::internal::ElementsMapping kElementsMapping= \
            theolizer::internal::emOrder;                                   \
        struct DefineUniqueName {typedef dUniqueClass UniqueClass;};        \
        static const bool kIsNonIntrusive=true;                             \
        static const bool kIsAuto=false;                                    \
        static const bool kIsVersion=false;                                 \
                                                                            \
        static std::string getClassName(                                    \
            theolizer::internal::VersionNoList const& iVersionNoList, unsigned)\
        {return THEOLIZER_INTERNAL_MAKE_TEMPLATE_NAME(                      \
            u8"" #dName,THEOLIZER_INTERNAL_UNPAREN dParam);}                \
        static char const* getUniqueName() {return #dUniqueClass;}          \
        static theolizer::internal::ElementsMapping getElementsMapping(unsigned)\
        {return kElementsMapping;}                                          \
        static theolizer::internal::ElementRange getElementRange(unsigned)  \
        {return theolizer::internal::ElementRange();}                       \
        static unsigned getTypeFlags(unsigned) {return 0;}                  \
                                                                            \
        static void saveClass(theolizer::internal::BaseSerializer&,TheolizerClass*&, unsigned)\
        {THEOLIZER_INTERNAL_ABORT("use THEOLIZER_NON_INTRUSIVE_XXX, if serialize this class.");}\
        static void loadClass(theolizer::internal::BaseSerializer&,TheolizerClass*&, unsigned)\
        {THEOLIZER_INTERNAL_ABORT("use THEOLIZER_NON_INTRUSIVE_XXX, if serialize this class.");}\
    };                                                                      \
    template<class tBaseSerializer, class tTheolizerVersion, unsigned tVersionNo>\
    struct TheolizerUserDefine                                              \
    {                                                                       \
        static const unsigned kTheolizerVersionNo=tVersionNo;               \
        static void saveClassManual                                         \
            (tBaseSerializer&, typename tTheolizerVersion::TheolizerTarget const*const&)\
        { }                                                                 \
        static void loadClassManual                                         \
            (tBaseSerializer&, typename tTheolizerVersion::TheolizerClass*&)\
        { }                                                                 \
    };                                                                      \
private:                                                                    \
    static const bool kIsTheolizer=true;                                    \
    template<class>         friend struct theolizer::internal::IsNonIntrusive;\
    template<class, class>  friend struct theolizer::internal::IsNonIntrusiveImpl;\
    template<class, typename, bool, theolizer::internal::TrackingMode, class>\
        friend struct theolizer::internal::Switcher;                        \
    template<typename, bool> friend struct theolizer::internal::RegisterToBaseClassEntrance;\
    template<class> friend class theolizer::internal::ClassTypeInfo

// ***************************************************************************
//      enum型のシリアライズ指定
// ***************************************************************************

//----------------------------------------------------------------------------
//      内部処理用マクロとstaticクラス
//----------------------------------------------------------------------------

#define THEOLIZER_INTERNAL_ENUM_FULL_AUTO(dEnum)                            \
    THEOLIZER_INTERNAL_ENUM(dEnum, 1, theolizer::internal::estName, true)

#define THEOLIZER_INTERNAL_ENUM(dEnum, dLastVersionNo, dEnumSaveType, dIsFullAuto)\
    template<>                                                              \
    struct TheolizerNonIntrusive<dEnum>                                     \
    {                                                                       \
        typedef dEnum TheolizerTarget;                                      \
        struct Theolizer                                                    \
        {                                                                   \
            static const bool kIsFullAuto=dIsFullAuto;                      \
            static const unsigned kVersionNo=dLastVersionNo;                \
            static const unsigned kLastVersionNo=dLastVersionNo;            \
            static const theolizer::internal::EnumSaveType kEnumSaveType=dEnumSaveType;\
            static const bool kIsNonIntrusive=true;                         \
                                                                            \
            static char const* getEnumName(unsigned iVersionNo);            \
            static unsigned getTypeFlags(unsigned);                         \
            static theolizer::internal::ElementRange getElementRange(unsigned);\
                                                                            \
            static void saveEnum(theolizer::internal::BaseSerializer& iSerializer,\
                                 dEnum& iInstance, unsigned iVersionNo);    \
            static void loadEnum(theolizer::internal::BaseSerializer& iSerializer,\
                                 dEnum& iInstance, unsigned iVersionNo);    \
        };                                                                  \
        typedef TheolizerNonIntrusive<dEnum> TheolizerClass;                \
        template<class tBaseSerializer, unsigned tVersionNo>                \
        struct TheolizerVersion { };                                        \
    }

// ***************************************************************************
//      トップ・レベルのシリアライズ／デシリアライズ処理
// ***************************************************************************

//----------------------------------------------------------------------------
//      補助マクロ
//          下記状況が発生するケースがあるので、その対処
//              MinGWではtypename/templateが必要
//              msvc ではtypename/tempalteががあるとエラー
//----------------------------------------------------------------------------

#if defined(_MSC_VER) && !defined(__clang__)
    #define THEOLIZER_INTERNAL_TYPENAME
    #define THEOLIZER_INTERNAL_TEMPLATE
#else
    #define THEOLIZER_INTERNAL_TYPENAME   typename
    #define THEOLIZER_INTERNAL_TEMPLATE    template
#endif

//----------------------------------------------------------------------------
//      内部処理用マクロ(TheolizerVersion<>内部からの呼び出しのみ)
//          dInstanceは参照なので、remove_referenceする。
//----------------------------------------------------------------------------

//      ---<<< 内部用保存処理(事前処理無し・参照解除・保存エラー無し) >>>---

#define THEOLIZER_INTERNAL_SAVE(dMidSerializer, dInstance, dTrackingMode)   \
    do {                                                                    \
        THEOLIZER_INTERNAL_SETUP(dMidSerializer, dInstance);                \
        theolizer::internal::Switcher<                                      \
            Serializer, Type, true, theolizer::internal::dTrackingMode      \
            >::save(dMidSerializer, dInstance);                             \
    } while(0)

//      ---<<< 内部用回復処理(事前処理無し・参照解除・回復エラー無し) >>>---

#define THEOLIZER_INTERNAL_LOAD(dMidSerializer, dInstance, dTrackingMode)   \
    do {                                                                    \
        THEOLIZER_INTERNAL_SETUP(dMidSerializer, dInstance);                \
        theolizer::internal::Switcher<                                      \
            Serializer, Type, true, theolizer::internal::dTrackingMode      \
            >::load(dMidSerializer, dInstance);                             \
    } while(0)

//      ---<<< 型取出し >>>---

#define THEOLIZER_INTERNAL_SETUP(dSerializer, dInstance)                    \
    typedef THEOLIZER_INTERNAL_TYPENAME                                     \
        std::remove_reference<decltype(dSerializer)>::type Serializer;      \
    typedef decltype(dInstance) TypeTemp;                                   \
    typedef THEOLIZER_INTERNAL_TYPENAME std::remove_reference<TypeTemp>::type Type;\
    typedef THEOLIZER_INTERNAL_TYPENAME std::remove_pointer<Type>::type TypeRemoved;\
    static_assert(!std::is_pointer<TypeRemoved>::value, "Not support Pointer to Pointer.")

//############################################################################
//      エラー報告サービス群
//############################################################################

// ***************************************************************************
//      シリアライズ状況管理
// ***************************************************************************

//----------------------------------------------------------------------------
//      シリアライズ状況クラス
//----------------------------------------------------------------------------

class BaseSerializer;

class THEOLIZER_INTERNAL_DLL AdditionalInfo : public BaseAdditionalInfo
{
    // 付加情報記録領域(ヘッダ依存回避)
    struct                  Indep;
    std::unique_ptr<Indep>  mIndep;

    // setErrorによるエラー報告先(基底Serializer)
    BaseSerializer&     mBaseSerializer;

public:
    // コンストラクタ／デストラクタ
    AdditionalInfo(BaseSerializer& iBaseSerializer) noexcept;
    ~AdditionalInfo() noexcept;

    // インスタンス情報登録／解除
    void pushInstanceInfo
    (
        char const* iName,
        size_t      iIndex=0,
        bool        iIsPointer=false,
        char const* iFileName=nullptr,
        size_t      iLineNo=0
    );
    void popInstanceInfo();

    // メッセージ取り出し
    u8string getString();

    // エラー情報伝達
    void setError(ErrorInfo const& iErrorInfo, bool iConstructor);
};

//############################################################################
//      オブジェクト追跡用クラス
//############################################################################

// ***************************************************************************
//      オブジェクトの追跡状態
// ***************************************************************************

enum TrackingStatus
{
    etsInit,            // 初期状態
    etsNullPtr,         // 全てのnullptrのための追跡状態
    etsPointed,         // 対象領域へのポインタ処理済(データは未処理)
    etsProcessed        // 対象領域のデータの処理(保存／回復)済
};

// ***************************************************************************
//      シリアライズ用オブジェクト情報
// ***************************************************************************

struct SerializeInfo
{
    void*           mAddress;
    std::type_index mStdTypeIndex;
    size_t          mObjectId;
    TrackingStatus  mStatus;

    SerializeInfo() :
        mAddress(nullptr),
        mStdTypeIndex(typeid(nullptr)),
        mObjectId(0),
        mStatus(etsInit)
    { }

    SerializeInfo
    (
        void* iAddress,
        std::type_info const& iTypeInfo,
        size_t iObjectId,
        TrackingStatus iStatus
    ) : mAddress(iAddress),
        mStdTypeIndex(iTypeInfo),
        mObjectId(iObjectId),
        mStatus(iStatus)
    { }
};

//----------------------------------------------------------------------------
//      デシリアライズ用オブジェクト情報
//----------------------------------------------------------------------------

struct DeserializeInfo
{
    void*           mAddress;
    TrackingStatus  mStatus;

    DeserializeInfo() :
        mAddress(nullptr),
        mStatus(etsInit)
    { }
};

// ***************************************************************************
//      shared_ptrデシリアライズ時の共有判定用テーブル
//          std::shared_ptr<>に用いるが、他の形式の共有ポインタにも使用できるよう
//          テーブルの構造は使う側に任せるので、boost::any方式とする。
// ***************************************************************************

struct SharedPtrTable
{

//      ---<<< 記録用内部クラスの基底型 >>>---

    struct HolderBase
    {
        virtual void clear() = 0;
        virtual ~HolderBase() { }
    };

//      ---<<< 記録用内部クラス >>>---

    template<typename tListType>
    struct Holder : public HolderBase
    {
        tListType   mList;

        Holder() { }
        ~Holder() = default;

        virtual void clear()
        {
            mList.clear();
        }
    };

//      ---<<< 記録領域 >>>---

    std::unique_ptr<HolderBase> mHolder;

//      ---<<< コンストラクタ >>>---

    SharedPtrTable() { }

    // 登録をクリアする
    void clear()
    {
        mHolder->clear();
    }
};

//############################################################################
//      Serializer用フレンド・リスト
//############################################################################

// theolizer::internal名前空間のクラスにて使用する
#define THEOLIZER_INTERNAL_FRIENDS_FOR_INTERNAL                                             \
    template<TrackingMode, class, class, typename, class>   friend struct BranchedProcess;  \
    template<TrackingMode, class, class tSerializer, typename tType>                        \
        friend void process(tSerializer&, tType const&, char const*, char const*, size_t);  \
    template<TrackingMode, class, class tSerializer, typename tType>                        \
        friend void process(tSerializer&, tType&,       char const*, char const*, size_t);  \
    template<typename>                                      friend class  ArrayTypeInfo;    \
    template<typename>                                      friend class  ClassTypeInfo;    \
    template<typename>                                      friend class  EnumTypeInfo;     \
    template<typename>                                      friend class  PrimitiveTypeInfo;\
    template<class, typename, bool, TrackingMode, class>    friend struct Switcher;         \
    template<class, typename, class>                        friend struct SavePointer;      \
    template<class, typename, class>                        friend struct LoadPointer;      \
    template<typename...>                                   friend struct ParameterName;    \
    template<class>                             friend struct ::TheolizerNonIntrusive;      \
    template<class>                             friend class TypeFunctions;                 \
    template<class, class, class, bool, bool>   friend class RegisterType;                  \
                                                friend class BaseTypeInfo;                  \
    template<typename>                          friend class Element;                       \
    template<typename>                          friend struct EnumElement;                  \
                                                friend struct MetaDeserializer;             \
                                                friend class AdditionalInfo

// theolizer名前空間のクラスにて使用する
#define THEOLIZER_INTERNAL_FRIENDS                                                          \
    template<internal::TrackingMode, class, class, typename, class>                         \
        friend struct internal::BranchedProcess;                                            \
    template<internal::TrackingMode, class, class tSerializer, typename tType>              \
        friend void process(tSerializer&, tType const&, char const*, char const*, size_t);  \
    template<internal::TrackingMode, class, class tSerializer, typename tType>              \
        friend void process(tSerializer&, tType&, char const*, char const*, size_t);        \
    template<class, class, class, bool, bool>   friend class internal::RegisterType

#endif  // THEOLIZER_INTERNAL_DOXYGEN
//############################################################################
//      基底Serializer
//############################################################################

//! @todo T.B.D.
class THEOLIZER_INTERNAL_DLL BaseSerializer : public ErrorBase
{
// ***************************************************************************
//      基本
// ***************************************************************************

private:
    THEOLIZER_INTERNAL_FRIENDS_FOR_INTERNAL;

    virtual void AbstructSerializer() = 0;      // インスタンス生成禁止

    BaseSerializer*         mSerializerBack;    // 前回処理中のSerializer(退避領域)

protected:
    Destinations const&     mDestinations;
    bool                    mIsSaver;
    bool                    mNoThrowException;

    GlobalVersionNoTableBase const*const    mGlobalVersionNoTable;
    unsigned                                mGlobalVersionNo;
    unsigned                                mLastGlobalVersionNo;
    bool                                    mDuringBackup;
    TypeInfoListImpl&                       mTypeInfoList;

private:
    size_t                                  mTypeIndexCount;
    bool                                    mRequireCheckTracking;

public:
#ifndef THEOLIZER_INTERNAL_DOXYGEN
    // デフォルトは保存先無し
    static const bool                       kHasDestination=false;
#endif  // THEOLIZER_INTERNAL_DOXYGEN

    //! @todo T.B.D.
    static const bool                       kIsFastSerialzier=false;

protected:
    // RegisterTypeコンストラクト時の余分なstruct定義回避
    constexpr static GlobalVersionNoTableBase const*const*const
        kPtrGlobalVersionNoTable=nullptr;
    static Destinations const               kDestinations;

    BaseSerializer
    (
        Destinations const& iDestinations,
        GlobalVersionNoTableBase const*const iGlobalVersionNoTable,
        unsigned iGlobalVersionNo,
        unsigned iLastGlobalVersionNo,
        CheckMode iCheckMode,
        bool iIsSaver,
        std::ostream* iOStream,
        bool mNoThrowException
    );
    virtual ~BaseSerializer() noexcept;

    // コピー禁止
    BaseSerializer(const BaseSerializer&) = delete;
    BaseSerializer& operator=(const BaseSerializer&) = delete;

// ***************************************************************************
//      シリアライズ／デシリアライズ補助API
// ***************************************************************************

protected:
    // 最新版処理
    unsigned getGlobalVersionNo() const {return mGlobalVersionNo;}

    // ClassType終了状態返却(デフォルト)
    virtual bool isTerminated() const {return false;}

// ***************************************************************************
//      エラー処理用
// ***************************************************************************

private:
    using ErrorBase::setError;  // ErrorBaseにAdditionalInfoをfriend指定したくないので

//      ---<<< シリアライズ情報管理 >>>---

protected:
    AdditionalInfo          mAdditionalInfo;

//      ---<<< 変数パスのpush/自動pop用 >>>---

    struct AutoRestoreSerializeInfo
    {
        AdditionalInfo& mAdditionalInfo;

        AutoRestoreSerializeInfo
        (
            BaseSerializer&     iSerializer,
            char const*         iName,
            size_t              iIndex=0,
            bool                iIsPointer=false,
            char const*         iFileName=nullptr,
            size_t              iLineNo=0
        ) noexcept : mAdditionalInfo(iSerializer.mAdditionalInfo)
        {
            mAdditionalInfo.pushInstanceInfo(iName, iIndex, iIsPointer, iFileName, iLineNo);
        }
        ~AutoRestoreSerializeInfo() noexcept
        {
            mAdditionalInfo.popInstanceInfo();
        }
    };

    // 例外許可獲得
    bool getNoThrowException() const {return mNoThrowException;}

// ***************************************************************************
//      オブジェクト・トラッキング処理
// ***************************************************************************

//----------------------------------------------------------------------------
//      クラス・インスンタスについてオブジェクト追跡中の管理
//          そのクラスの基底クラスについてもオブジェクト追跡を行うために用いる
//----------------------------------------------------------------------------

private:
    bool    mClassTracking;
    void clearClassTracking() {mClassTracking=false;}

    struct AutoClassTracking
    {
        bool    mClassTrackingBack;
        bool&   mClassTracking;
        AutoClassTracking(BaseSerializer& iSerializer) :
            mClassTrackingBack(iSerializer.mClassTracking),
            mClassTracking(iSerializer.mClassTracking)
        {
            mClassTracking=true;
        }
        ~AutoClassTracking()
        {
            mClassTracking=mClassTrackingBack;
        }
    };

//----------------------------------------------------------------------------
//      オブジェクト追跡テーブルをクリアする
//          追跡テーブルに登録されている領域は、２回目以降、保存／回復されない。
//          追跡テーブルをクリアすることで、保存／回復するようになる。
//      同時に未解決の追跡情報が残っていないかもチェックし、不正なら例外
//----------------------------------------------------------------------------

private:
    void clearTrackingImpl();

public:
    //! @todo T.B.D.
    void clearTracking();

//----------------------------------------------------------------------------
//      SharedPtrTable登録テーブル
//----------------------------------------------------------------------------

private:
    struct SharedPtrTables;
    std::unique_ptr<SharedPtrTables>            mSharedPtrTables;

    SharedPtrTable& registerSharedPtrTable(std::type_index iTypeIndex);

public:
    //! @todo T.B.D.
    template<typename tTypeList>
    tTypeList& getSharedPtrTable(std::type_index iTypeIndex)
    {
        SharedPtrTable& aSharedPtrTable=registerSharedPtrTable(iTypeIndex);

        // 実テーブルが未生成なら生成する
        if (!aSharedPtrTable.mHolder)
        {
            aSharedPtrTable.mHolder.reset(new SharedPtrTable::Holder<tTypeList>);
        }

        SharedPtrTable::Holder<tTypeList>* aHolder=
            dynamic_cast<SharedPtrTable::Holder<tTypeList>*>(aSharedPtrTable.mHolder.get());
        return aHolder->mList;
    }

// ***************************************************************************
//      シリアライズ／デシリアライズ補助
//          Theolizerの内部用
// ***************************************************************************

private:
    friend THEOLIZER_INTERNAL_DLL bool isLastVersion();
    friend THEOLIZER_INTERNAL_DLL bool isSaver();
    friend THEOLIZER_INTERNAL_DLL bool duringBackup();

    // 最新版処理
    bool isLastVersion() const {return mGlobalVersionNo == mLastGlobalVersionNo;}

    // 保存処理
    bool isSaver() const {return mIsSaver;}

    // バックアップ処理中
    bool duringBackup() const {return mDuringBackup;}

// ***************************************************************************
//      シリアライズ機能群
// ***************************************************************************

protected:
    CheckMode               mCheckMode;
    ElementsMapping         mElementsMapping;
    int                     mIndent;
    bool                    mCancelPrettyPrint;

//----------------------------------------------------------------------------
//      トップ・レベル処理補助クラス
//----------------------------------------------------------------------------

    struct THEOLIZER_INTERNAL_DLL AutoRestoreSaveProcess
    {
        BaseSerializer&     mSerializer;
        ElementsMapping     mElementsMapping;
        int                 mIndent;
        bool                mCancelPrettyPrint;

        AutoRestoreSaveProcess(BaseSerializer& iSerializer, size_t iTypeIndex);
        ~AutoRestoreSaveProcess() noexcept;
    };

//----------------------------------------------------------------------------
//      クラス(配列／侵入型／非侵入型)処理補助クラス
//----------------------------------------------------------------------------

    struct THEOLIZER_INTERNAL_DLL AutoRestoreSave
    {
        BaseSerializer&     mSerializer;
        ElementsMapping     mElementsMapping;
        int                 mIndent;
        bool                mCancelPrettyPrint;

        AutoRestoreSave(BaseSerializer& iSerializer,
                    ElementsMapping iElementsMapping=emOrder,
                    bool iCancelPrettyPrint=false);
        ~AutoRestoreSave() noexcept;
    };

//----------------------------------------------------------------------------
//      バージョン番号対応表
//----------------------------------------------------------------------------

    // 派生Serializerに対するバージョン番号
protected:
    unsigned            mSerializerVersionNo;

    // 各TypeIndexに対するバージョン番号
private:
    VersionNoList       mVersionNoList;

//----------------------------------------------------------------------------
//      型情報保存
//----------------------------------------------------------------------------

protected:

//      ---<<< 各TypeIndexに対するバージョン番号対応表作成 >>>---

    void createVersionNoTable();

//      ---<<< ヘッダ内型情報保存 >>>---

    // 通常シリアライズ
    void writeHeaderTypeInfo();

//      ---<<< トップ・レベル保存前後処理 >>>---

    void saveProcessStart(size_t iTypeIndex);
    void saveProcessEnd();

//----------------------------------------------------------------------------
//      中継用仮想関数群
//----------------------------------------------------------------------------

protected:

//      ---<<< 制御用 >>>---

    virtual void writePreElement(bool iDoProcess=false)     {THEOLIZER_INTERNAL_ABORT("");}
    virtual void saveClassStart(bool iIsTop=false)          {THEOLIZER_INTERNAL_ABORT("");}
    virtual void saveClassEnd(bool iIsTop=false)            {THEOLIZER_INTERNAL_ABORT("");}
    virtual void saveControl(int iControl)                  {THEOLIZER_INTERNAL_ABORT("");}
    virtual void saveControl(long iControl)                 {THEOLIZER_INTERNAL_ABORT("");}
    virtual void saveControl(long long iControl)            {THEOLIZER_INTERNAL_ABORT("");}
    virtual void saveControl(unsigned iControl)             {THEOLIZER_INTERNAL_ABORT("");}
    virtual void saveControl(unsigned long iControl)        {THEOLIZER_INTERNAL_ABORT("");}
    virtual void saveControl(unsigned long long iControl)   {THEOLIZER_INTERNAL_ABORT("");}
    virtual void saveControl(std::string const& iControl)   {THEOLIZER_INTERNAL_ABORT("");}
    virtual void saveElementName(ElementsMapping iElementsMapping, const u8string& iElementName)
                                                            {THEOLIZER_INTERNAL_ABORT("");}

//      ---<<< プリミティブ用 >>>---

    #define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSymbol)                \
        virtual void savePrimitive(dType const& iPrimitive) {THEOLIZER_INTERNAL_ABORT("");}

    #include "primitive.inc"

//----------------------------------------------------------------------------
//      Switcher::save()からの呼び出し
//----------------------------------------------------------------------------

//      ---<<< 生配列処理 >>>---

    template<typename tBaseSerializer, typename tArrayType, TrackingMode tTrackingMode>
    void saveArray(tArrayType& iArray)
    {
        AutoRestoreSave aAutoRestoreSave(*this);
        for (size_t i=0; i < std::extent<tArrayType>::value; ++i)
        {
            AutoRestoreSerializeInfo aAutoRestoreSerializeInfo(*this, nullptr, i);

            writePreElement();
            Switcher
            <
                tBaseSerializer,
                typename std::remove_extent<tArrayType>::type,
                false,
                tTrackingMode
            >::save(*this, iArray[i]);
        }
    }

//----------------------------------------------------------------------------
//      TheolizerVersion<>からの呼び出し
//----------------------------------------------------------------------------

//      ---<<< ClassType保存 >>>---

private:
    template<class tVersionType> friend void saveClass(BaseSerializer&, tVersionType&);

    template<class tVersionType>
    void saveClassImpl(tVersionType& iVersion)
    {
        ElementsMapping aElementsMapping = tVersionType::Theolizer::kElementsMapping;
        // ヘッダが有るなら、フォーマット上は順序対応でOK
        if (CheckMode::TypeCheck <= mCheckMode)
        {
            aElementsMapping=emOrder;
        }

        AutoRestoreSave aAutoRestoreSave(*this, aElementsMapping);
        for (size_t i=0; !tVersionType::getElementTheolizer(i).isSentinel() ; ++i)
        {
            if (!mDestinations.isMatch(tVersionType::getElementTheolizer(i).getDestinations()))
        continue;

            // 基底クラスの処理が終わったら、オーナー処理中をクリアする
            if (!tVersionType::getElementTheolizer(i).isBaseClass())
            {
                clearClassTracking();
            }

            char const* aElementName=tVersionType::getElementTheolizer(i).getName();
            AutoRestoreSerializeInfo aAutoRestoreSerializeInfo(*this, aElementName);
            writePreElement();
            saveElementName(aElementsMapping, aElementName);
            tVersionType::getElementTheolizer(i).saveElement(*this, iVersion);
        }
    }

// ***************************************************************************
//      デシリアライズ機能群
// ***************************************************************************

protected:

//----------------------------------------------------------------------------
//      トップ・レベル処理補助クラス
//----------------------------------------------------------------------------

    struct THEOLIZER_INTERNAL_DLL AutoRestoreLoadProcess
    {
        BaseSerializer&     mSerializer;

        AutoRestoreLoadProcess(BaseSerializer& iSerializer, size_t iTypeIndex);
        ~AutoRestoreLoadProcess() noexcept;
    };

//----------------------------------------------------------------------------
//      クラス(配列／侵入型／非侵入型)処理補助クラス
//----------------------------------------------------------------------------

    struct THEOLIZER_INTERNAL_DLL AutoRestoreLoad
    {
        BaseSerializer&     mSerializer;
        ElementsMapping     mElementsMapping;
        std::size_t         mDataTypeIndex;     // 処理中のシリアライズ・データ内TypeIndex

        AutoRestoreLoad(BaseSerializer& iSerializer,
                        ElementsMapping iElementsMapping=emOrder);
        ~AutoRestoreLoad() noexcept;
    };

//----------------------------------------------------------------------------
//      ヘッダ回復と型チェック
//----------------------------------------------------------------------------

protected:
    // 型リスト(TypeIndex基準用)
    struct SerializedTypeListI;
    std::unique_ptr<SerializedTypeListI>    mSerializedTypeListI;

    // 型リスト(TypeName(型名)基準用)
    struct SerializedTypeListN;
    std::unique_ptr<SerializedTypeListN>    mSerializedTypeListN;

    // 型名と型／バージョン番号対応表
    struct TypeNameMap;
    std::unique_ptr<TypeNameMap>            mTypeNameMap;

    // プログラム側TypeIndexから
    // 型リスト(mSerializedTypeListI, mSerializedTypeListN)へのインデック対応表
    std::vector<std::size_t>                mTypeIndexTable;

//      ---<<< 型名と型／バージョン番号対応表生成 >>>---

    void createTypeNameMap(bool iIsClassOnly=false);

//      ---<<< ヘッダ内型情報回復 >>>---

    // 通常デシリアライズ
    void readHeaderTypeInfo();

//      ---<<< TypeIndex一致判定 >>>---

    bool isMatchTypeIndex(size_t iSerializedTypeIndex,
                          size_t iProgramTypeIndex);

//      ---<<< トップ・レベル回復前後処理 >>>---

    void loadProcessStart(size_t iTypeIndex);
    void loadProcessEnd();

//----------------------------------------------------------------------------
//      中継用仮想関数群
//----------------------------------------------------------------------------

protected:

//      ---<<< 制御用 >>>---

    virtual ReadStat readPreElement(bool iDoProcess=false)  {THEOLIZER_INTERNAL_ABORT("");}
    virtual void disposeElement()                           {THEOLIZER_INTERNAL_ABORT("");}
    virtual void loadClassStart(bool iIsTop=false)          {THEOLIZER_INTERNAL_ABORT("");}
    virtual void loadClassEnd(bool iIsTop=false)            {THEOLIZER_INTERNAL_ABORT("");}
    virtual void loadControl(int& iControl)                 {THEOLIZER_INTERNAL_ABORT("");}
    virtual void loadControl(long& iControl)                {THEOLIZER_INTERNAL_ABORT("");}
    virtual void loadControl(long long& iControl)           {THEOLIZER_INTERNAL_ABORT("");}
    virtual void loadControl(unsigned& iControl)            {THEOLIZER_INTERNAL_ABORT("");}
    virtual void loadControl(unsigned long& iControl)       {THEOLIZER_INTERNAL_ABORT("");}
    virtual void loadControl(unsigned long long& iControl)  {THEOLIZER_INTERNAL_ABORT("");}
    virtual void loadControl(std::string& iControl)         {THEOLIZER_INTERNAL_ABORT("");}
    virtual std::string loadElementName(ElementsMapping iElementsMapping)
                                                            {THEOLIZER_INTERNAL_ABORT("");}

//      ---<<< プリミティブ用 >>>---

    #define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSymbol)                \
        virtual void loadPrimitive(dType& iPrimitive)     {THEOLIZER_INTERNAL_ABORT("");}

    #include "primitive.inc"

//----------------------------------------------------------------------------
//      Switcher::load()からの呼び出し
//----------------------------------------------------------------------------

//      ---<<< 生配列処理 >>>---

    template<typename tBaseSerializer, typename tArrayType, TrackingMode tTrackingMode>
    void loadArray(tArrayType& oArray)
    {
        AutoRestoreLoad aAutoRestoreLoad(*this);
        for (size_t i=0; true; ++i)
        {
            ReadStat aReadStat=readPreElement();
            if (!aReadStat)
        break;

            AutoRestoreSerializeInfo aAutoRestoreSerializeInfo(*this, nullptr, i);

            if (i < std::extent<tArrayType>::value)
            {
                Switcher
                <
                    tBaseSerializer,
                    typename std::remove_extent<tArrayType>::type,
                    false,
                    tTrackingMode
                >::load(*this, oArray[i]);
            }
            else
            {
                if (aReadStat == DontCare)
        break;

                // 破棄
                disposeElement();
            }
        }
    }

//----------------------------------------------------------------------------
//      TheolizerVersion<>からの呼び出し
//----------------------------------------------------------------------------

private:
    std::string getDataElementName(std::size_t iDataTypeIndex, std::size_t iDataIndex);

//      ---<<< ClassType回復 >>>---

    template<class tVersionType> friend void loadClass(BaseSerializer&, tVersionType&);

    template<class tVersionType>
    void loadClassImpl(tVersionType& iVersion)
    {
        ElementsMapping aElementsMapping = tVersionType::Theolizer::kElementsMapping;
        // ヘッダが有るなら、フォーマット上は順序対応でOK
        if (CheckMode::TypeCheck <= mCheckMode)
        {
            aElementsMapping=emOrder;
        }
        AutoRestoreLoad aAutoRestoreLoad(*this, aElementsMapping);

        // 要素がないならNOP
        if (tVersionType::getElementTheolizer(0).isSentinel())
    return;

        std::size_t aDataTypeIndex=0;
        if ((tVersionType::Theolizer::kElementsMapping == emName)
         && (CheckMode::TypeCheck <= mCheckMode))
        {
            std::size_t aTypeIndex = getTypeIndex<typename tVersionType::TheolizerTarget>();
            aDataTypeIndex=mTypeIndexTable[aTypeIndex];
            THEOLIZER_INTERNAL_ASSERT(aDataTypeIndex != kInvalidSize,
                "aTypeIndex=%d aDataTypeIndex=%d", aTypeIndex, aDataTypeIndex);
        }
        std::size_t aDataIndex=0;   // シリアライデ・データ側クラスの要素Index
        size_t aIndex=0;            // プログラム側クラスの要素Index
        while(true)
        {
            ReadStat aReadStat=readPreElement();
            if (!aReadStat)
        break;

            std::string  aElementName;
            if ((tVersionType::Theolizer::kElementsMapping == emName)
             && (CheckMode::TypeCheck <= mCheckMode))
            {
                aElementName=getDataElementName(aDataTypeIndex, aDataIndex++);
            }
            else
            {
                aElementName=loadElementName(aElementsMapping);
            }

            AutoRestoreSerializeInfo aAutoRestoreSerializeInfo(*this, aElementName.c_str());

            // 回復先を確定する
            size_t i=aIndex;
            bool aDoLoad=false;
            do
            {
                // 順序対応
                if (aElementName.empty())
                {
                    // 最後に到達したら破棄
                    if (tVersionType::getElementTheolizer(i).isSentinel()) {
                        aIndex=i;
            break;
                    }

                    // 保存先が一致したら回復
                    if (mDestinations.isMatch
                        (
                            tVersionType::getElementTheolizer(i).getDestinations()
                        )
                       )
                    {
                        aIndex=i;
                        aDoLoad=true;
            break;
                    }
                }

                // 名前対応、かつ、名前一致
                else if (aElementName == tVersionType::getElementTheolizer(i).getName())
                {
                    aIndex=i;
                    aDoLoad=true;
            break;
                }

                // 次処理
                ++i;
                if (tVersionType::getElementTheolizer(i).isSentinel())
                {
                    // 名前対応なら先頭へ戻す
                    if (!aElementName.empty())
                    {
                        i=0;
                    }
                    // 順序対応ならループ終了
                    else
                    {
            break;
                    }
                }
            }
            while(aIndex != i);

            // 回復 or 破棄処理
            if (aDoLoad)
            {
                // 基底クラスの処理が終わったら、オーナー処理中をクリアする
                if (!tVersionType::getElementTheolizer(aIndex).isBaseClass())
                {
                    clearClassTracking();
                }

                // 回復
                tVersionType::getElementTheolizer(aIndex).loadElement(*this, iVersion);

                // 次処理
                aIndex++;
                if (tVersionType::getElementTheolizer(aIndex).isSentinel())
                {
                    // FastSerializer(変更不可)の時は直ぐに終了
                    if (aReadStat == DontCare)
        break;
                    aIndex=0;
                }
            }
            else
            {
                // FastSerializer(変更不可)の時は破棄しない
                if (aReadStat == DontCare)
        break;

                // 破棄
                disposeElement();
            }
        }
    }

//----------------------------------------------------------------------------
//      ClassTypeの破棄処理
//----------------------------------------------------------------------------

protected:
    void disposeClass(ElementsMapping iElementsMapping);

// ***************************************************************************
//      オブジェクト追跡機能群
// ***************************************************************************

//----------------------------------------------------------------------------
//      シリアライズ用オブジェクト・リスト
//          オブジェクトID=0はnullptr用に予約
//----------------------------------------------------------------------------

private:
    size_t                              mObjectId;
    struct SerializeList;
    std::unique_ptr<SerializeList>      mSerializeList;

//----------------------------------------------------------------------------
//      シリアライズ用オブジェクト登録
//          戻り値はSerializeInfo&
//----------------------------------------------------------------------------

    SerializeInfo& registerObject
        (
            void* iAddress,
            std::type_info const& iTypeInfo,
            TrackingMode iTrackingMode,
            bool* oIsSaved
        );

//----------------------------------------------------------------------------
//      デシリアライズ用オブジェクト・リスト
//          オブジェクトID=0はnullptr用に予約
//----------------------------------------------------------------------------

    struct DeserializeList;
    std::unique_ptr<DeserializeList>    mDeserializeList;

//----------------------------------------------------------------------------
//      デシリアライズ用回復済チェック
//          戻り値：指定ObjectIdのオブジェクトが回復済ならtrue返却
//----------------------------------------------------------------------------

    bool isLoadedObject(size_t iObjectId, void*& oPointer);

//----------------------------------------------------------------------------
//      デシリアライズ用アドレス登録／回復
//          ポインタのアドレス登録／回復なら、oPointerが有効
//          オブジェクトのアドレスを登録済ポインタへ回復する時、iObjectが有効
//      戻り値：追跡指定異常時、false
//----------------------------------------------------------------------------

    void recoverObject
    (
        size_t iObjectId,
        void*& oPointer,
        std::type_info const& iTypeInfo,
        TrackingMode iTrackingMode,
        bool* oIsLoaded=nullptr
    );

// ***************************************************************************
//      ポリモーフィズム対応
// ***************************************************************************

//----------------------------------------------------------------------------
//      型名／TypeIndexを読み込み、対応するプログラム側のTypeIndexを求める
//----------------------------------------------------------------------------

    virtual TypeIndexList& getProgramTypeIndex();

// ***************************************************************************
//      メタ処理
// ***************************************************************************

//----------------------------------------------------------------------------
//      メタ・シリアライズ
//----------------------------------------------------------------------------

protected:
    std::ostream*   mOStream;
    bool            mIsMetaMode;

    // メタ・データのシリアライズ
    void writeMetaData();

private:
    // メタ・シリアライズ保存判定
    void judgeWriteTypes
    (
        std::size_t iTypeIndex,
        BaseTypeInfo* iTypeInfo,
        TypeInfoListImpl const& iTypeInfoList,
        std::vector<SaveStat>& ioSaveStatList
    );

    // プリミティブ(派生Serializer)のメタ・シリアライズ処理
    void writeMetaPrimitive();

    // enum型のメタ・シリアライズ処理
    void writeMetaEnum
    (
        std::vector<VersionNoList> const& iVersionNoLists,
        TypeInfoListImpl const& iTypeInfoList,
        std::vector<SaveStat>& ioSaveStatList
    );

    // クラスのメタ・シリアライズ処理
    struct UniqueNameSet;
    void writeMetaClass
    (
        std::vector<VersionNoList> const& iVersionNoLists,
        TypeInfoListImpl const& iTypeInfoList,
        std::vector<SaveStat>& ioSaveStatList,
        UniqueNameSet& ioUniqueNameSet
    );

//----------------------------------------------------------------------------
//      メタ・デシリアライズ
//----------------------------------------------------------------------------

protected:
    // メタ・データのデシリアライズ
    void readMetaData();

    static char const* getCppName(std::string const& iPrimitiveName, unsigned iSerializerVersionNo)
    {
        return "";
    }
};
#ifndef THEOLIZER_INTERNAL_DOXYGEN

//############################################################################
//      internal化のための中継処理
//############################################################################

// ***************************************************************************
//      指定Serializerへの中継
// ***************************************************************************

template<class tVersionType>
void saveClass(BaseSerializer& iBaseSerializer, tVersionType& iVersion)
{
    iBaseSerializer.saveClassImpl(iVersion);
}

template<class tVersionType>
void loadClass(BaseSerializer& iBaseSerializer, tVersionType& iVersion)
{
    iBaseSerializer.loadClassImpl(iVersion);
}

// ***************************************************************************
//      現在処理中のSerializerの状態獲得
// ***************************************************************************

THEOLIZER_INTERNAL_DLL bool isLastVersion();
THEOLIZER_INTERNAL_DLL bool isSaver();
THEOLIZER_INTERNAL_DLL bool duringBackup();

//############################################################################
//      End
//############################################################################

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal
}   // namespace theolizer

// ***************************************************************************
//          警告抑止解除
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#endif  // THEOLIZER_INTERNAL_CORE_SERIALIZER_H
