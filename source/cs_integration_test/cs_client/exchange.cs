//############################################################################
//      ユーザが記述するファイル
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
using System.Windows.Forms;
using cs_client;

namespace exchange
{
    // ユーザ・クラス(パラメータでやり取りするもの)のユーザ追加部
    partial class UserClassSub
    {
        public UserClassSub(UInt32 iUIntSub, string iStringSub)
        {
            mUIntSub=iUIntSub;
            mStringSub=iStringSub;
        }

        // デフォルト・コンストラクタ(共有テーブル登録のために必要)
        //  非デフォルト・コンストラクタを定義すると自動生成されない模様
        public UserClassSub() {}
    };

    // ユーザ・クラス(通知用)のユーザ追加部
    partial class UserClassNotify
    {
        TextBox   mTextBox;     // このメンバはC++へは送られない

        public UserClassNotify(TextBox iTextBox)
        {
            mMessage = "Constructed by user constructor";
            mCount = 12345;
            mTextBox = iTextBox;
        }

        // デフォルト・コンストラクタ(共有テーブル登録のために必要)
        //  非デフォルト・コンストラクタを定義すると自動生成されない模様
        public UserClassNotify() {}

        // 通知関数
        public void notify()
        {
            mTextBox.AppendText("Notify : " + Environment.NewLine);
            mTextBox.AppendText("  mMessage=" + mMessage + Environment.NewLine);
            mTextBox.AppendText("  mCount=" + mCount + Environment.NewLine);
        }
    };
}
