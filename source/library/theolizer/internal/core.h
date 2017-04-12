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

#if !defined(THEOLIZER_INTERNAL_CORE_H)
#define THEOLIZER_INTERNAL_CORE_H

#include "core_switcher.h"

//############################################################################
//      Begin
//############################################################################

#ifndef THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//      TheolizerNonKeepStepクラス
//          プリミティブ    部分特殊化
//          その他          プライマリ
//              ポインタ
//              非侵入型
// ***************************************************************************

//----------------------------------------------------------------------------
//      プライマリー
//----------------------------------------------------------------------------

template<class tClassType, class tEnable=void>
struct TheolizerNonKeepStep
{
    static const bool       kIsTheolizerNonKeepStep=true;
    typedef tClassType      Type;

private:
    typedef theolizer::internal::TrackingMode   TrackingMode;
    typedef TheolizerNonKeepStep<Type>          This;

//      ---<<< 変更と引継ぎ >>>---

    // 削除時の実体
    Type        mValue;

    // ターゲット
    Type*       mTarget;

public:
    // ターゲットからのコンストラクタ
    TheolizerNonKeepStep(Type& iTarget) :
        mValue(),
        mTarget(&iTarget)
    { }

    // 次からのコンストラクタ
    TheolizerNonKeepStep(This& iNext) :
        mValue(),
        mTarget(iNext.mTarget)
    { }

    // ムーブ・コンストラクタ(配列処理用)
    TheolizerNonKeepStep(This&& iNext) :
        mValue(),
        mTarget(iNext.mTarget)
    { }

    // デフォルト・コンストラクタ(次で削除されていた場合)
    TheolizerNonKeepStep() :
        mValue(),
        mTarget(&mValue)
    { }

//      ---<<< 値取り出し >>>---

//  Type get()      const { return *mTarget; }
//  operator Type() const { return get(); }

//      ---<<< 保存／回復処理 >>>---

    template<bool tIsRegister, TrackingMode tTrackingMode, class tBaseSerializer>
    void save(tBaseSerializer& iSerializer)
    {
std::cout << "TheolizerNonKeepStep<tClassType>\n";
        theolizer::internal::Switcher<tBaseSerializer, Type, tIsRegister, tTrackingMode>::
            save(iSerializer, *mTarget);
    }

    template<bool tIsRegister, TrackingMode tTrackingMode, class tBaseSerializer>
    void load(tBaseSerializer& iSerializer)
    {
        theolizer::internal::Switcher<tBaseSerializer, Type, tIsRegister, tTrackingMode>::
            load(iSerializer, *mTarget);
    }
};

//----------------------------------------------------------------------------
//      TheolizerNonKeepStepの分岐
//----------------------------------------------------------------------------

namespace theolizer
{
namespace internal
{

//      ---<<< プライマリー >>>---

template<typename tPrimitiveType, class tThis, class tEnable=void>
class TheolizerNonKeepStepBase { };

//      ---<<< 数値型用部分特殊化 >>>---

template<typename tArithmeticType, class tThis>
class TheolizerNonKeepStepBase
<
    tArithmeticType,
    tThis,
    theolizer::internal::EnableIf<std::is_arithmetic<tArithmeticType>::value>
>
{
    typedef tArithmeticType Type;
    typedef     tThis       This;
    Type    mData;
protected:
    TheolizerNonKeepStepBase(Type iData=0) : mData(iData) { }
    Type& getData() { return mData; }

    virtual This& operator=(Type iRhs)=0;
    virtual ~TheolizerNonKeepStepBase() { }

public:
    // 各種演算子定義
#if 0
    // 下記関数は「グローバルなoperator dCmp(Type, Type)やoperator dOpe(Type, Type)と曖昧になる」
    #define THEOLIZER_INTERNAL_COMPARE(dCmp)\
        bool operator dCmp(Type iRhs) const { return mData dCmp iRhs; }\
        friend bool operator dCmp(Type iLhs, This const& iRhs) { return iLhs dCmp iRhs.mData; }
        THEOLIZER_INTERNAL_COMPARE(==)
        THEOLIZER_INTERNAL_COMPARE(!=)
        THEOLIZER_INTERNAL_COMPARE(<)
        THEOLIZER_INTERNAL_COMPARE(>)
        THEOLIZER_INTERNAL_COMPARE(<=)
        THEOLIZER_INTERNAL_COMPARE(>=)
    #undef  THEOLIZER_INTERNAL_COMPARE
    #define THEOLIZER_INTERNAL_BINARY(dOpe)\
        Type operator dOpe(Type iRhs) const { return mData dOpe iRhs; }\
        friend Type operator dOpe(Type iLhs, This const& iRhs) { return iLhs dOpe iRhs.mData; }
        THEOLIZER_INTERNAL_BINARY(+)
        THEOLIZER_INTERNAL_BINARY(-)
        THEOLIZER_INTERNAL_BINARY(*)
        THEOLIZER_INTERNAL_BINARY(/)
        THEOLIZER_INTERNAL_BINARY(%)
        THEOLIZER_INTERNAL_BINARY(<<)
        THEOLIZER_INTERNAL_BINARY(>>)
        THEOLIZER_INTERNAL_BINARY(&)
        THEOLIZER_INTERNAL_BINARY(^)
        THEOLIZER_INTERNAL_BINARY(|)
    #undef  THEOLIZER_INTERNAL_BINARY

    #define THEOLIZER_INTERNAL_ASSIGN(dOpe)\
        This& operator dOpe##=(Type iRhs) { return operator=(mData dOpe iRhs); }
        THEOLIZER_INTERNAL_ASSIGN(+)
        THEOLIZER_INTERNAL_ASSIGN(-)
        THEOLIZER_INTERNAL_ASSIGN(*)
        THEOLIZER_INTERNAL_ASSIGN(/)
        THEOLIZER_INTERNAL_ASSIGN(%)
        THEOLIZER_INTERNAL_ASSIGN(<<)
        THEOLIZER_INTERNAL_ASSIGN(>>)
        THEOLIZER_INTERNAL_ASSIGN(&)
        THEOLIZER_INTERNAL_ASSIGN(^)
        THEOLIZER_INTERNAL_ASSIGN(|)
    #undef  THEOLIZER_INTERNAL_ASSIGN
    This& operator++()     { return operator=(mData+1); }\
    Type  operator++(Type) { Type temp=mData; operator=(mData+1); return temp; }
    This& operator--()     { return operator=(mData-1); }\
    Type  operator--(Type) { Type temp=mData; operator=(mData-1); return temp; }
#endif
};

//      ---<<< 文字列型用部分特殊化 >>>---

template<typename tStringType, class tThis>
struct TheolizerNonKeepStepBase
<
    tStringType,
    tThis,
    theolizer::internal::EnableIf<theolizer::internal::IsString<tStringType>::value>
> : public tStringType
{
protected:
    TheolizerNonKeepStepBase(tStringType const& iString="") : tStringType(iString) { }
    TheolizerNonKeepStepBase(tStringType&& iString) : tStringType(std::forward(iString)) { }
    tStringType& getData() { return *this; }

private:
    // 禁止するメソッド
    // Iterators
    using tStringType::begin;
    using tStringType::end;
    using tStringType::rbegin;
    using tStringType::rend;

    // Capacity
    using tStringType::resize;
    using tStringType::clear;

    // Element access
    using tStringType::operator[];
    using tStringType::at;

    // Modifiers
    using tStringType::operator+=;
    using tStringType::append;
    using tStringType::push_back;
    using tStringType::assign;
    using tStringType::insert;
    using tStringType::erase;
    using tStringType::replace;
    using tStringType::swap;
    using tStringType::pop_back;
};

}   // namespace internal
}   // namespace theolizer

