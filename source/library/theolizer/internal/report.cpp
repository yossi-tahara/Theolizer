//############################################################################
//      Theolizerシステム部
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

#include <ctime>

#include <fstream>
#include <sstream>
#include <iomanip>
#include <mutex>
#include <thread>
#include <chrono>
#include <tuple>
#include <algorithm>    // std::copy_if

namespace stdC=std::chrono;
using mSec=stdC::milliseconds;
using uSec=stdC::microseconds;

#define BOOST_ALL_NO_LIB        // Auto-linkを禁止する
                                //  #pragma comment(lib ...)があると
                                //  exeのリンク時にリンクされてしまう。
THEOLIZER_INTERNAL_DISABLE_WARNING()
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
namespace boostF = boost::filesystem;

#include "internal.h"

//############################################################################
//      Begin
//############################################################################

namespace theolizer
{
namespace internal
{

//############################################################################
//      ユーティリティ
//############################################################################

// ***************************************************************************
//      gmtimeとlocaltimeのスレッドセーフ版
// ***************************************************************************

struct tm getGmTime(time_t const& iTime)
{
    struct tm  aTm;

#if defined(_WIN32)
    if (gmtime_s(&aTm, &iTime))
    {
        aTm={};
    }
#else
    if (gmtime_r(&iTime, &aTm) == nullptr)
    {
        aTm={};
    }
#endif

    return aTm;
}
struct tm getLocalTime(time_t const& iTime)
{
    struct tm  aTm;

#if defined(_WIN32)
    if (localtime_s(&aTm, &iTime))
    {
        aTm={};
    }
#else
    if (localtime_r(&iTime, &aTm) == nullptr)
    {
        aTm={};
    }
#endif

    return aTm;
}

//############################################################################
//      ASSERT/ABORTマクロ
//############################################################################

void abort(char const* iFile, unsigned iLine, theolizer::u8string const& iMessage)
{
    std::cout << iFile << "(" << iLine << ") : Theolizer error!!\n"
              << iMessage << std::endl;
    exit(1);
}

//############################################################################
//      theolizer::printf
//          boost::formatの書式を用いるC++的なprintf
//############################################################################

// ***************************************************************************
//      boost::formatのラッパ
// ***************************************************************************

// ---------------------------------------------------------------------------
//      Formatの内部実装クラス
// ---------------------------------------------------------------------------

struct Format::Indep
{
    boost::format   mFormat;
    Indep(u8string const& iFormat) : mFormat(iFormat.str())
    { }

