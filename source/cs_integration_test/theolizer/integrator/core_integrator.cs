//############################################################################
//      Theolizerライブラリの連携処理クラス(C#側コア部)
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
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using theolizer.internal_space;

//############################################################################
//      各種ヘルパー
//############################################################################

namespace theolizer
{
    // ***************************************************************************
    //      スレッドで使用するインテグレータを管理
    // ***************************************************************************

    static class ThreadIntegrator
    {
        static ThreadLocal<CoreIntegrator> sThreadIntegrator;
        static ThreadIntegrator()
        {
            sThreadIntegrator = new ThreadLocal<CoreIntegrator>();
        }
        public static CoreIntegrator Integrator
        {
            get { return sThreadIntegrator.Value; }
            set
            {
                if (sThreadIntegrator.Value != value)
                {
                    if (sThreadIntegrator.Value != null)  sThreadIntegrator.Value.Dispose();
                    sThreadIntegrator.Value = value;
                }
            }
        }
    }

    // ***************************************************************************
    //      シリアライザの指定
    //          C++/C#共通
    // ***************************************************************************

    public enum SerializerType
    {
        Binary=1,           // Binary
        Json  =2            // Json
    }

}   // theolizer

//############################################################################
//      内部用（ユーザプログラムから使用不可）
//############################################################################

namespace theolizer.internal_space
{
    // ***************************************************************************
    //      シリアライズ対象用基底クラス
    // ***************************************************************************

    interface ITheolizerInternal
    {
        UInt64 getTypeIndex();
        void save(BaseSerializer iBaseSerializer);
        void load(BaseSerializer iBaseSerializer);
    }

    abstract class SharedDisposer : IDisposable
    {
        CoreIntegrator  mIntegrator = null;
        int             mIndex = -1;
        bool            mDisposed = false;

        public void setIntegrator(CoreIntegrator iIntegrator, int iIndex)
        {
            // 破棄済なら例外
            if (mDisposed)
        throw new InvalidOperationException("SharedDisposer was disposed.");

            mIntegrator = iIntegrator;
            mIndex      = iIndex;
        }

        ~SharedDisposer()
        {
            Dispose(false);
        }

        void Dispose(bool disposing)
        {
            // 破棄済なら何もしない
            if (mDisposed)
        return;

            // 登録済でDispose処理なら破棄できるかどうかチェックする
            if ((mIntegrator != null) && disposing)
            {
                if (!mIntegrator.canDispose(mIndex))
        throw new InvalidOperationException(
            "Can not dispose SharedDisposer, because this is preserved by C++.");
            }

            mDisposed = true;

            if (mIntegrator != null)
            {
                mIntegrator.disposeShared(mIndex);
            }
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
    }

    // ***************************************************************************
    //      共有テーブルのオブジェクト管理クラス
    // ***************************************************************************

    class SharedHolder
    {
        Object          mStrongRef;
        WeakReference   mWeakRef;

        public SharedHolder(Object iInstance)
        {
            mStrongRef = null;
            mWeakRef = new WeakReference(iInstance);
        }

        public void setStrong(bool iIsStrong)
        {
            if (iIsStrong)
            {
                mStrongRef = mWeakRef.Target;
            }
            else
            {
                mStrongRef = null;
            }
        }

        public bool canDispose()
        {
            return (mStrongRef == null);
        }

        public Object get()
        {
            return mWeakRef.Target;
        }
    }

    // ***************************************************************************
    //      共通インテグレータ
    // ***************************************************************************

    abstract class CoreIntegrator
    {
        public abstract UInt32 GlobalVersionNo { get; }

        //----------------------------------------------------------------------------
        //      共有テーブル管理
        //----------------------------------------------------------------------------

        List<SharedHolder>  mSharedTable = new List<SharedHolder>();

        // 受信時に使用
        //  Indexを受け取り、
        //      登録済ならそのインスタンスを返却
        //      未登録ならデフォルト・コンストラクタで生成して返却
        public tType registerSharedInstanceR<tType>(int iIndex) where tType : SharedDisposer, new()
        {
            if (mSharedTable.Count <= iIndex)
            {
                mSharedTable.Capacity = iIndex+1;
            }

            if (mSharedTable[iIndex] == null)
            {
                mSharedTable[iIndex] = new SharedHolder(new tType());
            }

            return (tType)mSharedTable[iIndex].get();
        }

        // 送信処理用：指定領域を共有テーブルへ登録。
        //  未登録なら新規登録し、そのIndex返却
        //  登録済ならそのIndex返却。
        public int registerSharedInstanceS<tType>(tType iInstance) where tType : SharedDisposer, new()
        {
            int ret;

            // 既に登録済チェック
            for (ret=0; ret < mSharedTable.Count; ++ret)
            {
                if ((mSharedTable[ret] != null)
                 && (mSharedTable[ret].get() == (Object)iInstance))
                {
        return ret;
                }
            }

            // 登録済でないなら先頭のnullを探す
            for (ret=0; ret < mSharedTable.Count; ++ret)
            {
                if (mSharedTable[ret] == null)
                {
                    mSharedTable[ret] = new SharedHolder(iInstance);
        return ret;
                }
            }

            // 新たに領域を増やす
            mSharedTable.Add(new SharedHolder(iInstance));
            return ret;
        }

        // Dispose可能判定
        public bool canDispose(int iIndex)
        {
            if (mSharedTable.Count <= iIndex)
        throw new InvalidOperationException(
            String.Format("mSharedTable[{0}] has no element.(0)", iIndex));

            if (mSharedTable[iIndex] == null)
        throw new InvalidOperationException(
            String.Format("mSharedTable[{0}] has no element.(1)", iIndex));

            return mSharedTable[iIndex].canDispose();
        }

        // Dispose処理
        public void disposeShared(int iIndex)
        {
            // ここには、下記処理を記述する。
            //  C++側へ伝達してC++側のオブジェクトをデストラクトして共有テーブルから削除し、

            // C#側共有テーブル削除
            mSharedTable[iIndex] = null;
        }

        //----------------------------------------------------------------------------
        //      C++側との通信用
        //----------------------------------------------------------------------------

        // 要求を発行し応答を受信
        public abstract void sendRequest(ITheolizerInternal iFuncObject, ITheolizerInternal oReturnObject);

        // 通知受信
        public abstract void receiveNotify();

        // 破棄(ThreadIntegrator経由で呼び出す)
        public abstract void Dispose();
    }
}   // theolizer.internal_space