//----------------------------------------------------------------------------
//      TheolizerNonKeepStepプリミティブ用部分特殊化の実体部
//----------------------------------------------------------------------------

template<typename tPrimitiveType>
struct TheolizerNonKeepStep
<
    tPrimitiveType,
    theolizer::internal::EnableIf<theolizer::internal::IsPrimitive<tPrimitiveType>::value>
> : public theolizer::internal::TheolizerNonKeepStepBase
    <
        tPrimitiveType,
        TheolizerNonKeepStep<tPrimitiveType>
    >
{
    static const bool       kIsTheolizerNonKeepStep=true;
    typedef tPrimitiveType  Type;

private:
    typedef TheolizerNonKeepStep<Type>                                          This;
    typedef theolizer::internal::TheolizerNonKeepStepBase<tPrimitiveType, This> Base;
    typedef theolizer::internal::TrackingMode                                   TrackingMode;
    typedef theolizer::internal::BaseSerializer                                 BaseSerializer;

//      ---<<< 変更と引継ぎ >>>---

    // 実体
    Type&       mValue;
    Type        mBackup;

    // 引継ぎ先（ターゲット or 次バージョンへのポインタ)
    Type*       mTarget;
    This*       mNextPtr;
    bool        mIsTarget;

    // 変更管理
    bool        mDoSucceed;     // 引継ぎフラグ
    bool        mIsChanged;     // これを含むクラスのupVersionにて変更された
    unsigned    mUpVersionCount;// upVersionカウンタ

public:
    // ターゲットからのコンストラクタ
    TheolizerNonKeepStep(Type& iTarget) :
        Base(iTarget),
        mValue(Base::getData()),
        mBackup(),
        mTarget(&iTarget),
        mNextPtr(nullptr),
        mDoSucceed(false),
        mIsChanged(false),
        mUpVersionCount(0)
    { }

    // 次からのコンストラクタ
    TheolizerNonKeepStep(This& iNext) :
        Base(iNext.mValue),
        mValue(Base::getData()),
        mBackup(),
        mTarget(iNext.mTarget),
        mNextPtr(&iNext),
        mDoSucceed(false),
        mIsChanged(false),
        mUpVersionCount(0)
    { }

    // ムーブ・コンストラクタ(配列処理用)
    TheolizerNonKeepStep(This&& iNext) :
        Base(std::move(iNext.mValue)),
        mValue(Base::getData()),
        mBackup(),
        mTarget(iNext.mTarget),
        mNextPtr(iNext.mNextPtr),
        mDoSucceed(false),
        mIsChanged(false),
        mUpVersionCount(0)
    { }

    // デフォルト・コンストラクタ(次で削除されていた場合)
    TheolizerNonKeepStep() :
        Base(),
        mValue(Base::getData()),
        mBackup(),
        mTarget(nullptr),
        mNextPtr(nullptr),
        mDoSucceed(false),
        mIsChanged(false),
        mUpVersionCount(0)
    { }

    // 当プリミティブを含むクラスやその上位クラスのupVersionからの通常設定
    This& operator=(Type iRhs)
    {
std::cout << "TheolizerNonKeepStep<" << THEOLIZER_INTERNAL_TYPE_NAME(Type)
          << ">::operator=(" << iRhs << ");\n";
std::cout << "    mUpVersionCount    =" << mUpVersionCount << "\n";
std::cout << "    getUpVersionCount()=" << theolizer::internal::getUpVersionCount() << "\n";

        unsigned aUpVersionCount=theolizer::internal::getUpVersionCount();

        // バージョン・ダウン中
        if (aUpVersionCount == 0)
        {
            mValue=iRhs;
        }

        // 回復後のバージョン・アップ中、かつ、有効なset呼び出し無し
        else if (!mDoSucceed)
        {
            // 最初の呼び出しなら、バックアップする
            if (mUpVersionCount == 0)
            {
                mBackup=mValue;
                mUpVersionCount=aUpVersionCount;
            }

            // 最初の呼び出しと同じupVersion()からの呼び出し
            if (mUpVersionCount == aUpVersionCount)
            {
                mValue=iRhs;
                mIsChanged=true;
            }
        }

std::cout << "    mValue               =" << mValue << "\n";
std::cout << "    mIsChanged           =" << mIsChanged << "\n";
std::cout << "    mDoSucceed           =" << mDoSucceed << "\n";
        return *this;
    }

    // 強制引継ぎ設定（他の変数から引き継ぐ）
    void set(Type iValue, bool iDoSucceed)  // iDoSucceedは引継ぎ元変数のmDoSucceed
    {
std::cout << "TheolizerNonKeepStep<" << THEOLIZER_INTERNAL_TYPE_NAME(Type)
          << ">::set(" << iValue << ", " << iDoSucceed << ");\n";
std::cout << "    mUpVersionCount    =" << mUpVersionCount << "\n";
std::cout << "    getUpVersionCount()=" << theolizer::internal::getUpVersionCount() << "\n";

        unsigned aUpVersionCount=theolizer::internal::getUpVersionCount();

        // バージョン・ダウン中
        if (aUpVersionCount == 0)
        {   // 使用禁止
            THEOLIZER_INTERNAL_ABORT("Do not call <member>.set() in downVersion().");
        }

        // 回復後のバージョン・アップ中、かつ、有効なset呼び出し
        else if (iDoSucceed)
        {
            // 最初の呼び出しなら、バックアップする
            if (mUpVersionCount == 0)
            {
                mBackup=mValue;
                mUpVersionCount=aUpVersionCount;
            }

            // 最初の呼び出しと同じupVersion()からの呼び出し
            if (mUpVersionCount == aUpVersionCount)
            {
                mValue=iValue;
                mDoSucceed=true;
            }
        }
std::cout << "    mValue               =" << mValue << "\n";
std::cout << "    mIsChanged           =" << mIsChanged << "\n";
std::cout << "    mDoSucceed           =" << mDoSucceed << "\n";
    }

private:
    // 前バージョンのデストラクタから呼ばれる引継ぎ処理
    void succeed(Type iValue, bool iDoSucceed)
    {
std::cout << "TheolizerNonKeepStep<" << THEOLIZER_INTERNAL_TYPE_NAME(Type)
          << ">::succeed(" << iValue << ");\n";
std::cout << "    mIsChanged =" << mIsChanged << "\n";

        // setされていない時
        if (!mDoSucceed)
        {   // 引継ぎ無し
            if (!iDoSucceed)
            {   // 変更されていたらリストアする
                if (mIsChanged)
                {
                    mValue=mBackup;
                }
            }

            // 引継ぎ有り
            else
            {
                mDoSucceed=true;

                // 変更されていないなら、引き継ぐ
                if (!mIsChanged)
                {
                    mValue=iValue;
                }
            }
        }
std::cout << "    mValue               =" << mValue << "\n";
std::cout << "    mIsChanged           =" << mIsChanged << "\n";
std::cout << "    mDoSucceed           =" << mDoSucceed << "\n";
    }

public:
    // デストラクタ(次バージョンへの引継ぎ実行)
    ~TheolizerNonKeepStep()
    {
std::cout << "TheolizerNonKeepStep<" << THEOLIZER_INTERNAL_TYPE_NAME(Type)
          << ">::TheolizerNonKeepStep();\n";
std::cout << "    mValue               =" << mValue << "\n";
std::cout << "    mIsChanged           =" << mIsChanged << "\n";
std::cout << "    mDoSucceed           =" << mDoSucceed << "\n";

        // 引継ぎ処理
        if (mNextPtr)
        {
std::cout << "    mNextPtr->succeed(" << mValue << ");\n";
            mNextPtr->succeed(mValue, mDoSucceed);
        }
        else if (mTarget)
        {
std::cout << "    *mTarget" << mValue << ");\n";
            *mTarget=mValue;
        }
    }

//      ---<<< 値取り出し >>>---

    Type get()      const { return (mTarget)?*mTarget:mNextPtr->get(); }
    operator Type() const { return get(); }

//      ---<<< 保存／回復処理 >>>---

    // 保存
    template<bool tIsRegister, TrackingMode tTrackingMode, class tBaseSerializer>
    void save(tBaseSerializer& iSerializer)
    {
std::cout << "TheolizerNonKeepStep<tPrimitiveType>\n";
        if (tTrackingMode == TrackingMode::etmDefault)
        {
            iSerializer.savePrimitive(mValue);
        }
        else
        {
            BaseSerializer::AutoRestoreSave aAutoRestoreSave
            (
                iSerializer,
                theolizer::internal::emOrder,
                true
            );
            bool aIsSaved;
            theolizer::internal::SerializeInfo& aSerializeInfo=
                iSerializer.registerObject
                (
                    mTarget,
                    typeid(mTarget),
                    tTrackingMode,
                    &aIsSaved
                );
            iSerializer.writePreElement();
            iSerializer.saveControl(aSerializeInfo.mObjectId);
            // 未保存の時のみ保存する
            if (!aIsSaved)
            {
                iSerializer.writePreElement();
                iSerializer.savePrimitive(mValue);
            }
        }
    }

    // 回復
    template<bool tIsRegister, TrackingMode tTrackingMode, class tBaseSerializer>
    void load(tBaseSerializer& iSerializer)
    {
        if (tTrackingMode == TrackingMode::etmDefault)
        {
            iSerializer.loadPrimitive(mValue);
        }
        else
        {
            BaseSerializer::AutoRestoreLoad aAutoRestoreLoad(iSerializer);
            size_t aObjectId;
            iSerializer.readPreElement();
            iSerializer.loadControl(aObjectId);
            bool aIsLoaded;
            iSerializer.recoverObject
            (
                aObjectId,
                reinterpret_cast<void*&>(mTarget),
                typeid(mTarget),
                tTrackingMode,
                &aIsLoaded
            );
            // 未回復の時のみ回復する
            if (!aIsLoaded)
            {
                iSerializer.readPreElement();
                iSerializer.loadPrimitive(mValue);
            }
        }
    }
};

