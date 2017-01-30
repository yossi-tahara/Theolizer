//############################################################################
//      ClassTypeの自動型用TheolizerVersion<>定義ファイル
//          適切なマクロを定義してversion.incをインクルードすることで、
//          ClassTypeのバージョン制御に必要なコードを生成する。
//
//          下記マクロがパラメータとなる。
//              THEOLIZER_GENERATED_NO_COMPILE       未使用時定義
//              THEOLIZER_GENERATED_FULL_AUTO        完全自動型の時定義
//              THEOLIZER_GENERATED_CLASS_TYPE       ターゲット・クラス
//              THEOLIZER_GENERATED_LAST_VERSION_NO  最新版のバージョン番号
//              THEOLIZER_GENERATED_PARAMETER_LIST   テンプレート時のパラメータリスト
//              THEOLIZER_GENERATED_UNIQUE_NAME      Global VersionNo Tableへの登録名
//
//              THEOLIZER_GENERATED_VERSION_NO       TheolizerVersion<>のバージョン番号
//              THEOLIZER_GENERATED_CLASS_NAME       このバージョン時のクラス名
//              THEOLIZER_GENERATED_ELEMENT_MAP      要素の対応方法指定
//              THEOLIZER_GENERATED_BASE_LIST        基底クラスのリスト
//                  THEOLIZER_INTERNAL_VERSION_B       次へ引継ぐクラス
//                  THEOLIZER_INTERNAL_VERSION_B_DEL   次で削除されるクラス
//              THEOLIZER_GENERATED_ELEMENT_LIST     要素のリスト
//                  THEOLIZER_INTERNAL_VERSION_EN      次へ引継ぐ要素(非TheolizerVersion)
//                  THEOLIZER_INTERNAL_VERSION_EN_DEL  次で削除される要素(非TheolizerVersion)
//                  THEOLIZER_INTERNAL_VERSION_EV      次へ引継ぐ要素(TheolizerVersion)
//                  THEOLIZER_INTERNAL_VERSION_EV_DEL  次で削除される要素(TheolizerVersion)
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

#if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wreorder"
#endif

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
        #define THEOLIZER_GENERATED_CLASS_TYPE                              \
            TheolizerNonIntrusive<THEOLIZER_GENERATED_FULL_AUTO>
        #define THEOLIZER_INTERNAL_TARGET  THEOLIZER_GENERATED_FULL_AUTO
        #define THEOLIZER_INTERNAL_IS_FULL_AUTO  true

        #if !defined(THEOLIZER_GENERATED_PARAMETER_LIST)
            THEOLIZER_INTERNAL_NON_INTRUSIVE(THEOLIZER_GENERATED_FULL_AUTO);
        #else
            THEOLIZER_INTERNAL_NON_INTRUSIVE_TEMPLATE(
                (THEOLIZER_GENERATED_PARAMETER_LIST),
                (THEOLIZER_GENERATED_FULL_AUTO),
                THEOLIZER_GENERATED_UNIQUE_NAME);
        #endif
    #else
        #define THEOLIZER_INTERNAL_TARGET  THEOLIZER_GENERATED_CLASS_TYPE
        #define THEOLIZER_INTERNAL_IS_FULL_AUTO  false
    #endif
#else   // THEOLIZER_GENERATED_LAST_VERSION_NO不正修正への対処(気がついたので実施)
    #if !defined(THEOLIZER_INTERNAL_IS_FULL_AUTO)
        #define THEOLIZER_INTERNAL_IS_FULL_AUTO  false
    #endif
#endif

// ***************************************************************************
//      クラス・テンプレート対応
// ***************************************************************************

#if !defined(THEOLIZER_GENERATED_PARAMETER_LIST)
    #define THEOLIZER_GENERATED_PARAMETER_LIST
#endif

// ***************************************************************************
//      バージョン管理用
// ***************************************************************************

#if (THEOLIZER_GENERATED_VERSION_NO == THEOLIZER_GENERATED_LAST_VERSION_NO)

#if defined(THEOLIZER_GENERATED_UNIQUE_NAME)
    struct THEOLIZER_GENERATED_UNIQUE_NAME {};
#endif

THEOLIZER_GENERATED_PARAMETER_LIST
struct THEOLIZER_GENERATED_CLASS_TYPE::Theolizer::DefineUniqueName
{
#if defined(THEOLIZER_GENERATED_UNIQUE_NAME)
    typedef THEOLIZER_GENERATED_UNIQUE_NAME UniqueClass;
#else
    typedef THEOLIZER_INTERNAL_TARGET UniqueClass;
#endif
};

#endif

// ***************************************************************************
//      コンパイル不要時処理
// ***************************************************************************

#if defined(THEOLIZER_INTERNAL_DRIVER) || !defined(THEOLIZER_GENERATED_NO_COMPILE)

// ***************************************************************************
//      ClassType用TheolizerSpecials<>部分特殊化定義
//          制御用メンバ変数を保持する
// ***************************************************************************

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
struct THEOLIZER_GENERATED_CLASS_TYPE::
    TheolizerSpecials<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>
{
    // 親クラス定義(最新版か古い版かで切り替える)
    #if (THEOLIZER_GENERATED_VERSION_NO == THEOLIZER_GENERATED_LAST_VERSION_NO)
    typedef THEOLIZER_GENERATED_CLASS_TYPE ParentVersion;
    #else
    typedef THEOLIZER_GENERATED_CLASS_TYPE::
        TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO+1> ParentVersion;
    #endif

    // ターゲットへのポインタ
    THEOLIZER_GENERATED_CLASS_TYPE* mTarget;

    // 親(ターゲット、もしくは、次バージョン)インスタンスへのポインタ
    ParentVersion*  mParentVersion;

    // 同じバージョンから生成された際のそのインスタンスへのポインタ
    typedef typename THEOLIZER_GENERATED_CLASS_TYPE::template
        TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO> SameVersion;
    SameVersion*    mSameVersion;

    // 回復処理中フラグ(これがfalseならupVersion()を呼ばない)
    bool            mIsLoading;

    // コンストラクタ(親、もしくは、次バージョンからコンストラクト)
    //  デフォルト・コンストラクタも共用している。
    TheolizerSpecials(ParentVersion* iParentVersion=nullptr) :
        #if (THEOLIZER_GENERATED_VERSION_NO == THEOLIZER_GENERATED_LAST_VERSION_NO)
            mTarget(iParentVersion)
        #else
            mTarget((iParentVersion)?
                        iParentVersion->mTheolizerSpecials.mTarget
                        :
                        nullptr)
        #endif
        ,mParentVersion(iParentVersion)
        ,mSameVersion(nullptr)
        ,mIsLoading(false)
    { }

    // コンストラクタ(同じバージョンからコンストラクト)
    TheolizerSpecials(SameVersion* iSameVersion) :
        mTarget(iSameVersion->mTheolizerSpecials.mTarget)
        ,mParentVersion(nullptr)
        ,mSameVersion(iSameVersion)
        ,mIsLoading(false)
    { }
};

