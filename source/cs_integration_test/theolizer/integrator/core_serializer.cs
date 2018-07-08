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
using System.IO;
using System.Text;

namespace theolizer.internal_space
{
    // ***************************************************************************
    //      型種別(メタ・シリアライズ用)
    // ***************************************************************************

    public class TypeKind
    {
        public enum Value : UInt32
        {
            Invalid             =0,

            // フラグ
            PrimitiveFlag       =1,                 // Primitive型
            EnumFlag            =2,                 // enum型
            ClassFlag           =4,                 // クラス型
            NonIntrusiveFlag    =8,                 //   非侵入型
            ManualFlag          =16,                //   手動型

            // 値
            Primitive           =PrimitiveFlag,
            Enum                =EnumFlag  | NonIntrusiveFlag,
            IntrusiveAuto       =ClassFlag,                                     // 半自動
            NonIntrusiveAuto    =ClassFlag | NonIntrusiveFlag,                  // 完全自動
            IntrusiveManual     =ClassFlag                    | ManualFlag,     // 未サポート
            NonIntrusiveManual  =ClassFlag | NonIntrusiveFlag | ManualFlag,     // 手動

            //   テンプレート
            Template            =32,

            // 追加情報の基数とマスク
            AdditionalRadix     =64,
            AdditionalMask      =(~0x3fu),

            // クラス用追加情報
            Order               =1*AdditionalRadix,     // 要素の対応方式       0:Name      1:Order

            // enum型用追加情報
            SaveValue           =1*AdditionalRadix,     // 保存方式             0:名前保存  1:値保存
            Scoped              =2*AdditionalRadix,     //                      0:Unscoped  1:Scoped
            UnderlyingRadix     =4*AdditionalRadix,     // 基底型部分の基数
            UnderlyingMask      =0x3c*AdditionalRadix,  // 基底型部分のマスク
            Signed              =1*UnderlyingRadix,     // 符号の有無
            Bool                =0*UnderlyingRadix,
            Int8                =1*UnderlyingRadix,
            UInt8               =2*UnderlyingRadix,
            Int16               =3*UnderlyingRadix,
            UInt16              =4*UnderlyingRadix,
            Int32               =5*UnderlyingRadix,
            UInt32              =6*UnderlyingRadix,
            Int64               =7*UnderlyingRadix,
            UInt64              =8*UnderlyingRadix
        }

        // コンストラクタ
        public TypeKind()
        {
            mValue = Value.Invalid;
        }
        public TypeKind(UInt32 iValue)
        {
            mValue = (Value)iValue;
        }
        public TypeKind(Value iValue)
        {
            mValue = iValue;
        }
#if false
        public TypeKind(TypeKind iTypeKind)
        {
            mValue = iTypeKind.mValue;
        }
#endif
#if false
        // 代入演算子
        public TypeKind& operator=(TypeKind iRhs)
        {
            mValue = iRhs.mValue;
            return *this;
        }
        public TypeKind& operator=(Value iRhs)
        {
            mValue = iRhs;
            return *this;
        }

        // 変換
        public operator TypeKind::Value() const { return mValue; }
#endif

        // 有効
        public bool isValid() { return mValue != Value.Invalid; }

        // データ返却
        public Value get() { return mValue; }

#if false
        // 比較演算子
        public bool operator==(TypeKind iRhs)        const { return mValue == iRhs.mValue; }
        public bool operator!=(TypeKind iRhs)        const { return mValue != iRhs.mValue; }
        public bool operator==(TypeKind::Value iRhs) const { return mValue == iRhs; }
        public bool operator!=(TypeKind::Value iRhs) const { return mValue != iRhs; }
#endif

