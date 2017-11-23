//############################################################################
//      Theolizerライブラリのコア部(C#側 基底シリアライザ)
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

namespace theolizer.internal_space
{
    // ***************************************************************************
    //      各種ヘルパー
    // ***************************************************************************

    // 型チェック・モード
    enum CheckMode
    {
        InMemory=0,                 // メモリ内動作のみ、かつ、型チェック無し（非公開）
        TypeCheckInData,            // データ内に型名を記録する（非公開）
        NoTypeCheck,                // 型チェック無し
        TypeCheck,                  // 型名による型チェック
        TypeCheckByIndex,           // TypeIndexによる型チェック
        MetaMode                    // メタ処理－型定義情報を保存／回復（非公開）
    }

    // クラス・メンバの対応方法(データはemOrderのみ使用)
    enum ElementsMapping
    {
        emName=0,                   // フィールド名(=0:完全自動時)
        emOrder                     // 順序
    }

    // シリアライズ・データ読出状態
    enum ReadStat {Terminated=0, Continue, DontCare}

    // ***************************************************************************
    //      基底シリアライザ
    // ***************************************************************************

    abstract class BaseSerializer : IDisposable
    {
        protected uint              mGlobalVersionNo;
        protected ElementsMapping   mElementsMapping;
        protected bool              mCancelPrettyPrint;
        protected int               mIndent;

        //----------------------------------------------------------------------------
        //      コンストラクタ
        //----------------------------------------------------------------------------

        public BaseSerializer(uint iGlobalVersionNo)
        {
            mGlobalVersionNo = iGlobalVersionNo;
            mElementsMapping = ElementsMapping.emName;
            mCancelPrettyPrint = false;
            mIndent = 0;
        }

        //----------------------------------------------------------------------------
        //      破棄処理
        //          非管理リソースを持たないので本来Disposeは不要だが、
        //          基底クラスのDisposeを呼んだ時、これを派生したクラスのDisposeが
        //          呼ばれるようにするため、この構造とする。
        //----------------------------------------------------------------------------

        ~BaseSerializer()
        {
            Dispose(false);
        }

