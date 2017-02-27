//############################################################################
//      EnumType用TheolizerVersion<>定義ファイル
//          適切なマクロを定義してversion_enum.incをインクルードすることで、
//          EnumTypeのバージョン制御に必要なコードを生成する。
//
//          下記マクロがパラメータとなる。
//              THEOLIZER_GENERATED_NO_COMPILE       未使用時定義
//              THEOLIZER_GENERATED_FULL_AUTO        完全自動型の時定義
//              THEOLIZER_GENERATED_ENUM_TYPE        ターゲットのEnumType
//              THEOLIZER_GENERATED_LAST_VERSION_NO  最新版のバージョン番号
//
//              THEOLIZER_GENERATED_VERSION_NO       EnumVersion<>のバージョン番号
//              THEOLIZER_GENERATED_ENUM_NAME        このバージョン時のEnumType名
//              THEOLIZER_GENERATED_BASE_TYPE        このバージョン時の基底型
//              THEOLIZER_GENERATED_DEFAULT_VALUE    デフォルト値
//              THEOLIZER_GENERATED_SAVE_TYPE        保存方法指定
//              THEOLIZER_GENERATED_ENUM_LIST        enum値のリスト
//          なお、各enum値はenumシンボルではなく数値で定義する。
//              旧バージョンのEnumTypeを保存しないので、
//              旧バージョンについては数値化することが必須だから。
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

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable:4100 4127)
#endif

// ***************************************************************************
//      TheolizerNonIntrusive型を定義
// ***************************************************************************

#define THEOLIZER_INTERNAL_DERIVED TheolizerNonIntrusive<THEOLIZER_GENERATED_ENUM_TYPE>

// ***************************************************************************
//      下記マクロの定義位置をドライパーが把握できるようにする準備
//          THEOLIZER_GENERATED_LAST_VERSION_NO
//          THEOLIZER_GENERATED_VERSION_NO
// ***************************************************************************

#define THEOLIZER_INTERNAL_DEFINE(dVar, dVerNo)    dVerNo

#if (THEOLIZER_GENERATED_VERSION_NO==0) || (THEOLIZER_GENERATED_LAST_VERSION_NO==0)
#error "Illigal THEOLIZER_GENERATED_VERSION_NO or THEOLIZER_GENERATED_LAST_VERSION_NO."
#endif

// ***************************************************************************
//      完全自動型対応
// ***************************************************************************

#if (THEOLIZER_GENERATED_VERSION_NO == THEOLIZER_GENERATED_LAST_VERSION_NO)
    #if defined(THEOLIZER_GENERATED_FULL_AUTO)
        THEOLIZER_INTERNAL_ENUM_FULL_AUTO(THEOLIZER_GENERATED_FULL_AUTO);
        #define THEOLIZER_GENERATED_ENUM_TYPE  THEOLIZER_GENERATED_FULL_AUTO
        #define THEOLIZER_INTERNAL_IS_FULL_AUTO true
    #else
        #define THEOLIZER_INTERNAL_IS_FULL_AUTO false
    #endif
#else   // THEOLIZER_GENERATED_LAST_VERSION_NO不正修正への対処(気がついたので実施)
    #if !defined(THEOLIZER_INTERNAL_IS_FULL_AUTO)
        #define THEOLIZER_INTERNAL_IS_FULL_AUTO false
    #endif
#endif

// ***************************************************************************
//      コンパイル不要時処理
// ***************************************************************************

#if defined(THEOLIZER_INTERNAL_DRIVER) || !defined(THEOLIZER_GENERATED_NO_COMPILE)

// ***************************************************************************
//      EnumType用TheolizerVersion<>部分特殊化定義
// ***************************************************************************

template<class tBaseSerializer>
struct THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
<
    tBaseSerializer,
    THEOLIZER_GENERATED_VERSION_NO
