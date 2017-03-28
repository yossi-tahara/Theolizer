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
//      変更テスト用クラス（名前対応）
// ***************************************************************************

struct ModifyClassName
{
    short       mShort;
    int         mInt;
    unsigned    mUnsigned;

    ModifyClassName()     : mShort(0)  , mInt(0)  , mUnsigned()    { }
    ModifyClassName(bool) : mShort(100), mInt(110), mUnsigned(120) { }
    void check()
    {
        THEOLIZER_EQUAL(mShort,    100);
        THEOLIZER_EQUAL(mInt,      110);
        THEOLIZER_EQUAL(mUnsigned, 120);
    }
//  THEOLIZER_INTRUSIVE(CS, (ModifyClassName), 1);
};

// ***************************************************************************
//      変更テスト用クラス（順序対応）
// ***************************************************************************

struct ModifyClassOrder
{
    short       mShort;
    int         mInt;
    unsigned    mUnsigned;

    ModifyClassOrder()     : mShort(0)  , mInt(0)  , mUnsigned()    { }
    ModifyClassOrder(bool) : mShort(200), mInt(210), mUnsigned(220) { }
    void check()
    {
        THEOLIZER_EQUAL(mShort,    200);
        THEOLIZER_EQUAL(mInt,      210);
        THEOLIZER_EQUAL(mUnsigned, 220);
    }
    THEOLIZER_INTRUSIVE_ORDER(CS, (ModifyClassOrder), 1);
};

// ***************************************************************************
//      配列の要素数上限テスト
// ***************************************************************************

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
    void check(bool isValued=false)
    {
        for (unsigned i=0; i < kSize; ++i)
        {
            THEOLIZER_EQUAL(mArray1D[i], ((isValued)?i:0), i);
            for (unsigned j=0; j < kSize; ++j)
            {
                THEOLIZER_EQUAL(mArray2D[i][j], ((isValued)?(i*1000+j):0), i, j);
                for (unsigned k=0; k < kSize; ++k)
                {
                    THEOLIZER_EQUAL(mArray3D[i][j][k], ((isValued)?((i*1000+j)*1000+k):0),
                        i, j, k);
                }
            }
        }
    }

    THEOLIZER_INTRUSIVE(CS, (ArraySizeTest), 1);
};

//----------------------------------------------------------------------------
//      
//----------------------------------------------------------------------------

#endif  // TEST_MODIFY_CLASS_H
