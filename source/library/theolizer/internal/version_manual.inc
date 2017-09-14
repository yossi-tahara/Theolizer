//############################################################################
//      ClassTypeの手動型用TheolizerVersion<>定義ファイル
//          適切なマクロを定義してversion.incをインクルードすることで、
//          ClassTypeのバージョン制御に必要なコードを生成する。
//
//          下記マクロがパラメータとなる。
//              THEOLIZER_GENERATED_NO_COMPILE       未使用時定義
//              THEOLIZER_GENERATED_CLASS_TYPE       ターゲット・クラス
//              THEOLIZER_GENERATED_LAST_VERSION_NO  最新版のバージョン番号
//              THEOLIZER_GENERATED_PARAMETER_LIST   テンプレート時のパラメータリスト
//              THEOLIZER_GENERATED_UNIQUE_NAME      Global VersionNo Tableへの登録名
//
//              THEOLIZER_GENERATED_VERSION_NO       TheolizerVersion<>のバージョン番号
//              THEOLIZER_GENERATED_CLASS_NAME       このバージョン時のクラス名
//              THEOLIZER_GENERATED_BASE_LIST        基底クラスのリスト
//                  THEOLIZER_GENERATED_BASE           基底クラス
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

#define THEOLIZER_INTERNAL_DERIVED TheolizerNonIntrusive<THEOLIZER_GENERATED_CLASS_TYPE>

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
//      手動型を基底クラスにするためのラッパー
//          参照するために中継が必要
//          中継する際の参照名は、_base_クラス名
// ***************************************************************************

#if (THEOLIZER_GENERATED_VERSION_NO == THEOLIZER_GENERATED_LAST_VERSION_NO)

#if !defined(THEOLIZER_GENERATED_PARAMETER_LIST)
    #define THEOLIZER_INTERNAL_PARAMETER_LIST template<>
    #define THEOLIZER_INTERNAL_RELAY_NAME  \
        THEOLIZER_INTERNAL_CAT(THEOLIZER_GENERATED_CLASS_TYPE,TheolizerBase)
#else
    #define THEOLIZER_INTERNAL_PARAMETER_LIST THEOLIZER_GENERATED_PARAMETER_LIST
    #define THEOLIZER_INTERNAL_RELAY_NAME  \
        THEOLIZER_INTERNAL_CAT(THEOLIZER_GENERATED_UNIQUE_NAME,TheolizerBase)
#endif

THEOLIZER_INTERNAL_PARAMETER_LIST
class TheolizerBase<THEOLIZER_GENERATED_CLASS_TYPE>
{
    theolizer::internal::DeferredGeneration<THEOLIZER_GENERATED_CLASS_TYPE> mInstance;
public:
    static const bool kIsTheolizerBase=true;
    typedef THEOLIZER_GENERATED_CLASS_TYPE Target;

    THEOLIZER_GENERATED_CLASS_TYPE& THEOLIZER_INTERNAL_RELAY_NAME;
    TheolizerBase(THEOLIZER_GENERATED_CLASS_TYPE& iTheolizerTarget) :
        THEOLIZER_INTERNAL_RELAY_NAME(iTheolizerTarget)
    { }
    TheolizerBase(TheolizerBase& iTheolizerBase) :
        THEOLIZER_INTERNAL_RELAY_NAME(iTheolizerBase.THEOLIZER_INTERNAL_RELAY_NAME)
    { }
    TheolizerBase() :
        THEOLIZER_INTERNAL_RELAY_NAME(mInstance.get())
    { }
    operator THEOLIZER_GENERATED_CLASS_TYPE&()             {return THEOLIZER_INTERNAL_RELAY_NAME;}
    operator THEOLIZER_GENERATED_CLASS_TYPE const&() const {return THEOLIZER_INTERNAL_RELAY_NAME;}
};
#undef THEOLIZER_INTERNAL_RELAY_NAME
#undef THEOLIZER_INTERNAL_PARAMETER_LIST