>
{
//----------------------------------------------------------------------------
//      保存／回復処理制御用メンバー群
//----------------------------------------------------------------------------

//      ---<<< 基本情報 >>>---

    typedef void TheolizerEnumVersion;
    typedef THEOLIZER_GENERATED_ENUM_TYPE  TheolizerTarget;
    typedef TheolizerNonIntrusive<THEOLIZER_GENERATED_ENUM_TYPE> TheolizerClass;

    typedef THEOLIZER_GENERATED_BASE_TYPE  BaseType;
    struct Theolizer
    {
        static const bool kIsFullAuto=THEOLIZER_INTERNAL_IS_FULL_AUTO;

        // ドライバへの定義位置通知用(dVarの定義位置が取得できる)
        #undef  THEOLIZER_INTERNAL_DEFINE
        #define THEOLIZER_INTERNAL_DEFINE(dVar, dVerNo) static const unsigned dVar=dVerNo;
        THEOLIZER_GENERATED_VERSION_NO
        THEOLIZER_GENERATED_LAST_VERSION_NO
        #undef  THEOLIZER_INTERNAL_DEFINE
        #define THEOLIZER_INTERNAL_DEFINE(dVar, dVerNo) dVerNo

        static const bool kIsVersion=true;
        static const bool kIsEnum=true;

        // ドライバ解析用(enum型名)
        #ifdef THEOLIZER_INTERNAL_DRIVER
            THEOLIZER_ANNOTATE(TS:THEOLIZER_GENERATED_ENUM_NAME)
        #endif
        static const theolizer::internal::EnumSaveType kEnumSaveType=
            theolizer::internal::THEOLIZER_GENERATED_SAVE_TYPE;

        // ドライバ解析用(要素情報)
        #ifdef THEOLIZER_INTERNAL_DRIVER
            THEOLIZER_ANNOTATE(TS:THEOLIZER_GENERATED_ENUM_LIST())
        #endif
        static const BaseType kDefaultValue=THEOLIZER_GENERATED_DEFAULT_VALUE;
    };

    // 親クラス定義(最新版か古い版かで切り替える)
    #if (THEOLIZER_GENERATED_VERSION_NO == THEOLIZER_GENERATED_LAST_VERSION_NO)
    typedef THEOLIZER_GENERATED_ENUM_TYPE   ParentVersion;
    #else
    typedef THEOLIZER_INTERNAL_DERIVED::
        TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO+1> ParentVersion;
    typedef typename ParentVersion::BaseType    ParentBaseType;
    #endif

    // ターゲットへのポインタ
    THEOLIZER_GENERATED_ENUM_TYPE* mTarget;

    // 親(ターゲット、もしくは、次バージョン)インスタンスへのポインタ
    ParentVersion*                  mParentVersion;

//      ---<<< 保存／回復関数 >>>---

    void saveEnum(tBaseSerializer& iSerializer, unsigned iVersionNo);
    void loadEnum(tBaseSerializer& iSerializer, unsigned iVersionNo);

//      ---<<< 型チェック用 >>>---

    static char const* getEnumName(unsigned iVersionNo);
    static char const* getEnumName() {return THEOLIZER_GENERATED_ENUM_NAME;}

    // 要素数返却
    static size_t getElementCount()
    {
        static bool wsInitialzied=false;
        static size_t  wsElementCount;
        if (!wsInitialzied)
        {
            wsInitialzied=true;
            for (wsElementCount=0;
                 !kEnumElementList[wsElementCount].isSentinel();
                 ++wsElementCount)
                ;
        }
        return wsElementCount;
    }

    // Enumシンボル枚挙用Range返却
    static theolizer::internal::ElementRange getElementRange(unsigned iVersionNo);

    // enum種別返却
    static unsigned getTypeFlags(unsigned iVersionNo);

//----------------------------------------------------------------------------
//      インスタンス実体
//----------------------------------------------------------------------------

    // 実体
    BaseType        mValue;

    // 実体の参照(同じバージョンの時は親インスタンスのmValueへの参照となる)
    BaseType&       mValueRef;

    // 回復処理中フラグ(これがfalseならupVersion()を呼ばない)
    bool            mIsLoad;

//      ---<<< 中身へのアクセス用 >>>---

    typedef BaseType AsElementOfArray;
    AsElementOfArray& getAccess()
    {
        return mValueRef;
    }

//      ---<<< initialize(), up/downVersion()関数内でのアクセス用 >>>---

    #define THEOLIZER_INTERNAL_COMPARE(dCmp)                            \
        bool operator dCmp(BaseType iRhs) const { return mValue dCmp iRhs; }
        THEOLIZER_INTERNAL_COMPARE(==)
        THEOLIZER_INTERNAL_COMPARE(!=)
        THEOLIZER_INTERNAL_COMPARE(<)
        THEOLIZER_INTERNAL_COMPARE(>)
        THEOLIZER_INTERNAL_COMPARE(<=)
        THEOLIZER_INTERNAL_COMPARE(>=)
    #undef  THEOLIZER_INTERNAL_COMPARE

    BaseType& operator=(BaseType iRhs) { mValue=iRhs; return mValue; }
    operator BaseType() { return mValue; }

//----------------------------------------------------------------------------
//      コンストラクタ(次バージョンからコンストラクト)
//          古い版からの場合は、TheolizerVersion<>なのでconstを付けて良い。
//          配列でイニシャライザ・リストでの初期化時、必要になる。
//----------------------------------------------------------------------------

#if (THEOLIZER_GENERATED_VERSION_NO == THEOLIZER_GENERATED_LAST_VERSION_NO)
    TheolizerVersion(ParentVersion& iParentVersion) :
        mTarget(&iParentVersion),
        mParentVersion(&iParentVersion),
        mValue(static_cast<BaseType>(iParentVersion)),
        mValueRef(mValue),
        mIsLoad(false)
    {
        // 内部的にstd::stringを使うので登録する
        theolizer::internal::RegisterType
        <
            tBaseSerializer,
            std::string
        >::getInstance();
    }
#else
    TheolizerVersion(ParentVersion & iParentVersion) :
        mTarget(iParentVersion.mTarget),
        mParentVersion(&iParentVersion),
        mValue(THEOLIZER_GENERATED_DEFAULT_VALUE),
        mValueRef(mValue),
        mIsLoad(false)
    {   // 最新版でない時のみバージョン・ダウン実施
        downVersion();
    }
#endif

//----------------------------------------------------------------------------
//      コンストラクタ(同じバージョンからコンストラクト)
//----------------------------------------------------------------------------

    TheolizerVersion(TheolizerVersion& iSameVersion) :
        mTarget(iSameVersion.mTarget),
        mParentVersion(nullptr),
        mValue(0),
        mValueRef(iSameVersion.mValueRef),
        mIsLoad(false)
    {
    }

//----------------------------------------------------------------------------
//      ムーブ・コンストラクタ(同じバージョンからコンストラクト)
//          次バージョンで削除されたもの対応で、生成された一時オブジェクトを引き継ぐ
//          生配列の次元数／要素数変更対応に必要
//----------------------------------------------------------------------------

    TheolizerVersion(TheolizerVersion&& iSameVersion) :
        mTarget(iSameVersion.mTarget),
        mParentVersion(nullptr),
        mValue(iSameVersion.mValueRef),
        mValueRef(mValue),
        mIsLoad(false)
    {
    }

//----------------------------------------------------------------------------
//      コンストラクタ(親無し)
//----------------------------------------------------------------------------

    TheolizerVersion() :
        mTarget(nullptr),
        mParentVersion(nullptr),
        mValue(THEOLIZER_GENERATED_DEFAULT_VALUE),
        mValueRef(mValue),
        mIsLoad(false)
    {
    }

//----------------------------------------------------------------------------
//      デストラクタ
//----------------------------------------------------------------------------

    ~TheolizerVersion()
    {   // 最新版でない時のみバージョン・アップ実施
        #if (THEOLIZER_GENERATED_VERSION_NO == THEOLIZER_GENERATED_LAST_VERSION_NO)
        if (mParentVersion) {
            *mParentVersion = static_cast<THEOLIZER_GENERATED_ENUM_TYPE>(mValueRef);
        }
        #else
        if (mParentVersion && mIsLoad) {
            upVersion();
        }
        #endif
    }

//----------------------------------------------------------------------------
//      コピー演算子(データのみコピーする。管理情報は対象外)
//----------------------------------------------------------------------------

    TheolizerVersion& operator=(TheolizerVersion const& iRhs)
    {
        if (&iRhs != this) mValue=iRhs.mValue;
        return *this;
    }

//----------------------------------------------------------------------------
//      バージョン・ダウン処理
//----------------------------------------------------------------------------

    #if (THEOLIZER_GENERATED_VERSION_NO != THEOLIZER_GENERATED_LAST_VERSION_NO)
    void downVersion()
    {
        // 見つからない時はユーザの定義ミス→保存時なのでエラーとする
        for (size_t i=0; !mParentVersion->kEnumElementList[i].isSentinel(); ++i)
        {
            if (mParentVersion->kEnumElementList[i].mValues[0] == mParentVersion->mValueRef)
            {
                mValueRef=mParentVersion->kEnumElementList[i].mPrevValue;
    return;
            }
        }

        mValueRef=THEOLIZER_GENERATED_DEFAULT_VALUE;
        THEOLIZER_INTERNAL_WRONG_USING(
            "Can not transform Version.%1%'s value(%2%) to Version.%3%'s value. "
            "Please check THEOLIZER_ANNOTATE() for %4%.",
            THEOLIZER_GENERATED_VERSION_NO+1,
            mParentVersion->mValueRef,
            THEOLIZER_GENERATED_VERSION_NO,
            THEOLIZER_GENERATED_ENUM_NAME);
    }
    #endif

//----------------------------------------------------------------------------
//      バージョン・アップ処理
//----------------------------------------------------------------------------

    #if (THEOLIZER_GENERATED_VERSION_NO != THEOLIZER_GENERATED_LAST_VERSION_NO)
    void upVersion()
    {
        // 見つからない時はユーザの定義ミス→できるだけ回復したいので警告とする
        for (size_t i=0; !mParentVersion->kEnumElementList[i].isSentinel(); ++i)
        {
            if (mValueRef == mParentVersion->kEnumElementList[i].mPrevValue)
            {
                mParentVersion->mValueRef=mParentVersion->kEnumElementList[i].mValues[0];
    return;
            }
        }

        mParentVersion->mValueRef=THEOLIZER_GENERATED_DEFAULT_VALUE;
        THEOLIZER_INTERNAL_WARNING(
            "Can not transform Version.%1%'s value(%2%) to Version.%3%'s value. "
            "Please check THEOLIZER_ANNOTATE() for %4%.",
            THEOLIZER_GENERATED_VERSION_NO,
            mValueRef,
            (THEOLIZER_GENERATED_VERSION_NO+1),
            THEOLIZER_GENERATED_ENUM_NAME);
    }
    #endif

//----------------------------------------------------------------------------
//      保存処理
//----------------------------------------------------------------------------

    void saveEnumImpl(tBaseSerializer& iSerializer)
    {
        // 値を保存する時の処理
        if (Theolizer::kEnumSaveType == theolizer::internal::estValue)
        {
            THEOLIZER_INTERNAL_SAVE(iSerializer, mValueRef, etmDefault);
    return;
        }

        // 名前を保存する時の処理
        for (size_t i=0; !kEnumElementList[i].isSentinel(); ++i)
        {
            if (kEnumElementList[i].mValues[0] == mValueRef)
            {
                std::string aEnumSymbol(kEnumElementList[i].mSymbols[0]);
                THEOLIZER_INTERNAL_SAVE(iSerializer, aEnumSymbol, etmDefault);
    return;
            }
        }

        THEOLIZER_INTERNAL_ERROR(
            "Can not find enum symbol's value(%1%) at Version.%2%. "
            "Please check THEOLIZER_ANNOTATE() for %3%.",
            mValueRef,
            THEOLIZER_GENERATED_VERSION_NO,
            THEOLIZER_GENERATED_ENUM_NAME);
    }

//----------------------------------------------------------------------------
//      回復処理
//----------------------------------------------------------------------------

    void loadEnumImpl(tBaseSerializer& iSerializer)
    {
        // 値で回復する時の処理
        if (Theolizer::kEnumSaveType == theolizer::internal::estValue)
        {
            THEOLIZER_INTERNAL_LOAD(iSerializer, mValueRef, etmDefault);
            for (size_t i=0; !kEnumElementList[i].isSentinel(); ++i)
            {
                for (size_t j=0; j < kEnumElementList[i].mValues.size(); ++j)
                {
                    if (mValueRef == kEnumElementList[i].mValues[j])
                    {
                        mValueRef=kEnumElementList[i].mValues[0];
    return;
                    }
                }
            }

        mValueRef=THEOLIZER_GENERATED_DEFAULT_VALUE;
        THEOLIZER_INTERNAL_WARNING(
            u8"Can not find loaded value(%1%) in Version.%2%'s symbol. "
            "Please check THEOLIZER_ANNOTATE() for %3%.",
            mValueRef,
            THEOLIZER_GENERATED_VERSION_NO,
            THEOLIZER_GENERATED_ENUM_NAME);
    return;
        }

        // 名前で回復する時の処理(見つからない時はエラー)
        std::string  aName;
        THEOLIZER_INTERNAL_LOAD(iSerializer, aName, etmDefault);

        mValueRef=THEOLIZER_GENERATED_DEFAULT_VALUE;
        for (size_t i=0; !kEnumElementList[i].isSentinel(); ++i)
        {
            for (size_t j=0; j < kEnumElementList[i].mSymbols.size(); ++j)
            {
                if (aName == kEnumElementList[i].mSymbols[j])
                {
                    mValueRef=kEnumElementList[i].mValues[0];
    return;
                }
            }
        }

        THEOLIZER_INTERNAL_WARNING(
            u8"Can not find loaded symbol(%1%) in Version.%2%'s symbol. "
            "Please check THEOLIZER_ANNOTATE() for %3%.",
            aName,
            THEOLIZER_GENERATED_VERSION_NO,
            THEOLIZER_GENERATED_ENUM_NAME);
    }

//----------------------------------------------------------------------------
//      enum種別返却
//----------------------------------------------------------------------------

    static unsigned getTypeFlags()
    {
        unsigned    ret=0;

        // signed/unsigned
        if (!std::is_signed<BaseType>::value) {
            ret |= theolizer::internal::eefUnsigned;
        }

        // scoped/unscoped
        if (!std::is_convertible<THEOLIZER_GENERATED_ENUM_TYPE, BaseType>::value) {
            ret |= theolizer::internal::eefScoped;
        }

        // Value/Name
        if (Theolizer::kEnumSaveType == theolizer::internal::estValue) {
            ret |= theolizer::internal::eefValue;
        }

        return ret;
    }

//----------------------------------------------------------------------------
//      Enum値リスト
//----------------------------------------------------------------------------

    static const theolizer::internal::EnumElement<BaseType>   kEnumElementList[];

//      ---<<< 枚挙用Range返却 >>>---

    static theolizer::internal::ElementRange getElementRange()
    {
        typedef const theolizer::internal::EnumElement
            <
                BaseType
            >* Iterator;

        return  theolizer::internal::ElementRange
                (
                    std::move(Iterator(&kEnumElementList[0])),
                    std::move(Iterator(&kEnumElementList[getElementCount()])),
                    getElementCount()
                );
    }
};