        protected virtual void Dispose(bool disposing)
        {
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        //----------------------------------------------------------------------------
        //      関数クラス（トップ・レベル）の保存開始／終了
        //----------------------------------------------------------------------------

        public sealed class AutoRestoreSaveProcess : IDisposable
        {
            BaseSerializer  mSerializer;
            ElementsMapping mElementsMapping;
            bool            mCancelPrettyPrint;
            int             mIndent;

            //      ---<<< コンストラクタ >>>---

            public AutoRestoreSaveProcess
            (
                BaseSerializer  iSerializer,
                UInt64          iTypeIndex
            )
            {
                mSerializer = iSerializer;
                mIndent = mSerializer.mIndent;

                mElementsMapping=mSerializer.mElementsMapping;
                mSerializer.mElementsMapping=ElementsMapping.emOrder;

                mCancelPrettyPrint=mSerializer.mCancelPrettyPrint;
                mSerializer.mCancelPrettyPrint=true;

                mSerializer.saveProcessStart(iTypeIndex);
            }

            //      ---<<< 破棄処理 >>>---

            public void Dispose()
            {
                mSerializer.mIndent=mIndent;
                mSerializer.saveProcessEnd();
                mSerializer.mCancelPrettyPrint=mCancelPrettyPrint;
                mSerializer.mElementsMapping=mElementsMapping;
            }
        }

        //----------------------------------------------------------------------------
        //      内部構造を持つデータの保存開始／終了
        //----------------------------------------------------------------------------

        public sealed class AutoRestoreSaveStructure : IDisposable
        {
            BaseSerializer  mSerializer;
            ElementsMapping mElementsMapping;
            bool            mCancelPrettyPrint;
            int             mIndent;

            //      ---<<< コンストラクタ >>>---

            public AutoRestoreSaveStructure
            (
                BaseSerializer  iSerializer,
                ElementsMapping iElementsMapping = ElementsMapping.emOrder,
                bool            iCancelPrettyPrint = false
            )
            {
                mSerializer = iSerializer;
                mIndent = mSerializer.mIndent;

                mElementsMapping=mSerializer.mElementsMapping;
                mSerializer.mElementsMapping=iElementsMapping;

                mCancelPrettyPrint=mSerializer.mCancelPrettyPrint;
                mSerializer.mCancelPrettyPrint=iCancelPrettyPrint;

                mSerializer.saveStructureStart();
            }

            //      ---<<< 破棄処理 >>>---

            public void Dispose()
            {
                mSerializer.mIndent=mIndent;
                mSerializer.saveStructureEnd();
                mSerializer.mCancelPrettyPrint=mCancelPrettyPrint;
                mSerializer.mElementsMapping=mElementsMapping;
            }
        }

        //----------------------------------------------------------------------------
        //      派生シリアライザで実装するシリアライズ補助関数群
        //----------------------------------------------------------------------------

        protected virtual void saveProcessStart(UInt64 iTypeIndex)  {throw new NotImplementedException();}
        protected virtual void saveProcessEnd()                     {throw new NotImplementedException();}
        protected virtual void saveStructureStart() {throw new NotImplementedException();}
        protected virtual void saveStructureEnd()   {throw new NotImplementedException();}
        public    virtual void writePreElement()    {throw new NotImplementedException();}
        public    virtual void flush()              {throw new NotImplementedException();}

        protected virtual void saveControl(Int32  iControl) {throw new NotImplementedException();}
        protected virtual void saveControl(Int64  iControl) {throw new NotImplementedException();}
        protected virtual void saveControl(UInt32 iControl) {throw new NotImplementedException();}
        protected virtual void saveControl(UInt64 iControl) {throw new NotImplementedException();}
        protected virtual void saveControl(String iControl) {throw new NotImplementedException();}
        protected virtual void saveElementName(ElementsMapping iElementsMapping, String iElementName)
                                                            {throw new NotImplementedException();}

        //----------------------------------------------------------------------------
        //      プリミティブ保存関数群
        //          DecimalはC++に対応する型がないので非サポート
        //          Objectはクラスにてサポート
        //----------------------------------------------------------------------------

        public    virtual void savePrimitive(Boolean iPrimitive) {throw new NotImplementedException();}
        public    virtual void savePrimitive(Byte    iPrimitive) {throw new NotImplementedException();}
        public    virtual void savePrimitive(SByte   iPrimitive) {throw new NotImplementedException();}
        public    virtual void savePrimitive(Char    iPrimitive) {throw new NotImplementedException();}
        public    virtual void savePrimitive(Int16   iPrimitive) {throw new NotImplementedException();}
        public    virtual void savePrimitive(UInt16  iPrimitive) {throw new NotImplementedException();}
        public    virtual void savePrimitive(Int32   iPrimitive) {throw new NotImplementedException();}
        public    virtual void savePrimitive(UInt32  iPrimitive) {throw new NotImplementedException();}
        public    virtual void savePrimitive(Int64   iPrimitive) {throw new NotImplementedException();}
        public    virtual void savePrimitive(UInt64  iPrimitive) {throw new NotImplementedException();}
        public    virtual void savePrimitive(Single  iPrimitive) {throw new NotImplementedException();}
        public    virtual void savePrimitive(Double  iPrimitive) {throw new NotImplementedException();}
        public    virtual void savePrimitive(String  iPrimitive) {throw new NotImplementedException();}

        //----------------------------------------------------------------------------
        //      ヘッダ内型情報保存
        //----------------------------------------------------------------------------

        protected void writeHeaderTypeInfo()
        {
            using (var temp =
                new BaseSerializer.AutoRestoreSaveStructure(this, ElementsMapping.emOrder, true))
            {
                // 型チェックのモード出力
                writePreElement();
                saveControl((int)CheckMode.NoTypeCheck);
            }
        }
    }
}