    u8string str()
    {
        return mFormat.str();
    }
};

// ---------------------------------------------------------------------------
//      コンストラクタ
// ---------------------------------------------------------------------------

Format::Format(u8string const& iFormat) noexcept :
    mIndep(std::unique_ptr<Indep>(new Indep(iFormat)))
{
}

// ---------------------------------------------------------------------------
//      デストラクタ
// ---------------------------------------------------------------------------

Format::~Format()
{
}

// ---------------------------------------------------------------------------
//      ムーブ
// ---------------------------------------------------------------------------

Format::Format(Format&& iRhs) : mIndep(std::move(iRhs.mIndep))
{
}

Format& Format::operator=(Format&& iRhs)
{
    if (&iRhs != this) mIndep=std::move(iRhs.mIndep);
    return *this;
}

// ---------------------------------------------------------------------------
//      クリア
// ---------------------------------------------------------------------------

Format& Format::clear()
{
    mIndep->mFormat.clear();
    return *this;
}

// ---------------------------------------------------------------------------
//      出力演算子
// ---------------------------------------------------------------------------

#define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSymbol)                    \
    Format& Format::operator%(dType iValue)       {mIndep->mFormat % iValue;return *this;}
#define THEOLIZER_INTERNAL_DEF_CHAR(dType, dSymbol)                         \
    Format& Format::operator%(dType const* iValue){mIndep->mFormat % iValue;return *this;}
Format& Format::operator%(u8string const& iValue) {mIndep->mFormat % iValue;return *this;}

#define THEOLIZER_INTERNAL_DEF_BOOL(dType, dSymbol)
#define THEOLIZER_INTERNAL_DEF_STRING(dType, dSymbol)
#define THEOLIZER_INTERNAL_DEF_WCHAR(dType, dSymbol)
#define THEOLIZER_INTERNAL_DEF_CHAR16(dType, dSymbol)
#define THEOLIZER_INTERNAL_DEF_CHAR32(dType, dSymbol)
#include "primitive.inc"

Format& Format::operator%(void* iValue) {mIndep->mFormat % iValue;return *this;}

// ---------------------------------------------------------------------------
//      文字列で取出し
// ---------------------------------------------------------------------------

u8string Format::str()
{
    return mIndep->str();
}

Format::operator u8string()
{
    return str();
}

//############################################################################
//      Theolizerエラー通知
//############################################################################

// ***************************************************************************
//      AutoSet<>実体定義
//      シングルトンとするため、ここでstatic定義する。
//          MinGWではヘッダで定義するとexeとdllで別インスタンスになってしまう。
//          おそらくgccも同じ。
// ***************************************************************************

template<typename tData, WeakData<tData>* tWeakData>
AutoSet<tData, tWeakData>& AutoSet<tData, tWeakData>::getInstance()
{
    static AutoSet instance;
    return instance;
}

// ***************************************************************************
//      エラー・ログのデフォルト・ファイル・パス("")
// ***************************************************************************

WeakData<char const*>   gErrorLogPath;

// DLL用に実体化する
template class AutoSet<char const*, &gErrorLogPath>;

}   // namespace internal

// ***************************************************************************
//      エラー種別
// ***************************************************************************

std::ostream& operator<<(std::ostream& iOStream, ErrorType iErrorType)
{
    switch(iErrorType)
    {
    case ErrorType::None:
        iOStream << "None";
        break;

    case ErrorType::Warning:
        iOStream << "Warning";
        break;

    case ErrorType::Error:
        iOStream << "Error";
        break;

    default:
        THEOLIZER_INTERNAL_ABORT("Unknown ErrorType in operator<<(ErrorType)");
        break;
    }

    return iOStream;
}

// ***************************************************************************
//      エラー分類
// ***************************************************************************

std::ostream& operator<<(std::ostream& iOStream, ErrorKind iErrorKind)
{
    switch(iErrorKind)
    {
    case ErrorKind::Unclassified:
        iOStream << "Unclassified";
        break;

    case ErrorKind::WrongUsing:
        iOStream << "WrongUsing";
        break;

    case ErrorKind::IOError:
        iOStream << "IOError";
        break;

    case ErrorKind::UnknownData:
        iOStream << "UnknownData";
        break;

    case ErrorKind::UnknownVerson:
        iOStream << "UnknownVerson";
        break;

    default:
        THEOLIZER_INTERNAL_ABORT("Unknown ErrorKind in operator<<(ErrorKind)");
        break;
    }

    return iOStream;
}

// ***************************************************************************
//      エラー情報全部
// ***************************************************************************

//      ---<<< 文字列 >>>---

std::string ErrorInfo::getString() const
{
    std::stringstream ss;
    ss << mErrorType;
    if (mErrorType != ErrorType::None)
    {
        ss << "(" << mErrorKind << ")," << mAdditionalInfo << " : " << mMessage;
    }
    return ss.str();
}

//      ---<<< ostream >>>---

std::ostream& operator<<(std::ostream& iOStream, ErrorInfo iErrorInfo)
{
    iOStream << iErrorInfo.getString();

    return iOStream;
}

// ***************************************************************************
//      エラー通知(例外 or getError())
// ***************************************************************************

void ErrorInfo::setMessage(u8string const& iMessage)
{
#if defined(__MINGW32__)
    std::copy_if(iMessage.str().begin(), iMessage.str().end(), mMessage,
        [](char c){return (c != 0);});
#else
    mMessage=iMessage;
#endif

}

void ErrorInfo::setAdditionalInfo(u8string const& iAdditionalInfo)
{
#if defined(__MINGW32__)
    std::copy_if(iAdditionalInfo.str().begin(), iAdditionalInfo.str().end(), mAdditionalInfo,
        [](char c){return (c != 0);});
#else
    mAdditionalInfo=iAdditionalInfo;
#endif
}

// ***************************************************************************
//      ErrorReporter実体定義
//      シングルトンとするため、ここでstatic定義する。
//          MinGWではヘッダで定義するとexeとdllで別インスタンスになってしまう。
//          おそらくgccも同じ。
// ***************************************************************************

ErrorReporter& ErrorReporter::getInstance()
{
    thread_local static ErrorReporter instance;
    return instance;
}

// ***************************************************************************
//      ログ出力
//          ログ・ファイルが指定されていたら、出力してtrue返却
// ***************************************************************************

bool ErrorReporter::writeErrorLog
(
    u8string const& iMessage,
    ErrorType       iErrorType,
    ErrorKind       iErrorKind,
    char const*     iFileName,
    unsigned        iLineNo
)
{
    u8string aAdditionalInfo;
    if (mAdditionalInfo)
    {
        aAdditionalInfo=mAdditionalInfo->getString();
    }

    // エラー記録
    if (!mProcessing)
    {
        mErrorInfo.mErrorType       =iErrorType;
        mErrorInfo.mErrorKind       =iErrorKind;
        mErrorInfo.mFileName        =iFileName;
        mErrorInfo.mLineNo          =iLineNo;
        mErrorInfo.setMessage(iMessage);
        mErrorInfo.setAdditionalInfo(aAdditionalInfo);
    }

    // エラー・ログ・ファイル出力有りの時のみ出力する
    char const* aErrorLogPath=
        THEOLIZER_INTERNAL_GET_WEAK_DATA(internal::gErrorLogPath, char const*);
    if (!aErrorLogPath)
return false;

    // エラー・ログ出力
    static WorkingLog instance(aErrorLogPath);
    WorkingLog::LogStream log=instance.getLogStream();
    log << iErrorType;
    if (iErrorType != ErrorType::None)
    {
        log << "(" << iErrorKind << ")," << aAdditionalInfo << " : " << iMessage;
        if (mProcessing)
        {
            log << "[under exception]";
        }
        if (iFileName)
        {
            log << "{" << iFileName << "(" << iLineNo << ")}";
        }
    }

    return true;
}

// ***************************************************************************
//      遅延例外
// ***************************************************************************

void ErrorReporter::throwDeferredImpl
(
    u8string const&  iMessage,
    ErrorKind        iErrorKind,
    char const*      iFileName,
    unsigned         iLineNo
)
{
    // mErrorInfo設定とログ出力
    writeErrorLog(iMessage, ErrorType::Error, iErrorKind, iFileName, iLineNo);

    // エラー処理中であったら、ログのみ
    if (mProcessing)
return;

    // エラー処理中とする
    mProcessing=true;

    // リソース開放中でなければ、例外を投げる
    if (mReleaseCount == 0)
    {
        mIsDeferred=false;
        std::cout.flush();
throw mErrorInfo;
    }

    // リソース開放中なら、例外を保留する
    mIsDeferred=true;
}

// ***************************************************************************
//      警告メッセージを記録
//          ログ・ファイルが指定されていたら、ログへ出力してtrue返却
// ***************************************************************************

bool ErrorReporter::writeWarningImpl
(
    u8string const& iMessage,
    ErrorKind       iErrorKind,
    char const*     iFileName,
    unsigned        iLineNo
) noexcept
{
    // mErrorInfo設定とログ出力
    return writeErrorLog(iMessage, ErrorType::Warning, iErrorKind, iFileName, iLineNo);
}

// ***************************************************************************
//      エラー・クリア
// ***************************************************************************

void ErrorReporter::clearError() noexcept
{
    THEOLIZER_INTERNAL_ASSERT(mReleaseCount == 0, "Can not clearError() in Releasing.");

    mErrorInfo=ErrorInfo();
}

// ***************************************************************************
//      エラー処理用
// ***************************************************************************

namespace internal
{
//----------------------------------------------------------------------------
//      std::mutex保持用
//----------------------------------------------------------------------------

struct ErrorBase::Impl
{
    std::mutex  mMutex;
};

//----------------------------------------------------------------------------
//      コンストラクタ／デストラクタ
//----------------------------------------------------------------------------

ErrorBase::ErrorBase() noexcept :
    mConstructorError(false),
    mImpl(std::unique_ptr<Impl>(new Impl()))
{
}

ErrorBase::~ErrorBase() noexcept
{
}

//----------------------------------------------------------------------------
//      エラー情報受領(排他制御する)
//          エラー／警告が発生していないなら受け取らない
//----------------------------------------------------------------------------

void ErrorBase::setError(ErrorInfo const& iErrorInfo, bool iConstructor) noexcept
{
    std::unique_lock<std::mutex> aLock(mImpl->mMutex);

    if (iErrorInfo)
    {
        mConstructorError=iConstructor;
        mErrorInfo=iErrorInfo;
    }
}

//----------------------------------------------------------------------------
//      エラー状態を解除する
//----------------------------------------------------------------------------

void ErrorBase::resetError()
{
    if (mConstructorError)
    {
        THEOLIZER_INTERNAL_ERROR(u8"Can not reset an error during the constructing.");
    }
    mErrorInfo=ErrorInfo();
}

//----------------------------------------------------------------------------
//      エラー・チェック
//          エラーが発生し、それがクリアされてない時は例外を投げる
//          これは、シリアライズ・エラー後リカバリ処理しない限り、
//          続くシリアライズ処理を確実にエラーにするための処理。
//----------------------------------------------------------------------------

void ErrorBase::checkError()
{
    if (isError())
    {
        theolizer::internal::throwDeferred
        (
            theolizer::print(mErrorInfo.getMessage()),
            ErrorKind::WrongUsing
        );
    }
}

}   // namespace internal

//############################################################################
//      デバッグ・ログ
//          ログ・ファイルの先頭行はファイルのシーケンシャル番号
//          順次インクリメントされる。(最大値の次は0なので注意)
//############################################################################

// ***************************************************************************
//      WorkingLogの実装クラス
// ***************************************************************************

struct WorkingLog::Impl
{
    friend  WorkingLog;
    friend  WorkingLog::LogStream;

//      ---<<< メンバー変数 >>>---

