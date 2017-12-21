//############################################################################
//      Theolizer Test Project for C# Integration
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

#if !defined(THEOLIZER_INTERNAL_TYPE_H)
#define THEOLIZER_INTERNAL_CPP_SERVER_H

#include <theolizer/integrator/core_integrator.h>

// ***************************************************************************
//      データ処理用クラス群
// ***************************************************************************

namespace exchange
{

class UserClassSub
{
    //      ---<<< メンバ変数群 >>>---
public:
    unsigned    mUIntSub;
    std::string mStringSub = "";
    THEOLIZER_INTRUSIVE_ORDER(CS, (UserClassSub), 1);
};

class UserClassMain
{
public:
    //      ---<<< メンバ変数群 >>>---

    int     mIntMain;

    //      ---<<< メンバ関数群 >>>---

    int func0(UserClassSub const& iUserClassSub)
    {
        std::cout << "func0()\n"
                  << "    mIntMain=" << mIntMain << "\n"
                  << "    iUserClassSub.mUIntSub  =" << iUserClassSub.mUIntSub << "\n"
                  << "    iUserClassSub.mStringSub=" << iUserClassSub.mStringSub << "\n";
        mIntMain *= 2;
        return iUserClassSub.mUIntSub * 3;
    }

    THEOLIZER_INTRUSIVE_ORDER(CS, (UserClassMain), 1);
};

}   // namespace exchange

// ***************************************************************************
//      自動生成予定のクラス群
// ***************************************************************************

namespace theolizer_integrator
{

class func0UserClassMainReturn
{
public:
    int                                             mReturn;
    SharedHelperTheolizer<exchange::UserClassMain>  mThis;
    THEOLIZER_INTRUSIVE_ORDER(CS, (func0UserClassMainReturn), 1);
};

class func0UserClassMain
{
public:
    SharedHelperTheolizer<exchange::UserClassMain>                      mThis;
    theolizer::internal::TheolizerParameter<exchange::UserClassSub>     miUserClassSub;
    THEOLIZER_INTRUSIVE_ORDER(CS, (func0UserClassMain), 1);

    func0UserClassMainReturn callFunc()
    {
        func0UserClassMainReturn    ret;

        ret.mReturn = mThis->func0(miUserClassSub);
        ret.mThis = mThis;
        return ret;
    }
};

}   // namespace theolizer_user_functions

THEOLIZER_INTERNAL_REGISTER_FUNC((theolizer_integrator::func0UserClassMain));

#endif  // THEOLIZER_INTERNAL_TYPE_H