#endif

// ***************************************************************************
//      クラス・テンプレート対応
// ***************************************************************************

#if !defined(THEOLIZER_GENERATED_PARAMETER_LIST)
    #define THEOLIZER_GENERATED_PARAMETER_LIST
#endif

// ***************************************************************************
//      コンパイル不要時処理
// ***************************************************************************

#if defined(THEOLIZER_INTERNAL_DRIVER) || !defined(THEOLIZER_GENERATED_NO_COMPILE)

// ***************************************************************************
//      ClassType用TheolizerVersion<>部分特殊化定義
//          メンバー変数については、ターゲット・クラスと同じ名前を付けるので、
//          衝突の可能性があるため、Theolizerプレフィックスを付ける。
//          関数や型については、TheolizerVersion<>内スコープなのでプレフィックス不要。
// ***************************************************************************

THEOLIZER_GENERATED_PARAMETER_LIST
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

    // 基本情報
    typedef THEOLIZER_GENERATED_CLASS_TYPE TheolizerTarget;
    struct Theolizer
    {
        #if defined(THEOLIZER_GENERATED_UNIQUE_NAME)
        friend struct THEOLIZER_GENERATED_UNIQUE_NAME;
        #endif

        // ドライバへの定義位置通知用(dVarの定義位置が取得できる)
        #undef  THEOLIZER_INTERNAL_DEFINE
        #define THEOLIZER_INTERNAL_DEFINE(dVar, dVerNo) static const unsigned dVar=dVerNo;
        THEOLIZER_GENERATED_VERSION_NO
        THEOLIZER_GENERATED_LAST_VERSION_NO
        #undef  THEOLIZER_INTERNAL_DEFINE
        #define THEOLIZER_INTERNAL_DEFINE(dVar, dVerNo) dVerNo

        static const bool kIsVersion=true;
        static const bool kIsEnum=false;
        static const bool kIsArray=false;

        #ifdef THEOLIZER_INTERNAL_DRIVER
            #define THEOLIZER_INTERNAL_CLASS_NAME(dName)                    \
                THEOLIZER_ANNOTATE(TS:THEOLIZER_INTERNAL_UNPAREN dName)
            #define THEOLIZER_INTERNAL_TEMPLATE_NAME(dName)                 \
                THEOLIZER_ANNOTATE(TS:TEMPLATE_NAME(dName))
            THEOLIZER_GENERATED_CLASS_NAME()
            #undef THEOLIZER_INTERNAL_CLASS_NAME
            #undef THEOLIZER_INTERNAL_TEMPLATE_NAME
        #endif
        static const theolizer::internal::ElementsMapping kElementsMapping=
            theolizer::internal::emOrder;

        // ドライバ解析用に要素の引き継ぎ情報を定義する
        //  ドライバ処理の都合上kVersionNoとkElementsMappingより後で定義する
        #ifdef THEOLIZER_INTERNAL_DRIVER
            #if defined(THEOLIZER_GENERATED_BASE_LIST)
                #define THEOLIZER_GENERATED_SEP
                #define THEOLIZER_INTERNAL_VERSION_B(                       \
                    dSpec,dTrack,dId,dType,dVerNo,dName,dVer,dAnno)         \
                    THEOLIZER_ANNOTATE(dAnno)                               \
                    void BaseDefine dType;
                #define THEOLIZER_INTERNAL_VERSION_B_DEL(                   \
                    dSpec,dTrack,dId,dType,dVerNo,dName,dVer,dAnno)         \
                    THEOLIZER_ANNOTATE(dAnno)                               \
                    void BaseDefine dType;
                THEOLIZER_GENERATED_BASE_LIST()
                #undef  THEOLIZER_GENERATED_SEP
                #undef  THEOLIZER_INTERNAL_VERSION_B
                #undef  THEOLIZER_INTERNAL_VERSION_B_DEL
            #endif
        #endif
    };

    // シリアライザ情報返却用
    typedef tBaseSerializer Serializer;

    // クラス名返却
    #define THEOLIZER_INTERNAL_CLASS_NAME(dName)                            \
        return dName
    #define THEOLIZER_INTERNAL_TEMPLATE_NAME(dName)                         \
        return THEOLIZER_INTERNAL_MAKE_TEMPLATE_NAME dName
    static std::string getClassName(theolizer::internal::VersionNoList const& iVersionNoList)
    {THEOLIZER_GENERATED_CLASS_NAME();}
    #undef THEOLIZER_INTERNAL_CLASS_NAME
    #undef THEOLIZER_INTERNAL_TEMPLATE_NAME

    // メンバ変数対応方法返却
    static theolizer::internal::ElementsMapping getElementsMapping(unsigned iVersionNo);

    // 要素枚挙用Range返却
    static theolizer::internal::ElementRange getElementRange(unsigned iVersionNo);

