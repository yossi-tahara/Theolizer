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

#if !defined(THEOLIZER_TEST_SERIALIZER_BASE_AUTO_GEN_H)
#define THEOLIZER_TEST_SERIALIZER_BASE_AUTO_GEN_H

//############################################################################
//      enum型テスト
//############################################################################

// ***************************************************************************
//      enum型(ターゲット)
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,2)
#define THEOLIZER_GENERATED_ENUM_TYPE EnumTest

//      ---<<< Version.2 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,2)
#define THEOLIZER_GENERATED_ENUM_NAME u8"EnumTest"
#define THEOLIZER_GENERATED_SAVE_TYPE estName
#define THEOLIZER_GENERATED_BASE_TYPE long
#define THEOLIZER_GENERATED_ENUM_LIST()\
    THEOLIZER_GENERATED_ELEMENT((u8"zero",u8"ZERO"),(0,20),(10))\
    THEOLIZER_GENERATED_ELEMENT((u8"one",u8"ONE"),(1,21),(11))\
    THEOLIZER_GENERATED_ELEMENT((u8"two",u8"TWO"),(2,22),(12))
#define THEOLIZER_GENERATED_DEFAULT_VALUE 0
#include <theolizer/internal/version_enum.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_ENUM_NAME u8"EnumTest"
#define THEOLIZER_GENERATED_SAVE_TYPE estValue
#define THEOLIZER_GENERATED_BASE_TYPE long
#define THEOLIZER_GENERATED_ENUM_LIST()\
    THEOLIZER_GENERATED_ELEMENT((u8"A"),(10),(10))\
    THEOLIZER_GENERATED_ELEMENT((u8"B"),(11),(11))\
    THEOLIZER_GENERATED_ELEMENT((u8"C"),(12),(12))
#define THEOLIZER_GENERATED_DEFAULT_VALUE 10
#include <theolizer/internal/version_enum.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

// ***************************************************************************
//      enum型(バージョン内名称変更テスト用)
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_FULL_AUTO another::EnumTest

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_ENUM_NAME u8"another::EnumTest"
#define THEOLIZER_GENERATED_SAVE_TYPE estName
#define THEOLIZER_GENERATED_BASE_TYPE long
#define THEOLIZER_GENERATED_ENUM_LIST()\
    THEOLIZER_GENERATED_ELEMENT((u8"ZERO"),(0),(0))\
    THEOLIZER_GENERATED_ELEMENT((u8"ONE"),(1),(1))\
    THEOLIZER_GENERATED_ELEMENT((u8"TWO"),(2),(2))
#define THEOLIZER_GENERATED_DEFAULT_VALUE 0
#include <theolizer/internal/version_enum.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

//############################################################################
//      クラス保存のテスト
//############################################################################

// ***************************************************************************
//      ユーザ・クラス定義(Base)
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,2)
#define THEOLIZER_GENERATED_CLASS_TYPE IntrusiveBase

//      ---<<< Version.2 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,2)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_CLASS_NAME((u8"IntrusiveBase"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mInt),mInt,etmDefault,\
        (theolizerD::Machine),\
        (int))\
    THEOLIZER_INTERNAL_ELEMENT_N((mShort),mShort,etmDefault,\
        (theolizerD::Machine),\
        (short))\
    THEOLIZER_INTERNAL_ELEMENT_N((mString),mString,etmDefault,\
        (theolizerD::User),\
        (std::string))
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_CLASS_NAME((u8"IntrusiveBase"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emOrder
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mInt),mInt,etmDefault,\
        (theolizerD::All),\
        (int))\
    THEOLIZER_INTERNAL_ELEMENT_N((mShort),mShort,etmDefault,\
        (theolizerD::All),\
        (short))\
    THEOLIZER_INTERNAL_ELEMENT_N((mString),mString,etmDefault,\
        (theolizerD::All),\
        (std::string))\
    THEOLIZER_INTERNAL_ELEMENT_N_DEL((mChar),,etmDefault,\
        (theolizerD::All),\
        (char))
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

