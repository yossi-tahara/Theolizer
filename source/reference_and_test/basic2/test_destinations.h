//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          保存先
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

#if !defined(TEST_DESTINATIONS_H)
#define TEST_DESTINATIONS_H


// ***************************************************************************
//      使い方の説明
// ***************************************************************************

//----------------------------------------------------------------------------
//      ポリモーフィズムによる保存／回復デモ
//----------------------------------------------------------------------------

//![DestinationPoly]

//      ---<<< 基底クラス >>>---

struct BaseCustomer
{
    std::string mName   THEOLIZER_ANNOTATE(FS:<theolizerD::Master>);    // 名前

    BaseCustomer() { }
    BaseCustomer(std::string const& iName) :
        mName(iName)
    { }
    virtual ~BaseCustomer() { }
    THEOLIZER_INTRUSIVE(CS, (BaseCustomer), 1);
};

//      ---<<< 派生クラス１ >>>---

struct CorporateCustomer : public BaseCustomer
{
    int mCapitalStock   THEOLIZER_ANNOTATE(FS:<theolizerD::Master>);    // 資本金
    int mTradeAccounts  THEOLIZER_ANNOTATE(FS:<theolizerD::Trade>);     // 売掛金

    CorporateCustomer() : mCapitalStock(0), mTradeAccounts(0) { }
    CorporateCustomer(std::string const& iName, int iCapitalStock, int iTradeAccounts) :
        BaseCustomer(iName),
        mCapitalStock(iCapitalStock),
        mTradeAccounts(iTradeAccounts)
    { }
    THEOLIZER_INTRUSIVE(CS, (CorporateCustomer), 1);
};
THEOLIZER_REGISTER_CLASS((CorporateCustomer));

//      ---<<< 派生クラス２ >>>---

struct IndividualCustomer : public BaseCustomer
{
    std::string mBirthday   THEOLIZER_ANNOTATE(FS:<theolizerD::Master>);// 誕生日
    int         mPoint      THEOLIZER_ANNOTATE(FS:<theolizerD::Trade>); // ポイント数

    IndividualCustomer() : mPoint(0) { }
    IndividualCustomer(std::string const& iName, std::string iBirthday, int iPoint) :
        BaseCustomer(iName),
        mBirthday(iBirthday),
        mPoint(iPoint)
    { }
    THEOLIZER_INTRUSIVE(CS, (IndividualCustomer), 1);
};
THEOLIZER_REGISTER_CLASS((IndividualCustomer));

//![DestinationPoly]

//----------------------------------------------------------------------------
//      子クラスに保存先指定する際の注意点
//----------------------------------------------------------------------------

//![DestinationChild/Parent]

//      ---<<< 子クラス >>>---

struct DestinationChild
{
    int     mAnnotateA      THEOLIZER_ANNOTATE(FS:<theolizerD::DestA>);
    int     mAnnotateB      THEOLIZER_ANNOTATE(FS:<theolizerD::DestB>);

    DestinationChild(int iValueA, int iValueB) :
        mAnnotateA(iValueA),
        mAnnotateB(iValueB)
    { }
};

//      ---<<< 親クラス >>>---

struct DestinationParent
{
    int                 mAnnotateA          THEOLIZER_ANNOTATE(FS:<theolizerD::DestA>);
    int                 mAnnotateB          THEOLIZER_ANNOTATE(FS:<theolizerD::DestB>);
    DestinationChild    mDestinationChild   THEOLIZER_ANNOTATE(FS:<theolizerD::DestA>);

    DestinationParent(int iValue=0) :
        mAnnotateA(1*iValue),
        mAnnotateB(2*iValue),
        mDestinationChild(3*iValue, 4*iValue)
    { }
};

//![DestinationChild/Parent]

// ***************************************************************************
//      保存の有無指定
//          完全自動型も指定可能ではあるが、意味はないので非サポートとする。
//          有用な使い方が見つかったらサポートする方向で検討する。
// ***************************************************************************