template<class tBaseSerializer>
const theolizer::internal::EnumElement<THEOLIZER_GENERATED_BASE_TYPE>
THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
<
    tBaseSerializer,
    THEOLIZER_GENERATED_VERSION_NO
>::kEnumElementList[]=
{
    #define THEOLIZER_GENERATED_ELEMENT(dSymbols, dValues, dPrevValue)      \
        theolizer::internal::EnumElement<BaseType>(                         \
            {THEOLIZER_INTERNAL_UNPAREN dSymbols},                          \
            {THEOLIZER_INTERNAL_UNPAREN dValues}, dPrevValue),
    THEOLIZER_GENERATED_ENUM_LIST()
    theolizer::internal::EnumElement<BaseType>({""}, {0}, 0)
    #undef  THEOLIZER_GENERATED_ELEMENT
};

// ***************************************************************************
//      TheolizerVersion<n!=1>用サービス関数定義
//      (THEOLIZER_GENERATED_VERSION_NO+1について定義する)
//          save/loadEnum()
//          getEnumName()
// ***************************************************************************

// 最新版以外の時定義する
#if (THEOLIZER_GENERATED_VERSION_NO != THEOLIZER_GENERATED_LAST_VERSION_NO)

//      ---<<< 保存 >>>---

template<class tBaseSerializer>
void THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
<
    tBaseSerializer,
    THEOLIZER_GENERATED_VERSION_NO+1
