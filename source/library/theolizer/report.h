//############################################################################
//      
/*!
    @brief      Theolizerシステム部
    @details    ①Theolizerからのエラー返却
                    エラー・コード
                    エラー通知用構造体
                    エラー・ログ
                    エラー通知用例外(エラー・ログが無い時、例外で通知)
                ②Theolizerの稼働記録
                    Theolizerログ
                ③デバッグ・ツール
                    ASSERT/ABORTマクロ
    @ingroup    TheolizerLib
    @file       report.h
    @author     Yoshinori Tahara
    @date       2015/08/06 Created
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

#if !defined(THEOLIZER_INTERNAL_REPORT_H)
#define THEOLIZER_INTERNAL_REPORT_H

#include "u8string.h"   // インクルード漏れチェックのため最初にインクルード
#include "internal/base.h"

//############################################################################
//      Begin
//############################################################################

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable:4100 4251)
#endif

namespace theolizer
{

//############################################################################
//      ASSERT/ABORTマクロ
//############################################################################

namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//      std::coutへメッセージを出力後、プロセスを強制終了する
// ***************************************************************************

THEOLIZER_INTERNAL_DLL
void abort[[ noreturn ]](char const* iFile, unsigned iLine, u8string const& iMessage);

#define THEOLIZER_INTERNAL_ABORT(...)                                   \
    theolizer::internal::abort                                          \
    (                                                                   \
        THEOLIZER_INTERNAL_FILE,                                        \
        __LINE__,                                                       \
        theolizer::print(__VA_ARGS__)                                   \
    )                                                                   \

// ***************************************************************************
//      !dJudgeの時、std::coutへメッセージを出力後、プロセスを強制終了する
// ***************************************************************************

void asserted(std::string const& iMessage, char const* iFile, unsigned iLine);
#define THEOLIZER_INTERNAL_ASSERT(dJudge, ...)                          \
    do {if (!(dJudge)) THEOLIZER_INTERNAL_ABORT(__VA_ARGS__);}while(0)  \

//############################################################################
//      theolizer::printf
//          boost::formatの書式を用いるC++的なprintf
//############################################################################

// ***************************************************************************
//      boost::formatのラッパ
// ***************************************************************************

class THEOLIZER_INTERNAL_DLL Format
{
private:
    // ヘッダ依存回避用
    struct                  Indep;
    std::unique_ptr<Indep>  mIndep;

    // コピー禁止
    Format(Format const&) = delete;
    Format& operator=(Format const&) = delete;

public:
    // コンストラクタ／デストラクタ
    Format(u8string const& iFormat) noexcept;
    ~Format() noexcept;

    // ムーブ許可
    Format(Format&& iRhs);
    Format& operator=(Format&& iRhs);

    // クリア
    Format& clear();

    // 出力演算子
    #define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSymbol)                \
        Format& operator%(dType iValue);
    #define THEOLIZER_INTERNAL_DEF_CHAR(dType, dSymbol)                     \
        Format& operator%(dType const* iValue);
    Format& operator%(const u8string& iValue);

    #define THEOLIZER_INTERNAL_DEF_BOOL(dType, dSymbol)
    #define THEOLIZER_INTERNAL_DEF_STRING(dType, dSymbol)
    #define THEOLIZER_INTERNAL_DEF_WCHAR(dType, dSymbol)
    #define THEOLIZER_INTERNAL_DEF_CHAR16(dType, dSymbol)
    #define THEOLIZER_INTERNAL_DEF_CHAR32(dType, dSymbol)
    #include "internal/primitive.inc"

    Format& operator%(void* iValue);

    // theolizer::u8stringで取出す
    u8string str();
    operator u8string();
};

// ***************************************************************************
//      可変長テンプレート実装部
// ***************************************************************************

inline u8string printImpl(Format& iFormat)
{
    return iFormat.str();
}

template<typename tFirst, typename... tParams>
u8string printImpl(Format& iFormat, tFirst iFirst, tParams... iParams)
{
    iFormat % iFirst;
    return printImpl(iFormat, iParams...);
}

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

// ***************************************************************************
//      theolizer::print()
// ***************************************************************************

/*!
@brief      boost::formatのラッパ。
@details    operator%ではなく、()内でカンマ区切りで表示したいアイテムを与える。<br>
            書式指定（iFormat）は<a href=http://www.boost.org/doc/libs/release/libs/format/>boost::format</a>参照のこと。
*/
template<typename... tParams>
u8string print(u8string const& iFormat, tParams... iParams)
{
    internal::Format aFormat(iFormat);
    return internal::printImpl(aFormat, iParams...);
}

namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

//############################################################################
//      Theolizerログ
//
//          考え方
//              １種類のログに対して、複数のログ・ファイルを生成する。
//                  iFileSizeを超えたら、次のファイルへ書き込む。
//                  ファイル番号は0からiFileCount-1まで変化する。
//                  若い番号から使用され、iFileCountになったら、0へ戻る。
//          使い方
//              ログ出力は、getLogStream()にてLogStreamクラスのインタンスを得て
//              それに出力を行う。(原則、１行出力したら開放する。)
//              LogStreamのコンストラクタで行ヘッダ(時刻やスレッドID等)
//              が出力される。
//
//          行ヘッダ
//              日付時刻をmSec精度で出力する。デフォルトはローカルタイム。
//                  時刻は下記で取得している。
//                      std::chrono::system_clock::now()
//              また、下記も出力する。
//                  排他制御用Mutexのロックにかかった時間(mSec)
//                  ThreadId
//                      std::this_thread::get_id()
//                      operator<<にて文字列化。
//                      その際、5桁以上とし、空きは'_'で埋めている。
//                      (std::setfill('_'), std::setw(5))
//
//      スレッド・セーフ性：安全
//          LogStreamのインスタンスにて排他制御実施
//############################################################################

// ***************************************************************************
//      WeakData(ログ・ファイル・パス指定用)
//          特に設定用の定義が実体化されなければ初期値のまま
//          使い方
//              使用したいcppにて下記を定義する。
//                  型  WeakData変数名;
//              下記で値を取り出す。
//                  THEOLIZER_INTERNAL_GET_WEAK_DATA(WeakData変数名, 型)
//              そのヘッダ・ファイルにて下記宣言を行う。
//                  extern 型   WeakData変数名;
//              上書きしたいcppにて下記を定義する。
//                  THEOLIZER_INTERNAL_SET_WEAK_DATA(WeakData変数名, 型, 値);
//
//          注意事項
//              tDataは組み込み型限定。
//              自動的に初期化される型は初期化タイミングを制御できないので
//              使えない。
// ***************************************************************************

// ---------------------------------------------------------------------------
//      領域定義用
//          直接編集できないようにするため
// ---------------------------------------------------------------------------

template<typename tData>
class WeakData
{
private:
    template<typename tData2, WeakData<tData2>*> friend class AutoSet;
    tData   mData;
    void    set(const tData& iData)
    {
        mData=iData;
    }
    const tData& get() {return mData;}

public:
    WeakData()
    {
        static_assert(std::is_scalar<tData>::value,
                      "Please use tData is scalar type for WeekData.");
    }
};

// ---------------------------------------------------------------------------
//      設定用
// ---------------------------------------------------------------------------

template<typename tData, WeakData<tData>* tWeakData>
class THEOLIZER_INTERNAL_DLL AutoSet
{
    AutoSet()
    {
        tWeakData->set(tData());
    }
    ~AutoSet() { }

    static AutoSet& getInstance();

public:
    // コピー／ムーブ禁止(デストラクタ定義がないとis_trivially_copyableになる)
    AutoSet(const AutoSet&)  = delete;
    AutoSet(      AutoSet&&) = delete;
    AutoSet& operator=(const AutoSet&)  = delete;
    AutoSet& operator=(      AutoSet&&) = delete;

    // 設定
    static int set(const tData& iData, char const* iFile, int iLineNo, int*)
    {
        getInstance();
        if ((tWeakData->get() != tData())
         && (tWeakData->get() != iData)) {
            std::cout << "\n" << iFile << "(" << iLineNo << ")"
                      << " : Multiple setup the WeakData.\n";
    exit(1);
        }
        tWeakData->set(iData);
        return 0;
    }