// ***************************************************************************
//      ClassType用TheolizerBackups<>部分特殊化定義
//          回復されなかったメンバ変数の値を元に戻すためのハックアップの保持
//          および、次バージョンで削除されたメンバの処理用記憶領域
//          基底クラスは、TheolizerBase+基底クラス名というメンバ変数名で定義。
// ***************************************************************************

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
struct THEOLIZER_GENERATED_CLASS_TYPE::
    TheolizerBackups<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>
{
    // 親クラス定義(最新版か古い版かで切り替える)
    #if (THEOLIZER_GENERATED_VERSION_NO == THEOLIZER_GENERATED_LAST_VERSION_NO)
    typedef THEOLIZER_GENERATED_CLASS_TYPE ParentVersion;
    #else
    typedef THEOLIZER_GENERATED_CLASS_TYPE::
        TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO+1> ParentVersion;
    #endif
    // 統一バージョン定義
    typedef typename THEOLIZER_GENERATED_CLASS_TYPE::template
        TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO> SameVersion;

    // バックアップされたらtrue
    bool    mIsBackuped;

    // バックアップ／削除対応領域生成
    #if defined(THEOLIZER_GENERATED_ELEMENT_LIST)
        #define THEOLIZER_INTERNAL_VERSION_EN(dName,dNextName,              \
            dTrack,dDest,dType,dVer,dAnno)                                  \
                THEOLIZER_INTERNAL_DEFERRED_GENERATION(dVer) dName;
        #define THEOLIZER_INTERNAL_VERSION_EN_DEL(dName,dNextName,          \
            dTrack,dDest,dType,dVer,dAnno)                                  \
                THEOLIZER_INTERNAL_DEFERRED_GENERATION(dVer) dName;
        #define THEOLIZER_INTERNAL_VERSION_EV(dName,dNextName,              \
            dTrack,dDest,dType,dVerNo,dVer,dAnno,dDef)                      \
                THEOLIZER_INTERNAL_##dDef((                                 \
                    THEOLIZER_INTERNAL_DEFERRED_GENERATION(dVer) dName;))
        #define THEOLIZER_INTERNAL_VERSION_EV_DEL(dName,dNextName,          \
            dTrack,dDest,dType,dVerNo,dVer,dAnno)                           \
                THEOLIZER_INTERNAL_DEFERRED_GENERATION(dVer) dName;
        THEOLIZER_GENERATED_ELEMENT_LIST()
        #undef  THEOLIZER_INTERNAL_VERSION_EN
        #undef  THEOLIZER_INTERNAL_VERSION_EN_DEL
        #undef  THEOLIZER_INTERNAL_VERSION_EV
        #undef  THEOLIZER_INTERNAL_VERSION_EV_DEL
    #endif

    // コンストラクタ(同じバージョンからコンストラクト)
    TheolizerBackups(SameVersion* iSameVersion) :
        mIsBackuped(false)
        #if defined(THEOLIZER_GENERATED_ELEMENT_LIST)
            #define THEOLIZER_INTERNAL_VERSION_EN(dName,dNextName,          \
                dTrack,dDest,dType,dVer,dAnno)                              \
                    ,dName()
            #define THEOLIZER_INTERNAL_VERSION_EN_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVer,dAnno)                              \
                    ,dName()
            #define THEOLIZER_INTERNAL_VERSION_EV(dName,dNextName,          \
                dTrack,dDest,dType,dVerNo,dVer,dAnno,dDef)                  \
                THEOLIZER_INTERNAL_##dDef((,dName()))
            #define THEOLIZER_INTERNAL_VERSION_EV_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVerNo,dVer,dAnno)                       \
                    ,dName()
            THEOLIZER_GENERATED_ELEMENT_LIST()
            #undef  THEOLIZER_INTERNAL_VERSION_EN
            #undef  THEOLIZER_INTERNAL_VERSION_EN_DEL
            #undef  THEOLIZER_INTERNAL_VERSION_EV
            #undef  THEOLIZER_INTERNAL_VERSION_EV_DEL
        #endif
    { }

    // デフォルト・コンストラクタ
    TheolizerBackups() :
        mIsBackuped(false)
        #if defined(THEOLIZER_GENERATED_ELEMENT_LIST)
            #define THEOLIZER_INTERNAL_VERSION_EN(dName,dNextName,          \
                dTrack,dDest,dType,dVer,dAnno)                              \
                    ,dName()
            #define THEOLIZER_INTERNAL_VERSION_EN_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVer,dAnno)                              \
                    ,dName()
            #define THEOLIZER_INTERNAL_VERSION_EV(dName,dNextName,          \
                dTrack,dDest,dType,dVerNo,dVer,dAnno,dDef)                  \
                THEOLIZER_INTERNAL_##dDef((,dName()))
            #define THEOLIZER_INTERNAL_VERSION_EV_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVerNo,dVer,dAnno)                       \
                    ,dName()
            THEOLIZER_GENERATED_ELEMENT_LIST()
            #undef  THEOLIZER_INTERNAL_VERSION_EN
            #undef  THEOLIZER_INTERNAL_VERSION_EN_DEL
            #undef  THEOLIZER_INTERNAL_VERSION_EV
            #undef  THEOLIZER_INTERNAL_VERSION_EV_DEL
        #endif
    { }
};

// ***************************************************************************
//      ClassType用TheolizerVersion<>部分特殊化定義
//          メンバー変数については、ターゲット・クラスと同じ名前を付けるので、
//          衝突の可能性があるため、Theolizerプレフィックスを付ける。
//          関数や型については、TheolizerVersion<>内スコープなのでプレフィックス不要。
// ***************************************************************************

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
struct THEOLIZER_GENERATED_CLASS_TYPE::
    TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>
    // 基底クラス指定
    #if defined(THEOLIZER_GENERATED_BASE_LIST)
        #define THEOLIZER_GENERATED_SEP   ,
        #define THEOLIZER_INTERNAL_VERSION_B(dSpec, dTrack, dId, dType, dVerNo, dVer, dAnno)\
            dSpec THEOLIZER_INTERNAL_UNPAREN dVer
        #define THEOLIZER_INTERNAL_VERSION_B_DEL(dSpec, dTrack, dId, dType, dVerNo, dVer, dAnno)\
            dSpec THEOLIZER_INTERNAL_UNPAREN dVer
        : THEOLIZER_GENERATED_BASE_LIST()
        #undef  THEOLIZER_GENERATED_SEP
        #undef  THEOLIZER_INTERNAL_VERSION_B
        #undef  THEOLIZER_INTERNAL_VERSION_B_DEL
    #endif
{
//----------------------------------------------------------------------------
//      保存／回復処理制御用メンバー群
//----------------------------------------------------------------------------

//      ---<<< 基本情報 >>>---

    static const bool kIsTheolizer=true;

    typedef THEOLIZER_INTERNAL_TARGET TheolizerTarget;
    typedef THEOLIZER_GENERATED_CLASS_TYPE TheolizerClass;    // 非侵入型ではTheolizerNonIntrusive
    struct Theolizer
    {
        #if defined(THEOLIZER_GENERATED_UNIQUE_NAME)
        friend struct THEOLIZER_GENERATED_UNIQUE_NAME;
        #endif
        static const bool kIsFullAuto=THEOLIZER_INTERNAL_IS_FULL_AUTO;

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

        // ドライバ解析用(クラス名)
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
            theolizer::internal::THEOLIZER_GENERATED_ELEMENT_MAP;

        // ドライバ解析用(要素情報：最新版との比較が必要なので要素毎に付ける)
        //  ドライバ処理の都合上kVersionNoとkElementsMappingより後で定義する
        #ifdef THEOLIZER_INTERNAL_DRIVER
            #if defined(THEOLIZER_GENERATED_BASE_LIST)
                #define THEOLIZER_GENERATED_SEP
                #define THEOLIZER_INTERNAL_VERSION_B(                       \
                    dSpec, dTrack, dId, dType, dVerNo, dVer, dAnno)         \
                    THEOLIZER_ANNOTATE(dAnno)                               \
                    void BaseDefine dType;
                #define THEOLIZER_INTERNAL_VERSION_B_DEL(                   \
                    dSpec, dTrack, dId, dType, dVerNo, dVer, dAnno)         \
                    THEOLIZER_ANNOTATE(dAnno)                               \
                    void BaseDefine dType;
                THEOLIZER_GENERATED_BASE_LIST()
                #undef  THEOLIZER_GENERATED_SEP
                #undef  THEOLIZER_INTERNAL_VERSION_B
                #undef  THEOLIZER_INTERNAL_VERSION_B_DEL
            #endif
            // 他の名前と被らないよう先頭に'_'を付加しておく
            #if defined(THEOLIZER_GENERATED_ELEMENT_LIST)
                #define THEOLIZER_INTERNAL_VERSION_EN(dName,dNextName,      \
                    dTrack,dDest,dType,dVer,dAnno)                          \
                        THEOLIZER_ANNOTATE(dAnno)                           \
                        typedef THEOLIZER_INTERNAL_UNPAREN dType   _##dName;
                #define THEOLIZER_INTERNAL_VERSION_EN_DEL(dName,dNextName,  \
                    dTrack,dDest,dType,dVer,dAnno)                          \
                        THEOLIZER_ANNOTATE(dAnno)                           \
                        typedef THEOLIZER_INTERNAL_UNPAREN dType   _##dName;
                #define THEOLIZER_INTERNAL_VERSION_EV(dName,dNextName,      \
                    dTrack,dDest,dType,dVerNo,dVer,dAnno,dDef)              \
                        THEOLIZER_ANNOTATE(dAnno)                           \
                        typedef THEOLIZER_INTERNAL_UNPAREN dType   _##dName;
                #define THEOLIZER_INTERNAL_VERSION_EV_DEL(dName,dNextName,  \
                    dTrack,dDest,dType,dVerNo,dVer,dAnno)                   \
                        THEOLIZER_ANNOTATE(dAnno)                           \
                        typedef THEOLIZER_INTERNAL_UNPAREN dType   _##dName;
                THEOLIZER_GENERATED_ELEMENT_LIST()
                #undef  THEOLIZER_INTERNAL_VERSION_EN
                #undef  THEOLIZER_INTERNAL_VERSION_EN_DEL
                #undef  THEOLIZER_INTERNAL_VERSION_EV
                #undef  THEOLIZER_INTERNAL_VERSION_EV_DEL
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

    // 親クラス定義(最新版か古い版かで切り替える)
    #if (THEOLIZER_GENERATED_VERSION_NO == THEOLIZER_GENERATED_LAST_VERSION_NO)
    typedef THEOLIZER_GENERATED_CLASS_TYPE ParentVersion;
    #else
    typedef THEOLIZER_GENERATED_CLASS_TYPE::
        TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO+1> ParentVersion;
    #endif