>::saveEnum(tBaseSerializer& iSerializer, unsigned iVersionNo)
{
//  typedef THEOLIZER_INTERNAL_DERIVED::
//      TheolizerVersion<tBaseSerializer,THEOLIZER_GENERATED_VERSION_NO+1> This;
    if (iVersionNo < (THEOLIZER_GENERATED_VERSION_NO+1)) {
        THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
        <
            tBaseSerializer,
            THEOLIZER_GENERATED_VERSION_NO
        > aVersion(*this);
        aVersion.saveEnum(iSerializer, iVersionNo);
return;
    } else {
        saveEnumImpl(iSerializer);
    }
}

//      ---<<< 回復 >>>---

template<class tBaseSerializer>
void THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
<
    tBaseSerializer,
    THEOLIZER_GENERATED_VERSION_NO+1
>::loadEnum(tBaseSerializer& iSerializer, unsigned iVersionNo)
{
    mIsLoad=true;   // デストラクト時upVersionを呼ぶため
    if (iVersionNo < (THEOLIZER_GENERATED_VERSION_NO+1)) {
        THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
        <
            tBaseSerializer,
            THEOLIZER_GENERATED_VERSION_NO
        > aVersion(*this);
        aVersion.loadEnum(iSerializer, iVersionNo);
return;
    } else {
        loadEnumImpl(iSerializer);
    }
}

