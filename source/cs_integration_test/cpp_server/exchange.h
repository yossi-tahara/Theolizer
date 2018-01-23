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

#if !defined(THEOLIZER_INTERNAL_EXCHANGE_H)
#define THEOLIZER_INTERNAL_EXCHANGE_H

#include <theolizer/integrator/core_integrator.h>

// ***************************************************************************
//      データ処理用クラス群
// ***************************************************************************

namespace exchange
{

//----------------------------------------------------------------------------
//      サブ・クラス
//----------------------------------------------------------------------------

class UserClassSub
{
    //      ---<<< メンバ変数群 >>>---
public:
    unsigned    mUIntSub;
    std::string mStringSub = "";
    short       mShortSub THEOLIZER_ANNOTATE(FN);

    // コンストラクタ
    UserClassSub() : mUIntSub(100), mShortSub(200)
    { }

    //      ---<<< メンバ関数群 >>>---

    // C++ → C#(アノテーションで指定する)
    void notify() const;

    THEOLIZER_INTRUSIVE_ORDER(CS, (UserClassSub), 1);
};

//----------------------------------------------------------------------------
//      メイン・クラス
//----------------------------------------------------------------------------

class UserClassMain
{
public:
    //      ---<<< メンバ変数群 >>>---

    int     mIntMain;
    short   mShortMain THEOLIZER_ANNOTATE(FN);

    // コンストラクタ
    UserClassMain() : mIntMain(10), mShortMain(20)
    { }

    //      ---<<< メンバ関数群 >>>---

    // C# → C++(アノテーションで指定する)
    //  中身はユーザが自由に記述する
    int request(UserClassSub const& iUserClassSub, UserClassSub& ioUserClassSub2);

    THEOLIZER_INTRUSIVE_ORDER(CS, (UserClassMain), 1);
};

}   // namespace exchange

// ***************************************************************************
//      自動生成予定のクラス群
// ***************************************************************************

//----------------------------------------------------------------------------
//      関数クラス
//----------------------------------------------------------------------------

namespace theolizer_integrator
{

//      ---<<< UserClassSubのnotify() >>>---

class notifyUserClassSub
{
public:
    SharedHelperTheolizer<exchange::UserClassSub>                       mThis;
    THEOLIZER_INTRUSIVE_ORDER(CS, (notifyUserClassSub), 1);
};

//      ---<<< UserClassMainのrequest()の応答 >>>---

class requestUserClassMainReturn
{
public:
    int                                             mReturn;
    SharedHelperTheolizer<exchange::UserClassMain>  mThis;
    SharedHelperTheolizer<exchange::UserClassSub>   mioUserClassSub2;
    THEOLIZER_INTRUSIVE_ORDER(CS, (requestUserClassMainReturn), 1);
};

//      ---<<< UserClassMainのrequest()の要求 >>>---

class requestUserClassMain
{
public:
    SharedHelperTheolizer<exchange::UserClassMain>                      mThis;
    theolizer::internal::TheolizerParameter<exchange::UserClassSub>     miUserClassSub;
    SharedHelperTheolizer<exchange::UserClassSub>                       mioUserClassSub2;
    THEOLIZER_INTRUSIVE_ORDER(CS, (requestUserClassMain), 1);

    requestUserClassMainReturn callFunc()
    {
        requestUserClassMainReturn    ret;

        ret.mReturn = mThis->request(miUserClassSub, mioUserClassSub2);
        ret.mThis = mThis;
        ret.mioUserClassSub2 = mioUserClassSub2;
        return ret;
    }
};

}   // namespace theolizer_user_functions

THEOLIZER_INTERNAL_REGISTER_FUNC((theolizer_integrator::requestUserClassMain));

//----------------------------------------------------------------------------
//      通知関数のシリアライズ処理
//----------------------------------------------------------------------------

namespace exchange
{
    inline void UserClassSub::notify() const
    {
        std::cout << "notify()\n";
        theolizer::ThreadIntegrator::getIntegrator()->notify(*this);
    }
}

#endif  // THEOLIZER_INTERNAL_EXCHANGE_H
