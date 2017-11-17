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

namespace theolizer
{
    // ***************************************************************************
    //      各種ヘルパー
    // ***************************************************************************

    // クラス用インタフェース
    internal interface ITheolizer
    {
        void save(BaseSerializer iBaseSerializer);
        void load(BaseSerializer iBaseSerializer);
    }

    // クラス・メンバの対応方法(データはemOrderのみ使用)
    enum ElementsMapping
    {
        emName,                 // フィールド名(=0:完全自動時)
        emOrder                 // 順序
    }

    // シリアライズ・データ読出状態
    enum ReadStat {Terminated=0, Continue, DontCare}

    // ***************************************************************************
    //      基底シリアライザ
    // ***************************************************************************

    abstract class BaseSerializer : IDisposable
    {
        protected int               mIndent;
        protected ElementsMapping   mElementsMapping;

        //----------------------------------------------------------------------------
        //      コンストラクタ
        //----------------------------------------------------------------------------

        public BaseSerializer()
        {
            mIndent = 0;
            mElementsMapping = ElementsMapping.emName;
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
        //      内部構造を持つデータの保存開始／終了
        //----------------------------------------------------------------------------

        public sealed class AutoRestoreSaveStructure : IDisposable
        {
            BaseSerializer  mSerializer;
            ElementsMapping mElementsMapping;
            int             mIndent;

            //      ---<<< コンストラクタ >>>---

            public AutoRestoreSaveStructure
            (
                BaseSerializer  iSerializer,
                ElementsMapping iElementsMapping
            )
            {
                mSerializer = iSerializer;
                mElementsMapping=iSerializer.mElementsMapping;
                mIndent = mSerializer.mIndent;
                mSerializer.saveStructureStart();
            }

            //      ---<<< 破棄処理 >>>---

            public void Dispose()
            {
                mSerializer.mIndent=mIndent;
                mSerializer.saveStructureEnd();
                mSerializer.mElementsMapping=mElementsMapping;
            }
        }

        //----------------------------------------------------------------------------
        //      派生シリアライザで実装するシリアライズ補助関数群
        //----------------------------------------------------------------------------

        protected virtual void saveStructureStart()
        {
            throw new NotImplementedException();
        }
        protected virtual void saveStructureEnd()
        {
            throw new NotImplementedException();
        }
    }
}
