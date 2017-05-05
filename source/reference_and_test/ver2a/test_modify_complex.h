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
            mData0=110;
            break;

        case ClassKind::Kind2:
            mData0=210;
            break;

        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
        if (mClassKind != ClassKind::Default)
        {
            mData1=10020;
        }
    }

    // チェック
    void check(ClassKind iClassKind, bool iProcess=false)
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

        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }

        // upVersion処理有り
        if (iProcess)
        {
            // Non-keep-stepのバージョン・アップ組合せテスト
            switch(gVersionList[gDataIndex].mVersionEnum)
            {
            case VersionEnum::ver1a:
            case VersionEnum::ver1b:
                THEOLIZER_EQUAL(mEnum00, ClassKind::Origin, (void*)&mEnum00);
                THEOLIZER_EQUAL(mEnum01, ClassKind::Origin, (void*)&mEnum01);
                THEOLIZER_EQUAL(mEnum02, ClassKind::Origin, (void*)&mEnum02);
                THEOLIZER_EQUAL(mEnum03, ClassKind::Origin, (void*)&mEnum03);
                THEOLIZER_EQUAL(mEnum04, ClassKind::Set,    (void*)&mEnum04);
                THEOLIZER_EQUAL(mEnum05, ClassKind::Set,    (void*)&mEnum05);
                THEOLIZER_EQUAL(mEnum06, ClassKind::Set,    (void*)&mEnum06);

                THEOLIZER_EQUAL(mEnum10, ClassKind::Origin, (void*)&mEnum10);
                THEOLIZER_EQUAL(mEnum11, ClassKind::Origin, (void*)&mEnum11);
                THEOLIZER_EQUAL(mEnum12, ClassKind::Origin, (void*)&mEnum12);
                THEOLIZER_EQUAL(mEnum13, ClassKind::Origin, (void*)&mEnum13);
                THEOLIZER_EQUAL(mEnum14, ClassKind::Set,    (void*)&mEnum14);
                THEOLIZER_EQUAL(mEnum15, ClassKind::Set,    (void*)&mEnum15);
                THEOLIZER_EQUAL(mEnum16, ClassKind::Set,    (void*)&mEnum16);

                THEOLIZER_EQUAL(mInt00, eisOrigin, (void*)&mInt00);
                THEOLIZER_EQUAL(mInt01, eisOrigin, (void*)&mInt01);
                THEOLIZER_EQUAL(mInt02, eisOrigin, (void*)&mInt02);
                THEOLIZER_EQUAL(mInt03, eisOrigin, (void*)&mInt03);
                THEOLIZER_EQUAL(mInt04, eisSet,    (void*)&mInt04);
                THEOLIZER_EQUAL(mInt05, eisSet,    (void*)&mInt05);
                THEOLIZER_EQUAL(mInt06, eisSet,    (void*)&mInt06);

                THEOLIZER_EQUAL(mInt10, eisOrigin, (void*)&mInt10);
                THEOLIZER_EQUAL(mInt11, eisOrigin, (void*)&mInt11);
                THEOLIZER_EQUAL(mInt12, eisOrigin, (void*)&mInt12);
                THEOLIZER_EQUAL(mInt13, eisOrigin, (void*)&mInt13);
                THEOLIZER_EQUAL(mInt14, eisSet,    (void*)&mInt14);
                THEOLIZER_EQUAL(mInt15, eisSet,    (void*)&mInt15);
                THEOLIZER_EQUAL(mInt16, eisSet,    (void*)&mInt16);
                break;

            case VersionEnum::ver1c:
                THEOLIZER_EQUAL(mEnum00, ClassKind::Load,   (void*)&mEnum00);
                THEOLIZER_EQUAL(mEnum01, ClassKind::Ope,    (void*)&mEnum01);
                THEOLIZER_EQUAL(mEnum02, ClassKind::Load,   (void*)&mEnum02);
                THEOLIZER_EQUAL(mEnum03, ClassKind::Ope,    (void*)&mEnum03);
                THEOLIZER_EQUAL(mEnum04, ClassKind::Set,    (void*)&mEnum04);
                THEOLIZER_EQUAL(mEnum05, ClassKind::Set,    (void*)&mEnum05);
                THEOLIZER_EQUAL(mEnum06, ClassKind::Set,    (void*)&mEnum06);

                THEOLIZER_EQUAL(mEnum10, ClassKind::Origin, (void*)&mEnum10);
                THEOLIZER_EQUAL(mEnum11, ClassKind::Origin, (void*)&mEnum11);
                THEOLIZER_EQUAL(mEnum12, ClassKind::Origin, (void*)&mEnum12);
                THEOLIZER_EQUAL(mEnum13, ClassKind::Origin, (void*)&mEnum13);
                THEOLIZER_EQUAL(mEnum14, ClassKind::Set,    (void*)&mEnum14);
                THEOLIZER_EQUAL(mEnum15, ClassKind::Set,    (void*)&mEnum15);
                THEOLIZER_EQUAL(mEnum16, ClassKind::Set,    (void*)&mEnum16);

                THEOLIZER_EQUAL(mInt00, eisLoad,   (void*)&mInt00);
                THEOLIZER_EQUAL(mInt01, eisOpe,    (void*)&mInt01);
                THEOLIZER_EQUAL(mInt02, eisLoad,   (void*)&mInt02);
                THEOLIZER_EQUAL(mInt03, eisOpe,    (void*)&mInt03);
                THEOLIZER_EQUAL(mInt04, eisSet,    (void*)&mInt04);
                THEOLIZER_EQUAL(mInt05, eisSet,    (void*)&mInt05);
                THEOLIZER_EQUAL(mInt06, eisSet,    (void*)&mInt06);

                THEOLIZER_EQUAL(mInt10, eisOrigin, (void*)&mInt10);
                THEOLIZER_EQUAL(mInt11, eisOrigin, (void*)&mInt11);
                THEOLIZER_EQUAL(mInt12, eisOrigin, (void*)&mInt12);
                THEOLIZER_EQUAL(mInt13, eisOrigin, (void*)&mInt13);
                THEOLIZER_EQUAL(mInt14, eisSet,    (void*)&mInt14);
                THEOLIZER_EQUAL(mInt15, eisSet,    (void*)&mInt15);
                THEOLIZER_EQUAL(mInt16, eisSet,    (void*)&mInt16);
                break;

            case VersionEnum::ver2a:
                THEOLIZER_EQUAL(mEnum00, ClassKind::Load, (void*)&mEnum00);
                THEOLIZER_EQUAL(mEnum01, ClassKind::Load, (void*)&mEnum01);
                THEOLIZER_EQUAL(mEnum02, ClassKind::Load, (void*)&mEnum02);
                THEOLIZER_EQUAL(mEnum03, ClassKind::Load, (void*)&mEnum03);
                THEOLIZER_EQUAL(mEnum04, ClassKind::Load, (void*)&mEnum04);
                THEOLIZER_EQUAL(mEnum05, ClassKind::Load, (void*)&mEnum05);
                THEOLIZER_EQUAL(mEnum06, ClassKind::Load, (void*)&mEnum06);

                THEOLIZER_EQUAL(mEnum10, ClassKind::Load, (void*)&mEnum10);
                THEOLIZER_EQUAL(mEnum11, ClassKind::Load, (void*)&mEnum11);
                THEOLIZER_EQUAL(mEnum12, ClassKind::Load, (void*)&mEnum12);
                THEOLIZER_EQUAL(mEnum13, ClassKind::Load, (void*)&mEnum13);
                THEOLIZER_EQUAL(mEnum14, ClassKind::Load, (void*)&mEnum14);
                THEOLIZER_EQUAL(mEnum15, ClassKind::Load, (void*)&mEnum15);
                THEOLIZER_EQUAL(mEnum16, ClassKind::Load, (void*)&mEnum16);

                THEOLIZER_EQUAL(mInt00, eisLoad, (void*)&mInt00);
                THEOLIZER_EQUAL(mInt01, eisLoad, (void*)&mInt01);
                THEOLIZER_EQUAL(mInt02, eisLoad, (void*)&mInt02);
                THEOLIZER_EQUAL(mInt03, eisLoad, (void*)&mInt03);
                THEOLIZER_EQUAL(mInt04, eisLoad, (void*)&mInt04);
                THEOLIZER_EQUAL(mInt05, eisLoad, (void*)&mInt05);
                THEOLIZER_EQUAL(mInt06, eisLoad, (void*)&mInt06);

                THEOLIZER_EQUAL(mInt10, eisLoad, (void*)&mInt10);
                THEOLIZER_EQUAL(mInt11, eisLoad, (void*)&mInt11);
                THEOLIZER_EQUAL(mInt12, eisLoad, (void*)&mInt12);
                THEOLIZER_EQUAL(mInt13, eisLoad, (void*)&mInt13);
                THEOLIZER_EQUAL(mInt14, eisLoad, (void*)&mInt14);
                THEOLIZER_EQUAL(mInt15, eisLoad, (void*)&mInt15);
                THEOLIZER_EQUAL(mInt16, eisLoad, (void*)&mInt16);
                break;

            case VersionEnum::ver3a:
            case VersionEnum::ver3b:
            default:
                // FAILさせる
                THEOLIZER_EQUAL(gDataIndex, gMyIndex);
                break;
            }
        }

        // upVersion処理無し
        else
        {
            // Non-keep-stepのバージョン・アップ組合せテスト
            switch(gVersionList[gDataIndex].mVersionEnum)
            {
            case VersionEnum::ver1a:
            case VersionEnum::ver1b:
                THEOLIZER_EQUAL(mEnum00, ClassKind::Origin, (void*)&mEnum00);
                THEOLIZER_EQUAL(mEnum01, ClassKind::Origin, (void*)&mEnum01);
                THEOLIZER_EQUAL(mEnum02, ClassKind::Origin, (void*)&mEnum02);
                THEOLIZER_EQUAL(mEnum03, ClassKind::Origin, (void*)&mEnum03);
                THEOLIZER_EQUAL(mEnum04, ClassKind::Inner,  (void*)&mEnum04);
                THEOLIZER_EQUAL(mEnum05, ClassKind::Inner,  (void*)&mEnum05);
                THEOLIZER_EQUAL(mEnum06, ClassKind::Inner,  (void*)&mEnum06);

                THEOLIZER_EQUAL(mEnum10, ClassKind::Origin, (void*)&mEnum10);
                THEOLIZER_EQUAL(mEnum11, ClassKind::Origin, (void*)&mEnum11);
                THEOLIZER_EQUAL(mEnum12, ClassKind::Origin, (void*)&mEnum12);
                THEOLIZER_EQUAL(mEnum13, ClassKind::Origin, (void*)&mEnum13);
                THEOLIZER_EQUAL(mEnum14, ClassKind::Inner,  (void*)&mEnum14);
                THEOLIZER_EQUAL(mEnum15, ClassKind::Inner,  (void*)&mEnum15);
                THEOLIZER_EQUAL(mEnum16, ClassKind::Inner,  (void*)&mEnum16);

                THEOLIZER_EQUAL(mInt00, eisOrigin, (void*)&mInt00);
                THEOLIZER_EQUAL(mInt01, eisOrigin, (void*)&mInt01);
                THEOLIZER_EQUAL(mInt02, eisOrigin, (void*)&mInt02);
                THEOLIZER_EQUAL(mInt03, eisOrigin, (void*)&mInt03);
                THEOLIZER_EQUAL(mInt04, eisInner,  (void*)&mInt04);
                THEOLIZER_EQUAL(mInt05, eisInner,  (void*)&mInt05);
                THEOLIZER_EQUAL(mInt06, eisInner,  (void*)&mInt06);

                THEOLIZER_EQUAL(mInt10, eisOrigin, (void*)&mInt10);
                THEOLIZER_EQUAL(mInt11, eisOrigin, (void*)&mInt11);
                THEOLIZER_EQUAL(mInt12, eisOrigin, (void*)&mInt12);
                THEOLIZER_EQUAL(mInt13, eisOrigin, (void*)&mInt13);
                THEOLIZER_EQUAL(mInt14, eisInner,  (void*)&mInt14);
                THEOLIZER_EQUAL(mInt15, eisInner,  (void*)&mInt15);
                THEOLIZER_EQUAL(mInt16, eisInner,  (void*)&mInt16);
                break;

            case VersionEnum::ver1c:
                THEOLIZER_EQUAL(mEnum00, ClassKind::Load,   (void*)&mEnum00);
                THEOLIZER_EQUAL(mEnum01, ClassKind::Inner,  (void*)&mEnum01);
                THEOLIZER_EQUAL(mEnum02, ClassKind::Load,   (void*)&mEnum02);
                THEOLIZER_EQUAL(mEnum03, ClassKind::Inner,  (void*)&mEnum03);
                THEOLIZER_EQUAL(mEnum04, ClassKind::Inner,  (void*)&mEnum04);
                THEOLIZER_EQUAL(mEnum05, ClassKind::Inner,  (void*)&mEnum05);
                THEOLIZER_EQUAL(mEnum06, ClassKind::Inner,  (void*)&mEnum06);

                THEOLIZER_EQUAL(mEnum10, ClassKind::Origin, (void*)&mEnum10);
                THEOLIZER_EQUAL(mEnum11, ClassKind::Origin, (void*)&mEnum11);
                THEOLIZER_EQUAL(mEnum12, ClassKind::Origin, (void*)&mEnum12);
                THEOLIZER_EQUAL(mEnum13, ClassKind::Origin, (void*)&mEnum13);
                THEOLIZER_EQUAL(mEnum14, ClassKind::Inner,  (void*)&mEnum14);
                THEOLIZER_EQUAL(mEnum15, ClassKind::Inner,  (void*)&mEnum15);
                THEOLIZER_EQUAL(mEnum16, ClassKind::Inner,  (void*)&mEnum16);

                THEOLIZER_EQUAL(mInt00, eisLoad,   (void*)&mInt00);
                THEOLIZER_EQUAL(mInt01, eisInner,  (void*)&mInt01);
                THEOLIZER_EQUAL(mInt02, eisLoad,   (void*)&mInt02);
                THEOLIZER_EQUAL(mInt03, eisInner,  (void*)&mInt03);
                THEOLIZER_EQUAL(mInt04, eisInner,  (void*)&mInt04);
                THEOLIZER_EQUAL(mInt05, eisInner,  (void*)&mInt05);
                THEOLIZER_EQUAL(mInt06, eisInner,  (void*)&mInt06);

                THEOLIZER_EQUAL(mInt10, eisOrigin, (void*)&mInt10);
                THEOLIZER_EQUAL(mInt11, eisOrigin, (void*)&mInt11);
                THEOLIZER_EQUAL(mInt12, eisOrigin, (void*)&mInt12);
                THEOLIZER_EQUAL(mInt13, eisOrigin, (void*)&mInt13);
                THEOLIZER_EQUAL(mInt14, eisInner,  (void*)&mInt14);
                THEOLIZER_EQUAL(mInt15, eisInner,  (void*)&mInt15);
                THEOLIZER_EQUAL(mInt16, eisInner,  (void*)&mInt16);
                break;

            case VersionEnum::ver2a:
                THEOLIZER_EQUAL(mEnum00, ClassKind::Load, (void*)&mEnum00);
                THEOLIZER_EQUAL(mEnum01, ClassKind::Load, (void*)&mEnum01);
                THEOLIZER_EQUAL(mEnum02, ClassKind::Load, (void*)&mEnum02);
                THEOLIZER_EQUAL(mEnum03, ClassKind::Load, (void*)&mEnum03);
                THEOLIZER_EQUAL(mEnum04, ClassKind::Load, (void*)&mEnum04);
                THEOLIZER_EQUAL(mEnum05, ClassKind::Load, (void*)&mEnum05);
                THEOLIZER_EQUAL(mEnum06, ClassKind::Load, (void*)&mEnum06);

                THEOLIZER_EQUAL(mEnum10, ClassKind::Load, (void*)&mEnum10);
                THEOLIZER_EQUAL(mEnum11, ClassKind::Load, (void*)&mEnum11);
                THEOLIZER_EQUAL(mEnum12, ClassKind::Load, (void*)&mEnum12);
                THEOLIZER_EQUAL(mEnum13, ClassKind::Load, (void*)&mEnum13);
                THEOLIZER_EQUAL(mEnum14, ClassKind::Load, (void*)&mEnum14);
                THEOLIZER_EQUAL(mEnum15, ClassKind::Load, (void*)&mEnum15);
                THEOLIZER_EQUAL(mEnum16, ClassKind::Load, (void*)&mEnum16);

                THEOLIZER_EQUAL(mInt00, eisLoad, (void*)&mInt00);
                THEOLIZER_EQUAL(mInt01, eisLoad, (void*)&mInt01);
                THEOLIZER_EQUAL(mInt02, eisLoad, (void*)&mInt02);
                THEOLIZER_EQUAL(mInt03, eisLoad, (void*)&mInt03);
                THEOLIZER_EQUAL(mInt04, eisLoad, (void*)&mInt04);
                THEOLIZER_EQUAL(mInt05, eisLoad, (void*)&mInt05);
                THEOLIZER_EQUAL(mInt06, eisLoad, (void*)&mInt06);

                THEOLIZER_EQUAL(mInt10, eisLoad, (void*)&mInt10);
                THEOLIZER_EQUAL(mInt11, eisLoad, (void*)&mInt11);
                THEOLIZER_EQUAL(mInt12, eisLoad, (void*)&mInt12);
                THEOLIZER_EQUAL(mInt13, eisLoad, (void*)&mInt13);
                THEOLIZER_EQUAL(mInt14, eisLoad, (void*)&mInt14);
                THEOLIZER_EQUAL(mInt15, eisLoad, (void*)&mInt15);
                THEOLIZER_EQUAL(mInt16, eisLoad, (void*)&mInt16);
                break;

            case VersionEnum::ver3a:
            case VersionEnum::ver3b:
            default:
                // FAILさせる
                THEOLIZER_EQUAL(gDataIndex, gMyIndex);
                break;
            }
        }
    }

    THEOLIZER_INTRUSIVE(CS, (VersionUpDownTest), 2);
};

