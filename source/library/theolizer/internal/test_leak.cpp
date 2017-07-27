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
#elif !defined(__MINGW32__)
    #include <mcheck.h>
#endif

// ***************************************************************************
//      メモリ・リーク検出用のクラス
// ***************************************************************************

namespace theolizer
{
namespace internal
{

struct TestLeak
{
    TestLeak()
    {
    #if defined(_MSC_VER)
        _CrtSetReportMode(_CRT_WARN,  _CRTDBG_MODE_FILE);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN,  _CRTDBG_FILE_STDERR);
        _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #elif !defined(__MINGW32__)
        mtrace();
    #endif
    }

    ~TestLeak()
    {
    #if defined(_MSC_VER)
    #elif !defined(__MINGW32__)
        muntrace();
    #endif
    }

    void enableTestLeak() { }
};

// ***************************************************************************
//      リーク検出用グローバル変数
// ***************************************************************************

#if defined(_MSC_VER)
    #pragma warning(disable : 4073)
    #pragma init_seg(lib)
    TestLeak  sTestLeak;
#elif !defined(__MINGW32__)
    __attribute__((init_priority(65534))) TestLeak  sTestLeak;
#endif

void enableTestLeak()
{
    sTestLeak.enableTestLeak();
}

}   // namespace internal
}   // namespace theolizer