//      ---<<< クラス名返却 >>>---

template<class tBaseSerializer>
char const* THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
<
    tBaseSerializer,
    THEOLIZER_GENERATED_VERSION_NO+1
>::getEnumName(unsigned iVersionNo)
{
    if (iVersionNo < (THEOLIZER_GENERATED_VERSION_NO+1)) {
        return THEOLIZER_INTERNAL_DERIVED::
            TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>::
                getEnumName(iVersionNo);
    }

    return getEnumName();
}

//      ---<<< Enumシンボル枚挙用Range返却 >>>---

template<class tBaseSerializer>
theolizer::internal::ElementRange
THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
<
    tBaseSerializer,
    THEOLIZER_GENERATED_VERSION_NO+1
>::getElementRange(unsigned iVersionNo)
{
    if (iVersionNo < (THEOLIZER_GENERATED_VERSION_NO+1))
    {
        return THEOLIZER_INTERNAL_DERIVED::
            TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>::
                getElementRange(iVersionNo);
    }

    return getElementRange();
}

//      ---<<< Enum種別返却 >>>---

template<class tBaseSerializer>
unsigned
THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
<
    tBaseSerializer,
    THEOLIZER_GENERATED_VERSION_NO+1
>::getTypeFlags(unsigned iVersionNo)
{
    if (iVersionNo < (THEOLIZER_GENERATED_VERSION_NO+1))
    {
        return THEOLIZER_INTERNAL_DERIVED::
            TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>::
                getTypeFlags(iVersionNo);
    }

    return  getTypeFlags();
}

