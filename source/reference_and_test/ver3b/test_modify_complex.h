//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          クラス変更の複合テスト
//              オブジェクト追跡
//              バージョン・アップ／ダウン処理（参照テスト含む）
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

#if !defined(TEST_MODIFY_COMPLEX_H)
#define TEST_MODIFY_COMPLEX_H

#include <theolizer/memory.h>   // for std::unique_ptr<>

// ***************************************************************************
//      オブジェクト追跡のバージョン変更テスト
// ***************************************************************************

//----------------------------------------------------------------------------
//      他のクラスに含まれるクラス（侵入型半自動）
//----------------------------------------------------------------------------

struct PointeeClass
{
    int     mInt;

    PointeeClass(int iInt=0) : mInt(iInt) { }
    THEOLIZER_INTRUSIVE(CS, (PointeeClass), 1);
};

//----------------------------------------------------------------------------
//      他のクラスを含む被ポイント側クラス
//----------------------------------------------------------------------------

struct PointeeInclude
{
    std::unique_ptr<PointeeClass>   mPointeeClassOwner;                 // オーナ指定ポインタ
                                                                        // 被ポインタ実体
    PointeeClass    mPointeeClassX   THEOLIZER_ANNOTATE(FS:mPointeeClass<>Pointee);
    PointeeClass&   mPointeeClassRef THEOLIZER_ANNOTATE(FS:<>Pointee);  // 被ポインタ参照

    PointeeInclude(PointeeClass& iPointeeClassRef) :
        mPointeeClassOwner(new PointeeClass(0)),
        mPointeeClassX(0),
        mPointeeClassRef(iPointeeClassRef)
    { }

    PointeeInclude(PointeeClass& iPointeeClassRef, bool) :
        mPointeeClassOwner(new PointeeClass(100)),
        mPointeeClassX(101),
        mPointeeClassRef(iPointeeClassRef)
    {
        mPointeeClassRef.mInt=102;
    }

    void check()
    {
        THEOLIZER_EQUAL(mPointeeClassOwner->mInt,   100);
        THEOLIZER_EQUAL(mPointeeClassX.mInt,        101);
        THEOLIZER_EQUAL(mPointeeClassRef.mInt,      102);
    }
    THEOLIZER_INTRUSIVE(CS, (PointeeInclude), 3);
};

//----------------------------------------------------------------------------
//      他のクラスを含むポインタ側クラス
//          ver1 : 
//              mPointeeClass0 → mPointeeClassOwner
//              mPointeeClass1 → mPointeeClassX
//              mPointeeClass2 → mPointeeClassRef
//----------------------------------------------------------------------------

struct PointerInclude
{
    PointeeClass*   mPointeeClass0;
    PointeeClass*   mPointeeClass1;
    PointeeClass*   mPointeeClass2;

    PointerInclude() :
        mPointeeClass0(nullptr),
        mPointeeClass1(nullptr),
        mPointeeClass2(nullptr)
    { }

    PointerInclude(PointeeInclude& iPointeeInclude) :
        mPointeeClass0(&iPointeeInclude.mPointeeClassRef),
        mPointeeClass1( iPointeeInclude.mPointeeClassOwner.get()),
        mPointeeClass2(&iPointeeInclude.mPointeeClassX)
    { }

