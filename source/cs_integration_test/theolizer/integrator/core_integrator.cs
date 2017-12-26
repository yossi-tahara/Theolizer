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
using System.Threading;
using theolizer.internal_space;

// ***************************************************************************
//      各種ヘルパー
// ***************************************************************************

namespace theolizer
{
    //----------------------------------------------------------------------------
    //      スレッドで使用するインテグレータを管理
    //----------------------------------------------------------------------------

    static class ThreadIntegrator
    {
        static ThreadLocal<IIntegrator> sThreadIntegrator;
        static ThreadIntegrator()
        {
            sThreadIntegrator = new ThreadLocal<IIntegrator>();
        }
        public static IIntegrator Integrator
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

    //----------------------------------------------------------------------------
    //      シリアライザの指定
    //----------------------------------------------------------------------------

    public enum SerializerType
    {
        Binary,             // Binary
        Json                // Json
    }

}   // theolizer

// ***************************************************************************
//      内部用（ユーザプログラムから使用不可）
// ***************************************************************************

namespace theolizer.internal_space
{
    //----------------------------------------------------------------------------
    //      共通インテグレータ
    //----------------------------------------------------------------------------

    interface IIntegrator
    {
        // 要求を発行し応答を受信
        void sendRequest(ITheolizerInternal iFuncObject, ITheolizerInternal oReturnObject);

        // 通知受信
        void receiveNotify();

        // 破棄処理
        void Dispose();
    }

    interface ITheolizerInternal
    {
        UInt64 getTypeIndex();
        void save(BaseSerializer iBaseSerializer);
        void load(BaseSerializer iBaseSerializer);
    }

    //----------------------------------------------------------------------------
    //      共有インスタンス交換用基底クラス
    //----------------------------------------------------------------------------

    abstract class SharedHelperTheolizer<tType> : ITheolizerInternal
    {
        UInt64      mIndex;         // 共有テーブルのインデックス番号
        tType       mInstance;      // 交換対象インスタンス

        abstract public UInt64 getTypeIndex();

        // シリアライズ
        public void save(BaseSerializer iBaseSerializer)
        {
            using (var temp = new BaseSerializer.AutoRestoreSaveStructure(iBaseSerializer))
            {
                iBaseSerializer.writePreElement();
                iBaseSerializer.savePrimitive(mIndex);
                iBaseSerializer.writePreElement();
                ((ITheolizer)mInstance).getTheolizer().save(iBaseSerializer);
            }
        }

        // デシリアライズ
        public void load(BaseSerializer iBaseSerializer)
        {
            using (var temp = new BaseSerializer.AutoRestoreLoadStructure(iBaseSerializer))
            {
            }
        }
    }
}   // theolizer.internal_space