#endif  // THEOLIZER_INTERNAL_DOXYGEN

//############################################################################
//      バージョン対応用定義群
//############################################################################

namespace theolizer
{
namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

//----------------------------------------------------------------------------
//      Element/EnumElementの基底クラス
//----------------------------------------------------------------------------

struct ElementBase
{
    // クラス/enum共通
    virtual void writeMetaData
    (
        BaseSerializer& iSerializer,
        VersionNoList const* iVersionNoList=nullptr
    ) const = 0;

    // クラスのみ
    virtual char const* getName() const                 {return "";}
    virtual std::size_t getTypeIndex() const            {return 0;}
    virtual bool isBaseClass() const                    {return false;}
    virtual Destinations const& getDestinations() const = 0;
};

// ***************************************************************************
//      バージョン定義(クラス型)
// ***************************************************************************

//----------------------------------------------------------------------------
//      TheolizerVersion<>内の要素(メンバー変数)管理クラス
//----------------------------------------------------------------------------

template<class tVersionType>
class Element : public ElementBase
{
private:
    typedef typename tVersionType::Serializer Serializer;

//      ---<<< 記録用内部クラスの基底型 >>>---

    struct HolderBase
    {
        virtual size_t getTypeIndex() const =0;
        virtual void saveElement(Serializer& iSerializer, tVersionType& iInstance) const=0;
        virtual void loadElement(Serializer& iSerializer, tVersionType& iInstance) const=0;
        virtual ~HolderBase() { }
    };

//      ---<<< 記録用内部クラス >>>---

    template<typename tTypeFunc, typename tSaveFunc, typename tLoadFunc>
    struct Holder : public HolderBase
    {
        tTypeFunc           mTypeFunc;          // TypeIndex関数
        tSaveFunc           mSaveFunc;          // 要素保存関数
        tLoadFunc           mLoadFunc;          // 要素回復関数

        Holder(tTypeFunc iTypeFunc, tSaveFunc iSaveFunc, tLoadFunc iLoadFunc) :
                    mTypeFunc(iTypeFunc),
                    mSaveFunc(iSaveFunc),
                    mLoadFunc(iLoadFunc)
        { }

        ~Holder() = default;

        virtual size_t getTypeIndex() const
        {
            return mTypeFunc();
        }

        virtual void saveElement(Serializer& iSerializer,
                                 tVersionType& iInstance) const
        {
            mSaveFunc(iSerializer, iInstance);
        }
        virtual void loadElement(Serializer& iSerializer,
                                 tVersionType& iInstance) const
        {
            mLoadFunc(iSerializer, iInstance);
        }
    };

//      ---<<< 記録領域 >>>---