        // 文字列返却
        public String to_string()
        {
            StringBuilder ret = new StringBuilder();

            Action<String> add=(String iRhs)=>
            {
                if (ret.Length == 0)
                {
                    ret.Append('|');
                }
                ret.Append(iRhs);
            };

            switch(mValue & ~(Value.AdditionalMask | Value.Template))
            {
            case Value.Primitive:           ret.Append("Primitive");            break;
            case Value.Enum:                ret.Append("Enum");                 break;
            case Value.IntrusiveAuto:       ret.Append("IntrusiveAuto");        break;
            case Value.NonIntrusiveAuto:    ret.Append("NonIntrusiveAuto");     break;
            case Value.IntrusiveManual:     ret.Append("IntrusiveManual");      break;
            case Value.NonIntrusiveManual:  ret.Append("NonIntrusiveManual");   break;
            default:
                if ((mValue & Value.PrimitiveFlag)!=0)    add("PrimitiveFlag");
                if ((mValue & Value.EnumFlag)!=0)         add("EnumFlag");
                if ((mValue & Value.ClassFlag)!=0)        add("ClassFlag");
                if ((mValue & Value.NonIntrusiveFlag)!=0) add("NonIntrusiveFlag");
                if ((mValue & Value.ManualFlag)!=0)       add("ManualFlag");
                break;
            }
            if ((mValue & Value.Template)!=0)             add("Template");
            if ((mValue & Value.ClassFlag)!=0)
            {
                if ((mValue & Value.Order)!=0)            add("Order");
            }
            else if ((mValue & Value.EnumFlag)!=0)
            {
                if ((mValue & Value.SaveValue)!=0)        add("SaveValue");
                if ((mValue & Value.Scoped)!=0)           add("Scoped");
                switch(mValue & Value.UnderlyingMask)
                {
                case Value.Bool:      add("Bool");    break;
                case Value.Int8:      add("Int8");    break;
                case Value.UInt8:     add("UInt8");   break;
                case Value.Int16:     add("Int16");   break;
                case Value.UInt16:    add("UInt16");  break;
                case Value.Int32:     add("Int32");   break;
                case Value.UInt32:    add("UInt32");  break;
                case Value.Int64:     add("Int64");   break;
                case Value.UInt64:    add("UInt64");  break;
                }
            }

            return ret.ToString();
        }

#if false
        // 出力
        friend std::ostream& operator<<(std::ostream& iOStream, TypeKind iTypeKindXXXX)
        {
            iOStream << iTypeKindXXXX.mValue;
            return iOStream;
        }

        // 入力
        friend std::istream& operator>>(std::istream& iIStream, TypeKind& oTypeKindXXXX)
        {
            unsigned    temp;
            iIStream >> temp;
            oTypeKindXXXX.mValue = static_cast<Value>(temp);
            return iIStream;
        }
#endif

        Value   mValue;
    }

    // ***************************************************************************
    //      TypeIndex本体
    // ***************************************************************************

    public class TypeIndex
    {
        const UInt32 kInvalidUnsigned = 0x80000000u;

        UInt32  mTypeIndexImpl;

        public TypeIndex()
        {
            mTypeIndexImpl = kInvalidUnsigned;
        }
        public TypeIndex(short iIndex) // primitive用
        {
            mTypeIndexImpl = (UInt32)iIndex*TypeIndexRadix;
        }
        public TypeIndex(UInt32 iIndex)
        {
            mTypeIndexImpl = iIndex*TypeIndexRadix;
        }
#if false
        public TypeIndex(TypeIndex iTypeIndex)
        {
            mTypeIndexImpl = iTypeIndex.mTypeIndexImpl;
        }
        TypeIndex& operator=(UInt32 iIndex)
        {
            mTypeIndexImpl = iIndex*TypeIndexRadix;
            return *this;
        }
        TypeIndex& operator=(TypeIndex const& iTypeIndex)
        {
            mTypeIndexImpl = iTypeIndex.mTypeIndexImpl;
            return *this;
        }
#endif
        public void set(UInt32 iIndex, UInt32 iAdditional)
        {
            mTypeIndexImpl = iIndex*TypeIndexRadix + iAdditional;
        }
        public void setRaw(UInt32 iTypeIndexImpl)
        {
            mTypeIndexImpl = iTypeIndexImpl;
        }
        public UInt32 getRaw()
        {
            return mTypeIndexImpl;
        }
        public bool isValid()
        {
            return mTypeIndexImpl != kInvalidUnsigned;
        }
#if false
        public bool operator==(TypeIndex iTypeIndex)
        {
            return mTypeIndexImpl == iTypeIndex.mTypeIndexImpl;
        }
        public bool operator!=(TypeIndex iTypeIndex)
        {
            return mTypeIndexImpl != iTypeIndex.mTypeIndexImpl;
        }
#endif
        public UInt32 getIndex()
        {
            return mTypeIndexImpl/TypeIndexRadix;
        }