//      ---<<< バージョン・アップ／ダウン処理 ver1 <-> ver2 >>>---

template<class tTheolizerVersion, class tNextVersion>
struct VersionUpDownTest::TheolizerUserDefine<tTheolizerVersion, tNextVersion, 1>
{
    // Members version down.
    static void downVersion(tNextVersion const& iNextVersion, tTheolizerVersion& oNowVersion)
    {
        switch(iNextVersion.mClassKind)
        {
        case kClassKind2Default:
            break;

        case kClassKind2Kind1:
            oNowVersion.mData0=iNextVersion.mData0-100;
            break;

        case kClassKind2Kind2:
            oNowVersion.mData0=iNextVersion.mData0-200;
            break;

        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
        oNowVersion.mData1=iNextVersion.mData1-10000;
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

        // Non-keep-stepのバージョン・アップ組合せテスト
        //  外側にupVersionが有るときは、内側のupVersionが無視されることを確認する
        oNextVersion.mEnum01=kClassKind2Inner;              // operator=
        oNextVersion.mEnum02.set(kClassKind2Inner, false);  // 無効set
        oNextVersion.mEnum03=kClassKind2Inner;              // operator=
        oNextVersion.mEnum03.set(kClassKind2Inner, false);  // 無効set
        oNextVersion.mEnum04.set(kClassKind2Inner, true);   // 有効set
        oNextVersion.mEnum05=kClassKind2Inner;              // operator=→
        oNextVersion.mEnum05.set(kClassKind2Inner, true);   // 有効set
        oNextVersion.mEnum06.set(kClassKind2Inner, true);   // 有効set  →
        oNextVersion.mEnum06=kClassKind2Inner;              // operator=

        oNextVersion.mEnum11=kClassKind2Inner;              // operator=
        oNextVersion.mEnum12.set(kClassKind2Inner, false);  // 無効set
        oNextVersion.mEnum13=kClassKind2Inner;              // operator=
        oNextVersion.mEnum13.set(kClassKind2Inner, false);  // 無効set
        oNextVersion.mEnum14.set(kClassKind2Inner, true);   // 有効set
        oNextVersion.mEnum15=kClassKind2Inner;              // operator=→
        oNextVersion.mEnum15.set(kClassKind2Inner, true);   // 有効set
        oNextVersion.mEnum16.set(kClassKind2Inner, true);   // 有効set  →
        oNextVersion.mEnum16=kClassKind2Inner;              // operator=

        oNextVersion.mInt01=eisInner;                       // operator=
        oNextVersion.mInt02.set(eisInner, false);           // 無効set
        oNextVersion.mInt03=eisInner;                       // operator=
        oNextVersion.mInt03.set(eisInner, false);           // 無効set
        oNextVersion.mInt04.set(eisInner, true);            // 有効set
        oNextVersion.mInt05=eisInner;                       // operator=→
        oNextVersion.mInt05.set(eisInner, true);            // 有効set
        oNextVersion.mInt06.set(eisInner, true);            // 有効set  →
        oNextVersion.mInt06=eisInner;                       // operator=

        oNextVersion.mInt11=eisInner;                       // operator=
        oNextVersion.mInt12.set(eisInner, false);           // 無効set
        oNextVersion.mInt13=eisInner;                       // operator=
        oNextVersion.mInt13.set(eisInner, false);           // 無効set
        oNextVersion.mInt14.set(eisInner, true);            // 有効set
        oNextVersion.mInt15=eisInner;                       // operator=→
        oNextVersion.mInt15.set(eisInner, true);            // 有効set
        oNextVersion.mInt16.set(eisInner, true);            // 有効set  →
        oNextVersion.mInt16=eisInner;                       // operator=
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
            THEOLIZER_EQUAL(mManual, 2.5);
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
        VersionUpDownTest::check(ClassKind::Kind2, true);
        VersionFullAuto::check();
        VersionManual::check();
        mVersionUpDownTest.check(ClassKind::Kind2);
        THEOLIZER_EQUAL_PTR( mVersionUpDownTestPtr, &iVersionUpDownTest);
        THEOLIZER_EQUAL_PTR(&mVersionUpDownTestRef, &iVersionUpDownTest);
        mVersionFullAuto.check();
        mVersionManual.check();
    }

    THEOLIZER_INTRUSIVE(CS, (KeepStepTest), 2);
};

//      ---<<< バージョン・アップ／ダウン処理 ver1 <-> ver2 >>>---

template<class tTheolizerVersion, class tNextVersion>
struct KeepStepTest::TheolizerUserDefine<tTheolizerVersion, tNextVersion, 1>
{
    // Members version down.
    static void downVersion(tNextVersion const& iNextVersion, tTheolizerVersion& oNowVersion)
    {
    }

