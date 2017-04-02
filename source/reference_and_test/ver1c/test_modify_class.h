//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          クラス変更の使い方
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

struct ModifyHalfAutoX
{
    std::string mHalfAutoX;

    ModifyHalfAutoX(std::string const& iValue=0) : mHalfAutoX(iValue) { }
    void check(std::string const& iValue)
    {
        THEOLIZER_EQUAL(mHalfAutoX, iValue);
    }
    THEOLIZER_INTRUSIVE(CS, (ModifyHalfAutoX), 1);
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

#ifndef DISABLE_MODIFY_CLASS_TEST_NAME
struct ModifyClassName :
    // --- 基底クラス ---
    public ModifyManual,                // 順序変更
    public ModifyFullAuto,
//  public ModifyHalfAuto,              // 削除
    public ModifyHalfAutoX              // 追加
{
    // --- クラス型メンバ変数---
    ModifyManual    mManualMember;      // 順序変更
    ModifyFullAuto  mFullAutoMember;
//  ModifyHalfAuto  mHalfAutoMember;    // 削除
    ModifyHalfAutoX mHalfAutoXMember;   // 追加

    // --- 基本型メンバ変数---
    unsigned    mUnsigned;              // 順序変更
    short       mShort;
//  int         mInt;                   // 削除
    long        mLong;                  // 追加

//----------------------------------------------------------------------------
//      デフォルト・コンストラクタ
//----------------------------------------------------------------------------

    ModifyClassName() :
        // --- 基底クラス ---
        ModifyManual(0),                // 順序変更
        ModifyFullAuto(0),
//      ModifyHalfAuto(0),              // 削除
        ModifyHalfAutoX(""),            // 追加

        // --- クラス型メンバ変数---
        mManualMember(0),               // 順序変更
        mFullAutoMember(0),
//      mHalfAutoMember(0),             // 削除
        mHalfAutoXMember(""),           // 追加

        // --- 基本型メンバ変数---
        mUnsigned(0),                   // 順序変更
        mShort(0),
//      mInt(0),                        // 削除
        mLong(0)                        // 追加
    { }

//----------------------------------------------------------------------------
//      保存データ設定用コンストラクタ
//----------------------------------------------------------------------------

    ModifyClassName(bool) :
        // --- 基底クラス ---
        ModifyManual(102),              // 順序変更
        ModifyFullAuto(100),
//      ModifyHalfAuto(101),            // 削除
        ModifyHalfAutoX("103"),         // 追加

        // --- クラス型メンバ変数---
        mManualMember(112),             // 順序変更
        mFullAutoMember(110),
//      mHalfAutoMember(111),           // 削除
        mHalfAutoXMember("113"),        // 追加

        // --- 基本型メンバ変数---
        mUnsigned(122),                 // 順序変更
        mShort(120),
//      mInt(121),                      // 削除
        mLong(123)                      // 追加
    { }

//----------------------------------------------------------------------------
//      チェック
//----------------------------------------------------------------------------

    void check()
    {
        // --- 基底クラス ---
        THEOLIZER_EQUAL(mFullAuto, 100);
//      THEOLIZER_EQUAL(mHalfAuto, 101);
        THEOLIZER_EQUAL(mManual,   102);
        switch(gVersionList[gDataIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
            THEOLIZER_EQUAL(mHalfAutoX, "");
            break;

        case VersionEnum::ver1b:
        case VersionEnum::ver1c:
            THEOLIZER_EQUAL(mHalfAutoX, "103");
            break;

        case VersionEnum::ver2a:
        case VersionEnum::ver3a:
        case VersionEnum::ver3b:
        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }

        // --- 基本型メンバ変数---
//      THEOLIZER_EQUAL(mInt,      121);
        THEOLIZER_EQUAL(mUnsigned, 122);
        switch(gVersionList[gDataIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
            THEOLIZER_EQUAL(mShort, 120);
            THEOLIZER_EQUAL(mLong,  0);
            break;

        case VersionEnum::ver1b:
        case VersionEnum::ver1c:
            switch(gVersionList[gProgramIndex].mVersionEnum)
            {
            case VersionEnum::ver3b:
                THEOLIZER_EQUAL(mShort, 0);
                THEOLIZER_EQUAL(mLong,  0);
                break;

            default:
                THEOLIZER_EQUAL(mShort, 120);
                THEOLIZER_EQUAL(mLong,  123);
                break;
            }
            break;

        case VersionEnum::ver2a:
        case VersionEnum::ver3a:
        case VersionEnum::ver3b:
        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }
    THEOLIZER_INTRUSIVE(CS, (ModifyClassName), 1);
};
#endif  // DISABLE_MODIFY_CLASS_TEST_NAME

// ***************************************************************************
//      変更テスト用クラス（順序対応）  メンバ変数追加
// ***************************************************************************

#ifndef DISABLE_MODIFY_CLASS_TEST_ORDER
struct ModifyClassOrder
{
    short       mShort;
    int         mInt;
    unsigned    mUnsigned;
    long        mLong;      // 追加

    ModifyClassOrder()     : mShort(0)  , mInt(0)  , mUnsigned()   , mLong()    { }
    ModifyClassOrder(bool) : mShort(220), mInt(221), mUnsigned(222), mLong(223) { }
    void check()
    {
        THEOLIZER_EQUAL(mShort,    220);
        THEOLIZER_EQUAL(mInt,      221);
        THEOLIZER_EQUAL(mUnsigned, 222);
        switch(gVersionList[gDataIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
            THEOLIZER_EQUAL(mLong,  0);
            break;

        case VersionEnum::ver1b:
        case VersionEnum::ver1c:
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

        case VersionEnum::ver2a:
        case VersionEnum::ver3a:
        case VersionEnum::ver3b:
        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }
    THEOLIZER_INTRUSIVE_ORDER(CS, (ModifyClassOrder), 1);
};
#endif  // DISABLE_MODIFY_CLASS_TEST_ORDER

// ***************************************************************************
//      配列の要素数上限テスト
// ***************************************************************************

#ifndef DISABLE_MODIFY_CLASS_TEST_ARRAY
struct ArraySizeTest
{
    static const unsigned   kSize=kDefSize;
    unsigned    mArray1D[kSize];
    unsigned    mArray2D[kSize][kSize];
    unsigned    mArray3D[kSize][kSize][kSize];

    ArraySizeTest() : mArray1D{}, mArray2D{}, mArray3D{}
    { }
    ArraySizeTest(bool)
    {
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
    void check()
    {
        for (unsigned i=0; i < kSize; ++i)
        {
            THEOLIZER_EQUAL(mArray1D[i], i:0), i);
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

    THEOLIZER_INTRUSIVE(CS, (ArraySizeTest), 1);
};
#endif  // DISABLE_MODIFY_CLASS_TEST_ARRAY

#endif  // TEST_MODIFY_CLASS_H
