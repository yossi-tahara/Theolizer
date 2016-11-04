//############################################################################
//      テスト用ツール群
/*
    Copyright (c) 2016 Yohinori Tahara(Theoride Technology) - http://theolizer.com/

    商用ライセンス
        有効なTheolizer商用ライセンスを保持している人は、
        田原良則(Theoride Technology)と締結している商用ライセンス契約の
        使用条件に従って、このファイルを取り扱うことができます。

    General Public License Version 3(以下GPLv3)
        Free Software Foundationが公表するGPLv3の使用条件に従って、
        あなたはこのファイルを取り扱うことができます。
        GPLv3の内容を https://www.gnu.org/licenses/gpl.txt にて確認して下さい。
        GPLv3のコピーをLICENSE.TXTファイルにおいてます。
*/
//############################################################################

#define THEOLIZER_INTERNAL_EXCLUDE_VERSION_H

//############################################################################
//      インクルード
//############################################################################

#include "avoid-trouble.h"

#include <regex>
#include <algorithm>
#include <mutex>

#define BOOST_ALL_NO_LIB        // Auto-linkを禁止する
#define BOOST_TEST_NO_MAIN
#define BOOST_NO_EXCEPTIONS
THEOLIZER_INTERNAL_DISABLE_WARNING()
#include <boost/test/included/prg_exec_monitor.hpp>

#include <boost/filesystem.hpp>
namespace boostF = boost::filesystem;

#if defined(_MSC_VER) && defined(_DEBUG)
    #define _CRTDBG_MAP_ALLOC
    #include <crtdbg.h>
#endif

#include "internal.h"
#include "../test_tool.h"

//############################################################################
//      Begin
//############################################################################

#if defined(_MSC_VER)   // disabling MSVC warnings
    #pragma warning(disable:4996)
#endif

namespace theolizer
{

//############################################################################
//      ユーティリティ
//############################################################################

// ***************************************************************************
//      環境変数獲得
// ***************************************************************************

std::string getenv(std::string const& iEnv)
{
    char *p = ::getenv(iEnv.c_str());
    if (p == nullptr)
return "";

    return p;
}

int putenv(char const* iPutEnv)
{
#if defined(_MSC_VER)
    return ::_putenv(iPutEnv);
#else
    return ::putenv(const_cast<char*>(iPutEnv));
#endif
}

//############################################################################
//      ファイル操作
//############################################################################

// ***************************************************************************
//      テスト用フォルダの準備
// ***************************************************************************

//----------------------------------------------------------------------------
//      テスト開始準備
//----------------------------------------------------------------------------

PrepareDir::PrepareDir(std::string const& iDirPath, bool iIsNoDelete) :
    mDirPath(iDirPath),
    mIsNoDelete(iIsNoDelete)
{
    boost::system::error_code error;
    boostF::remove_all(mDirPath, error);

    boostF::create_directories(mDirPath, error);
    // ubuntuではcreate_directories()の戻り値(常にfalse)もerror(常に成功)も使えなかった
    // exists()でフォルダの存在確認できたのでこれを用いる。
    if (!boostF::exists(mDirPath, error))
    {
        std::stringstream ss;
        ss << "Can not create directory<"
           << iDirPath << "> : "
           << "boost::filesystem::create_directories() returned error="
           << error;
        throw std::runtime_error(ss.str().c_str());
    }
}

//----------------------------------------------------------------------------
//      後始末
//----------------------------------------------------------------------------

PrepareDir::~PrepareDir()
{
    if (!mIsNoDelete) {
        boost::system::error_code error;
        boostF::remove_all(mDirPath, error);
    }
};

// ***************************************************************************
//      ファイル／フォルダの有無チェック
// ***************************************************************************

bool isExist(u8string const& iPath)
{
    boost::system::error_code error;
    bool result = boostF::exists(iPath.get_bstring(), error);

    return result;
}

// ***************************************************************************
//      ファイル削除
// ***************************************************************************

void removeFile(u8string const& iFilePath)
{
    if (boostF::is_regular_file(iFilePath.get_bstring()))
    {
        boost::system::error_code error;
        boostF::remove(iFilePath.get_bstring(), error);
    }
}

// ***************************************************************************
//      ファイル・リスト獲得
// ***************************************************************************

std::vector<std::string> getFileList(std::string const& iDirPath, std::string const& iRegex)
{
    std::wregex aPattern(internal::Converter<wchar_t, char>::conv(iRegex));
    std::vector<std::string>    list;
    for (boostF::directory_iterator it(iDirPath);
         it != boostF::directory_iterator();
         ++it)
    {
#if defined(_WIN32) // ubuntuでtheolizerをsharedライブラリにした時、例外が発生する(原因不明)
        std::string aFileName = theolizer::u8string(it->path().filename().wstring());
#else               // こちらはWindowsでうまくいかない。(Shift-JISで返却されるため)
        std::string aFileName = it->path().filename().string();
#endif
        if (!boostF::is_regular_file(it->status()))
    continue;

        if ((iRegex!="")
         && (!std::regex_match(u8string(aFileName).get_wstring(), aPattern)))
    continue;

        list.push_back(std::move(aFileName));
    }

    std::sort(list.begin(), list.end());        // Linuxではソートされてないらしい

    return list;
}

// ***************************************************************************
//      アクセス許可設定
// ***************************************************************************

void setWritePermission(const u8string& iPath, bool iIsEnable)
{
    boostF::path aPath(iPath.get_bstring());
    boost::system::error_code error;
    if (iIsEnable)
    {
        boostF::permissions(aPath, boostF::add_perms | boostF::owner_write, error);
    }
    else
    {
        boostF::permissions(aPath, boostF::remove_perms | boostF::owner_write, error);
    }
}

//############################################################################
//      main()関数関連処理
//############################################################################

// ***************************************************************************
//      PASS結果出力の有無
// ***************************************************************************

bool    gDoDisplayPass = false;

// ***************************************************************************
//      結果出力先
// ***************************************************************************

namespace
{
std::ostream*   gOStream = &std::cout;
}   // namepsace

void setOStream(std::ostream& iOStream)
{
    gOStream=&iOStream;
}
std::ostream& getOStream()
{
    return *gOStream;
}

// ***************************************************************************
//      test_main()呼び出しヘルパー
// ***************************************************************************

namespace
{

struct TestMainCaller
{
    TestMainCaller(int (*iTestMain)(int, char**), int iArgc, char** iArgv) :
                    mTestMain(iTestMain),
                    mArgc(iArgc),
                    mArgv(iArgv)
    { }