//      ---<<< バックアップ用メンバ変数群 >>>---
//          次バージョンでの削除メンバの領域としても使用するため、
//          最初に初期化する必要があり、他の要素より最初に定義する

    typedef typename THEOLIZER_GENERATED_CLASS_TYPE::template
        TheolizerBackups<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>   TheolizerBackups;
    std::unique_ptr<TheolizerBackups>                                       mTheolizerBackups;

    typename THEOLIZER_GENERATED_CLASS_TYPE::template
        TheolizerSpecials<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>  mTheolizerSpecials;

//      ---<<< 保存／回復関数 >>>---

    void saveClass(tBaseSerializer& iSerializer, unsigned iVersionNo);
    void loadClass(tBaseSerializer& iSerializer, unsigned iVersionNo);

//      ---<<< 型チェック用 >>>---

    static std::string getClassName
    (
        theolizer::internal::VersionNoList const& iVersionNoList,
        unsigned iVersionNo
    );

    // 要素数返却(実体)
    static size_t getElementCount()
    {
        static bool wsInitialzied=false;
        static size_t  wsElementCount;
        if (!wsInitialzied)
        {
            wsInitialzied=true;
            for (wsElementCount=0;
                 !getElementTheolizer(wsElementCount).isSentinel();
                 ++wsElementCount)
                ;
        }
        return wsElementCount;
    }

    // メンバ変数対応方法返却
    static theolizer::internal::ElementsMapping getElementsMapping(unsigned iVersionNo);

    // 要素枚挙用Range返却
    static theolizer::internal::ElementRange getElementRange(unsigned iVersionNo);

    // クラス種別返却
    static unsigned getTypeFlags(unsigned iVersionNo);

//      ---<<< ポリモーフィズム対応 >>>---

#if (THEOLIZER_GENERATED_VERSION_NO == THEOLIZER_GENERATED_LAST_VERSION_NO)
    static void registerToBaseClass2()
    {
        // 自分を基底クラスへ登録する
        #if defined(THEOLIZER_GENERATED_BASE_LIST)
            #define THEOLIZER_GENERATED_SEP
            #define THEOLIZER_INTERNAL_VERSION_B(dSpec, dTrack, dId, dType, dVerNo, dVer, dAnno)\
                THEOLIZER_INTERNAL_MAKE_CLASS_TYPE(dType)::getInstance().template\
                    registerDrivedClass<THEOLIZER_GENERATED_CLASS_TYPE>();
            #define THEOLIZER_INTERNAL_VERSION_B_DEL(dSpec,dTrack,dId,dType,dVerNo,dVer,dAnno)\
                THEOLIZER_INTERNAL_VERSION_B(dSpec,dTrack,dId,dType,dVerNo,dVer,dAnno)
            THEOLIZER_GENERATED_BASE_LIST()
            #undef  THEOLIZER_GENERATED_SEP
            #undef  THEOLIZER_INTERNAL_VERSION_B
            #undef  THEOLIZER_INTERNAL_VERSION_B_DEL
        #endif
    }
#endif

//----------------------------------------------------------------------------
//      要素群
//----------------------------------------------------------------------------

    #if defined(THEOLIZER_GENERATED_ELEMENT_LIST)
        #define THEOLIZER_INTERNAL_VERSION_EN(dName,dNextName,              \
            dTrack,dDest,dType,dVer,dAnno)                                  \
                THEOLIZER_INTERNAL_UNPAREN dVer& dName;
        #define THEOLIZER_INTERNAL_VERSION_EN_DEL(dName,dNextName,          \
            dTrack,dDest,dType,dVer,dAnno)                                  \
                THEOLIZER_INTERNAL_UNPAREN dVer& dName;
        #define THEOLIZER_INTERNAL_VERSION_EV(dName,dNextName,              \
            dTrack,dDest,dType,dVerNo,dVer,dAnno,dDef)                      \
                THEOLIZER_INTERNAL_UNPAREN dVer dName;
        #define THEOLIZER_INTERNAL_VERSION_EV_DEL(dName,dNextName,          \
            dTrack,dDest,dType,dVerNo,dVer,dAnno)                           \
                THEOLIZER_INTERNAL_UNPAREN dVer dName;
        THEOLIZER_GENERATED_ELEMENT_LIST()
        #undef  THEOLIZER_INTERNAL_VERSION_EN
        #undef  THEOLIZER_INTERNAL_VERSION_EN_DEL
        #undef  THEOLIZER_INTERNAL_VERSION_EV
        #undef  THEOLIZER_INTERNAL_VERSION_EV_DEL
    #endif

//      ---<<< 中身へのアクセス用(ArrayManager<>から使用する) >>>---
//          enum型と歩調を合わせるために定義する

    typedef TheolizerVersion AsElementOfArray;
    AsElementOfArray& getAccess()
    {
        return *this;
    }

//      ---<<< 引き継ぎフラグ定義 >>>---

    #if defined(THEOLIZER_GENERATED_ELEMENT_LIST)
        #define THEOLIZER_INTERNAL_VERSION_EN(dName,dNextName,              \
            dTrack,dDest,dType,dVer,dAnno)                                  \
                bool dName##TheolizerSucceed;
        #define THEOLIZER_INTERNAL_VERSION_EN_DEL(dName,dNextName,          \
            dTrack,dDest,dType,dVer,dAnno)                                  \
                bool dName##TheolizerSucceed;
        #define THEOLIZER_INTERNAL_VERSION_EV(dName,dNextName,              \
            dTrack,dDest,dType,dVerNo,dVer,dAnno,dDef)                      \
                bool dName##TheolizerSucceed;
        #define THEOLIZER_INTERNAL_VERSION_EV_DEL(dName,dNextName,          \
            dTrack,dDest,dType,dVerNo,dVer,dAnno)                           \
                bool dName##TheolizerSucceed;
        THEOLIZER_GENERATED_ELEMENT_LIST()
        #undef  THEOLIZER_INTERNAL_VERSION_EN
        #undef  THEOLIZER_INTERNAL_VERSION_EN_DEL
        #undef  THEOLIZER_INTERNAL_VERSION_EV
        #undef  THEOLIZER_INTERNAL_VERSION_EV_DEL
    #endif

