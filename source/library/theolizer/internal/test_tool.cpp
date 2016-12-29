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

//############################################################################
//      theolizer名前空間外のユーティリティ
//############################################################################

// ***************************************************************************
//      文字型の表示補助
// ***************************************************************************

std::ostream& operator<<(std::ostream& iOStream, char iChar)
{
    iOStream << static_cast<int>(iChar);
    return iOStream;
}

std::ostream& operator<<(std::ostream& iOStream, signed char iSChar)
{
    iOStream << static_cast<int>(iSChar);
    return iOStream;
}

std::ostream& operator<<(std::ostream& iOStream, unsigned char iUChar)
{
    iOStream << static_cast<int>(iUChar);
    return iOStream;
}

// ***************************************************************************
//      type_info型表示補助
// ***************************************************************************

std::ostream& operator<<(std::ostream& iOStream, std::type_info const& iTypeInfo)
{
    iOStream << theolizer::internal::getNameByTypeId(iTypeInfo);
    return iOStream;
}

//############################################################################
//      ファイル操作
//############################################################################

namespace theolizer
{

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
//      ユーティリティ
//############################################################################

// ***************************************************************************
//      環境変数獲得
// ***************************************************************************

namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

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

// ***************************************************************************
//      結果出力先
// ***************************************************************************

std::ostream*   gOStream = &std::cout;

void setOStream(std::ostream& iOStream)
{
    gOStream=&iOStream;
}
std::ostream& getOStream()
{
    return *gOStream;
}

// ***************************************************************************
//      テスト中断用例外を投げる
//          任意の例外はboost::excution_monitorでunkown type例外になってしまう。
//          そこで、boost::execution_exception例外を投げ、判別できるようにする。
// ***************************************************************************

const boost::unit_test::const_string TheolizerTestAborted = "TheolizerTestAborted";

void throwAbort()
{
    throw boost::execution_exception(boost::execution_exception::user_error,
                                     TheolizerTestAborted,
                                     boost::execution_exception::location());
}

// ***************************************************************************
//      エラー・カウンタと出力整理用Mutex
// ***************************************************************************

std::mutex  gTestMutex;

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

// ***************************************************************************
//      PASS結果出力の有無
// ***************************************************************************

bool    gDoDisplayPass = false;

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

// ***************************************************************************
//      PASS表示処理
// ***************************************************************************

DisplayPass::DisplayPass() : internal::AutoRestore<bool>(internal::gDoDisplayPass, true)
{
}

bool DisplayPass::on()
{
    return internal::gDoDisplayPass;
}

// ***************************************************************************
//      MinGW不具合対策
// ***************************************************************************

#if defined(__MINGW32__)
void cutOptimize()
{
}
#endif

// ***************************************************************************
//      PASS/FAIL数処理
// ***************************************************************************

void initResult()
{
    internal::gTotal=0;
    internal::gFailCount=0;
}

void incrementTotalCount()
{
    ++internal::gFailCount;
}

void incrementFailCount()
{
    ++internal::gFailCount;
}

bool printResult(char const* iTitle)
{
    if (iTitle)
    {
        *internal::gOStream << iTitle << "\n";
    }
    else
    {
        *internal::gOStream << "\n------------- End Test ----------\n";
    }

    *internal::gOStream
        << "    Total = " << internal::gTotal << "\n"
        << "    Pass  = " << internal::gTotal - internal::gFailCount << "\n"
        << "    Fail  = " << internal::gFailCount << "\n";

    return (internal::gFailCount == 0);
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
