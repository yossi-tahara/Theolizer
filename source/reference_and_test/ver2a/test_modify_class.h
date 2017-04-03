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

//      ---<<< 旧バージョンのみに存在する完全自動型 >>>---

struct OldFullAuto0
{
    int     mOldFullAuto0;
    OldFullAuto0() : mOldFullAuto0(0) { }
};

struct OldFullAuto1
{
    int     mOldFullAuto1;
    OldFullAuto1() : mOldFullAuto1(0) { }
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

#ifndef DISABLE_MODIFY_CLASS_TEST_NAME
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
    ModifyHalfAutoY mHalfAutoYMember THEOLIZER_ANNOTATE(FS:mHalfAutoXMember);// 1b:追加→2a:名変更

    // --- 基本型メンバ変数 ---
    unsigned    mUnsigned;              // 1b:順序変更
    short       mShort;
//  int         mInt;                   // 1b:削除
    long        mLong;                  // 1b:追加

    // --- クラス配列型メンバ変数 ---
    ModifyFullAuto  mFullAutoArray[2];  // 2b:追加(3a:削除)
    ModifyHalfAuto  mHalfAutoArray[2];  // 2b:追加(3a:削除)
    ModifyManual    mManualArray[2];    // 2b:追加(3a:削除)

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
        mShort(0),
//      mInt(0),                        // 1b:削除
        mLong(0),                       // 1b:追加

        // --- クラス配列型メンバ変数 ---
        mFullAutoArray{},               // 2b:追加(3a:削除)
        mHalfAutoArray{},               // 2b:追加(3a:削除)
        mManualArray{}                  // 2b:追加(3a:削除)
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
        mShort(120),
//      mInt(121),                      // 1b:削除
        mLong(123),                     // 1b:追加

        // --- クラス配列型メンバ変数 ---
        mFullAutoArray{130, 131},       // 2b:追加(3a:削除)
        mHalfAutoArray{132, 133},       // 2b:追加(3a:削除)
        mManualArray{134, 135}          // 2b:追加(3a:削除)
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
            THEOLIZER_EQUAL(mHalfAutoY,  "");
            break;

        case VersionEnum::ver1b:
        case VersionEnum::ver1c:
        case VersionEnum::ver2a:
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

        case VersionEnum::ver3a:
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

        case VersionEnum::ver3a:
        case VersionEnum::ver3b:
        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }

        // --- 基本型メンバ変数 ---
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
        case VersionEnum::ver2a:
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

        case VersionEnum::ver3a:
        case VersionEnum::ver3b:
        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }

        // --- クラス配列型メンバ変数 ---
        // ver2aが生成したver2aデータのみ値がある
        if ((gVersionList[gDataIndex].mVersionEnum == VersionEnum::ver2a)
         && (gVersionList[gProgramIndex].mVersionEnum == VersionEnum::ver2a))
        {
            THEOLIZER_EQUAL(mFullAutoArray[0].mFullAuto,    130);
            THEOLIZER_EQUAL(mFullAutoArray[1].mFullAuto,    131);
            THEOLIZER_EQUAL(mHalfAutoArray[0].mHalfAuto,    132);
            THEOLIZER_EQUAL(mHalfAutoArray[1].mHalfAuto,    133);
            THEOLIZER_EQUAL(mManualArray[0].mManual,        134);
            THEOLIZER_EQUAL(mManualArray[1].mManual,        135);
        }
        else if (gVersionList[gDataIndex].mVersionEnum < VersionEnum::ver3a)
        {
            THEOLIZER_EQUAL(mFullAutoArray[0].mFullAuto,    0);
            THEOLIZER_EQUAL(mFullAutoArray[1].mFullAuto,    0);
            THEOLIZER_EQUAL(mHalfAutoArray[0].mHalfAuto,    0);
            THEOLIZER_EQUAL(mHalfAutoArray[1].mHalfAuto,    0);
            THEOLIZER_EQUAL(mManualArray[0].mManual,        0);
            THEOLIZER_EQUAL(mManualArray[1].mManual,        0);
        }
        else
        {
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
        }
    }
    THEOLIZER_INTRUSIVE(CS, (ModifyClassName), 2);
};
#endif  // DISABLE_MODIFY_CLASS_TEST_NAME