    // 返却
    static tData const& get()
    {
        getInstance();
        return tWeakData->get();
    }
};

// ---------------------------------------------------------------------------
//      使用するためのマクロ
//
//          ライブラリ側(デフォルト値を設定する)
//              WeakDataの定義
//                  cpp :        WeakData<型> グローバル変数名;
//                  h   : extern WeakData<型> グローバル変数名;
//              設定されているWeakDataの取出し
//                  cpp : 型 取出し先=THEOLIZER_INTERNAL_GET_WEAK_DATA(グローバル変数名, 型);
//
//          ユーザ側(オプション指定を行う)
//                  cpp : THEOLIZER_INTERNAL_SET_WEAK_DATA(グローバル変数名, 型);
// ---------------------------------------------------------------------------

#define THEOLIZER_INTERNAL_GET_WEAK_DATA(dWeakData, dType)                  \
    theolizer::internal::AutoSet<dType, &dWeakData>::get()

// &dUniqueをset()へ渡しているのはコンパイラの「未使用警告」を消すため
#define THEOLIZER_INTERNAL_SET_WEAK_DATA(dWeakData, dType, dData, dUnique)  \
    namespace                                                               \
    {                                                                       \
        int dUnique=theolizer::internal::AutoSet<dType, &dWeakData>::       \
            set(dData, THEOLIZER_INTERNAL_FILE, __LINE__, &dUnique);        \
    }

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

// ***************************************************************************
//      クラス本体
// ***************************************************************************

//! @todo   T.B.D.
class THEOLIZER_INTERNAL_DLL WorkingLog
{
//----------------------------------------------------------------------------
//      1行出力用ストリーム・クラス(LogStream)
//          1行出力のスレッド間排他制御も行う。
//----------------------------------------------------------------------------

public:
    //! @todo   T.B.D.
    class THEOLIZER_INTERNAL_DLL LogStream
    {
    private:
        WorkingLog*         mWorkingLog;

        // コンストラクタとデストラクタを対応させるため、コピー不可
        LogStream(const LogStream&) = delete;
        void operator =(const LogStream&) = delete;

    public:
        //! @todo   T.B.D.
        explicit LogStream(WorkingLog* iWorkingLog);
        //! @todo   T.B.D.
        LogStream(LogStream&&);
        ~LogStream() noexcept;

        //! @todo   T.B.D.
        template<typename Type>
        LogStream& operator<<(Type iObject)
        {
            mWorkingLog->getOStream() << iObject;
            return *this;
        }
    };

//----------------------------------------------------------------------------
//      メンバー変数
//----------------------------------------------------------------------------

private:
    struct                  Impl;
    std::unique_ptr<Impl>   mImpl;
    static std::string      mErrorLogPath;
    bool                    mIsUTC;

    std::ostream& getOStream();

//----------------------------------------------------------------------------
//      メンバー関数
//----------------------------------------------------------------------------

    // 排他制御を混乱させないため、コピー不可
    WorkingLog(const WorkingLog&) = delete;
    void operator =(const WorkingLog&) = delete;

    // スレッドセーフ化
public:
    // コンストラクタ／デストラクタ
    // iPath        ログ・ファイルのパス。
    //              ファイル名には1つの%1%が必要。%1%はファイル番号に展開される
    // iFileCount   ログ・ファイル数
    // iFileSize    1ファイルのサイズ(バイト)
    //! @todo   T.B.D.
    WorkingLog(const u8string& iPath, size_t iFileSize=1024*1024, unsigned iFileCount=2);
     ~WorkingLog() noexcept;

    // 行ヘッダの時刻を設定する(UTCタイム or ローカルタイム)
    //! @todo   T.B.D.
    void setUTCTime(bool iIsUTC) {mIsUTC = iIsUTC;}

    // ログ出力用ストリーム返却
    //  LogStreamのインスタンスでロックする
    //! @todo   T.B.D.
    LogStream getLogStream();

//----------------------------------------------------------------------------
//      行ヘッダ解析
//----------------------------------------------------------------------------

