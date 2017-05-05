//############################################################################
//      各serializerのテスト
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

#if !defined(THEOLIZER_TEST_SERIALIZER_BASE02_H)
#define THEOLIZER_TEST_SERIALIZER_BASE02_H

// ***************************************************************************
//      通常のインクルード
// ***************************************************************************

using namespace std;

#include <theolizer/serializer_json.h>
using theolizer::u8string;

//############################################################################
//      グローバル・バージョン番号テーブル
//############################################################################

// TODO:GVNTを最終版へ変更した時、このテストは全体的に見直す
//      現在は無理やりなシミュレーションなので。
THEOLIZER_DEFINE_GLOBAL_VERSION_TABLE(MyGVNT, 4);

//############################################################################
//      クラス変更エラーのテスト
//############################################################################

// ***************************************************************************
//      ユーザ・クラス定義(Base2)
// ***************************************************************************

class IntrusiveBase2
{
    THEOLIZER_INTRUSIVE_ORDER(CS, (IntrusiveBase2), 1);

public:
    long                mLong;
    long long           mLongLong;
    unsigned long       mULong;
    unsigned short      mULongLong;     // unsigned long long -> unsigned short

    IntrusiveBase2() :
                mLong(),
                mLongLong(),
                mULong(),
                mULongLong()
    { }
};

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE THEOLIZER_INTERNAL_UNPAREN(IntrusiveBase2)

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_CLASS_NAME((u8"IntrusiveBase2"))
#define THEOLIZER_GENERATED_ELEMENT_MAP emOrder
#define THEOLIZER_GENERATED_ELEMENT_LIST()\
    THEOLIZER_INTERNAL_ELEMENT_N((mLong),mLong,etmDefault,\
        (theolizerD::All),\
        (long))\
    THEOLIZER_INTERNAL_ELEMENT_N((mLongLong),mLongLong,etmDefault,\
        (theolizerD::All),\
        (long long))\
    THEOLIZER_INTERNAL_ELEMENT_N((mULong),mULong,etmDefault,\
        (theolizerD::All),\
        (unsigned long))\
    THEOLIZER_INTERNAL_ELEMENT_N((mULongLong),mULongLong,etmDefault,\
        (theolizerD::All),\
        (unsigned short))
#include <theolizer/internal/version_auto.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

namespace theolizer{namespace internal{
namespace global_table{
    MyGVNT::MyGVNT()
    {
        add(typeid(theolizer::internal::JsonMidOSerializer), 1u, 1u, 1u, 2u);
        add(typeid(theolizer::internal::JsonMidISerializer), 1u, 1u, 1u, 2u);
        add(typeid(IntrusiveBase2),                          1u, 1u, 1u, 1u);
    }
}   // namespace global_table
}}  // namespace theolizer

#endif
