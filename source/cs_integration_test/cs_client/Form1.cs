using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
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
            mDllIntegrator = DllIntegrator.get();
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            // 送信
            int data = (int)numericUpDown.Value;
            mDllIntegrator.RequestWriter.WriteLine(data);
            mDllIntegrator.RequestWriter.Flush();
            numericUpDown.UpButton();

Debug.WriteLine("Write");
            // 受信
            var str = mDllIntegrator.ResponseReader.ReadLine();

            // 表示
            textBox.AppendText("Send(" + data + ") -> " + str + Environment.NewLine);
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