    char const* const           mName;
    bool const                  mIsBaseClass;
    Destinations                mDestinations;
    std::unique_ptr<HolderBase> mHolder;
    char const* const           mNextName;
    TrackingMode                mTrackingMode;
    unsigned                    mVersionNo;

//      ---<<< コンストラクタ >>>---

public:
    Element() :
        mName(),
        mIsBaseClass(false),
        mDestinations(),
        mHolder(),
        mNextName(),
        mTrackingMode(),
        mVersionNo(0)
    { }

    template<typename tTypeFunc, typename tSaveFunc, typename tLoadFunc>
    Element
    (
        char const* const iName,
        bool iIsBaseClass,
        std::initializer_list<Destination> const& iDestinationList,
        tTypeFunc iTypeFunc,
        tSaveFunc iSaveFunc,
        tLoadFunc iLoadFunc,
        char const* const iNextName,
        TrackingMode iTrackingMode,
        unsigned iVersionNo
    ) : mName(iName),
        mIsBaseClass(iIsBaseClass),
        mDestinations(iDestinationList),
        mHolder
        (
            new Holder<tTypeFunc, tSaveFunc, tLoadFunc>
            (
                iTypeFunc,
                iSaveFunc,
                iLoadFunc
            )
        ),
        mNextName(iNextName),
        mTrackingMode(iTrackingMode),
        mVersionNo(iVersionNo)
    { }

    // ムーブ・コンストラクタ
    Element(Element&& iRhs) noexcept :
        mName        (iRhs.mName),
        mIsBaseClass (iRhs.mIsBaseClass),
        mDestinations(std::move(iRhs.mDestinations)),
        mHolder      (std::move(iRhs.mHolder)),
        mNextName    (iRhs.mNextName),
        mTrackingMode(iRhs.mTrackingMode),
        mVersionNo   (iRhs.mVersionNo)
    { }


//      ---<<< 基本サービス関数 >>>---

    const bool isSentinel() const       {return mName == nullptr;}

    char const* getName() const         {return mName;}
    Destinations const& getDestinations() const
    {
        return mDestinations;
    }
    bool isBaseClass() const {return mIsBaseClass;}
    size_t getTypeIndex() const
    {
        return mHolder->getTypeIndex();
    }
    void saveElement(Serializer& iSerializer, tVersionType& iInstance) const
    {
        mHolder->saveElement(iSerializer, iInstance);
    }
    void loadElement(Serializer& iSerializer, tVersionType& iInstance) const
    {
        mHolder->loadElement(iSerializer, iInstance);
    }

//      ---<<< メタ・シリアライズ >>>---

    void writeMetaData
    (
        BaseSerializer& iSerializer,
        VersionNoList const* iVersionNoList
    ) const
    {
        // 要素毎の塊
        iSerializer.writePreElement();
        BaseSerializer::AutoRestoreSave aAutoRestoreSave(iSerializer, emOrder, true);

        // TypeInfo獲得
        auto aTypeIndex=getTypeIndex();
        auto aTypeInfo=TypeInfoList::getInstance().getList()[aTypeIndex];

        // 要素名
        iSerializer.writePreElement();
        iSerializer.saveControl(mName);

        // 次バージョン要素名
        iSerializer.writePreElement();
        iSerializer.saveControl(mNextName);

        // TypeKind
        iSerializer.writePreElement();
        iSerializer.saveControl(aTypeInfo->getTypeKind());

        // 形名(*/[]修飾付き)
        iSerializer.writePreElement();
        iSerializer.saveControl(aTypeInfo->getTypeName(*iVersionNoList));

        // バージョン番号
        iSerializer.writePreElement();
        iSerializer.saveControl(mVersionNo);

        // オブジェクト追跡指定
        iSerializer.writePreElement();
        iSerializer.saveControl(mTrackingMode);
    }
};

//----------------------------------------------------------------------------
//      Element登録用マクロ
//----------------------------------------------------------------------------

//      ---<<< 基底クラス登録用マクロ >>>---
//          RegisterPointee<>は、派生クラス登録(THEOLIZER_REGISTER_CLASS)時に
//          基底クラスへの登録処理が間に合わないケース対処のため、ここで呼び出す。
//              boostのかなり特殊なテクニックを使っている。
//
//          2016/08/02現在、基底Serializerを非テンプレート化することで、
//          基底クラスへの登録を単純化できたので、登録場所はここである必要はない筈。

#define THEOLIZER_INTERNAL_REGISTER_BASE(dSpec, dTrack, dId, dType, dVerNo, dName, dVer)\
    {                                                                       \
        /*基底クラス名*/    "{" dName "}",                                  \
        /*Is基底クラス*/    true,                                           \
        /*保存先*/          {theolizerD::All},                              \
        /*TypeIndex関数*/                                                   \
        []()                                                                \
        {                                                                   \
            typedef THEOLIZER_INTERNAL_UNPAREN dType Type;                  \
            theolizer::internal::                                           \
                RegisterType<tBaseSerializer, Type, ::tTheolizerVersion, true>::getInstance();\
            return theolizer::internal::getTypeIndex<Type>();               \
        },                                                                  \
        /*保存関数*/                                                        \
        [](tBaseSerializer& iSerializer, TheolizerVersion& iInstance)       \
        {                                                                   \
            typedef THEOLIZER_INTERNAL_UNPAREN dVer VerType;                \
            THEOLIZER_INTERNAL_SAVE(iSerializer,                            \
                    static_cast<VerType&>(iInstance), dTrack);              \
        },                                                                  \
        /*回復関数*/                                                        \
        [](tBaseSerializer& iSerializer, TheolizerVersion& iInstance)       \
        {                                                                   \
            typedef THEOLIZER_INTERNAL_UNPAREN dVer VerType;                \
            THEOLIZER_INTERNAL_LOAD(iSerializer,                            \
                    static_cast<VerType&>(iInstance), dTrack);              \
        },                                                                  \
        /*public/private/protected(次バージョンの要素名)*/                  \
        #dSpec,                                                             \
        /*(オブジェクト追跡モード)*/                                        \
        theolizer::internal::etmDefault,                                    \
        /*バージョン番号*/                                                  \
        dVerNo                                                              \
    }

//      ---<<< 基底クラス登録用マクロ2 >>>---
//      非侵入型手動の基底クラス用（ポリモーフィズム用の基底クラス・リストの定義）
//      保存／回復処理は手動なので保存／回復処理不要

#define THEOLIZER_INTERNAL_REGISTER_MBASE(dSpec, dTrack, dType, dVerNo, dName, dVer)\
    {                                                                       \
        /*基底クラス名*/    "{" dName "}",                                  \
        /*Is基底クラス*/    true,                                           \
        /*保存先*/          {theolizerD::All},                              \
        /*TypeIndex関数*/                                                   \
        []()                                                                \
        {                                                                   \
            typedef THEOLIZER_INTERNAL_UNPAREN dType Type;                  \
            theolizer::internal::                                           \
                RegisterType<tBaseSerializer, Type, ::tTheolizerVersion, true>::getInstance();\
            return theolizer::internal::getTypeIndex<Type>();               \
        },                                                                  \
        [](tBaseSerializer& iSerializer, TheolizerVersion& iInstance){},    \
        [](tBaseSerializer& iSerializer, TheolizerVersion& iInstance){},    \
        #dSpec,                                                             \
        theolizer::internal::etmDefault,                                    \
        dVerNo                                                              \
    }

//      ---<<< 要素登録用マクロ >>>---

#define THEOLIZER_INTERNAL_REGISTER_ELEMENT(dName,dNextName,dTrack,dDest,dType,dVerNo,dVer)\
    {                                                                       \
        /*要素名*/          THEOLIZER_INTERNAL_STRINGIZE(THEOLIZER_INTERNAL_FIRST dName),\
        /*Is基底クラス*/    false,                                          \
        /*保存先*/          {THEOLIZER_INTERNAL_UNPAREN dDest},             \
        /*TypeIndex関数*/                                                   \
        []()                                                                \
        {                                                                   \
            using Type=THEOLIZER_INTERNAL_UNPAREN dType;                    \
            return theolizer::internal::getTypeIndex<Type>();               \
        },                                                                  \
        /*保存関数*/                                                        \
        [](tBaseSerializer& iSerializer, TheolizerVersion& iInstance)       \
        {                                                                   \
            THEOLIZER_INTERNAL_SAVE(iSerializer,                            \
                iInstance.THEOLIZER_INTERNAL_FIRST dName, dTrack);          \
        },                                                                  \
        /*回復関数*/                                                        \
        [](tBaseSerializer& iSerializer, TheolizerVersion& iInstance)       \
        {                                                                   \
            THEOLIZER_INTERNAL_LOAD(iSerializer,                            \
                iInstance.THEOLIZER_INTERNAL_FIRST dName, dTrack);          \
            iInstance.THEOLIZER_INTERNAL_SUCCEED_FLG(dName)=true;           \
        },                                                                  \
        /*(次バージョンの要素名)*/                                          \
        #dNextName,                                                         \
        /*(オブジェクト追跡モード)*/                                        \
        theolizer::internal::dTrack,                                        \
        /*バージョン番号*/                                                  \
        dVerNo                                                              \
    }

//      ---<<< 番兵 >>>---

#define THEOLIZER_INTERNAL_REGISTER_SENTINEL(dVersion)                      \
    {                                                                       \
        nullptr,                                                            \
        false,                                                              \
        {theolizerD::All},                                                  \
        [](){return size_t(0);},                                            \
        [](tBaseSerializer& iSerializer, dVersion& iInstance){},            \
        [](tBaseSerializer& iSerializer, dVersion& iInstance){},            \
        nullptr,                                                            \
        theolizer::internal::etmDefault,                                    \
        0                                                                   \
    }

// ***************************************************************************
//      バージョン定義(enum型)
// ***************************************************************************

//----------------------------------------------------------------------------
//      TheolizerVersion<>内の要素(enum値)管理クラス
//          mSymbols
//              mName[0]        当enum値のシンポル名
//              mName[1以上]    当enum値の別名
//          mValues
//              mValues[0]      当enum値の値
//              mValues[1以上]  当enum値の別値
//          mPrevValue          前バージョンの対応する値
//----------------------------------------------------------------------------

template<typename tBaseType>
struct EnumElement : public ElementBase
{
    std::vector<char const*>    mSymbols;
    std::vector<tBaseType>      mValues;
    tBaseType                   mPrevValue;

