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

#endif  // TEST_MODIFY_COMPLEX_H
