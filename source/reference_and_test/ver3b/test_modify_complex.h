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
//      複合テスト対象クラス
// ***************************************************************************

//----------------------------------------------------------------------------
//      他のクラスに含まれるクラス（侵入型半自動）
//----------------------------------------------------------------------------

struct PointeeClass
{
    int     mInt;

    PointeeClass(int iInt=0) : mInt(iInt) { }
    THEOLIZER_INTRUSIVE(CS, (PointeeClass), 2);
};

//      ---<<< バージョン・アップ／ダウン処理 ver1/ver2 >>>---

template<class tTheolizerVersion, class tNextVersion>
struct PointeeClass::TheolizerUserDefine<tTheolizerVersion, tNextVersion, 1>
{
    // Initialize members that is deleted in next version.
    static void initialize(tTheolizerVersion& oNowVersion)
    {
std::cout << "PointeeClass1::initialize()\n";
    }

    // Members version down.
    static void downVersion(tNextVersion const& iNextVersion, tTheolizerVersion& oNowVersion)
    {
std::cout << "PointeeClass1::downVersion()\n";
    }

    // Members version up.
    static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)
    {
std::cout << "PointeeClass1::upVersion()\n";
    }
};

//      ---<<< バージョン・アップ／ダウン処理 ver2/ver3 >>>---

template<class tTheolizerVersion, class tNextVersion>
struct PointeeClass::TheolizerUserDefine<tTheolizerVersion, tNextVersion, 2>
{
    // Initialize members that is deleted in next version.
    static void initialize(tTheolizerVersion& oNowVersion)
    {
std::cout << "PointeeClass2::initialize()\n";
    }

    // Members version down.
    static void downVersion(tNextVersion const& iNextVersion, tTheolizerVersion& oNowVersion)
    {
std::cout << "PointeeClass2::downVersion()\n";
    }

    // Members version up.
    static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)
    {
std::cout << "PointeeClass2::upVersion()\n";
    }
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
    THEOLIZER_INTRUSIVE(CS, (PointeeInclude), 2);
};

//      ---<<< バージョン・アップ／ダウン処理 ver1/ver2 >>>---

template<class tTheolizerVersion, class tNextVersion>
struct PointeeInclude::TheolizerUserDefine<tTheolizerVersion, tNextVersion, 1>
{
    // Initialize members that is deleted in next version.
    static void initialize(tTheolizerVersion& oNowVersion)
    {
std::cout << "PointeeInclude1::initialize()\n";
    }

    // Members version down.
    static void downVersion(tNextVersion const& iNextVersion, tTheolizerVersion& oNowVersion)
    {
std::cout << "PointeeInclude1::downVersion()\n";
    }

    // Members version up.
    static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)
    {
std::cout << "PointeeInclude1::upVersion()\n";
    }
};

//      ---<<< バージョン・アップ／ダウン処理 ver2/ver3 >>>---

template<class tTheolizerVersion, class tNextVersion>
struct PointeeInclude::TheolizerUserDefine<tTheolizerVersion, tNextVersion, 2>
{
    // Initialize members that is deleted in next version.
    static void initialize(tTheolizerVersion& oNowVersion)
    {
std::cout << "PointeeInclude2::initialize()\n";
    }

    // Members version down.
    static void downVersion(tNextVersion const& iNextVersion, tTheolizerVersion& oNowVersion)
    {
std::cout << "PointeeInclude2::downVersion()\n";
    }

    // Members version up.
    static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)
    {
std::cout << "PointeeInclude2::upVersion()\n";
    }
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
        THEOLIZER_EQUAL_PTR(mPointeeClass0,  iPointeeInclude.mPointeeClassOwner.get());
        THEOLIZER_EQUAL_PTR(mPointeeClass1, &iPointeeInclude.mPointeeClass);
        THEOLIZER_EQUAL_PTR(mPointeeClass2, &iPointeeInclude.mPointeeClassRef);
    }
    THEOLIZER_INTRUSIVE(CS, (PointerInclude), 2);
};

//      ---<<< バージョン・アップ／ダウン処理 ver1/ver2 >>>---

template<class tTheolizerVersion, class tNextVersion>
struct PointerInclude::TheolizerUserDefine<tTheolizerVersion, tNextVersion, 1>
{
    // Initialize members that is deleted in next version.
    static void initialize(tTheolizerVersion& oNowVersion)
    {
std::cout << "PointerInclude1::initialize()\n";
    }

    // Members version down.
    static void downVersion(tNextVersion const& iNextVersion, tTheolizerVersion& oNowVersion)
    {
std::cout << "PointerInclude1::downVersion()\n";
    }

    // Members version up.
    static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)
    {
std::cout << "PointerInclude1::upVersion()\n";
    }
};

//      ---<<< バージョン・アップ／ダウン処理 ver2/ver3 >>>---

template<class tTheolizerVersion, class tNextVersion>
struct PointerInclude::TheolizerUserDefine<tTheolizerVersion, tNextVersion, 2>
{
    // Initialize members that is deleted in next version.
    static void initialize(tTheolizerVersion& oNowVersion)
    {
std::cout << "PointerInclude2::initialize()\n";
    }

    // Members version down.
    static void downVersion(tNextVersion const& iNextVersion, tTheolizerVersion& oNowVersion)
    {
std::cout << "PointerInclude2::downVersion()\n";
    }

    // Members version up.
    static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)
    {
std::cout << "PointerInclude2::upVersion()\n";
    }
};

#endif  // TEST_MODIFY_COMPLEX_H
