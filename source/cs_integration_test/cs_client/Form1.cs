using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
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
        public Form1()
        {
            InitializeComponent();
            DllIntegrator.get();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            using (CppOStream cos = new CppOStream())
            using (StreamWriter sw = new StreamWriter(cos, Encoding.UTF8))
            {
                sw.WriteLine(123);
            }
        }
    }
}
