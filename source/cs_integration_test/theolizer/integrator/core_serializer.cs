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

    // 定数
    static partial class Constants
    {
        public static UInt64 kInvalidSize = 18446744073709551615;
    }

    // ***************************************************************************
    //      基底シリアライザ
    // ***************************************************************************

    abstract class BaseSerializer : IDisposable
    {
        protected UInt32            mGlobalVersionNo;
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

        // ***************************************************************************
        //      シリアライズ機能群
        // ***************************************************************************

        public UInt32 getGlobalVersionNo() {return mGlobalVersionNo;}

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
                if (mSerializer != null)
                {
                    mSerializer.mIndent=mIndent;
                    mSerializer.saveProcessEnd();
                    mSerializer.mCancelPrettyPrint=mCancelPrettyPrint;
                    mSerializer.mElementsMapping=mElementsMapping;
                    mSerializer = null;
                }
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

                mSerializer.saveGroupStart();
            }

            //      ---<<< 破棄処理 >>>---

            public void Dispose()
            {
                if (mSerializer != null)
                {
                    mSerializer.mIndent=mIndent;
                    mSerializer.saveGroupEnd();
                    mSerializer.mCancelPrettyPrint=mCancelPrettyPrint;
                    mSerializer.mElementsMapping=mElementsMapping;
                    mSerializer = null;
                }
            }
        }

        //----------------------------------------------------------------------------
        //      派生シリアライザで実装するシリアライズ補助関数群
        //----------------------------------------------------------------------------

        public    virtual void writePreElement()                    {throw new NotSupportedException();}
        protected virtual void saveGroupStart(bool iIsTop=false)    {throw new NotSupportedException();}
        protected virtual void saveGroupEnd(bool iIsTop=false)      {throw new NotSupportedException();}
        protected virtual void saveStructureStart()                 {throw new NotSupportedException();}
        protected virtual void saveStructureEnd()                   {throw new NotSupportedException();}
        public    virtual void flush()                              {throw new NotSupportedException();}

        protected virtual void saveControl(Int32  iControl)         {throw new NotSupportedException();}
        protected virtual void saveControl(Int64  iControl)         {throw new NotSupportedException();}
        protected virtual void saveControl(UInt32 iControl)         {throw new NotSupportedException();}
        protected virtual void saveControl(UInt64 iControl)         {throw new NotSupportedException();}
        protected virtual void saveControl(String iControl)         {throw new NotSupportedException();}
        protected virtual void saveElementName(ElementsMapping iElementsMapping, String iElementName)
                                                                    {throw new NotSupportedException();}

        //----------------------------------------------------------------------------
        //      プリミティブ保存関数群
        //          DecimalはC++に対応する型がないので非サポート
        //          Objectはクラスにてサポート
        //----------------------------------------------------------------------------

        public    virtual void savePrimitive(Boolean iPrimitive)    {throw new NotSupportedException();}
        public    virtual void savePrimitive(Byte    iPrimitive)    {throw new NotSupportedException();}
        public    virtual void savePrimitive(SByte   iPrimitive)    {throw new NotSupportedException();}
        public    virtual void savePrimitive(Char    iPrimitive)    {throw new NotSupportedException();}
        public    virtual void savePrimitive(Int16   iPrimitive)    {throw new NotSupportedException();}
        public    virtual void savePrimitive(UInt16  iPrimitive)    {throw new NotSupportedException();}
        public    virtual void savePrimitive(Int32   iPrimitive)    {throw new NotSupportedException();}
        public    virtual void savePrimitive(UInt32  iPrimitive)    {throw new NotSupportedException();}
        public    virtual void savePrimitive(Int64   iPrimitive)    {throw new NotSupportedException();}
        public    virtual void savePrimitive(UInt64  iPrimitive)    {throw new NotSupportedException();}
        public    virtual void savePrimitive(Single  iPrimitive)    {throw new NotSupportedException();}
        public    virtual void savePrimitive(Double  iPrimitive)    {throw new NotSupportedException();}
        public    virtual void savePrimitive(String  iPrimitive)    {throw new NotSupportedException();}

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

        void saveProcessStart(UInt64 iTypeIndex)
        {
            saveGroupStart();

            writePreElement();
            saveControl(iTypeIndex);

            writePreElement();
        }

        void saveProcessEnd()
        {
            saveGroupEnd();
        }

        // ***************************************************************************
        //      デシリアライズ機能群
        // ***************************************************************************

        //----------------------------------------------------------------------------
        //      関数クラス（トップ・レベル）の回復開始／終了
        //----------------------------------------------------------------------------

        public sealed class AutoRestoreLoadProcess : IDisposable
        {
            BaseSerializer  mSerializer;
            ElementsMapping mElementsMapping;

            //      ---<<< コンストラクタ >>>---

            // iTypeIndexがkInvalidSizeの時は回復したTypeIndexを返却する
            public AutoRestoreLoadProcess
            (
                BaseSerializer  iSerializer,
                ref UInt64      iTypeIndex
            )
            {
                mSerializer = iSerializer;

                mElementsMapping=mSerializer.mElementsMapping;
                mSerializer.mElementsMapping=ElementsMapping.emOrder;

                mSerializer.loadProcessStart(ref iTypeIndex);
            }

            //      ---<<< 破棄処理 >>>---

            public void Dispose()
            {
                if (mSerializer != null)
                {
                    mSerializer.loadProcessEnd();
                    mSerializer.mElementsMapping=mElementsMapping;
                    mSerializer = null;
                }
            }
        }

        //----------------------------------------------------------------------------
        //      内部構造を持つデータの回復開始／終了
        //----------------------------------------------------------------------------

        public sealed class AutoRestoreLoadStructure : IDisposable
        {
            BaseSerializer  mSerializer;
            ElementsMapping mElementsMapping;

            //      ---<<< コンストラクタ >>>---

            public AutoRestoreLoadStructure
            (
                BaseSerializer  iSerializer,
                ElementsMapping iElementsMapping = ElementsMapping.emOrder
            )
            {
                mSerializer = iSerializer;

                mElementsMapping=mSerializer.mElementsMapping;
                mSerializer.mElementsMapping=iElementsMapping;

                mSerializer.loadStructureStart();
            }

            //      ---<<< 破棄処理 >>>---

            public void Dispose()
            {
                if (mSerializer != null)
                {
                    mSerializer.loadStructureEnd();
                    mSerializer.mElementsMapping=mElementsMapping;
                    mSerializer = null;
                }
            }
        }

        //----------------------------------------------------------------------------
        //      派生シリアライザで実装するデシリアライズ補助関数群
        //----------------------------------------------------------------------------

        public    virtual ReadStat readPreElement()                 {throw new NotSupportedException();}
        protected virtual void loadGroupStart(bool iIsTop=false)    {throw new NotSupportedException();}
        protected virtual void loadGroupEnd(bool iIsTop=false)      {throw new NotSupportedException();}
        protected virtual void loadStructureStart()                 {throw new NotSupportedException();}
        protected virtual void loadStructureEnd()                   {throw new NotSupportedException();}

        protected virtual void loadControl(out Int32  iControl)     {throw new NotSupportedException();}
        protected virtual void loadControl(out Int64  iControl)     {throw new NotSupportedException();}
        protected virtual void loadControl(out UInt32 iControl)     {throw new NotSupportedException();}
        protected virtual void loadControl(out UInt64 iControl)     {throw new NotSupportedException();}
        protected virtual void loadControl(out String iControl)     {throw new NotSupportedException();}
        protected virtual String loadElementName(ElementsMapping iElementsMapping)
                                                                    {throw new NotSupportedException();}

        //----------------------------------------------------------------------------
        //      プリミティブ回復関数群
        //          DecimalはC++に対応する型がないので非サポート
        //          Objectはクラスにてサポート
        //----------------------------------------------------------------------------

        public virtual void loadPrimitive(out Boolean iPrimitive)   {throw new NotSupportedException();}
        public virtual void loadPrimitive(out Byte    iPrimitive)   {throw new NotSupportedException();}
        public virtual void loadPrimitive(out SByte   iPrimitive)   {throw new NotSupportedException();}
        public virtual void loadPrimitive(out Char    iPrimitive)   {throw new NotSupportedException();}
        public virtual void loadPrimitive(out Int16   iPrimitive)   {throw new NotSupportedException();}
        public virtual void loadPrimitive(out UInt16  iPrimitive)   {throw new NotSupportedException();}
        public virtual void loadPrimitive(out Int32   iPrimitive)   {throw new NotSupportedException();}
        public virtual void loadPrimitive(out UInt32  iPrimitive)   {throw new NotSupportedException();}
        public virtual void loadPrimitive(out Int64   iPrimitive)   {throw new NotSupportedException();}
        public virtual void loadPrimitive(out UInt64  iPrimitive)   {throw new NotSupportedException();}
        public virtual void loadPrimitive(out Single  iPrimitive)   {throw new NotSupportedException();}
        public virtual void loadPrimitive(out Double  iPrimitive)   {throw new NotSupportedException();}
        public virtual void loadPrimitive(out String  iPrimitive)   {throw new NotSupportedException();}

        //----------------------------------------------------------------------------
        //      ヘッダ内型情報回復
        //----------------------------------------------------------------------------

        protected void readHeaderTypeInfo()
        {
            using (var temp =
                new BaseSerializer.AutoRestoreLoadStructure(this, ElementsMapping.emOrder))
            {
                // 型チェックのモード確認
                readPreElement();
                int aCheckMode;
                loadControl(out aCheckMode);
                if (aCheckMode != (int)CheckMode.NoTypeCheck)
            throw new InvalidOperationException("CheckMode Error : " + aCheckMode);
            }
        }

        void loadProcessStart(ref UInt64 iTypeIndex)
        {
            loadGroupStart(true);

            if (readPreElement() == ReadStat.Terminated)
            {
        throw new InvalidOperationException("Format Error.");
            }

            UInt64 aTypeIndex;
            loadControl(out aTypeIndex);
            if (iTypeIndex != Constants.kInvalidSize)
            {
#if false   // ヘッダ処理を実装するまでの仮実装(仮なのでかなりいい加減だがデバッグには使える)
                if (!isMatchTypeIndex(aTypeIndex, iTypeIndex))
                {
        throw new InvalidOperationException("Unmatch type.");
                }
#endif
            }
            else
            {
#if true    // ヘッダ処理を実装するまでの仮実装(仮なのでかなりいい加減だがデバッグには使える)
                iTypeIndex = aTypeIndex;
#else
                auto& aElementType = mSerializedTypeListI->at(aTypeIndex);
                ret = aElementType.mProgramTypeIndex;
#endif
            }

            if (readPreElement() == ReadStat.Terminated)
            {
        throw new InvalidOperationException("Format Error.");
            }
        }

        void loadProcessEnd()
        {
            loadGroupEnd(true);
        }
    }
}