//----------------------------------------------------------------------------
//      各要素をTypeInfoListへ登録する
//          RegisterType<>を実体化して登録する
//----------------------------------------------------------------------------

    static void registerElements()
    {
        #if defined(THEOLIZER_GENERATED_BASE_LIST)
            #define THEOLIZER_GENERATED_SEP
            #define THEOLIZER_INTERNAL_VERSION_B(dSpec,dTrack,dId,dType,dVerNo,dVer,dAnno)\
                theolizer::internal::RegisterType                           \
                <                                                           \
                    tBaseSerializer,                                        \
                    THEOLIZER_INTERNAL_UNPAREN dType                        \
                >::getInstance();
            #define THEOLIZER_INTERNAL_VERSION_B_DEL(dSpec,dTrack,dId,dType,dVerNo,dVer,dAnno)\
                theolizer::internal::RegisterType                           \
                <                                                           \
                    tBaseSerializer,                                        \
                    THEOLIZER_INTERNAL_UNPAREN dType                        \
                >::getInstance();
            THEOLIZER_GENERATED_BASE_LIST()
            #undef  THEOLIZER_GENERATED_SEP
            #undef  THEOLIZER_INTERNAL_VERSION_B
            #undef  THEOLIZER_INTERNAL_VERSION_B_DEL
        #endif
        // 参照有りの場合に備えて参照を外したものも登録する
        #if defined(THEOLIZER_GENERATED_ELEMENT_LIST)
            #define THEOLIZER_INTERNAL_VERSION_EN(dName,dNextName,          \
                dTrack,dDest,dType,dVer,dAnno)                              \
                    theolizer::internal::RegisterType                       \
                    <                                                       \
                        tBaseSerializer,                                    \
                        THEOLIZER_INTERNAL_UNPAREN dType                    \
                    >::getInstance();
            #define THEOLIZER_INTERNAL_VERSION_EN_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVer,dAnno)                              \
                    theolizer::internal::RegisterType                       \
                    <                                                       \
                        tBaseSerializer,                                    \
                        THEOLIZER_INTERNAL_UNPAREN dType                    \
                    >::getInstance();
            #define THEOLIZER_INTERNAL_VERSION_EV(dName,dNextName,          \
                dTrack,dDest,dType,dVerNo,dVer,dAnno,dDef)                  \
                    theolizer::internal::RegisterType                       \
                    <                                                       \
                        tBaseSerializer,                                    \
                        THEOLIZER_INTERNAL_UNPAREN dType                    \
                    >::getInstance();
            #define THEOLIZER_INTERNAL_VERSION_EV_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVerNo,dVer,dAnno)                       \
                    theolizer::internal::RegisterType                       \
                    <                                                       \
                        tBaseSerializer,                                    \
                        THEOLIZER_INTERNAL_UNPAREN dType                    \
                    >::getInstance();
            THEOLIZER_GENERATED_ELEMENT_LIST()
            #undef  THEOLIZER_INTERNAL_VERSION_EN
            #undef  THEOLIZER_INTERNAL_VERSION_EN_DEL
            #undef  THEOLIZER_INTERNAL_VERSION_EV
            #undef  THEOLIZER_INTERNAL_VERSION_EV_DEL
        #endif
    }

//----------------------------------------------------------------------------
//      完全自動型用型変換
//          ターゲットのクラスを受け取り、
//          TheolizerNonIntrusive<>型のコンストラクタを呼ぶ
//----------------------------------------------------------------------------

#if (THEOLIZER_GENERATED_VERSION_NO == THEOLIZER_GENERATED_LAST_VERSION_NO)
    #if defined(THEOLIZER_GENERATED_FULL_AUTO)
    TheolizerVersion(TheolizerTarget& iTheolizerTarget) :
        TheolizerVersion(reinterpret_cast<ParentVersion&>(iTheolizerTarget))
    { }
    #endif
#endif

//----------------------------------------------------------------------------
//      コンストラクタ(親、もしくは、次バージョンからコンストラクト)
//----------------------------------------------------------------------------

    TheolizerVersion
    (
        ParentVersion& iParentVersion,
        unsigned iVersionNo=THEOLIZER_GENERATED_VERSION_NO,
        bool iIsSave=false
    ) :
        #if defined(THEOLIZER_GENERATED_BASE_LIST)
            #define THEOLIZER_GENERATED_SEP
            #define THEOLIZER_INTERNAL_VERSION_B(dSpec,dTrack,dId,dType,dVerNo,dVer,dAnno)\
                THEOLIZER_INTERNAL_UNPAREN dVer(iParentVersion),
            #define THEOLIZER_INTERNAL_VERSION_B_DEL(dSpec,dTrack,dId,dType,dVerNo,dVer,dAnno)\
                THEOLIZER_INTERNAL_UNPAREN dVer(),
            THEOLIZER_GENERATED_BASE_LIST()
            #undef  THEOLIZER_GENERATED_SEP
            #undef  THEOLIZER_INTERNAL_VERSION_B
            #undef  THEOLIZER_INTERNAL_VERSION_B_DEL
        #endif
        mTheolizerBackups
        (
            std::unique_ptr<TheolizerBackups>(new TheolizerBackups(this))
        ),
        mTheolizerSpecials(&iParentVersion)  // ,後置→,前置へ変えている
        #if defined(THEOLIZER_GENERATED_ELEMENT_LIST)
            #define THEOLIZER_INTERNAL_VERSION_EN(dName,dNextName,          \
                dTrack,dDest,dType,dVer,dAnno)                              \
                    ,dName(iParentVersion.dNextName)                        \
                    ,dName##TheolizerSucceed(false)
            #define THEOLIZER_INTERNAL_VERSION_EN_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVer,dAnno)                              \
                    ,dName(mTheolizerBackups->dName.get())                  \
                    ,dName##TheolizerSucceed(false)
            #define THEOLIZER_INTERNAL_VERSION_EV(dName,dNextName,          \
                dTrack,dDest,dType,dVerNo,dVer,dAnno,dDef)                  \
                    ,dName(iParentVersion.dNextName)                        \
                    ,dName##TheolizerSucceed(false)
            #define THEOLIZER_INTERNAL_VERSION_EV_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVerNo,dVer,dAnno)                       \
                    ,dName()                                                \
                    ,dName##TheolizerSucceed(false)
            THEOLIZER_GENERATED_ELEMENT_LIST()
            #undef  THEOLIZER_INTERNAL_VERSION_EN
            #undef  THEOLIZER_INTERNAL_VERSION_EN_DEL
            #undef  THEOLIZER_INTERNAL_VERSION_EV
            #undef  THEOLIZER_INTERNAL_VERSION_EV_DEL
        #endif
    {
        // 要素登録
        registerElements();

       // 最新版でない時のみdownVersion()有り
        #if (THEOLIZER_GENERATED_VERSION_NO != THEOLIZER_GENERATED_LAST_VERSION_NO)
            // バックアップ中はdownVersion()しない
            if (!theolizer::internal::duringBackup())
            {
                THEOLIZER_GENERATED_CLASS_TYPE::TheolizerUserDefine
                <
                    TheolizerVersion,
                    ParentVersion,
                    THEOLIZER_GENERATED_VERSION_NO
                >::downVersion(*(mTheolizerSpecials.mParentVersion), *this);
            }
        #endif

        // 最新版処理でないならバックアップする
        if (theolizer::internal::isLastVersion())
    return;

        // 保存時は、自分が最新版の時のみバックアップする
        if ((theolizer::internal::isSaver())
         && (THEOLIZER_GENERATED_VERSION_NO < Theolizer::kLastVersionNo))
    return;

        mTheolizerBackups->mIsBackuped=true;
        #if defined(THEOLIZER_GENERATED_ELEMENT_LIST)
            #define THEOLIZER_INTERNAL_VERSION_EN(dName,dNextName,          \
                dTrack,dDest,dType,dVer,dAnno)                              \
                    theolizer::internal::copyData(mTheolizerBackups->dName.get(), dName);
            #define THEOLIZER_INTERNAL_VERSION_EN_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVer,dAnno)
            #define THEOLIZER_INTERNAL_VERSION_EV(dName,dNextName,          \
                dTrack,dDest,dType,dVerNo,dVer,dAnno,dDef)                  \
                    THEOLIZER_INTERNAL_##dDef((                             \
                        theolizer::internal::copyData(mTheolizerBackups->dName.get(), dName);))
            #define THEOLIZER_INTERNAL_VERSION_EV_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVerNo,dVer,dAnno)
            THEOLIZER_GENERATED_ELEMENT_LIST()
            #undef  THEOLIZER_INTERNAL_VERSION_EN
            #undef  THEOLIZER_INTERNAL_VERSION_EN_DEL
            #undef  THEOLIZER_INTERNAL_VERSION_EV
            #undef  THEOLIZER_INTERNAL_VERSION_EV_DEL
        #endif
    }

