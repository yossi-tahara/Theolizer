//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          THEOLIZER_PROCESS()の基本的な使い方
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

#if !defined(TEST_BASIC_PROCESS_H)
#define TEST_BASIC_PROCESS_H

#include <string>

// ***************************************************************************
//      enum型(完全自動型)
// ***************************************************************************

enum NormalEnum
{
    eneZero,
    eneOne,
    eneTwo
};

enum class ScopedEnum : long
{
    ZERO,
    ONE,
    TWO
};

// ***************************************************************************
//      クラス(完全自動型)
// ***************************************************************************

struct ClassBasicTest
{
    int         mInt;
    std::string mString;
    NormalEnum  mNormalEnum;

    ClassBasicTest() : mInt(0), mString(""), mNormalEnum(eneZero)
    { }

    // 以下は自動テスト（配列）用の定義
    ClassBasicTest(int iInt, char const* iString, NormalEnum iNormalEnum) :
        mInt(iInt),
        mString(iString),
        mNormalEnum(iNormalEnum)
    { }

    bool operator==(ClassBasicTest const& iRhs) const
    {
        return (mInt==iRhs.mInt) && (mString==iRhs.mString) && (mNormalEnum==iRhs.mNormalEnum);
    }

    friend std::ostream& operator<<(std::ostream& iOStream, ClassBasicTest const& iRhs)
    {
        iOStream <<"("<< iRhs.mInt << ", \"" << iRhs.mString << "\", " << iRhs.mNormalEnum<<")";
        return iOStream;
    }
};

#endif  // TEST_BASIC_PROCESS_H
