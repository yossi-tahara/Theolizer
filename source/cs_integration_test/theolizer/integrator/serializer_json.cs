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
            mOStream.Flush();
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

        // クラス出力開始
        protected override void saveGroupStart(bool iIsTop=false)
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

        // クラス出力終了
        protected override void saveGroupEnd(bool iIsTop=false)
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
            if ((iIsTop) && (mIndent == 0))
                mOStream.Write("\n");
        }

        // 構造出力開始
        protected override void saveStructureStart()
        {
            saveGroupStart();
        }

        // 次の要素出力準備
        public    override void writePreElement()
        {
            writeCommaIndent(mWriteComma);
        }

        // 構造出力終了
        protected override void saveStructureEnd()
        {
            saveGroupEnd();
        }

        // カンマとインデント出力
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

//############################################################################
//      デシリアライザ
//############################################################################

    // ***************************************************************************
    //      StreamReader拡張
    //          数値を読めるようにする
    //          デリミタは、" ,\n]}\t"
    // ***************************************************************************

    class StreamReaderEx : StreamReader
    {
        public StreamReaderEx(Stream iStream, Encoding iEncoding) : base(iStream, iEncoding) { }

        // デリミタまでの文字列を読み出す
        const String sDelimiter = " ,\n]}\t";
        String ReadDelim()
        {
            var sb = new StringBuilder();
            int ch;
            while((ch = Peek()) > -1)
            {
                if (sDelimiter.IndexOf((char)ch) > -1)
            break;
                sb.Append((char)ch);

                // 読み捨て
                Read();
            }
            return sb.ToString();
        }

        public void Read(out Boolean iPrimitive) {iPrimitive=(int.Parse(ReadDelim())!=0)?true:false;}
        public void Read(out Byte    iPrimitive) {iPrimitive=Byte.Parse(ReadDelim());}
        public void Read(out SByte   iPrimitive) {iPrimitive=SByte.Parse(ReadDelim());}
        public void Read(out Char    iPrimitive) {iPrimitive=(char)(int.Parse(ReadDelim()));}
        public void Read(out Int16   iPrimitive) {iPrimitive=Int16.Parse(ReadDelim());}
        public void Read(out UInt16  iPrimitive) {iPrimitive=UInt16.Parse(ReadDelim());}
        public void Read(out Int32   iPrimitive) {iPrimitive=Int32.Parse(ReadDelim());}
        public void Read(out UInt32  iPrimitive) {iPrimitive=UInt32.Parse(ReadDelim());}
        public void Read(out Int64   iPrimitive) {iPrimitive=Int64.Parse(ReadDelim());}
        public void Read(out UInt64  iPrimitive) {iPrimitive=UInt64.Parse(ReadDelim());}
        public void Read(out Single  iPrimitive) {iPrimitive=Single.Parse(ReadDelim());}
        public void Read(out Double  iPrimitive) {iPrimitive=Double.Parse(ReadDelim());}
    }

    // ***************************************************************************
    //      本体
    // ***************************************************************************

    class JsonISerializer : BaseSerializer
    {

        Stream          mStream;
        StreamReaderEx  mIStream;
        bool            mReadComma;
        bool            mTerminated;

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
            mIStream = new StreamReaderEx(mStream, new UTF8Encoding(false));
            mReadComma = false;
            mTerminated = false;

            // 通常ヘッダ回復
            readHeader();
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
                mIStream.Dispose();
                mStream.Dispose();
            }
            base.Dispose(disposing);
        }

        //----------------------------------------------------------------------------
        //      デシリアライズ補助関数群
        //----------------------------------------------------------------------------

        // クラス入力開始
        protected override void loadGroupStart(bool iIsTop=false)
        {
            mReadComma=false;
            char ch = getValidChar();
            switch (mElementsMapping)
            {
            case ElementsMapping.emName:
                if (ch != '{')
                {
        throw new InvalidOperationException("Format Error.");
                }
                break;

            case ElementsMapping.emOrder:
                if (ch != '[')
                {
        throw new InvalidOperationException("Format Error.");
                }
                break;
            }
        }

        // クラス入力終了
        protected override void loadGroupEnd(bool iIsTop=false)
        {
            // まだ終了処理されてないなら、エラー(C#側は変更対処しない)
            if (!mTerminated)
            {
                if (readPreElement() != ReadStat.Terminated)
        throw new InvalidOperationException("Format Error.");
            }
            mTerminated=false;
            mReadComma=true;
        }

        // 構造入力開始
        protected override void loadStructureStart()
        {
            loadGroupStart();
        }

        // 構造入力終了
        protected override void loadStructureEnd()
        {
            loadGroupEnd();
        }

        // 次の読み出し準備
        protected override ReadStat readPreElement()
        {
            bool aContinue=readComma(mReadComma);
            mReadComma=true;

            return (aContinue)?ReadStat.Continue:ReadStat.Terminated;
        }

        // カンマまで読み飛ばし(C++と異なりungetがないのでPeek→読み捨てで処理する)
        bool readComma(bool iReadComma)
        {
            int     ch;
            while((ch = mIStream.Peek()) > -1)
            {
                if (sSpaceChar.IndexOf((char)ch) < 0)
            break;

                // 読み捨て
                mIStream.Read();
            }
            if (ch < 0)
        throw new InvalidOperationException("Format Error.");

            if (ch == ',')
            {
                if (iReadComma)
                {
                    // 読み捨て
                    mIStream.Read();
        return true;
                }
                else
                {
        throw new InvalidOperationException("Format Error.");
                }
            }

            // 終端マークなら、false返却
            if (checkTerminal((char)ch))
            {
                // 読み捨て
                mIStream.Read();
        return false;
            }

            return true;
        }

        // 終了マーク確認
        bool checkTerminal(char iCh)
        {
            switch (mElementsMapping)
            {
            case ElementsMapping.emName:
                if (iCh != '}')
        return false;
                break;

            case ElementsMapping.emOrder:
                if (iCh != ']')
        return false;
                break;
            }
            mTerminated=true;

            return true;
        }

        // 各種制御コード入力
        protected override void loadControl(out Int32  iControl)    {loadPrimitive(out iControl);}
        protected override void loadControl(out Int64  iControl)    {loadPrimitive(out iControl);}
        protected override void loadControl(out UInt32 iControl)    {loadPrimitive(out iControl);}
        protected override void loadControl(out UInt64 iControl)    {loadPrimitive(out iControl);}
        protected override void loadControl(out String iControl)    {decodeJsonString(out iControl);}
        protected override String loadElementName(ElementsMapping iElementsMapping)
        {
            String aElementName;
            decodeJsonString(out aElementName);
            char ch = getValidChar();
            if (ch != ':')
            {
        throw new InvalidOperationException("Format Error.");
            }
            return aElementName;
        }

        // 有効な文字入力(スペース等を読み飛ばす)
        const String sSpaceChar = " ,\t\n";
        char getValidChar()
        {
            int     ch;
            while((ch = mIStream.Read()) > -1)
            {
                if (sSpaceChar.IndexOf((char)ch) < 0)
        return (char)ch;
            }

            throw new InvalidOperationException("EOF occured.");
        }

        //----------------------------------------------------------------------------
        //      プリミティブ保存関数群
        //----------------------------------------------------------------------------

        public    override void loadPrimitive(out Boolean iPrimitive) {mIStream.Read(out iPrimitive);}
        public    override void loadPrimitive(out Byte    iPrimitive) {mIStream.Read(out iPrimitive);}
        public    override void loadPrimitive(out SByte   iPrimitive) {mIStream.Read(out iPrimitive);}
        public    override void loadPrimitive(out Char    iPrimitive) {mIStream.Read(out iPrimitive);}
        public    override void loadPrimitive(out Int16   iPrimitive) {mIStream.Read(out iPrimitive);}
        public    override void loadPrimitive(out UInt16  iPrimitive) {mIStream.Read(out iPrimitive);}
        public    override void loadPrimitive(out Int32   iPrimitive) {mIStream.Read(out iPrimitive);}
        public    override void loadPrimitive(out UInt32  iPrimitive) {mIStream.Read(out iPrimitive);}
        public    override void loadPrimitive(out Int64   iPrimitive) {mIStream.Read(out iPrimitive);}
        public    override void loadPrimitive(out UInt64  iPrimitive) {mIStream.Read(out iPrimitive);}
        public    override void loadPrimitive(out Single  iPrimitive) {mIStream.Read(out iPrimitive);}
        public    override void loadPrimitive(out Double iPrimitive)  {mIStream.Read(out iPrimitive);}
        public    override void loadPrimitive(out String  iPrimitive)
        {
            decodeJsonString(out iPrimitive);
        }

        // Json文字列へデコード
        void decodeJsonString(out string iString)
        {
            //      ---<<< "までスキップ >>>---

            char ch = getValidChar();
            if (ch != '\"')
        throw new InvalidOperationException("Json String Format Error.");

            //      ---<<< "の直前までを追加する >>>---

            var sb = new StringBuilder();
            while(true)
            {
                ch = getValidChar();

                // Escape文字
                if (ch == '\\')
                {
                    ch=getValidChar();
                    switch (ch)
                    {
                    case '\"':              break;
                    case '\\':              break;
                    case '/':               break;
                    case 'b':   ch='\x08';  break;
                    case 'f':   ch='\x0C';  break;
                    case 'n':   ch='\n';    break;
                    case 'r':   ch='\r';    break;
                    case 't':   ch='\t';    break;
                    default:
        throw new InvalidOperationException("Json String Format Error.");
                    }

                // "(終了文字)
                }
                else if (ch == '\"')
                {
            break;
                }
                sb.Append(ch);
            }
            iString = sb.ToString();
        }

        //----------------------------------------------------------------------------
        //      ヘッダ回復
        //----------------------------------------------------------------------------

        void readHeader()
        {
            using (var temp = new BaseSerializer.AutoRestoreLoadStructure(this, ElementsMapping.emName))
            {
                //      ---<<< 名前に従って回復 >>>---
System.Windows.Forms.TextBox textBox = cs_client.Form1.sTextBox;

                bool aExistSerializerName=false;
                bool aExistGlobalVersionNo=false;
                bool aExistTypeInfo=false;

                while(readPreElement() != ReadStat.Terminated)
                {
                    String aInfoName=loadElementName(ElementsMapping.emName);

                    if (aInfoName == "SerialzierName")
                    {
                        aExistSerializerName=true;
                        String aSerialzierName;
                        loadControl(out aSerialzierName);
textBox.AppendText(aSerialzierName + Environment.NewLine);
                        if (aSerialzierName != Constants.kJsonSerializerName)
                        {
        throw new InvalidOperationException
                                ("JsonMidISerializer : Unmatch serializer name("+aSerialzierName+")");
                        }
                    }
                    else if (aInfoName == "GlobalVersionNo")
                    {
                        aExistGlobalVersionNo=true;
                        loadControl(out mGlobalVersionNo);
textBox.AppendText(mGlobalVersionNo + Environment.NewLine);
                    }
                    else if (aInfoName == "TypeInfoList")
                    {
                        aExistTypeInfo=true;
                        readHeaderTypeInfo();
                    }
                    else
                    {
        throw new InvalidOperationException("Format Error.");
                    }
                }

                //      ---<<< 情報無しチェック >>>---

                if (!aExistSerializerName)
                {
        throw new InvalidOperationException("JsonMidISerializer : No Serializer name.");
                }

                if (!aExistGlobalVersionNo)
                {
        throw new InvalidOperationException("JsonMidISerializer : No global version number.");
                }

                if (!aExistTypeInfo)
                {
        throw new InvalidOperationException("JsonMidISerializer : No types infomation.");
                }
            }
        }
    }
}
