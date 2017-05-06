//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          クラス変更の複合テスト
//              オブジェクト追跡
//              バージョン・アップ／ダウン処理（参照テスト含む）
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
    PointeeClass    mPointeeClass    THEOLIZER_ANNOTATE(FS:<>Pointee);  // 被ポインタ実体
    PointeeClass&   mPointeeClassRef THEOLIZER_ANNOTATE(FS:<>Pointee);  // 被ポインタ参照

    PointeeInclude(PointeeClass& iPointeeClassRef) :
        mPointeeClassOwner(new PointeeClass(0)),
        mPointeeClass(0),
        mPointeeClassRef(iPointeeClassRef)
    { }

    PointeeInclude(PointeeClass& iPointeeClassRef, bool) :
        mPointeeClassOwner(new PointeeClass(100)),
        mPointeeClass(101),
        mPointeeClassRef(iPointeeClassRef)
    {
        mPointeeClassRef.mInt=102;
    }

    void check()
    {
        THEOLIZER_EQUAL(mPointeeClassOwner->mInt,   100);
        THEOLIZER_EQUAL(mPointeeClass.mInt,         101);
        THEOLIZER_EQUAL(mPointeeClassRef.mInt,      102);
    }
    THEOLIZER_INTRUSIVE(CS, (PointeeInclude), 1);
};

