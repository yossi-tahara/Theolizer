//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          STLサポート
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

#if !defined(TEST_SUPPORT_STL_H)
#define TEST_SUPPORT_STL_H

// スマート・ポインタ
#include <theolizer/memory.h>

// 標準コンテナ
#include <theolizer/array.h>
#include <theolizer/vector.h>
#include <theolizer/deque.h>
#include <theolizer/list.h>

// ***************************************************************************
//      使い方の説明
// ***************************************************************************

//----------------------------------------------------------------------------
//      基底クラス
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//      派生クラス
//----------------------------------------------------------------------------

//############################################################################
//      網羅テスト
//############################################################################

// ***************************************************************************
//      スマート・ポインタのシリアライズ対象クラス
// ***************************************************************************

//----------------------------------------------------------------------------
//      基底クラス
//----------------------------------------------------------------------------

class SmartBase
{
protected:
    int     mBaseData;
public:
    SmartBase(int iBaseData=0) : mBaseData(iBaseData) { }
    virtual ~SmartBase() { }
    virtual void check(int iValue)=0;
};

//----------------------------------------------------------------------------
//      派生クラス
//----------------------------------------------------------------------------

class SmartDerived : public SmartBase
{
    int     mDerivedData;
public:
    SmartDerived(int iValue=0) : SmartBase{iValue}, mDerivedData{iValue+1} { }
    void check(int iValue)
    {
        THEOLIZER_EQUAL(mBaseData, iValue);
        THEOLIZER_EQUAL(mDerivedData, iValue+1);
    }
    THEOLIZER_INTRUSIVE(CS, (SmartDerived), 1);
};
THEOLIZER_REGISTER_CLASS((SmartDerived));

// ***************************************************************************
//      スマート・ポインタのシリアライズ処理クラス
//          自動と手動（非トップ・レベル）のテスト用
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動（FullAuto)と手動(トップ・レベル)
//----------------------------------------------------------------------------

struct SmartTestAuto
{
    std::unique_ptr<SmartBase>  mUniquePtr;
    std::shared_ptr<SmartBase>  mSharedPtr;
    std::weak_ptr<SmartBase>    mWeakPtr;

    SmartTestAuto() { }
    SmartTestAuto
    (
        int iUnique,
        std::shared_ptr<SmartBase>& iShared,
        std::shared_ptr<SmartBase>& iForWeak
    ) : mUniquePtr(std::unique_ptr<SmartBase>((iUnique)?new SmartDerived{iUnique}:nullptr)),
        mSharedPtr(std::shared_ptr<SmartBase>(iShared)),
        mWeakPtr(std::weak_ptr<SmartBase>(iForWeak))
    { }

    // iUnique == 0の時は全てnullptrと比較する
    // それ以外の時は、
    //  unique_ptrは型と値チェック
    //  shared_ptrは型とポインタ値チェック
    //  weak_ptrは型とポインタ値チェック
    void check
    (
        int iUnique,
        std::shared_ptr<SmartBase>& iShared,
        std::shared_ptr<SmartBase>& iForWeak
    )
    {
        if (iUnique == 0)
        {
            THEOLIZER_EQUAL_PTR(mUniquePtr.get(), nullptr);
            THEOLIZER_EQUAL_PTR(mSharedPtr.get(), nullptr);
            THEOLIZER_EQUAL_PTR(mWeakPtr.lock().get(), nullptr);
        }
        else
        {
            THEOLIZER_EQUAL(typeid(*mUniquePtr.get()), typeid(SmartDerived));
            THEOLIZER_EQUAL(typeid(*mSharedPtr.get()), typeid(SmartDerived));
            THEOLIZER_EQUAL(typeid(*mWeakPtr.lock().get()), typeid(SmartDerived));
            mUniquePtr->check(iUnique);
            THEOLIZER_EQUAL_PTR(mSharedPtr.get(), iShared.get());
            THEOLIZER_EQUAL_PTR(mWeakPtr.lock().get(), iForWeak.get());
        }
    }
};

//      ---<<< 手動(トップ・レベル)による保存／回復 >>>---

