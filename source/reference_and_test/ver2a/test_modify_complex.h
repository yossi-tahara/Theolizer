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
    PointeeClass    mPointeeClass    THEOLIZER_ANNOTATE(FS:<>Pointee);  // 被ポインタ実体
    PointeeClass&   mPointeeClassRef THEOLIZER_ANNOTATE(FS:<>Pointee);  // 被ポインタ参照
    PointeeClass    mPointeeClassAdd THEOLIZER_ANNOTATE(FS:<>Pointee);  // 被ポインタ実体2

    PointeeInclude(PointeeClass& iPointeeClassRef) :
        mPointeeClassOwner(new PointeeClass(0)),
        mPointeeClass(0),
        mPointeeClassRef(iPointeeClassRef),
        mPointeeClassAdd(0)
    { }

    PointeeInclude(PointeeClass& iPointeeClassRef, bool) :
        mPointeeClassOwner(new PointeeClass(100)),
        mPointeeClass(101),
        mPointeeClassRef(iPointeeClassRef),
        mPointeeClassAdd(103)
    {
        mPointeeClassRef.mInt=102;
    }

    void check()
    {
        THEOLIZER_EQUAL(mPointeeClassOwner->mInt,   100);
        THEOLIZER_EQUAL(mPointeeClass.mInt,         101);
        THEOLIZER_EQUAL(mPointeeClassRef.mInt,      102);

        // ver2aが生成したver2aデータのみ値がある
        if ((gVersionList[gDataIndex].mVersionEnum == VersionEnum::ver2a)
         && (gVersionList[gProgramIndex].mVersionEnum == VersionEnum::ver2a))
        {
            THEOLIZER_EQUAL(mPointeeClassAdd.mInt,  103);
        }
        else if (gVersionList[gDataIndex].mVersionEnum < VersionEnum::ver3a)
        {
            THEOLIZER_EQUAL(mPointeeClassAdd.mInt,  0);
        }
        else
        {
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
        }
    }
    THEOLIZER_INTRUSIVE(CS, (PointeeInclude), 2);
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
    // 2a:追加し、3aで削除
    PointeeClass*   mPointeeClass3;
    PointeeClass*   mPointeeClass4;

    PointerInclude() :
        mPointeeClass0(nullptr),
        mPointeeClass1(nullptr),
        mPointeeClass2(nullptr),
        mPointeeClass3(nullptr),
        mPointeeClass4(nullptr)
    { }

    PointerInclude(PointeeInclude& iPointeeInclude) :
        mPointeeClass0(&iPointeeInclude.mPointeeClass),
        mPointeeClass1(&iPointeeInclude.mPointeeClassRef),
        mPointeeClass2( iPointeeInclude.mPointeeClassOwner.get()),
        mPointeeClass3(&iPointeeInclude.mPointeeClass),
        mPointeeClass4(&iPointeeInclude.mPointeeClassAdd)
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

        // ver2aが生成したver2aデータのみ値がある
        if ((gVersionList[gDataIndex].mVersionEnum == VersionEnum::ver2a)
         && (gVersionList[gProgramIndex].mVersionEnum == VersionEnum::ver2a))
        {
            THEOLIZER_EQUAL_PTR(mPointeeClass3, &iPointeeInclude.mPointeeClass);
            THEOLIZER_EQUAL_PTR(mPointeeClass4, &iPointeeInclude.mPointeeClassAdd);
        }
        else if (gVersionList[gDataIndex].mVersionEnum < VersionEnum::ver3a)
        {
            THEOLIZER_EQUAL_PTR(mPointeeClass3, nullptr);
            THEOLIZER_EQUAL_PTR(mPointeeClass4, nullptr);
        }
        else
        {
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
        }
    }
    THEOLIZER_INTRUSIVE(CS, (PointerInclude), 2);
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
    short       mData1      THEOLIZER_ANNOTATE(FS:<theolizerD::DestB>);

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
            mData0=110;
            break;

        case ClassKind::Kind2:
            mData0=210;
            break;
        }
        if (mClassKind != ClassKind::Default)
        {
            mData1=10020;
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
            THEOLIZER_EQUAL(mData0, 110);
            THEOLIZER_EQUAL(mData1, 10020);
            break;

        case ClassKind::Kind2:
            THEOLIZER_EQUAL(mData0, 210);
            THEOLIZER_EQUAL(mData1, 10020);
            break;
        }
    }

    THEOLIZER_INTRUSIVE(CS, (VersionUpDownTest), 2);
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
            break;

        case ClassKind::Kind2:
            oNowVersion.mData0=iNextVersion.mData0-200;
            break;
        }
        oNowVersion.mData1=iNextVersion.mData1-10000;
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
            break;

        case ClassKind::Kind2:  // ここには来ない(ver1では定義されていないため)
            oNextVersion.mData0=iNowVersion.mData0+200;
            break;
        }
        oNextVersion.mData1=iNowVersion.mData1+10000;
    }
};

//----------------------------------------------------------------------------
//      Keep-stepとNon-Keep-stepのテスト用
//----------------------------------------------------------------------------

struct KeepStepTest :
    public VersionUpDownTest                                                    // Keep-step
{
    VersionUpDownTest   mVersionUpDownTest;                                     // Keep-step
    VersionUpDownTest*  mVersionUpDownTestPtr;                                  // Non-Keep-step
    VersionUpDownTest&  mVersionUpDownTestRef THEOLIZER_ANNOTATE(FS:<>Pointee); // Non-Keep-step

    KeepStepTest(VersionUpDownTest& iVersionUpDownTest, ClassKind iClassKind=ClassKind::Default) :
        VersionUpDownTest(iClassKind),
        mVersionUpDownTest(iClassKind),
        mVersionUpDownTestPtr((iClassKind==ClassKind::Default)?nullptr:&iVersionUpDownTest),
        mVersionUpDownTestRef(iVersionUpDownTest)
    { }

    void check(VersionUpDownTest& iVersionUpDownTest, ClassKind iClassKind)
    {
        VersionUpDownTest::check(iClassKind);
        mVersionUpDownTest.check(iClassKind);
        THEOLIZER_EQUAL_PTR( mVersionUpDownTestPtr, &iVersionUpDownTest);
        THEOLIZER_EQUAL_PTR(&mVersionUpDownTestRef, &iVersionUpDownTest);
    }

    THEOLIZER_INTRUSIVE(CS, (KeepStepTest), 2);
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
    }
};

#endif  // TEST_MODIFY_COMPLEX_H
