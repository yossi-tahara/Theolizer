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

#include "test_serializer_base_auto_gen.h"
#include <theolizer/vector.h>
#include <theolizer/list.h>
#include <theolizer/memory.h>

namespace theolizer{namespace internal{
namespace global_table{
    MyGVNT::MyGVNT()
    {
        THEOLIZER_INTERNAL_ADD(theolizer::internal::JsonSerializerVersion, 1u, 1u, 1u, 1u);
        THEOLIZER_INTERNAL_ADD(IntrusiveBase,                      1u,1u,2u,2u);
        THEOLIZER_INTERNAL_ADD(IntrusiveBase2,                     1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD(IntrusiveDerived,                   1u,2u,3u,4u);
        THEOLIZER_INTERNAL_ADD(IntrusiveDerived2,                  1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD(DerivedChange,                      1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD(change_order::dummy::DerivedChange, 1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD(DerivedDelete,                      1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD(EnumTest,                           1u,2u,2u,2u);
        THEOLIZER_INTERNAL_ADD(another::EnumTest,                  1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD(DestinationTest,                    1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD(Included1,                          1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD(Included2,                          1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD(Tracking,                           1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD(Dynamic,                            1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD(ArrayClass,                         1u,1u,2u,2u);
        THEOLIZER_INTERNAL_ADD(NonIntrusiveBase,                   1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD(NonIntrusiveDrived,                 1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD(ArrayClassTemplatePrimary,          1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD(Pointers,                           1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD(vectorTheolizer,                    1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD(listTheolizer,                      1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD(unique_ptrTheolizer,                1u,1u,1u,1u);
    }
}   // namespace global_table
}}  // namespace theolizer

namespace theolizer{namespace internal{namespace{
    RegisterLocalVersions::RegisterLocalVersions()
    {
        THEOLIZER_INTERNAL_ADD2(theolizer::internal::JsonSerializerVersion, 1u, 1u, 1u, 1u);
        THEOLIZER_INTERNAL_ADD2(IntrusiveBase,                      1u,1u,2u,2u);
        THEOLIZER_INTERNAL_ADD2(IntrusiveBase2,                     1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD2(IntrusiveDerived,                   1u,2u,3u,4u);
        THEOLIZER_INTERNAL_ADD2(IntrusiveDerived2,                  1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD2(DerivedChange,                      1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD2(change_order::dummy::DerivedChange, 1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD2(DerivedDelete,                      1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD2(EnumTest,                           1u,2u,2u,2u);
        THEOLIZER_INTERNAL_ADD2(another::EnumTest,                  1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD2(DestinationTest,                    1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD2(Included1,                          1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD2(Included2,                          1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD2(Tracking,                           1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD2(Dynamic,                            1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD2(ArrayClass,                         1u,1u,2u,2u);
        THEOLIZER_INTERNAL_ADD2(NonIntrusiveBase,                   1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD2(NonIntrusiveDrived,                 1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD2(ArrayClassTemplatePrimary,          1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD2(Pointers,                           1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD2(vectorTheolizer,                    1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD2(listTheolizer,                      1u,1u,1u,1u);
        THEOLIZER_INTERNAL_ADD2(unique_ptrTheolizer,                1u,1u,1u,1u);
    }
}}}  // namespace theolizer
