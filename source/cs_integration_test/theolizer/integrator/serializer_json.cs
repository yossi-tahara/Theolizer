//############################################################################
//      Jsonシリアライザ C#側
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
using System.IO;
using System.Text;

namespace theolizer.internal_space
{
//############################################################################
//      シリアライザ用定数
//############################################################################

    static partial class Constants
    {
        public static String kJsonSerializerName = "JsonTheolizer";
    }

//############################################################################
//      シリアライザ
//############################################################################

    // ***************************************************************************
    //      本体
    // ***************************************************************************

    class JsonOSerializer : BaseSerializer
    {
        Stream          mStream;
        StreamWriter    mOStream;
        bool            mWriteComma;

        //----------------------------------------------------------------------------
        //      コンストラクタ
        //----------------------------------------------------------------------------

        public JsonOSerializer(Stream iStream, uint iGlobalVersionNo) : base(iGlobalVersionNo)
        {
            if (!iStream.CanWrite)
            {
        throw new ArgumentException(iStream.ToString() + " can not Write.");
            }

            mStream = iStream;
            mOStream = new StreamWriter(mStream, new UTF8Encoding(false));
            mWriteComma = false;

            // 通常ヘッダ保存
            writeHeader();
            mOStream.Write("\n");
            mWriteComma=false;
        }

        //----------------------------------------------------------------------------
        //      破棄処理
        //----------------------------------------------------------------------------

        bool Disposed = false;
        protected override void Dispose(bool disposing)
        {
            if (Disposed)
        return;
            Disposed = true;

            // マネージド・リソースの開放(デストラクタから呼ばれた時はGCに任せる)
            if (disposing)
            {
                mOStream.Dispose();
                mStream.Dispose();
            }

            base.Dispose(disposing);
        }

        //----------------------------------------------------------------------------
        //      シリアライズ補助関数群
        //----------------------------------------------------------------------------

        // 構造出力開始
        protected override void saveStructureStart()
        {
            mWriteComma=false;
            if (!mCancelPrettyPrint) mIndent++;
            switch (mElementsMapping)
            {
            case ElementsMapping.emName:
                mOStream.Write("{");
                break;

            case ElementsMapping.emOrder:
                mOStream.Write("[");
                break;
            }
        }

        // 構造出力終了
        protected override void saveStructureEnd()
        {
            mWriteComma=false;
            writeCommaIndent();
            switch (mElementsMapping)
            {
            case ElementsMapping.emName:
                mOStream.Write("}");
                break;

            case ElementsMapping.emOrder:
                mOStream.Write("]");
                break;
            }
        }

        // 次の要素出力準備
        public    override void writePreElement()
        {
            writeCommaIndent(mWriteComma);
        }

        void writeCommaIndent(bool iWriteComma = false)
        {
            if (iWriteComma)
            {
                mOStream.Write(",");
            }

            if (!mCancelPrettyPrint)
            {
                mOStream.Write("\n");
                for (int i=0; i < mIndent; ++i)
                    mOStream.Write("    ");
            }

            mWriteComma=true;
        }

        // flush
        public    override void flush()
        {
            mOStream.Write("\n");
            mOStream.Flush();
        }

        // 各種制御コード出力
        protected override void saveControl(Int32  iControl)    {savePrimitive(iControl);}
        protected override void saveControl(Int64  iControl)    {savePrimitive(iControl);}
        protected override void saveControl(UInt32 iControl)    {savePrimitive(iControl);}
        protected override void saveControl(UInt64 iControl)    {savePrimitive(iControl);}
        protected override void saveControl(String iControl)    {encodeJsonString(iControl);}
        protected override void saveElementName(ElementsMapping iElementsMapping, String iElementName)
        {
            if (iElementsMapping == ElementsMapping.emName)
            {
                encodeJsonString(iElementName);
                mOStream.Write(":");
            }
        }

        //----------------------------------------------------------------------------
        //      プリミティブ保存関数群
        //----------------------------------------------------------------------------

