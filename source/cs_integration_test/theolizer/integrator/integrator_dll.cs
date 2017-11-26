//############################################################################
//      Theolizerライブラリの連携処理クラス(C#側dll用)
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
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using theolizer.internal_space;

namespace theolizer
{
    // ***************************************************************************
    //      C++DLL用連携処理統括クラス
    //          DLLの場合、通常１つしかインスタンス不要なのでシングルトンとする
    // ***************************************************************************

    sealed class DllIntegrator : internal_space.IIntegrator, IDisposable
    {
        //----------------------------------------------------------------------------
        //      コンストラクタ
        //          get()のみマルチスレッド対応
        //----------------------------------------------------------------------------

        private static DllIntegrator sInstance;
        public static DllIntegrator getInstance(SerializerType iSerializerType, uint iGlobalVersionNo)
        {
            if (sInstance == null)
            {
                sInstance = new DllIntegrator(iSerializerType, iGlobalVersionNo);
            }
            return sInstance;
        }

        [DllImport(Constants.CppDllName)]
        extern static void CppInitialize(out Streams oStreams);

        private DllIntegrator(SerializerType iSerializerType, uint iGlobalVersionNo)
        {
            CppInitialize(out mStreams);

            mRequestStream = new CppOStream(mStreams.mRequest);
            mResponseStream = new CppIStream(mStreams.mResponse);
             mNotifyStream = new CppIStream(mStreams.mNotify);

            switch (iSerializerType)
            {
            case SerializerType.Binary:
                throw new NotImplementedException();

            case SerializerType.Json:
                mRequestSerializer = new JsonOSerializer(mRequestStream, iGlobalVersionNo);
                mResponseSerializer = new JsonISerializer(mResponseStream);
                break;
            }
        }

        //----------------------------------------------------------------------------
        //      破棄
        //----------------------------------------------------------------------------

        ~DllIntegrator()
        {
            Dispose(false);
        }

        protected void Dispose(bool disposing)
        {
            if (mRequestSerializer != null)     mRequestSerializer.Dispose();
            if (mResponseSerializer != null)    mResponseSerializer.Dispose();
            if (mNotifySerializer != null)      mNotifySerializer.Dispose();
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        //----------------------------------------------------------------------------
        //      ストリーム群
        //----------------------------------------------------------------------------

        [StructLayout(LayoutKind.Sequential)]
        struct Streams
        {
            public IntPtr   mRequest;           // C#->Cpp要求用ストリーム
            public IntPtr   mResponse;          // Cpp->C#応答用ストリーム
            public IntPtr   mNotify;            // Cpp->C#通知用ストリーム
        }

        Streams     mStreams = new Streams();

        // 内部ストリーム
        Stream          mRequestStream;
        Stream          mResponseStream;
        Stream          mNotifyStream;

        //----------------------------------------------------------------------------
        //      シリアライザ
        //----------------------------------------------------------------------------

        BaseSerializer   mRequestSerializer;
        BaseSerializer   mResponseSerializer;
        BaseSerializer   mNotifySerializer;

        // 要求を発行し応答を受信
        public void sendRequest(ITheolizerInternal iFuncObject, ITheolizerInternal oReturnObject)
        {
            // 要求送信
            using (var temp = new BaseSerializer.AutoRestoreSaveProcess
                (mRequestSerializer, iFuncObject.getTypeIndex()))
            {
                iFuncObject.save(mRequestSerializer);
            }
            mRequestSerializer.flush();

            // 応答受信
            UInt64 aTypeIndex = oReturnObject.getTypeIndex();
            using (var temp = new BaseSerializer.AutoRestoreLoadProcess
                (mResponseSerializer, ref aTypeIndex))
            {
                oReturnObject.load(mResponseSerializer);
            }
        }

        // 通知受信
        public void receiveNotify()
        {
            throw new NotImplementedException();
        }

        // 応答シリアライザのグローバル・バージョン番号(デバッグ用)
        public UInt32 GlobalVersionNo
        {
            get { return mResponseSerializer.getGlobalVersionNo(); }
        }
    }
}
