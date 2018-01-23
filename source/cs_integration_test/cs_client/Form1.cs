using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using theolizer;

namespace cs_client
{
    public partial class Form1 : Form
    {
        // 共有するため寿命を長くする
        exchange.UserClassMain  mUserClassMain;
        exchange.UserClassSub   mUserClassSub2;

        public Form1()
        {
            InitializeComponent();

            mUserClassMain = new exchange.UserClassMain();
            mUserClassSub2 = new exchange.UserClassSub(5678, "mUserClassSub2 [\"][\\][/][\x08][\x0c][\n][\r][\t]");
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            // インテグレータ準備
            if (ThreadIntegrator.Integrator == null)
            {
                ThreadIntegrator.Integrator = DllIntegrator.getInstance
                    (SerializerType.Json, true, Theolizer.GlobalVersionNo);

                textBox.AppendText("GlobalVersionNo = "
                    + ThreadIntegrator.Integrator.GlobalVersionNo + Environment.NewLine);
            }

            // 送信準備
            mUserClassMain.mIntMain = (int)numericUpDown.Value;
            var aUserClassSub=new exchange.UserClassSub(5678, "aUserClassSub");

            // 送信前
            textBox.AppendText("Request : " + Environment.NewLine);
            textBox.AppendText("  mUserClassMain.mIntMain="
                + mUserClassMain.mIntMain + Environment.NewLine);

            textBox.AppendText("  aUserClassSub.mUIntSub="
                + aUserClassSub.mUIntSub + Environment.NewLine);
            textBox.AppendText("  aUserClassSub.mStringSub="
                + aUserClassSub.mStringSub + Environment.NewLine);

            textBox.AppendText("  mUserClassSub2.mUIntSub="
                + mUserClassSub2.mUIntSub + Environment.NewLine);
            textBox.AppendText("  mUserClassSub2.mStringSub="
                + mUserClassSub2.mStringSub + Environment.NewLine);

            // 送信
            var ret = mUserClassMain.request(aUserClassSub, mUserClassSub2);

            // 結果
            textBox.AppendText("Response : " + Environment.NewLine);
            textBox.AppendText("  ret =" + ret + Environment.NewLine);
            textBox.AppendText("  UserClassMain.mIntMain="
                + mUserClassMain.mIntMain + Environment.NewLine);

            textBox.AppendText("  mUserClassSub2.mUIntSub="
                + mUserClassSub2.mUIntSub + Environment.NewLine);
            textBox.AppendText("  mUserClassSub2.mStringSub="
                + mUserClassSub2.mStringSub + Environment.NewLine);
        }

         private void buttonDispose_Click(object sender, EventArgs e)
        {
            DllIntegrator.disposeInstance();
            ThreadIntegrator.Integrator = null;
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            DllIntegrator.disposeInstance();
        }
    }
}

// このファイルの最初で定義するとForm1.Designer.csの名前空間までtheolizerとなり、異常になる
namespace theolizer
{
    public static class Constants
    {
        public const String CppDllName = "cpp_server.dll";
    }
}