        // 追加情報
        public UInt32 getAdditional()
        {
            return mTypeIndexImpl & ~TypeIndexMask;
        }
        // 追加文字列
        //  付加情報有りの時、$に続けて
        //      etipPointee:"e"     etipNormalPointer:"p"   etipOwnerPointer:"o"
        //      etiaArray1:"1"      etiaArray2:"2"          etiaArray3:"3"
        //  通常ポインタの1次元配列:$p1
        public String getAdditionalString()
        {
            StringBuilder ret = new StringBuilder();
            if ((mTypeIndexImpl & (UInt32)TypeIndexArray.etiaMask) != 0)
            {
                ret.Append('$');
            }
            switch((TypeIndexArray)mTypeIndexImpl & TypeIndexArray.etiaMask)
            {
            case TypeIndexArray.etiaNone:                              break;
            case TypeIndexArray.etiaArray1:        ret.Append('1');    break;
            case TypeIndexArray.etiaArray2:        ret.Append('2');    break;
            case TypeIndexArray.etiaArray3:        ret.Append('3');    break;
            }
            return ret.ToString();
        }
        // 追加文字列分離
        //  戻り値は付加情報
        public static UInt32 splitAdditional(ref String ioTypeName)
        {
            int pos = ioTypeName.IndexOf('$');

            // 付加文字列無し
            if (pos < 0)
        return 0;

            char    first  = ioTypeName[pos+1];

            UInt32      ret=0;
            bool        skip=true;
#if false
            switch (first)
            {
            case 'e':   ret += etipPointee;         break;
            case 'p':   ret += etipNormalPointer;   break;
            case 'o':   ret += etipOwnerPointer;    break;
            default:    skip=false;                 break;
            }
#endif
            // ポインタ情報があったら、配列情報が続いているかも知れない
            if (skip)
            {
                // 配列情報が無かったら解析終了
                if (pos+2 >= ioTypeName.Length)
                {
                    ioTypeName.Substring(0, pos);
        return ret;
                }
                first=ioTypeName[pos+2];
            }

            switch(first)
            {
            case '1':   ret += (UInt32)TypeIndexArray.etiaArray1;   break;
            case '2':   ret += (UInt32)TypeIndexArray.etiaArray2;   break;
            case '3':   ret += (UInt32)TypeIndexArray.etiaArray3;   break;
            default:
        throw new InvalidOperationException(String.Format("Unknown additional.({0})", ioTypeName));
            }

            ioTypeName.Substring(0, pos);
            return ret;
        }

        // 配列種別
        enum TypeIndexArray : UInt32
        {
            etiaMask                =0x03,
            etiaNone                =0x00,      // 非配列
            etiaArray1              =0x01,      // 1次元配列
            etiaArray2              =0x02,      // 2次元配列
            etiaArray3              =0x03,      // 3次元配列
        }

        public UInt32 getRank()
        {
            return mTypeIndexImpl & (UInt32)TypeIndexArray.etiaMask;
        }
        public void setRank(UInt32 iRank)
        {
            if (3 < iRank)
            {
        throw new InvalidOperationException(String.Format("too large array rank({0})", iRank));
            }
            mTypeIndexImpl |= iRank;
        }

