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

    // 同じバージョン
    TheolizerVersion*               mSameVersion;

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
    BaseType    mValue;
    BaseType    mBackup;

//      ---<<< バージョン・アップ時の引継ぎ処理用 >>>---

    unsigned    mUpVersionCount;    // upVersionカウンタ
    bool        mDoSucceed;         // 引継ぎフラグ
    bool        mIsChanged;         // これを含むクラスのupVersionにて変更された

    // 通常設定(旧バージョンはenum型が異なるので、数値で設定する)
    TheolizerVersion& operator=(BaseType iRhs)
    {
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "enum::TheolizerVersion<" << THEOLIZER_GENERATED_VERSION_NO
              << ">::operator=(" << iRhs
              << "); " << mTarget << "\n";
    std::cout << "    mUpVersionCount    =" << mUpVersionCount << "\n";
    std::cout << "    getUpVersionCount()=" << theolizer::internal::getUpVersionCount() << "\n";
));
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
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "    mValue               =" << mValue << "\n";
    std::cout << "    mIsChanged           =" << mIsChanged << "\n";
    std::cout << "    mDoSucceed           =" << mDoSucceed << "\n";
));
        return *this;
    }

    // BaseType以外は非サポート
    template<typename tType>
    TheolizerVersion& operator=(tType const& iRhs)
    {
        static_assert(!std::is_same<tType, TheolizerVersion>::value,
            "Can not assign enum type to prepare for changing enum definition.");
        static_assert(std::is_same<tType, TheolizerVersion>::value,
            "Can not supprt this type.");
    }

    // 強制引継ぎ設定（他の変数から引き継ぐ）
    void set(BaseType iValue, bool iDoSucceed)  // iDoSucceedは引継ぎ元変数のmDoSucceed
    {
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "enum::TheolizerVersion<" << THEOLIZER_GENERATED_VERSION_NO
              << ">::succeed(" << iValue << ", " << iDoSucceed
              << "); " << mTarget << "\n";
    std::cout << "    mUpVersionCount    =" << mUpVersionCount << "\n";
    std::cout << "    getUpVersionCount()=" << theolizer::internal::getUpVersionCount() << "\n";
));
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
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "    mValue               =" << mValue << "\n";
    std::cout << "    mIsChanged           =" << mIsChanged << "\n";
    std::cout << "    mDoSucceed           =" << mDoSucceed << "\n";
));
    }

    // 前バージョンのデストラクタから呼ばれる引継ぎ処理
    void succeed(BaseType iValue, bool iDoSucceed)
    {
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "enum::TheolizerVersion<" << THEOLIZER_GENERATED_VERSION_NO
              << ">::succeed(" << iValue << ", " << iDoSucceed
              << "); " << mTarget << "\n";
    std::cout << "    mIsChanged =" << mIsChanged << "\n";
));

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
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "    mValue               =" << mValue << "\n";
    std::cout << "    mIsChanged           =" << mIsChanged << "\n";
    std::cout << "    mDoSucceed           =" << mDoSucceed << "\n";
));
    }

//      ---<<< 配列処理の中身へのアクセス用 >>>---

    typedef TheolizerVersion AsElementOfArray;
    AsElementOfArray&       getAccess()       { return *this; }
    AsElementOfArray const& getAccess() const { return *this; }

//      ---<<< up/downVersion()関数内でのアクセス用 >>>---

    BaseType get()        const { return mValue; }
    operator BaseType()   const { return get(); }
    bool getDoSucceed()          const { return mDoSucceed; }

    // 非constはライト・オンリー
private:
    BaseType get();
    operator BaseType();
public:

//----------------------------------------------------------------------------
//      コンストラクタ(次バージョンからコンストラクト)
//          古い版からの場合は、TheolizerVersion<>なのでconstを付けて良い。
//          配列でイニシャライザ・リストでの初期化時、必要になる。
//----------------------------------------------------------------------------