// ***************************************************************************
//      ユーザ・クラス定義(Base2)
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE IntrusiveBase2

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME() THEOLIZER_INTERNAL_CLASS_NAME((u8"IntrusiveBase2"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emOrder
#define THEOLIZER_GENERATED_ELEMENT_LIST() \
    THEOLIZER_INTERNAL_ELEMENT_N((mLong),mLong,etmDefault,\
        (theolizerD::Machine,theolizerD::Document),\
        (long))\
    THEOLIZER_INTERNAL_ELEMENT_N((mLongLong),mLongLong,etmDefault,\
        (theolizerD::Machine,theolizerD::Document),\
        (long long))\
    THEOLIZER_INTERNAL_ELEMENT_N((mULong),mULong,etmDefault,\
        (theolizerD::User,theolizerD::Document),\
        (unsigned long))\
    THEOLIZER_INTERNAL_ELEMENT_N((mULongLong),mULongLong,etmDefault,\
        (theolizerD::User,theolizerD::Document),\
        (unsigned long long))
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

// ***************************************************************************
//      ユーザ・クラス定義(Derived)
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,4)
#define THEOLIZER_GENERATED_CLASS_TYPE IntrusiveDerived

//      ---<<< Version.4 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,4)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_CLASS_NAME((u8"IntrusiveDerived"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_BASE_LIST()\
    THEOLIZER_INTERNAL_BASE_KI(public,etmDefault,0,(IntrusiveBase),2)
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mUInt),mUInt,etmDefault,\
        (theolizerD::All),\
        (unsigned int))\
    THEOLIZER_INTERNAL_ELEMENT_N((mUShort),mUShort,etmDefault,\
        (theolizerD::All),\
        (unsigned short))\
    THEOLIZER_INTERNAL_ELEMENT_KI((mIntrusiveBase),mIntrusiveBase,etmDefault,\
        (theolizerD::All),\
        (IntrusiveBase),2)\
    THEOLIZER_INTERNAL_ELEMENT_KN((mEnum),mEnum,etmDefault,\
        (theolizerD::All),\
        (EnumTest),2)
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

//      ---<<< Version.3 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,3)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_CLASS_NAME((u8"IntrusiveDerived"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_BASE_LIST()\
    THEOLIZER_INTERNAL_BASE_KI(public,etmDefault,0,(IntrusiveBase),2) THEOLIZER_GENERATED_SEP\
    THEOLIZER_INTERNAL_BASE_KI_DEL(public,etmDefault,1,(IntrusiveBase2),1)
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mUInt),mUInt,etmDefault,\
        (theolizerD::All),\
        (unsigned int))\
    THEOLIZER_INTERNAL_ELEMENT_N((mUShort2),mUShort,etmDefault,\
        (theolizerD::All),\
        (unsigned short))\
    THEOLIZER_INTERNAL_ELEMENT_KI((mIntrusiveBase),mIntrusiveBase,etmDefault,\
        (theolizerD::All),\
        (IntrusiveBase),2)\
    THEOLIZER_INTERNAL_ELEMENT_KI_DEL((mIntrusiveBase2),,etmDefault,\
        (theolizerD::All),\
        (IntrusiveBase2),1)\
    THEOLIZER_INTERNAL_ELEMENT_KN((mEnum),mEnum,etmDefault,\
        (theolizerD::All),\
        (EnumTest),2)
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