//----------------------------------------------------------------------------
//      デフォルト保存クラス
//----------------------------------------------------------------------------

//![DefaultSave]

class DefaultSave
{
    int     mNoAnnotate;
    int     mAnnotateSave       THEOLIZER_ANNOTATE(FS);
    int     mAnnotateNonSave    THEOLIZER_ANNOTATE(FN);
public:
    DefaultSave() : mNoAnnotate(0), mAnnotateSave(0), mAnnotateNonSave(0)
    { }
    DefaultSave(bool) : mNoAnnotate(100), mAnnotateSave(200), mAnnotateNonSave(300)
    { }
    void check()
    {
        THEOLIZER_EQUAL(mNoAnnotate, 100);
        THEOLIZER_EQUAL(mAnnotateSave, 200);
        THEOLIZER_EQUAL(mAnnotateNonSave, 0);
    }

    // 侵入型半自動 指定
    THEOLIZER_INTRUSIVE(CS, (DefaultSave), 1);
};

//![DefaultSave]

//----------------------------------------------------------------------------
//      デフォルト非保存クラス
//----------------------------------------------------------------------------

//![DefaultNonSave]

class DefaultNonSave
{
    int     mNoAnnotate;
    int     mAnnotateSave       THEOLIZER_ANNOTATE(FS);
    int     mAnnotateNonSave    THEOLIZER_ANNOTATE(FN);
public:
    DefaultNonSave() : mNoAnnotate(0), mAnnotateSave(0), mAnnotateNonSave(0)
    { }
    DefaultNonSave(bool) : mNoAnnotate(1000), mAnnotateSave(2000), mAnnotateNonSave(3000)
    { }
    void check()
    {
        THEOLIZER_EQUAL(mNoAnnotate, 0);
        THEOLIZER_EQUAL(mAnnotateSave, 2000);
        THEOLIZER_EQUAL(mAnnotateNonSave, 0);
    }

    // 侵入型半自動 指定
    THEOLIZER_INTRUSIVE(CN, (DefaultNonSave), 1);
};

//![DefaultNonSave]

// ***************************************************************************
//      保存先指定
//          完全自動型も指定可能ではあるが、意味はないので非サポートとする。
//          有用な使い方が見つかったらサポートする方向で検討する。
// ***************************************************************************

//----------------------------------------------------------------------------
//      組み合わせテスト
//----------------------------------------------------------------------------

//      ---<<< 子クラス >>>---

class DestinationTestChild
{
    int     mNoAnnotate;
    int     mAnnotateA      THEOLIZER_ANNOTATE(FS:<theolizerD::DestA>);
    int     mAnnotateB      THEOLIZER_ANNOTATE(FS:<theolizerD::DestB>);
    int     mAnnotateAB     THEOLIZER_ANNOTATE(FS:<theolizerD::DestA, theolizerD::DestB>);
    int     mAnnotateC      THEOLIZER_ANNOTATE(FS:<theolizerD::DestC>);
    int     mAnnotateN      THEOLIZER_ANNOTATE(FN);
public:
    DestinationTestChild(int iValue=0) :
        mNoAnnotate(1*iValue),
        mAnnotateA(2*iValue),
        mAnnotateB(3*iValue),
        mAnnotateAB(4*iValue),
        mAnnotateC(5*iValue),
        mAnnotateN(6*iValue)
    { }
    void check(theolizer::internal::Destinations const& iDestinations, int iValue)
    {
        THEOLIZER_EQUAL(mNoAnnotate, 1*iValue);

        if (iDestinations.isMatch({theolizerD::DestA}))
        {
            THEOLIZER_EQUAL(mAnnotateA, 2*iValue);
        }
        else
        {
            THEOLIZER_EQUAL(mAnnotateA, 0);
        }

        if (iDestinations.isMatch({theolizerD::DestB}))
        {
            THEOLIZER_EQUAL(mAnnotateB, 3*iValue);
        }
        else
        {
            THEOLIZER_EQUAL(mAnnotateB, 0);
        }

        if (iDestinations.isMatch({theolizerD::DestA, theolizerD::DestB}))
        {
            THEOLIZER_EQUAL(mAnnotateAB, 4*iValue);
        }
        else
        {
            THEOLIZER_EQUAL(mAnnotateAB, 0);
        }

        if (iDestinations.isMatch({theolizerD::DestC}))
        {
            THEOLIZER_EQUAL(mAnnotateC, 5*iValue);
        }
        else
        {
            THEOLIZER_EQUAL(mAnnotateC, 0);
        }
        THEOLIZER_EQUAL(mAnnotateN, 0);
    }