//      ---<<< 保存／回復関数 >>>---

    // 入口関数
    void saveClass(tBaseSerializer& iSerializer, unsigned iVersionNo);
    void loadClass(tBaseSerializer& iSerializer, unsigned iVersionNo);

//      ---<<< 型チェック用 >>>---

    static std::string getClassName
    (
        theolizer::internal::VersionNoList const& iVersionNoList,
        unsigned iVersionNo
    );

//      ---<<< ターゲットへのポインタ >>>---

    TheolizerTarget*&    mInstance;

//      ---<<< ポリモーフィズム対応 >>>---

#if (THEOLIZER_GENERATED_VERSION_NO == THEOLIZER_GENERATED_LAST_VERSION_NO)
    static void registerToBaseClass2()
    {
        // 自分を基底クラスへ登録する
        #if defined(THEOLIZER_GENERATED_BASE_LIST)
            #define THEOLIZER_GENERATED_SEP
            #define THEOLIZER_INTERNAL_VERSION_B(                           \
                dSpec,dTrack,dId,dType,dVerNo,dName,dVer,dAnno)             \
                THEOLIZER_INTERNAL_MAKE_CLASS_TYPE(dType)::getInstance().template\
                    registerDrivedClass<THEOLIZER_INTERNAL_DERIVED>();
            #define THEOLIZER_INTERNAL_VERSION_B_DEL(                       \
                dSpec,dTrack,dId,dType,dVerNo,dName,dVer,dAnno)             \
                THEOLIZER_INTERNAL_VERSION_B(dSpec,dTrack,dId,dType,dVerNo,dVer,dAnno)
            THEOLIZER_GENERATED_BASE_LIST()
            #undef  THEOLIZER_GENERATED_SEP
            #undef  THEOLIZER_INTERNAL_VERSION_B
            #undef  THEOLIZER_INTERNAL_VERSION_B_DEL
        #endif
    }
#endif

//----------------------------------------------------------------------------
//      コンストラクタ(ターゲットからコンストラクト)
//----------------------------------------------------------------------------

    TheolizerVersion(TheolizerTarget*& iTargetInstance) :
        mInstance(iTargetInstance)
    { }

//----------------------------------------------------------------------------
//      デストラクタ
//----------------------------------------------------------------------------

    ~TheolizerVersion()
    { }

//----------------------------------------------------------------------------
//      基底クラスと要素のリスト
//----------------------------------------------------------------------------