    u8string        mPath;
    std::ofstream   mOfstream;
    std::mutex      mMutex;

    size_t          mFileSize;
    unsigned        mFileCount;
    unsigned        mNowNumber;
    bool            mFirst;

//----------------------------------------------------------------------------
//      ログ・ファイル・オープン管理
//          指定サイズを超えていたら、クローズする
//          オープンしてなければ、オープンする
//----------------------------------------------------------------------------

    void openOfstream(bool iFirst=false)
    {
        if (mOfstream.is_open())
        {
            std::size_t size = static_cast<std::size_t>(mOfstream.tellp());
            if (mFileSize < size)
            {
                mOfstream.close();
                mNowNumber++;
            }
        }

        if (!mOfstream.is_open())
        {
            unsigned aFileNumber = mNowNumber % mFileCount;
            u8string aFileName=std::move((boost::format(mPath.str())% aFileNumber).str());

            if (iFirst)
            {
                mOfstream.open(aFileName.get_fstring(),
                               std::ofstream::out | std::ofstream::app);
            }
            else
            {
                mOfstream.open(aFileName.get_fstring(),
                               std::ofstream::out | std::ofstream::trunc);
            }
            THEOLIZER_INTERNAL_ASSERT
            (
                ((mOfstream.rdstate() & std::ifstream::failbit) == 0),
                u8"WorkingLog : ログ・ファイル<%1%>を開けませんでした。", aFileName
            );
            mOfstream.seekp(0, std::ios_base::end);
            if (static_cast<long>(mOfstream.tellp()) == 0) {
                mOfstream << mNowNumber << "\n";
            }
            mFirst=false;
        }
    }

//----------------------------------------------------------------------------
//      コンストラクタ
//----------------------------------------------------------------------------

