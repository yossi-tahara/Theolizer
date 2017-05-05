//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          クラス変更の使い方
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

#if !defined(TEST_MODIFY_CLASS_H)
#define TEST_MODIFY_CLASS_H

#include <string>

// ***************************************************************************
//      変更テストの被包含クラス
// ***************************************************************************

//----------------------------------------------------------------------------
//      非侵入型全自動
//----------------------------------------------------------------------------

struct ModifyFullAuto
{
    int     mFullAuto;

    ModifyFullAuto(int iValue=0) : mFullAuto(iValue) { }
    void check(int iValue)
    {
        THEOLIZER_EQUAL(mFullAuto, iValue);
    }
};

//----------------------------------------------------------------------------
//      侵入型半自動
//----------------------------------------------------------------------------

//      ---<<< ver1aとver3bで使用する >>>---

struct ModifyHalfAuto
{
    int     mHalfAuto;

    ModifyHalfAuto(int iValue=0) : mHalfAuto(iValue) { }
    void check(int iValue)
    {
        THEOLIZER_EQUAL(mHalfAuto, iValue);
    }
    THEOLIZER_INTRUSIVE(CS, (ModifyHalfAuto), 1);
};

//----------------------------------------------------------------------------
//      非侵入型手動
//----------------------------------------------------------------------------

struct ModifyManual
{
    int     mManual;

    ModifyManual(int iValue=0) : mManual(iValue) { }
    void check(int iValue)
    {
        THEOLIZER_EQUAL(mManual, iValue);
    }
};

// 非侵入型手動クラスの指定
THEOLIZER_NON_INTRUSIVE_ORDER((ModifyManual), 1);

// 保存処理／回復処理関数
template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<ModifyManual>::
    TheolizerUserDefine<tBaseSerializer, tTheolizerVersion, 1>
{
    // Save members.
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        THEOLIZER_PROCESS(iSerializer, iInstance->mManual);
    }

    // Load members.
    static void loadClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        if (!oInstance) oInstance=new typename tTheolizerVersion::TheolizerTarget();
        
        THEOLIZER_PROCESS(iSerializer, oInstance->mManual);
    }
};

// ***************************************************************************
//      変更テスト用クラス（名前対応）
// ***************************************************************************

#ifdef ENABLE_MODIFY_CLASS_TEST_NAME
struct ModifyClassName :
    // --- 基底クラス ---
    public ModifyFullAuto,
    public ModifyHalfAuto,
    public ModifyManual
{
    // --- クラス型メンバ変数 ---
    ModifyFullAuto  mFullAutoMember;
    ModifyHalfAuto  mHalfAutoMember;
    ModifyManual    mManualMember;

    // --- 基本型メンバ変数 ---
    short           mShort;
    int             mInt;
    unsigned        mUnsigned;

//  以下略
//  ↑ドキュメント用のコメント

//----------------------------------------------------------------------------
//      デフォルト・コンストラクタ
//----------------------------------------------------------------------------

    ModifyClassName()     :
        // --- 基底クラス ---
        ModifyFullAuto(0),
        ModifyHalfAuto(0),
        ModifyManual(0),

        // --- クラス型メンバ変数 ---
        mFullAutoMember(0),
        mHalfAutoMember(0),
        mManualMember(0),

        // --- 基本型メンバ変数 ---
        mShort(0),
        mInt(0),
        mUnsigned(0)
    { }

//----------------------------------------------------------------------------
//      保存データ設定用コンストラクタ
//----------------------------------------------------------------------------

    ModifyClassName(bool) :
        // --- 基底クラス ---
        ModifyFullAuto(100),
        ModifyHalfAuto(101),
        ModifyManual(102),

        // --- クラス型メンバ変数 ---
        mFullAutoMember(110),
        mHalfAutoMember(111),
        mManualMember(112),

        // --- 基本型メンバ変数 ---
        mShort(120),
        mInt(121),
        mUnsigned(122)
    { }

//----------------------------------------------------------------------------
//      チェック
//----------------------------------------------------------------------------

    void check()
    {
        // --- 基底クラス ---
        THEOLIZER_EQUAL(mFullAuto, 100);
        THEOLIZER_EQUAL(mHalfAuto, 101);
        THEOLIZER_EQUAL(mManual,   102);

        // --- クラス型メンバ変数 ---
        THEOLIZER_EQUAL(mFullAutoMember.mFullAuto, 110);
        THEOLIZER_EQUAL(mHalfAutoMember.mHalfAuto, 111);
        THEOLIZER_EQUAL(mManualMember.mManual,     112);

        // --- 基本型メンバ変数 ---
        THEOLIZER_EQUAL(mShort,    120);
        THEOLIZER_EQUAL(mInt,      121);
        THEOLIZER_EQUAL(mUnsigned, 122);
    }
//  THEOLIZER_INTRUSIVE(CS, (ModifyClassName), 1);  // 最初は非侵入型完全自動
};
#endif  // ENABLE_MODIFY_CLASS_TEST_NAME

// ***************************************************************************
//      変更テスト用クラス（順序対応）
// ***************************************************************************

