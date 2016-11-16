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
//      scoped enum型表示補助
/*!
    @todo   T.B.D.
*/
// ***************************************************************************

template<typename tEnum, THEOLIZER_INTERNAL_OVERLOAD((std::is_enum<tEnum>::value))>
std::ostream& operator<<(std::ostream& iOStream, tEnum iEnum)
{
    typedef typename std::underlying_type<tEnum>::type  Type;
    iOStream << static_cast<Type>(iEnum);
    return iOStream;
}

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
//      テスト中断用例外を投げる
// ***************************************************************************

THEOLIZER_INTERNAL_DLL void throwAbort();

// ***************************************************************************
//      開放時、関数呼び出し
//          ほぼN4189のscope_exitの名前を変えただけ。
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
        if (!mMoved) {
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

// ファクトリ
/*!
    @todo   T.B.D.
*/
template <typename tReleaser>
internal::ScopeExit<tReleaser> makeScopeExit(tReleaser &&iReleaser) noexcept
{
    return  internal::ScopeExit<typename std::remove_reference<tReleaser>::type>
            (
                std::forward<tReleaser>(iReleaser)
            );
}

// ***************************************************************************
//      PASS結果表示の有無指定
//          trueにするとPASS結果も表示する
// ***************************************************************************

//----------------------------------------------------------------------------
//      ヘルパー・クラス
//----------------------------------------------------------------------------

namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

template<typename tType>
class AutoRestore
{
    tType   mBackup;
    tType&  mTarget;

public:
    explicit AutoRestore(tType& iTarget, tType iInitialValue) :
                    mBackup(iTarget),   // iTargetの値をmBackupへバックアップ
                    mTarget(iTarget)    // iTargetのアドレスをmTargetへ設定
    {
        mTarget=iInitialValue;
    }
    ~AutoRestore()
    {
        mTarget=mBackup;
    }
    AutoRestore(AutoRestore const&) = delete;
    AutoRestore& operator=(AutoRestore const&) = delete;
};

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

//----------------------------------------------------------------------------
//      本体
//----------------------------------------------------------------------------

// 自動回復
/*!
    @todo   T.B.D.
*/
struct THEOLIZER_INTERNAL_DLL DisplayPass : public internal::AutoRestore<bool>
{
/*!
    @todo   T.B.D.
*/
    explicit DisplayPass();

/*!
    @todo   T.B.D.
*/
    static bool on();
};

// ***************************************************************************
//      MinGW不具合対処
//          関数テンプレートで浮動小数点を取り扱う時、最適化上の不具合がある模様
//          処理に割り込んで最適化させないためのダミー関数
/*!
    @todo   T.B.D.
*/
// ***************************************************************************

#if defined(__MINGW32__)
    THEOLIZER_INTERNAL_DLL void cutOptimize();
#else
    inline void cutOptimize() { }
#endif

// ***************************************************************************
//      テスト結果集計用
//
/*!
    @todo   T.B.D.
*/
// ***************************************************************************

/*!
    @todo   T.B.D.
*/
THEOLIZER_INTERNAL_DLL void initResult();

/*!
    @todo   T.B.D.
*/
THEOLIZER_INTERNAL_DLL void incrementTotalCount();

/*!
    @todo   T.B.D.
*/
THEOLIZER_INTERNAL_DLL void incrementFailCount();

/*!
    @todo   T.B.D.
*/
THEOLIZER_INTERNAL_DLL bool printResult();

//############################################################################
//      ファイル操作
//############################################################################

// ***************************************************************************
//      テスト用フォルダの準備
//          もし、iDirPathフォルダが既に存在していたら削除する。
//          次に、iDirPathフォルダを作成する。
//          デストラクタにてiIsNoDeleteがfalseなら、iDirPathフォルダを削除する。
//
//          iDirPathフォルダを生成できなかったら、std::runtime_errorを投げるので、
//          catchしないことにより、テストが中断される。
/*!
    @todo   T.B.D.
*/
// ***************************************************************************

class THEOLIZER_INTERNAL_DLL PrepareDir
{
private:
    std::string mDirPath;
    bool        mIsNoDelete;

public:
/*!
    @todo   T.B.D.
*/
    PrepareDir(std::string const& iDirPath, bool iIsNoDelete=false);
    ~PrepareDir();
};

// ***************************************************************************
//      ファイル／フォルダの有無チェック
// ***************************************************************************

/*!
    @todo   T.B.D.
*/
THEOLIZER_INTERNAL_DLL bool isExist(u8string const& iFilePath);

// ***************************************************************************
//      ファイルの削除
//          指定ファイルが無くても正常終了する。
//          iFilePathがフォルダの場合何もしない。
// ***************************************************************************

/*!
    @todo   T.B.D.
*/
THEOLIZER_INTERNAL_DLL void removeFile(u8string const& iFilePath);

// ***************************************************************************
//      ファイル・リスト獲得
//          指定フォルダ内のファイル(フォルダ除く)のリストを返却する。
//          ファイル名(拡張子含む)が正規表現iRegexとマッチするもののみ。
//          iRegex=""の時は全て返却。
//          リスト取得後、std:sort()でソートして返却する。
//
//      注意事項
//          正規表現のマッチング処理はstd::wregexとregex_matchにより実装。
//          MSVC 2015とMinGW 4.9.2は、サロゲートペアに非対応なので注意。
// ***************************************************************************

/*!
    @todo   T.B.D.
*/
THEOLIZER_INTERNAL_DLL
std::vector<std::string> getFileList(std::string const& iDirPath, std::string const& iRegex="");

// ***************************************************************************
//      アクセス許可設定
//          ownerの書き込み許可をセット／クリアする
// ***************************************************************************

/*!
    @todo   T.B.D.
*/
THEOLIZER_INTERNAL_DLL void setWritePermission(const u8string& iPath, bool iIsEnable);

// ***************************************************************************
//      スレッド・オブジェクト
//          コンストラクタで指定スレッドを生成し、
//          デストラクタでそのスレッドの終了を待つ。
//
//          ThreadIdは、boost::interprocess::ipcdetail::get_current_thread_id()
//          にて取得する。
//          std::this_thread::get_id()が返す型は扱いにくいため。
/*!
    @todo   T.B.D.
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
    // デフォルト・コンストラクタ
/*!
    @todo   T.B.D.
*/
    ThreadGuard() noexcept
    {
        setThreadId();
    }

    // コンストラクタ
/*!
    @todo   T.B.D.
*/
    template <class Fn, class... Args>
    ThreadGuard(Fn&& fn, Args&&... args) : thread(fn, args...)
    { setThreadId(); }

    // コピー禁止
    ThreadGuard           (const ThreadGuard&)  = delete;
    ThreadGuard& operator=(const ThreadGuard&)  = delete;

    // ムーブ有効
/*!
    @todo   T.B.D.
*/
    ThreadGuard(ThreadGuard&& iThread) noexcept :
            thread(static_cast<std::thread&&>(iThread)),
            mThreadId(std::move(iThread.mThreadId))
    { }
/*!
    @todo   T.B.D.
*/
    ThreadGuard& operator=(ThreadGuard&& iTheread)
    {
        static_cast<std::thread&>(*this)=static_cast<std::thread&&>(iTheread);
        mThreadId=std::move(iTheread.mThreadId);
        return *this;
    }

    // デストラクタ
    ~ThreadGuard () {if (thread::joinable()) thread::join();}

/*!
    @todo   T.B.D.
*/
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
        if (!aIsPass || theolizer::DisplayPass::on())                       \
        {                                                                   \
            std::ostream& os=theolizer::internal::getOStream();             \
            std::streamsize precision=os.precision();                       \
            os.precision(std::numeric_limits<long double>::digits10);       \
            os << THEOLIZER_INTERNAL_U8(#dResult) " : " << dResult << "\n"; \
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
            theolizer::internal::getOStream() << "\n" THEOLIZER_INTERNAL_FAIL\
                << ((dAbort)?"(skiped following tests)":"") << "\n";        \
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
        if (!aIsPass)                                                       \
        {                                                                   \
            theolizer::internal::getOStream()                               \
                << THEOLIZER_INTERNAL_FILE << "("  << __LINE__ << ")\n"     \
                << "Expression : " << u8##dExpression << "\n";              \
        }                                                                   \
        else if (theolizer::DisplayPass::on())                              \
        {                                                                   \
            theolizer::internal::getOStream() << "\n" THEOLIZER_INTERNAL_PASS "\n"\
                << THEOLIZER_INTERNAL_FILE << "("  << __LINE__ << ")\n"     \
                << "Expression : " << u8##dExpression << "\n";              \
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

/*! @def    THEOLIZER_EQUAL
    @todo   T.B.D.
*/
#define THEOLIZER_EQUAL(dLhs, ...)                                          \
    do                                                                      \
    {                                                                       \
        bool    aIsPass=true;                                               \
        THEOLIZER_INTERNAL_JUDGE_ONLY(false,                                \
            ((dLhs)==THEOLZIER_INTERNAL_FIRST(__VA_ARGS__)));               \
        if (!aIsPass) {                                                     \
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

//      ---<<< 結果をチェックする(Fail時、処理継続) >>>---

/*! @def    THEOLIZER_CHECK
    @todo   T.B.D.
*/
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

/*! @def    THEOLIZER_REQUIRE
    @todo   T.B.D.
*/
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

/*! @def    THEOLIZER_CHECK_EXCEPTION
    @todo   T.B.D.
*/
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

/*! @def    THEOLIZER_REQUIRE_EXCEPTION
    @todo   T.B.D.
*/
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

/*! @def    THEOLIZER_CHECK_EXCEPTION2
    @todo   T.B.D.
*/
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

/*! @def    THEOLIZER_REQUIRE_EXCEPTION2
    @todo   T.B.D.
*/
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
