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
        DllIntegrator   mDllIntegrator;

        public Form1()
        {
            InitializeComponent();
            mDllIntegrator = DllIntegrator.getInstance(SerializerType.Json, Theolizer.GlobalVersionNo);
            ThreadIntegrator.Integrator = mDllIntegrator;

            textBox.AppendText("GlobalVersionNo = "
                + mDllIntegrator.GlobalVersionNo + Environment.NewLine);
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            // 送信
            int data = (int)numericUpDown.Value;
            var aUserClassMain = new exchange.UserClassMain();
            aUserClassMain.mIntMain = data;
            var aUserClassSub=new exchange.UserClassSub(5678, "test [\"][\\][/][\x08][\x0c][\n][\r][\t]");
            var ret = aUserClassMain.func0(aUserClassSub);

            textBox.AppendText("Request : " + Environment.NewLine);
            textBox.AppendText("  UserClassMain.mIntMain="
                + aUserClassMain.mIntMain + Environment.NewLine);
            textBox.AppendText("  UserClassSub.mUIntSub="
                + aUserClassSub.mUIntSub + Environment.NewLine);
            textBox.AppendText("  UserClassSub.mIntMain="
                + aUserClassSub.mStringSub + Environment.NewLine);
            textBox.AppendText("Response : " + Environment.NewLine);
            textBox.AppendText("  ret =" + ret + Environment.NewLine);
            textBox.AppendText("  UserClassMain.mIntMain="
                + aUserClassMain.mIntMain + Environment.NewLine);
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