    EnumElement(
        std::initializer_list<char const*> iSymbols,
        std::initializer_list<tBaseType>   iValues,
        tBaseType                          iPrevValue) :
            mSymbols(iSymbols),
            mValues(iValues),
            mPrevValue(iPrevValue)
    { }

    const bool isSentinel() const {return *mSymbols[0] == 0;}

    // メタ・データ保存
    void writeMetaData
    (
        BaseSerializer& iSerializer,
        VersionNoList const*
    ) const
    {
        // シンボル名リスト
        {
            iSerializer.writePreElement();
            BaseSerializer::AutoRestoreSave aAutoRestoreSave(iSerializer,emOrder,true);
            for (auto symbol : mSymbols)
            {
                iSerializer.writePreElement();
                std::string aSymbol(symbol);
                iSerializer.saveControl(aSymbol);
            }
        }
        // シンボル値リスト
        {
            iSerializer.writePreElement();
            BaseSerializer::AutoRestoreSave aAutoRestoreSave(iSerializer,emOrder,true);
            for (auto value : mValues)
            {
                iSerializer.writePreElement();
                iSerializer.saveControl(value);
            }
        }
        // 次バージョン値
        {
            iSerializer.writePreElement();
            iSerializer.saveControl(mPrevValue);
        }
    }

