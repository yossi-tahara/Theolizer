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
using System.Collections.Generic;
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
    //----------------------------------------------------------------------------
    //      サブ・クラス
    //----------------------------------------------------------------------------

    partial class UserClassSub : SharedDestructor, ITheolizer
    {
        //      ---<<< メンバ変数群 >>>---

        public UInt32 mUIntSub;
        public string mStringSub = "";

        //      ---<<< シリアライズ処理 >>>---

        public static readonly theolizer.internal_space.TypeIndex kTypeIndex2 =
            new theolizer.internal_space.TypeIndex(8u);

        TheolizerInternal   mTheolizerInternal = null;
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
            public theolizer.internal_space.TypeIndex getTypeIndex2() { return UserClassSub.kTypeIndex2; }
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

    //----------------------------------------------------------------------------
    //      メイン・クラス
    //----------------------------------------------------------------------------

    partial class UserClassMain : SharedDestructor, ITheolizer
    {
        //      ---<<< メンバ変数群 >>>---

        public Int32 mIntMain;

        //      ---<<< シリアライズ処理 >>>---

        public static readonly theolizer.internal_space.TypeIndex kTypeIndex2 =
            new theolizer.internal_space.TypeIndex(14u);

        TheolizerInternal   mTheolizerInternal = null;
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
            public theolizer.internal_space.TypeIndex getTypeIndex2() { return UserClassMain.kTypeIndex2; }
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

        public Int32 request(UserClassSub iUserClassSub, UserClassSub ioUserClassSub2)
        {
            // インテグレータ獲得
            var aIntegrator = ThreadIntegrator.Integrator;

            // 要求送信と応答受信
            var aFuncObject = new theolizer_integrator.requestUserClassMain(this, iUserClassSub, ioUserClassSub2);
            var aReturnObject = new theolizer_integrator.requestUserClassMainReturn();

            aIntegrator.sendRequest(aFuncObject, aReturnObject);

            return aReturnObject.mReturn;
        }
    }

//----------------------------------------------------------------------------
//      通知用クラス
//----------------------------------------------------------------------------

    partial class UserClassNotify : SharedDestructor, ITheolizer
    {
        //      ---<<< メンバ変数群 >>>---

        String  mMessage;
        Int32   mCount;

        //      ---<<< シリアライズ処理 >>>---

        public static readonly theolizer.internal_space.TypeIndex kTypeIndex2 =
            new theolizer.internal_space.TypeIndex(15u);

        TheolizerInternal   mTheolizerInternal = null;
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
            UserClassNotify mTheolizer;
            public TheolizerInternal(UserClassNotify iTheolizer)
            {
                mTheolizer=iTheolizer;
            }
            public theolizer.internal_space.TypeIndex getTypeIndex2() { return UserClassNotify.kTypeIndex2; }
            public void save(BaseSerializer iBaseSerializer)
            {
                using (var temp = new BaseSerializer.AutoRestoreSaveStructure(iBaseSerializer))
                {
                    iBaseSerializer.writePreElement();
                    iBaseSerializer.savePrimitive(mTheolizer.mMessage);

                    iBaseSerializer.writePreElement();
                    iBaseSerializer.savePrimitive(mTheolizer.mCount);
                }
            }
            public void load(BaseSerializer iBaseSerializer)
            {
                using (var temp = new BaseSerializer.AutoRestoreLoadStructure(iBaseSerializer))
                {
                    if (iBaseSerializer.readPreElement() == ReadStat.Terminated)
            throw new InvalidOperationException("Format Error.");
                    iBaseSerializer.loadPrimitive(out mTheolizer.mMessage);

                    if (iBaseSerializer.readPreElement() == ReadStat.Terminated)
            throw new InvalidOperationException("Format Error.");
                    iBaseSerializer.loadPrimitive(out mTheolizer.mCount);
                }
            }
        }

        //      ---<<< メンバ関数群 >>>---

        public void initialize()
        {
            // インテグレータ獲得
            var aIntegrator = ThreadIntegrator.Integrator;

            // 要求送信と応答受信
            var aFuncObject = new theolizer_integrator.initializeUserClassNotify(this);
            var aReturnObject = new theolizer_integrator.initializeUserClassNotifyReturn();

            aIntegrator.sendRequest(aFuncObject, aReturnObject);
        }
    }
}

// ***************************************************************************
//      C++側自動生成のミラー
// ***************************************************************************

namespace theolizer_integrator
{
    //----------------------------------------------------------------------------
    //      共有処理クラス定義(exchange::UserClassMain)
    //----------------------------------------------------------------------------