#if (THEOLIZER_GENERATED_VERSION_NO == THEOLIZER_GENERATED_LAST_VERSION_NO)
    explicit TheolizerVersion(ParentVersion& iParentVersion) :
        mTarget(&iParentVersion),
        mParentVersion(&iParentVersion),
        mSameVersion(nullptr),
        mValue(static_cast<BaseType>(iParentVersion)),
        mBackup(mValue),
        mUpVersionCount(0),
        mDoSucceed(false),
        mIsChanged(false)
    {
        // 内部的にstd::stringを使うので登録する
        theolizer::internal::RegisterType
        <
            tBaseSerializer,
            std::string
        >::getInstance();
    }
#else
    explicit TheolizerVersion(ParentVersion & iParentVersion) :
        mTarget(iParentVersion.mTarget),
        mParentVersion(&iParentVersion),
        mSameVersion(nullptr),
        mValue(iParentVersion.mValue),
        mBackup(mValue),
        mUpVersionCount(0),
        mDoSucceed(false),
        mIsChanged(false)
    {   // 最新版でない時のみバージョン・ダウン実施
        downVersion();
    }
#endif

//----------------------------------------------------------------------------
//      コンストラクタ(同じバージョンからコンストラクト)
//----------------------------------------------------------------------------

    explicit TheolizerVersion(TheolizerVersion& iSameVersion) :
        mTarget(iSameVersion.mTarget),
        mParentVersion(nullptr),
        mSameVersion(&iSameVersion),
        mValue(iSameVersion.mValue),
        mBackup(mValue),
        mUpVersionCount(0),
        mDoSucceed(false),
        mIsChanged(false)
    {
    }

//----------------------------------------------------------------------------
//      ムーブ・コンストラクタ(同じバージョンからコンストラクト)
//          次バージョンで削除されたもの対応で、生成された一時オブジェクトを引き継ぐ
//          生配列の次元数／要素数変更対応に必要
//----------------------------------------------------------------------------

    explicit TheolizerVersion(TheolizerVersion&& iSameVersion) :
        mTarget(iSameVersion.mTarget),
        mParentVersion(nullptr),
        mSameVersion(nullptr),
        mValue(iSameVersion.mValue),
        mBackup(mValue),
        mUpVersionCount(0),
        mDoSucceed(false),
        mIsChanged(false)
    {
    }

//----------------------------------------------------------------------------
//      コンストラクタ(親無し)
//----------------------------------------------------------------------------

    TheolizerVersion() :
        mTarget(nullptr),
        mParentVersion(nullptr),
        mSameVersion(nullptr),
        mValue(THEOLIZER_GENERATED_DEFAULT_VALUE),
        mBackup(mValue),
        mUpVersionCount(0),
        mDoSucceed(false),
        mIsChanged(false)
    {
    }

//----------------------------------------------------------------------------
//      デストラクタ
//----------------------------------------------------------------------------

    ~TheolizerVersion()
    {   // 最新版でない時のみバージョン・アップ実施
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "enum::~TheolizerVersion<" << THEOLIZER_GENERATED_VERSION_NO << "> "
              << mTarget << "\n";
    std::cout << "    mValue               =" << mValue << "\n";
    std::cout << "    mIsChanged           =" << mIsChanged << "\n";
    std::cout << "    mDoSucceed           =" << mDoSucceed << "\n";
));
        #if (THEOLIZER_GENERATED_VERSION_NO == THEOLIZER_GENERATED_LAST_VERSION_NO)
THEOLIZER_INTERNAL_VER_DEBUG((std::cout << "    mParentVersion=" << mParentVersion << "\n";));
            if (mParentVersion && mDoSucceed)
            {
THEOLIZER_INTERNAL_VER_DEBUG((std::cout << "    *mParentVersion=" << mValue << ";\n";));
                *mParentVersion = static_cast<THEOLIZER_GENERATED_ENUM_TYPE>(mValue);
            }
            else if (mSameVersion)
            {
THEOLIZER_INTERNAL_VER_DEBUG((std::cout << "    mSameVersion->succeed(" << mValue << ");\n";));
                mSameVersion->succeed(mValue, mDoSucceed);
            }
        #else
            if (mParentVersion)
            {
                // 変更済か引継ぎ無しなら、バーション・アップを省略する
                if (!mParentVersion->mIsChanged && mDoSucceed)
                {
                    upVersion();
                }

                // 引継ぎ処理
THEOLIZER_INTERNAL_VER_DEBUG((std::cout << "    mParentVersion->succeed(" << mValue << ");\n";));
                mParentVersion->succeed(mValue, mDoSucceed);
            }
            else if (mSameVersion)
            {
THEOLIZER_INTERNAL_VER_DEBUG((std::cout << "    mSameVersion->succeed(" << mValue << ");\n";));
                mSameVersion->succeed(mValue, mDoSucceed);
            }
        #endif
    }