//      ---<<< Version.2 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,2)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_CLASS_NAME((u8"IntrusiveDerived"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_BASE_LIST()\
    THEOLIZER_INTERNAL_BASE_KI(public,etmDefault,0,(IntrusiveBase),1) THEOLIZER_GENERATED_SEP\
    THEOLIZER_INTERNAL_BASE_KI(public,etmDefault,1,(IntrusiveBase2),1)
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mUShort1),mUShort2,etmDefault,\
        (theolizerD::All),\
        (unsigned short))\
    THEOLIZER_INTERNAL_ELEMENT_KI((mIntrusiveBase),mIntrusiveBase,etmDefault,\
        (theolizerD::All),\
        (IntrusiveBase),1)\
    THEOLIZER_INTERNAL_ELEMENT_KI((mIntrusiveBase2),mIntrusiveBase2,etmDefault,\
        (theolizerD::All),\
        (IntrusiveBase2),1)\
    THEOLIZER_INTERNAL_ELEMENT_KN((mEnum),mEnum,etmDefault,\
        (theolizerD::All),\
        (EnumTest),2)
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_CLASS_NAME((u8"IntrusiveDerived"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_BASE_LIST()\
    THEOLIZER_INTERNAL_BASE_KI(public,etmDefault,0,(IntrusiveBase),1)
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mUShort0),mUShort1,etmDefault,\
        (theolizerD::All),\
        (unsigned short))\
    THEOLIZER_INTERNAL_ELEMENT_N_DEL((mUChar),,etmDefault,\
        (theolizerD::All),\
        (unsigned char))\
    THEOLIZER_INTERNAL_ELEMENT_KI((mIntrusiveBase),mIntrusiveBase,etmDefault,\
        (theolizerD::All),\
        (IntrusiveBase),1)\
    THEOLIZER_INTERNAL_ELEMENT_KN((mEnum),mEnum,etmDefault,\
        (theolizerD::All),\
        (EnumTest),1)
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

// ***************************************************************************
//      完全自動型
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_FULL_AUTO FullAuto

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_CLASS_NAME((u8"FullAuto"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mInt),mInt,etmDefault,\
        (theolizerD::All),\
        (int))\
    THEOLIZER_INTERNAL_ELEMENT_N((mShort),mShort,etmDefault,\
        (theolizerD::All),\
        (short))
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

// ***************************************************************************
//      完全自動型テンプレート
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO  THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_PARAMETER_LIST template<typename tType0,typename tType1>
#define THEOLIZER_GENERATED_UNIQUE_NAME FullAutoTemplatePrimary
#define THEOLIZER_GENERATED_FULL_AUTO FullAutoTemplate<tType0,tType1>

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8"FullAutoTemplate",tType0,tType1))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mType0),mType0,etmDefault,\
        (theolizerD::All),\
        (tType0))\
    THEOLIZER_INTERNAL_ELEMENT_N((mType1),mType1,etmDefault,\
        (theolizerD::All),\
        (tType1))
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

//############################################################################
//      同じバージョン内変更テスト
//############################################################################