    //! @todo   T.B.D.
    struct LineHeader
    {
        time_t      mDateTime;              //!< Mutex獲得直後の日付時刻(秒単位)
        uint32_t    mMilliseconds;          //!< Mutex獲得直後の日付時刻のmSec
        uint32_t    mWaitedMicroseconds;    //!< Mutex獲得に待った時間
        std::string mThereadId;             //!< ThreadId
    };

    // 行ヘッダを解析する
    //  行ヘッダを取り出せたら、iLineHeaderに返却し、trueが返る。
    //  その時、*iPosにはログ文字列の先頭Indexが返る。
    //! @todo   T.B.D.
    static bool getLineHeader
    (
        std::string const& iLine,
        LineHeader& iLineHeader,
        size_t* iPos=nullptr
    ) noexcept;
};

//############################################################################
//      エラー通知
//############################################################################

// ***************************************************************************
//      エラー付加情報
// ***************************************************************************

class ErrorInfo;

namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

struct THEOLIZER_INTERNAL_DLL BaseAdditionalInfo
{
    BaseAdditionalInfo() { }

    // メッセージ取り出し
    virtual u8string getString()=0;

    // エラー情報伝達
    virtual void setError(ErrorInfo const& iErrorInfo, bool iConstructor)
    {
    }
};

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

// ***************************************************************************
//!     エラー種別
// ***************************************************************************

enum class ErrorType
{
    None=0,         //!< エラー無し
    Warning,        //!< 警告
    Error           //!< エラー
};

//! エラー種別をシンボル名で出力する
THEOLIZER_INTERNAL_DLL std::ostream& operator<<(std::ostream& iOStream, ErrorType iErrorType);

// ***************************************************************************
//!     エラー分類
// ***************************************************************************

enum class ErrorKind
{
    Unclassified=0, //!< 未分類
    WrongUsing,     //!< 使い方エラー

    // 動作エラー
    IOError,        //!< Read/Write時のI/Oエラー

    // デシリアイズ時のエラー
    UnknownData,    //!< 非対応シリアライズ・データ・フォーマット
    UnknownVerson   //!< 非対応バージョン
};

//! エラー分類をシンボル名で出力する
THEOLIZER_INTERNAL_DLL std::ostream& operator<<(std::ostream& iOStream, ErrorKind iErrorKind);

// ***************************************************************************
//      エラー通知(例外 or getError())
//          MinGW不具合対策
//              thread_localのデストラクタが適切に動作しない不具合がある。
//              https://sourceforge.net/p/mingw-w64/bugs/445/
//              デストラクタを呼ばないで良いようstd::stringを使わず固定長とする。
// ***************************************************************************

//! @todo   T.B.D.
class THEOLIZER_INTERNAL_DLL ErrorInfo
{
    friend class ErrorReporter;

    ErrorType   mErrorType;             // エラー種別
    ErrorKind   mErrorKind;             // エラー分類

    // エラー検出位置
    char const* mFileName;              // ファイル名
    unsigned    mLineNo;                // 行番号

public:
    // コンストラクタ
    //! @todo   T.B.D.
    ErrorInfo() :
        mErrorType(ErrorType::None),
        mErrorKind(ErrorKind::Unclassified),
        mFileName(nullptr),
        mLineNo(0),
        mMessage(),
        mAdditionalInfo()
    { }

    // メッセージと追加情報設定／取り出し
    //! @todo   T.B.D.
    void setMessage(u8string const& iMessage);
    //! @todo   T.B.D.
    void setAdditionalInfo(u8string const& iAdditionalInfo);

    // エラー情報取り出し
    //! @todo   T.B.D.
    ErrorType   getErrorType() const {return mErrorType;}
    //! @todo   T.B.D.
    ErrorKind   getErrorKind() const {return mErrorKind;}
    //! @todo   T.B.D.
    char const* getFileName()  const {return (mFileName)?mFileName:"<none>";}
    //! @todo   T.B.D.
    unsigned    getLineNo()    const {return mLineNo;}
    //! @todo   T.B.D.
#if defined(__MINGW32__)
    u8string const getMessage()         const { return mMessage; }
#else
    u8string const& getMessage()        const { return mMessage; }
#endif
    //! @todo   T.B.D.
#if defined(__MINGW32__)
    u8string const getAdditionalInfo()  const { return mAdditionalInfo; }
#else
    u8string const& getAdditionalInfo() const { return mAdditionalInfo; }
#endif
    // 全エラー情報
    //! @todo   T.B.D.
    std::string getString() const;

