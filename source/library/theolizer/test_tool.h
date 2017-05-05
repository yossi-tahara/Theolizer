//############################################################################
/*!
    @brief      テスト用ツール群
    @details    各種テスト用マクロとツールを提供する。
    @ingroup    TheolizerLib
    @file       test_tool.h
    @author     Yoshinori Tahara(Theoride Technology)
    @date       2015/08/08 Created
*/
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

#if !defined(THEOLIZER_INTERNAL_TEST_TOOL_H)
#define THEOLIZER_INTERNAL_TEST_TOOL_H

#include <thread>
#include <sstream>
#include <iomanip>

// ***************************************************************************
//      基本ツールのインクルード
// ***************************************************************************

#include "report.h"

//############################################################################
//      Begin
//############################################################################

// ***************************************************************************
//          DLL用の警告禁止
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable:4251)
#endif

//############################################################################
//      theolizer名前空間外のユーティリティ
//############################################################################

// ***************************************************************************
//      文字型の表示補助
// ***************************************************************************

//! @ingroup Test
//! char型を数値として出力する
THEOLIZER_INTERNAL_DLL std::ostream& operator<<(std::ostream& iOStream, char iChar);

//! @ingroup Test
//! signed char型を数値として出力する
THEOLIZER_INTERNAL_DLL std::ostream& operator<<(std::ostream& iOStream, signed char iSChar);

//! @ingroup Test
//! unsgined char型を数値として出力する
THEOLIZER_INTERNAL_DLL std::ostream& operator<<(std::ostream& iOStream, unsigned char iUChar);

// ***************************************************************************
//!     @ingroup Test
//!     scoped enum型表示補助
// ***************************************************************************

template<typename tEnum, THEOLIZER_INTERNAL_OVERLOAD((std::is_enum<tEnum>::value))>
std::ostream& operator<<(std::ostream& iOStream, tEnum iEnum)
{
    typedef typename std::underlying_type<tEnum>::type  Type;
    iOStream << static_cast<Type>(iEnum);
    return iOStream;
}

// ***************************************************************************
//!     @ingroup Test
//!     type_info型を型名へ変換して表示する
// ***************************************************************************

THEOLIZER_INTERNAL_DLL std::ostream& operator<<(std::ostream& iOStream, std::type_info const&);

//############################################################################
//      theolizer名前空間内のユーティリティ
//############################################################################

namespace theolizer
{
namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//      環境変数操作(テスト用)
// ***************************************************************************

THEOLIZER_INTERNAL_DLL std::string getenv(std::string const& iEnv);
THEOLIZER_INTERNAL_DLL int putenv(char const* iPutEnv);

// ***************************************************************************
//      結果出力先設定／獲得
// ***************************************************************************

THEOLIZER_INTERNAL_DLL void setOStream(std::ostream& iOStream);
THEOLIZER_INTERNAL_DLL std::ostream& getOStream();

// ***************************************************************************
//      テスト中断
// ***************************************************************************

THEOLIZER_INTERNAL_DLL void throwAbort();

// ***************************************************************************
//!     @ingroup Test
//!     開放時、関数呼び出し
//!         ほぼN4189のscope_exitの名前を変えただけ。
// ***************************************************************************

//----------------------------------------------------------------------------
//      ヘルパー・クラス
//----------------------------------------------------------------------------

template<typename tReleaser>
class ScopeExit
{
    tReleaser   mReleaser;
    bool        mMoved;

public:
    explicit ScopeExit(tReleaser &&iReleaser) noexcept :
        mReleaser(std::move(iReleaser)), mMoved{false}
    {}
    ~ScopeExit() noexcept(noexcept(mReleaser()))
    {
        if (!mMoved)
        {
            mReleaser();
        }
    }

    // ムーブ・コンストラクタ可
    ScopeExit(ScopeExit &&iRhs) noexcept :
        mReleaser(std::move(iRhs.mReleaser)),
        mMoved{iRhs.mMoved}
    {
        iRhs.mMoved=true;
    }

