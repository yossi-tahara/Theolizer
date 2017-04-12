//############################################################################
//      各serializerのテスト
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

#if !defined(THEOLIZER_TEST_SERIALIZER_BASE_H)
#define THEOLIZER_TEST_SERIALIZER_BASE_H

//#define NO_ARRAY

// ***************************************************************************
//      通常のインクルード
// ***************************************************************************

#include <theolizer/serializer_json.h>
#include <theolizer/serializer_binary.h>
using theolizer::u8string;

using namespace std;

//############################################################################
//      オプション指定
//############################################################################

// ***************************************************************************
//      保存先リスト定義
// ***************************************************************************

THEOLIZER_DESTINATIONS
(
    All,
    Machine,            // PC単位設定
    User,               // User単位設定
    Document,           // ドキュメントとしての保存
    SpecialDocument,    // ドキュメントとしての保存(特殊型)
    TestDocument        // ドキュメントとしての保存(テスト用)
);

// ***************************************************************************
//      グローバル・バージョン番号テーブル
// ***************************************************************************

THEOLIZER_DEFINE_GLOBAL_VERSION_TABLE(MyGVNT, 4);

//############################################################################
//      enum型テスト
//############################################################################

// ***************************************************************************
//      enum型(ターゲット)
// ***************************************************************************

enum class EnumTest : long
{
    zero,
    one,
    two
};

THEOLIZER_ENUM(EnumTest,2);

// ***************************************************************************
//      enum型(バージョン内名称変更テスト用)
// ***************************************************************************

namespace another
{

enum class EnumTest : long
{
    ZERO,
    ONE,
    TWO
};

}

//############################################################################
//      クラス保存のテスト
//
//      Global  Base    Derived
//      4       2       4(Base2削除)             mUShort2->mUShort名前変更)
//      3       2       3(mUInt追加,             mUShort1->mUShort2名前変更)
//      2       1       2(Base2追加, mUChar削除, mUShort0->mUShort1名前変更)
//      1       1       1(                       mUShort0用意)
//############################################################################

// ***************************************************************************
//      ユーザ・クラス定義(Base)
// ***************************************************************************

class IntrusiveBase
{
    THEOLIZER_INTRUSIVE(CS, (IntrusiveBase), 2);
    int         mInt;

public:
    short       mShort;
    string      mString;

    IntrusiveBase() :
                mInt(),
                mShort(),
                mString()
    { }

    void setInt(int iInt) {mInt=iInt;}
    int  getInt() {return mInt;}
};

//----------------------------------------------------------------------------
//      ユーザ定義
//----------------------------------------------------------------------------

//      ---<<< Version.1 >>>---

template<class tTheolizerVersion, class tNextVersion>
struct IntrusiveBase::TheolizerUserDefine<tTheolizerVersion, tNextVersion, 1>
{
    // 他のクラスで使用され、次バージョンで使用しなくなった場合に初期値を設定する
    static void initialize(tTheolizerVersion& oNowVersion)
    {
    }

    // 保存／回復の両方で新版から旧版へ変換する
    static void downVersion(tNextVersion const& iNextVersion, tTheolizerVersion& oNowVersion)
    {
        oNowVersion.mInt   = iNextVersion.mInt  +10000;
        oNowVersion.mShort = iNextVersion.mShort+10000;
        oNowVersion.mString= iNextVersion.mString+"-Appended";
    }

    // 回復時、旧版から新版へ変換する
    static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)
    {
        oNextVersion.mInt   = iNowVersion.mInt  -10000;
        oNextVersion.mShort = iNowVersion.mShort-10000;
        oNextVersion.mString= iNowVersion.mString.substr(0, iNowVersion.mString.size()-9);
    }
};

// ***************************************************************************
//      ユーザ・クラス定義(Base2)
// ***************************************************************************

class IntrusiveBase2
{
    THEOLIZER_INTRUSIVE_ORDER(CS, (IntrusiveBase2),1);

public:
    long                mLong;
    long long           mLongLong;
    unsigned long       mULong;
    unsigned long long  mULongLong;