template<class tSerializer>
inline void processSmart(tSerializer& iSerializer, SmartTestAuto& iSmartTestAuto)
{
    THEOLIZER_PROCESS(iSerializer, iSmartTestAuto.mUniquePtr);
    THEOLIZER_PROCESS(iSerializer, iSmartTestAuto.mSharedPtr);
    THEOLIZER_PROCESS(iSerializer, iSmartTestAuto.mWeakPtr);
}

//----------------------------------------------------------------------------
//      手動(非トップ・レベル)
//----------------------------------------------------------------------------

struct SmartTestManual
{
    std::unique_ptr<SmartBase>  mUniquePtr;
    std::shared_ptr<SmartBase>  mSharedPtr;
    std::weak_ptr<SmartBase>    mWeakPtr;

    SmartTestManual() { }
    SmartTestManual
    (
        int iUnique,
        std::shared_ptr<SmartBase>& iShared,
        std::shared_ptr<SmartBase>& iForWeak
    ) : mUniquePtr(std::unique_ptr<SmartBase>((iUnique)?new SmartDerived{iUnique}:nullptr)),
        mSharedPtr(std::shared_ptr<SmartBase>(iShared)),
        mWeakPtr(std::weak_ptr<SmartBase>(iForWeak))
    { }

    // iUnique == 0の時は全てnullptrと比較する
    // それ以外の時は、
    //  unique_ptrは型と値チェック
    //  shared_ptrは型とポインタ値チェック
    //  weak_ptrは型とポインタ値チェック
    void check
    (
        int iUnique,
        std::shared_ptr<SmartBase>& iShared,
        std::shared_ptr<SmartBase>& iForWeak
    )
    {
        if (iUnique == 0)
        {
            THEOLIZER_EQUAL_PTR(mUniquePtr.get(), nullptr);
            THEOLIZER_EQUAL_PTR(mSharedPtr.get(), nullptr);
            THEOLIZER_EQUAL_PTR(mWeakPtr.lock().get(), nullptr);
        }
        else
        {
            THEOLIZER_EQUAL(typeid(*mUniquePtr.get()), typeid(SmartDerived));
            THEOLIZER_EQUAL(typeid(*mSharedPtr.get()), typeid(SmartDerived));
            THEOLIZER_EQUAL(typeid(*mWeakPtr.lock().get()), typeid(SmartDerived));
            mUniquePtr->check(iUnique);
            THEOLIZER_EQUAL_PTR(mSharedPtr.get(), iShared.get());
            THEOLIZER_EQUAL_PTR(mWeakPtr.lock().get(), iForWeak.get());
        }
    }
};

THEOLIZER_NON_INTRUSIVE_ORDER((SmartTestManual), 1);

template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<SmartTestManual>::
    TheolizerUserDefine<tBaseSerializer, tTheolizerVersion, 1>
{
    // Save members.
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        THEOLIZER_PROCESS(iSerializer, iInstance->mUniquePtr);
        THEOLIZER_PROCESS(iSerializer, iInstance->mSharedPtr);
        THEOLIZER_PROCESS(iSerializer, iInstance->mWeakPtr);
    }

    // Load members.
    static void loadClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        if (!oInstance) oInstance=new typename tTheolizerVersion::TheolizerTarget();

        THEOLIZER_PROCESS(iSerializer, oInstance->mUniquePtr);
        THEOLIZER_PROCESS(iSerializer, oInstance->mSharedPtr);
        THEOLIZER_PROCESS(iSerializer, oInstance->mWeakPtr);
    }
};

// ***************************************************************************
//      標準コンテナのシリアライズ対象クラス
// ***************************************************************************

class TestStl
{
    int     mData;
public:
    TestStl(int iData=0) : mData(iData) { }
    TestStl operator+(int iRhs) const
    {
        return TestStl(mData+iRhs);
    }
    bool operator==(TestStl const& iRhs)
    {
        return mData == iRhs.mData;
    }
    friend std::ostream& operator<<(std::ostream& os, TestStl const& iRhs)
    {
        os << iRhs.mData;
        return os;
    }

    THEOLIZER_INTRUSIVE(CS, (TestStl), 1);
};

#endif  // TEST_SUPPORT_STL_H