//----------------------------------------------------------------------------
//      コンストラクタ(同じバージョンからコンストラクト)
//----------------------------------------------------------------------------

    TheolizerVersion(TheolizerVersion& iSameVersion) :
        #if defined(THEOLIZER_GENERATED_BASE_LIST)
            #define THEOLIZER_GENERATED_SEP
            #define THEOLIZER_INTERNAL_VERSION_B(dSpec,dTrack,dId,dType,dVerNo,dVer,dAnno)\
                THEOLIZER_INTERNAL_UNPAREN dVer(iSameVersion),
            #define THEOLIZER_INTERNAL_VERSION_B_DEL(dSpec,dTrack,dId,dType,dVerNo,dVer,dAnno)\
                THEOLIZER_INTERNAL_UNPAREN dVer(iSameVersion),
            THEOLIZER_GENERATED_BASE_LIST()
            #undef  THEOLIZER_GENERATED_SEP
            #undef  THEOLIZER_INTERNAL_VERSION_B
            #undef  THEOLIZER_INTERNAL_VERSION_B_DEL
        #endif
        mTheolizerSpecials(&iSameVersion)    // ,後置→,前置へ変えている
        #if defined(THEOLIZER_GENERATED_ELEMENT_LIST)
            #define THEOLIZER_INTERNAL_VERSION_EN(dName,dNextName,          \
                dTrack,dDest,dType,dVer,dAnno)                              \
                    ,dName(iSameVersion.dName)                              \
                    ,dName##TheolizerSucceed(false)
            #define THEOLIZER_INTERNAL_VERSION_EN_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVer,dAnno)                              \
                    ,dName(iSameVersion.dName)                              \
                    ,dName##TheolizerSucceed(false)
            #define THEOLIZER_INTERNAL_VERSION_EV(dName,dNextName,          \
                dTrack,dDest,dType,dVerNo,dVer,dAnno,dDef)                  \
                    ,dName(iSameVersion.dName)                              \
                    ,dName##TheolizerSucceed(false)
            #define THEOLIZER_INTERNAL_VERSION_EV_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVerNo,dVer,dAnno)                       \
                    ,dName(iSameVersion.dName)                              \
                    ,dName##TheolizerSucceed(false)
            THEOLIZER_GENERATED_ELEMENT_LIST()
            #undef  THEOLIZER_INTERNAL_VERSION_EN
            #undef  THEOLIZER_INTERNAL_VERSION_EN_DEL
            #undef  THEOLIZER_INTERNAL_VERSION_EV
            #undef  THEOLIZER_INTERNAL_VERSION_EV_DEL
        #endif
    {
    }

//----------------------------------------------------------------------------
//      ムーブ・コンストラクタ(同じバージョンからコンストラクト)
//          生配列の次元数／要素数変更対応に必要
//----------------------------------------------------------------------------

    TheolizerVersion(TheolizerVersion&& iSameVersion) :
        #if defined(THEOLIZER_GENERATED_BASE_LIST)
            #define THEOLIZER_GENERATED_SEP
            #define THEOLIZER_INTERNAL_VERSION_B(dSpec,dTrack,dId,dType,dVerNo,dVer,dAnno)\
                THEOLIZER_INTERNAL_UNPAREN dVer(iSameVersion),
            #define THEOLIZER_INTERNAL_VERSION_B_DEL(dSpec,dTrack,dId,dType,dVerNo,dVer,dAnno)\
                THEOLIZER_INTERNAL_UNPAREN dVer(iSameVersion),
            THEOLIZER_GENERATED_BASE_LIST()
            #undef  THEOLIZER_GENERATED_SEP
            #undef  THEOLIZER_INTERNAL_VERSION_B
            #undef  THEOLIZER_INTERNAL_VERSION_B_DEL
        #endif
        mTheolizerBackups(std::move(iSameVersion.mTheolizerBackups)),
        mTheolizerSpecials(std::move(iSameVersion.mTheolizerSpecials))  // ,後置→,前置へ変えている
        #if defined(THEOLIZER_GENERATED_ELEMENT_LIST)
            #define THEOLIZER_INTERNAL_VERSION_EN(dName,dNextName,          \
                dTrack,dDest,dType,dVer,dAnno)                              \
                    ,dName(iSameVersion.dName)                              \
                    ,dName##TheolizerSucceed(false)
            #define THEOLIZER_INTERNAL_VERSION_EN_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVer,dAnno)                              \
                    ,dName(iSameVersion.dName)                              \
                    ,dName##TheolizerSucceed(false)
            #define THEOLIZER_INTERNAL_VERSION_EV(dName,dNextName,          \
                dTrack,dDest,dType,dVerNo,dVer,dAnno,dDef)                  \
                    ,dName(iSameVersion.dName)                              \
                    ,dName##TheolizerSucceed(false)
            #define THEOLIZER_INTERNAL_VERSION_EV_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVerNo,dVer,dAnno)                       \
                    ,dName(iSameVersion.dName)                              \
                    ,dName##TheolizerSucceed(false)
            THEOLIZER_GENERATED_ELEMENT_LIST()
            #undef  THEOLIZER_INTERNAL_VERSION_EN
            #undef  THEOLIZER_INTERNAL_VERSION_EN_DEL
            #undef  THEOLIZER_INTERNAL_VERSION_EV
            #undef  THEOLIZER_INTERNAL_VERSION_EV_DEL
        #endif
    {
    }

