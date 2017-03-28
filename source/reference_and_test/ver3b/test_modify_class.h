﻿//############################################################################
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
//      変更テスト用クラス（名前対応）
// ***************************************************************************

struct ChangedModifyClassName
{
    unsigned    mUnsigned;  // 順序変更
//  short       mShort;     // 削除
    int         mIntChanged THEOLIZER_ANNOTATE(FS:mInt);    // 変数名変更
    long        mLong;      // 追加

    ChangedModifyClassName()     : mUnsigned()   , mIntChanged(0)  , mLong()    { }
    ChangedModifyClassName(bool) : mUnsigned(120), mIntChanged(110), mLong(130) { }
    void check()
    {
//      THEOLIZER_EQUAL(mShort,      100);
        THEOLIZER_EQUAL(mIntChanged, 110);
        THEOLIZER_EQUAL(mUnsigned,   120);
        switch(gVersionList[gDataIndex].mVersionEnum)
        {
        case VersionEnum::ver1a:
            THEOLIZER_EQUAL(mLong,  0);
            break;

        case VersionEnum::ver1b:
        case VersionEnum::ver1c:
        case VersionEnum::ver2a:
        case VersionEnum::ver3a:
        case VersionEnum::ver3b:
            THEOLIZER_EQUAL(mLong,  130);
            break;

        default:
            // FAILさせる
            THEOLIZER_EQUAL(gDataIndex, gMyIndex);
            break;
        }
    }
    THEOLIZER_INTRUSIVE(CS, (ChangedModifyClassName), 3);
};

// ***************************************************************************
//      変更テスト用クラス（順序対応）  メンバ変数追加
// ***************************************************************************

struct ChangedModifyClassOrder
{
    short       mShort;
    int         mIntChanged THEOLIZER_ANNOTATE(FS:mInt);    // 変数名変更
    unsigned    mUnsigned;
//  long        mLong;      // 追加→バージョンアップ→削除

    ChangedModifyClassOrder()     : mShort(0)  , mIntChanged(0)  , mUnsigned()    { }
    ChangedModifyClassOrder(bool) : mShort(200), mIntChanged(210), mUnsigned(220) { }
    void check()
    {
        THEOLIZER_EQUAL(mShort,      200);
        THEOLIZER_EQUAL(mIntChanged, 210);
        THEOLIZER_EQUAL(mUnsigned,   220);
    }
    THEOLIZER_INTRUSIVE_ORDER(CS, (ChangedModifyClassOrder), 3);
};

// ***************************************************************************
//      配列の要素数上限テスト
// ***************************************************************************

struct ArraySizeTest
{
    static const unsigned   kSize=kVer3Size;
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
    void check(bool isValued=false, unsigned iSize=kSize)
    {
        for (unsigned i=0; i < kSize; ++i)
        {
            if (i < iSize)
            {
                THEOLIZER_EQUAL(mArray1D[i], ((isValued)?i:0), i);
            }
            else
            {
                THEOLIZER_EQUAL(mArray1D[i], 0, i);
            }
            for (unsigned j=0; j < kSize; ++j)
            {
                if ((i < iSize) && (j < iSize))
                {
                    THEOLIZER_EQUAL(mArray2D[i][j], ((isValued)?(i*1000+j):0), i, j);
                }
                else
                {
                    THEOLIZER_EQUAL(mArray2D[i][j], 0, i, j);
                }
                for (unsigned k=0; k < kSize; ++k)
                {
                    if ((i < iSize) && (j < iSize) && (k < iSize))
                    {
                        THEOLIZER_EQUAL(mArray3D[i][j][k], ((isValued)?((i*1000+j)*1000+k):0),
                            i, j, k);
                    }
                    else
                    {
                        THEOLIZER_EQUAL(mArray3D[i][j][k], 0, i, j, k);
                    }
                }
            }
        }
    }

    THEOLIZER_INTRUSIVE(CS, (ArraySizeTest), 2);
};

//----------------------------------------------------------------------------
//      
//----------------------------------------------------------------------------

#endif  // TEST_MODIFY_CLASS_H