    IntrusiveBase2() :
                mLong(),
                mLongLong(),
                mULong(),
                mULongLong()
    { }
};

// ***************************************************************************
//      ユーザ・クラス定義(Derived)
// ***************************************************************************

class IntrusiveDerived : public IntrusiveBase
{
    THEOLIZER_INTRUSIVE(CS, (IntrusiveDerived), 4);

public:
    unsigned int    mUInt;
    unsigned short  mUShort;
    IntrusiveBase   mIntrusiveBase;
    EnumTest        mEnum;

    IntrusiveDerived() :
                IntrusiveBase(),
                mUInt(),
                mUShort(),
                mIntrusiveBase(),
                mEnum(EnumTest::zero)
    { }
};

//----------------------------------------------------------------------------
//      ユーザ定義
//----------------------------------------------------------------------------

//      ---<<< Version.2 >>>---

template<class tTheolizerVersion, class tNextVersion>
struct IntrusiveDerived::TheolizerUserDefine<tTheolizerVersion, tNextVersion, 2>
{
    // 他のクラスで使用され、次バージョンで使用しなくなった場合に初期値を設定する
    static void initialize(tTheolizerVersion& oNowVersion)
    {
    }

    // 保存／回復の両方で新版から旧版へ変換する
    static void downVersion(tNextVersion const& iNextVersion, tTheolizerVersion& oNowVersion)
    {
        oNowVersion.mUShort1 = iNextVersion.mUShort2+1;
    }

    // 回復時、旧版から新版へ変換する
    static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)
    {
    }
};

//      ---<<< Version.1 >>>---

template<class tTheolizerVersion, class tNextVersion>
struct IntrusiveDerived::TheolizerUserDefine<tTheolizerVersion, tNextVersion, 1>
{
    // 他のクラスで使用され、次バージョンで使用しなくなった場合に初期値を設定する
    static void initialize(tTheolizerVersion& oNowVersion)
    {
    }

    // 保存／回復の両方で新版から旧版へ変換する
    static void downVersion(tNextVersion const& iNextVersion, tTheolizerVersion& oNowVersion)
    {
        oNowVersion.mUShort0 = iNextVersion.mUShort1+1;
    }

    // 回復時、旧版から新版へ変換する
    static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)
    {
    }
};

// ***************************************************************************
//      完全自動型
// ***************************************************************************

class FullAuto
{
protected:
    int    mInt;
    short  mShort;

public:
    FullAuto() : mInt(), mShort()
    { }

    FullAuto(int iInt, short iShort) : mInt(iInt), mShort(iShort)
    { }

    void  setInt(int iInt) {mInt=iInt;}
    int   getInt()         {return mInt;}
    void  setShort(short iShort) {mShort=iShort;}
    short getShort()             {return mShort;}
};

// ***************************************************************************
//      完全自動型テンプレート
// ***************************************************************************

template<typename tType0, typename tType1>
class FullAutoTemplate
{
protected:
    tType0  mType0;
    tType1  mType1;

public:
    FullAutoTemplate() : mType0(), mType1()
    { }

    void  setType0(tType0 iType0) {mType0=iType0;}
    int   getType0()              {return mType0;}
    void  setType1(tType1 iType1) {mType1=iType1;}
    int   getType1()              {return mType1;}
};

//############################################################################
//      同じバージョン内変更テスト
//############################################################################

// ***************************************************************************
//      同じバージョン内変更テストの保存側クラス
//          型チェックがない派生Serializerだから可能なテスト。
//          型チェックがある場合、同じクラス名で異なるクラス定義が必要。
// ***************************************************************************

class DerivedChange : public IntrusiveBase2, public IntrusiveBase
{
    THEOLIZER_INTRUSIVE(CS, (DerivedChange),1);

public:
    IntrusiveBase2  mIntrusiveBase2;
    IntrusiveBase   mIntrusiveBase;
    unsigned short  mUShort;
    unsigned int    mUInt;

    DerivedChange() :
                IntrusiveBase2(),
                IntrusiveBase(),
                mIntrusiveBase2(),
                mIntrusiveBase(),
                mUShort(),
                mUInt()
    { }
};