#endif

// ***************************************************************************
//      TheolizerVersion<1>用サービス関数定義
//          save/loadEnum()
//          getEnumName()
// ***************************************************************************

#if (THEOLIZER_GENERATED_VERSION_NO == 1)

//      ---<<< 保存 >>>---

template<class tBaseSerializer>
void THEOLIZER_INTERNAL_DERIVED::TheolizerVersion<tBaseSerializer, 1>::saveEnum
(
    tBaseSerializer& iSerializer, unsigned iVersionNo
)
{
    saveEnumImpl(iSerializer);
}

//      ---<<< 回復 >>>---

template<class tBaseSerializer>
void THEOLIZER_INTERNAL_DERIVED::TheolizerVersion<tBaseSerializer, 1>::loadEnum
(
    tBaseSerializer& iSerializer,
    unsigned iVersionNo
)
{
    mIsLoad=true;   // デストラクト時upVersionを呼ぶため
    loadEnumImpl(iSerializer);
}

//      ---<<< クラス名返却 >>>---

template<class tBaseSerializer>
char const* THEOLIZER_INTERNAL_DERIVED::TheolizerVersion<tBaseSerializer, 1>::getEnumName
(
    unsigned iVersionNo
)
{
    return getEnumName();
}

//      ---<<< Enumシンボル枚挙用Range返却 >>>---

template<class tBaseSerializer>
theolizer::internal::ElementRange
THEOLIZER_INTERNAL_DERIVED::TheolizerVersion<tBaseSerializer, 1>::getElementRange
(
    unsigned iVersionNo
)
{
    return getElementRange();
}

//      ---<<< Enum種別返却 >>>---