//----------------------------------------------------------------------------
//      バージョン・ダウン処理
//----------------------------------------------------------------------------

    #if (THEOLIZER_GENERATED_VERSION_NO != THEOLIZER_GENERATED_LAST_VERSION_NO)
    void downVersion()
    {
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "enum::downVersion<"
              << THEOLIZER_GENERATED_VERSION_NO << "> " << mTarget << "\n";
    std::cout << "    mParentVersion->mValue=" << mParentVersion->mValue << "\n";
));
        for (size_t i=0; !mParentVersion->kEnumElementList[i].isSentinel(); ++i)
        {
            if (mParentVersion->kEnumElementList[i].mValues[0] == mParentVersion->mValue)
            {
                mBackup=mValue=mParentVersion->kEnumElementList[i].mPrevValue;
THEOLIZER_INTERNAL_VER_DEBUG((std::cout << "    mValue=" << mValue << "\n";));
    return;
            }
        }

        // 見つからなければ、警告しデフォルト値へ変換する。
        mBackup=mValue=THEOLIZER_GENERATED_DEFAULT_VALUE;
        THEOLIZER_INTERNAL_WARNING(
            "Can not transform Version.%1%'s value(%2%) to Version.%3%'s value. "
            "Please check THEOLIZER_ANNOTATE() for %4%.",
            THEOLIZER_GENERATED_VERSION_NO+1,
            mParentVersion->mValue,
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
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "enum::upVersion<"
          << THEOLIZER_GENERATED_VERSION_NO << "> " << mTarget << "\n";
));
        for (size_t i=0; !mParentVersion->kEnumElementList[i].isSentinel(); ++i)
        {
            if (mValue == mParentVersion->kEnumElementList[i].mPrevValue)
            {
                *mParentVersion=mParentVersion->kEnumElementList[i].mValues[0];
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "    (1)*mParentVersion=" << mParentVersion->mValue << ";\n";
));
    return;
            }
        }

        // 見つからなければ、警告しデフォルト値へ変換する。
        *mParentVersion=static_cast<BaseType>(THEOLIZER_GENERATED_DEFAULT_VALUE);
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "    (2)*mParentVersion=" << mParentVersion->mValue << ";\n";
));
        THEOLIZER_INTERNAL_WARNING(
            "Can not transform Version.%1%'s value(%2%) to Version.%3%'s value. "
            "Please check THEOLIZER_ANNOTATE() for %4%.",
            THEOLIZER_GENERATED_VERSION_NO,
            mValue,
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
            THEOLIZER_INTERNAL_SAVE(iSerializer, mValue, etmDefault);
    return;
        }

        // 名前を保存する時の処理
        for (size_t i=0; !kEnumElementList[i].isSentinel(); ++i)
        {
            if (kEnumElementList[i].mValues[0] == mValue)
            {
                std::string aEnumSymbol(kEnumElementList[i].mSymbols[0]);
                THEOLIZER_INTERNAL_SAVE(iSerializer, aEnumSymbol, etmDefault);
    return;
            }
        }

        // 見つからなければ、警告しデフォルト値を保存する
        std::string aEnumSymbol(kEnumElementList[0].mSymbols[0]);
        THEOLIZER_INTERNAL_SAVE(iSerializer, aEnumSymbol, etmDefault);
        THEOLIZER_INTERNAL_WARNING(
            "Can not find enum symbol's value(%1%) at Version.%2%. "
            "Please check THEOLIZER_ANNOTATE() for %3%.",
            mValue,
            THEOLIZER_GENERATED_VERSION_NO,
            THEOLIZER_GENERATED_ENUM_NAME);
    }

