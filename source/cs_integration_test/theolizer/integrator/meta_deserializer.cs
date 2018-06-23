using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace meta_deserializer
{
    static class Program
    {
        /// <summary>
        /// アプリケーションのメイン エントリ ポイントです。
        /// </summary>
        [STAThread]
        static void Main()
        {
            DateTime now = DateTime.Now;
            Console.WriteLine("現在の時刻は、 {0} です", now);
        }
    }
}