    // コピー不可／ムーブ代入不可
    ScopeExit(ScopeExit const&) = delete;
    void operator=(ScopeExit const &)=delete;
    ScopeExit& operator=(ScopeExit &&)=delete;
};

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

//----------------------------------------------------------------------------
//      本体
//----------------------------------------------------------------------------

//! @ingroup Test
//! ScopeExitファクトリ
template <typename tReleaser>
internal::ScopeExit<tReleaser> makeScopeExit(tReleaser &&iReleaser) noexcept
{
    return  internal::ScopeExit<typename std::remove_reference<tReleaser>::type>
            (
                std::forward<tReleaser>(iReleaser)
            );
}

// ***************************************************************************
//!     @brief      PASS結果表示の有無指定
//!     @details    trueにするとPASS結果も表示する
// ***************************************************************************

//----------------------------------------------------------------------------
//      本体
//----------------------------------------------------------------------------

struct THEOLIZER_INTERNAL_DLL DisplayPass : public internal::AutoRestore<bool>
{
    //! @ingroup Test
    //! デフォルト・コンストラクタ
    explicit DisplayPass();

    //! @ingroup Test
    //! PASS結果表示の状態を返却する
    static bool on();
};

// ***************************************************************************
//!     @ingroup Test
//!     @brief      MinGW不具合対処
//!     @details    関数テンプレートで浮動小数点を取り扱う時、最適化上の不具合がある模様<br>
//!                 処理に割り込んで最適化させないためのダミー関数
// ***************************************************************************

#if defined(__MINGW32__)
    THEOLIZER_INTERNAL_DLL void cutOptimize();
#else
    inline void cutOptimize() { }
#endif

// ***************************************************************************
//      テスト結果集計用
// ***************************************************************************

//! @ingroup Test
//! テスト集計初期化
THEOLIZER_INTERNAL_DLL void initResult();

//! @ingroup Test
//! テスト結果のFAIL数をインクリメントする
THEOLIZER_INTERNAL_DLL void incrementFailCount();

//! @ingroup Test
//! テスト結果を表示する
THEOLIZER_INTERNAL_DLL bool printResult(char const* iTitle=nullptr);

//############################################################################
//      ファイル操作
//############################################################################

// ***************************************************************************
/*!
    @ingroup Test
    @brief      テスト用フォルダの準備
    @details    もし、iDirPathフォルダが既に存在していたら削除する。<br>
                次に、iDirPathフォルダを作成する。<br>
                デストラクタにてiIsNoDeleteがfalseなら、iDirPathフォルダを削除する。<br>
                iDirPathフォルダを生成できなかったら、std::runtime_errorを投げるので、<br>
                catchしないことにより、テストが中断される。<br>
*/
// ***************************************************************************

class THEOLIZER_INTERNAL_DLL PrepareDir
{
private:
    std::string mDirPath;
    bool        mIsNoDelete;

public:
    //! @ingroup Test
    //! コンストラクタ
    PrepareDir(std::string const& iDirPath, bool iIsNoDelete=false);