// ***************************************************************************
//      同じバージョン内変更テストの回復側クラス(追加／順序変更)
// ***************************************************************************

namespace change_order
{
namespace dummy
{

class DerivedChange : public IntrusiveBase,public IntrusiveBase2
{
    THEOLIZER_INTRUSIVE(CS, (DerivedChange), 1);

public:
    unsigned int    mUInt;
    unsigned short  mUShort;
    IntrusiveBase   mIntrusiveBase;
    unsigned char   mUChar;             // 追加
    IntrusiveBase2  mIntrusiveBase2;

    DerivedChange() :
                IntrusiveBase(),
                IntrusiveBase2(),
                mUInt(),
                mUShort(),
                mIntrusiveBase(),
                mUChar(),
                mIntrusiveBase2()
    { }
};

}
}

// ***************************************************************************
//      同じバージョン内変更テストの回復側クラス(削除)
// ***************************************************************************

class DerivedDelete : public IntrusiveBase
{
    THEOLIZER_INTRUSIVE(CS, (DerivedDelete), 1);

public:
    unsigned int    mUInt;
    IntrusiveBase2  mIntrusiveBase2;

    DerivedDelete() :
                IntrusiveBase(),
                mUInt(),
                mIntrusiveBase2()
    { }
};

//############################################################################
//      ヘッダ・テスト
//          保存先毎のTypeInfoList登録テスト
//############################################################################

// ***************************************************************************
//      基底／包含クラス1
// ***************************************************************************

class Included1
{
    THEOLIZER_INTRUSIVE(CS, (Included1), 1);

public:
    int     mInt;
    Included1() : mInt(0) { }
    Included1(int iInt) : mInt(iInt) { }
};

// ***************************************************************************
//      基底／包含クラス2
// ***************************************************************************

class Included2
{
    THEOLIZER_INTRUSIVE(CS, (Included2), 1);

public:
    short   mShort;
    Included2() : mShort(0) { }
};

// ***************************************************************************
//      対象クラス
// ***************************************************************************

class DestinationTest
{
    THEOLIZER_INTRUSIVE(CS, (DestinationTest), 1);

public:
    Included1   mIncluded1;
    Included2   mIncluded2;
    FullAutoTemplate<int, short>    mFullAutoTemplate;

    DestinationTest() : 
        mIncluded1(),
        mIncluded2(),
        mFullAutoTemplate()
    { }
};

//############################################################################
//      オブジェクト追跡指定
//############################################################################

// ***************************************************************************
//      静的定義領域追跡テスト用クラス
// ***************************************************************************

class Tracking : public Included1
{
    THEOLIZER_INTRUSIVE(CS, (Tracking), 1);

public:
    unsigned        mUInt;
    EnumTest        mEnum;
    Included2       mIncluded2;
    string          mString;
    unsigned        mArray[2][3];
    Tracking() : 
        mUInt(0),
        mEnum(EnumTest::zero),
        mArray{}
    { }
};

// ***************************************************************************
//      ポインタ回復テスト用クラス
// ***************************************************************************

class Pointers
{
    THEOLIZER_INTRUSIVE(CS, (Pointers), 1);

public:
    unsigned*       mUInt;
    EnumTest*       mEnum;
    Included2*      mIncluded2;
    string*         mString;

    int                                     (*mArrayInt)[2][3];
    FullAutoTemplate<int[2], short (*)[3]>  mFullAutoTemplateArray;

    Pointers() : 
        mUInt(nullptr),
        mEnum(nullptr),
        mIncluded2(nullptr),
        mString(nullptr),
        mArrayInt(nullptr),
        mFullAutoTemplateArray()
    { }
};

// ***************************************************************************
//      動的生成領域追跡テスト用クラス
// ***************************************************************************

class Dynamic
{
    THEOLIZER_INTRUSIVE(CS, (Dynamic), 1);

public:
    unsigned*       mUInt;
    EnumTest*       mEnum;
    Included1*      mIncluded1;
    string*         mString;
    int*            mArray[3];
    int             (*mArrayPtr)[2];