//----------------------------------------------------------------------------
//      コンストラクタ(親無し)
//----------------------------------------------------------------------------

    TheolizerVersion() :
        #if defined(THEOLIZER_GENERATED_BASE_LIST)
            #define THEOLIZER_GENERATED_SEP
            #define THEOLIZER_INTERNAL_VERSION_B(dSpec,dTrack,dId,dType,dVerNo,dVer,dAnno)\
                THEOLIZER_INTERNAL_UNPAREN dVer(),
            #define THEOLIZER_INTERNAL_VERSION_B_DEL(dSpec,dTrack,dId,dType,dVerNo,dVer,dAnno)\
                THEOLIZER_INTERNAL_UNPAREN dVer(),
            THEOLIZER_GENERATED_BASE_LIST()
            #undef  THEOLIZER_GENERATED_SEP
            #undef  THEOLIZER_INTERNAL_VERSION_B
            #undef  THEOLIZER_INTERNAL_VERSION_B_DEL
        #endif
        mTheolizerBackups
        (
            std::unique_ptr<TheolizerBackups>(new TheolizerBackups())
        ),
        mTheolizerSpecials()             // ,後置→,前置へ変えている
        #if defined(THEOLIZER_GENERATED_ELEMENT_LIST)
            #define THEOLIZER_INTERNAL_VERSION_EN(dName,dNextName,          \
                dTrack,dDest,dType,dVer,dAnno)                              \
                    ,dName(mTheolizerBackups->dName.get())                  \
                    ,dName##TheolizerSucceed(false)
            #define THEOLIZER_INTERNAL_VERSION_EN_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVer,dAnno)                              \
                    ,dName(mTheolizerBackups->dName.get())                  \
                    ,dName##TheolizerSucceed(false)
            #define THEOLIZER_INTERNAL_VERSION_EV(dName,dNextName,          \
                dTrack,dDest,dType,dVerNo,dVer,dAnno,dDef)                  \
                    ,dName()                                                \
                    ,dName##TheolizerSucceed(false)
            #define THEOLIZER_INTERNAL_VERSION_EV_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVerNo,dVer,dAnno)                       \
                    ,dName()                                                \
                    ,dName##TheolizerSucceed(false)
            THEOLIZER_GENERATED_ELEMENT_LIST()
            #undef  THEOLIZER_INTERNAL_VERSION_EN
            #undef  THEOLIZER_INTERNAL_VERSION_EN_DEL
            #undef  THEOLIZER_INTERNAL_VERSION_EV
            #undef  THEOLIZER_INTERNAL_VERSION_EV_DEL
        #endif
    {   // 最新版でない時のみinitialize()有り
        #if (THEOLIZER_GENERATED_VERSION_NO != THEOLIZER_GENERATED_LAST_VERSION_NO)
            THEOLIZER_GENERATED_CLASS_TYPE::TheolizerUserDefine
            <
                TheolizerVersion,
                ParentVersion,
                THEOLIZER_GENERATED_VERSION_NO
            >::initialize(*this);
        #endif

        // 要素登録(新バージョンで削除され、他で保存されないケース対応のため)
        registerElements();
    }

//----------------------------------------------------------------------------
//      デストラクタ
//----------------------------------------------------------------------------

    ~TheolizerVersion()
    {
        // 各要素を必要に応じてリストアする(目標バージョンが自分でない時のみ)
        //  コンストラクト時にバックアップされた時。
        //  引き継ぎするもの(回復されたもの)はリストアしない
        if (mTheolizerBackups && mTheolizerBackups->mIsBackuped)
        {
        #if defined(THEOLIZER_GENERATED_ELEMENT_LIST)
            #define THEOLIZER_INTERNAL_VERSION_EN(dName,dNextName,          \
                dTrack,dDest,dType,dVer,dAnno)                              \
                if (!dName##TheolizerSucceed)                               \
                {                                                           \
                    theolizer::internal::copyData(dName, mTheolizerBackups->dName.get());\
                }
            #define THEOLIZER_INTERNAL_VERSION_EN_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVer,dAnno)
            #define THEOLIZER_INTERNAL_VERSION_EV(dName,dNextName,          \
                dTrack,dDest,dType,dVerNo,dVer,dAnno,dDef)                  \
                THEOLIZER_INTERNAL_##dDef((                                 \
                    if (!dName##TheolizerSucceed)                           \
                    {                                                       \
                        theolizer::internal::copyData(dName, mTheolizerBackups->dName.get());\
                    }))
            #define THEOLIZER_INTERNAL_VERSION_EV_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVerNo,dVer,dAnno)
            THEOLIZER_GENERATED_ELEMENT_LIST()
            #undef  THEOLIZER_INTERNAL_VERSION_EN
            #undef  THEOLIZER_INTERNAL_VERSION_EN_DEL
            #undef  THEOLIZER_INTERNAL_VERSION_EV
            #undef  THEOLIZER_INTERNAL_VERSION_EV_DEL
        #endif
        }

        // 最新版でない時か同一バージョン時に、XxxTheolizerSucceed引き継ぎ
        #if (THEOLIZER_GENERATED_VERSION_NO != THEOLIZER_GENERATED_LAST_VERSION_NO)
            #define THEOLIZER_INTERNAL_SUCCEED(dName, dNextName)            \
                if (mTheolizerSpecials.mParentVersion)                      \
                {                                                           \
                    mTheolizerSpecials.mParentVersion->dNextName##TheolizerSucceed=\
                        dName##TheolizerSucceed;                            \
                }                                                           \
                if (mTheolizerSpecials.mSameVersion)                        \
                {                                                           \
                    mTheolizerSpecials.mSameVersion->dName##TheolizerSucceed=\
                        dName##TheolizerSucceed;                            \
                }
        #else
            #define THEOLIZER_INTERNAL_SUCCEED(dName, dNextName)            \
                if (mTheolizerSpecials.mSameVersion)                        \
                {                                                           \
                    mTheolizerSpecials.mSameVersion->dName##TheolizerSucceed=\
                        dName##TheolizerSucceed;                            \
                }
        #endif

        {
            #if defined(THEOLIZER_GENERATED_ELEMENT_LIST)
                #define THEOLIZER_INTERNAL_VERSION_EN(dName,dNextName,      \
                    dTrack,dDest,dType,dVer,dAnno)                          \
                        THEOLIZER_INTERNAL_SUCCEED(dName, dNextName);
                #define THEOLIZER_INTERNAL_VERSION_EN_DEL(dName,dNextName,  \
                    dTrack,dDest,dType,dVer,dAnno)
                #define THEOLIZER_INTERNAL_VERSION_EV(dName,dNextName,      \
                    dTrack,dDest,dType,dVerNo,dVer,dAnno,dDef)              \
                        THEOLIZER_INTERNAL_SUCCEED(dName, dNextName);
                #define THEOLIZER_INTERNAL_VERSION_EV_DEL(dName,dNextName,  \
                    dTrack,dDest,dType,dVerNo,dVer,dAnno)
                THEOLIZER_GENERATED_ELEMENT_LIST()
                #undef  THEOLIZER_INTERNAL_VERSION_EN
                #undef  THEOLIZER_INTERNAL_VERSION_EN_DEL
                #undef  THEOLIZER_INTERNAL_VERSION_EV
                #undef  THEOLIZER_INTERNAL_VERSION_EV_DEL
            #endif
        }
        #undef THEOLIZER_INTERNAL_SUCCEED

        // 最新版でない時のみupVersion()有り
        #if (THEOLIZER_GENERATED_VERSION_NO != THEOLIZER_GENERATED_LAST_VERSION_NO)
            // 保存中とバックアップ中はupVersion()しない
            if ((mTheolizerSpecials.mIsLoading)
             && (!theolizer::internal::duringBackup()))
            {
                // 次バージョンからのコンストラクト時はバージョン・アップとmIsLoading引き継ぎ
                if (mTheolizerSpecials.mParentVersion)
                {
                    THEOLIZER_GENERATED_CLASS_TYPE::TheolizerUserDefine
                    <
                        TheolizerVersion,
                        ParentVersion,
                        THEOLIZER_GENERATED_VERSION_NO
                    >::upVersion(*this, *(mTheolizerSpecials.mParentVersion));

                    mTheolizerSpecials.mParentVersion->mTheolizerSpecials.mIsLoading
                        =mTheolizerSpecials.mIsLoading;
                }

                // 同バージョンからコンストラクト時はmIsLoading引き継ぎのみ
                if (mTheolizerSpecials.mSameVersion)
                {
                    mTheolizerSpecials.mSameVersion->mTheolizerSpecials.mIsLoading
                        =mTheolizerSpecials.mIsLoading;
                }
            }
        #endif
    }

//----------------------------------------------------------------------------
//      クラス種別返却
//----------------------------------------------------------------------------

    static unsigned getTypeFlags()
    {
        unsigned ret=0;

        if (THEOLIZER_GENERATED_CLASS_TYPE::Theolizer::kIsFullAuto) {
            ret |= theolizer::internal::ecfFullAuto;
        }

        if (theolizer::internal::THEOLIZER_GENERATED_ELEMENT_MAP
         == theolizer::internal::emOrder)
        {
            ret |= theolizer::internal::ecfOrder;
        }
        return ret;
    }

//----------------------------------------------------------------------------
//      要素リスト
//----------------------------------------------------------------------------

//      ---<<< 宣言 >>>---

    static const theolizer::internal::Element<TheolizerVersion>&
        getElementTheolizer(std::size_t iIndex);

