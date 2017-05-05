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
// ***************************************************************************

//----------------------------------------------------------------------------
//      非可逆バージョン・アップの原因となるenum型
//          良くあるバージョン・アップを想定
//----------------------------------------------------------------------------

enum class ClassKind
{
    Default,
    Kind1
};
THEOLIZER_ENUM(ClassKind, 1);

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
        mData0((mClassKind==ClassKind::Default)?0:10),
        mData1((mClassKind==ClassKind::Default)?0:20)
    { }

    void check(ClassKind iClassKind)
    {
        THEOLIZER_EQUAL(mClassKind, iClassKind);
        THEOLIZER_EQUAL(mData0, 10);
        THEOLIZER_EQUAL(mData1, 20);
    }

    THEOLIZER_INTRUSIVE(CS, (VersionUpDownTest), 1);
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

    KeepStepTest(VersionUpDownTest& iVersionUpDownTest, bool iValued=false) :
        VersionUpDownTest((iValued)?ClassKind::Kind1:ClassKind::Default),
        mVersionUpDownTest((iValued)?ClassKind::Kind1:ClassKind::Default),
        mVersionUpDownTestPtr((iValued)?&iVersionUpDownTest:nullptr),
        mVersionUpDownTestRef(iVersionUpDownTest)
    { }

    void check(VersionUpDownTest& iVersionUpDownTest)
    {
        VersionUpDownTest::check(ClassKind::Kind1);
        mVersionUpDownTest.check(ClassKind::Kind1);
        THEOLIZER_EQUAL_PTR( mVersionUpDownTestPtr, &iVersionUpDownTest);
        THEOLIZER_EQUAL_PTR(&mVersionUpDownTestRef, &iVersionUpDownTest);
    }

//  THEOLIZER_INTRUSIVE(CS, (KeepStepTest), 1); // 最初は非侵入型完全自動
};

// ***************************************************************************
//      型違い検出テスト／文字型互換テスト
// ***************************************************************************

struct ChangeFromString
{
    std::string mStringToWString;
    std::string mStringToU16String;
    std::string mStringToU32String;

    ChangeFromString() { }
    ChangeFromString(bool) :
        mStringToWString(  u8"ＵＴＦ－８a"),
        mStringToU16String(u8"ＵＴＦ－８b"),
        mStringToU32String(u8"ＵＴＦ－８c")
    { }

    void check()
    {
        THEOLIZER_EQUAL(mStringToWString,   u8"ＵＴＦ－８a");
        THEOLIZER_EQUAL(mStringToU16String, u8"ＵＴＦ－８b");
        THEOLIZER_EQUAL(mStringToU32String, u8"ＵＴＦ－８c");
    }
};

struct ChangeFromWString
{
    std::wstring    mWStringToString;
    std::wstring    mWStringToU16String;
    std::wstring    mWStringToU32String;

    ChangeFromWString() { }
    ChangeFromWString(bool) :
        mWStringToString(   L"ＵＴＦ－１６Ｌa"),
        mWStringToU16String(L"ＵＴＦ－１６Ｌb"),
        mWStringToU32String(L"ＵＴＦ－１６Ｌc")
    { }

    void check()
    {
        THEOLIZER_EQUAL(mWStringToString,    L"ＵＴＦ－１６Ｌa");
        THEOLIZER_EQUAL(mWStringToU16String, L"ＵＴＦ－１６Ｌb");
        THEOLIZER_EQUAL(mWStringToU32String, L"ＵＴＦ－１６Ｌc");
    }
};

struct ChangeFromU16String
{
    std::u16string  mU16StringToString;
    std::u16string  mU16StringToWString;
    std::u16string  mU16StringToU32String;

    ChangeFromU16String() { }
    ChangeFromU16String(bool) :
        mU16StringToString(   u"ＵＴＦ－１６ｕa"),
        mU16StringToWString(  u"ＵＴＦ－１６ｕb"),
        mU16StringToU32String(u"ＵＴＦ－１６ｕc")
    { }

    void check()
    {
        THEOLIZER_EQUAL(mU16StringToString,    u"ＵＴＦ－１６ｕa");
        THEOLIZER_EQUAL(mU16StringToWString,   u"ＵＴＦ－１６ｕb");
        THEOLIZER_EQUAL(mU16StringToU32String, u"ＵＴＦ－１６ｕc");
    }
};

struct ChangeFromU32String
{
    std::u32string  mU32StringToString;
    std::u32string  mU32StringToWString;
    std::u32string  mU32StringToU16String;

    ChangeFromU32String() { }
    ChangeFromU32String(bool) :
        mU32StringToString(   U"ＵＴＦ－３２a"),
        mU32StringToWString(  U"ＵＴＦ－３２b"),
        mU32StringToU16String(U"ＵＴＦ－３２c")
    { }

    void check()
    {
        THEOLIZER_EQUAL(mU32StringToString,    U"ＵＴＦ－３２a");
        THEOLIZER_EQUAL(mU32StringToWString,   U"ＵＴＦ－３２b");
        THEOLIZER_EQUAL(mU32StringToU16String, U"ＵＴＦ－３２c");
    }
};

#endif  // TEST_MODIFY_COMPLEX_H
