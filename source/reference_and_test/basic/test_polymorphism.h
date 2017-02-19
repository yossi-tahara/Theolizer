//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          ポリモーフィズム
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

#if !defined(TEST_POLYMORPHISM_H)
#define TEST_POLYMORPHISM_H

// ***************************************************************************
//      使い方の説明
// ***************************************************************************

//----------------------------------------------------------------------------
//      基底クラス
//----------------------------------------------------------------------------

struct PolyBase
{
    int     mInt;
    PolyBase(int iInt) : mInt(iInt) { }
    virtual ~PolyBase() { }
    virtual void check()=0;
};

//----------------------------------------------------------------------------
//      派生クラス
//----------------------------------------------------------------------------

struct PolyDerived0 : public PolyBase
{
    short   mShort;
    PolyDerived0() : PolyBase(0), mShort(0) { }
    PolyDerived0(bool) : PolyBase(100), mShort(200) { }
    void check()
    {
        THEOLIZER_EQUAL(mInt,   100);
        THEOLIZER_EQUAL(mShort, 200);
    }
};
THEOLIZER_REGISTER_CLASS((PolyDerived0));

struct PolyDerived1 : public PolyBase
{
    std::string mString;
    PolyDerived1() : PolyBase(0), mString("") { }
    PolyDerived1(bool) : PolyBase(1000), mString("string") { }
    void check()
    {
        THEOLIZER_EQUAL(mInt,    1000);
        THEOLIZER_EQUAL(mString, "string");
    }
};
THEOLIZER_REGISTER_CLASS((PolyDerived1));

//############################################################################
//      網羅テスト
//############################################################################

// ***************************************************************************
//      基底クラス定義
// ***************************************************************************

//----------------------------------------------------------------------------
//      非侵入型完全自動
//----------------------------------------------------------------------------

struct PolyBaseFullAuto
{
    int     mBaseData;
    PolyBaseFullAuto(int iBaseData=0) : mBaseData(iBaseData) { }
    virtual ~PolyBaseFullAuto() { }
    virtual void check(bool iIsNonDefault)=0;
    void check(int iBaseData)
    {
        THEOLIZER_EQUAL(mBaseData, iBaseData);
    }
};

//----------------------------------------------------------------------------
//      侵入型半自動
//----------------------------------------------------------------------------

struct PolyBaseHalfAuto
{
    unsigned    mBaseData;
    PolyBaseHalfAuto(unsigned iBaseData=0) : mBaseData(iBaseData) { }
    virtual ~PolyBaseHalfAuto() { }
    virtual void check(bool iIsNonDefault)=0;
    void check(unsigned iBaseData)
    {
        THEOLIZER_EQUAL(mBaseData, iBaseData);
    }

    THEOLIZER_INTRUSIVE(CS, (PolyBaseHalfAuto), 1);
};

//----------------------------------------------------------------------------
//      非侵入型手動
//----------------------------------------------------------------------------

struct PolyBaseManual
{
    std::string mBaseData;
    PolyBaseManual(char const* iBaseData="") : mBaseData(iBaseData) { }
    virtual ~PolyBaseManual() { }
    virtual void check(bool) { }    // 手動型は抽象クラス非サポート
    void check(char const* iBaseData)
    {
        THEOLIZER_EQUAL(mBaseData, iBaseData);
    }
};

THEOLIZER_NON_INTRUSIVE_ORDER((PolyBaseManual), 1);

template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<PolyBaseManual>::
    TheolizerUserDefine<tBaseSerializer, tTheolizerVersion, 1>
{
    // Save members.
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        THEOLIZER_PROCESS(iSerializer, iInstance->mBaseData);
    }

    // Load members.
    static void loadClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        if (!oInstance) oInstance=new typename tTheolizerVersion::TheolizerTarget();

        THEOLIZER_PROCESS(iSerializer, oInstance->mBaseData);
    }
};

// ***************************************************************************
//      派生クラス定義
// ***************************************************************************

//----------------------------------------------------------------------------
//      非侵入型完全自動
//----------------------------------------------------------------------------