    Impl(const u8string& iPath, size_t iFileSize, unsigned iFileCount) :
        mPath(iPath),
        mFileSize(iFileSize),
        mFileCount(iFileCount),
        mNowNumber(0),
        mFirst(true)
    {
        boostF::path aPath(mPath.get_bstring());

        // ファイル名取出し
        boostF::path aFileName = aPath.filename();
        THEOLIZER_INTERNAL_ASSERT
        (
            (aFileName.native() != u8string(".")),
            u8"WorkingLog : iPath(%1%)にはファイル名も入れて下さい。", iPath
        );

        // %1%が含まれることをチェック
        size_t found = aFileName.native().find(u8string("%1%"));
        THEOLIZER_INTERNAL_ASSERT
        (
            (found != std::string::npos),
            u8"WorkingLog : iPath(%1%)に%%1%%を１つ含めて下さい。", iPath
        );

        // フォルダ・パス取出し
        boostF::path aFolder = aPath.parent_path();
        // 空なら、カレント指定
        if (aFolder.empty()) {
            aFolder=L"./";
        }

        // %1%が含まれないことをチェック
        found = aFolder.native().find(u8string("%1%"));
        THEOLIZER_INTERNAL_ASSERT
        (
            (found == std::string::npos),
            u8"WorkingLog : iPath(%1%)のフォルダ・パスに%%1%%を含めないで下さい。", iPath
        );

        // フォルダが存在しない時のために、フォルダを生成しておく
        if (aFolder != L"./")   // カレントなら生成しない(これがないとboost1.60.0で異常動作する)
        {
            boost::system::error_code error;
            boostF::create_directories(aFolder, error);
            THEOLIZER_INTERNAL_ASSERT
            (
                !error,
                u8"WorkingLog : %1%がフォルダではありません。", u8string(aFolder.native())
            );
        }

        // 最初に用いるログ・ファイル番号獲得 
        bool    aIsFirst=true;

        mNowNumber=0;
        for (unsigned i=0; i < mFileCount; ++i)
        {
            u8string aFileName2=std::move((boost::format(mPath.str())%i).str());
            // 通常のファイルであることを確認
            boostF::path temp(aFileName2.get_bstring()); 
            if (!boostF::exists(temp))
        continue;
            THEOLIZER_INTERNAL_ASSERT
            (
                (boostF::is_regular_file(temp)),
                u8"WorkingLog : %1%がファイルではありません。取り除いて下さい。", aFileName2
            );
            // オープンして先頭行チェック
            std::ifstream ifs(aFileName2.get_fstring());
            unsigned    aNowNumber;
            ifs >> aNowNumber;
            if (aIsFirst
             || ((aNowNumber-mNowNumber) == 1)) {
                aIsFirst=false;
                mNowNumber=aNowNumber;
            }
        }

        // ログ・ファイル・オープン
        openOfstream(true);
    }

//----------------------------------------------------------------------------
//      デストラクタ
//----------------------------------------------------------------------------