// ***************************************************************************
//      同じバージョン内変更テストの保存側クラス
//          型チェックがない派生Serializerだから可能なテスト。
//          型チェックがある場合、同じクラス名で異なるクラス定義が必要。
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE DerivedChange

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME() THEOLIZER_INTERNAL_CLASS_NAME((u8"DerivedChange"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_BASE_LIST()\
    THEOLIZER_INTERNAL_BASE_KI(public,etmDefault,0,(IntrusiveBase2),1) THEOLIZER_GENERATED_SEP\
    THEOLIZER_INTERNAL_BASE_KI(public,etmDefault,1,(IntrusiveBase),2)
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_KI((mIntrusiveBase2),mIntrusiveBase2,etmDefault,\
        (theolizerD::All),\
        (IntrusiveBase2),1)\
    THEOLIZER_INTERNAL_ELEMENT_KI((mIntrusiveBase),mIntrusiveBase,etmDefault,\
        (theolizerD::All),\
        (IntrusiveBase),2)\
    THEOLIZER_INTERNAL_ELEMENT_N((mUShort),mUShort,etmDefault,\
        (theolizerD::All),\
        (unsigned short))\
    THEOLIZER_INTERNAL_ELEMENT_N((mUInt),mUInt,etmDefault,\
        (theolizerD::All),\
        (unsigned int))
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

// ***************************************************************************
//      同じバージョン内変更テストの回復側クラス(追加／順序変更)
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------
namespace change_order{namespace dummy{

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE change_order::dummy::DerivedChange

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_CLASS_NAME((u8"change_order::dummy::DerivedChange"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_BASE_LIST()\
    THEOLIZER_INTERNAL_BASE_KI(public,etmDefault,0,(IntrusiveBase),2) THEOLIZER_GENERATED_SEP\
    THEOLIZER_INTERNAL_BASE_KI(public,etmDefault,1,(IntrusiveBase2),1)
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mUInt),mUInt,etmDefault,\
        (theolizerD::All),\
        (unsigned int))\
    THEOLIZER_INTERNAL_ELEMENT_N((mUShort),mUShort,etmDefault,\
        (theolizerD::All),\
        (unsigned short))\
    THEOLIZER_INTERNAL_ELEMENT_KI((mIntrusiveBase),mIntrusiveBase,etmDefault,\
        (theolizerD::All),\
        (IntrusiveBase),2)\
    THEOLIZER_INTERNAL_ELEMENT_N((mUChar),mUChar,etmDefault,\
        (theolizerD::All),\
        (unsigned char))\
    THEOLIZER_INTERNAL_ELEMENT_KI((mIntrusiveBase2),mIntrusiveBase2,etmDefault,\
        (theolizerD::All),\
        (IntrusiveBase2),1)
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

}   // namespace dummy
}   // namespace change_order

// ***************************************************************************
//      同じバージョン内変更テストの回復側クラス(削除)
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE DerivedDelete

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME() THEOLIZER_INTERNAL_CLASS_NAME((u8"DerivedDelete"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_BASE_LIST()\
    THEOLIZER_INTERNAL_BASE_KI(public,etmDefault,0,(IntrusiveBase),2)
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mUInt),mUInt,etmDefault,\
        (theolizerD::All),\
        (unsigned int))\
    THEOLIZER_INTERNAL_ELEMENT_KI((mIntrusiveBase2),mIntrusiveBase2,etmDefault,\
        (theolizerD::All),\
        (IntrusiveBase2),1)
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

//############################################################################
//      ヘッダ・テスト
//          保存先毎のTypeInfoList登録テスト
//############################################################################

// ***************************************************************************
//      基底／包含クラス1
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE Included1

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME() THEOLIZER_INTERNAL_CLASS_NAME((u8"Included1"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mInt),mInt,etmPointee,\
        (theolizerD::All),\
        (int))
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

// ***************************************************************************
//      基底／包含クラス2
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE Included2

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME() THEOLIZER_INTERNAL_CLASS_NAME((u8"Included2"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mShort),mShort,etmDefault,\
        (theolizerD::All),\
        (short))
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

// ***************************************************************************
//      対象クラス
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE DestinationTest

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME() THEOLIZER_INTERNAL_CLASS_NAME((u8"DestinationTest"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_KI((mIncluded1),mIncluded1,etmDefault,\
        (theolizerD::Machine),\
        (Included1),1)\
    THEOLIZER_INTERNAL_ELEMENT_KI((mIncluded2),mIncluded2,etmDefault,\
        (theolizerD::User),\
        (Included2),1)\
    THEOLIZER_INTERNAL_ELEMENT_KN((mFullAutoTemplate),mFullAutoTemplate,etmDefault,\
        (theolizerD::All),\
        (FullAutoTemplate<int, short>),1)
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

//############################################################################
//      オブジェクト追跡指定
//############################################################################

// ***************************************************************************
//      静的定義領域追跡テスト用クラス
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE Tracking

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME() THEOLIZER_INTERNAL_CLASS_NAME((u8"Tracking"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_BASE_LIST()\
    THEOLIZER_INTERNAL_BASE_KI(public,etmDefault,0,(Included1),1)
#ifndef NO_ARRAY
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mUInt),mUInt,etmPointee,\
        (theolizerD::All),\
        (unsigned))\
    THEOLIZER_INTERNAL_ELEMENT_KN((mEnum),mEnum,etmPointee,\
        (theolizerD::All),\
        (EnumTest),2)\
    THEOLIZER_INTERNAL_ELEMENT_KI((mIncluded2),mIncluded2,etmPointee,\
        (theolizerD::All),\
        (Included2),1)\
    THEOLIZER_INTERNAL_ELEMENT_N((mString),mString,etmPointee,\
        (theolizerD::All),\
        (string))\
    THEOLIZER_INTERNAL_ELEMENT_AN((mArray),mArray,etmPointee,\
        (theolizerD::All),\
        (unsigned),2,3)
#else
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mUInt),mUInt,etmPointee,\
        (theolizerD::All),\
        (unsigned))\
    THEOLIZER_INTERNAL_ELEMENT_KN((mEnum),mEnum,etmPointee,\
        (theolizerD::All),\
        (EnumTest),2)\
    THEOLIZER_INTERNAL_ELEMENT_KI((mIncluded2),mIncluded2,etmPointee,\
        (theolizerD::All),\
        (Included2),1)\
    THEOLIZER_INTERNAL_ELEMENT_N((mString),mString,etmPointee,\
        (theolizerD::All),\
        (string))
#endif
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

// ***************************************************************************
//      ポインタ回復テスト用クラス
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE Pointers

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME() THEOLIZER_INTERNAL_CLASS_NAME((u8"Pointers"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mUInt),mUInt,etmDefault,\
        (theolizerD::All),\
        (unsigned*))\
    THEOLIZER_INTERNAL_ELEMENT_N((mEnum),mEnum,etmDefault,\
        (theolizerD::All),\
        (EnumTest*))\
    THEOLIZER_INTERNAL_ELEMENT_N((mIncluded2),mIncluded2,etmDefault,\
        (theolizerD::All),\
        (Included2*))\
    THEOLIZER_INTERNAL_ELEMENT_N((mString),mString,etmDefault,\
        (theolizerD::All),\
        (string*))\
    THEOLIZER_INTERNAL_ELEMENT_N((mArrayInt),mArrayInt,etmDefault,\
        (theolizerD::All),\
        (int (*)[2][3]))\
    THEOLIZER_INTERNAL_ELEMENT_KN((mFullAutoTemplateArray),mFullAutoTemplateArray,etmDefault,\
        (theolizerD::All),\
        (FullAutoTemplate<int[2], short (*)[3]>),1)
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

// ***************************************************************************
//      動的生成領域追跡テスト用クラス
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE Dynamic

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME() THEOLIZER_INTERNAL_CLASS_NAME((u8"Dynamic"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#ifndef NO_ARRAY
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mUInt),mUInt,etmOwner,\
        (theolizerD::All),\
        (unsigned*))\
    THEOLIZER_INTERNAL_ELEMENT_N((mEnum),mEnum,etmOwner,\
        (theolizerD::All),\
        (EnumTest*))\
    THEOLIZER_INTERNAL_ELEMENT_N((mIncluded1),mIncluded1,etmOwner,\
        (theolizerD::All),\
        (Included1*))\
    THEOLIZER_INTERNAL_ELEMENT_N((mString),mString,etmOwner,\
        (theolizerD::All),\
        (string*))\
    THEOLIZER_INTERNAL_ELEMENT_AN((mArray),mArray,etmOwner,\
        (theolizerD::All),\
        (int*),3)\
    THEOLIZER_INTERNAL_ELEMENT_N((mArrayPtr),mArrayPtr,etmOwner,\
        (theolizerD::All),\
        (int(*)[2]))
#else
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mUInt),mUInt,etmOwner,\
        (theolizerD::All),\
        (unsigned*))\
    THEOLIZER_INTERNAL_ELEMENT_N((mEnum),mEnum,etmOwner,\
        (theolizerD::All),\
        (EnumTest*))\
    THEOLIZER_INTERNAL_ELEMENT_N((mIncluded1),mIncluded1,etmOwner,\
        (theolizerD::All),\
        (Included1*))\
    THEOLIZER_INTERNAL_ELEMENT_N((mString),mString,etmOwner,\
        (theolizerD::All),\
        (string*))