//----------------------------------------------------------------------------
//      回復処理
//----------------------------------------------------------------------------

    void loadEnumImpl(tBaseSerializer& iSerializer)
    {
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "loadEnumImpl(" << THEOLIZER_GENERATED_VERSION_NO << ") " << mTarget << "\n";
));
        mDoSucceed=true;
        // 値で回復する時の処理（既知の値であることをチェックする）
        if (Theolizer::kEnumSaveType == theolizer::internal::estValue)
        {
            THEOLIZER_INTERNAL_LOAD(iSerializer, mValue, etmDefault);
            for (size_t i=0; !kEnumElementList[i].isSentinel(); ++i)
            {
                for (size_t j=0; j < kEnumElementList[i].mValues.size(); ++j)
                {
                    if (mValue == kEnumElementList[i].mValues[j])
                    {
                        mValue=kEnumElementList[i].mValues[0];
THEOLIZER_INTERNAL_VER_DEBUG((std::cout << "    (1)mValue=" << mValue << "\n";));
    return;
                    }
                }
            }

        mValue=THEOLIZER_GENERATED_DEFAULT_VALUE;
        THEOLIZER_INTERNAL_WARNING(
            u8"Can not find loaded value(%1%) in Version.%2%'s symbol. "
            "Please check THEOLIZER_ANNOTATE() for %3%.",
            mValue,
            THEOLIZER_GENERATED_VERSION_NO,
            THEOLIZER_GENERATED_ENUM_NAME);
THEOLIZER_INTERNAL_VER_DEBUG((std::cout << "    (2)mValue=" << mValue << "\n";));
    return;
        }

        // 名前で回復する時の処理
        std::string  aName;
        THEOLIZER_INTERNAL_LOAD(iSerializer, aName, etmDefault);
THEOLIZER_INTERNAL_VER_DEBUG((std::cout << "    aName=" << aName << "\n";));

        mValue=THEOLIZER_GENERATED_DEFAULT_VALUE;
        for (size_t i=0; !kEnumElementList[i].isSentinel(); ++i)
        {
            for (size_t j=0; j < kEnumElementList[i].mSymbols.size(); ++j)
            {
                if (aName == kEnumElementList[i].mSymbols[j])
                {
                    mValue=kEnumElementList[i].mValues[0];
THEOLIZER_INTERNAL_VER_DEBUG((std::cout << "    (3)mValue=" << mValue << "\n";));
    return;
                }
            }
        }

        // 見つからなければ、警告しデフォルト値として回復する。
        mValue=THEOLIZER_GENERATED_DEFAULT_VALUE;
        THEOLIZER_INTERNAL_WARNING(
            u8"Can not find loaded symbol(%1%) in Version.%2%'s symbol. "
            "Please check THEOLIZER_ANNOTATE() for %3%.",
            aName,
            THEOLIZER_GENERATED_VERSION_NO,
            THEOLIZER_GENERATED_ENUM_NAME);
THEOLIZER_INTERNAL_VER_DEBUG((std::cout << "    (4)mValue=" << mValue << "\n";));
    }