//      ---<<< 登録用 >>>---
//      initializer_listによるコンストラクト、および、emplace_backは使えなかった。
//      Elementのコンストラクタがテンプレートであることが原因の可能性あり。

    typedef std::vector<theolizer::internal::Element<TheolizerVersion> > ElementList;

    // 基底クラス・リスト生成
    static ElementList& getElementList()
    {
        static ElementList sElementList;
        static bool wsInitialzied=false;
        if (!wsInitialzied)
        {
            wsInitialzied=true;
            #if defined(THEOLIZER_GENERATED_BASE_LIST)
                #define THEOLIZER_GENERATED_SEP
                #define THEOLIZER_INTERNAL_VERSION_B(                       \
                    dSpec,dTrack,dId,dType,dVerNo,dName,dVer,dAnno)         \
                    {                                                       \
                        theolizer::internal::Element<TheolizerVersion>      \
                        aElement                                            \
                        THEOLIZER_INTERNAL_REGISTER_MBASE(dSpec,dTrack,dType,dVerNo,dName,dVer);\
                        sElementList.push_back(std::move(aElement));        \
                    }
                #define THEOLIZER_INTERNAL_VERSION_B_DEL(                   \
                    dSpec,dTrack,dId,dType,dVerNo,dName,dVer,dAnno)         \
                    {                                                       \
                        theolizer::internal::Element<TheolizerVersion>      \
                        aElement                                            \
                        THEOLIZER_INTERNAL_REGISTER_MBASE(dSpec,dTrack,dType,dVerNo,dName,dVer);\
                        sElementList.push_back(std::move(aElement));        \
                    }
                THEOLIZER_GENERATED_BASE_LIST()
                #undef  THEOLIZER_GENERATED_SEP
                #undef  THEOLIZER_INTERNAL_VERSION_B
                #undef  THEOLIZER_INTERNAL_VERSION_B_DEL
            #endif
        }
        return sElementList;
    }

    // 要素追加登録
    template<class tTypeFunc>
    static void addElement(tTypeFunc iTypeFunc)
    {
        getElementList().push_back
        (
            theolizer::internal::Element<TheolizerVersion>
            (
                "",                                             // 要素名(不要)
                false,                                          // Is基底クラス
                {theolizerD::All},                              // 保存先
                iTypeFunc,                                      // TypeIndex関数
                [](tBaseSerializer&, TheolizerVersion&){},      // 回復関数(不要)
                [](tBaseSerializer&, TheolizerVersion&){},      // 保存関数(不要)
                "",                                             // 次バージョン要素名(不要)
                theolizer::internal::etmDefault,                // オブジェクト追跡モード(不要)
                0                                               // バージョン番号
            )
        );
    }

//      ---<<< メンバ変数対応方法返却 >>>---

    static theolizer::internal::ElementsMapping getElementsMapping()
    {
        return Theolizer::kElementsMapping;
    }

//      ---<<< 枚挙用Range返却 >>>---

    static theolizer::internal::ElementRange getElementRange()
    {
        ElementList& aElementList=getElementList();
        return  theolizer::internal::ElementRange
                    (
                        aElementList.cbegin(),
                        aElementList.cend(),
                        aElementList.size()
                    );
    }
};

// ***************************************************************************
//      TheolizerVersion<n!=1>用サービス関数定義
//      (THEOLIZER_GENERATED_VERSION_NO+1について定義する)
//          save/loadClass()
//          getClassName()
//          getElementCount()
//          getElementTypeId()
// ***************************************************************************

#if (THEOLIZER_GENERATED_VERSION_NO != THEOLIZER_GENERATED_LAST_VERSION_NO)

//      ---<<< 保存 >>>---
//          基底Serializerから直接呼ばれる場合があるのでポストフィックスあり

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
void THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
<
    tBaseSerializer,
    THEOLIZER_GENERATED_VERSION_NO+1
>::saveClass(tBaseSerializer& iSerializer, unsigned iVersionNo)
{
    // 旧バージョン処理
    if (iVersionNo < (THEOLIZER_GENERATED_VERSION_NO+1))
    {
        THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
        <
            tBaseSerializer,
            THEOLIZER_GENERATED_VERSION_NO
        > aVersion(mInstance);
        aVersion.saveClass(iSerializer, iVersionNo);
return;
    }

    // 当バージョン／クラスの処理
    theolizer::internal::BaseSerializer::AutoRestoreSave
        aAutoSaveEnd(iSerializer, theolizer::internal::emOrder);
    THEOLIZER_INTERNAL_DERIVED::TheolizerUserDefine
    <
        tBaseSerializer,
        TheolizerVersion,
        THEOLIZER_GENERATED_VERSION_NO+1
    >::saveClassManual(iSerializer, this->mInstance);
}