#endif
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

//############################################################################
//      生配列を含むクラスのテスト
//############################################################################

// ***************************************************************************
//      テスト用クラス
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,2)
#define THEOLIZER_GENERATED_CLASS_TYPE ArrayClass

//      ---<<< Version.2 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,2)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_CLASS_NAME((u8"ArrayClass"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#ifndef NO_ARRAY
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_AN((mArrayInc),mArrayInc,etmDefault,\
        (theolizerD::All),\
        (short),2,3,4)\
    THEOLIZER_INTERNAL_ELEMENT_AN((mArrayDec),mArrayDec,etmDefault,\
        (theolizerD::All),\
        (unsigned int),2)\
    THEOLIZER_INTERNAL_ELEMENT_AKI((mClassArrayInc),mClassArrayInc,etmDefault,\
        (theolizerD::All),\
        (IntrusiveBase),2,2,3,4)\
    THEOLIZER_INTERNAL_ELEMENT_AKI((mClassArrayDec),mClassArrayDec,etmDefault,\
        (theolizerD::All),\
        (IntrusiveBase),2,2)\
    THEOLIZER_INTERNAL_ELEMENT_AKN((mEnumArrayInc),mEnumArrayInc,etmDefault,\
        (theolizerD::All),\
        (EnumTest),2,2,3,4)\
    THEOLIZER_INTERNAL_ELEMENT_AKN((mEnumArrayDec),mEnumArrayDec,etmDefault,\
        (theolizerD::All),\
        (EnumTest),2,2)
