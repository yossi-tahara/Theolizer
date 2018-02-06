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
    //      mutex用lockクラス(C++のstd::lock_guardに当たる)
    // ***************************************************************************

    class Lock : IDisposable
    {
        Mutex   mMutex;
        bool    mDisposed = false;

        public Lock(Mutex iMutex)
        {
            mMutex = iMutex;
            mMutex.WaitOne();
        }

        ~Lock()
        {
            Dispose(false);
        }

        void Dispose(bool disposing)
        {
            // 破棄済なら何もしない
            if (mDisposed)
        return;

            mDisposed = true;

            mMutex.ReleaseMutex();
        }

        public void Dispose()
        {
            Dispose(true);
        }
    }

    // ***************************************************************************
    //      シリアライズ対象用基底クラス
    // ***************************************************************************

    interface ITheolizerInternal
    {
        UInt64 getTypeIndex();
        void save(BaseSerializer iBaseSerializer);
        void load(BaseSerializer iBaseSerializer);
        //void callFunc();
    }
    interface ICallFunc
    {
        void callFunc();
    }

    abstract class SharedDestructor
    {
        CoreIntegrator  mIntegrator = null;
        int             mIndex = -1;

        public void setIntegrator(CoreIntegrator iIntegrator, int iIndex)
        {
            mIntegrator = iIntegrator;
            mIndex      = iIndex;
        }

        ~SharedDestructor()
        {
System.Diagnostics.Debug.WriteLine(this.GetType().Name + ".~SharedDestructor()");
            // 登録済なら破棄できるかどうかチェックして破棄する
            if (mIntegrator != null)
            {
                if (!mIntegrator.canDispose(mIndex))
        throw new InvalidOperationException(
            "Can not dispose SharedDestructor, because this is preserved by C++.");

                mIntegrator.disposeShared(mIndex);
            }
        }
    }

    // ***************************************************************************
    //      共有テーブルのオブジェクト管理クラス
    //          C++側で共有オブジェクトを獲得／保持したが、
    //          それを送って来る前のmWeakRefはnullである。
    // ***************************************************************************

    class SharedHolder
    {
        Object          mStrongRef;
        WeakReference   mWeakRef;
        bool            mUserPresaved;

        // C#側で共有オブジェクトを獲得した際に使用する
        public SharedHolder(Object iInstance)
        {
            mStrongRef = null;
            mWeakRef = new WeakReference(iInstance);
            mUserPresaved = false;
        }

        // C++側で共有オブジェクトを獲得した際に使用する
        public SharedHolder()
        {
            mStrongRef = null;
            mWeakRef = null;
            mUserPresaved = false;
        }

        // C++側で共有オブジェクトを獲得後に送信してきた時に設定する際に使用する
        public void setWeak<tType>() where tType : new()
        {
            if (mWeakRef == null)
            {
                mWeakRef = new WeakReference(new tType());
                if (mUserPresaved)
                {
                    mStrongRef = mWeakRef.Target;
                }
            }
        }

        // C++側で共有オブジェクトを保持しているかどうかを設定する
        //  戻り値：テーブルから破棄して良い時true(iUserPresaved==false、かつ、インスタンス未登録)
        public bool setStrong(bool iUserPresaved)
        {
            mUserPresaved = iUserPresaved;
            if (mUserPresaved)
            {
                if (mWeakRef != null)
                {
                    mStrongRef = mWeakRef.Target;
                }
            }
            else
            {
                mStrongRef = null;
                if (mWeakRef == null)
        return true;
            }
            return false;
        }

        // C++側で共有オブジェクトを保持していたらDispose不可
        public bool canDispose()
        {
            return !mUserPresaved;
        }

        // オブジェクトがあれば返却する
        public Object get()
        {
            if (mWeakRef == null)
        return null;
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
        Mutex               mMutex = new Mutex();

        // 受信時に使用
        //  Indexを受け取り、
        //      登録済ならそのインスタンスを返却
        //      未登録ならデフォルト・コンストラクタで生成して返却
        public tType registerSharedInstanceR<tType>(int iIndex) where tType : SharedDestructor, new()
        {
            using (var aLock = new Lock(mMutex))
            {
                if (mSharedTable.Count <= iIndex)
                {
                    for (int i=mSharedTable.Count; i < iIndex+1; ++i)
                    {
                        mSharedTable.Add(null);
                    }
                }

                if (mSharedTable[iIndex] == null)
                {
                    mSharedTable[iIndex] = new SharedHolder(new tType());
                }
                else
                {
                    mSharedTable[iIndex].setWeak<tType>();
                }
System.Diagnostics.Debug.WriteLine("registerSharedInstanceR<" + mSharedTable[iIndex].get().GetType().Name + "> index=" + iIndex);
                return (tType)mSharedTable[iIndex].get();
            }
        }

        // 送信処理用：指定領域を共有テーブルへ登録。
        //  未登録なら新規登録し、そのIndex返却
        //  登録済ならそのIndex返却。
        public int registerSharedInstanceS<tType>(tType iInstance) where tType : SharedDestructor, new()
        {
            int ret;

            using (var aLock = new Lock(mMutex))
            {
                // 既に登録済チェック
                for (ret=0; ret < mSharedTable.Count; ++ret)
                {
                    if ((mSharedTable[ret] != null)
                     && (mSharedTable[ret].get() == (Object)iInstance))
                    {
System.Diagnostics.Debug.WriteLine("registerSharedInstanceS<" + iInstance.GetType().Name + "> index="+ret);
            return ret;
                    }
                }

                // 登録済でないなら先頭のnullを探す
                for (ret=0; ret < mSharedTable.Count; ++ret)
                {
                    if (mSharedTable[ret] == null)
                    {
                        mSharedTable[ret] = new SharedHolder(iInstance);
System.Diagnostics.Debug.WriteLine("registerSharedInstanceS<" + iInstance.GetType().Name + "> index="+ret);
            return ret;
                    }
                }

                // 新たに領域を増やす
                mSharedTable.Add(new SharedHolder(iInstance));
System.Diagnostics.Debug.WriteLine("registerSharedInstanceS<" + iInstance.GetType().Name + "> index="+ret);
            }
            return ret;
        }

        // 共有オブジェクト状態をC++側から受け取る
        protected void notifySharedObject(int iIndex, bool iUserPresaved)
        {
            using (var aLock = new Lock(mMutex))
            {
System.Diagnostics.Debug.WriteLine("notifySharedObject(" + iIndex + ", " + iUserPresaved + ")");

                if (iUserPresaved)
                {
                    if (mSharedTable.Count <= iIndex)
                    {
                        for (int i=mSharedTable.Count; i < iIndex+1; ++i)
                        {
                            mSharedTable.Add(null);
                        }
                    }

                    if (mSharedTable[iIndex] == null)
                    {
                        mSharedTable[iIndex] = new SharedHolder();
                    }
                }
                if (mSharedTable[iIndex].setStrong(iUserPresaved))
                {
                    disposeShared(iIndex);
                }
            }
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
            //  C++側へ伝達してC++側のオブジェクトをデストラクトして共有テーブルから削除する
            disposeSharedDerived(iIndex);

            // C#側共有テーブル削除
            mSharedTable[iIndex] = null;
        }

        //----------------------------------------------------------------------------
        //      C++側との通信用
        //----------------------------------------------------------------------------

        // 要求を発行し応答を受信
        public abstract void sendRequest(ITheolizerInternal iFuncObject, ITheolizerInternal oReturnObject);

        // 破棄(ThreadIntegrator経由で呼び出す)
        public abstract void Dispose();

        // 共有オブジェクトの破棄
        protected abstract void disposeSharedDerived(int iIndex);
    }
}   // theolizer.internal_space