    class SharedHelperTheolizer_exchange_UserClassMain : ITheolizerInternal
    {
        int                     mIndex;     // 共有テーブルのインデックス番号
        exchange.UserClassMain  mInstance;  // 交換対象インスタンス

        // インスタンスを共有テーブルへ登録する
        public exchange.UserClassMain Instance
        {
            set
            {
                // インテグレータ獲得
                var aIntegrator = ThreadIntegrator.Integrator;

                // 共有テーブルへ登録
                mInstance = value;
                mIndex = aIntegrator.registerSharedInstanceS<exchange.UserClassMain>(mInstance);

                // SharedDisposeへ登録
                mInstance.setIntegrator(aIntegrator, mIndex);
            }
            get
            {
                return mInstance;
            }
        }

        public static readonly theolizer.internal_space.TypeIndex kTypeIndex2 =
            new theolizer.internal_space.TypeIndex(7u);
        public theolizer.internal_space.TypeIndex getTypeIndex2() { return kTypeIndex2; }

        // シリアライズ
        public void save(BaseSerializer iBaseSerializer)
        {
            using (var temp = new BaseSerializer.AutoRestoreSaveStructure(iBaseSerializer))
            {
                // mIndex保存
                iBaseSerializer.writePreElement();
                iBaseSerializer.savePrimitive(mIndex);

                // mInstance保存
                iBaseSerializer.writePreElement();
                ((ITheolizer)mInstance).getTheolizer().save(iBaseSerializer);
            }
        }

        // デシリアライズ
        public void load(BaseSerializer iBaseSerializer)
        {
            using (var temp = new BaseSerializer.AutoRestoreLoadStructure(iBaseSerializer))
            {
                // mIndex回復
                if (iBaseSerializer.readPreElement() == ReadStat.Terminated)
        throw new InvalidOperationException("Format Error.");
                iBaseSerializer.loadPrimitive(out mIndex);

                // インテグレータ獲得し、共有テーブルからインスタンス取り出し
                var aIntegrator = ThreadIntegrator.Integrator;
                mInstance = aIntegrator.registerSharedInstanceR<exchange.UserClassMain>(mIndex);

                // mInstance回復
                if (iBaseSerializer.readPreElement() == ReadStat.Terminated)
        throw new InvalidOperationException("Format Error.");
                ((ITheolizer)mInstance).getTheolizer().load(iBaseSerializer);
            }
        }
    }

    //----------------------------------------------------------------------------
    //      共有処理クラス定義(exchange::UserClassSub)
    //----------------------------------------------------------------------------

    class SharedHelperTheolizer_exchange_UserClassSub : ITheolizerInternal
    {
        int                     mIndex;     // 共有テーブルのインデックス番号
        exchange.UserClassSub   mInstance;  // 交換対象インスタンス

        // インスタンスを共有テーブルへ登録する
        public exchange.UserClassSub Instance
        {
            set
            {
                // インテグレータ獲得
                var aIntegrator = ThreadIntegrator.Integrator;

                // 共有テーブルへ登録
                mInstance = value;
                mIndex = aIntegrator.registerSharedInstanceS<exchange.UserClassSub>(mInstance);

                // SharedDisposeへ登録
                mInstance.setIntegrator(aIntegrator, mIndex);
            }
            get
            {
                return mInstance;
            }
        }

        public static readonly theolizer.internal_space.TypeIndex kTypeIndex2 =
            new theolizer.internal_space.TypeIndex(9u);
        public theolizer.internal_space.TypeIndex getTypeIndex2() { return kTypeIndex2; }

        // シリアライズ
        public void save(BaseSerializer iBaseSerializer)
        {
            using (var temp = new BaseSerializer.AutoRestoreSaveStructure(iBaseSerializer))
            {
                // mIndex保存
                iBaseSerializer.writePreElement();
                iBaseSerializer.savePrimitive(mIndex);

                // mInstance保存
                iBaseSerializer.writePreElement();
                ((ITheolizer)mInstance).getTheolizer().save(iBaseSerializer);
            }
        }

