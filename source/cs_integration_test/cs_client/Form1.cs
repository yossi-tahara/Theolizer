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

namespace theolizer
{
    public static class Constants
    {
        public const String CppDllName="cpp_server.dll";
    }
}

namespace cs_client
{
    public partial class Form1 : Form
    {
        DllIntegrator   mDllIntegrator;
        StreamWriter    mRequestWriter;
        public Form1()
        {
            InitializeComponent();
            mDllIntegrator = DllIntegrator.get();
            mRequestWriter = mDllIntegrator.RequestWriter;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            mRequestWriter.WriteLine(123);
            mRequestWriter.Flush();
        }
    }
}
