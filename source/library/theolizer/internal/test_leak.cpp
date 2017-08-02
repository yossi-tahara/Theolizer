//############################################################################
//      メモリ・リークのテスト
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

#if defined(_MSC_VER)
    #include <crtdbg.h>
#endif

// ***************************************************************************
//      メモリ・リーク検出用のクラスとグローバル変数
// ***************************************************************************

namespace theolizer
{
namespace internal
{

#if defined(_MSC_VER)
struct TestLeak
{
    TestLeak()
    {
        _CrtSetReportMode(_CRT_WARN,  _CRTDBG_MODE_FILE);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN,  _CRTDBG_FILE_STDERR);
        _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    }

    ~TestLeak()
    {
    }

    void enableTestLeak() { }
};
#endif

#if defined(_MSC_VER)
    #pragma warning(disable : 4073)
    #pragma init_seg(lib)
    TestLeak  sTestLeak;
#endif

// ***************************************************************************
//      グローバル変数の有効化用
//          gccではこれがないと定義されない。
//          msvcではなくても定義されるが、念のため残す。
//          この関数は実際には呼ばれないが、呼び出すコードが存在する。
// ***************************************************************************

void enableTestLeak()
{
#if defined(_MSC_VER)
    sTestLeak.enableTestLeak();
#endif
}

}   // namespace internal
}   // namespace theolizer