//----------------------------------------------------------------------------
//      他のクラスを含むポインタ側クラス
//          ver1 : 
//              mPointeeClass0 → mPointeeClassOwner
//              mPointeeClass1 → mPointeeClass
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
        mPointeeClass0( iPointeeInclude.mPointeeClassOwner.get()),
        mPointeeClass1(&iPointeeInclude.mPointeeClass),
        mPointeeClass2(&iPointeeInclude.mPointeeClassRef)
    { }

    void check(PointeeInclude& iPointeeInclude)
    {
        switch(gVersionList[gProgramIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
        case VersionEnum::ver1b:
        case VersionEnum::ver1c:
            THEOLIZER_EQUAL_PTR(mPointeeClass0,  iPointeeInclude.mPointeeClassOwner.get());
            THEOLIZER_EQUAL_PTR(mPointeeClass1, &iPointeeInclude.mPointeeClass);
            THEOLIZER_EQUAL_PTR(mPointeeClass2, &iPointeeInclude.mPointeeClassRef);
            break;

        case VersionEnum::ver2a:
            THEOLIZER_EQUAL_PTR(mPointeeClass0, &iPointeeInclude.mPointeeClass);
            THEOLIZER_EQUAL_PTR(mPointeeClass1, &iPointeeInclude.mPointeeClassRef);
            THEOLIZER_EQUAL_PTR(mPointeeClass2,  iPointeeInclude.mPointeeClassOwner.get());
            break;

        case VersionEnum::ver3a:
        case VersionEnum::ver3b:
            THEOLIZER_EQUAL_PTR(mPointeeClass0, &iPointeeInclude.mPointeeClassRef);
            THEOLIZER_EQUAL_PTR(mPointeeClass1,  iPointeeInclude.mPointeeClassOwner.get());
            THEOLIZER_EQUAL_PTR(mPointeeClass2, &iPointeeInclude.mPointeeClass);
            break;

        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }
    THEOLIZER_INTRUSIVE(CS, (PointerInclude), 1);
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

    // enum型のバージョン・アップ組合せテスト用シンボル定義
    Origin  =100,
    Load    =200,
    Set     =300,
    Ope     =400,
    Inner   =500
};
THEOLIZER_ENUM(ClassKind, 1);

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
    short       mData1      THEOLIZER_ANNOTATE(FS:<theolizerD::DestB>);

    // Non-keep-stepのバージョン・アップ組合せテスト
    ClassKind   mEnum00;
    ClassKind   mEnum01;
    ClassKind   mEnum02;
    ClassKind   mEnum03;
    ClassKind   mEnum04;
    ClassKind   mEnum05;
    ClassKind   mEnum06;

    int         mInt00;
    int         mInt01;
    int         mInt02;
    int         mInt03;
    int         mInt04;
    int         mInt05;
    int         mInt06;

    // コンストラクタ
    VersionUpDownTest(ClassKind iClassKind=ClassKind::Default) :
        mClassKind(iClassKind),
        mData0((mClassKind==ClassKind::Default)?0:10),
        mData1((mClassKind==ClassKind::Default)?0:20),
        mEnum00((iClassKind==ClassKind::Default)?ClassKind::Origin:ClassKind::Load),
        mEnum01(mEnum00),
        mEnum02(mEnum00),
        mEnum03(mEnum00),
        mEnum04(mEnum00),
        mEnum05(mEnum00),
        mEnum06(mEnum00),
        mInt00((iClassKind==ClassKind::Default)?eisOrigin:eisLoad),
        mInt01(mInt00),
        mInt02(mInt00),
        mInt03(mInt00),
        mInt04(mInt00),
        mInt05(mInt00),
        mInt06(mInt00)
    { }

    // チェック
    void check(ClassKind iClassKind)
    {
        THEOLIZER_EQUAL(mClassKind, iClassKind);
        THEOLIZER_EQUAL(mData0, 10);
        THEOLIZER_EQUAL(mData1, 20);

        switch(gVersionList[gDataIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
        case VersionEnum::ver1b:
            THEOLIZER_EQUAL(mEnum00, ClassKind::Origin, (void*)&mEnum00);
            THEOLIZER_EQUAL(mEnum01, ClassKind::Origin, (void*)&mEnum01);
            THEOLIZER_EQUAL(mEnum02, ClassKind::Origin, (void*)&mEnum02);
            THEOLIZER_EQUAL(mEnum03, ClassKind::Origin, (void*)&mEnum03);
            THEOLIZER_EQUAL(mEnum04, ClassKind::Origin, (void*)&mEnum04);
            THEOLIZER_EQUAL(mEnum05, ClassKind::Origin, (void*)&mEnum05);
            THEOLIZER_EQUAL(mEnum06, ClassKind::Origin, (void*)&mEnum06);

            THEOLIZER_EQUAL(mInt00, eisOrigin, (void*)&mInt00);
            THEOLIZER_EQUAL(mInt01, eisOrigin, (void*)&mInt01);
            THEOLIZER_EQUAL(mInt02, eisOrigin, (void*)&mInt02);
            THEOLIZER_EQUAL(mInt03, eisOrigin, (void*)&mInt03);
            THEOLIZER_EQUAL(mInt04, eisOrigin, (void*)&mInt04);
            THEOLIZER_EQUAL(mInt05, eisOrigin, (void*)&mInt05);
            THEOLIZER_EQUAL(mInt06, eisOrigin, (void*)&mInt06);
            break;

        case VersionEnum::ver1c:
        case VersionEnum::ver2a:
        case VersionEnum::ver3a:
        case VersionEnum::ver3b:
            THEOLIZER_EQUAL(mEnum00, ClassKind::Load, (void*)&mEnum00);
            THEOLIZER_EQUAL(mEnum01, ClassKind::Load, (void*)&mEnum01);
            THEOLIZER_EQUAL(mEnum02, ClassKind::Load, (void*)&mEnum02);
            THEOLIZER_EQUAL(mEnum03, ClassKind::Load, (void*)&mEnum03);
            THEOLIZER_EQUAL(mEnum04, ClassKind::Load, (void*)&mEnum04);
            THEOLIZER_EQUAL(mEnum05, ClassKind::Load, (void*)&mEnum05);
            THEOLIZER_EQUAL(mEnum06, ClassKind::Load, (void*)&mEnum06);

            THEOLIZER_EQUAL(mInt00, eisLoad, (void*)&mInt00);
            THEOLIZER_EQUAL(mInt01, eisLoad, (void*)&mInt01);
            THEOLIZER_EQUAL(mInt02, eisLoad, (void*)&mInt02);
            THEOLIZER_EQUAL(mInt03, eisLoad, (void*)&mInt03);
            THEOLIZER_EQUAL(mInt04, eisLoad, (void*)&mInt04);
            THEOLIZER_EQUAL(mInt05, eisLoad, (void*)&mInt05);
            THEOLIZER_EQUAL(mInt06, eisLoad, (void*)&mInt06);
            break;

        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }

    THEOLIZER_INTRUSIVE(CS, (VersionUpDownTest), 1);
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
            THEOLIZER_EQUAL(mFullAuto, 123);
            break;

        case VersionEnum::ver3a:
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
    int     mManual;
public:
    int&       get()       { return mManual; }
    int const& get() const { return mManual; }

    VersionManual(bool iValued=false) :
        mManual((iValued)?2:0)
    { }
    void check()
    {
        switch(gVersionList[gDataIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
        case VersionEnum::ver1b:
            THEOLIZER_EQUAL(mManual, 0);
            break;

        case VersionEnum::ver1c:
        case VersionEnum::ver2a:
            THEOLIZER_EQUAL(mManual, 2);
            break;

        case VersionEnum::ver3a:
        case VersionEnum::ver3b:
        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }
};

// 非侵入型手動クラスの指定
THEOLIZER_NON_INTRUSIVE_ORDER((VersionManual), 1);

// 保存処理／回復処理関数ver1
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
        VersionUpDownTest((iValued)?ClassKind::Kind1:ClassKind::Default),
        VersionFullAuto(iValued),
        VersionManual(iValued),
        mVersionUpDownTest((iValued)?ClassKind::Kind1:ClassKind::Default),
        mVersionUpDownTestPtr((iValued)?&iVersionUpDownTest:nullptr),
        mVersionUpDownTestRef(iVersionUpDownTest),
        mVersionFullAuto(iValued),
        mVersionManual(iValued)
    { }

    void check(VersionUpDownTest& iVersionUpDownTest)
    {
        VersionUpDownTest::check(ClassKind::Kind1);
        VersionFullAuto::check();
        VersionManual::check();
        mVersionUpDownTest.check(ClassKind::Kind1);
        THEOLIZER_EQUAL_PTR( mVersionUpDownTestPtr, &iVersionUpDownTest);
        THEOLIZER_EQUAL_PTR(&mVersionUpDownTestRef, &iVersionUpDownTest);
        mVersionFullAuto.check();
        mVersionManual.check();
    }

    THEOLIZER_INTRUSIVE(CS, (KeepStepTest), 1); // 侵入型半自動へ変更後、バージョン・アップする
};

// ***************************************************************************
//      リカバリー・テスト用
// ***************************************************************************

class TestRecovery
{
    int     mInt;
public:
    TestRecovery() : mInt(0)
    { }

    TestRecovery(bool) : mInt(10001)
    { }

    void check()
    {
        THEOLIZER_EQUAL(mInt, 10001);
    }

    THEOLIZER_INTRUSIVE(CS, (TestRecovery), 1);
};

#endif  // TEST_MODIFY_COMPLEX_H