struct PolyDerivedFullAuto :
    public PolyBaseFullAuto,
    public PolyBaseHalfAuto,
    public PolyBaseManual
{
    int     mDerivedData;
    PolyDerivedFullAuto() :
        PolyBaseFullAuto{},
        PolyBaseHalfAuto{},
        PolyBaseManual{},
        mDerivedData{0}
    { }
    PolyDerivedFullAuto(bool) :
        PolyBaseFullAuto{-100},
        PolyBaseHalfAuto{100},
        PolyBaseManual{"String100"},
        mDerivedData{1000}
    { }
    void check(bool iIsNonDefault)
    {
        PolyBaseFullAuto::check((iIsNonDefault)?-100:0);
        PolyBaseHalfAuto::check((iIsNonDefault)?100u:0u);
        PolyBaseManual::check((iIsNonDefault)?"String100":"");
        if (!iIsNonDefault)
            THEOLIZER_EQUAL(mDerivedData, 0);
        else 
            THEOLIZER_EQUAL(mDerivedData, 1000);
    }
};

THEOLIZER_REGISTER_CLASS((PolyDerivedFullAuto));

//----------------------------------------------------------------------------
//      侵入型半自動
//----------------------------------------------------------------------------

struct PolyDerivedHalfAuto :
    public PolyBaseHalfAuto,
    public PolyBaseManual,
    public PolyBaseFullAuto
{
    int     mDerivedData;
    PolyDerivedHalfAuto() :
        PolyBaseHalfAuto{},
        PolyBaseManual{},
        PolyBaseFullAuto{},
        mDerivedData{0}
    { }
    PolyDerivedHalfAuto(bool) :
        PolyBaseHalfAuto{200},
        PolyBaseManual{"String200"},
        PolyBaseFullAuto{-200},
        mDerivedData{2000}
    { }
    void check(bool iIsNonDefault)
    {
        PolyBaseFullAuto::check((iIsNonDefault)?-200:0);
        PolyBaseHalfAuto::check((iIsNonDefault)?200u:0u);
        PolyBaseManual::check((iIsNonDefault)?"String200":"");
        if (!iIsNonDefault)
            THEOLIZER_EQUAL(mDerivedData, 0);
        else 
            THEOLIZER_EQUAL(mDerivedData, 2000);
    }

    THEOLIZER_INTRUSIVE(CS, (PolyDerivedHalfAuto), 1);
};

THEOLIZER_REGISTER_CLASS((PolyDerivedHalfAuto));

//----------------------------------------------------------------------------
//      非侵入型手動
//----------------------------------------------------------------------------

struct PolyDerivedManual :
    public PolyBaseManual,
    public PolyBaseFullAuto,
    public PolyBaseHalfAuto
{
    int     mDerivedData;
    PolyDerivedManual() :
        PolyBaseManual{},
        PolyBaseFullAuto{},
        PolyBaseHalfAuto{},
        mDerivedData{0}
    { }
    PolyDerivedManual(bool) :
        PolyBaseManual{"String300"},
        PolyBaseFullAuto{-300},
        PolyBaseHalfAuto{300},
        mDerivedData{3000}
    { }
    void check(bool iIsNonDefault)
    {
        PolyBaseFullAuto::check((iIsNonDefault)?-300:0);
        PolyBaseHalfAuto::check((iIsNonDefault)?300u:0u);
        PolyBaseManual::check((iIsNonDefault)?"String300":"");
        if (!iIsNonDefault)
            THEOLIZER_EQUAL(mDerivedData, 0);
        else 
            THEOLIZER_EQUAL(mDerivedData, 3000);
    }
};

THEOLIZER_NON_INTRUSIVE_ORDER((PolyDerivedManual), 1);

template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<PolyDerivedManual>::
    TheolizerUserDefine<tBaseSerializer, tTheolizerVersion, 1>
{
    // Save members.
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        THEOLIZER_PROCESS(iSerializer, static_cast<PolyBaseFullAuto const&>(*iInstance));
        THEOLIZER_PROCESS(iSerializer, static_cast<PolyBaseHalfAuto const&>(*iInstance));
        THEOLIZER_PROCESS(iSerializer, static_cast<PolyBaseManual const&>(*iInstance));
        THEOLIZER_PROCESS(iSerializer, iInstance->mDerivedData);
    }

    // Load members.
    static void loadClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        if (!oInstance) oInstance=new typename tTheolizerVersion::TheolizerTarget();

        THEOLIZER_PROCESS(iSerializer, static_cast<PolyBaseFullAuto&>(*oInstance));
        THEOLIZER_PROCESS(iSerializer, static_cast<PolyBaseHalfAuto&>(*oInstance));
        THEOLIZER_PROCESS(iSerializer, static_cast<PolyBaseManual&>(*oInstance));
        THEOLIZER_PROCESS(iSerializer, oInstance->mDerivedData);
    }
};

THEOLIZER_REGISTER_CLASS((PolyDerivedManual));

#endif  // TEST_POLYMORPHISM_H