//      ---<<< 回復 >>>---
//          基底Serializerから直接呼ばれる場合があるのでポストフィックスあり

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
void THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
<
    tBaseSerializer,
    THEOLIZER_GENERATED_VERSION_NO+1
>::loadClass(tBaseSerializer& iSerializer, unsigned iVersionNo)
{
    // 旧バージョン処理
    if (iVersionNo < (THEOLIZER_GENERATED_VERSION_NO+1))
    {
        THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
        <
            tBaseSerializer,
            THEOLIZER_GENERATED_VERSION_NO
        > aVersion(mInstance);
        aVersion.loadClass(iSerializer, iVersionNo);
return;
    }

    // 当バージョン／クラスの処理
    theolizer::internal::BaseSerializer::AutoRestoreLoad
        aAutoLoadEnd(iSerializer, theolizer::internal::emOrder);
    THEOLIZER_INTERNAL_DERIVED::TheolizerUserDefine
    <
        tBaseSerializer,
        TheolizerVersion,
        THEOLIZER_GENERATED_VERSION_NO+1
    >::loadClassManual(iSerializer, this->mInstance);
}

//----------------------------------------------------------------------------
//      型チェック用
//----------------------------------------------------------------------------

//      ---<<< クラス名返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
std::string
THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
<
    tBaseSerializer,
    THEOLIZER_GENERATED_VERSION_NO+1
>::getClassName
(
    theolizer::internal::VersionNoList const& iVersionNoList,
    unsigned iVersionNo
)
{
    // 旧バージョン名返却
    if (iVersionNo < (THEOLIZER_GENERATED_VERSION_NO+1))
    {
        return THEOLIZER_INTERNAL_DERIVED::
            TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>::
                getClassName(iVersionNoList, iVersionNo);
    }

    // 当バージョン／クラス名返却
    return getClassName(iVersionNoList);
}

//      ---<<< メンバ変数対応方法返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
theolizer::internal::ElementsMapping
THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
<
    tBaseSerializer,
    THEOLIZER_GENERATED_VERSION_NO+1
>::getElementsMapping
(
    unsigned iVersionNo
)
{
    if (iVersionNo < (THEOLIZER_GENERATED_VERSION_NO+1))
    {
        return THEOLIZER_INTERNAL_DERIVED::
            TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>::
                getElementsMapping(iVersionNo);
    }

    return getElementsMapping();
}

//      ---<<< 要素枚挙用Range返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
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
#endif

// ***************************************************************************
//      TheolizerVersion<1>用サービス関数定義
//          save/loadClass()
//          getClassName()
//          getElementCount()
//          getElementTypeId()
// ***************************************************************************

#if (THEOLIZER_GENERATED_VERSION_NO == 1)

//      ---<<< 保存 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
void THEOLIZER_INTERNAL_DERIVED::TheolizerVersion<tBaseSerializer, 1>::saveClass
(
    tBaseSerializer& iSerializer,
    unsigned iVersionNo
)
{
    // データ内に型名保存
    std::size_t aTypeIndex = theolizer::internal::kInvalidSize;
    if (iSerializer.mCheckMode == theolizer::CheckMode::TypeCheckInData)
    {
        aTypeIndex = theolizer::internal::getTypeIndex<THEOLIZER_GENERATED_CLASS_TYPE>();
    }

    // 当バージョン／クラスの処理
    theolizer::internal::BaseSerializer::AutoRestoreSaveStructure
        aAutoRestoreSaveStructure
        (
            iSerializer,
            theolizer::internal::emOrder,
            theolizer::internal::Structure::Class,
            aTypeIndex
        );
    THEOLIZER_INTERNAL_DERIVED::TheolizerUserDefine
    <
        tBaseSerializer,
        TheolizerVersion,
        1
    >::saveClassManual(iSerializer, this->mInstance);
}