    // dummy
    Destinations    mDestinations;
    Destinations const& getDestinations() const {return mDestinations;}
};

// ***************************************************************************
//      Non-keep-step用定義
// ***************************************************************************

#define THEOLIZER_INTERNAL_NON_KEEP_STEP(dType)                             \
    TheolizerNonKeepStep<typename std::remove_reference<THEOLIZER_INTERNAL_UNPAREN dType>::type>

// ***************************************************************************
//      バージョン定義(共通)
// ***************************************************************************

//----------------------------------------------------------------------------
//      ClassTypeとEnumTypeのバージョン定義用クラス
//          version.incにて部分特殊化する。
//          tTypeがクラスの場合、ClassType用
//          tTypeがenumの場合、  EnumType用
//----------------------------------------------------------------------------

//      ---<<< TheolizerVersion<>記述マクロ >>>---

// 侵入型(class/struct用)
#define THEOLIZER_INTERNAL_VERTYPE_I(dClass, dVersionNo)                    \
    THEOLIZER_INTERNAL_UNPAREN dClass::TheolizerVersion<tBaseSerializer, dVersionNo>

// 非侵入型(現在はenum用、class/structにも使用可能)
#define THEOLIZER_INTERNAL_VERTYPE_N(dType, dVersionNo)                     \
    TheolizerNonIntrusive<THEOLIZER_INTERNAL_UNPAREN dType>::               \
        TheolizerVersion<tBaseSerializer, dVersionNo>

//----------------------------------------------------------------------------
//      THEOLIZER_GENERATED_BASE_LIST()とTHEOLIZER_GENERATED_ELEMENT_LIST()展開用
//          ユーザ・ソースへ自動生成されたマクロを集約する
//          version*.incにて展開される
//          dAnnoパラメータにつけているTS:_STAFF_ONLY_XXXの'_'はマクロ展開防止用
//----------------------------------------------------------------------------

//      ---<<< THEOLIZER_INTERNAL_UNPARENとstd::remove_reference<>短縮 >>>---

#define THEOLIZER_INTERNAL_UNREFERENCE(dType)                               \
    typename std::remove_reference<THEOLIZER_INTERNAL_UNPAREN dType>::type

//      ---<<< バージョン・アップ／ダウンの足並みを揃えない基底クラス >>>---
//          手動型(非侵入型手動)
//          remove_reference時、typenameが入るとダメなので、
//          THEOLIZER_INTERNAL_UNREFERENCEを使わない。
//          dIdは各TheolzierVersion<>において、基底クラスのバックアップ領域に付ける変数ID

// 継続
#define THEOLIZER_INTERNAL_BASE_N(dSpec, dTrack, dId, dType, dName)         \
    THEOLIZER_INTERNAL_VERSION_B(dSpec, dTrack, dId, dType, 0, dName,       \
        (TheolizerBase<THEOLIZER_INTERNAL_UNPAREN dType>),                  \
        TS:_THEOLIZER_INTERNAL_BASE_N(dSpec,dTrack,dId,dType,dName))

// 次バージョンで削除
#define THEOLIZER_INTERNAL_BASE_N_DEL(dSpec, dTrack, dId, dType, dName)     \
    THEOLIZER_INTERNAL_VERSION_B_DEL(dSpec, dTrack, dId, dType, 0, dName,   \
        (TheolizerBase<THEOLIZER_INTERNAL_UNPAREN dType>),                  \
        TS:_THEOLIZER_INTERNAL_BASE_N(dSpec,dTrack,dId,dType,dName))

//      ---<<< バージョン・アップ／ダウンの足並みを揃える基底クラス >>>---
//          半自動型(侵入型半自動)

// 継続
#define THEOLIZER_INTERNAL_BASE_KI(dSpec, dTrack, dId, dType, dVerNo, dName)\
    THEOLIZER_INTERNAL_VERSION_B(dSpec, dTrack, dId, dType, dVerNo, dName,  \
        (THEOLIZER_INTERNAL_VERTYPE_I(dType, dVerNo)),                      \
        TS:_THEOLIZER_INTERNAL_BASE_KI(dSpec,dTrack,dId,dType,dVerNo,dName))

// 次バージョンで削除
#define THEOLIZER_INTERNAL_BASE_KI_DEL(dSpec, dTrack, dId, dType, dVerNo, dName)\
    THEOLIZER_INTERNAL_VERSION_B_DEL(dSpec, dTrack, dId, dType, dVerNo, dName,\
        (THEOLIZER_INTERNAL_VERTYPE_I(dType, dVerNo)),                      \
        TS:_THEOLIZER_INTERNAL_BASE_KI(dSpec,dTrack,dId,dType,dVerNo,dName))

//      ---<<< バージョン・アップ／ダウンの足並みを揃える基底クラス >>>---
//          完全自動型

// 継続
#define THEOLIZER_INTERNAL_BASE_KN(dSpec, dTrack, dId, dType, dVerNo, dName)\
    THEOLIZER_INTERNAL_VERSION_B(dSpec, dTrack, dId, dType, dVerNo, dName,  \
        (THEOLIZER_INTERNAL_VERTYPE_N(dType, dVerNo)),                      \
        TS:_THEOLIZER_INTERNAL_BASE_KN(dSpec,dTrack,dId,dType,dVerNo,dName))

// 次バージョンで削除
#define THEOLIZER_INTERNAL_BASE_KN_DEL(dSpec, dTrack, dId, dType, dVerNo, dName)\
    THEOLIZER_INTERNAL_VERSION_B_DEL(dSpec, dTrack, dId, dType, dVerNo, dName,\
        (THEOLIZER_INTERNAL_VERTYPE_N(dType, dVerNo)),                      \
        TS:_THEOLIZER_INTERNAL_BASE_KN(dSpec,dTrack,dId,dType,dVerNo,dName))

//      ---<<< バージョン・アップ／ダウンの足並みを揃えない要素 >>>---
//          プリミティブ、ポインタ、手動型

// 継続
#define THEOLIZER_INTERNAL_ELEMENT_N(dName, dNextName, dTrack, dDest, dType)\
    THEOLIZER_INTERNAL_VERSION_EN(dName, dNextName, dTrack, dDest, dType,   \
        (THEOLIZER_INTERNAL_NON_KEEP_STEP(dType)),                          \
        TS:_THEOLIZER_INTERNAL_ELEMENT_N(dName,dNextName,dTrack,\\dDest,\\dType))

// 次バージョンで削除
#define THEOLIZER_INTERNAL_ELEMENT_N_DEL(dName, dNextName, dTrack, dDest, dType)\
    THEOLIZER_INTERNAL_VERSION_EN_DEL(dName, dNextName, dTrack, dDest, dType,\
        (THEOLIZER_INTERNAL_NON_KEEP_STEP(dType)),                          \
        TS:_THEOLIZER_INTERNAL_ELEMENT_N(dName,dNextName,dTrack,\\dDest,\\dType))

//      ---<<< THEOLIZER_INTERNAL_VERSION_EV/_DEL()展開用補助マクロ >>>---

#define THEOLIZER_INTERNAL_NONE(dDef) 
#define THEOLIZER_INTERNAL_DEF(dDef)  THEOLIZER_INTERNAL_UNPAREN dDef

//      ---<<< バージョン・アップ／ダウンの足並みを揃える要素 >>>---
//          侵入型

// 継続
#define THEOLIZER_INTERNAL_ELEMENT_KI(dName, dNextName, dTrack, dDest, dType, dVerNo)\
    THEOLIZER_INTERNAL_VERSION_EV(dName, dNextName, dTrack, dDest, dType, dVerNo,\
        (THEOLIZER_INTERNAL_VERTYPE_I(dType, dVerNo)),                      \
        TS:_THEOLIZER_INTERNAL_ELEMENT_KI(dName,dNextName,dTrack,\\dDest,\\dType,dVerNo), NONE)

// 次バージョンで削除
#define THEOLIZER_INTERNAL_ELEMENT_KI_DEL(dName, dNextName, dTrack, dDest, dType, dVerNo)\
    THEOLIZER_INTERNAL_VERSION_EV_DEL(dName, dNextName, dTrack, dDest, dType, dVerNo,\
        (THEOLIZER_INTERNAL_VERTYPE_I(dType, dVerNo)),                      \
        TS:_THEOLIZER_INTERNAL_ELEMENT_KI(dName,dNextName,dTrack,\\dDest,\\dType,dVerNo))

//      ---<<< バージョン・アップ／ダウンの足並みを揃える要素 >>>---
//          enum型(非侵入型)／完全自動型

// 継続
#define THEOLIZER_INTERNAL_ELEMENT_KN(dName, dNextName, dTrack, dDest, dType, dVerNo)\
    THEOLIZER_INTERNAL_VERSION_EV(dName, dNextName, dTrack, dDest, dType, dVerNo,\
        (THEOLIZER_INTERNAL_VERTYPE_N(dType, dVerNo)),                      \
        TS:_THEOLIZER_INTERNAL_ELEMENT_KN(dName,dNextName,dTrack,\\dDest,\\dType,dVerNo), DEF)

// 次バージョンで削除
#define THEOLIZER_INTERNAL_ELEMENT_KN_DEL(dName, dNextName, dTrack, dDest, dType, dVerNo)\
    THEOLIZER_INTERNAL_VERSION_EV_DEL(dName, dNextName, dTrack, dDest, dType, dVerNo,\
        (THEOLIZER_INTERNAL_VERTYPE_N(dType, dVerNo)),                      \
        TS:_THEOLIZER_INTERNAL_ELEMENT_KN(dName,dNextName,dTrack,\\dDest,\\dType,dVerNo))

//      ---<<< バージョン・アップ／ダウンの足並みを揃えない生配列型要素 >>>---
//          プリミティブ、ポインタ、非侵入型
//          生配列側は次元や要素数変更に対応するため、バージョンを持つ

// 継続
#define THEOLIZER_INTERNAL_ELEMENT_AN(dName, dNextName, dTrack, dDest, dType, ...)  \
    THEOLIZER_INTERNAL_VERSION_EV(dName, dNextName, dTrack, dDest,          \
        (theolizer::internal::Array<THEOLIZER_INTERNAL_UNPAREN dType, __VA_ARGS__>), 0,\
        (typename theolizer::internal::ArrayManager<                        \
            THEOLIZER_INTERNAL_UNPAREN dType&, __VA_ARGS__>::template       \
                TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>),\
        TS:_THEOLIZER_INTERNAL_ELEMENT_AN(dName,dNextName,dTrack,\\dDest,\\dType,__VA_ARGS__),\
        DEF)

// 次バージョンで削除
#define THEOLIZER_INTERNAL_ELEMENT_AN_DEL(dName, dNextName, dTrack, dDest, dType, ...)\
    THEOLIZER_INTERNAL_VERSION_EV_DEL(dName, dNextName, dTrack, dDest,      \
        (theolizer::internal::Array<THEOLIZER_INTERNAL_UNPAREN dType, __VA_ARGS__>), 0,\
        (typename theolizer::internal::ArrayManager<                        \
            THEOLIZER_INTERNAL_UNPAREN dType&, __VA_ARGS__>::template       \
                TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>),\
        TS:_THEOLIZER_INTERNAL_ELEMENT_AN(dName,dNextName,dTrack,\\dDest,\\dType,__VA_ARGS__))

//      ---<<< バージョン・アップ／ダウンの足並みを揃える生配列型要素 >>>---
//          侵入型

// 継続
#define THEOLIZER_INTERNAL_ELEMENT_AKI(dName, dNextName, dTrack, dDest, dType, dVerNo, ...)\
    THEOLIZER_INTERNAL_VERSION_EV(dName, dNextName, dTrack, dDest,          \
        (theolizer::internal::Array<THEOLIZER_INTERNAL_UNPAREN dType, __VA_ARGS__>), dVerNo,\
        (typename theolizer::internal::ArrayManager<                        \
            THEOLIZER_INTERNAL_VERTYPE_I(dType, dVerNo), __VA_ARGS__>::template\
                TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>),\
        TS:_THEOLIZER_INTERNAL_ELEMENT_AKI(dName,dNextName,dTrack,\\dDest,\\dType,dVerNo,__VA_ARGS__), NONE)

// 次バージョンで削除
#define THEOLIZER_INTERNAL_ELEMENT_AKI_DEL(dName, dNextName, dTrack, dDest, dType, dVerNo, ...)\
    THEOLIZER_INTERNAL_VERSION_EV_DEL(dName, dNextName, dTrack, dDest,      \
        (theolizer::internal::Array<THEOLIZER_INTERNAL_UNPAREN dType, __VA_ARGS__>), dVerNo,\
        (typename theolizer::internal::ArrayManager<                        \
            THEOLIZER_INTERNAL_VERTYPE_I(dType, dVerNo), __VA_ARGS__>::template\
                TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>),\
        TS:_THEOLIZER_INTERNAL_ELEMENT_AKI(dName,dNextName,dTrack,\\dDest,\\dType,dVerNo,__VA_ARGS__))

//      ---<<< バージョン・アップ／ダウンの足並みを揃える生配列型要素 >>>---
//          enum型(非侵入型)

// 継続
#define THEOLIZER_INTERNAL_ELEMENT_AKN(dName, dNextName, dTrack, dDest, dType, dVerNo, ...)\
    THEOLIZER_INTERNAL_VERSION_EV(dName, dNextName, dTrack, dDest,          \
        (theolizer::internal::Array<THEOLIZER_INTERNAL_UNPAREN dType, __VA_ARGS__>), dVerNo,\
        (typename theolizer::internal::ArrayManager<                        \
            THEOLIZER_INTERNAL_VERTYPE_N(dType, dVerNo), __VA_ARGS__>::template\
                TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>),\
        TS:_THEOLIZER_INTERNAL_ELEMENT_AKN(dName,dNextName,dTrack,\\dDest,\\dType,dVerNo,__VA_ARGS__), DEF)

// 次バージョンで削除
#define THEOLIZER_INTERNAL_ELEMENT_AKN_DEL(dName, dNextName, dTrack, dDest, dType, dVerNo, ...)\
    THEOLIZER_INTERNAL_VERSION_EV_DEL(dName, dNextName, dTrack, dDest,      \
        (theolizer::internal::Array<THEOLIZER_INTERNAL_UNPAREN dType, __VA_ARGS__>), dVerNo,\
        (typename theolizer::internal::ArrayManager<                        \
            THEOLIZER_INTERNAL_VERTYPE_N(dType, dVerNo), __VA_ARGS__>::template\
                TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>),\
        TS:_THEOLIZER_INTERNAL_ELEMENT_AKN(dName,dNextName,dTrack,\\dDest,\\dType,dVerNo,__VA_ARGS__))

//      ---<<< ポリモーフィズム用(基底クラスのClassTypeInfo抽出) >>>---

#define THEOLIZER_INTERNAL_MAKE_CLASS_TYPE(dType)                           \
    theolizer::internal::ClassTypeInfo                                      \
    <                                                                       \
        typename std::conditional                                           \
        <                                                                   \
            theolizer::internal::IsNonIntrusive<THEOLIZER_INTERNAL_UNPAREN dType>::value,\
            TheolizerNonIntrusive<THEOLIZER_INTERNAL_UNPAREN dType>,        \
            THEOLIZER_INTERNAL_UNPAREN dType                                \
        >::type                                                             \
    >

//      ---<<< TheolizerSucceedフラグ展開 >>>---

#define THEOLIZER_INTERNAL_SUCCEED_FLG(dName)                               \
    THEOLIZER_INTERNAL_SUCCEED_FLG_I(THEOLIZER_INTERNAL_FIRST dName)
#define THEOLIZER_INTERNAL_SUCCEED_FLG_I(dName)                             \
    THEOLIZER_INTERNAL_CAT(dName, TheolizerSucceed)

//############################################################################
//      ポリモーフィズム対応
//############################################################################

// ***************************************************************************
//      基底Serializer × 派生クラスの組合せてのClassTypeInfoを実体化する
// ***************************************************************************

template<class tClass>
class RegisterToBaseClass
{
    RegisterToBaseClass()
    {
        theolizer::internal::RegisterType
        <
            theolizer::internal::BaseSerializer,
            tClass,
            ::tTheolizerVersion,
            true
        >::getInstance();
    }
};

// ***************************************************************************
//      ClassTypeInfoからのBaseSerializer呼び出し
// ***************************************************************************

//----------------------------------------------------------------------------
//      型、および、インスタンス保存(保存したらtrue返却)
//----------------------------------------------------------------------------

template<class tClassType>
bool ClassTypeInfo<tClassType>::saveTypeInstance
(
    BaseSerializer& iSerializer,
    tClassType*& iPointer,
    std::type_index iStdTypeIndex
)
{
    unsigned aVersionNo=iSerializer.mVersionNoList.at(BaseTypeInfo::mTypeIndex);

    if (getTargetStdTypeIndex() == iStdTypeIndex)
    {
        if ((iSerializer.mCheckMode != CheckMode::TypeCheckByIndex)
         && (iSerializer.mCheckMode != CheckMode::InMemory))
        {
            // 型名保存
            std::string aTypeName=getTypeName(iSerializer.mVersionNoList);
            iSerializer.saveControl(aTypeName);
        }
        else
        {
            // TypeIndex保存
            iSerializer.saveControl(BaseTypeInfo::mTypeIndex);
        }
        // インスタンス保存
        iSerializer.writePreElement();

        tClassType::Theolizer::saveClass
        (
            iSerializer,
            iPointer,
            aVersionNo
        );
return true;
    }

    for (auto&& aDrivedClass : mDrivedClassList)
    {
        if (aDrivedClass->saveTypeInstance(iSerializer, iPointer, iStdTypeIndex))
return true;
    }
    return false;
}

//----------------------------------------------------------------------------
//      デフォルト・コンストラクタがある時のみコンストラクトする
//----------------------------------------------------------------------------

template
<
    class tClassType,
    THEOLIZER_INTERNAL_OVERLOAD((!std::is_constructible<tClassType>::value))
>
tClassType* createClass(tClassType const*)
{
    return nullptr;
}

template
<
    class tClassType,
    THEOLIZER_INTERNAL_OVERLOAD((std::is_constructible<tClassType>::value))
>
tClassType* createClass(tClassType const*)
{
    return new tClassType();
}
//----------------------------------------------------------------------------
template
<
    class tClassType,
    THEOLIZER_INTERNAL_OVERLOAD((!std::is_constructible<tClassType>::value))
>
tClassType* createClass2()
{
    return nullptr;
}

template
<
    class tClassType,
    THEOLIZER_INTERNAL_OVERLOAD((std::is_constructible<tClassType>::value))
>
tClassType* createClass2()
{
    return new tClassType();
}

//----------------------------------------------------------------------------
//      型、および、インスタンス回復(回復したらtrue返却)
//----------------------------------------------------------------------------

template<class tClassType>
bool ClassTypeInfo<tClassType>::loadTypeInstance
(
    BaseSerializer& iSerializer,
    tClassType*& iPointer,
    TypeIndexList& iTypeIndexList
)
{
    typedef typename tClassType::TheolizerTarget    TheolizerTarget;

    std::size_t aFoundTypeIndex=0;
    bool aFound=false;
    bool aDoRelease=true;
    for (auto aTypeIndex : iTypeIndexList)
    {
        if (aTypeIndex != BaseTypeInfo::mTypeIndex)
    continue;

        if (!aFound)
        {
            aFound=true;
            aFoundTypeIndex=aTypeIndex;
        }

        // ポイント先領域のstd::type_indexと、シリアライズ・データ中のstd::type_indexが一致
        if ((iPointer != nullptr)
         && (std::type_index(typeid(*iPointer))
          == TypeInfoList::getInstance().getList()[aTypeIndex]->getStdTypeIndex(true)))
        {   // この場合のみ領域開放しない
            aDoRelease=false;
    break;
        }
    }

    if (aFound)
    {
        // 参照経由で処理中の時、アドレス変更不可
        if (iSerializer.mRefProcessing)
        {
            if (aDoRelease)
            {
                THEOLIZER_INTERNAL_WRONG_USING(
                    "Type of this instance(%1%) is different from saved.",
                    getNameByTypeInfo(typeid(*iPointer)));
            }
        }
        else
        {
            if (aDoRelease)
            {
                // シリアライズ・データとポインタ指す先の型が異なる場合、解放する
                //  ただし、std::shared_ptrが管理する領域の場合は、std::shared_ptr側で解放するので
                //  ここでは解放しない。
                if (!iSerializer.mIsShared)
                {
                    delete iPointer;
                }
                iPointer=nullptr;
            }

            // 自動型で、nullptrなら領域獲得
            if (tClassType::Theolizer::kIsAuto)
            {
                if (!iPointer)
                {
                    iPointer=reinterpret_cast<tClassType*>(createClass2<TheolizerTarget>());
                }
            }
        }

        // インスタンス回復
        if (!iSerializer.readPreElement())
        {
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
        }

        {
            // shared_ptr処理クリア
            AutoRestoreIsShared aAutoRestoreIsShared(iSerializer, false);

            // クラス・インスタンス回復
            tClassType::Theolizer::loadClass
            (
                iSerializer,
                iPointer,
                iSerializer.mVersionNoList.at(aFoundTypeIndex)
            );
        }
return true;
    }

    for (auto&& aDrivedClass : mDrivedClassList)
    {
        if (aDrivedClass->loadTypeInstance(iSerializer, iPointer, iTypeIndexList))
return true;
    }

    return false;
}

//----------------------------------------------------------------------------
//      現インスタンスの先頭アドレス返却
//          ２番目以降の基底クラスへのポインタに対応するため
//----------------------------------------------------------------------------

template<class tClassType>
void const*  ClassTypeInfo<tClassType>::getDerivedPointer
(
    typename tClassType::TheolizerTarget* iBasePointer
)
{
    if (getTargetStdTypeIndex() == std::type_index(typeid(*iBasePointer)))
return iBasePointer;

    for (auto&& aDrivedClass : mDrivedClassList)
    {
        void const* ret=aDrivedClass->getDerivedPointer(iBasePointer);
        if (ret)
return ret;
    }

    return nullptr;
}

//############################################################################
//      End
//############################################################################

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal
}   // namespace theolizer

#endif  // THEOLIZER_INTERNAL_CORE_H