        // TypeIndexの基数
        const UInt32    TypeIndexRadix = 0x10;
        const UInt32    TypeIndexMask  = (~0xfu);

#if false
        // 文字列型への出力
        public static void Write(StreamReaderEx StreadWriter iOStream, TypeIndex iTypeIndex)
        {
            if (iTypeIndex.isValid())
            {
                iOStream << (iTypeIndex.mTypeIndexImpl/TypeIndexRadix) << "e"
                         << (iTypeIndex.mTypeIndexImpl&(~TypeIndexMask));
            }
            else
            {
                iOStream << "[Invalid TypeIndex]";
            }
            return iOStream;
        }

        // 文字列型からの入力
        public static void Read(StreamReaderEx iIStream, TypeIndex oTypeIndex)
        {
            UInt32 aIndex;
            iIStream.Read(out aIndex);
            if (iIStream.Read() != 'e')
            {
        throw new InvalidOperationException(String.Format("Format Error."));
            }
            UInt32 aAdditional;
            iIStream.Read(out aAdditional);
            if (TypeIndexRadix <= aAdditional)
            {
        throw new InvalidOperationException(String.Format("Format Error."));
            }
            oTypeIndex.set(aIndex, aAdditional);
        }
#endif
    }

    // ***************************************************************************
    //      各種ヘルパー
    // ***************************************************************************

    // 型チェック・モード
    enum CheckMode
    {
        InMemory=0,                 // メモリ内動作のみ、かつ、型チェック無し（非公開）
        TypeCheckInData,            // データ内に型名を記録する（非公開）
        NoTypeCheck,                // 型チェック無し
        TypeCheck,                  // TypeIndexによる型チェック
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
        protected CheckMode         mCheckMode;

        //----------------------------------------------------------------------------
        //      コンストラクタ
        //----------------------------------------------------------------------------