    // Members version up.
    static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)
    {
        THEOLIZER_EQUAL(iNowVersion.mClassKind.get(), kClassKind1Kind1);
        THEOLIZER_EQUAL(iNowVersion.mVersionUpDownTest.mClassKind.get(), kClassKind1Kind1);

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
            THEOLIZER_EQUAL(iNowVersion.mFullAuto,  123);
            oNextVersion.VersionManualTheolizerBase.check();
            THEOLIZER_EQUAL(iNowVersion.mVersionFullAuto.mFullAuto,  123);
            iNowVersion.mVersionManual.get().check();
            break;

        case VersionEnum::ver3a:
        case VersionEnum::ver3b:
        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }

        // Non-keep-stepのバージョン・アップ組合せテスト
        //  基底クラスVersionUpDownTestのメンバを書き換える
        oNextVersion.mEnum01=kClassKind2Ope;            // operator=
        oNextVersion.mEnum02.set(kClassKind2Set, false);// 無効set
        oNextVersion.mEnum03=kClassKind2Ope;            // operator=
        oNextVersion.mEnum03.set(kClassKind2Set, false);// 無効set
        oNextVersion.mEnum04.set(kClassKind2Set, true); // 有効set
        oNextVersion.mEnum05=kClassKind2Ope;            // operator=→
        oNextVersion.mEnum05.set(kClassKind2Set, true); // 有効set
        oNextVersion.mEnum06.set(kClassKind2Set, true); // 有効set→
        oNextVersion.mEnum06=kClassKind2Ope;            // operator=