    Dynamic() : mUInt(nullptr), mEnum(nullptr), mIncluded1(nullptr),
                mString(nullptr), mArray{nullptr}, mArrayPtr(nullptr)
    { }

    ~Dynamic()
    {
        delete mUInt;
        delete mEnum;
        delete mIncluded1;
        delete mString;
        for (std::size_t i=0; i < std::extent<decltype(mArray)>::value; ++i)
            delete mArray[i];
        delete[] mArrayPtr;
    }
};

//############################################################################
//      生配列を含むクラスのテスト
//############################################################################

// ***************************************************************************
//      テスト用クラス
// ***************************************************************************

class ArrayClass
{
    THEOLIZER_INTRUSIVE(CS, (ArrayClass), 2);

public:
    short           mArrayInc[2][3][4];         // [2]→[2][3][4]
    unsigned        mArrayDec[2];               // [2][3][4]→[2]
    IntrusiveBase   mClassArrayInc[2][3][4];
    IntrusiveBase   mClassArrayDec[2];
    EnumTest        mEnumArrayInc[2][3][4];
    EnumTest        mEnumArrayDec[2];
    ArrayClass() : 
        mArrayInc{},
        mArrayDec{},
        mClassArrayInc{},
        mClassArrayDec{},
        mEnumArrayInc{},
        mEnumArrayDec{}
    { }
};

//----------------------------------------------------------------------------
//      ユーザ定義
//----------------------------------------------------------------------------

//      ---<<< Version.1 >>>---

template<class tTheolizerVersion, class tNextVersion>
struct ArrayClass::TheolizerUserDefine<tTheolizerVersion, tNextVersion, 1>
{
    // 他のクラスで使用され、次バージョンで使用しなくなった場合に初期値を設定する
    static void initialize(tTheolizerVersion& oNowVersion)
    {
    }

    // 保存／回復の両方で新版から旧版へ変換する
    static void downVersion(tNextVersion const& iNextVersion, tTheolizerVersion& oNowVersion)
    {
    }

    // 回復時、旧版から新版へ変換する
    static void upVersion(tTheolizerVersion const& iNowVersion, tNextVersion& oNextVersion)
    {
#ifndef NO_ARRAY
        for (std::size_t i=0; i < oNextVersion.mArrayInc.size(); ++i)
        {
            for (std::size_t j=0; j < oNextVersion.mArrayInc[i].size(); ++j)
            {
                for (std::size_t k=0; k < oNextVersion.mArrayInc[i][j].size(); ++k)
                {
                    if ((i==0)&&(j==0)&&(k<2))
                    {
                    }
                    else
                    {
std::cout << "upVersion() : mArrayInc[" << i << "][" << j << "][" << k << "]="
          << oNextVersion.mArrayInc[i][j][k] << "\n";
                        oNextVersion.mArrayInc[i][j][k]=99;
                    }
                }
            }
        }

        for (std::size_t i=0; i < oNextVersion.mClassArrayInc.size(); ++i)
        {
            for (std::size_t j=0; j < oNextVersion.mClassArrayInc[i].size(); ++j)
            {
                for (std::size_t k=0; k < oNextVersion.mClassArrayInc[i][j].size(); ++k)
                {
                    if ((i==0)&&(j==0)&&(k<2))
                    {
                    }
                    else
                    {
std::cout << "upVersion() : mClassArrayInc[" << i << "][" << j << "][" << k << "]="
          << oNextVersion.mClassArrayInc[i][j][k].mShort << "\n";
                        oNextVersion.mClassArrayInc[i][j][k].mShort.
                            set(999, iNowVersion.mClassArrayInc[0].mShort.getDoSucceed());
                    }
                }
            }
        }

        for (std::size_t i=0; i < oNextVersion.mEnumArrayInc.size(); ++i)
        {
            for (std::size_t j=0; j < oNextVersion.mEnumArrayInc[i].size(); ++j)
            {
                for (std::size_t k=0; k < oNextVersion.mEnumArrayInc[i][j].size(); ++k)
                {
                    if ((i==0)&&(j==0)&&(k<2))
                    {
                    }
                    else
                    {
std::cout << "upVersion() : mEnumArrayInc[" << i << "][" << j << "][" << k << "]="
          << oNextVersion.mEnumArrayInc[i][j][k].getUnderlyingType() << "\n";
                        oNextVersion.mEnumArrayInc[i][j][k].
                            set(1, iNowVersion.mEnumArrayInc[0].getDoSucceed());
                    }
                }
            }
        }
#endif
    }
};

