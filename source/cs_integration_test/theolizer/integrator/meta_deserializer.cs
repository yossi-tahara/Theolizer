using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace theolizer
{
    static class Program
    {
        /// <summary>
        /// アプリケーションのメイン エントリ ポイントです。
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            if (args.Length == 0)
            {
                Console.WriteLine("Need a parameter : <meta serialize file path>");
        return;
            }

            Console.WriteLine("MetaDeserializer");
            Console.WriteLine("Path={0}", args[0]);
        }
    }
}