#else
#define THEOLIZER_GENERATED_ELEMENT_LIST()
#endif
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_CLASS_NAME((u8"ArrayClass"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#ifndef NO_ARRAY
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_AN((mArrayInc),mArrayInc,etmDefault,\
        (theolizerD::All),\
        (short),2)\
    THEOLIZER_INTERNAL_ELEMENT_AN((mArrayDec),mArrayDec,etmDefault,\
        (theolizerD::All),\
        (unsigned int),2,3,4)\
    THEOLIZER_INTERNAL_ELEMENT_AKI((mClassArrayInc),mClassArrayInc,etmDefault,\
        (theolizerD::All),\
        (IntrusiveBase),1,2)\
    THEOLIZER_INTERNAL_ELEMENT_AKI((mClassArrayDec),mClassArrayDec,etmDefault,\
        (theolizerD::All),\
        (IntrusiveBase),1,2,3,4)\
    THEOLIZER_INTERNAL_ELEMENT_AKN((mEnumArrayInc),mEnumArrayInc,etmDefault,\
        (theolizerD::All),\
        (EnumTest),1,2)\
    THEOLIZER_INTERNAL_ELEMENT_AKN((mEnumArrayDec),mEnumArrayDec,etmDefault,\
        (theolizerD::All),\
        (EnumTest),1,2,3,4)
#else
#define THEOLIZER_GENERATED_ELEMENT_LIST()
#endif
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

// ***************************************************************************
//      生配列を含むクラス・テンプレート
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE ArrayClassTemplate<tDim0, tDim1>
#define THEOLIZER_GENERATED_PARAMETER_LIST template<std::size_t tDim0, std::size_t tDim1>
#define THEOLIZER_GENERATED_UNIQUE_NAME ArrayClassTemplatePrimary

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8"ArrayClassTemplate",\
        theolizer::internal::NonType<std::size_t,tDim0>,\
        theolizer::internal::NonType<std::size_t,tDim1>))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#ifndef NO_ARRAY
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_AN((mIntArray),mIntArray,etmDefault,\
        (theolizerD::All),\
        (int),tDim0*2,tDim1*2)
#else
#define THEOLIZER_GENERATED_ELEMENT_LIST()
#endif
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

// ***************************************************************************
//      非侵入型(基底クラス)
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE NonIntrusiveBase

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME() THEOLIZER_INTERNAL_CLASS_NAME((u8"NonIntrusiveBase"))
#include <theolizer/internal/version_manual.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

// ***************************************************************************
//      非侵入型(派生クラス)
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE NonIntrusiveDrived

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_CLASS_NAME((u8"NonIntrusiveDrived"))
#define THEOLIZER_GENERATED_BASE_LIST()\
    THEOLIZER_INTERNAL_BASE_N(public,etmDefault,0,(NonIntrusiveBase))
#include <theolizer/internal/version_manual.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

// ***************************************************************************
//      侵入型(派生クラス)
// ***************************************************************************

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE IntrusiveDerived2

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME() THEOLIZER_INTERNAL_CLASS_NAME((u8"IntrusiveDerived2"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emName
#define THEOLIZER_GENERATED_BASE_LIST()\
    THEOLIZER_INTERNAL_BASE_N(public,etmDefault,0,(NonIntrusiveBase))
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mShort),mShort,etmDefault,\
        (theolizerD::All),\
        (short))
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

#endif
