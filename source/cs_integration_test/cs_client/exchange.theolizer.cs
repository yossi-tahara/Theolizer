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

// ***************************************************************************
//      theolizer名前空間
// ***************************************************************************

namespace theolizer
{
    //----------------------------------------------------------------------------
    //      必要な定数定義
    //----------------------------------------------------------------------------

    static class Theolizer
    {
        public static uint GlobalVersionNo = 1;
    }

    //----------------------------------------------------------------------------
    //      各種ヘルパー
    //----------------------------------------------------------------------------

    // クラス用インタフェース
    internal interface ITheolizer
    {
        ITheolizerInternal getTheolizer();
    }
}

// ***************************************************************************
//      ユーザ・クラスのミラー
//          ユーザがメンバを追加できるようpartial
// ***************************************************************************

namespace exchange
{

    partial class UserClassSub :  ITheolizer
    {
        //      ---<<< メンバ変数群 >>>---

        public UInt32 mUIntSub;
        public string mStringSub = "";

        //      ---<<< シリアライズ処理 >>>---

        const UInt64 kTypeIndex = 2;

        TheolizerInternal   mTheolizerInternal;
        ITheolizerInternal ITheolizer.getTheolizer()
        {
            if (mTheolizerInternal == null)
            {
                mTheolizerInternal = new TheolizerInternal(this);
            }
            return mTheolizerInternal;
        }
        class TheolizerInternal : ITheolizerInternal
        {
            UserClassSub mTheolizer;
            public TheolizerInternal(UserClassSub iTheolizer)
            {
                mTheolizer=iTheolizer;
            }
            public UInt64 getTypeIndex() { return UserClassSub.kTypeIndex; }
            public void save(BaseSerializer iBaseSerializer)
            {
                using (var temp = new BaseSerializer.AutoRestoreSaveStructure(iBaseSerializer))
                {
                    iBaseSerializer.writePreElement();
                    iBaseSerializer.savePrimitive(mTheolizer.mUIntSub);
                    iBaseSerializer.writePreElement();
                    iBaseSerializer.savePrimitive(mTheolizer.mStringSub);
                }
            }
            public void load(BaseSerializer iBaseSerializer)
            {
                using (var temp = new BaseSerializer.AutoRestoreLoadStructure(iBaseSerializer))
                {
                    if (iBaseSerializer.readPreElement() == ReadStat.Terminated)
            throw new InvalidOperationException("Format Error.");
                    iBaseSerializer.loadPrimitive(out mTheolizer.mUIntSub);

                    if (iBaseSerializer.readPreElement() == ReadStat.Terminated)
            throw new InvalidOperationException("Format Error.");
                    iBaseSerializer.loadPrimitive(out mTheolizer.mStringSub);
                }
            }
        }
    }

    partial class UserClassMain :  ITheolizer
    {
        //      ---<<< メンバ変数群 >>>---

        public Int32 mIntMain;

        //      ---<<< シリアライズ処理 >>>---

        const UInt64 kTypeIndex = 3;

        TheolizerInternal   mTheolizerInternal;
        ITheolizerInternal ITheolizer.getTheolizer()
        {
            if (mTheolizerInternal == null)
            {
                mTheolizerInternal = new TheolizerInternal(this);
            }
            return mTheolizerInternal;
        }
        class TheolizerInternal : ITheolizerInternal
        {
            UserClassMain mTheolizer;
            public TheolizerInternal(UserClassMain iTheolizer)
            {
                mTheolizer=iTheolizer;
            }
            public UInt64 getTypeIndex() { return UserClassMain.kTypeIndex; }
            public void save(BaseSerializer iBaseSerializer)
            {
                using (var temp = new BaseSerializer.AutoRestoreSaveStructure(iBaseSerializer))
                {
                    iBaseSerializer.writePreElement();
                    iBaseSerializer.savePrimitive(mTheolizer.mIntMain);
                }
            }
            public void load(BaseSerializer iBaseSerializer)
            {
                using (var temp = new BaseSerializer.AutoRestoreLoadStructure(iBaseSerializer))
                {
                    if (iBaseSerializer.readPreElement() == ReadStat.Terminated)
            throw new InvalidOperationException("Format Error.");
                    iBaseSerializer.loadPrimitive(out mTheolizer.mIntMain);
                }
            }
        }

        //      ---<<< メンバ関数群 >>>---

        public Int32 func0(UserClassSub iUserClassSub)
        {
            // インテグレータ獲得
            var aIntegrator = ThreadIntegrator.Integrator;

            // 要求送信と応答受信
            var aFuncObject = new theolizer_integrator.func0UserClassMain(this, iUserClassSub);
            var aReturnObject = new theolizer_integrator.func0UserClassMainReturn(this);
            aIntegrator.sendRequest(aFuncObject, aReturnObject);

            return aReturnObject.mReturn;
        }
    }
}

// ***************************************************************************
//      C++側自動生成のミラー
// ***************************************************************************

namespace theolizer_integrator
{
    // UserClassMain::func0用戻り値クラス
    class func0UserClassMainReturn : ITheolizerInternal
    {
        public Int32    mReturn;
        exchange.UserClassMain mThis;
        public func0UserClassMainReturn
        (
            exchange.UserClassMain iThis
        )
        {
            mThis=iThis;
        }

        const UInt64 kTypeIndex = 1;
        public UInt64 getTypeIndex() { return kTypeIndex; }
        public void save(BaseSerializer iBaseSerializer)
        {
            // 省略
            throw new NotImplementedException();
        }
        public void load(BaseSerializer iBaseSerializer)
        {
            using (var temp = new BaseSerializer.AutoRestoreLoadStructure(iBaseSerializer))
            {
                if (iBaseSerializer.readPreElement() == ReadStat.Terminated)
        throw new InvalidOperationException("Format Error.");
                iBaseSerializer.loadPrimitive(out mReturn);

                if (iBaseSerializer.readPreElement() == ReadStat.Terminated)
        throw new InvalidOperationException("Format Error.");
                ((ITheolizer)mThis).getTheolizer().load(iBaseSerializer);
            }
        }
    }

    // UserClassMain::func0用関数クラス
    class func0UserClassMain : ITheolizerInternal
    {
        exchange.UserClassMain mThis;
        exchange.UserClassSub  miUserClassSub;
        public func0UserClassMain
        (
            exchange.UserClassMain iThis,
            exchange.UserClassSub iUserClassSub
        )
        {
            mThis=iThis;
            miUserClassSub=iUserClassSub;
        }

        // TypeIndex
        public const UInt64 kTypeIndex = 0;
        public UInt64 getTypeIndex() { return kTypeIndex; }

        // シリアライズ
        public void save(BaseSerializer iBaseSerializer)
        {
            using (var temp = new BaseSerializer.AutoRestoreSaveStructure(iBaseSerializer))
            {
                iBaseSerializer.writePreElement();
                ((ITheolizer)mThis).getTheolizer().save(iBaseSerializer);

                iBaseSerializer.writePreElement();
                ((ITheolizer)miUserClassSub).getTheolizer().save(iBaseSerializer);
            }
        }

        // デシリアライズ
        public void load(BaseSerializer iBaseSerializer)
        {
            // 省略
            throw new NotImplementedException();
        }
    }
}
