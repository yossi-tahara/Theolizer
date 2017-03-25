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
//      配列の要素数上限テスト
// ***************************************************************************

struct ArraySizeTest
{
    static const unsigned   kSize=kVer3Size;
    unsigned    mArray1D[kSize];
    unsigned    mArray2D[kSize][kSize];
    unsigned    mArray3D[kSize][kSize][kSize];

    ArraySizeTest() : mArray1D{}, mArray2D{}
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
