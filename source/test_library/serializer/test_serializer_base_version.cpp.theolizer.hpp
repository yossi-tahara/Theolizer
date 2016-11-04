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

#include "test_serializer_base_auto_gen.h"
#include <theolizer/vector.h>
#include <theolizer/list.h>
#include <theolizer/memory.h>

namespace theolizer{namespace internal{
namespace global_table{
    MyGVNT::MyGVNT()
    {
        add(typeid(theolizer::internal::JsonMidOSerializer),1u,1u,1u,2u);
        add(typeid(theolizer::internal::JsonMidISerializer),1u,1u,1u,2u);
        add(typeid(IntrusiveBase),                      1u,1u,2u,2u);
        add(typeid(IntrusiveBase2),                     1u,1u,1u,1u);
        add(typeid(IntrusiveDerived),                   1u,2u,3u,4u);
        add(typeid(IntrusiveDerived2),                  1u,1u,1u,1u);
        add(typeid(DerivedChange),                      1u,1u,1u,1u);
        add(typeid(change_order::dummy::DerivedChange), 1u,1u,1u,1u);
        add(typeid(DerivedDelete),                      1u,1u,1u,1u);
        add(typeid(EnumTest),                           1u,2u,2u,2u);
        add(typeid(another::EnumTest),                  1u,1u,1u,1u);
        add(typeid(DestinationTest),                    1u,1u,1u,1u);
        add(typeid(Included1),                          1u,1u,1u,1u);
        add(typeid(Included2),                          1u,1u,1u,1u);
        add(typeid(Tracking),                           1u,1u,1u,1u);
        add(typeid(Dynamic),                            1u,1u,1u,1u);
        add(typeid(ArrayClass),                         1u,1u,2u,2u);
        add(typeid(NonIntrusiveBase),                   1u,1u,1u,1u);
        add(typeid(NonIntrusiveDrived),                 1u,1u,1u,1u);
        add(typeid(ArrayClassTemplatePrimary),          1u,1u,1u,1u);
        add(typeid(Pointers),                           1u,1u,1u,1u);
        add(typeid(vectorPrimary),                      1u,1u,1u,1u);
        add(typeid(listPrimary),                        1u,1u,1u,1u);
        add(typeid(unique_ptrPrimary),                  1u,1u,1u,1u);
    }
}   // namespace global_table
}}  // namespace theolizer