    // boolへ変換(エラーが発生していたらtrue)
    //! @todo   T.B.D.
    operator bool() const  { return mErrorType != ErrorType::None; }
    //! @todo   T.B.D.
    bool operator!() const { return mErrorType == ErrorType::None; }

private:
#if defined(__MINGW32__)
    char        mMessage[256];          // エラー・メッセージ(固定長)
    char        mAdditionalInfo[256];   // 追加情報(固定長)
#else
    u8string    mMessage;               // エラー・メッセージ
    u8string    mAdditionalInfo;        // 追加情報
#endif
};

//! エラーの内容を出力する
THEOLIZER_INTERNAL_DLL std::ostream& operator<<(std::ostream& iOStream, ErrorInfo iErrorInfo);

// ***************************************************************************
//      エラー・ログのファイル・パス指定(UTF-8)
//
//      内部構造：
//          TheolizerLib.lib内にてweakシンボルとして定義している。
// ***************************************************************************

namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN
THEOLIZER_INTERNAL_DLL extern WeakData<char const*> gErrorLogPath;
#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

//! @todo   T.B.D.
#define THEOLIZER_ERRORLOG_FILE(dErrorLogName)                              \
    THEOLIZER_INTERNAL_SET_WEAK_DATA(theolizer::internal::gErrorLogPath,    \
                            char const*, dErrorLogName, ErrorLogPathDummy)

// ***************************************************************************
//      ErrorReporter内部で使うための宣言
// ***************************************************************************

namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN
class Releasing;
class ApiBoundary;
void  throwDeferred(u8string const&, ErrorKind, char const*, unsigned);
bool  writeWarning(u8string const&, ErrorKind, char const*, unsigned) noexcept;
#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

// ***************************************************************************
//      エラー管理クラス(シングルトン)
//          遅延例外：
//              リソース解放処理中なら、リソース解放完了まで遅延する。
//              そうでない時は直ぐに投げる。
//              ただし、既に例外が発行(mProcessing==true)されていたら、
//              多重例外を防ぐため投げない。
// ***************************************************************************

//! @todo   T.B.D.
class THEOLIZER_INTERNAL_DLL ErrorReporter
{
    friend  class internal::Releasing;
    friend  class internal::ApiBoundary;
    friend  void internal::throwDeferred(u8string const&, ErrorKind, char const*, unsigned);
    friend  bool internal::writeWarning(u8string const&,ErrorKind,char const*,unsigned) noexcept;

    unsigned                        mReleaseCount;      // リソース開放処理中ネスト・カウント
    bool                            mIsDeferred;        // 例外保留
    bool                            mProcessing;        // 発生したエラー処理中
    internal::BaseAdditionalInfo*   mAdditionalInfo;    // 付加情報提供クラス
    bool                            mRegistered;        // mAdditionalInfo登録済
    ErrorInfo                       mErrorInfo;         // エラー情報

    // コピー／ムーブ禁止
    ErrorReporter(const ErrorReporter&)  = delete;
    ErrorReporter(      ErrorReporter&&) = delete;
    ErrorReporter& operator=(const ErrorReporter&)  = delete;
    ErrorReporter& operator=(      ErrorReporter&&) = delete;

    // コンストラクタ
    ErrorReporter() noexcept :
        mReleaseCount(0),
        mIsDeferred(false),
        mProcessing(false),
        mAdditionalInfo(nullptr),
        mRegistered(false),
        mErrorInfo()
    { }

    // インスタンス生成
    static ErrorReporter& getInstance();

    // エラー・ログ出力とエラー記録
    bool writeErrorLog
    (
        u8string const& iMessage,
        ErrorType       iErrorType,
        ErrorKind       iErrorKind,
        char const*     iFileName,
        unsigned        iLineNo
    );

