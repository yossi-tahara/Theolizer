//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          テスト基本部
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

// ***************************************************************************
//      インクルード
// ***************************************************************************

// 標準ライブラリ
#include <iostream>
#include <fstream>
#include <string>

// theolizerライブラリ
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_json.h>

// プロジェクト・ヘッダ
#include "common.h"
#include "test_top_level.h"

// 自動生成ソース
#include "test_top_level.cpp.theolizer.hpp"

//############################################################################
//      プリミティブ、enum型
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

template<class tSerializer>
void savePrimitiveAndEnum(tSerializer& iSerializer)
{

//      ---<<< Primitive >>>---

    THEOLIZER_PROCESS(iSerializer, -100);

    char    aChar=-101;
    THEOLIZER_PROCESS(iSerializer, aChar);

    short   aShort=-102;
    THEOLIZER_PROCESS(iSerializer, aShort);

    int aInt=-103;
    THEOLIZER_PROCESS(iSerializer, aInt);

    long     aLong=-104;
    THEOLIZER_PROCESS(iSerializer, aLong);

    long long   aLongLong=-105;
    THEOLIZER_PROCESS(iSerializer, aLongLong);

    unsigned char   aUChar=101;
    THEOLIZER_PROCESS(iSerializer, aUChar);

    unsigned short  aUShort=102;
    THEOLIZER_PROCESS(iSerializer, aUShort);

    unsigned int    aUInt=103;
    THEOLIZER_PROCESS(iSerializer, aUInt);

    unsigned long   aULong=104;
    THEOLIZER_PROCESS(iSerializer, aULong);

    unsigned long long  aULongLong=105;
    THEOLIZER_PROCESS(iSerializer, aULongLong);

    float       aFloat=1.23456F;
    THEOLIZER_PROCESS(iSerializer, aFloat);

    double      aDouble=1.23456789012345;
    THEOLIZER_PROCESS(iSerializer, aDouble);

    long double aLongDouble=1.23456789012345L;
    THEOLIZER_PROCESS(iSerializer, aLongDouble);

    std::string aString=u8"ＵＴＦ－８";
    THEOLIZER_PROCESS(iSerializer, aString);

    std::wstring aWstring=L"ＵＴＦ－１６／３２";
    THEOLIZER_PROCESS(iSerializer, aWstring);

    std::u16string aU16string=u"ＵＴＦ－１６";
    THEOLIZER_PROCESS(iSerializer, aU16string);

    std::u32string aU32string=U"ＵＴＦ－３２";
    THEOLIZER_PROCESS(iSerializer, aU32string);

//      ---<<< enum型 >>>---

    THEOLIZER_PROCESS(iSerializer, EnumTest::ONE);

    EnumTest aEnumTest=EnumTest::TWO;
    THEOLIZER_PROCESS(iSerializer, aEnumTest);
}

// ***************************************************************************
//      回復
// ***************************************************************************

template<class tSerializer>
void loadPrimitiveAndEnum(tSerializer& iSerializer)
{

//      ---<<< Primitive >>>---

    int aTemp;
    THEOLIZER_PROCESS(iSerializer, aTemp);
    THEOLIZER_EQUAL(aTemp, -100);

    char    aChar;
    THEOLIZER_PROCESS(iSerializer, aChar);
    THEOLIZER_EQUAL(aChar, -101);

    short   aShort;
    THEOLIZER_PROCESS(iSerializer, aShort);
    THEOLIZER_EQUAL(aShort, -102);

    int aInt;
    THEOLIZER_PROCESS(iSerializer, aInt);
    THEOLIZER_EQUAL(aInt, -103);

    long    aLong;
    THEOLIZER_PROCESS(iSerializer, aLong);
    THEOLIZER_EQUAL(aLong, -104);

    long long   aLongLong;
    THEOLIZER_PROCESS(iSerializer, aLongLong);
    THEOLIZER_EQUAL(aLongLong, -105);

    unsigned char   aUChar;
    THEOLIZER_PROCESS(iSerializer, aUChar);
    THEOLIZER_EQUAL(aUChar, 101);

    unsigned short  aUShort;
    THEOLIZER_PROCESS(iSerializer, aUShort);
    THEOLIZER_EQUAL(aUShort, 102);

    unsigned int    aUInt;
    THEOLIZER_PROCESS(iSerializer, aUInt);
    THEOLIZER_EQUAL(aUInt, 103);

    unsigned long   aULong;
    THEOLIZER_PROCESS(iSerializer, aULong);
    THEOLIZER_EQUAL(aULong, 104);

    unsigned long long  aULongLong;
    THEOLIZER_PROCESS(iSerializer, aULongLong);
    THEOLIZER_EQUAL(aULongLong, 105);

    float       aFloat;
    THEOLIZER_PROCESS(iSerializer, aFloat);
    THEOLIZER_EQUAL(aFloat, 1.23456F);

    double      aDouble;
    THEOLIZER_PROCESS(iSerializer, aDouble);
    THEOLIZER_EQUAL(aDouble, 1.23456789012345);

    long double aLongDouble;
    THEOLIZER_PROCESS(iSerializer, aLongDouble);
    THEOLIZER_EQUAL(aLongDouble, 1.23456789012345L);

    std::string aString;
    THEOLIZER_PROCESS(iSerializer, aString);
    THEOLIZER_EQUAL(aString, u8"ＵＴＦ－８");

    std::wstring aWstring;
    THEOLIZER_PROCESS(iSerializer, aWstring);
    THEOLIZER_EQUAL(aWstring, L"ＵＴＦ－１６／３２");

    std::u16string aU16string;
    THEOLIZER_PROCESS(iSerializer, aU16string);
    THEOLIZER_EQUAL(aU16string, u"ＵＴＦ－１６");

    std::u32string aU32string;
    THEOLIZER_PROCESS(iSerializer, aU32string);
    THEOLIZER_EQUAL(aU32string, U"ＵＴＦ－３２");

//      ---<<< enum型 >>>---

    EnumTest aEnumTest=EnumTest::ZERO;
    THEOLIZER_PROCESS(iSerializer, aEnumTest);
    THEOLIZER_EQUAL(aEnumTest, EnumTest::ONE);

    THEOLIZER_PROCESS(iSerializer, aEnumTest);
    THEOLIZER_EQUAL(aEnumTest, EnumTest::TWO);
}

//############################################################################
//      テスト群呼び出し中継
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

template<class tSerializer>
void saveTopLevel(tSerializer& iSerializer)
{
    savePrimitiveAndEnum(iSerializer);
}

INSTANTIATION_ALL(void, saveTopLevel);

// ***************************************************************************
//      回復
// ***************************************************************************

template<class tSerializer>
void loadTopLevel(tSerializer& iSerializer)
{
    loadPrimitiveAndEnum(iSerializer);
}

INSTANTIATION_ALL(void, loadTopLevel);
