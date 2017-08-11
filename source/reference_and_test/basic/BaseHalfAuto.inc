//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          msvc 2017対策用
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

#define   BaseHalfAuto    \
  THEOLIZER_INTERNAL_CAT(THEOLIZER_INTERNAL_CAT(BaseHalfAuto_, tInitialValue),_)

class BaseHalfAuto
{
    static const int kIntPrivate  =tInitialValue;
    static const int kIntProtected=tInitialValue+1;
    static const int kIntPublic   =tInitialValue+2;

    int     mIntPrivate;

protected:
    int     mIntProtected;

public:
    int     mIntPublic;

    // デフォルト・コンストラクタ
    BaseHalfAuto() : mIntPrivate(0), mIntProtected(0), mIntPublic(0)
    { }

    // コンストラクタ
    //  (デフォルト・コンストラクタは意図的に無し)
    BaseHalfAuto(bool iIsValued) :
        mIntPrivate(  (iIsValued)?kIntPrivate:0),
        mIntProtected((iIsValued)?kIntProtected:0),
        mIntPublic(   (iIsValued)?kIntPublic:0)
    { }

    // 一致チェック
    void checkPrivate(bool iIsValued=false)
    {
        THEOLIZER_EQUAL(mIntPrivate, ((!iIsValued)?0:kIntPrivate));
    }
    void checkProtected(bool iIsValued=false)
    {
        THEOLIZER_EQUAL(mIntProtected, ((!iIsValued)?0:kIntProtected));
    }
    void checkPublic(bool iIsValued=false)
    {
        THEOLIZER_EQUAL(mIntPublic, ((!iIsValued)?0:kIntPublic));
    }

    // 侵入型半自動 指定
    THEOLIZER_INTRUSIVE(CS, (BaseHalfAuto), 1);
};

#undef  BaseHalfAuto
#undef  tInitialValue