    // 遅延例外の実装
    void throwDeferredImpl
    (
        u8string const& iMessage,
        ErrorKind       iErrorKind,
        char const*     iFileName,
        unsigned        iLineNo
    );

    // 警告通知の実装
    bool writeWarningImpl
    (
        u8string const& iMessage,
        ErrorKind       iErrorKind,
        char const*     iFileName,
        unsigned        iLineNo
    ) noexcept;

    // エラー・クリア
    void clearError() noexcept;

public:
    // エラー情報獲得
    //! @todo   T.B.D.
    static ErrorInfo const& getErrorInfo() noexcept
    {
        return getInstance().mErrorInfo;
    }
};

namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

//----------------------------------------------------------------------------
//      private化するために中継
//----------------------------------------------------------------------------

// 遅延例外
inline void throwDeferred
(
    u8string const& iMessage,
    ErrorKind       iErrorKind=ErrorKind::Unclassified,
    char const*     iFileName=nullptr,
    unsigned        iLineNo=0
)
{
    ErrorReporter::getInstance().throwDeferredImpl(iMessage, iErrorKind, iFileName, iLineNo);
}

// 警告メッセージを記録
//  ログ・ファイルが指定されていたら、ログへ出力してtrue返却
inline bool writeWarning
(
    u8string const& iMessage,
    ErrorKind       iErrorKind=ErrorKind::Unclassified,
    char const*     iFileName=nullptr,
    unsigned        iLineNo=0
) noexcept
{
    return ErrorReporter::getInstance().writeWarningImpl(iMessage, iErrorKind, iFileName, iLineNo);
}

// ***************************************************************************
//      リソース解放処理中の保護と遅延例外の発行
// ***************************************************************************

class Releasing
{
public:
    Releasing() noexcept
    {
        ErrorReporter::getInstance().mReleaseCount++;
    }
    ~Releasing() noexcept(false)
    {
        auto&& aErrorReporter=ErrorReporter::getInstance();

        THEOLIZER_INTERNAL_ASSERT(aErrorReporter.mReleaseCount,
            "mReleaseCount is zero at ~Releasing().");

        aErrorReporter.mReleaseCount--;

        // リソース解放が終わり、かつ、保留されていたら、例外を投げる
        if ((aErrorReporter.mReleaseCount == 0) && (aErrorReporter.mIsDeferred))
        {
            aErrorReporter.mIsDeferred=false;
    throw aErrorReporter.mErrorInfo;
        }
    }
};

// ***************************************************************************
//      APIの境界関数でこのインスタンスを生成する
//          また、これはエラー追加情報管理領域をErrorReporterへ登録する
//          すでに登録されていたら
//              生成時／開放時の両方で更新しない
//          iConstructorについて
//              例外発行を禁止されたコンストラクタから呼ぶ場合に、trueとする。
//              iAdditionalInfoは上記コンストラクタのクラスが保持する。
// ***************************************************************************

class ApiBoundary
{
    bool    mRegistered;
    bool    mConstructor;

public:
    ApiBoundary(BaseAdditionalInfo* iAdditionalInfo, bool iConstructor=false) noexcept :
        mRegistered(false), mConstructor(iConstructor)
    {
        auto&& aErrorReporter=ErrorReporter::getInstance();
        if (aErrorReporter.mRegistered)
    return;

        mRegistered=true;
        aErrorReporter.clearError();
        aErrorReporter.mAdditionalInfo=iAdditionalInfo;
        aErrorReporter.mRegistered=true;
    }
    ~ApiBoundary() noexcept
    {
        auto&& aErrorReporter=ErrorReporter::getInstance();

        if (mRegistered)
        {
            if (aErrorReporter.mAdditionalInfo)
            {
                aErrorReporter.mAdditionalInfo->setError(aErrorReporter.mErrorInfo, mConstructor);
            }
            aErrorReporter.mRegistered=false;
            aErrorReporter.mAdditionalInfo=nullptr;
            aErrorReporter.mProcessing=false;   // 例外処理中終了
            aErrorReporter.mIsDeferred=false;   // 念のため
        }
    }
};

#endif  // THEOLIZER_INTERNAL_DOXYGEN
// ***************************************************************************
//      エラー管理用基底クラス
// ***************************************************************************