        // デシリアライズ
        public void load(BaseSerializer iBaseSerializer)
        {
            using (var temp = new BaseSerializer.AutoRestoreLoadStructure(iBaseSerializer))
            {
                // mIndex回復
                if (iBaseSerializer.readPreElement() == ReadStat.Terminated)
        throw new InvalidOperationException("Format Error.");
                iBaseSerializer.loadPrimitive(out mIndex);

                // インテグレータ獲得し、共有テーブルからインスタンス取り出し
                var aIntegrator = ThreadIntegrator.Integrator;
                mInstance = aIntegrator.registerSharedInstanceR<exchange.UserClassSub>(mIndex);

                // mInstance回復
                if (iBaseSerializer.readPreElement() == ReadStat.Terminated)
        throw new InvalidOperationException("Format Error.");
                ((ITheolizer)mInstance).getTheolizer().load(iBaseSerializer);
            }
        }
    }

    //----------------------------------------------------------------------------
    //      共有処理クラス定義(exchange::UserClassNotify)
    //----------------------------------------------------------------------------

    class SharedHelperTheolizer_exchange_UserClassNotify : ITheolizerInternal
    {
        int                         mIndex;     // 共有テーブルのインデックス番号
        exchange.UserClassNotify    mInstance;  // 交換対象インスタンス

        // インスタンスを共有テーブルへ登録する
        public exchange.UserClassNotify Instance
        {
            set
            {
                // インテグレータ獲得
                var aIntegrator = ThreadIntegrator.Integrator;

                // 共有テーブルへ登録
                mInstance = value;
                mIndex = aIntegrator.registerSharedInstanceS<exchange.UserClassNotify>(mInstance);

                // SharedDisposeへ登録
                mInstance.setIntegrator(aIntegrator, mIndex);
            }
            get
            {
                return mInstance;
            }
        }

        public static readonly theolizer.internal_space.TypeIndex kTypeIndex2 =
            new theolizer.internal_space.TypeIndex(5u);
        public theolizer.internal_space.TypeIndex getTypeIndex2() { return kTypeIndex2; }

        // シリアライズ
        public void save(BaseSerializer iBaseSerializer)
        {
            using (var temp = new BaseSerializer.AutoRestoreSaveStructure(iBaseSerializer))
            {
                // mIndex保存
                iBaseSerializer.writePreElement();
                iBaseSerializer.savePrimitive(mIndex);

                // mInstance保存
                iBaseSerializer.writePreElement();
                ((ITheolizer)mInstance).getTheolizer().save(iBaseSerializer);
            }
        }

        // デシリアライズ
        public void load(BaseSerializer iBaseSerializer)
        {
            using (var temp = new BaseSerializer.AutoRestoreLoadStructure(iBaseSerializer))
            {
                // mIndex回復
                if (iBaseSerializer.readPreElement() == ReadStat.Terminated)
        throw new InvalidOperationException("Format Error.");
                iBaseSerializer.loadPrimitive(out mIndex);

                // インテグレータ獲得し、共有テーブルからインスタンス取り出し
                var aIntegrator = ThreadIntegrator.Integrator;
                mInstance = aIntegrator.registerSharedInstanceR<exchange.UserClassNotify>(mIndex);

                // mInstance回復
                if (iBaseSerializer.readPreElement() == ReadStat.Terminated)
        throw new InvalidOperationException("Format Error.");
                ((ITheolizer)mInstance).getTheolizer().load(iBaseSerializer);
            }
        }
    }

    //----------------------------------------------------------------------------
    //      UserClassMain::request用戻り値クラス
    //----------------------------------------------------------------------------

    class requestUserClassMainReturn : ITheolizerInternal
    {
        public Int32 mReturn;
        SharedHelperTheolizer_exchange_UserClassMain mThis =
            new SharedHelperTheolizer_exchange_UserClassMain();
        SharedHelperTheolizer_exchange_UserClassSub mioUserClassSub2 =
            new SharedHelperTheolizer_exchange_UserClassSub();

        public static readonly theolizer.internal_space.TypeIndex kTypeIndex2 =
            new theolizer.internal_space.TypeIndex(3u);
        public theolizer.internal_space.TypeIndex getTypeIndex2() { return kTypeIndex2; }

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
                mThis.load(iBaseSerializer);