        public    override void savePrimitive(Boolean iPrimitive)
        {
            mOStream.Write((iPrimitive)?1:0);
        }
        public    override void savePrimitive(Byte    iPrimitive) {mOStream.Write(iPrimitive);}
        public    override void savePrimitive(SByte   iPrimitive) {mOStream.Write(iPrimitive);}
        public    override void savePrimitive(Char    iPrimitive) {mOStream.Write(iPrimitive);}
        public    override void savePrimitive(Int16   iPrimitive) {mOStream.Write(iPrimitive);}
        public    override void savePrimitive(UInt16  iPrimitive) {mOStream.Write(iPrimitive);}
        public    override void savePrimitive(Int32   iPrimitive) {mOStream.Write(iPrimitive);}
        public    override void savePrimitive(UInt32  iPrimitive) {mOStream.Write(iPrimitive);}
        public    override void savePrimitive(Int64   iPrimitive) {mOStream.Write(iPrimitive);}
        public    override void savePrimitive(UInt64  iPrimitive) {mOStream.Write(iPrimitive);}
        public    override void savePrimitive(Single  iPrimitive)
        {
            mOStream.Write(iPrimitive.ToString("R"));
        }
        public    override void savePrimitive(Double  iPrimitive)
        {
            mOStream.Write(iPrimitive.ToString("R"));
        }
        public    override void savePrimitive(String  iPrimitive)
        {
            encodeJsonString(iPrimitive);
        }

        // Json文字列へエンコード
        void encodeJsonString(string iString)
        {
            mOStream.Write("\"");
            foreach (var ch in iString)
            {
                switch(ch)
                {
                case '\"':      mOStream.Write("\\\""); break;
                case '\\':      mOStream.Write("\\\\"); break;
                case '/':       mOStream.Write("\\/");  break;
                case '\x08':    mOStream.Write("\\b");  break;
                case '\x0C':    mOStream.Write("\\f");  break;
                case '\n':      mOStream.Write("\\n");  break;
                case '\r':      mOStream.Write("\\r");  break;
                case '\t':      mOStream.Write("\\t");  break;
                default:        mOStream.Write(ch);     break;
                }
            }
            mOStream.Write("\"");
        }

        //----------------------------------------------------------------------------
        //      ヘッダ保存
        //----------------------------------------------------------------------------

        void writeHeader()
        {
            using (var temp = new BaseSerializer.AutoRestoreSaveStructure(this, ElementsMapping.emName))
            {
                // シリアライザ名出力
                writePreElement();
                saveElementName(ElementsMapping.emName, "SerialzierName");
                saveControl(Constants.kJsonSerializerName);

                // グローバル・バージョン番号出力
                writePreElement();
                saveElementName(ElementsMapping.emName, "GlobalVersionNo");
                saveControl(mGlobalVersionNo);

                // 型情報出力
                writePreElement();
                saveElementName(ElementsMapping.emName, "TypeInfoList");
                writeHeaderTypeInfo();
            }
        }
    }

    // ***************************************************************************
    //      定義
    //          現時点ではNoCheckType用。将来的にメタ・シリアライズ情報交換用
    // ***************************************************************************
#if false
{
    "SerialzierName":"JsonTheolizer",
    "GlobalVersionNo":1,
    "TypeInfoList":[2]
}
#endif

    //############################################################################
    //      デシリアライザ
    //############################################################################

    class JsonISerializer : BaseSerializer
    {

        Stream          mStream;
        StreamReader    mStreamReader;

        //----------------------------------------------------------------------------
        //      コンストラクタ
        //----------------------------------------------------------------------------

        public JsonISerializer(Stream iStream) : base(0)
        {
            if (!iStream.CanRead)
            {
        throw new ArgumentException(iStream.ToString() + " can not Read.");
            }

            mStream = iStream;
            mStreamReader = new StreamReader(mStream, new UTF8Encoding(false));
        }

        //----------------------------------------------------------------------------
        //      破棄処理
        //----------------------------------------------------------------------------

        bool Disposed = false;
        protected override void Dispose(bool disposing)
        {
            if (Disposed)
        return;
            Disposed = true;

            // マネージド・リソースの開放(デストラクタから呼ばれた時はGCに任せる)
            if (disposing)
            {
                mStreamReader.Dispose();
                mStream.Dispose();
            }
            base.Dispose(disposing);
        }
    }
}