//----------------------------------------------------------------------------
//      enum種別返却
//----------------------------------------------------------------------------

    static unsigned getTypeFlags()
    {
        unsigned    ret=0;

        // 基底型
        switch(theolizer::internal::getEnumUnderlyng<BaseType>())
        {
        case      1:    ret |= theolizer::internal::TypeKind::Bool;     break;
        case 128+ 8:    ret |= theolizer::internal::TypeKind::Int8;     break;
        case      8:    ret |= theolizer::internal::TypeKind::UInt8;    break;
        case 128+16:    ret |= theolizer::internal::TypeKind::Int16;    break;
        case     16:    ret |= theolizer::internal::TypeKind::UInt16;   break;
        case 128+32:    ret |= theolizer::internal::TypeKind::Int32;    break;
        case     32:    ret |= theolizer::internal::TypeKind::UInt32;   break;
        case 128+64:    ret |= theolizer::internal::TypeKind::Int64;    break;
        case     64:    ret |= theolizer::internal::TypeKind::UInt64;   break;
        }

        // scoped/unscoped
        if (!std::is_convertible<THEOLIZER_GENERATED_ENUM_TYPE, BaseType>::value)
        {
            ret |= theolizer::internal::TypeKind::Scoped;
        }

        // Value/Name
        if (Theolizer::kEnumSaveType == theolizer::internal::estValue)
        {
            ret |= theolizer::internal::TypeKind::SaveValue;
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
            {THEOLIZER_INTERNAL_UNPAREN dValues}, THEOLIZER_INTERNAL_UNPAREN dPrevValue),
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
    if (iVersionNo < (THEOLIZER_GENERATED_VERSION_NO+1))
    {
        THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
        <
            tBaseSerializer,
            THEOLIZER_GENERATED_VERSION_NO
        > aVersion(*this);
        aVersion.saveEnum(iSerializer, iVersionNo);
return;
    }
    else
    {
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
    if (iVersionNo < (THEOLIZER_GENERATED_VERSION_NO+1))
    {
        THEOLIZER_INTERNAL_DERIVED::TheolizerVersion
        <
            tBaseSerializer,
            THEOLIZER_GENERATED_VERSION_NO
        > aVersion(*this);
        aVersion.loadEnum(iSerializer, iVersionNo);
    }
    else
    {
        loadEnumImpl(iSerializer);

        // バージョン・アップ開始(upVersionカウンタ=1)
        theolizer::internal::getUpVersionCount()=1;
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "(4)UpVersionCount=" << theolizer::internal::getUpVersionCount()
              << " " << THEOLIZER_INTERNAL_TYPE_NAME_VAR(*this) << std::endl;
));
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
    if (iVersionNo < (THEOLIZER_GENERATED_VERSION_NO+1))
    {
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
    loadEnumImpl(iSerializer);

    // バージョン・アップ開始(upVersionカウンタ=1)
    theolizer::internal::getUpVersionCount()=1;
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "(3)UpVersionCount=" << theolizer::internal::getUpVersionCount()
              << " " << THEOLIZER_INTERNAL_TYPE_NAME_VAR(*this) << std::endl;
));
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

    // データ内に型名保存
    if (iSerializer.mCheckMode == theolizer::CheckMode::TypeCheckInData)
    {
        auto aTypeIndex=theolizer::internal::getTypeIndex<THEOLIZER_GENERATED_ENUM_TYPE>();
        theolizer::internal::BaseSerializer::AutoRestoreSaveStructure
            aAutoRestoreSaveStructure
            (
                iSerializer,
                theolizer::internal::emOrder,
                theolizer::internal::Structure::None,
                aTypeIndex
            );
        iSerializer.writePreElement();
        aVersion.saveEnum(iSerializer, iVersionNo);
    }
    else
    {
        aVersion.saveEnum(iSerializer, iVersionNo);
    }
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

    // データ内に型名保存
    if (iSerializer.mCheckMode == theolizer::CheckMode::TypeCheckInData)
    {
        auto aTypeIndex=theolizer::internal::getTypeIndex<THEOLIZER_GENERATED_ENUM_TYPE>();
        theolizer::internal::BaseSerializer::AutoRestoreLoadStructure
            aAutoRestoreLoadStructure
            (
                iSerializer,
                theolizer::internal::emOrder,
                theolizer::internal::Structure::None,
                aTypeIndex
            );
        if (!iSerializer.readPreElement())
        {
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Enum Format Error.(%1%)",
                THEOLIZER_INTERNAL_STRINGIZE(THEOLIZER_GENERATED_ENUM_TYPE));
        }

        aVersion.loadEnum(iSerializer, iVersionNo);
    }
    else
    {
        aVersion.loadEnum(iSerializer, iVersionNo);
    }
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