//! @todo   T.B.D.
class THEOLIZER_INTERNAL_DLL ErrorBase
{
//      ---<<< 発生したエラーの管理 >>>---

protected:
    bool                    mConstructorError;      // コンストラクト実行中エラー
private:
    ErrorInfo               mErrorInfo;             // エラー情報

    // エラー報告受領時の排他制御(ヘッダ依存回避のためpImpl)
    //  ErrorReporterはスレッド毎に存在し、ErrorReporterからエラーを受領するため
    struct                  Impl;
    std::unique_ptr<Impl>   mImpl;

protected:
    ErrorBase()  noexcept;
    ~ErrorBase() noexcept;

    // エラー情報受領
    void setError(ErrorInfo const& iErrorInfo, bool iConstructor) noexcept;

    // エラー・チェック(エラーが発生したまま呼び出したら例外)
    void checkError();

public:
    // エラー／警告情報返却
    //! @todo   T.B.D.
    ErrorInfo const& getErrorInfo() {return mErrorInfo;}

    // エラー発生
    //! @todo   T.B.D.
    bool isError()
    {
        if (!mErrorInfo)
    return false;
        return mErrorInfo.getErrorType() != ErrorType::Warning;
    }

    /*!
        エラー状態を解除する

        なお、例外不許可の場合、コンストラクト中にエラーが発生すると、エラー状態となる。
        このコンストラクト中のエラーは解除できない。（解除しても動作できないため）
        コンストラクト中にエラーになった場合、デストラクトすることのみ可能。
    */
    void resetError();
};
#ifndef THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//      エラー報告マクロ
//          エラー・ログがあれば、記録する
// ***************************************************************************

#define THEOLIZER_INTERNAL_IO_ERROR(...)                                    \
    theolizer::internal::throwDeferred                                      \
    (                                                                       \
        theolizer::print(__VA_ARGS__),                                      \
        theolizer::ErrorKind::IOError,                                      \
        THEOLIZER_INTERNAL_FILE,                                            \
        __LINE__                                                            \
    )

#define THEOLIZER_INTERNAL_WRONG_USING(...)                                 \
    theolizer::internal::throwDeferred                                      \
    (                                                                       \
        theolizer::print(__VA_ARGS__),                                      \
        theolizer::ErrorKind::WrongUsing,                                   \
        THEOLIZER_INTERNAL_FILE,                                            \
        __LINE__                                                            \
    )

#define THEOLIZER_INTERNAL_DATA_ERROR(...)                                  \
    theolizer::internal::throwDeferred                                      \
    (                                                                       \
        theolizer::print(__VA_ARGS__),                                      \
        theolizer::ErrorKind::UnknownData,                                  \
        THEOLIZER_INTERNAL_FILE,                                            \
        __LINE__                                                            \
    )

#define THEOLIZER_INTERNAL_VERSION_ERROR(...)                               \
    theolizer::internal::throwDeferred                                      \
    (                                                                       \
        theolizer::print(__VA_ARGS__),                                      \
        theolizer::ErrorKind::UnknownVerson,                                \
        THEOLIZER_INTERNAL_FILE,                                            \
        __LINE__                                                            \
    )

//      ---<<< 分類無し >>>---

#define THEOLIZER_INTERNAL_ERROR(...)                                       \
    theolizer::internal::throwDeferred                                      \
    (                                                                       \
        theolizer::print(__VA_ARGS__),                                      \
        theolizer::ErrorKind::Unclassified,                                 \
        THEOLIZER_INTERNAL_FILE,                                            \
        __LINE__                                                            \
    )

#define THEOLIZER_INTERNAL_WARNING(...)                                     \
    theolizer::internal::writeWarning                                       \
    (                                                                       \
        theolizer::print(__VA_ARGS__),                                      \
        theolizer::ErrorKind::Unclassified,                                 \
        THEOLIZER_INTERNAL_FILE,                                            \
        __LINE__                                                            \
    )

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

//############################################################################
//      End
//############################################################################

}   // namespace theolizer

// ***************************************************************************
//          警告抑止解除
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#endif  // THEOLIZER_INTERNAL_REPORT_H
