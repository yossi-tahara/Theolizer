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

namespace exchange
{
    // ユーザ・クラス
    //  ユーザが追加できるようpartial
    partial class UserClassSub : ITheolizer
    {
        public UInt32 mUIntSub;
        public string mStringSub = "";

        // シリアライズ
        void ITheolizer.save(BaseSerializer iBaseSerializer)
        {
        }

        // デシリアライズ
        void ITheolizer.load(BaseSerializer iBaseSerializer)
        {
        }
    }

    partial class UserClassMain : ITheolizer
    {
        public Int32 mIntMain;

        public void func0(UserClassSub iUserClassSub)
        {
            var aFuncObject = new func0Theolizer(this, iUserClassSub);
            // インテグレータ獲得
            // シリアライザ獲得
            //aFuncObject.save(シリアライザ);
        }
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
            }

            // デシリアライズ
            void ITheolizer.load(BaseSerializer iBaseSerializer)
            {
            }
        }

        // シリアライズ
        void ITheolizer.save(BaseSerializer iBaseSerializer)
        {
        }

        // デシリアライズ
        void ITheolizer.load(BaseSerializer iBaseSerializer)
        {
        }
    }
}