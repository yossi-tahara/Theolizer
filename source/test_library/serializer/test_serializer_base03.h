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

#if !defined(THEOLIZER_TEST_SERIALIZER_BASE03_H)
#define THEOLIZER_TEST_SERIALIZER_BASE03_H

// ***************************************************************************
//      通常のインクルード
// ***************************************************************************

using namespace std;

#include <theolizer/serializer_json.h>
using theolizer::u8string;

//############################################################################
//      ポリモーフィズムのテスト
//############################################################################

// ***************************************************************************
//      基底クラス定義(BaseClass)
// ***************************************************************************

struct BaseClass
{
    int         mInt;
    BaseClass() : mInt(0) { }
    BaseClass(int iInt) : mInt(iInt) { }
    virtual ~BaseClass() {}
    THEOLIZER_INTRUSIVE(CS, (BaseClass),1);
};

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE THEOLIZER_INTERNAL_UNPAREN(BaseClass)

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_CLASS_NAME((u8"BaseClass"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mInt),mInt,etmDefault,\
        (theolizerD::All),\
        (int))
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

// ***************************************************************************
//      派生クラス定義(DerivedClass)
// ***************************************************************************

struct DerivedClass : public BaseClass
{
    unsigned    mUInt;
    THEOLIZER_INTRUSIVE(CS, (DerivedClass), 1);
};

THEOLIZER_REGISTER_CLASS((DerivedClass));

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE THEOLIZER_INTERNAL_UNPAREN(DerivedClass)

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_CLASS_NAME((u8"DerivedClass"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_BASE_LIST()\
    THEOLIZER_INTERNAL_BASE_KI(public,etmDefault,0,(BaseClass),1,u8"BaseClass")
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mUInt),mUInt,etmDefault,\
        (theolizerD::All),\
        (unsigned int))
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

// ***************************************************************************
//      更なる派生クラス定義(Derived2Class)
// ***************************************************************************

struct Derived2Class : public DerivedClass
{
    long        mLong;
    THEOLIZER_INTRUSIVE(CS, (Derived2Class), 1);
};

THEOLIZER_REGISTER_CLASS((Derived2Class));

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE THEOLIZER_INTERNAL_UNPAREN(Derived2Class)

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_CLASS_NAME((u8"Derived2Class"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_BASE_LIST()\
    THEOLIZER_INTERNAL_BASE_KI(public,etmDefault,0,(DerivedClass),1,u8"DerivedClass")
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mLong),mLong,etmDefault,\
        (theolizerD::All),\
        (long))
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

//############################################################################
//      namespace内クラス
//############################################################################

#if 1
#define NAMESPACE_TEST

namespace user
{

// ***************************************************************************
//      基底クラス定義(NamespaceClass)
// ***************************************************************************

struct NamespaceClass
{
    unsigned    mUInt;
    virtual ~NamespaceClass() {}
    THEOLIZER_INTRUSIVE(CS, (NamespaceClass), 1);
};

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE THEOLIZER_INTERNAL_UNPAREN(user::NamespaceClass)

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_CLASS_NAME((u8"user::NamespaceClass"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mUInt),mUInt,etmDefault,\
        (theolizerD::All),\
        (unsigned int))
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

}   // namespace user

#endif

//############################################################################
//      参照メンバのテスト
//############################################################################

#if 1
#define REFERENCE_TEST

// ***************************************************************************
//      参照クラス定義(ReferenceClass)
// ***************************************************************************

struct ReferenceClass
{
    unsigned&   mUInt;
    BaseClass&  mBaseClass;
    int         (&mIntArray)[2];

    ReferenceClass
    (
        unsigned&   iUInt,
        BaseClass&  iBaseClass,
        int         (&iIntArray)[2]
    ) : mUInt(iUInt),
        mBaseClass(iBaseClass),
        mIntArray(iIntArray)
    {
    }

    THEOLIZER_INTRUSIVE(CS, (ReferenceClass), 1);
};

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE THEOLIZER_INTERNAL_UNPAREN(ReferenceClass)

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_CLASS_NAME((u8"ReferenceClass"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mUInt),mUInt,etmDefault,\
        (theolizerD::All),\
        (unsigned int &))\
    THEOLIZER_INTERNAL_ELEMENT_N((mBaseClass),mBaseClass,etmDefault,\
        (theolizerD::All),\
        (BaseClass &))\
    THEOLIZER_INTERNAL_ELEMENT_N((mIntArray),mIntArray,etmDefault,\
        (theolizerD::All),\
        (int (&)[2]))
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

#endif

//############################################################################
//      グローバル・バージョン番号テーブル用
//############################################################################

THEOLIZER_GENERATED_GLOBAL_TABLE();

#endif