//      ---<<< メンバ変数対応方法返却 >>>---

    static theolizer::internal::ElementsMapping getElementsMapping()
    {
        return Theolizer::kElementsMapping;
    }

//      ---<<< 枚挙用Range返却 >>>---

    static theolizer::internal::ElementRange getElementRange()
    {
        typedef theolizer::internal::Element
            <
                TheolizerVersion
                <
                    tBaseSerializer,
                    THEOLIZER_GENERATED_VERSION_NO
                >
            > const* Iterator;

        return  theolizer::internal::ElementRange
                (
                    Iterator(&getElementTheolizer(0)),
                    Iterator(&getElementTheolizer(getElementCount())),
                    getElementCount()
                );
    }
};

//      ---<<< 定義 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
const theolizer::internal::Element
<
    typename THEOLIZER_GENERATED_CLASS_TYPE::THEOLIZER_INTERNAL_TEMPLATE
    TheolizerVersion
    <
        tBaseSerializer,
        THEOLIZER_GENERATED_VERSION_NO
    >
>&
THEOLIZER_GENERATED_CLASS_TYPE::TheolizerVersion
<
    tBaseSerializer,
    THEOLIZER_GENERATED_VERSION_NO
>::getElementTheolizer(std::size_t iIndex)
{
    static const theolizer::internal::Element<TheolizerVersion>
        kElementList[]=
    {
        #if defined(THEOLIZER_GENERATED_BASE_LIST)
            #define THEOLIZER_GENERATED_SEP
            #define THEOLIZER_INTERNAL_VERSION_B(dSpec,dTrack,dId,dType,dVerNo,dVer,dAnno)\
                THEOLIZER_INTERNAL_REGISTER_BASE(dSpec, dTrack, dId, dType, dVerNo, dVer),
            #define THEOLIZER_INTERNAL_VERSION_B_DEL(dSpec,dTrack,dId,dType,dVerNo,dVer,dAnno)\
                THEOLIZER_INTERNAL_REGISTER_BASE(dSpec, dTrack, dId, dType, dVerNo, dVer),
            THEOLIZER_GENERATED_BASE_LIST()
            #undef  THEOLIZER_GENERATED_SEP
            #undef  THEOLIZER_INTERNAL_VERSION_B
            #undef  THEOLIZER_INTERNAL_VERSION_B_DEL
        #endif

        #if defined(THEOLIZER_GENERATED_ELEMENT_LIST)
            #define THEOLIZER_INTERNAL_VERSION_EN(dName,dNextName,          \
                dTrack,dDest,dType,dVer,dAnno)                              \
                    THEOLIZER_INTERNAL_REGISTER_ELEMENT(dName,dNextName,dTrack,dDest,dType,0,\
                        (THEOLIZER_INTERNAL_UNPAREN dVer&)),
            #define THEOLIZER_INTERNAL_VERSION_EN_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVer,dAnno)                              \
                    THEOLIZER_INTERNAL_REGISTER_ELEMENT(dName,dNextName,dTrack,dDest,dType,0,\
                        (THEOLIZER_INTERNAL_UNPAREN dVer&)),
            #define THEOLIZER_INTERNAL_VERSION_EV(dName,dNextName,          \
                dTrack,dDest,dType,dVerNo,dVer,dAnno,dDef)                  \
                    THEOLIZER_INTERNAL_REGISTER_ELEMENT(dName,dNextName,    \
                        dTrack,dDest,dType,dVerNo,dVer),
            #define THEOLIZER_INTERNAL_VERSION_EV_DEL(dName,dNextName,      \
                dTrack,dDest,dType,dVerNo,dVer,dAnno)                       \
                    THEOLIZER_INTERNAL_REGISTER_ELEMENT(dName,dNextName,    \
                        dTrack,dDest,dType,dVerNo,dVer),
            THEOLIZER_GENERATED_ELEMENT_LIST()
            #undef  THEOLIZER_INTERNAL_VERSION_EN
            #undef  THEOLIZER_INTERNAL_VERSION_EN_DEL
            #undef  THEOLIZER_INTERNAL_VERSION_EV
            #undef  THEOLIZER_INTERNAL_VERSION_EV_DEL
        #endif
        THEOLIZER_INTERNAL_REGISTER_SENTINEL(TheolizerVersion) // 番兵
    };
    return kElementList[iIndex];
}

// ***************************************************************************
//      TheolizerVersion<n!=1>用サービス関数定義(THEOLIZER_GENERATED_VERSION_NO+1について定義する)
// ***************************************************************************

#if (THEOLIZER_GENERATED_VERSION_NO != THEOLIZER_GENERATED_LAST_VERSION_NO)

//      ---<<< 保存 >>>---
//          基底Serializerから直接呼ばれる場合があるのでポストフィックスあり

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
void
THEOLIZER_GENERATED_CLASS_TYPE::TheolizerVersion
<
    tBaseSerializer,
    THEOLIZER_GENERATED_VERSION_NO+1
>::saveClass(tBaseSerializer& iSerializer, unsigned iVersionNo)
{
    if (iVersionNo < (THEOLIZER_GENERATED_VERSION_NO+1))
    {
        THEOLIZER_GENERATED_CLASS_TYPE::
            TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>
                aVersion(*this, iVersionNo, true);
        aVersion.saveClass(iSerializer, iVersionNo);
    }
    else
    {
        theolizer::internal::saveClass(iSerializer, *this);
    }
}

//      ---<<< 回復 >>>---
//          基底Serializerから直接呼ばれる場合があるのでポストフィックスあり

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
void
THEOLIZER_GENERATED_CLASS_TYPE::TheolizerVersion
<
    tBaseSerializer,
    THEOLIZER_GENERATED_VERSION_NO+1
>::loadClass(tBaseSerializer& iSerializer, unsigned iVersionNo)
{
    mTheolizerSpecials.mIsLoading=true; // デストラクト時upVersionを呼ぶため
    if (iVersionNo < (THEOLIZER_GENERATED_VERSION_NO+1))
    {
        THEOLIZER_GENERATED_CLASS_TYPE::
            TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>
                aVersion(*this, iVersionNo, false);
        aVersion.loadClass(iSerializer, iVersionNo);
    }
    else
    {
        theolizer::internal::loadClass(iSerializer, *this);
    }
}

//----------------------------------------------------------------------------
//      型チェック用
//----------------------------------------------------------------------------

//      ---<<< クラス名返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
std::string
THEOLIZER_GENERATED_CLASS_TYPE::TheolizerVersion
<
    tBaseSerializer,
    THEOLIZER_GENERATED_VERSION_NO+1
>::getClassName
(
    theolizer::internal::VersionNoList const& iVersionNoList,
    unsigned iVersionNo
)
{
    if (iVersionNo < (THEOLIZER_GENERATED_VERSION_NO+1))
    {
        return THEOLIZER_GENERATED_CLASS_TYPE::
            TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>::
            getClassName(iVersionNoList, iVersionNo);
    }

    return getClassName(iVersionNoList);
}

//      ---<<< メンバ変数対応方法返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
theolizer::internal::ElementsMapping
THEOLIZER_GENERATED_CLASS_TYPE::TheolizerVersion
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
        return THEOLIZER_GENERATED_CLASS_TYPE::
            TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>::
                getElementsMapping(iVersionNo);
    }

    return getElementsMapping();
}

//      ---<<< 要素枚挙用Range返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
theolizer::internal::ElementRange
THEOLIZER_GENERATED_CLASS_TYPE::TheolizerVersion
<
    tBaseSerializer,
    THEOLIZER_GENERATED_VERSION_NO+1
>::getElementRange(unsigned iVersionNo)
{
    if (iVersionNo < (THEOLIZER_GENERATED_VERSION_NO+1))
    {
        return THEOLIZER_GENERATED_CLASS_TYPE::
            TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>::
                getElementRange(iVersionNo);
    }

    return getElementRange();
}