        public BaseSerializer(uint iGlobalVersionNo)
        {
            mGlobalVersionNo = iGlobalVersionNo;
            mElementsMapping = ElementsMapping.emName;
            mCancelPrettyPrint = false;
            mIndent = 0;
            mCheckMode = CheckMode.NoTypeCheck;
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
        //      ヘッダ内型情報保存
        //----------------------------------------------------------------------------

        protected void writeHeaderTypeInfo()
        {
            using (var temp =
                new BaseSerializer.AutoRestoreSaveStructure(this, ElementsMapping.emOrder, true))
            {
                // 型チェックのモード出力
                writePreElement();
                saveControl((int)mCheckMode);
            }
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
        //      トップ・レベル保存前後処理
        //----------------------------------------------------------------------------

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
        protected virtual void saveControl(TypeIndex iTypeIndex)    {throw new NotSupportedException();}
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

        // ***************************************************************************
        //      デシリアライズ機能群
        // ***************************************************************************

        //----------------------------------------------------------------------------
        //      ヘッダ内型情報回復
        //----------------------------------------------------------------------------

        protected void readHeaderTypeInfo()
        {
            using (var aAutoRestoreLoad =
                new BaseSerializer.AutoRestoreLoadStructure(this, ElementsMapping.emOrder))
            {
                //      ---<<< 型チェックのモード確認 >>>---

                readPreElement();
                int aCheckMode;
                loadControl(out aCheckMode);
                mCheckMode = (CheckMode)aCheckMode;
                switch((CheckMode)aCheckMode)
                {
                case CheckMode.NoTypeCheck:
        return;

                case CheckMode.TypeCheck:
                    break;

                case CheckMode.MetaMode:
                    if (MetaDeserializer.get() == null)
            throw new InvalidOperationException("Meta-serialized data.");

                    deserializeMeta();
                    break;

                default:
            throw new InvalidOperationException("CheckMode Error : " + aCheckMode);
                }

            }
        }

        //----------------------------------------------------------------------------
        //      ヘッダ内型情報回復
        //----------------------------------------------------------------------------

        protected void deserializeMeta()
        {
            //      ---<<< ヘッダ内の型情報回復 >>>---
            //      C#側は自バージョン・データのみなのでTypeIndex対応表は不要。

            readPreElement();
            UInt64 aTypeIndexCount;
            loadControl(out aTypeIndexCount);

            // 型情報取出し
            while(readPreElement() != ReadStat.Terminated)
            {
                // 開始マーク
                using (var aAutoRestoreLoad = new BaseSerializer.AutoRestoreLoadStructure(this))
                {
                    if (readPreElement() == ReadStat.Terminated)
            break;
                    // TypeIndex取出し
                    TypeIndex aDataTypeIndex = new TypeIndex();
                    loadControl(aDataTypeIndex);

                    if (readPreElement() == ReadStat.Terminated)
            break;
                    String  aTypeName;
                    loadControl(out aTypeName);

                    // 型の種別を回復する
                    if (readPreElement() == ReadStat.Terminated)
            break;
                    UInt32      temp;
                    loadControl(out temp);
                    TypeKind    aTypeKind=new TypeKind(temp);

                    // 次処理準備
                    if (readPreElement() == ReadStat.Terminated)
            continue;

                    // TypeKindによる分岐
                    var aBaseType = aTypeKind.get() & ~TypeKind.Value.AdditionalMask;
                    Console.WriteLine(aBaseType);

                    // プリミティブ
                    if (aBaseType == TypeKind.Value.PrimitiveFlag)
                    {
                        Console.WriteLine("Primitive " + aTypeName);
                    }

                    // enum型
                    else if ((UInt32)(aBaseType & TypeKind.Value.EnumFlag) != 0)
                    {
                        Console.WriteLine("Enum " + aTypeName);
                    }

                    // クラス
                    else if (((UInt32)(aBaseType & TypeKind.Value.ClassFlag) != 0)
                          && ((UInt32)(aBaseType & TypeKind.Value.ManualFlag) == 0))
                    {
                        Console.WriteLine("Class " + aTypeName);
                        deserializeMetaClass(aTypeKind);
                    }
                    else
                    {
                        Console.WriteLine("another " + aTypeName);
                    }
                }
            }
        }

        //----------------------------------------------------------------------------
        //      メタ・デシリアライズ（クラス）
        //----------------------------------------------------------------------------

        protected void deserializeMetaClass(TypeKind iTypeKind)
        {
            ElementsMapping aElementsMapping =
                ((UInt32)(iTypeKind.get() & TypeKind.Value.Order)!=0)?ElementsMapping.emOrder:ElementsMapping.emName;

            // 開始マーク
            using (var aAutoRestoreLoad = new BaseSerializer.AutoRestoreLoadStructure(this))
            {
                // 名前対応時のみ要素名を取り出す
                String aElementName=loadElementName(aElementsMapping);

                while(readPreElement() != ReadStat.Terminated)
                {
                    TypeIndex aElementTypeIndex = new TypeIndex();
                    loadControl(aElementTypeIndex);

Console.WriteLine(((aElementName==null)?"<null>":aElementName) + " : " +
    aElementTypeIndex.getIndex() + "e" + aElementTypeIndex.getAdditional());
                }
            }
        }

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
        //      トップ・レベル回復前後処理
        //----------------------------------------------------------------------------

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
        protected virtual void loadControl(TypeIndex oTypeIndex)    {throw new NotSupportedException();}
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
    }

    // ***************************************************************************
    //      メタ・シリアライズ出力用のストリーム
    // ***************************************************************************

    class MetaDeserializer : IDisposable
    {
        static StreamWriter sStreamWriter = null;
        public static StreamWriter get() { return sStreamWriter; }

        public MetaDeserializer(string path)
        {
            if (sStreamWriter != null)
        throw new InvalidOperationException("Multiple constuct MetaDeserializer.");

            sStreamWriter = new StreamWriter(path);
        }

        ~MetaDeserializer()
        {
            Dispose(false);
        }

        void Dispose(bool disposing)
        {
            // 破棄済なら何もしない
            if (sStreamWriter == null)
        return;

            sStreamWriter.Dispose();
            sStreamWriter=null;
        }

        public void Dispose()
        {
            Dispose(true);
        }
    }
}
