using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace theolizer.internal_space
{
    static class Program
    {
        /// <summary>
        /// アプリケーションのメイン エントリ ポイントです。
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
var data = new MyData[2];
data[0] = new MyData{ Name="abc", Value="def"};
data[1] = new MyData{ Name="ABC", Value="dEF"};
var template = new CreateSource(data);
Console.WriteLine(template.TransformText());

            if (args.Length < 2)
            {
                Console.WriteLine("Need a parameter : <meta serialize file path> <output cs file>");
        return;
            }

            Console.WriteLine("MetaDeserializer");
            Console.WriteLine("Json={0}", args[0]);
            Console.WriteLine("CS  ={0}", args[1]);

            // 1.メタ・デシリアライズ用のソース自動生成先ファイルをオープン
            //  毎度新規作成（trunc）でOK
            // 2.それをメタ・デシリアライズ通知クラスへ設定
            // 3.args[0]をオープン（jsonなのでStreamWriter）
            // 4.JsonISerializerへ与える
            //  これのコンストラクタにてメタ・デシリアライズ処理実行

            using (var meta = new MetaDeserializer(args[1]))
            {
                using (var ifs = new FileStream(args[0], FileMode.Open))
                using (var jis = new JsonISerializer(ifs))
                {
                }
            }
        }
    }
    public class MyData
    {
        public string Name;
        public string Value;
    }
}
partial class CreateSource
{
    private theolizer.internal_space.MyData[] m_data;
    public CreateSource(theolizer.internal_space.MyData[] data) { this.m_data = data; }
}