    int operator()() { return (*mTestMain)(mArgc, mArgv); }

private:
    int     (*mTestMain)(int, char**);
    int     mArgc;
    char**  mArgv;
};

} // namespace

// ***************************************************************************
//      テスト中断用例外を投げる
//          任意の例外はboost::excution_monitorでunkown type例外になってしまう。
//          そこで、boost::execution_exception例外を投げ、判別できるようにする。
// ***************************************************************************

namespace
{
const boost::unit_test::const_string TheolizerTestAborted = "TheolizerTestAborted";
}   // namespace

void throwAbort()
{
    throw boost::execution_exception(boost::execution_exception::user_error,
                                     TheolizerTestAborted,
                                     boost::execution_exception::location());
}

// ***************************************************************************
//      エラー・カウンタと出力整理用Mutex
// ***************************************************************************

namespace
{
    std::mutex  gTestMutex;
}

namespace internal
{
    unsigned    gTotal;
    unsigned    gFailCount;

    void lockMutex()
    {
        gTestMutex.lock();
    }
    void unlockMutex()
    {
        gTestMutex.unlock();
    }
}

// ***************************************************************************
//      boost::execution_monitor経由のTestMain()呼び出し
// ***************************************************************************

int callTestMain(int (*iTestMain)(int, char**), int iArgc, char** iArgv)
{
    int result = kExitSuccess;

#if defined(_MSC_VER) && defined(_DEBUG)
    _CrtSetReportMode(_CRT_WARN,  _CRTDBG_MODE_FILE);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN,  _CRTDBG_FILE_STDERR);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    try
    {
        internal::gTotal=0;
        internal::gFailCount=0;

        result = ::boost::execution_monitor().execute(
                                    TestMainCaller(iTestMain, iArgc, iArgv));
    }
#ifndef BOOST_NO_EXCEPTIONS
    catch(boost::execution_exception const& exex)
    {
        if ((exex.code() == boost::execution_exception::user_error)
         && (exex.what() == TheolizerTestAborted)) {
            result = kExitFatal;
        } else {
            std::stringstream ss;
            ss << exex.what();  // const_stringは直接char*やstd::stringへ変換できない。
            *gOStream << "\n**** uncatched exception."
                      << "\n**** code(" << exex.code() << ") : "
                      <<  ss.str();
            result = kExitException;
        }
    }
    catch(boost::system_error const& ex)
    {
        *gOStream << "\n**** failed to initialize execution monitor."
                  << "\n**** expression at fault: " << ex.p_failed_exp 
                  << "\n**** error(" << ex.p_errno << "): "
                  << std::strerror( ex.p_errno ) << std::endl;
        result = kExitException;
    }
#else
    catch(ErrorInfo& e)
    {
        internal::gTotal++;
        internal::gFailCount++;
        *gOStream << e;
        result = kExitException;
    }
    catch(std::exception& e)
    {
        internal::gTotal++;
        internal::gFailCount++;
        *gOStream << "\n**** std::exception : " << e.what() << std::endl;
        result = kExitException;
    }
#endif

    *gOStream << "\n------------- End Test ----------\n"
        << "    Total = " << internal::gTotal << "\n"
        << "    Pass  = " << internal::gTotal - internal::gFailCount << "\n"
        << "    Fail  = " << internal::gFailCount << "\n";

    return result;
}

//############################################################################
//      End
//############################################################################

} // namespace theolizer

// ***************************************************************************
//      BOOST_NO_EXCEPTIONSを定義するとthrow_exception()の定義が必要
// ***************************************************************************

#ifdef BOOST_NO_EXCEPTIONS
namespace boost
{
    void throw_exception(std::exception const& e)
    {
        THEOLIZER_INTERNAL_ABORT(e.what());
    }
}
#endif