    ~Impl() {}

};

// ***************************************************************************
//      Theolizerログ用出力ストリーム
// ***************************************************************************

//----------------------------------------------------------------------------
//      通常コンストラクタ(ロックと行ヘッダ出力)
//----------------------------------------------------------------------------

WorkingLog::LogStream::LogStream(WorkingLog* iWorkingLog) :
                        mWorkingLog(iWorkingLog)
{
    // Mutex獲得
    internal::FineTimer   ft;
    std::unique_lock<std::mutex> aLock(mWorkingLog->mImpl->mMutex);
    int64_t     aWaitedMicroseconds=ft.GetuSec();
    // 現在時刻獲得
    stdC::system_clock::time_point aNowDateTime = stdC::system_clock::now();

    // ログ・ファイルのオープン管理
    mWorkingLog->mImpl->openOfstream();

    // 行ヘッダ計算
    time_t aTime = stdC::system_clock::to_time_t(aNowDateTime);
    struct std::tm aTm = (mWorkingLog->mIsUTC)?
                            (internal::getGmTime(aTime))
                            :
                            (internal::getLocalTime(aTime));

    uint32_t aMilliseconds
        = stdC::duration_cast<mSec>(aNowDateTime.time_since_epoch()).count()%1000;

    mWorkingLog->getOStream()
        << "," << std::setw(4)                      << (1900+aTm.tm_year)
        << "-" << std::setfill('0') << std::setw(2) << (aTm.tm_mon+1)
        << "-" << std::setfill('0') << std::setw(2) << aTm.tm_mday
        << "," << std::setfill('0') << std::setw(2) << aTm.tm_hour
        << ":" << std::setfill('0') << std::setw(2) << aTm.tm_min
        << ":" << std::setfill('0') << std::setw(2) << aTm.tm_sec
        << "." << std::setfill('0') << std::setw(3) << aMilliseconds
        << "," << std::setfill('0') << std::setw(6) << aWaitedMicroseconds
        << ",_" << std::setfill('_') << std::setw(15) << std::this_thread::get_id()
        << ",";
//mWorkingLog->getOStream() << "[" << ft << "]";

    // 正常終了時、ロック開放しない
    aLock.release();
}

//----------------------------------------------------------------------------
//      ムーブ
//----------------------------------------------------------------------------

WorkingLog::LogStream::LogStream(WorkingLog::LogStream&& iRhs) :
                                        mWorkingLog(iRhs.mWorkingLog)
{
    iRhs.mWorkingLog = nullptr;
}

//----------------------------------------------------------------------------
//      デストラクタ(改行出力とロック開放)
//----------------------------------------------------------------------------

WorkingLog::LogStream::~LogStream() noexcept
{
    if (mWorkingLog)
    {
        mWorkingLog->getOStream() << std::endl;

        // ロック開放
        mWorkingLog->mImpl->mMutex.unlock();
    }
}

// ***************************************************************************
//      Theolizerログ
// ***************************************************************************

//----------------------------------------------------------------------------
//      現在時刻獲得(ログ・システムと同じ時間計測方法で獲得する)
//----------------------------------------------------------------------------

time_t WorkingLog::getTime(bool iIsUTC)
{
    // 現在時刻獲得
    stdC::system_clock::time_point aNowDateTime = stdC::system_clock::now();
    time_t aTime = stdC::system_clock::to_time_t(aNowDateTime);
    if (iIsUTC)
    {
        struct tm  aTm=internal::getGmTime(aTime);
        aTime=mktime(&aTm);
    }
    return aTime;
}

//----------------------------------------------------------------------------
//      コンストラクタ
//----------------------------------------------------------------------------

WorkingLog::WorkingLog(const u8string& iPath,
                       size_t iFileSize, unsigned iFileCount)  :
            mImpl(std::unique_ptr<Impl>(new Impl(iPath, iFileSize, iFileCount))),
            mIsUTC(false)
{
}

//----------------------------------------------------------------------------
//      デストラクタ
//----------------------------------------------------------------------------

WorkingLog::~WorkingLog()
{
}

//----------------------------------------------------------------------------
//      出力ストリーム返却(pImplイディオム対応用)
//----------------------------------------------------------------------------

std::ostream& WorkingLog::getOStream()
{
    return mImpl->mOfstream;
}

//----------------------------------------------------------------------------
//      ログ出力用ストリーム返却
//----------------------------------------------------------------------------

WorkingLog::LogStream WorkingLog::getLogStream()
{
    return LogStream(this);
}

//----------------------------------------------------------------------------
//      行ヘッダ解析
//          0123456789012345678901234567890123456
//          ,yyyy-mm-dd,hh:mm:ss.mmm,wait,_thdid,
//----------------------------------------------------------------------------

bool WorkingLog::getLineHeader(std::string const& iLine,
                               LineHeader& iLineHeader, size_t* iPos) noexcept
{
    // 行ヘッダが不足していたらfalse
    if (iLine.length() < 37)
return false;

    try
    {
        struct std::tm aTm;
        aTm.tm_isdst    =0;
        aTm.tm_year =static_cast<int>(std::stoul(iLine.substr(1))-1900);
        aTm.tm_mon  =static_cast<int>(std::stoul(iLine.substr(6))-1);
        aTm.tm_mday =static_cast<int>(std::stoul(iLine.substr(9)));
        aTm.tm_hour =static_cast<int>(std::stoul(iLine.substr(12)));
        aTm.tm_min  =static_cast<int>(std::stoul(iLine.substr(15)));
        aTm.tm_sec  =static_cast<int>(std::stoul(iLine.substr(18)));
        iLineHeader.mDateTime = mktime(&aTm);
        iLineHeader.mMilliseconds = static_cast<uint32_t>(std::stoul(iLine.substr(21)));

        // 待ち時間取出し
        size_t  aPos=25;
        iLineHeader.mWaitedMicroseconds = static_cast<uint16_t>(std::stoul(iLine.substr(aPos)));

        // TheadId取出し
        aPos=iLine.find(',', aPos);
        if (aPos == std::string::npos)
return false;
        aPos+=2;
        size_t  aPos2 = iLine.find(',', aPos);
        if (aPos2 == std::string::npos)
return false;
        iLineHeader.mThereadId = iLine.substr(aPos, aPos2-aPos);

        // ログ文字列先頭取出し
        if (iPos)
        {
            *iPos = aPos2+1;
        }
        return true;
    }
    catch(...)
    {
    }
    return false;
}

//############################################################################
//      End
//############################################################################

}   // namespace theolizer