//      ---<<< 回復 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
void THEOLIZER_INTERNAL_DERIVED::TheolizerVersion<tBaseSerializer, 1>::loadClass
(
    tBaseSerializer& iSerializer,
    unsigned iVersionNo
)
{
    // データ内に型名保存
    std::size_t aTypeIndex = theolizer::internal::kInvalidSize;
    if (iSerializer.mCheckMode == theolizer::CheckMode::TypeCheckInData)
    {
        aTypeIndex = theolizer::internal::getTypeIndex<THEOLIZER_GENERATED_CLASS_TYPE>();
    }

    // 当バージョン／クラスの処理
    theolizer::internal::BaseSerializer::AutoRestoreLoadStructure
        aAutoRestoreLoadStructure
        (
            iSerializer,
            theolizer::internal::emOrder,
            theolizer::internal::Structure::Class,
            aTypeIndex
        );
    THEOLIZER_INTERNAL_DERIVED::TheolizerUserDefine
    <
        tBaseSerializer,
        TheolizerVersion,
        1
    >::loadClassManual(iSerializer, this->mInstance);
}

//----------------------------------------------------------------------------
//      型チェック用
//----------------------------------------------------------------------------

//      ---<<< クラス名返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
std::string
THEOLIZER_INTERNAL_DERIVED::TheolizerVersion<tBaseSerializer, 1>::getClassName
(
    theolizer::internal::VersionNoList const& iVersionNoList,
    unsigned iVersionNo
)
{
    // 当バージョン／クラス名返却
    return getClassName(iVersionNoList);
}

//      ---<<< メンバ変数対応方法返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
theolizer::internal::ElementsMapping
THEOLIZER_INTERNAL_DERIVED::TheolizerVersion<tBaseSerializer, 1>::getElementsMapping
(
    unsigned iVersionNo
)
{
    return getElementsMapping();
}

//      ---<<< 要素枚挙用Range返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
theolizer::internal::ElementRange
THEOLIZER_INTERNAL_DERIVED::TheolizerVersion<tBaseSerializer, 1>::getElementRange
(
    unsigned iVersionNo
)
{
    return getElementRange();
}
#endif

// ***************************************************************************
//      ターゲット・クラス用サービス関数定義
//          save/loadClass()
//          getClassName()
//          getElementCount()
//          getElementTypeId()
// ***************************************************************************

#if (THEOLIZER_GENERATED_VERSION_NO == 1)

//      ---<<< 保存 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
inline void THEOLIZER_INTERNAL_DERIVED::Theolizer::saveClass
(
    theolizer::internal::BaseSerializer& iSerializer,
    THEOLIZER_INTERNAL_DERIVED*& iInstancePtr,
    unsigned iVersionNo
)
{
    THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
    <
        theolizer::internal::BaseSerializer,
        Theolizer::kLastVersionNo
    > aVersion(reinterpret_cast<THEOLIZER_GENERATED_CLASS_TYPE*&>(iInstancePtr));
    aVersion.saveClass(iSerializer, iVersionNo);
}

//      ---<<< 回復 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
inline void THEOLIZER_INTERNAL_DERIVED::Theolizer::loadClass
(
    theolizer::internal::BaseSerializer& iSerializer,
    THEOLIZER_INTERNAL_DERIVED*& iInstancePtr,
    unsigned iVersionNo
)
{
    THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
    <
        theolizer::internal::BaseSerializer,
        Theolizer::kLastVersionNo
    > aVersion(reinterpret_cast<THEOLIZER_GENERATED_CLASS_TYPE*&>(iInstancePtr));
    aVersion.loadClass(iSerializer, iVersionNo);
}