        oNextVersion.mEnum11=kClassKind2Ope;            // operator=
        oNextVersion.mEnum12.set(kClassKind2Set, false);// 無効set
        oNextVersion.mEnum13=kClassKind2Ope;            // operator=
        oNextVersion.mEnum13.set(kClassKind2Set, false);// 無効set
        oNextVersion.mEnum14.set(kClassKind2Set, true); // 有効set
        oNextVersion.mEnum15=kClassKind2Ope;            // operator=→
        oNextVersion.mEnum15.set(kClassKind2Set, true); // 有効set
        oNextVersion.mEnum16.set(kClassKind2Set, true); // 有効set→
        oNextVersion.mEnum16=kClassKind2Ope;            // operator=

        oNextVersion.mInt01=eisOpe;                     // operator=
        oNextVersion.mInt02.set(eisSet, false);         // 無効set
        oNextVersion.mInt03=eisOpe;                     // operator=
        oNextVersion.mInt03.set(eisSet, false);         // 無効set
        oNextVersion.mInt04.set(eisSet, true);          // 有効set
        oNextVersion.mInt05=eisOpe;                     // operator=→
        oNextVersion.mInt05.set(eisSet, true);          // 有効set
        oNextVersion.mInt06.set(eisSet, true);          // 有効set  →
        oNextVersion.mInt06=eisOpe;                     // operator=

        oNextVersion.mInt11=eisOpe;                     // operator=
        oNextVersion.mInt12.set(eisSet, false);         // 無効set
        oNextVersion.mInt13=eisOpe;                     // operator=
        oNextVersion.mInt13.set(eisSet, false);         // 無効set
        oNextVersion.mInt14.set(eisSet, true);          // 有効set
        oNextVersion.mInt15=eisOpe;                     // operator=→
        oNextVersion.mInt15.set(eisSet, true);          // 有効set
        oNextVersion.mInt16.set(eisSet, true);          // 有効set  →
        oNextVersion.mInt16=eisOpe;                     // operator=
    }
};

#endif  // TEST_MODIFY_COMPLEX_H