    void check(PointeeInclude& iPointeeInclude)
    {
        switch(gVersionList[gProgramIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
        case VersionEnum::ver1b:
        case VersionEnum::ver1c:
            THEOLIZER_EQUAL_PTR(mPointeeClass0,  iPointeeInclude.mPointeeClassOwner.get());
            THEOLIZER_EQUAL_PTR(mPointeeClass1, &iPointeeInclude.mPointeeClassX);
            THEOLIZER_EQUAL_PTR(mPointeeClass2, &iPointeeInclude.mPointeeClassRef);
            break;

        case VersionEnum::ver2a:
            THEOLIZER_EQUAL_PTR(mPointeeClass0, &iPointeeInclude.mPointeeClassX);
            THEOLIZER_EQUAL_PTR(mPointeeClass1, &iPointeeInclude.mPointeeClassRef);
            THEOLIZER_EQUAL_PTR(mPointeeClass2,  iPointeeInclude.mPointeeClassOwner.get());
            break;

        case VersionEnum::ver3a:
        case VersionEnum::ver3b:
            THEOLIZER_EQUAL_PTR(mPointeeClass0, &iPointeeInclude.mPointeeClassRef);
            THEOLIZER_EQUAL_PTR(mPointeeClass1,  iPointeeInclude.mPointeeClassOwner.get());
            THEOLIZER_EQUAL_PTR(mPointeeClass2, &iPointeeInclude.mPointeeClassX);
            break;

        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }
    THEOLIZER_INTRUSIVE(CS, (PointerInclude), 3);
};

// ***************************************************************************
//      バージョン・アップ／ダウンのテスト
//          バージョン・アップ組合せテストは
//              ver1cは準備
//              ver2a, ver3aにて主テスト
//              ver3bはNOP
// ***************************************************************************

//----------------------------------------------------------------------------
//      非可逆バージョン・アップの原因となるenum型
//          良くあるバージョン・アップを想定
//----------------------------------------------------------------------------

enum class ClassKind
{
    Default,
    Kind1,
    Kind2   THEOLIZER_ANNOTATE(ES:::1),

    // enum型のバージョン・アップ組合せテスト用シンボル定義
    Origin  =100,
    Load    =200,
    Set     =300,
    Ope     =400,
    Inner   =500
};
THEOLIZER_ENUM(ClassKind, 2);

//      ---<<< バージョン変更に備える >>>---

static const int kClassKind1Default=0;
static const int kClassKind1Kind1  =1;
static const int kClassKind1Origin=100;
static const int kClassKind1Load  =200;
static const int kClassKind1Set   =300;
static const int kClassKind1Ope   =400;
static const int kClassKind1Inner =500;

static const int kClassKind2Default=0;
static const int kClassKind2Kind1  =1;
static const int kClassKind2Kind2  =2;
static const int kClassKind2Origin=100;
static const int kClassKind2Load  =200;
static const int kClassKind2Set   =300;
static const int kClassKind2Ope   =400;
static const int kClassKind2Inner =500;

//      ---<<< テスト補助 >>>---

#if 0
inline std::ostream& operator<<(std::ostream& iOStream, ClassKind iClassKind)
{
    iOStream << "[" << static_cast<int>(iClassKind) << "] ";
    return iOStream;
}
#endif

//----------------------------------------------------------------------------
//      プリミティブ(int型)のバージョン・アップ組合せテスト用シンボル定義
//----------------------------------------------------------------------------

enum IntSymbol
{
    eisOrigin   =100,
    eisLoad     =200,
    eisSet      =300,
    eisOpe      =400,
    eisInner    =500
};

//----------------------------------------------------------------------------
//      バージョン・アップ組合せテスト（保存先指定含む）
//----------------------------------------------------------------------------

struct VersionUpDownTest
{
    ClassKind   mClassKind  THEOLIZER_ANNOTATE(FS:<theolizerD::DestA>);
    int         mData0      THEOLIZER_ANNOTATE(FS:<theolizerD::DestA>);
    int         mData1      THEOLIZER_ANNOTATE(FS:<theolizerD::DestB>);

    // Non-keep-stepのバージョン・アップ組合せテスト
    ClassKind   mEnum00;
    ClassKind   mEnum01;
    ClassKind   mEnum02;
    ClassKind   mEnum03;
    ClassKind   mEnum04;
    ClassKind   mEnum05;
    ClassKind   mEnum06;
    ClassKind   mEnum10;
    ClassKind   mEnum11;
    ClassKind   mEnum12;
    ClassKind   mEnum13;
    ClassKind   mEnum14;
    ClassKind   mEnum15;
    ClassKind   mEnum16;

    int         mInt00;
    int         mInt01;
    int         mInt02;
    int         mInt03;
    int         mInt04;
    int         mInt05;
    int         mInt06;
    int         mInt10;
    int         mInt11;
    int         mInt12;
    int         mInt13;
    int         mInt14;
    int         mInt15;
    int         mInt16;

    // コンストラクタ
    VersionUpDownTest(ClassKind iClassKind=ClassKind::Default) :
        mClassKind(iClassKind),
        mData0(0),
        mData1(0),
        mEnum00((iClassKind==ClassKind::Default)?ClassKind::Origin:ClassKind::Load),
        mEnum01(mEnum00),
        mEnum02(mEnum00),
        mEnum03(mEnum00),
        mEnum04(mEnum00),
        mEnum05(mEnum00),
        mEnum06(mEnum00),
        mEnum10(mEnum00),
        mEnum11(mEnum00),
        mEnum12(mEnum00),
        mEnum13(mEnum00),
        mEnum14(mEnum00),
        mEnum15(mEnum00),
        mEnum16(mEnum00),
        mInt00((iClassKind==ClassKind::Default)?eisOrigin:eisLoad),
        mInt01(mInt00),
        mInt02(mInt00),
        mInt03(mInt00),
        mInt04(mInt00),
        mInt05(mInt00),
        mInt06(mInt00),
        mInt10(mInt00),
        mInt11(mInt00),
        mInt12(mInt00),
        mInt13(mInt00),
        mInt14(mInt00),
        mInt15(mInt00),
        mInt16(mInt00)
    {
        switch(mClassKind)
        {
        case ClassKind::Default:
            break;

        case ClassKind::Kind1:
            mData0=1110;
            break;

        case ClassKind::Kind2:
            mData0=2210;
            break;

        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
        if (mClassKind != ClassKind::Default)
        {
            mData1=110020;
        }
    }

    // チェック
    void check(ClassKind iClassKind)
    {
        ClassKind aClassKind=iClassKind;
        if (gVersionList[gDataIndex].mVersionEnum < VersionEnum::ver2a)
        {
            aClassKind=ClassKind::Kind1;
        }

        THEOLIZER_EQUAL(mClassKind, aClassKind);
        switch(mClassKind)
        {
        case ClassKind::Default:
            THEOLIZER_EQUAL(mData0, 0);
            THEOLIZER_EQUAL(mData1, 0);
            break;

        case ClassKind::Kind1:
            THEOLIZER_EQUAL(mData0, 1110);
            THEOLIZER_EQUAL(mData1, 110020);
            break;

        case ClassKind::Kind2:
            THEOLIZER_EQUAL(mData0, 2210);
            THEOLIZER_EQUAL(mData1, 110020);
            break;

        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }

        // Non-keep-stepのバージョン・アップ組合せテスト
        switch(gVersionList[gDataIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
        case VersionEnum::ver1b:
            THEOLIZER_EQUAL(mEnum00, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum01, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum02, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum03, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum04, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum05, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum06, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum10, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum11, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum12, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum13, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum14, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum15, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum16, ClassKind::Origin);

            THEOLIZER_EQUAL(mInt00, eisOrigin);
            THEOLIZER_EQUAL(mInt01, eisOrigin);
            THEOLIZER_EQUAL(mInt02, eisOrigin);
            THEOLIZER_EQUAL(mInt03, eisOrigin);
            THEOLIZER_EQUAL(mInt04, eisOrigin);
            THEOLIZER_EQUAL(mInt05, eisOrigin);
            THEOLIZER_EQUAL(mInt06, eisOrigin);
            THEOLIZER_EQUAL(mInt10, eisOrigin);
            THEOLIZER_EQUAL(mInt11, eisOrigin);
            THEOLIZER_EQUAL(mInt12, eisOrigin);
            THEOLIZER_EQUAL(mInt13, eisOrigin);
            THEOLIZER_EQUAL(mInt14, eisOrigin);
            THEOLIZER_EQUAL(mInt15, eisOrigin);
            THEOLIZER_EQUAL(mInt16, eisOrigin);
            break;

        case VersionEnum::ver1c:
            THEOLIZER_EQUAL(mEnum00, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum01, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum02, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum03, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum04, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum05, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum06, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum10, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum11, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum12, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum13, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum14, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum15, ClassKind::Origin);
            THEOLIZER_EQUAL(mEnum16, ClassKind::Origin);

            THEOLIZER_EQUAL(mInt00, eisLoad);
            THEOLIZER_EQUAL(mInt01, eisLoad);
            THEOLIZER_EQUAL(mInt02, eisLoad);
            THEOLIZER_EQUAL(mInt03, eisLoad);
            THEOLIZER_EQUAL(mInt04, eisLoad);
            THEOLIZER_EQUAL(mInt05, eisLoad);
            THEOLIZER_EQUAL(mInt06, eisLoad);
            THEOLIZER_EQUAL(mInt10, eisOrigin);
            THEOLIZER_EQUAL(mInt11, eisOrigin);
            THEOLIZER_EQUAL(mInt12, eisOrigin);
            THEOLIZER_EQUAL(mInt13, eisOrigin);
            THEOLIZER_EQUAL(mInt14, eisOrigin);
            THEOLIZER_EQUAL(mInt15, eisOrigin);
            THEOLIZER_EQUAL(mInt16, eisOrigin);
            break;

        case VersionEnum::ver2a:
        case VersionEnum::ver3a:
        case VersionEnum::ver3b:
            THEOLIZER_EQUAL(mEnum00, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum01, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum02, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum03, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum04, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum05, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum06, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum10, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum11, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum12, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum13, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum14, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum15, ClassKind::Load);
            THEOLIZER_EQUAL(mEnum16, ClassKind::Load);

            THEOLIZER_EQUAL(mInt00, eisLoad);
            THEOLIZER_EQUAL(mInt01, eisLoad);
            THEOLIZER_EQUAL(mInt02, eisLoad);
            THEOLIZER_EQUAL(mInt03, eisLoad);
            THEOLIZER_EQUAL(mInt04, eisLoad);
            THEOLIZER_EQUAL(mInt05, eisLoad);
            THEOLIZER_EQUAL(mInt06, eisLoad);
            THEOLIZER_EQUAL(mInt10, eisLoad);
            THEOLIZER_EQUAL(mInt11, eisLoad);
            THEOLIZER_EQUAL(mInt12, eisLoad);
            THEOLIZER_EQUAL(mInt13, eisLoad);
            THEOLIZER_EQUAL(mInt14, eisLoad);
            THEOLIZER_EQUAL(mInt15, eisLoad);
            THEOLIZER_EQUAL(mInt16, eisLoad);
            break;

        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }

    THEOLIZER_INTRUSIVE(CS, (VersionUpDownTest), 3);
};

//      ---<<< バージョン・アップ／ダウン処理 ver1 <-> ver2 >>>---

template<class tTheolizerVersion, class tNextVersion>
struct VersionUpDownTest::TheolizerUserDefine<tTheolizerVersion, tNextVersion, 1>
{
    // Initialize members that is deleted in next version.
    static void initialize(tTheolizerVersion& oNowVersion)
    {
    }

    // Members version down.
    static void downVersion(tNextVersion const& iNextVersion, tTheolizerVersion& oNowVersion)
    {
        switch(iNextVersion.mClassKind)
        {
        case kClassKind2Default:
            break;

        case kClassKind2Kind1:
            oNowVersion.mData0=iNextVersion.mData0-100;
            oNowVersion.mData1=iNextVersion.mData1-10000;
            break;

        case kClassKind2Kind2:
            oNowVersion.mData0=iNextVersion.mData0-200;
            oNowVersion.mData1=iNextVersion.mData1-10000;
            break;

        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }

    // Members version up.
    static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)
    {
        switch(iNowVersion.mClassKind)
        {
        case kClassKind1Default:
            break;

        case kClassKind1Kind1:
            oNextVersion.mData0=iNowVersion.mData0+100;
            oNextVersion.mData1=iNowVersion.mData1+10000;
            break;

        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }
};

//      ---<<< バージョン・アップ／ダウン処理 ver2 <-> ver3 >>>---

template<class tTheolizerVersion, class tNextVersion>
struct VersionUpDownTest::TheolizerUserDefine<tTheolizerVersion, tNextVersion, 2>
{
    // Initialize members that is deleted in next version.
    static void initialize(tTheolizerVersion& oNowVersion)
    {
    }

    // Members version down.
    static void downVersion(tNextVersion const& iNextVersion, tTheolizerVersion& oNowVersion)
    {
        switch(iNextVersion.mClassKind)
        {
        case kClassKind2Default:
            oNowVersion.mData1=static_cast<short>(iNextVersion.mData1);
            break;

        case kClassKind2Kind1:
            oNowVersion.mData0=iNextVersion.mData0-1000;
            oNowVersion.mData1=static_cast<short>(iNextVersion.mData1-100000);
            break;

        case kClassKind2Kind2:
            oNowVersion.mData0=iNextVersion.mData0-2000;
            oNowVersion.mData1=static_cast<short>(iNextVersion.mData1-100000);
            break;

        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }

    // Members version up.
    static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)
    {
        switch(iNowVersion.mClassKind)
        {
        case kClassKind2Default:
            oNextVersion.mData1.                                    // 型が異なる
                set(iNowVersion.mData1, iNowVersion.mData1.getDoSucceed());
            break;

        case kClassKind2Kind1:
            oNextVersion.mData0=iNowVersion.mData0+1000;
            oNextVersion.mData1.                                // 型が異なる
                set(iNowVersion.mData1+100000, iNowVersion.mData1.getDoSucceed());
            break;

        case kClassKind2Kind2:
            oNextVersion.mData0=iNowVersion.mData0+2000;
            oNextVersion.mData1.                                // 型が異なる
                set(iNowVersion.mData1+100000, iNowVersion.mData1.getDoSucceed());
            break;

        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }
};

//----------------------------------------------------------------------------
//      Keep-stepとNon-Keep-stepのテスト用
//----------------------------------------------------------------------------

//      ---<<< 非侵入型完全自動 >>>---

struct VersionFullAuto
{
    int     mFullAuto;

    VersionFullAuto(bool iValued=false) :
        mFullAuto((iValued)?123:0)
    { }
    void check() const
    {
        switch(gVersionList[gDataIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
        case VersionEnum::ver1b:
            THEOLIZER_EQUAL(mFullAuto, 0);
            break;

        case VersionEnum::ver1c:
        case VersionEnum::ver2a:
        case VersionEnum::ver3a:
        case VersionEnum::ver3b:
            THEOLIZER_EQUAL(mFullAuto, 123);
            break;

        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }
};

//      ---<<< 非侵入型手動 >>>---
//          ver1:int型で保存(切り捨て)
//          ver2:double型で保存

class VersionManual
{
    double  mManual;
public:
    double&       get()       { return mManual; }
    double const& get() const { return mManual; }

    VersionManual(bool iValued=false) :
        mManual((iValued)?2.5:0.0)
    { }
    void check() const
    {
        switch(gVersionList[gDataIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
        case VersionEnum::ver1b:
            THEOLIZER_EQUAL(mManual, 0);
            break;

        case VersionEnum::ver1c:
            THEOLIZER_EQUAL(mManual, 2);
            break;

        case VersionEnum::ver2a:
        case VersionEnum::ver3a:
        case VersionEnum::ver3b:
            THEOLIZER_EQUAL(mManual, 2.5);
            break;

        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }
};

// 非侵入型手動クラスの指定
THEOLIZER_NON_INTRUSIVE_ORDER((VersionManual), 2);

// 保存処理／回復処理関数
template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<VersionManual>::
    TheolizerUserDefine<tBaseSerializer, tTheolizerVersion, 1>
{
    // Save members.
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        THEOLIZER_PROCESS(iSerializer, static_cast<int>(iInstance->get()));
    }

    // Load members.
    static void loadClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        if (!oInstance) oInstance=new typename tTheolizerVersion::TheolizerTarget();
        int temp;
        THEOLIZER_PROCESS(iSerializer, temp);
        oInstance->get()=temp;
    }
};

// 保存処理／回復処理関数
template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<VersionManual>::
    TheolizerUserDefine<tBaseSerializer, tTheolizerVersion, 2>
{
    // Save members.
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        THEOLIZER_PROCESS(iSerializer, iInstance->get());
    }

    // Load members.
    static void loadClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        if (!oInstance) oInstance=new typename tTheolizerVersion::TheolizerTarget();
        
        THEOLIZER_PROCESS(iSerializer, oInstance->get());
    }
};

//      ---<<< テスト対象クラス >>>---

struct KeepStepTest :
    public VersionUpDownTest,                                                   // Keep-step
    public VersionFullAuto,                                                     // FullAuto
    public VersionManual                                                        // Manual
{
    VersionUpDownTest   mVersionUpDownTest;                                     // Keep-step
    VersionUpDownTest*  mVersionUpDownTestPtr;                                  // Non-Keep-step
    VersionUpDownTest&  mVersionUpDownTestRef THEOLIZER_ANNOTATE(FS:<>Pointee); // Non-Keep-step
    VersionFullAuto     mVersionFullAuto;                                       // FullAuto
    VersionManual       mVersionManual;                                         // Manual

    KeepStepTest(VersionUpDownTest& iVersionUpDownTest, bool iValued=false) :
        VersionUpDownTest((iValued)?ClassKind::Kind2:ClassKind::Default),
        VersionFullAuto(iValued),
        VersionManual(iValued),
        mVersionUpDownTest((iValued)?ClassKind::Kind2:ClassKind::Default),
        mVersionUpDownTestPtr((iValued)?&iVersionUpDownTest:nullptr),
        mVersionUpDownTestRef(iVersionUpDownTest),
        mVersionFullAuto(iValued),
        mVersionManual(iValued)
    { }

    void check(VersionUpDownTest& iVersionUpDownTest)
    {
        VersionUpDownTest::check(ClassKind::Kind2);
        VersionFullAuto::check();
        VersionManual::check();
        mVersionUpDownTest.check(ClassKind::Kind2);
        THEOLIZER_EQUAL_PTR( mVersionUpDownTestPtr, &iVersionUpDownTest);
        THEOLIZER_EQUAL_PTR(&mVersionUpDownTestRef, &iVersionUpDownTest);
        mVersionFullAuto.check();
        mVersionManual.check();
    }

    THEOLIZER_INTRUSIVE(CS, (KeepStepTest), 3);
};

//      ---<<< バージョン・アップ／ダウン処理 ver1 <-> ver2 >>>---

template<class tTheolizerVersion, class tNextVersion>
struct KeepStepTest::TheolizerUserDefine<tTheolizerVersion, tNextVersion, 1>
{
    // Initialize members that is deleted in next version.
    static void initialize(tTheolizerVersion& oNowVersion)
    {
    }

    // Members version down.
    static void downVersion(tNextVersion const& iNextVersion, tTheolizerVersion& oNowVersion)
    {
    }

    // Members version up.
    static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)
    {
        // Keep-step/Non-keep-stepのテスト
        // Keep-step(バージョン・ダウンにより値が変化する)
        THEOLIZER_EQUAL(iNowVersion.mClassKind, kClassKind1Kind1);
        THEOLIZER_EQUAL(iNowVersion.mVersionUpDownTest.mClassKind, kClassKind1Kind1);

        // Non-keep-step(値は最新版なので、enum型である)
        switch(gVersionList[gDataIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
        case VersionEnum::ver1b:
        case VersionEnum::ver1c:
            THEOLIZER_EQUAL(iNowVersion.mVersionUpDownTestPtr.get()->mClassKind, ClassKind::Kind1);
            THEOLIZER_EQUAL(iNowVersion.mVersionUpDownTestRef.get().mClassKind,  ClassKind::Kind1);
            break;

        case VersionEnum::ver2a:
        case VersionEnum::ver3a:
            THEOLIZER_EQUAL(iNowVersion.mVersionUpDownTestPtr.get()->mClassKind, ClassKind::Kind2);
            THEOLIZER_EQUAL(iNowVersion.mVersionUpDownTestRef.get().mClassKind,  ClassKind::Kind2);
            break;

        case VersionEnum::ver3b:
        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }
};

//      ---<<< バージョン・アップ／ダウン処理 ver2 <-> ver3 >>>---

template<class tTheolizerVersion, class tNextVersion>
struct KeepStepTest::TheolizerUserDefine<tTheolizerVersion, tNextVersion, 2>
{
    // Initialize members that is deleted in next version.
    static void initialize(tTheolizerVersion& oNowVersion)
    {
    }

    // Members version down.
    static void downVersion(tNextVersion const& iNextVersion, tTheolizerVersion& oNowVersion)
    {
    }

    // Members version up.
    static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)
    {
        // ver2->ver1->ver2が不可逆変換だが、
        //  回復しなかった時は元に戻る。
        //  ver1から回復した時のみKind1になる。(ver2はKind2を設定するため)
        if ((!iNowVersion.mClassKind.getDoSucceed())
         || (VersionEnum::ver2a <= gVersionList[gDataIndex].mVersionEnum))
        {
            THEOLIZER_EQUAL(iNowVersion.mClassKind.get(), kClassKind2Kind2);
            THEOLIZER_EQUAL(iNowVersion.mVersionUpDownTest.mClassKind.get(), kClassKind2Kind2);
        }
        else
        {
            THEOLIZER_EQUAL(iNowVersion.mClassKind.get(), kClassKind1Kind1);
            THEOLIZER_EQUAL(iNowVersion.mVersionUpDownTest.mClassKind.get(), kClassKind1Kind1);
        }

        switch(gVersionList[gDataIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
        case VersionEnum::ver1b:
            THEOLIZER_EQUAL(iNowVersion.mFullAuto,  0);
            oNextVersion.VersionManualTheolizerBase.check();
            THEOLIZER_EQUAL(iNowVersion.mVersionFullAuto.mFullAuto,  0);
            iNowVersion.mVersionManual.get().check();
            break;

        case VersionEnum::ver1c:
        case VersionEnum::ver2a:
        case VersionEnum::ver3a:
        case VersionEnum::ver3b:
            THEOLIZER_EQUAL(iNowVersion.mFullAuto,  123);
            oNextVersion.VersionManualTheolizerBase.check();
            THEOLIZER_EQUAL(iNowVersion.mVersionFullAuto.mFullAuto,  123);
            iNowVersion.mVersionManual.get().check();
            break;

        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }
};

#endif  // TEST_MODIFY_COMPLEX_H
