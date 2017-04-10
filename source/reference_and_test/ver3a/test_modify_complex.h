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
// ***************************************************************************

//----------------------------------------------------------------------------
//      非可逆バージョン・アップの原因となるenum型
//          良くあるバージョン・アップを想定
//----------------------------------------------------------------------------

enum class ClassKind
{
    Default,
    Kind1,
    Kind2   THEOLIZER_ANNOTATE(ES:::1)
};
THEOLIZER_ENUM(ClassKind, 2);

//----------------------------------------------------------------------------
//      バージョン・アップ／ダウン処理テスト（保存先指定含む）
//----------------------------------------------------------------------------

struct VersionUpDownTest
{
    ClassKind   mClassKind  THEOLIZER_ANNOTATE(FS:<theolizerD::DestA>);
    int         mData0      THEOLIZER_ANNOTATE(FS:<theolizerD::DestA>);
    int         mData1      THEOLIZER_ANNOTATE(FS:<theolizerD::DestB>);

    VersionUpDownTest(ClassKind iClassKind=ClassKind::Default) :
        mClassKind(iClassKind),
        mData0(0),
        mData1(0)
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
        }
        if (mClassKind != ClassKind::Default)
        {
            mData1=110020;
        }
    }

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
        case ClassKind::Default:
            break;

        case ClassKind::Kind1:
            oNowVersion.mData0=iNextVersion.mData0-100;
            oNowVersion.mData1=iNextVersion.mData1-10000;
            break;

        case ClassKind::Kind2:
            oNowVersion.mData0=iNextVersion.mData0-200;
            oNowVersion.mData1=iNextVersion.mData1-10000;
            break;
        }
    }

    // Members version up.
    static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)
    {
        switch(iNowVersion.mClassKind)
        {
        case ClassKind::Default:
            break;

        case ClassKind::Kind1:
            oNextVersion.mData0=iNowVersion.mData0+100;
            oNextVersion.mData1=iNowVersion.mData1+10000;
            break;

        case ClassKind::Kind2:  // ここには来ない(ver1では定義されていないため)
            oNextVersion.mData0=iNowVersion.mData0+200;
            oNextVersion.mData1=iNowVersion.mData1+10000;
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
        case ClassKind::Default:
            oNowVersion.mData1=static_cast<short>(iNextVersion.mData1);
            break;

        case ClassKind::Kind1:
            oNowVersion.mData0=iNextVersion.mData0-1000;
            oNowVersion.mData1=static_cast<short>(iNextVersion.mData1-100000);
            break;

        case ClassKind::Kind2:
            oNowVersion.mData0=iNextVersion.mData0-2000;
            oNowVersion.mData1=static_cast<short>(iNextVersion.mData1-100000);
            break;
        }
    }

    // Members version up.
    static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)
    {
        switch(iNowVersion.mClassKind)
        {
        case ClassKind::Default:
            oNextVersion.mData1=iNowVersion.mData1;
            break;

        case ClassKind::Kind1:
            oNextVersion.mData0=iNowVersion.mData0+1000;
            oNextVersion.mData1=iNowVersion.mData1+100000;
            break;

        case ClassKind::Kind2:
            oNextVersion.mData0=iNowVersion.mData0+2000;
            oNextVersion.mData1=iNowVersion.mData1+100000;
            break;
        }
        oNextVersion.mData1TheolizerSucceed=iNowVersion.mData1TheolizerSucceed;
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
            THEOLIZER_EQUAL(mFullAuto, 123);
            break;

        case VersionEnum::ver3b:
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
            THEOLIZER_EQUAL(mManual, 2.5);
            break;

        case VersionEnum::ver3b:
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
        THEOLIZER_CHECK(iNowVersion.mClassKind != oNextVersion.mClassKind,
                        iNowVersion.mClassKind.getIntegral());
        THEOLIZER_CHECK(iNowVersion.mVersionUpDownTest.mClassKind
                        != oNextVersion.mVersionUpDownTest.mClassKind,
                        iNowVersion.mVersionUpDownTest.mClassKind.getIntegral());

        // Non-keep-step(値は最新版のまま)
        THEOLIZER_CHECK(iNowVersion.mVersionUpDownTestPtr->mClassKind
                        == oNextVersion.mVersionUpDownTestPtr->mClassKind,
                        iNowVersion.mVersionUpDownTestPtr->mClassKind);
        THEOLIZER_CHECK(iNowVersion.mVersionUpDownTestRef.mClassKind
                        == oNextVersion.mVersionUpDownTestRef.mClassKind,
                        iNowVersion.mVersionUpDownTestRef.mClassKind);
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
        if ((!iNowVersion.mClassKindTheolizerSucceed)
         || (VersionEnum::ver2a <= gVersionList[gDataIndex].mVersionEnum))
        {
            THEOLIZER_EQUAL(iNowVersion.mClassKind.get(), ClassKind::Kind2);
            THEOLIZER_EQUAL(iNowVersion.mVersionUpDownTest.mClassKind.get(), ClassKind::Kind2);
        }
        else
        {
            THEOLIZER_EQUAL(iNowVersion.mClassKind.get(), ClassKind::Kind1);
            THEOLIZER_EQUAL(iNowVersion.mVersionUpDownTest.mClassKind.get(), ClassKind::Kind1);
        }

        switch(gVersionList[gDataIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
        case VersionEnum::ver1b:
            THEOLIZER_EQUAL(iNowVersion.mFullAuto,  0);
            oNextVersion.VersionManualTheolizerBase.check();
            THEOLIZER_EQUAL(iNowVersion.mVersionFullAuto.mFullAuto,  0);
            iNowVersion.mVersionManual.check();
            break;

        case VersionEnum::ver1c:
        case VersionEnum::ver2a:
        case VersionEnum::ver3a:
            THEOLIZER_EQUAL(iNowVersion.mFullAuto,  123);
            oNextVersion.VersionManualTheolizerBase.check();
            THEOLIZER_EQUAL(iNowVersion.mVersionFullAuto.mFullAuto,  123);
            iNowVersion.mVersionManual.check();
            break;

        case VersionEnum::ver3b:
        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }
};

#endif  // TEST_MODIFY_COMPLEX_H