// ***************************************************************************
//      変更テスト用クラス（順序対応）  メンバ変数追加
// ***************************************************************************

#ifndef DISABLE_MODIFY_CLASS_TEST_ORDER
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

    // 1b:追加
    ModifyHalfAutoY mHalfAutoYMember THEOLIZER_ANNOTATE(FS:mHalfAutoXMember);// 2a:名変更
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
        mInt(0),
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
        mInt(221),
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

        case VersionEnum::ver3a:
        case VersionEnum::ver3b:
        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }

        // --- 基本型メンバ変数 ---
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
        case VersionEnum::ver2a:
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

        case VersionEnum::ver3a:
        case VersionEnum::ver3b:
        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }
    THEOLIZER_INTRUSIVE_ORDER(CS, (ModifyClassOrder), 2);
};
#endif  // DISABLE_MODIFY_CLASS_TEST_ORDER

// ***************************************************************************
//      配列の要素数上限テスト
// ***************************************************************************

#ifndef DISABLE_MODIFY_CLASS_TEST_ARRAY
struct ArrayTest
{
    // --- 次元変更テスト用 ---
    static const unsigned   kSize1x=2;
    static const unsigned   kSize0x=3;
    static const unsigned   kSize2=2;
    static const unsigned   kSize1=3;
    static const unsigned   kSize0=2;
    unsigned    mArrayDim[kSize2][kSize1][kSize0];

    // --- サイズ上限テスト用 ---
    static const unsigned   kSize=kDefSize;
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
        for (unsigned i=0; i < kSize0; ++i)
        {
            for (unsigned j=0; j < kSize1; ++j)
            {
                for (unsigned k=0; k < kSize2; ++k)
                {
                    mArrayDim[i][j][k]=i*100+j*10+k;
                }
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
        for (unsigned i=0; i < kSize2; ++i)
        {
            for (unsigned j=0; j < kSize1; ++j)
            {
                for (unsigned k=0; k < kSize0; ++k)
                {
                    if ((gVersionList[gDataIndex].mVersionEnum == VersionEnum::ver2a)
                     && (gVersionList[gProgramIndex].mVersionEnum == VersionEnum::ver2a))
                    {
                        THEOLIZER_EQUAL(mArrayDim[i][j][k], i*100+j*10+k, i, j, k);
                    }
                    else if ((gVersionList[gDataIndex].mVersionEnum == VersionEnum::ver2a)
                          && (gVersionList[gProgramIndex].mVersionEnum == VersionEnum::ver3a))
                    {
                        if (i == 0)
                        {
                            THEOLIZER_EQUAL(mArrayDim[i][j][k], i*100+j*10+k, i, j, k);
                        }
                        else
                        {
                            THEOLIZER_EQUAL(mArrayDim[i][j][k], 0, i, j, k);
                        }
                    }
                    else if ((VersionEnum::ver1c <= gVersionList[gDataIndex].mVersionEnum)
                          && (gVersionList[gDataIndex].mVersionEnum <= VersionEnum::ver2a))
                    {
                        if ((i == 0) && (j < kSize1x) && (k < kSize0x))
                        {
                            THEOLIZER_EQUAL(mArrayDim[i][j][k], i*100+j*10+k, i, j, k);
                        }
                        else
                        {
                            THEOLIZER_EQUAL(mArrayDim[i][j][k], 0, i, j, k);
                        }
                    }
                    else if (gVersionList[gDataIndex].mVersionEnum < VersionEnum::ver1c)
                    {
                        THEOLIZER_EQUAL(mArrayDim[i][j][k], 0, i, j, k);
                    }
                    else
                    {
                        // FAILさせる
                        THEOLIZER_EQUAL(gDataIndex, gMyIndex, i, j, k);
                    }
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

    THEOLIZER_INTRUSIVE(CS, (ArrayTest), 2);
};
#endif  // DISABLE_MODIFY_CLASS_TEST_ARRAY

#endif  // TEST_MODIFY_CLASS_H