//      ---<<< クラス種別返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
unsigned
THEOLIZER_GENERATED_CLASS_TYPE::TheolizerVersion
<
    tBaseSerializer,
    THEOLIZER_GENERATED_VERSION_NO+1
>::getTypeFlags(unsigned iVersionNo)
{
    if (iVersionNo < (THEOLIZER_GENERATED_VERSION_NO+1))
    {
        return THEOLIZER_GENERATED_CLASS_TYPE::
            TheolizerVersion<tBaseSerializer, THEOLIZER_GENERATED_VERSION_NO>::
                getTypeFlags(iVersionNo);
    }

    return  getTypeFlags();
}
#endif

// ***************************************************************************
//      TheolizerVersion<1>用サービス関数定義
// ***************************************************************************

#if (THEOLIZER_GENERATED_VERSION_NO == 1)

//      ---<<< 保存 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
void
THEOLIZER_GENERATED_CLASS_TYPE::TheolizerVersion<tBaseSerializer, 1>::saveClass
(
    tBaseSerializer& iSerializer,
    unsigned iVersionNo
)
{
    theolizer::internal::saveClass(iSerializer, *this);
}

//      ---<<< 回復 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
void
THEOLIZER_GENERATED_CLASS_TYPE::TheolizerVersion<tBaseSerializer, 1>::loadClass
(
    tBaseSerializer& iSerializer,
    unsigned iVersionNo
)
{
    mTheolizerSpecials.mIsLoading=true; // デストラクト時upVersionを呼ぶため
    theolizer::internal::loadClass(iSerializer, *this);
}

//----------------------------------------------------------------------------
//      型チェック用
//----------------------------------------------------------------------------

//      ---<<< クラス名返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
std::string
THEOLIZER_GENERATED_CLASS_TYPE::TheolizerVersion<tBaseSerializer, 1>::getClassName
(
    theolizer::internal::VersionNoList const& iVersionNoList,
    unsigned iVersionNo
)
{
    return getClassName(iVersionNoList);
}

//      ---<<< メンバ変数対応方法返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
theolizer::internal::ElementsMapping
THEOLIZER_GENERATED_CLASS_TYPE::TheolizerVersion<tBaseSerializer, 1>::getElementsMapping
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
THEOLIZER_GENERATED_CLASS_TYPE::TheolizerVersion<tBaseSerializer, 1>::getElementRange
(
    unsigned iVersionNo
)
{
    return getElementRange();
}

//      ---<<< クラス種別返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
template<class tBaseSerializer>
unsigned
THEOLIZER_GENERATED_CLASS_TYPE::TheolizerVersion<tBaseSerializer, 1>::getTypeFlags
(
    unsigned iVersionNo
)
{
    return getTypeFlags();
}
#endif

// ***************************************************************************
//      ターゲット・クラス用サービス関数定義
// ***************************************************************************

#if (THEOLIZER_GENERATED_VERSION_NO == 1)

//      ---<<< 保存 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
inline void
THEOLIZER_GENERATED_CLASS_TYPE::Theolizer::saveClass
(
    theolizer::internal::BaseSerializer& iSerializer,
    THEOLIZER_GENERATED_CLASS_TYPE*& iPointer,
    unsigned iVersionNo
)
{
    THEOLIZER_GENERATED_CLASS_TYPE::TheolizerVersion
    <
        theolizer::internal::BaseSerializer,
        Theolizer::kLastVersionNo
    > aVersion(*iPointer, iVersionNo, true);
    aVersion.saveClass(iSerializer, iVersionNo);
}

//      ---<<< 回復 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
inline void
THEOLIZER_GENERATED_CLASS_TYPE::Theolizer::loadClass
(
    theolizer::internal::BaseSerializer& iSerializer,
    THEOLIZER_GENERATED_CLASS_TYPE*& ioPointer,
    unsigned iVersionNo
)
{
    THEOLIZER_GENERATED_CLASS_TYPE::TheolizerVersion
    <
        theolizer::internal::BaseSerializer,
        Theolizer::kLastVersionNo
    > aVersion(*ioPointer, iVersionNo, false);
    aVersion.loadClass(iSerializer, iVersionNo);
}

//----------------------------------------------------------------------------
//      型チェック用
//----------------------------------------------------------------------------

//      ---<<< クラス名返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
inline std::string
THEOLIZER_GENERATED_CLASS_TYPE::Theolizer::getClassName
(
    theolizer::internal::VersionNoList const& iVersionNoList,
    unsigned iVersionNo
)
{
    return THEOLIZER_GENERATED_CLASS_TYPE::
        TheolizerVersion<theolizer::internal::BaseSerializer, Theolizer::kLastVersionNo>::
            getClassName(iVersionNoList, iVersionNo);
}

//      ---<<< ユニーク名返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
inline char const* THEOLIZER_GENERATED_CLASS_TYPE::Theolizer::getUniqueName()
{
    #if defined(THEOLIZER_GENERATED_UNIQUE_NAME)
        return THEOLIZER_INTERNAL_STRINGIZE(THEOLIZER_GENERATED_UNIQUE_NAME);
    #else
        THEOLIZER_INTERNAL_ABORT(THEOLIZER_INTERNAL_BUG);
    #endif
}

//      ---<<< メンバ変数対応方法返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
inline theolizer::internal::ElementsMapping THEOLIZER_GENERATED_CLASS_TYPE::Theolizer::
    getElementsMapping(unsigned iVersionNo)
{
    return THEOLIZER_GENERATED_CLASS_TYPE::
        TheolizerVersion
        <
            theolizer::internal::BaseSerializer,
            THEOLIZER_GENERATED_LAST_VERSION_NO
        >::getElementsMapping(iVersionNo);
}

//      ---<<< 要素枚挙用Range返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
inline theolizer::internal::ElementRange
THEOLIZER_GENERATED_CLASS_TYPE::Theolizer::getElementRange(unsigned iVersionNo)
{
    return THEOLIZER_GENERATED_CLASS_TYPE::
        TheolizerVersion
        <
            theolizer::internal::BaseSerializer,
            THEOLIZER_GENERATED_LAST_VERSION_NO
        >::getElementRange(iVersionNo);
}

//      ---<<< クラス種別返却 >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
inline unsigned
THEOLIZER_GENERATED_CLASS_TYPE::Theolizer::getTypeFlags(unsigned iVersionNo)
{
    return THEOLIZER_GENERATED_CLASS_TYPE::
        TheolizerVersion
        <
            theolizer::internal::BaseSerializer,
            THEOLIZER_GENERATED_LAST_VERSION_NO
        >::getTypeFlags(iVersionNo);
}

//----------------------------------------------------------------------------
//      ポリモーフィズム対応
//----------------------------------------------------------------------------

//      ---<<< 基底クラスへ自分を登録する >>>---

THEOLIZER_GENERATED_PARAMETER_LIST
inline void THEOLIZER_GENERATED_CLASS_TYPE::Theolizer::registerToBaseClass
(
    theolizer::internal::BaseTypeInfo* iBaseTypeInfo
)
{
    THEOLIZER_GENERATED_CLASS_TYPE::TheolizerVersion
    <
        theolizer::internal::BaseSerializer,
        Theolizer::kLastVersionNo
    >::registerToBaseClass2();
}
#endif

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
    #undef THEOLIZER_INTERNAL_IS_FULL_AUTO
    #undef THEOLIZER_INTERNAL_TARGET
    #undef THEOLIZER_GENERATED_LAST_VERSION_NO
    #undef THEOLIZER_GENERATED_CLASS_TYPE
    #undef THEOLIZER_GENERATED_PARAMETER_LIST
    #undef THEOLIZER_GENERATED_UNIQUE_NAME
#endif

#undef THEOLIZER_GENERATED_CLASS_NAME
#undef THEOLIZER_GENERATED_ELEMENT_MAP
#undef THEOLIZER_GENERATED_BASE_LIST
#undef THEOLIZER_GENERATED_ELEMENT_LIST

// ***************************************************************************
//          警告抑止解除
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#if defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