// ***************************************************************************
//      生配列を含むクラス・テンプレート
// ***************************************************************************

template<std::size_t tDim0, std::size_t tDim1>
struct ArrayClassTemplate
{
    int         mIntArray[tDim0*2][tDim1*2];

    ArrayClassTemplate() : mIntArray{}
    {}
    THEOLIZER_INTRUSIVE_TEMPLATE_ORDER(CS, (template<std::size_t tDim0, std::size_t tDim1>),
        (ArrayClassTemplate<tDim0, tDim1>), 1, ArrayClassTemplatePrimary);
};

// ***************************************************************************
//      非侵入型(基底クラス)
// ***************************************************************************

//----------------------------------------------------------------------------
//      ターゲット
//          自動生成より前が多いだろう
//----------------------------------------------------------------------------

struct NonIntrusiveBase
{
    long    mLong;
    NonIntrusiveBase() : mLong() { }
};

//----------------------------------------------------------------------------
//      ユーザによるシリアライズ指定
//----------------------------------------------------------------------------

THEOLIZER_NON_INTRUSIVE_ORDER((NonIntrusiveBase), 1);

//----------------------------------------------------------------------------
//      ユーザ定義
//----------------------------------------------------------------------------

//      ---<<< Version.1 >>>---

template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<NonIntrusiveBase>::
    TheolizerUserDefine<tBaseSerializer, tTheolizerVersion, 1>
{
    // 保存
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        THEOLIZER_PROCESS(iSerializer, iInstance->mLong);
    }

    // 回復
    static void loadClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        // もし、nullptrなら、インスタンス生成
        if (!oInstance)   oInstance=new typename tTheolizerVersion::TheolizerTarget();

        THEOLIZER_PROCESS(iSerializer, oInstance->mLong);
    }
};

// ***************************************************************************
//      非侵入型(派生クラス)
// ***************************************************************************

//----------------------------------------------------------------------------
//      ターゲット
//----------------------------------------------------------------------------

struct NonIntrusiveDrived : public NonIntrusiveBase
{
    int mInt;
    NonIntrusiveDrived() : NonIntrusiveBase(), mInt() { }
};

//----------------------------------------------------------------------------
//      ユーザによるシリアライズ指定
//----------------------------------------------------------------------------

THEOLIZER_NON_INTRUSIVE_ORDER((NonIntrusiveDrived), 1);

//----------------------------------------------------------------------------
//      ユーザ定義
//----------------------------------------------------------------------------

//      ---<<< Version.0 >>>---

template<class tBaseSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<NonIntrusiveDrived>::
    TheolizerUserDefine<tBaseSerializer, tTheolizerVersion, 1>
{
    // 保存
    static void saveClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        THEOLIZER_PROCESS_BASE(iSerializer, NonIntrusiveBase, iInstance);
        THEOLIZER_PROCESS(iSerializer, iInstance->mInt);
    }

    // 回復
    static void loadClassManual
    (
        tBaseSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        // もし、nullptrなら、インスタンス生成
        if (!oInstance)   oInstance=new typename tTheolizerVersion::TheolizerTarget();

        THEOLIZER_PROCESS_BASE(iSerializer, NonIntrusiveBase, oInstance);
        THEOLIZER_PROCESS(iSerializer, oInstance->mInt);
    }
};

// ***************************************************************************
//      侵入型(派生クラス)
// ***************************************************************************

//----------------------------------------------------------------------------
//      ターゲット
//----------------------------------------------------------------------------

struct IntrusiveDerived2 : public NonIntrusiveBase
{
    short mShort;
    IntrusiveDerived2() : NonIntrusiveBase(), mShort() { }

    THEOLIZER_INTRUSIVE(CS, (IntrusiveDerived2), 1);
};

#endif