#ifdef ENABLE_MODIFY_CLASS_TEST_ORDER
struct ModifyClassOrder :
    // --- 基底クラス ---
    public ModifyFullAuto,
    public ModifyHalfAuto,
    public ModifyManual
{
    // --- クラス型メンバ変数 ---
    ModifyFullAuto  mFullAutoMember;
    ModifyHalfAuto  mHalfAutoMember;
    ModifyManual    mManualMember;

    // --- 基本型メンバ変数 ---
    short           mShort;
    int             mInt;
    unsigned        mUnsigned;

//  以下略
//  ↑ドキュメント用のコメント

//----------------------------------------------------------------------------
//      デフォルト・コンストラクタ
//----------------------------------------------------------------------------

    ModifyClassOrder() :
        // --- 基底クラス ---
        ModifyFullAuto(0),
        ModifyHalfAuto(0),
        ModifyManual(0),

        // --- クラス型メンバ変数 ---
        mFullAutoMember(0),
        mHalfAutoMember(0),
        mManualMember(0),

        // --- 基本型メンバ変数 ---
        mShort(0),
        mInt(0),
        mUnsigned(0)
    { }

//----------------------------------------------------------------------------
//      保存データ設定用コンストラクタ
//----------------------------------------------------------------------------

    ModifyClassOrder(bool) :
        // --- 基底クラス ---
        ModifyFullAuto(200),
        ModifyHalfAuto(201),
        ModifyManual(202),

        // --- クラス型メンバ変数 ---
        mFullAutoMember(210),
        mHalfAutoMember(211),
        mManualMember(212),

        // --- 基本型メンバ変数 ---
        mShort(220),
        mInt(221),
        mUnsigned(222)
    { }

//----------------------------------------------------------------------------
//      チェック
//----------------------------------------------------------------------------

    void check()
    {
        // --- 基底クラス ---
        THEOLIZER_EQUAL(mFullAuto, 200);
        THEOLIZER_EQUAL(mHalfAuto, 201);
        THEOLIZER_EQUAL(mManual,   202);

        // --- クラス型メンバ変数 ---
        THEOLIZER_EQUAL(mFullAutoMember.mFullAuto, 210);
        THEOLIZER_EQUAL(mHalfAutoMember.mHalfAuto, 211);
        THEOLIZER_EQUAL(mManualMember.mManual,     212);

        // --- 基本型メンバ変数 ---
        THEOLIZER_EQUAL(mShort,    220);
        THEOLIZER_EQUAL(mInt,      221);
        THEOLIZER_EQUAL(mUnsigned, 222);
    }
    THEOLIZER_INTRUSIVE_ORDER(CS, (ModifyClassOrder), 1);
};
#endif  // ENABLE_MODIFY_CLASS_TEST_ORDER

// ***************************************************************************
//      配列の要素数上限テスト
// ***************************************************************************

#ifdef ENABLE_MODIFY_CLASS_TEST_ARRAY
struct ArrayTest
{
    // --- サイズ上限テスト用 ---
    static const unsigned   kSizeA=2;
    static const unsigned   kSizeB=3;
    unsigned    mArray1D[kSizeA];
    unsigned    mArray2D[kSizeA][kSizeB];
    unsigned    mArray3D[kSizeA][kSizeB][kSizeA];

//  以下略
//  ↑ドキュメント用のコメント

//----------------------------------------------------------------------------
//      デフォルト・コンストラクタ
//----------------------------------------------------------------------------

    ArrayTest() :
        // --- サイズ上限テスト用 ---
        mArray1D{},
        mArray2D{},
        mArray3D{}
    { }

//----------------------------------------------------------------------------
//      保存データ設定用コンストラクタ
//----------------------------------------------------------------------------

    ArrayTest(bool)
    {
        // --- サイズ上限テスト用 ---
        for (unsigned i=0; i < kSizeA; ++i)
        {
            mArray1D[i]=i;
            for (unsigned j=0; j < kSizeB; ++j)
            {
                mArray2D[i][j]=i*1000+j;
                for (unsigned k=0; k < kSizeA; ++k)
                {
                    mArray3D[i][j][k]=(i*1000+j)*1000+k;
                }
            }
        }
    }

//----------------------------------------------------------------------------
//      チェック
//----------------------------------------------------------------------------

    void check()
    {
        unsigned   kSizeMin=2;
        if (gVersionList[gDataIndex].mVersionEnum == VersionEnum::ver1a)
        {
            kSizeMin=3;
        }

        // --- サイズ上限テスト用 ---
        for (unsigned i=0; i < kSizeA; ++i)
        {
            bool aValued0=i < kSizeMin;
            THEOLIZER_EQUAL(mArray1D[i], (aValued0)?i:0);
            for (unsigned j=0; j < kSizeB; ++j)
            {
                bool aValued1=aValued0 && (j < kSizeMin);
                THEOLIZER_EQUAL(mArray2D[i][j], (aValued1)?(i*1000+j):0, i, j);
                for (unsigned k=0; k < kSizeA; ++k)
                {
                    bool aValued2=aValued1 && (k < kSizeMin);
                    THEOLIZER_EQUAL(mArray3D[i][j][k], (aValued2)?((i*1000+j)*1000+k):0,
                        i, j, k);
                }
            }
        }
    }

    THEOLIZER_INTRUSIVE(CS, (ArrayTest), 1);
};
#endif  // ENABLE_MODIFY_CLASS_TEST_ARRAY

#endif  // TEST_MODIFY_CLASS_H
