//############################################################################
//      自動生成を想定したファイル
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

using System;
using theolizer;
using theolizer.internal_space;

namespace exchange
{
    // ***************************************************************************
    //      ユーザ・クラス
    //          ユーザがメンバを追加できるようpartial
    // ***************************************************************************

    partial class UserClassSub : ITheolizer
    {
        //      ---<<< メンバ変数群 >>>---

        public UInt32 mUIntSub;
        public string mStringSub = "";

        // シリアライズ
        void ITheolizer.save(BaseSerializer iBaseSerializer)
        {
            using (var temp = new BaseSerializer.AutoRestoreSaveStructure(iBaseSerializer))
            {
                iBaseSerializer.writePreElement();
                iBaseSerializer.savePrimitive(mUIntSub);
                iBaseSerializer.writePreElement();
                iBaseSerializer.savePrimitive(mStringSub);
            }
        }

        // デシリアライズ
        void ITheolizer.load(BaseSerializer iBaseSerializer)
        {
        }
    }

    partial class UserClassMain : ITheolizer
    {
        //      ---<<< メンバ変数群 >>>---

        public Int32 mIntMain;

        // シリアライズ
        void ITheolizer.save(BaseSerializer iBaseSerializer)
        {
            using (var temp = new BaseSerializer.AutoRestoreSaveStructure(iBaseSerializer))
            {
                iBaseSerializer.writePreElement();
                iBaseSerializer.savePrimitive(mIntMain);
            }
        }

        // デシリアライズ
        void ITheolizer.load(BaseSerializer iBaseSerializer)
        {
        }

        //      ---<<< メンバ関数群 >>>---

        public void func0(UserClassSub iUserClassSub)
        {
            var aFuncObject = new func0Theolizer(this, iUserClassSub);
            // インテグレータ獲得
            var aIntegrator = ThreadIntegrator.Integrator;
            // シリアライザ獲得
            var aSerializer = aIntegrator.Serializer;
            // 保存
            ((ITheolizer)aFuncObject).save(aSerializer);
            // 出力
            aSerializer.flush();
        }
    }

    // UserClassMain::func0用関数クラス
    class func0Theolizer : ITheolizer
    {
        UserClassMain   mThis;
        UserClassSub    miUserClassSub;
        public func0Theolizer
        (
            UserClassMain iThis,
            UserClassSub iUserClassSub
        )
        {
            mThis=iThis;
            miUserClassSub=iUserClassSub;
        }

        // シリアライズ
        void ITheolizer.save(BaseSerializer iBaseSerializer)
        {
            using (var temp = new BaseSerializer.AutoRestoreSaveStructure(iBaseSerializer))
            {
                iBaseSerializer.writePreElement();
                ((ITheolizer)mThis).save(iBaseSerializer);
                iBaseSerializer.writePreElement();
                ((ITheolizer)miUserClassSub).save(iBaseSerializer);
            }
        }

        // デシリアライズ
        void ITheolizer.load(BaseSerializer iBaseSerializer)
        {
        }
    }

    // ***************************************************************************
    //      各種ヘルパー
    // ***************************************************************************

    // クラス用インタフェース
    internal interface ITheolizer
    {
        void save(BaseSerializer iBaseSerializer);
        void load(BaseSerializer iBaseSerializer);
    }
}