    // 侵入型半自動 指定
    THEOLIZER_INTRUSIVE(CS, (DestinationTestChild), 1);
};

//      ---<<< 親クラス >>>---

class DestinationTestParent : public DestinationTestChild
{
    DestinationTestChild mNoAnnotate;
    DestinationTestChild mAnnotateA  THEOLIZER_ANNOTATE(FS:<theolizerD::DestA>);
    DestinationTestChild mAnnotateB  THEOLIZER_ANNOTATE(FS:<theolizerD::DestB>);
    DestinationTestChild mAnnotateAB THEOLIZER_ANNOTATE(FS:<theolizerD::DestA, theolizerD::DestB>);
    DestinationTestChild mAnnotateC  THEOLIZER_ANNOTATE(FS:<theolizerD::DestC>);
    DestinationTestChild mAnnotateN  THEOLIZER_ANNOTATE(FN);
public:
    DestinationTestParent() :
        DestinationTestChild(0),
        mNoAnnotate(0),
        mAnnotateA(0),
        mAnnotateB(0),
        mAnnotateAB(0),
        mAnnotateC(0),
        mAnnotateN(0)
    { }
    DestinationTestParent(bool) :
        DestinationTestChild(1),
        mNoAnnotate(10),
        mAnnotateA(100),
        mAnnotateB(1000),
        mAnnotateAB(10000),
        mAnnotateC(100000),
        mAnnotateN(1000000)
    { }
    void check(theolizer::internal::Destinations const& iDestinations)
    {
        DestinationTestChild::check(iDestinations, 1);
        mNoAnnotate.check(iDestinations, 10);

        if (iDestinations.isMatch({theolizerD::DestA}))
        {
            mAnnotateA.check(iDestinations, 100);
        }
        else
        {
            mAnnotateA.check(iDestinations, 0);
        }

        if (iDestinations.isMatch({theolizerD::DestB}))
        {
            mAnnotateB.check(iDestinations, 1000);
        }
        else
        {
            mAnnotateB.check(iDestinations, 0);
        }

        if (iDestinations.isMatch({theolizerD::DestA, theolizerD::DestB}))
        {
            mAnnotateAB.check(iDestinations, 10000);
        }
        else
        {
            mAnnotateAB.check(iDestinations, 0);
        }

        if (iDestinations.isMatch({theolizerD::DestC}))
        {
            mAnnotateC.check(iDestinations, 100000);
        }
        else
        {
            mAnnotateC.check(iDestinations, 0);
        }
        mAnnotateN.check(iDestinations, 0);
    }
    // DestA→DestB合成時のテスト
    void check()
    {
        // 保存先指定がないものは全て処理する
        DestinationTestChild::check({theolizerD::DestA, theolizerD::DestB}, 1);
        mNoAnnotate.check({theolizerD::DestA, theolizerD::DestB}, 10);

        mAnnotateA.check({theolizerD::DestA}, 100);
        mAnnotateB.check({theolizerD::DestB}, 1000);
        mAnnotateAB.check({theolizerD::DestA, theolizerD::DestB}, 10000);
        mAnnotateC.check({theolizerD::DestC}, 0);
    }

    // 侵入型半自動 指定
    THEOLIZER_INTRUSIVE(CS, (DestinationTestParent), 1);
};


#endif  // TEST_DESTINATIONS_H
