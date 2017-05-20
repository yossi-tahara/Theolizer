#include <iostream>
#include <fstream>
#include <string>
#include <theolizer/serializer_json.h>

struct Foo
{
    std::string name;
    int age;
};

#include "example.cpp.theolizer.hpp"            // Theolizer自動生成先

int main(int argc, char* argv[])
{
    // 保存
    {
        Foo foo;
        foo.name="Taro Yamada";
        foo.age=22;

        std::ofstream ofs("sample.txt");
        theolizer::JsonOSerializer<> jos(ofs);  // シリアライザを生成
        THEOLIZER_PROCESS(jos, foo);            // ファイルへfooを保存
    }

    // 回復
    {
        Foo foo;
        std::ifstream ifs("sample.txt");
        theolizer::JsonISerializer<> jis(ifs);  // デシリアライザを生成
        THEOLIZER_PROCESS(jis, foo);            // ファイルからfooを回復

        theolizer::JsonOSerializer<> jos(std::cout);
        THEOLIZER_PROCESS(jos, foo);            // 回復結果の簡易表示
    }
}