//----------------------------------------------------------------------------
//      型チェック用
//----------------------------------------------------------------------------

//      ---<<< クラス名返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
inline std::string
THEOLIZER_INTERNAL_DERIVED::Theolizer::getClassName
(
    theolizer::internal::VersionNoList const& iVersionNoList,
    unsigned iVersionNo
)
{
    return THEOLIZER_INTERNAL_DERIVED::
        TheolizerVersion<theolizer::internal::BaseSerializer, Theolizer::kLastVersionNo>::
            getClassName(iVersionNoList, iVersionNo);
}

//          ---<<< ユニーク名返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
inline char const*
THEOLIZER_INTERNAL_DERIVED::Theolizer::getUniqueName()
{
    #if defined(THEOLIZER_GENERATED_UNIQUE_NAME)
        return THEOLIZER_INTERNAL_STRINGIZE(THEOLIZER_GENERATED_UNIQUE_NAME);
    #else
        THEOLIZER_INTERNAL_ABORT(THEOLIZER_INTERNAL_BUG);
    #endif
}
//      ---<<< メンバ変数対応方法返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
inline theolizer::internal::ElementsMapping
THEOLIZER_INTERNAL_DERIVED::Theolizer::getElementsMapping(unsigned iVersionNo)
{
    return THEOLIZER_INTERNAL_DERIVED::
        TheolizerVersion
        <
            theolizer::internal::BaseSerializer,
            THEOLIZER_GENERATED_LAST_VERSION_NO
        >::getElementsMapping(iVersionNo);
}

//      ---<<< 要素枚挙用Range返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
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

//----------------------------------------------------------------------------
//      ポリモーフィズム対応
//----------------------------------------------------------------------------

//      ---<<< 基底クラスへ自分を登録する >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
inline void THEOLIZER_INTERNAL_DERIVED::Theolizer::registerToBaseClass
(
    theolizer::internal::BaseTypeInfo* iBaseTypeInfo
)
{
    THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
    <
        theolizer::internal::BaseSerializer,
        Theolizer::kLastVersionNo
    >::registerToBaseClass2();
}
#endif

#endif  // THEOLIZER_INTERNAL_DRIVER and THEOLIZER_GENERATED_NO_COMPILE

//----------------------------------------------------------------------------
//      バージョン管理用
//----------------------------------------------------------------------------

#if (THEOLIZER_GENERATED_VERSION_NO == 1)

#if defined(THEOLIZER_GENERATED_UNIQUE_NAME)
    struct THEOLIZER_GENERATED_UNIQUE_NAME {};
#endif

THEOLIZER_GENERATED_PARAMETER_LIST
struct THEOLIZER_INTERNAL_DERIVED::Theolizer::DefineUniqueName
{
#if defined(THEOLIZER_GENERATED_UNIQUE_NAME)
    typedef THEOLIZER_GENERATED_UNIQUE_NAME UniqueClass;
#else
    typedef THEOLIZER_GENERATED_CLASS_TYPE UniqueClass;
#endif
};
#endif

// ***************************************************************************
//      後始末
// ***************************************************************************

#if (THEOLIZER_GENERATED_VERSION_NO == 1)
    #if defined(THEOLIZER_GENERATED_NO_COMPILE)
        #undef THEOLIZER_GENERATED_NO_COMPILE
    #endif
    #undef THEOLIZER_GENERATED_LAST_VERSION_NO
    #undef THEOLIZER_GENERATED_CLASS_TYPE
    #undef THEOLIZER_GENERATED_PARAMETER_LIST
    #undef THEOLIZER_GENERATED_UNIQUE_NAME
#endif

#undef THEOLIZER_GENERATED_CLASS_NAME
#undef THEOLIZER_GENERATED_BASE_LIST

#undef THEOLIZER_INTERNAL_DERIVED

// ***************************************************************************
//          警告抑止解除
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(pop)
#endif