    //! @ingroup Test
    //! デストラクタ
    ~PrepareDir();
};

// ***************************************************************************
//!     ファイル／フォルダの有無チェック
// ***************************************************************************

THEOLIZER_INTERNAL_DLL bool isExist(u8string const& iFilePath);

// ***************************************************************************
/*!
    @ingroup    Test
    @brief      ファイルの削除
    @details    指定ファイルが無くても正常終了する。
                iFilePathがフォルダの場合何もしない。
*/
// ***************************************************************************

THEOLIZER_INTERNAL_DLL void removeFile(u8string const& iFilePath);

// ***************************************************************************
/*!
    @ingroup    Test
    @brief      ファイル・リスト獲得
    @details    指定フォルダ内のファイル(フォルダ除く)のリストを返却する。<br>
                　　ファイル名(拡張子含む)が正規表現iRegexとマッチするもののみ。<br>
                　　iRegex=""の時は全て返却。<br>
                　　リスト取得後、std:sort()でソートして返却する。<br>
<br>
                注意事項<br>
                　　正規表現のマッチング処理はstd::wregexとregex_matchにより実装。<br>
                　　MSVC 2015とMinGW 4.9.2は、サロゲートペアに非対応なので注意。<br>
*/
// ***************************************************************************

THEOLIZER_INTERNAL_DLL
std::vector<std::string> getFileList(std::string const& iDirPath, std::string const& iRegex="");

// ***************************************************************************
/*!
    @ingroup    Test
    @brief      アクセス許可設定
    @details    ownerの書き込み許可をセット／クリアする
*/
// ***************************************************************************

THEOLIZER_INTERNAL_DLL void setWritePermission(const u8string& iPath, bool iIsEnable);

// ***************************************************************************
/*!
    @ingroup    Test
    @brief      スレッド・オブジェクト<br>
    @details    コンストラクタで指定スレッドを生成し、<br>
                デストラクタでそのスレッドの終了を待つ。<br>
<br>
                ThreadIdは、boost::interprocess::ipcdetail::get_current_thread_id()<br>
                にて取得する。<br>
                std::this_thread::get_id()が返す型は扱いにくいため。<br>
*/
// ***************************************************************************

class ThreadGuard : public std::thread
{
private:
    std::string mThreadId;
    void        setThreadId()
    {
        std::stringstream ss;
        ss << std::setfill('_') << std::setw(15) << get_id();
        mThreadId=ss.str();
    }

public :
    //! @ingroup Test
    //! デフォルト・コンストラクタ
    ThreadGuard() noexcept
    {
        setThreadId();
    }

    //! @ingroup Test
    //! 汎用コンストラクタ
    template <class Fn, class... Args>
    ThreadGuard(Fn&& fn, Args&&... args) : thread(fn, args...)
    { setThreadId(); }

    //! @ingroup Test
    //! コピー禁止
    ThreadGuard           (const ThreadGuard&)  = delete;
    ThreadGuard& operator=(const ThreadGuard&)  = delete;

    //! @ingroup Test
    //! ムーブ可
    ThreadGuard(ThreadGuard&& iThread) noexcept :
            thread(static_cast<std::thread&&>(iThread)),
            mThreadId(std::move(iThread.mThreadId))
    { }

    //! @ingroup Test
    //! ムーブ演算子
    ThreadGuard& operator=(ThreadGuard&& iTheread)
    {
        static_cast<std::thread&>(*this)=static_cast<std::thread&&>(iTheread);
        mThreadId=std::move(iTheread.mThreadId);
        return *this;
    }

    //! @ingroup Test
    //! デストラクタ
    ~ThreadGuard () {if (thread::joinable()) thread::join();}

