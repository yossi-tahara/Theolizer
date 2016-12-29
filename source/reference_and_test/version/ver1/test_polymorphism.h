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
//      
//----------------------------------------------------------------------------

// ***************************************************************************
//      基底クラス定義
// ***************************************************************************

//----------------------------------------------------------------------------
//      非侵入型完全自動
//----------------------------------------------------------------------------

struct PolyBaseFullAuto
{
    int     mBaseData;
    PolyBaseFullAuto() : mBaseData(0) { }
    PolyBaseFullAuto(bool) : mBaseData(100) { }
    virtual ~PolyBaseFullAuto() { }
    virtual void check(bool iIsNonDefault)
    {
        if (!iIsNonDefault)
            THEOLIZER_EQUAL(mBaseData, 0);
        else 
            THEOLIZER_EQUAL(mBaseData, 100);
    }
};

//----------------------------------------------------------------------------
//      侵入型半自動
//----------------------------------------------------------------------------

struct PolyBaseHalfAuto
{
    int     mBaseData;
    PolyBaseHalfAuto() : mBaseData(0) { }
    PolyBaseHalfAuto(bool) : mBaseData(200) { }
    virtual ~PolyBaseHalfAuto() { }
    virtual void check(bool iIsNonDefault)
    {
        if (!iIsNonDefault)
            THEOLIZER_EQUAL(mBaseData, 0);
        else 
            THEOLIZER_EQUAL(mBaseData, 200);
    }

    THEOLIZER_INTRUSIVE(CS, (PolyBaseHalfAuto), 1);
};

//----------------------------------------------------------------------------
//      非侵入型手動
//----------------------------------------------------------------------------

struct PolyBaseManual
{
    int     mBaseData;
    PolyBaseManual() : mBaseData(0) { }
    PolyBaseManual(bool) : mBaseData(300) { }
    virtual ~PolyBaseManual() { }
    virtual void check(bool iIsNonDefault)
    {
        if (!iIsNonDefault)
            THEOLIZER_EQUAL(mBaseData, 0);
        else 
            THEOLIZER_EQUAL(mBaseData, 300);
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
        PolyBaseFullAuto{true},
        PolyBaseHalfAuto{true},
        PolyBaseManual{true},
        mDerivedData{1000}
    { }
    void check(bool iIsNonDefault)
    {
        PolyBaseFullAuto::check(iIsNonDefault);
        PolyBaseHalfAuto::check(iIsNonDefault);
        PolyBaseManual::check(iIsNonDefault);
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
    public PolyBaseFullAuto,
    public PolyBaseHalfAuto,
    public PolyBaseManual
{
    int     mDerivedData;
    PolyDerivedHalfAuto() :
        PolyBaseFullAuto{},
        PolyBaseHalfAuto{},
        PolyBaseManual{},
        mDerivedData{0}
    { }
    PolyDerivedHalfAuto(bool) :
        PolyBaseFullAuto{true},
        PolyBaseHalfAuto{true},
        PolyBaseManual{true},
        mDerivedData{2000}
    { }
    void check(bool iIsNonDefault)
    {
        PolyBaseFullAuto::check(iIsNonDefault);
        PolyBaseHalfAuto::check(iIsNonDefault);
        PolyBaseManual::check(iIsNonDefault);
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
    public PolyBaseFullAuto,
    public PolyBaseHalfAuto,
    public PolyBaseManual
{
    int     mDerivedData;
    PolyDerivedManual() :
        PolyBaseFullAuto{},
        PolyBaseHalfAuto{},
        PolyBaseManual{},
        mDerivedData{0}
    { }
    PolyDerivedManual(bool) :
        PolyBaseFullAuto{true},
        PolyBaseHalfAuto{true},
        PolyBaseManual{true},
        mDerivedData{3000}
    { }
    void check(bool iIsNonDefault)
    {
        PolyBaseFullAuto::check(iIsNonDefault);
        PolyBaseHalfAuto::check(iIsNonDefault);
        PolyBaseManual::check(iIsNonDefault);
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