                if (iBaseSerializer.readPreElement() == ReadStat.Terminated)
        throw new InvalidOperationException("Format Error.");
                mioUserClassSub2.load(iBaseSerializer);
            }
        }
    }

    //----------------------------------------------------------------------------
    //      UserClassMain::request用関数クラス
    //----------------------------------------------------------------------------

    class requestUserClassMain : ITheolizerInternal
    {
        SharedHelperTheolizer_exchange_UserClassMain mThis =
            new SharedHelperTheolizer_exchange_UserClassMain();
        exchange.UserClassSub miUserClassSub;
        SharedHelperTheolizer_exchange_UserClassSub mioUserClassSub2 =
            new SharedHelperTheolizer_exchange_UserClassSub();

        public requestUserClassMain
        (
            exchange.UserClassMain iThis,
            exchange.UserClassSub iUserClassSub,
            exchange.UserClassSub ioUserClassSub2
        )
        {
            mThis.Instance = iThis;
            miUserClassSub=iUserClassSub;
            mioUserClassSub2.Instance = ioUserClassSub2;
        }

        // TypeIndex
        public static readonly theolizer.internal_space.TypeIndex kTypeIndex2 =
            new theolizer.internal_space.TypeIndex(0u);
        public theolizer.internal_space.TypeIndex getTypeIndex2() { return kTypeIndex2; }

        // シリアライズ
        public void save(BaseSerializer iBaseSerializer)
        {
            using (var temp = new BaseSerializer.AutoRestoreSaveStructure(iBaseSerializer))
            {
                iBaseSerializer.writePreElement();
                mThis.save(iBaseSerializer);

                iBaseSerializer.writePreElement();
                ((ITheolizer)miUserClassSub).getTheolizer().save(iBaseSerializer);

                iBaseSerializer.writePreElement();
                mioUserClassSub2.save(iBaseSerializer);
            }
        }

        // デシリアライズ
        public void load(BaseSerializer iBaseSerializer)
        {
            // 省略
            throw new NotImplementedException();
        }
    }

    //----------------------------------------------------------------------------
    //      UserClassNotify::initialize用戻り値クラス
    //----------------------------------------------------------------------------

    class initializeUserClassNotifyReturn : ITheolizerInternal
    {
        SharedHelperTheolizer_exchange_UserClassNotify mThis =
            new SharedHelperTheolizer_exchange_UserClassNotify();

        public static readonly theolizer.internal_space.TypeIndex kTypeIndex2 =
            new theolizer.internal_space.TypeIndex(4u);
        public theolizer.internal_space.TypeIndex getTypeIndex2() { return kTypeIndex2; }

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
                mThis.load(iBaseSerializer);
            }
        }
    }

    //----------------------------------------------------------------------------
    //      UserClassNotify::initialize用関数クラス
    //----------------------------------------------------------------------------

    class initializeUserClassNotify : ITheolizerInternal
    {
        SharedHelperTheolizer_exchange_UserClassNotify mThis =
            new SharedHelperTheolizer_exchange_UserClassNotify();

        public initializeUserClassNotify
        (
            exchange.UserClassNotify iThis
        )
        {
            mThis.Instance = iThis;
        }

        // TypeIndex
        public static readonly theolizer.internal_space.TypeIndex kTypeIndex2 =
            new theolizer.internal_space.TypeIndex(1u);
        public theolizer.internal_space.TypeIndex getTypeIndex2() { return kTypeIndex2; }

        // シリアライズ
        public void save(BaseSerializer iBaseSerializer)
        {
            using (var temp = new BaseSerializer.AutoRestoreSaveStructure(iBaseSerializer))
            {
                iBaseSerializer.writePreElement();
                mThis.save(iBaseSerializer);
            }
        }

        // デシリアライズ
        public void load(BaseSerializer iBaseSerializer)
        {
            // 省略
            throw new NotImplementedException();
        }
    }

    //----------------------------------------------------------------------------
    //      UserClassNotify::notify用クラス
    //----------------------------------------------------------------------------

    class notifyUserClassNotify : ITheolizerInternal, ICallFunc
    {
        SharedHelperTheolizer_exchange_UserClassNotify mThis =
            new SharedHelperTheolizer_exchange_UserClassNotify();

        public static readonly theolizer.internal_space.TypeIndex kTypeIndex2 =
            new theolizer.internal_space.TypeIndex(2u);
        public theolizer.internal_space.TypeIndex getTypeIndex2() { return kTypeIndex2; }

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
                mThis.load(iBaseSerializer);
            }
        }
        public void callFunc()
        {
            mThis.Instance.notify();
        }
    }

    //----------------------------------------------------------------------------
    //      関数クラス・テーブル
    //----------------------------------------------------------------------------

    static class FuncClassList
    {
        static Dictionary<UInt32,ITheolizerInternal> mFuncClassList;
        static FuncClassList()
        {
            mFuncClassList = new Dictionary<UInt32, ITheolizerInternal>
            {
                {notifyUserClassNotify.kTypeIndex2.getIndex(), new notifyUserClassNotify()}
            };
        }
        public static ITheolizerInternal get(UInt32 iIndex)
        {
            return mFuncClassList[iIndex];
        }
    }
}