    //! @ingroup Test
    //! スレッドID返却
    std::string const& getThreadId() {return mThreadId;}
};

//############################################################################
//      テスト用マクロ
//############################################################################

#ifndef THEOLIZER_INTERNAL_DOXYGEN

#define THEOLIZER_INTERNAL_PASS  "<<<PASS>>>"
#define THEOLIZER_INTERNAL_FAIL  "<<<FAIL>>>"

namespace internal
{
    THEOLIZER_INTERNAL_DLL extern unsigned     gTotal;
    THEOLIZER_INTERNAL_DLL extern unsigned     gFailCount;
    THEOLIZER_INTERNAL_DLL extern bool         gAbortedOutput;
    THEOLIZER_INTERNAL_DLL bool checkFailCount();
    THEOLIZER_INTERNAL_DLL void lockMutex();
    THEOLIZER_INTERNAL_DLL void unlockMutex();
}   // namespace internal

#endif //THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//      内部処理用マクロ
// ***************************************************************************

#ifndef THEOLIZER_INTERNAL_DOXYGEN

#define THEOLIZER_INTERNAL_U8(dString)          u8##dString
#define THEOLZIER_INTERNAL_FIRST(dFirst, ...)   (dFirst)

//      ---<<< 判定と表示用マクロ >>>---

#define THEOLIZER_INTERNAL_RESULT(dResult)                                  \
    do                                                                      \
    {                                                                       \
        if ((!aIsPass && theolizer::internal::checkFailCount()) || theolizer::DisplayPass::on())\
        {                                                                   \
            std::ostream& os=theolizer::internal::getOStream();             \
            std::streamsize precision=os.precision();                       \
            os.precision(std::numeric_limits<long double>::digits10);       \
            os << THEOLIZER_INTERNAL_U8(#dResult) " : " << (dResult) << "\n";\
            os.precision(precision);                                        \
        }                                                                   \
    }                                                                       \
    while(0)

#define THEOLIZER_INTERNAL_RESULT_FOR(...)                                  \
    THEOLIZER_INTERNAL_FOR(THEOLIZER_INTERNAL_RESULT, __VA_ARGS__)

//      --- ここでgTestMutexをロックする ---

#define THEOLIZER_INTERNAL_JUDGE_ONLY(dAbort, dJudge)                       \
    do                                                                      \
    {                                                                       \
        if (!(dJudge))                                                      \
        {                                                                   \
            theolizer::internal::lockMutex();                               \
            theolizer::internal::gFailCount++;                              \
            aIsPass=false;                                                  \
            if (theolizer::internal::checkFailCount()) {                    \
                theolizer::internal::getOStream() << "\n" THEOLIZER_INTERNAL_FAIL\
                    << ((dAbort)?"(skiped following tests)":"") << "\n";    \
            }                                                               \
        }                                                                   \
        else                                                                \
        {                                                                   \
            theolizer::internal::lockMutex();                               \
        }                                                                   \
        theolizer::internal::gTotal++;                                      \
    }                                                                       \
    while(0)

#define THEOLIZER_INTERNAL_JUDGE(dAbort, dJudge, dExpression)               \
    do                                                                      \
    {                                                                       \
        THEOLIZER_INTERNAL_JUDGE_ONLY(dAbort, dJudge);                      \
        if (!aIsPass && theolizer::internal::checkFailCount())              \
        {                                                                   \
            theolizer::internal::getOStream()                               \
                << THEOLIZER_INTERNAL_FILE << "("  << __LINE__ << ")\n"     \
                << "Expression : " << (u8##dExpression) << "\n";            \
        }                                                                   \
        else if (theolizer::DisplayPass::on())                              \
        {                                                                   \
            theolizer::internal::getOStream() << "\n" THEOLIZER_INTERNAL_PASS "\n"\
                << THEOLIZER_INTERNAL_FILE << "("  << __LINE__ << ")\n"     \
                << "Expression : " << (u8##dExpression) << "\n";            \
        }                                                                   \
    }                                                                       \
    while(0)

//      --- ポインタ表示用のマクロ ---
//      char*は文字列として出力されるのでそれを回避する

namespace internal
{
    template<typename tType>
    tType const* OutputData(tType const* iType)
    {
        return iType;
    }

    inline void const* OutputData(char const* iCharPtr)
    {
        return reinterpret_cast<void const*>(iCharPtr);
    }
}

#define THEOLIZER_INTERNAL_RESULT_PTR(dResult)                              \
    do                                                                      \
    {                                                                       \
        if ((!aIsPass && theolizer::internal::checkFailCount()) || theolizer::DisplayPass::on())\
        {                                                                   \
            std::ostream& os=theolizer::internal::getOStream();             \
            std::streamsize precision=os.precision();                       \
            os.precision(std::numeric_limits<long double>::digits10);       \
            os << THEOLIZER_INTERNAL_U8(#dResult) " : "                     \
               << theolizer::internal::OutputData(dResult) << "\n";         \
            os.precision(precision);                                        \
        }                                                                   \
    }                                                                       \
    while(0)

#endif //THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//      結果判定マクロ群
//          dStatement  例外の発生をチェックする文
//          dException  この例外が発生すればPass
//                      catch(dException)と定義している。
//          dJudge      判定式(true時Pass)
//          dResult     処理結果
// ***************************************************************************

//      ---<<< 等しいことをチェックする(Fail時、処理継続) >>>---
//          最も良く使うので専用で用意する

//! @ingroup Test
//! 値が一致することをテストする。（ポインタ以外）
#define THEOLIZER_EQUAL(dLhs, ...)                                          \
    do                                                                      \
    {                                                                       \
        bool    aIsPass=true;                                               \
        THEOLIZER_INTERNAL_JUDGE_ONLY(false,                                \
            ((dLhs)==THEOLZIER_INTERNAL_FIRST(__VA_ARGS__)));               \
        if (!aIsPass && theolizer::internal::checkFailCount()) {            \
            theolizer::internal::getOStream()                               \
                << THEOLIZER_INTERNAL_FILE << "("  << __LINE__ << ")\n";    \
        } else if (theolizer::DisplayPass::on()) {                          \
            theolizer::internal::getOStream() << "\n" THEOLIZER_INTERNAL_PASS "\n"\
                << THEOLIZER_INTERNAL_FILE << "("  << __LINE__ << ")\n";    \
        }                                                                   \
        THEOLIZER_INTERNAL_RESULT(dLhs);                                    \
        THEOLIZER_INTERNAL_RESULT_FOR(__VA_ARGS__);                         \
        theolizer::internal::unlockMutex();                                 \
    }                                                                       \
    while(0)

//      ---<<< ポインタが等しいことをチェックする(Fail時、処理継続) >>>---
//      char型へのポインタの値を出力しようとすると文字列として出力される。
//      ポインタ値がnullptrだったら落ちる。

//! @ingroup Test
//! ポインタの値が一致することをテストする。
#define THEOLIZER_EQUAL_PTR(dLhs, dRhs)                                     \
    do                                                                      \
    {                                                                       \
        bool    aIsPass=true;                                               \
        THEOLIZER_INTERNAL_JUDGE_ONLY(false, (dLhs) == (dRhs));             \
        if (!aIsPass && theolizer::internal::checkFailCount()) {            \
            theolizer::internal::getOStream()                               \
                << THEOLIZER_INTERNAL_FILE << "("  << __LINE__ << ")\n";    \
        } else if (theolizer::DisplayPass::on()) {                          \
            theolizer::internal::getOStream() << "\n" THEOLIZER_INTERNAL_PASS "\n"\
                << THEOLIZER_INTERNAL_FILE << "("  << __LINE__ << ")\n";    \
        }                                                                   \
        THEOLIZER_INTERNAL_RESULT_PTR(dLhs);                                \
        THEOLIZER_INTERNAL_RESULT_PTR(dRhs);                                \
        theolizer::internal::unlockMutex();                                 \
    }                                                                       \
    while(0)

//      ---<<< 結果をチェックする(Fail時、処理継続) >>>---

//! @ingroup Test
//! 結果がtrueになることをテストする。
#define THEOLIZER_CHECK(dJudge, ...)                                        \
    do                                                                      \
    {                                                                       \
        bool    aIsPass=true;                                               \
        THEOLIZER_INTERNAL_JUDGE(false, dJudge, #dJudge);                   \
        THEOLIZER_INTERNAL_RESULT_FOR(__VA_ARGS__);                         \
        theolizer::internal::unlockMutex();                                 \
    }                                                                       \
    while(0)

//      ---<<< 結果をチェックする(Fail時、処理中断) >>>---

//! @ingroup Test
//! 結果がtrueになることをテストする。（Fail時、以降の処理を中断する）
#define THEOLIZER_REQUIRE(dJudge, ...)                                      \
    do                                                                      \
    {                                                                       \
        bool    aIsPass=true;                                               \
        THEOLIZER_INTERNAL_JUDGE(true, dJudge, #dJudge);                    \
        THEOLIZER_INTERNAL_RESULT_FOR(__VA_ARGS__);                         \
        theolizer::internal::unlockMutex();                                 \
        if (!aIsPass) theolizer::internal::throwAbort();                    \
    }                                                                       \
    while(0)

//      ---<<< 例外が発生することをチェックする(Fail時、処理継続) >>>---

//! @ingroup Test
//! 例外が発生することをチェックする(Fail時、処理継続)
#define THEOLIZER_CHECK_EXCEPTION(dStatements, dException)                  \
    do                                                                      \
    {                                                                       \
        bool    aIsPass=true;                                               \
        try                                                                 \
        {                                                                   \
            dStatements                                                     \
            aIsPass=false;                                                  \
            THEOLIZER_INTERNAL_JUDGE(false, false, "No exception(" #dException ")");\
        }                                                                   \
        catch (dException)                                                  \
        {                                                                   \
            THEOLIZER_INTERNAL_JUDGE(false, true, "exception(" #dException ")");\
        }                                                                   \
        theolizer::internal::unlockMutex();                                 \
    }                                                                       \
    while(0)

//      ---<<< 例外が発生することをチェックする(Fail時、処理中断) >>>---

//! @ingroup Test
//! 例外が発生することをチェックする(Fail時、処理中断)
#define THEOLIZER_REQUIRE_EXCEPTION(dStatements, dException)                \
    do                                                                      \
    {                                                                       \
        bool    aIsPass=true;                                               \
        try                                                                 \
        {                                                                   \
            dStatements                                                     \
            aIsPass=false;                                                  \
            THEOLIZER_INTERNAL_JUDGE(true, false, "No exception(" #dException ")");\
        }                                                                   \
        catch (dException)                                                  \
        {                                                                   \
            THEOLIZER_INTERNAL_JUDGE(false, true, "exception(" #dException ")");\
        }                                                                   \
        theolizer::internal::unlockMutex();                                 \
        if (!aIsPass) theolizer::internal::throwAbort();                    \
    }                                                                       \
    while(0)

//      ---<<< 例外が発生することと結果をチェックする(Fail時、処理継続) >>>---

//! @ingroup Test
//! 例外が発生することをチェックする(Fail時、処理継続)
#define THEOLIZER_CHECK_EXCEPTION2(dStatement, dException, dJudge, dResult) \
    do                                                                      \
    {                                                                       \
        bool    aIsPass=true;                                               \
        try                                                                 \
        {                                                                   \
            dStatement;                                                     \
            aIsPass=false;                                                  \
            THEOLIZER_INTERNAL_JUDGE(false, false, "No exception(" #dException ")");\
        }                                                                   \
        catch (dException)                                                  \
        {                                                                   \
            THEOLIZER_INTERNAL_JUDGE(false, dJudge, #dJudge);               \
            THEOLIZER_INTERNAL_RESULT(dResult);                             \
        }                                                                   \
        theolizer::internal::unlockMutex();                                 \
    }                                                                       \
    while(0)

//      ---<<< 例外が発生することと結果をチェックする(Fail時、処理中断) >>>---

//! @ingroup Test
//! 例外が発生することをチェックする(Fail時、処理中断)
#define THEOLIZER_REQUIRE_EXCEPTION2(dStatement, dException, dJudge, dResult)\
    do                                                                      \
    {                                                                       \
        bool    aIsPass=true;                                               \
        try                                                                 \
        {                                                                   \
            dStatement;                                                     \
            aIsPass=false;                                                  \
            THEOLIZER_INTERNAL_JUDGE(true, false, "No exception(" #dException ")");\
        }                                                                   \
        catch (dException)                                                  \
        {                                                                   \
            THEOLIZER_INTERNAL_JUDGE(true, dJudge, #dJudge);                \
            THEOLIZER_INTERNAL_RESULT(dResult);                             \
        }                                                                   \
        theolizer::internal::unlockMutex();                                 \
        if (!aIsPass) theolizer::internal::throwAbort();                    \
    }                                                                       \
    while(0)

//############################################################################
//      End
//############################################################################

}   // namespace theolizer

// ***************************************************************************
//          DLL用の警告禁止解除
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#endif  // THEOLIZER_INTERNAL_TEST_TOOL_H
