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

//      ---<<< 旧バージョンのみに存在する完全自動クラス型 >>>---

struct OldFullAuto0
{
    int     mOldFullAuto0;
    OldFullAuto0(int iValue=0) : mOldFullAuto0(iValue) { }
};

struct OldFullAuto1
{
    int     mOldFullAuto1;
    OldFullAuto1(int iValue=0) : mOldFullAuto1(iValue) { }
};

//      ---<<< 旧バージョンのみに存在する完全自動enul型 >>>---

enum OldFullAutoEnum
{
    Symbol0,
    Symbol1
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

//      ---<<< ver1b以降で使用する >>>---

struct ModifyHalfAutoY
{
    std::string mHalfAutoY THEOLIZER_ANNOTATE(FS:mHalfAutoX);

    ModifyHalfAutoY(std::string const& iValue=0) : mHalfAutoY(iValue) { }
    void check(std::string const& iValue)
    {
        THEOLIZER_EQUAL(mHalfAutoY, iValue);
    }
    THEOLIZER_INTRUSIVE(CS, (ModifyHalfAutoY), 2);
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
    public ModifyManual,                // 1b:順序変更
    public ModifyFullAuto,
//  public ModifyHalfAuto,              // 1b:削除
    public ModifyHalfAutoY              // 1b:追加→2a:名前変更
{
    // --- クラス型メンバ変数 ---
    ModifyManual    mManualMember;      // 1b:順序変更
    ModifyFullAuto  mFullAutoMember;
//  ModifyHalfAuto  mHalfAutoMember;    // 1b:削除
    ModifyHalfAutoY mHalfAutoYMember;   // 1b:追加→2a:名前変更

    // --- 基本型メンバ変数 ---
    unsigned    mUnsigned;              // 1b:順序変更
    short       mShortChanged THEOLIZER_ANNOTATE(FS:mShort); // 3a:変数名変更
//  int         mInt;                   // 1b:削除
    long        mLong;                  // 1b:追加

//----------------------------------------------------------------------------
//      デフォルト・コンストラクタ
//----------------------------------------------------------------------------

    ModifyClassName() :
        // --- 基底クラス ---
        ModifyManual(0),                // 1b:順序変更
        ModifyFullAuto(0),
//      ModifyHalfAuto(0),              // 1b:削除
        ModifyHalfAutoY(""),            // 1b:追加→2a:名変更

        // --- クラス型メンバ変数 ---
        mManualMember(0),               // 1b:順序変更
        mFullAutoMember(0),
//      mHalfAutoMember(0),             // 1b:削除
        mHalfAutoYMember(""),           // 1b:追加→2a:名変更

        // --- 基本型メンバ変数 ---
        mUnsigned(0),                   // 1b:順序変更
        mShortChanged(0),
//      mInt(0),                        // 1b:削除
        mLong(0)                        // 1b:追加
    { }

//----------------------------------------------------------------------------
//      保存データ設定用コンストラクタ
//----------------------------------------------------------------------------

    ModifyClassName(bool) :
        // --- 基底クラス ---
        ModifyManual(102),              // 1b:順序変更
        ModifyFullAuto(100),
//      ModifyHalfAuto(101),            // 1b:削除
        ModifyHalfAutoY("103"),         // 1b:追加→2a:名変更

        // --- クラス型メンバ変数 ---
        mManualMember(112),             // 1b:順序変更
        mFullAutoMember(110),
//      mHalfAutoMember(111),           // 1b:削除
        mHalfAutoYMember("113"),        // 1b:追加→2a:名変更

        // --- 基本型メンバ変数 ---
        mUnsigned(122),                 // 1b:順序変更
        mShortChanged(120),
//      mInt(121),                      // 1b:削除
        mLong(123)                      // 1b:追加
    { }

//----------------------------------------------------------------------------
//      チェック
//----------------------------------------------------------------------------

    void check()
    {
        // --- 基底クラス ---
//      THEOLIZER_EQUAL(mHalfAuto, 101);
        THEOLIZER_EQUAL(mManual,   102);
        switch(gVersionList[gDataIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
            THEOLIZER_EQUAL(mFullAuto, 100);
            THEOLIZER_EQUAL(mHalfAutoY, "");
            break;

        case VersionEnum::ver1b:
        case VersionEnum::ver1c:
        case VersionEnum::ver2a:
        case VersionEnum::ver3a:
            switch(gVersionList[gProgramIndex].mVersionEnum)
            {
            case VersionEnum::ver3b:
                THEOLIZER_EQUAL(mFullAuto,  0);
                THEOLIZER_EQUAL(mHalfAutoY, "");
                break;

            default:
                THEOLIZER_EQUAL(mFullAuto,  100);
                THEOLIZER_EQUAL(mHalfAutoY, "103");
                break;
            }
            break;

        case VersionEnum::ver3b:
        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }

        // --- クラス型メンバ変数 ---
//      THEOLIZER_EQUAL(mHalfAutoMember.mHalfAuto, 111);
        THEOLIZER_EQUAL(mManualMember.mManual,     112);
        switch(gVersionList[gDataIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
            THEOLIZER_EQUAL(mFullAutoMember.mFullAuto,   110);
            THEOLIZER_EQUAL(mHalfAutoYMember.mHalfAutoY, "");
            break;

        case VersionEnum::ver1b:
        case VersionEnum::ver1c:
        case VersionEnum::ver2a:
        case VersionEnum::ver3a:
            switch(gVersionList[gProgramIndex].mVersionEnum)
            {
            case VersionEnum::ver3b:
                THEOLIZER_EQUAL(mFullAutoMember.mFullAuto,   0);
                THEOLIZER_EQUAL(mHalfAutoYMember.mHalfAutoY, "");
                break;

            default:
                THEOLIZER_EQUAL(mFullAutoMember.mFullAuto,   110);
                THEOLIZER_EQUAL(mHalfAutoYMember.mHalfAutoY, "113");
                break;
            }
            break;

        case VersionEnum::ver3b:
        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }

        // --- 基本型メンバ変数 ---
//      THEOLIZER_EQUAL(mInt,           121);
        THEOLIZER_EQUAL(mUnsigned,      122);
        switch(gVersionList[gDataIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
            THEOLIZER_EQUAL(mShortChanged,  120);
            THEOLIZER_EQUAL(mLong,  0);
            break;

        case VersionEnum::ver1b:
        case VersionEnum::ver1c:
        case VersionEnum::ver2a:
        case VersionEnum::ver3a:
            switch(gVersionList[gProgramIndex].mVersionEnum)
            {
            case VersionEnum::ver3b:
                THEOLIZER_EQUAL(mShortChanged,  0);
                THEOLIZER_EQUAL(mLong,          0);
                break;

            default:
                THEOLIZER_EQUAL(mShortChanged,  120);
                THEOLIZER_EQUAL(mLong,          123);
                break;
            }
            break;

        case VersionEnum::ver3b:
        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }
    THEOLIZER_INTRUSIVE(CS, (ModifyClassName), 3);
};
#endif  // ENABLE_MODIFY_CLASS_TEST_NAME

// ***************************************************************************
//      変更テスト用クラス（順序対応）  メンバ変数追加
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
    int             mIntChanged THEOLIZER_ANNOTATE(FS:mInt);    // 変数名変更
    unsigned        mUnsigned;

    // 1b:追加→3b:削除
    ModifyHalfAutoY mHalfAutoYMember;   // 2a:名変更
    long            mLong;

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
        mIntChanged(0),
        mUnsigned(0),

        // 1b:追加
        mHalfAutoYMember(""),   // 2a:名変更
        mLong(0)
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
        mIntChanged(221),
        mUnsigned(222),

        // 1b:追加
        mHalfAutoYMember("213"),    // 2a:名変更
        mLong(223)
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
        THEOLIZER_EQUAL(mFullAutoMember.mFullAuto,   210);
        THEOLIZER_EQUAL(mHalfAutoMember.mHalfAuto,   211);
        THEOLIZER_EQUAL(mManualMember.mManual,       212);
        switch(gVersionList[gDataIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
            THEOLIZER_EQUAL(mHalfAutoYMember.mHalfAutoY, "");
            break;

        case VersionEnum::ver1b:
        case VersionEnum::ver1c:
        case VersionEnum::ver2a:
        case VersionEnum::ver3a:
            switch(gVersionList[gProgramIndex].mVersionEnum)
            {
            case VersionEnum::ver3b:
                THEOLIZER_EQUAL(mHalfAutoYMember.mHalfAutoY, "");
                break;

            default:
                THEOLIZER_EQUAL(mHalfAutoYMember.mHalfAutoY, "213");
                break;
            }
            break;

        case VersionEnum::ver3b:
        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }

        // --- 基本型メンバ変数 ---
        THEOLIZER_EQUAL(mShort,      220);
        THEOLIZER_EQUAL(mIntChanged, 221);
        THEOLIZER_EQUAL(mUnsigned,   222);
        switch(gVersionList[gDataIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
            THEOLIZER_EQUAL(mLong,  0);
            break;

        case VersionEnum::ver1b:
        case VersionEnum::ver1c:
        case VersionEnum::ver2a:
        case VersionEnum::ver3a:
            switch(gVersionList[gProgramIndex].mVersionEnum)
            {
            case VersionEnum::ver3b:
                THEOLIZER_EQUAL(mLong,  0);
                break;

            default:
                THEOLIZER_EQUAL(mLong,  223);
                break;
            }
            break;

        case VersionEnum::ver3b:
        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }
    THEOLIZER_INTRUSIVE_ORDER(CS, (ModifyClassOrder), 3);
};
#endif  // ENABLE_MODIFY_CLASS_TEST_ORDER

// ***************************************************************************
//      配列の要素数上限テスト
// ***************************************************************************

#ifdef ENABLE_MODIFY_CLASS_TEST_ARRAY
struct ArrayTest
{
    // --- 次元変更テスト用 ---
    static const unsigned   kSize1x=2;
    static const unsigned   kSize0x=3;
    static const unsigned   kSize1=3;
    static const unsigned   kSize0=2;
    unsigned    mArrayDim[kSize1][kSize0];

    // --- サイズ上限テスト用 ---
    static const unsigned   kSize=2;
    unsigned    mArray1D[kSize];
    unsigned    mArray2D[kSize][kSize];
    unsigned    mArray3D[kSize][kSize][kSize];

//----------------------------------------------------------------------------
//      デフォルト・コンストラクタ
//----------------------------------------------------------------------------

    ArrayTest() :
        // --- 次元変更テスト用 ---
        mArrayDim{},

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
        // --- 次元変更テスト用 ---
        for (unsigned j=0; j < kSize1; ++j)
        {
            for (unsigned k=0; k < kSize0; ++k)
            {
                mArrayDim[j][k]=j*10+k;
            }
        }

        // --- サイズ上限テスト用 ---
        for (unsigned i=0; i < kSize; ++i)
        {
            mArray1D[i]=i;
            for (unsigned j=0; j < kSize; ++j)
            {
                mArray2D[i][j]=i*1000+j;
                for (unsigned k=0; k < kSize; ++k)
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
        // --- 次元変更テスト用 ---
        for (unsigned j=0; j < kSize1; ++j)
        {
            for (unsigned k=0; k < kSize0; ++k)
            {
                if ((gVersionList[gDataIndex].mVersionEnum == VersionEnum::ver2a)
                 && ((gVersionList[gProgramIndex].mVersionEnum == VersionEnum::ver2a)
                  || (gVersionList[gProgramIndex].mVersionEnum == VersionEnum::ver3a)))
                {
                    THEOLIZER_EQUAL(mArrayDim[j][k], j*10+k, j, k);
                }
                else if ((gVersionList[gDataIndex].mVersionEnum == VersionEnum::ver3a)
                      && (gVersionList[gProgramIndex].mVersionEnum == VersionEnum::ver3a))
                {
                    THEOLIZER_EQUAL(mArrayDim[j][k], j*10+k, j, k);
                }
                else if ((VersionEnum::ver1c <= gVersionList[gDataIndex].mVersionEnum)
                      && (gVersionList[gDataIndex].mVersionEnum <= VersionEnum::ver3a))
                {
                    if ((j < kSize1x) && (k < kSize0x))
                    {
                        THEOLIZER_EQUAL(mArrayDim[j][k], j*10+k, j, k);
                    }
                    else
                    {
                        THEOLIZER_EQUAL(mArrayDim[j][k], 0, j, k);
                    }
                }
                else if (gVersionList[gDataIndex].mVersionEnum < VersionEnum::ver1c)
                {
                    THEOLIZER_EQUAL(mArrayDim[j][k], 0, j, k);
                }
                else
                {
                    // FAILさせる
                    THEOLIZER_EQUAL(gDataIndex, gMyIndex, j, k);
                }
            }
        }

        // --- サイズ上限テスト用 ---
        for (unsigned i=0; i < kSize; ++i)
        {
            THEOLIZER_EQUAL(mArray1D[i], i);
            for (unsigned j=0; j < kSize; ++j)
            {
                THEOLIZER_EQUAL(mArray2D[i][j], i*1000+j, i, j);
                for (unsigned k=0; k < kSize; ++k)
                {
                    THEOLIZER_EQUAL(mArray3D[i][j][k], (i*1000+j)*1000+k,
                        i, j, k);
                }
            }
        }
    }

    THEOLIZER_INTRUSIVE(CS, (ArrayTest), 3);
};
#endif  // ENABLE_MODIFY_CLASS_TEST_ARRAY

#endif  // TEST_MODIFY_CLASS_H