template<class tBaseSerializer>
unsigned THEOLIZER_INTERNAL_DERIVED::TheolizerVersion<tBaseSerializer, 1>::getTypeFlags
(
    unsigned iVersionNo
)
{
    return getTypeFlags();
}

#endif

// ***************************************************************************
//      サービス関数定義
//          save/loadEnum()
//          getEnumName()
// ***************************************************************************

#if (THEOLIZER_GENERATED_VERSION_NO == 1)

//      ---<<< 保存 >>>---

inline void THEOLIZER_INTERNAL_DERIVED::Theolizer::saveEnum
(
    theolizer::internal::BaseSerializer& iSerializer,
    THEOLIZER_GENERATED_ENUM_TYPE& iInstance,
    unsigned iVersionNo
)
{
    THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
    <
        theolizer::internal::BaseSerializer,
        THEOLIZER_GENERATED_LAST_VERSION_NO
    > aVersion(iInstance);
    aVersion.saveEnum(iSerializer, iVersionNo);
}

//      ---<<< 回復 >>>---

inline void THEOLIZER_INTERNAL_DERIVED::Theolizer::loadEnum
(
    theolizer::internal::BaseSerializer& iSerializer,
    THEOLIZER_GENERATED_ENUM_TYPE& iInstance,
    unsigned iVersionNo
)
{
    THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
    <
        theolizer::internal::BaseSerializer,
        THEOLIZER_GENERATED_LAST_VERSION_NO
    >   aVersion(iInstance);
    aVersion.loadEnum(iSerializer, iVersionNo);
}

//      ---<<< クラス名返却 >>>---

inline char const* THEOLIZER_INTERNAL_DERIVED::Theolizer::getEnumName(unsigned iVersionNo)
{
    return THEOLIZER_INTERNAL_DERIVED::
        TheolizerVersion
        <
            theolizer::internal::BaseSerializer,
            THEOLIZER_GENERATED_LAST_VERSION_NO
        >::getEnumName(iVersionNo);
}

//      ---<<< Enumシンボル枚挙用Range返却 >>>---

inline theolizer::internal::ElementRange
THEOLIZER_INTERNAL_DERIVED::Theolizer::getElementRange(unsigned iVersionNo)
{
    return THEOLIZER_INTERNAL_DERIVED::
        TheolizerVersion
        <
            theolizer::internal::BaseSerializer,
            THEOLIZER_GENERATED_LAST_VERSION_NO
        >::getElementRange(iVersionNo);
}

//      ---<<< Enum種別返却 >>>---

inline unsigned THEOLIZER_INTERNAL_DERIVED::Theolizer::getTypeFlags(unsigned iVersionNo)
{
    return THEOLIZER_INTERNAL_DERIVED::
        TheolizerVersion
        <
            theolizer::internal::BaseSerializer,
            THEOLIZER_GENERATED_LAST_VERSION_NO
        >::getTypeFlags(iVersionNo);
}

#endif  // THEOLIZER_GENERATED_VERSION_NO
 
#endif  // THEOLIZER_INTERNAL_DRIVER and THEOLIZER_GENERATED_NO_COMPILE

// ***************************************************************************
//      後始末
// ***************************************************************************

#if (THEOLIZER_GENERATED_VERSION_NO == 1)
    #if defined(THEOLIZER_GENERATED_NO_COMPILE)
        #undef THEOLIZER_GENERATED_NO_COMPILE
    #endif
    #if defined(THEOLIZER_GENERATED_FULL_AUTO)
        #undef THEOLIZER_GENERATED_FULL_AUTO
    #endif
    #undef THEOLIZER_GENERATED_LAST_VERSION_NO
    #undef THEOLIZER_GENERATED_ENUM_TYPE
    #undef THEOLIZER_INTERNAL_IS_FULL_AUTO
#endif

#undef  THEOLIZER_GENERATED_ENUM_NAME
#undef  THEOLIZER_GENERATED_BASE_TYPE
#undef  THEOLIZER_GENERATED_DEFAULT_VALUE
#undef  THEOLIZER_GENERATED_SAVE_TYPE
#undef  THEOLIZER_GENERATED_ENUM_LIST

#undef  THEOLIZER_INTERNAL_DERIVED

#undef  THEOLIZER_INTERNAL_DEFINE

// ***************************************************************************
//          警告抑止解除
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(pop)
#endif
