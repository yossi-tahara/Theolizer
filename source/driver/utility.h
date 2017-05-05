//############################################################################
//      ユーティリティ／共有クラス定義
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

#if !defined(THEOLIZER_UTILITY_H)
#define THEOLIZER_UTILITY_H

// ***************************************************************************
//      重要定義
// ***************************************************************************

//----------------------------------------------------------------------------
//      各種定義
//----------------------------------------------------------------------------

// ログ出力設定ファイル名
char const* kTheolizerSetupFile = "driver.txt";

// ログ・ファイル名
char const* kTheolizerLogFile = "debug.log";

// TheolizerDriver.exe判定用文字列
char const* kTheolizerMarker = THEOLIZER_INTERNAL_PRODUCT_NAME;

// TheolizerDriver専用パラメータ
#define ARG_THEOLIZER   "--theolizer"                           // 継続パラメータ
char const* kTheolizerAnalyzeParam  = "THEOLIZER_ANALYZE";      // none
char const* kTheolizerCompilerParam = ARG_THEOLIZER "-compiler";// =<OrigPath>
char const* kTheolizerReplaceParam  = ARG_THEOLIZER "-replace"; // =<Path>;<Path>;...
char const* kTheolizerRestoreParam  = ARG_THEOLIZER "-restore"; // =<Path>;<Path>;...
char const* kTheolizerVersionParam  = ARG_THEOLIZER "-version"; // none

// エラー出力ヘッダ
char const*  kDiagMarker = "[" THEOLIZER_INTERNAL_PRODUCT_NAME "] ";

// 排他制御用リソース名
char const* kTheolizerFileLock = "file_lock";

//############################################################################
//      コンパイラの相違点を吸収する
//############################################################################

// ***************************************************************************
//      POSIX関係
// ***************************************************************************

#ifdef _MSC_VER
    #include <io.h>
    int chdir(char const* iPath);
    char *GetCurrentDirName();
#else
    #include <unistd.h>
    char *GetCurrentDirName();
#endif

//############################################################################
//      汎用ルーチン
//############################################################################

extern SourceManager*           gSourceManager; // デバッグ用
extern unsigned                 gAutoDeleteSec; // Logファイルの自動削除経過秒数

// ***************************************************************************
//      ASSERT群
// ***************************************************************************

void DriverAbort(char const* iDate, char const* iFile, const int iLine, 
                     const char *iFormat, ...)
{
    std::string      aOutputString;
    int         aOutputNum;
    char        aBuff[2048];
    const int   aBuffSize((int)sizeof(aBuff));

    va_list args;
    va_start(args, iFormat);
    aOutputNum = snprintf(aBuff, aBuffSize, "%s(%d) [%s] ", iFile, iLine, iDate);
    aOutputString = aBuff;
    if ((aOutputNum < 0) || (aBuffSize <= aOutputNum))
    {
        aOutputString += "[[TRANCATED]]";
    }
    aOutputNum = vsnprintf(aBuff, aBuffSize, iFormat, args);
    aOutputString += aBuff;
    if ((aOutputNum < 0) || (aBuffSize <= aOutputNum))
    {
        aOutputString += "[[TRANCATED]]";
    }
    va_end(args);

    llvm::errs() << aOutputString << "\n";
    std::exit(1);
}

#define DRIVER_ABORT(...)                                          \
    DriverAbort(__DATE__, __FILE__, __LINE__, __VA_ARGS__);
#define DRIVER_ASSERT(iCondition, ...) if (!(iCondition))          \
    DriverAbort(__DATE__, __FILE__, __LINE__, __VA_ARGS__);

// ***************************************************************************
//      アクセス権付与
// ***************************************************************************

void setPermissions(char const* iPath)
{
    chmod(iPath, 0777);
}

// ***************************************************************************
//      テンポラリ・フォルダ管理
//          一度だけ動けば良いのでシングルトンとする
// ***************************************************************************

class TemporaryDir
{
private:
    SmallString<128>    mTemporaryDir;
    TemporaryDir()
    {
        // テンポラリ・フォルダのパス生成
        SmallString<128>    temp_dir;
        llvmS::path::system_temp_directory(true, temp_dir);
        mTemporaryDir = temp_dir;
        mTemporaryDir += "/theolizer/";

        // テンポラリ・フォルダの生成
        std::string aTemporaryDir(mTemporaryDir.begin());
        boost::system::error_code error;
        boostF::create_directories(aTemporaryDir, error);
        setPermissions(aTemporaryDir.c_str());
    }
    TemporaryDir(const TemporaryDir&) = delete;
    void operator =(const TemporaryDir&) = delete;

public:
    static char const* get()
    {
        static TemporaryDir instance;
        return instance.mTemporaryDir.c_str();
    }
};

// ***************************************************************************
//      マルチ・プロセス・ログ
//          複数のプロセスのログを1つのログ・ファイルへ出力する
//          テンポラリ・フォルダの下へTheolizerフォルダ生成し、
//          そこにログ・ファイルを保存する。
// ***************************************************************************

//----------------------------------------------------------------------------
//      ログ用のフォルダとファイルの生成と書き込み
//          ログ・ファイルが最後の更新から1時間以上経過していたら、削除する
//          ログ・ファイルは１つだけなのでシングルトンとする
//----------------------------------------------------------------------------

class LogFile
{
private:
    struct LockingFile
    {
        std::string             mLogPath;
        std::string             mLockPath;
        llvm::raw_fd_ostream    mRawFdOStream;
        boostI::file_lock       mFileLock;
        bool                    mValid;

        LockingFile(std::string const& iLogPath, std::error_code& iError) :
            mLogPath(iLogPath),
            mLockPath(mLogPath+"_lock"),
            mRawFdOStream(mLogPath, iError, llvmS::fs::F_Append | llvmS::fs::F_Text),
            mFileLock(),
            mValid(!iError)
        {
            setPermissions(mLogPath.c_str());

            // ロック用ファイルがなかったら作る
            if (!llvmS::fs::exists(mLockPath))
            {
                llvm::raw_fd_ostream    aTempFdOStream(mLockPath, iError, llvmS::fs::F_None);
            }
            setPermissions(mLockPath.c_str());
            mFileLock=boostI::file_lock(mLockPath.c_str());
        }
    };
    std::unique_ptr<LockingFile>        mLockingFile;

//      ---<<< ロックとログ出力 >>>---

public:
    class LogOStream
    {
    private:
        LockingFile&        mLockingFile;
    public:
        LogOStream(LockingFile& iLockingFile, int iLogKind) : mLockingFile(iLockingFile)
        {
            mLockingFile.mFileLock.lock();

            if (mLockingFile.mValid)
            {
                boost::posix_time::time_duration  now
                    = boost::posix_time::microsec_clock::local_time().time_of_day();
                long msec=now.total_milliseconds() % 1000;
                boostI::ipcdetail::OS_thread_id_t thread_id =
                                boostI::ipcdetail::get_current_thread_id();

                std::stringstream ss;
                ss << ",";
                ss << std::setfill('0') << std::setw(2) << now.hours() << ":";
                ss << std::setfill('0') << std::setw(2) << now.minutes()  << ":";
                ss << std::setfill('0') << std::setw(2) << now.seconds()  << ".";
                ss << std::setfill('0') << std::setw(3) << msec << ",_";
                ss << std::setfill('0') << std::setw(5) << thread_id << ",";
                ss << std::setw(0) << "[Kind" << iLogKind << "],";

                mLockingFile.mRawFdOStream << ss.str();
            }
        }
        ~LogOStream()
        {
            if (mLockingFile.mValid)
            {
                mLockingFile.mRawFdOStream << "\n";
                mLockingFile.mRawFdOStream.flush();
            }
            else
            {
                llvm::outs() << "\n";
                llvm::outs().flush();
            }
            mLockingFile.mFileLock.unlock();
        }
        template<typename Type>
        LogOStream& operator<<(Type iObject)
        {
            if (mLockingFile.mValid)
            {
                mLockingFile.mRawFdOStream << iObject;
            }
            else
            {
                llvm::outs() << iObject;
            }
            return *this;
        }
    };

//      ---<<< 生成と削除 >>>---

private:
    LogFile() : mLogKindSet(LogKindSet().set()), mInitialized(false) { }
    LogFile(const LogFile&) = delete;
    void operator =(const LogFile&) = delete;

    void initialize()
    {
        // 最初に出力指定された時、ファイル生成する
        if (mInitialized)
    return;
        if (!mLogKindSet.any())
    return;
        mInitialized=true;

        // ログ・ファイルのパス生成
        std::string aLogPath(TemporaryDir::get()); 
        aLogPath.append(kTheolizerLogFile);

        // ログ・ファイルがあれば、更新した日付時刻を取り出して、
        // gAutoDeleteSec秒以上経過していたら、削除する。
        if (llvmS::fs::exists(aLogPath))
        {
            llvmS::fs::file_status  result;
            if (!llvmS::fs::status(aLogPath, result))
            {
                llvmS::TimeValue now = llvmS::TimeValue::now();
                llvmS::TimeValue last = result.getLastModificationTime();
                llvmS::TimeValue diff = now - last;

                if (diff.seconds() >= gAutoDeleteSec)
                {
                    llvmS::fs::remove(aLogPath);
                }
            }
        }

        // raw_fd_streamとfile_lock生成
        std::error_code error;
        mLockingFile = std::move(std::unique_ptr<LockingFile>(new LockingFile(aLogPath, error)));
        if (error)
        {
            llvm::errs() << kDiagMarker << "Can't open log file\n"
                         << "    File : " << aLogPath << "\n"
                         << "    Error: " << error.message() << "\n";
        }
    }

public:
    static LogFile& getInstance()
    {
        static LogFile instance;
        return instance;
    }

//      ---<<< 出力先 >>>---
public:
    enum LogKind
    {
        System,                         // ログ・システム
        Time,                           // 処理時間計測
        Parameter,                      // パラメータ処理デバッグ出力
        Driver,                         // ドライバー制御デバッグ出力
        DoAnalyze,                      // AST解析実行判定デバッグ出力
        AstAnalyze,                     // AST解析動作デバッグ出力
        LogKindNumber                   // 出力先の数
    };
    typedef std::bitset<LogKindNumber>  LogKindSet;

private:
    LogKindSet  mLogKindSet;            // 出力先集合
    bool        mInitialized;           // 初期化済

public:
    void        setLogKindSet(LogKindSet iLogKindSet)
    {
        mLogKindSet=iLogKindSet;
        initialize();
    }
    LogKindSet  getLogKindSet() {return mLogKindSet;}

//      ---<<< raw_ostream生成と返却 >>>---
//      このraw_ostreamが有効な間、ログ・ファイルをロックする。

public:
    LogOStream getRawOStream(LogKind iLogKind)
    {
        return LogOStream(*(mLockingFile.get()), iLogKind);
    }

};

//----------------------------------------------------------------------------
//      メッセージ出力用マクロ
//----------------------------------------------------------------------------

#define ENABLE_OUTPUT(LogKindSet)                                           \
            LogFile::getInstance().setLogKindSet(LogKindSet)

#define DISABLE_OUTPUT()                                                    \
            LogFile::getInstance().setLogKindSet(LogFile::LogKindSet(0))

#define KIND(Kind)              LogFile::LogKindSet().set(LogFile::Kind)
#define KIND_ALL                LogFile::LogKindSet().set()
#define TOUT(Kind)              LogFile::getInstance().getRawOStream(LogFile::Kind)
#define JUDGE(Kind)             LogFile::getInstance().getLogKindSet().test(Kind)

template<typename Type>
LogFile::LogOStream& operator,(LogFile::LogOStream& iOStream, Type iObject)
{
    iOStream << iObject;
    return iOStream;
}

#define IS_TIME_OUTPUT          JUDGE(LogFile::Time)
#define TIME_OUTPUT(...)        \
            do{if(IS_TIME_OUTPUT)TOUT(Time)<<__VA_ARGS__;}while(0)

#define IS_PARAMETER_OUTPUT     JUDGE(LogFile::Parameter)
#define PARAMETER_OUTPUT(...)   \
            do{if(IS_PARAMETER_OUTPUT)TOUT(Parameter)<<__VA_ARGS__;}while(0)

#define IS_DRIVER_OUTPUT        JUDGE(LogFile::Driver)
#define DRIVER_OUTPUT(...)      \
            do{if(IS_DRIVER_OUTPUT)TOUT(Driver)<<__VA_ARGS__;}while(0)

#define IS_DOANALYZE_OUTPUT     JUDGE(LogFile::DoAnalyze)
#define DOANALYZE_OUTPUT(...)   \
            do{if(IS_DOANALYZE_OUTPUT)TOUT(DoAnalyze)<<__VA_ARGS__;}while(0)

#define IS_ASTANALYZE_OUTPUT    JUDGE(LogFile::AstAnalyze)
#define ASTANALYZE_OUTPUT(...)  \
            do{if(IS_ASTANALYZE_OUTPUT)TOUT(AstAnalyze)<<__VA_ARGS__;}while(0)

// ***************************************************************************
//      ユーティリティ
// ***************************************************************************

//----------------------------------------------------------------------------
//      スコープを抜けたらfalseへ戻す
//----------------------------------------------------------------------------

struct AutoFalse
{
    bool&   mTarget;
    AutoFalse(bool& iTarget) :
        mTarget(iTarget)
    {
        mTarget=true;
    }
    ~AutoFalse()
    {
        mTarget=false;
    }
};

//----------------------------------------------------------------------------
//      スコープを抜けたらデバッグ出力
//----------------------------------------------------------------------------

struct AutoPrint
{
    static unsigned mLevel;
    std::string mString;
    AutoPrint(std::string const& iString) :
        mString(iString)
    {
        ASTANALYZE_OUTPUT(std::string(mLevel*2, ' '), "Start of ", mString);
        mLevel++;
    }
    ~AutoPrint()
    {
        mLevel--;
        ASTANALYZE_OUTPUT(std::string(mLevel*2, ' '), "End of   ", mString);
    }
};
unsigned AutoPrint::mLevel=0;

//----------------------------------------------------------------------------
//      boolキャストを持つiterator
//          end()と一致する時、falseとなる。
//          使用例
//              std::map<int, std::string> foo;
//              Iterator<std::map<int, std::string> > found(fool.find(10), foo);
//              if (!found)
//              {
//                  // 見つからなかった時の処理
//              }
//----------------------------------------------------------------------------

template<class tContainer>
class Iterator : public tContainer::iterator
{
private:
    tContainer* mContainer;
public:
    Iterator(typename tContainer::iterator iIterator, tContainer* iContainer) :
        tContainer::iterator(iIterator),
        mContainer(iContainer)
    { }

    operator bool()
    { return (mContainer->end() != *this);}
};

// ***************************************************************************
//      AST解析用ユーティリティ
// ***************************************************************************

//----------------------------------------------------------------------------
//      未定義VersionNoコード
//----------------------------------------------------------------------------

namespace
{
unsigned const kInvalidNumber=std::numeric_limits<unsigned>::max();
}

//----------------------------------------------------------------------------
//      バージョン番号取り出し
//          ASPInt型で取り出したバージョン番号をint型へ変換する
//          戻り値：成功時true
//----------------------------------------------------------------------------

bool getVersionNumber(const llvm::APSInt& iAPSInt, unsigned& oValue)
{
    int64_t value = iAPSInt.getSExtValue();
    if ((value < std::numeric_limits<unsigned>::min())
     || (std::numeric_limits<unsigned>::max() <= value))
return false;

    oValue=static_cast<unsigned>(value);
    return true;
}

bool getVersionNumber(const APValue* iAPValue, unsigned& oValue)
{
    if (!iAPValue->isInt())
return false;

    return getVersionNumber(iAPValue->getInt(), oValue);
}

//----------------------------------------------------------------------------
//      制御番号取り出し
//          ASPInt型で取り出したバージョン番号をunsigned int型へ変換する
//          戻り値：成功時true
//----------------------------------------------------------------------------

bool getControlNumber(const llvm::APInt& iAPInt, unsigned int& oValue)
{
    int64_t  value = iAPInt.getZExtValue();
    if (std::numeric_limits<unsigned int>::max() <= value)
return false;

    oValue=static_cast<unsigned int>(value);
    return true;
}

//----------------------------------------------------------------------------
//      bool値取り出し
//          戻り値：成功時true
//----------------------------------------------------------------------------

bool getBool(const APValue* iAPValue, bool& oValue)
{
    if (!iAPValue->isInt())
return false;

    int64_t value = iAPValue->getInt().getSExtValue();

    oValue=(value!=0);
    return true;
}

//----------------------------------------------------------------------------
//      名前空間取り出し
//----------------------------------------------------------------------------

class NameSpaces
{
    struct NamespaceInfo
    {
        bool        mIsInline;
        StringRef   mName;
        NamespaceInfo(bool iIsInline, StringRef iName) :
            mIsInline(iIsInline), mName(iName)
        { }
    };
    typedef SmallVector<NamespaceInfo, 8>           Vector;
    typedef typename Vector::const_reverse_iterator Iterator;

    Vector  mVector;

public:
    NameSpaces() { }
    NameSpaces(NamedDecl const* iNamedDecl, bool iSuppressUnwrittenScope=true)
    {
        clang::DeclContext const* aContext = iNamedDecl->getDeclContext();
        while (aContext && clang::isa<NamedDecl>(aContext))
        {
            clang::NamespaceDecl const* nsd = dyn_cast<clang::NamespaceDecl>(aContext);
            if (nsd)
            {
                if (iSuppressUnwrittenScope)
                {
                    if (!(nsd->isAnonymousNamespace()) && (!(nsd->isInline())))
                        mVector.push_back(NamespaceInfo(false, nsd->getName()));
                }
                else
                {
                    if (nsd->isAnonymousNamespace())
                    {
                        mVector.push_back(NamespaceInfo(false, StringRef("")));
                    }
                    else
                    {
                        mVector.push_back(NamespaceInfo(nsd->isInline(), nsd->getName()));
                    }
                }
            }
            aContext = aContext->getParent();
        }
    }

    Iterator begin() {return mVector.rbegin();}
    Iterator end()   {return mVector.rend();}
};

//----------------------------------------------------------------------------
//      名前空間付きの名前獲得
//----------------------------------------------------------------------------

std::string getQualifiedName(NamedDecl const* iNamedDecl, bool iSuppressUnwrittenScope=true)
{
    std::string ret;
    for (auto aNameSpace : NameSpaces(iNamedDecl, iSuppressUnwrittenScope))
    {
        ret.append(aNameSpace.mName.str());
        ret.append("::");
    }
    ret.append(iNamedDecl->getName().str());

    return ret;
}

//----------------------------------------------------------------------------
//      生配列の基本型取り出し
//----------------------------------------------------------------------------

QualType getUnderlyingType(QualType iQt)
{
    if (!iQt->isArrayType())
return iQt;

    clang::ArrayType const* aArrayType=dyn_cast<clang::ArrayType const>(iQt.getTypePtr());
    return getUnderlyingType(aArrayType->getElementType());
}

//############################################################################
//      主要共有クラス
//############################################################################

// ***************************************************************************
//      ソース保存排他制御オブジェクト
//          ミューテックスと各フラグはアトミックに設定する必要があるが、
//          Theolizerドライバーはシングル・スレッドなのでアトミック化を省略。
// ***************************************************************************

class ExclusiveControl
{
//----------------------------------------------------------------------------
//      排他制御用のファイル生成
//----------------------------------------------------------------------------

private:
    struct FileCreator
    {
        SmallString<128>    mFilePathSharable;
        SmallString<128>    mFilePathUpgradable;
        FileCreator(char const* iPrefix) : 
                        mFilePathSharable(TemporaryDir::get()),
                        mFilePathUpgradable(TemporaryDir::get())
        {
            mFilePathSharable += iPrefix;
            mFilePathSharable += "_sharable";

            mFilePathUpgradable += iPrefix;
            mFilePathUpgradable += "_upgradable";

            int result_fd;
            llvmS::fs::openFileForWrite(StringRef(mFilePathSharable),
                                        result_fd, llvmS::fs::F_Append);
            llvmS::Process::SafelyCloseFileDescriptor(result_fd);
            setPermissions(mFilePathSharable.c_str());

            llvmS::fs::openFileForWrite(StringRef(mFilePathUpgradable),
                                        result_fd, llvmS::fs::F_Append);
            llvmS::Process::SafelyCloseFileDescriptor(result_fd);
            setPermissions(mFilePathUpgradable.c_str());
        }
    };
    FileCreator             mFileCreator;
    boostI::file_lock       mFileLockSharable;
    boostI::file_lock       mFileLockUpgradable;
    bool                    mSharableLocked;
    bool                    mUpgradableLocked;
    bool                    mLocked;

    bool                    mRedoRequest;

//----------------------------------------------------------------------------
//      生成と削除
//          デストラクト時ロックを開放しているが、これは必要なロジックではない。
//----------------------------------------------------------------------------

public:
    ExclusiveControl(char const* iName) :
        mFileCreator(iName),
        mFileLockSharable(mFileCreator.mFilePathSharable.c_str()),
        mFileLockUpgradable(mFileCreator.mFilePathUpgradable.c_str()),
        mSharableLocked(false),
        mUpgradableLocked(false),
        mLocked(false),
        mRedoRequest(false)
    { }
    ~ExclusiveControl()
    {
        unlock();
        unlock_upgradable();
        unlock_sharable();
    }

//----------------------------------------------------------------------------
//      Sharable Lock
//----------------------------------------------------------------------------

//      ---<<< Lock >>>---

    void lock_sharable()
    {
        DRIVER_OUTPUT("ExclusiveControl : lock_sharable()");
        {
            boostI::sharable_lock<boostI::file_lock> lock(mFileLockUpgradable);
            mFileLockSharable.lock_sharable();
        }
        mSharableLocked = true;
        mRedoRequest=false;
        DRIVER_OUTPUT("ExclusiveControl : ->success!! mRedoRequest=",
                      mRedoRequest);
    }

//      ---<<< Unlock >>>---

    void unlock_sharable()
    {
        if (!mSharableLocked)
    return;
        mSharableLocked = false;
        mFileLockSharable.unlock_sharable();
        DRIVER_OUTPUT("ExclusiveControl : unlock_sharable() mRedoRequest=",
                      mRedoRequest);
    }

//----------------------------------------------------------------------------
//      Upgradable Lock
//----------------------------------------------------------------------------

//      ---<<< Try Lock >>>---
//          Upgradeできないということは、既に他のプロセスがソース修正中である。
//          この場合、AST解析からやり直す必要がある。

    bool try_lock_upgradable()
    {
        DRIVER_OUTPUT("ExclusiveControl : try_lock_upgradable()");
        if (mFileLockUpgradable.try_lock())
        {
            DRIVER_OUTPUT("ExclusiveControl : ->success!! mRedoRequest=",
                          mRedoRequest);
            mUpgradableLocked = true;
            unlock_sharable();
    return true;
        }
        mRedoRequest=true;
        DRIVER_OUTPUT("ExclusiveControl : ->fail!! mRedoRequest=",
                      mRedoRequest);
        return false;
    }

//      ---<<< Lock >>>---
//          排他制御機能のデバッグ／動作検証用

    void lock_upgradable()
    {
        DRIVER_OUTPUT("ExclusiveControl : lock_upgradable()");
        mFileLockUpgradable.lock();
        DRIVER_OUTPUT("ExclusiveControl : ->success!! mRedoRequest=",
                      mRedoRequest);
        mUpgradableLocked = true;
        unlock_sharable();
    }

//      ---<<< Unlock >>>---

    void unlock_upgradable()
    {
        if (!mUpgradableLocked)
    return;
        mUpgradableLocked = false;
        lock_sharable();
        mFileLockUpgradable.unlock();
        DRIVER_OUTPUT("ExclusiveControl : unlock_upgradable() mRedoRequest=",
                      mRedoRequest);
    }

//----------------------------------------------------------------------------
//      Exclusive Lock
//----------------------------------------------------------------------------

//      ---<<< Lock >>>---

    void lock()
    {
        DRIVER_OUTPUT("ExclusiveControl : lock()");
        mFileLockSharable.lock();
        mLocked = true;
        mUpgradableLocked = false;
        DRIVER_OUTPUT("ExclusiveControl : ->success!! mRedoRequest=", mRedoRequest);
    }

//      ---<<< Unlock >>>---

    void unlock()
    {
        if (!mLocked)
    return;
        mUpgradableLocked = true;
        mLocked = false;
        mFileLockSharable.unlock();
        DRIVER_OUTPUT("ExclusiveControl : unlock() mRedoRequest=", mRedoRequest);
    }

//----------------------------------------------------------------------------
//      ドライバー専用機能
//          try_lock_upgradableに失敗していたらtrueを返却する。
//----------------------------------------------------------------------------

    bool getRedoRequest()
    {
        DRIVER_OUTPUT("ExclusiveControl : getRedoRequest()=", mRedoRequest);
        return mRedoRequest;
    }
};

// ***************************************************************************
//      診断結果表示用DiagnosticConsumer
// ***************************************************************************

extern ASTContext*  gASTContext;
std::string         gLastErrorMessage;

class TheolizerDiagnosticConsumer : public clang::TextDiagnosticPrinter
{
private:
    bool                    mHasErrorOccurred;
    SmallVector<char, 128>  mMessage;

public:
    TheolizerDiagnosticConsumer(clang::raw_ostream &iOStream, clang::DiagnosticOptions *iDiagOpt) :
        TextDiagnosticPrinter(iOStream, iDiagOpt),
        mHasErrorOccurred(false)
    { }
    void clear() {mHasErrorOccurred=false;}
    bool hasErrorOccurred() {return mHasErrorOccurred;}

    void HandleDiagnostic(clang::DiagnosticsEngine::Level iDiagLevel,
                          clang::Diagnostic const& iInfo) override
    {
        mMessage.clear();
        iInfo.FormatDiagnostic(mMessage);
        std::string aMessage(mMessage.begin(), mMessage.size());

        ASTANALYZE_OUTPUT("### HandleDiagnostic ###");
        ASTANALYZE_OUTPUT("    DiagLevel = ", iDiagLevel);
        std::string aLocation;
        if (iInfo.hasSourceManager())
        {
            aLocation=gASTContext->getFullLoc(iInfo.getLocation()).getSpellingLoc().
                    printToString(iInfo.getSourceManager());
        }
        else
        {
            aLocation="<no-source>";
        }
        ASTANALYZE_OUTPUT("    mMessage  = ", aMessage);
        ASTANALYZE_OUTPUT("    aLocation = ", aLocation);

        // 最後のErrorとFatalを記録する
        if (clang::DiagnosticsEngine::Level::Error <= iDiagLevel)
        {
            gLastErrorMessage=aMessage + "\n" + aLocation;
        }

        // 致命的エラーやTheolizerエラーを反映する
        if ((iDiagLevel == clang::DiagnosticsEngine::Level::Fatal)
         || (StringRef(aMessage).startswith(kDiagMarker)))
        {
            clang::TextDiagnosticPrinter::HandleDiagnostic(iDiagLevel, iInfo);
            mHasErrorOccurred=true;
            ASTANALYZE_OUTPUT("mHasErrorOccurred=true;");
        }
    }
};

// ***************************************************************************
//      診断メーセッジ出力
// ***************************************************************************

class CustomDiag
{
private:
    DiagnosticsEngine*  mDiags;

    DiagnosticBuilder Report(DiagnosticIDs::Level iLevel,
                             SourceLocation iLoc,
                             char const* iFormat)
    {
        unsigned custom_id = mDiags->getDiagnosticIDs()->getCustomDiagID(
                                                iLevel, StringRef(iFormat));
        return mDiags->Report(iLoc, custom_id);
    }
    std::string mBuff;
    char const* getFormat(std::string const& iFormat)
    {
        mBuff = kDiagMarker;
        mBuff.append(iFormat);
        return mBuff.c_str();
    }
    TheolizerDiagnosticConsumer*        mClient;
    std::unique_ptr<DiagnosticConsumer> mClientBackup;

public:
    CustomDiag() : mDiags(nullptr), mClient(nullptr)
    { }

    void setDiagnosticsEngine(DiagnosticsEngine* iDiags)
    {

        mDiags = iDiags;
        mClient= static_cast<TheolizerDiagnosticConsumer*>(mDiags->getClient());
        DRIVER_ASSERT(mClient, "");
        mClient->clear();
    }
    void resetDiagnosticsEngine()
    {
    }

    bool hasErrorOccurred()
    {
        DRIVER_ASSERT(mClient, "");
        return mClient->hasErrorOccurred();
    }

    DiagnosticBuilder IgnoredReport(SourceLocation iLoc, std::string const& iFormat)
    {
        return Report(DiagnosticIDs::Ignored, iLoc, getFormat(iFormat));
    }
    DiagnosticBuilder NoteReport(SourceLocation iLoc, std::string const& iFormat)
    {
        return Report(DiagnosticIDs::Note, iLoc, getFormat(iFormat));
    }
    DiagnosticBuilder RemarkReport(SourceLocation iLoc, std::string const& iFormat)
    {
        return Report(DiagnosticIDs::Remark, iLoc, getFormat(iFormat));
    }
    DiagnosticBuilder WarningReport(SourceLocation iLoc, std::string const& iFormat)
    {
        return Report(DiagnosticIDs::Warning, iLoc, getFormat(iFormat));
    }
    DiagnosticBuilder ErrorReport(SourceLocation iLoc, std::string const& iFormat)
    {
        return Report(DiagnosticIDs::Error, iLoc, getFormat(iFormat));
    }
    DiagnosticBuilder FatalReport(SourceLocation iLoc, std::string const& iFormat)
    {
        return Report(DiagnosticIDs::Fatal, iLoc, getFormat(iFormat));
    }

    DiagnosticsEngine& getDiags() { return *mDiags; }

//      ---<<< 複数箇所で用いるメッセージの定義 >>>---

    static inline char const*   getNotSupportUnicon()
        { return "theolizer does not support union.";}
    static inline char const*   getNotSupportNoName()
        { return "theolizer does not support no-named class/struct";}
};

//----------------------------------------------------------------------------
//      グローバル変数
//----------------------------------------------------------------------------

CustomDiag              gCustomDiag;

//----------------------------------------------------------------------------
//      ソース不正修正エラー表示用マクロ
//----------------------------------------------------------------------------

#define ERROR(dCond, dDecl, ...)                                            \
    do {                                                                    \
        if (dCond) {                                                        \
            gCustomDiag.ErrorReport(dDecl->getLocation(),                   \
                "Do not modify the source generated by Theolizer.(%0)")     \
            << __LINE__;                                                    \
    return __VA_ARGS__;                                                     \
        }                                                                   \
    } while(0)

#define ERROR_ST(dCond, dStmt, ...)                                         \
    do {                                                                    \
        if (dCond) {                                                        \
            gCustomDiag.ErrorReport(dStmt->getLocStart(),                   \
                "Do not modify the source generated by Theolizer.(%0)")     \
            << __LINE__;                                                    \
    return __VA_ARGS__;                                                     \
        }                                                                   \
    } while(0)

#define ERROR_LOC(dCond, dLoc, ...)                                         \
    do {                                                                    \
        if (dCond) {                                                        \
            gCustomDiag.ErrorReport(dLoc,                                   \
                "Do not modify the source generated by Theolizer.(%0)")     \
            << __LINE__;                                                    \
    return __VA_ARGS__;                                                     \
        }                                                                   \
    } while(0)

#define WARNING(dCond, dDecl, ...)                                          \
    if (dCond) {                                                            \
        gCustomDiag.WarningReport(dDecl->getLocation(),                     \
            "Theolizer unkown pattern. Please report to Theolizer developper.(%0)")\
        << __LINE__;                                                        \
    return __VA_ARGS__;                                                     \
    }                                                                       \

#define WARNING_CONT(dCond, dDecl)                                          \
    if (dCond) {                                                            \
        gCustomDiag.WarningReport(dDecl->getLocation(),                     \
            "Theolizer unkown pattern. Please report to Theolizer developper.(%0)")\
        << __LINE__;                                                        \
    continue;                                                               \
    }                                                                       \

#define NOT_SUPPORT(dCond, dDecl, ...)                                      \
    if (dCond) {                                                            \
        gCustomDiag.WarningReport(dDecl->getLocation(),                     \
            "Theolizer does not support pattern.");                         \
    return __VA_ARGS__;                                                     \
    }                                                                       \

#define NOT_SUPPORT_CONT(dCond, dDecl)                                      \
    if (dCond) {                                                            \
        gCustomDiag.WarningReport(dDecl->getLocation(),                     \
            "Theolizer does not support pattern.");                         \
    continue;                                                               \
    }                                                                       \

// ***************************************************************************
//      アノテーション取り出しと解析
// ***************************************************************************

//----------------------------------------------------------------------------
//      アノテーション情報
//----------------------------------------------------------------------------

struct AnnotationInfo
{
    enum Annotate
    {
        Error=-1,       // エラー
        None=0,         // アノテーション無し
        CM,             // 手動保存用Class(2016/04/06現在、未対応)
        CS,             // 自動保存用Class・デフォルト保存
        CN,             // 自動保存用Class・デフォルト非保存
        FS,             // 自動保存用Classの保存するフィールド(パラメータに保存先を指定可)
        FN,             // 自動保存用Classの保存しないフィールド
        ES,             // enum型のシンボルへのアノテーション
        TS              // Theolizer Serializer内部処理用情報
    };

    Annotate    mAnnotate;      // アノテーションの種別
    std::string mParameter;     // パラメータ

    AnnotationInfo(Annotate iAnnoate=None): mAnnotate(iAnnoate),
                                            mParameter("")
    { }

    void setAnnotation(const StringRef& iAnnotation)
    {
        if      (iAnnotation.equals("CM"))  mAnnotate = CM;
        else if (iAnnotation.equals("CS"))  mAnnotate = CS;
        else if (iAnnotation.equals("CN"))  mAnnotate = CN;
        else if (iAnnotation.equals("FS"))  mAnnotate = FS;
        else if (iAnnotation.equals("FN"))  mAnnotate = FN;
        else if (iAnnotation.equals("ES"))  mAnnotate = ES;
        else if (iAnnotation.equals("TS"))  mAnnotate = TS;
        else                                mAnnotate = Error;
    }

    char const* c_str() const
    {
        switch (mAnnotate)
        {
        case CM:    return "CM";
        case CS:    return "CS";
        case CN:    return "CN";
        case FS:    return "FS";
        case FN:    return "FN";
        case ES:    return "ES";
        case TS:    return "TS";
        default:    break;
        }
        return "<None>";
    }
};

//----------------------------------------------------------------------------
//      取り出すアノテーションの指定
//----------------------------------------------------------------------------

enum struct AnnotateType
{
    None,
    Class,
    Field,
    Enum,
    TS
};

//----------------------------------------------------------------------------
//      アノテーション情報抽出
//----------------------------------------------------------------------------

AnnotationInfo getAnnotationInfo(const Decl *iDecl, AnnotateType iAnnotateType)
{
    if (!iDecl->hasAttrs())
return AnnotationInfo();

//          ---<<< アノテーション取り出し >>>---

    std::pair<StringRef, StringRef> temp;
    AnnotateAttr* aTheolizerAttr = nullptr;

    const AttrVec &Attrs = iDecl->getAttrs();
    for (auto attr : Attrs)
    {
        AnnotateAttr *aAnnotateAttr = dyn_cast<AnnotateAttr>(attr);
        if (!aAnnotateAttr)
    continue;

        temp = aAnnotateAttr->getAnnotation().split(':');
        if (temp.first.equals("THEORIDE"))
        {
            // 2つ目ならば、エラー
            if (aTheolizerAttr)
            {
                gCustomDiag.ErrorReport(attr->getLocation(),
                    "THEOLIZER_INTERNAL_ANNOTATE() cannot appear multiple times");
return AnnotationInfo(AnnotationInfo::Error);
            }

            aTheolizerAttr = aAnnotateAttr;
        }
    }
    if (!aTheolizerAttr)
return AnnotationInfo();

//          ---<<< アノテーション解析 >>>---

    AnnotationInfo  ret;

    temp=temp.second.split(':');
    ret.setAnnotation(temp.first);
    if (ret.mAnnotate == AnnotationInfo::Error)
    {
        gCustomDiag.ErrorReport(aTheolizerAttr->getLocation(),
                "\"%0\" is unknown parameter for THEOLIZER_ANNOTATE()")
                << temp.first;
return AnnotationInfo(AnnotationInfo::Error);
    }

    switch (ret.mAnnotate)
    {
    case AnnotationInfo::None:
    case AnnotationInfo::CM:
    case AnnotationInfo::CS:
    case AnnotationInfo::FN:
        if (!temp.second.empty())
        {
            gCustomDiag.ErrorReport(aTheolizerAttr->getLocation(),
                    "%0 can not have any distination(%1).")
                    << temp.first << temp.second;
return AnnotationInfo(AnnotationInfo::Error);
        }
        break;

    case AnnotationInfo::CN:
    case AnnotationInfo::FS:
    case AnnotationInfo::ES:
    case AnnotationInfo::TS:
        ret.mParameter = temp.second;
        break;
    default:
        break;
    }

//          ---<<< エラー・チェック >>>---

    switch (iAnnotateType)
    {
    case AnnotateType::None:
    case AnnotateType::TS:
        if (ret.mAnnotate != AnnotationInfo::TS)
        {
            gCustomDiag.ErrorReport(aTheolizerAttr->getLocation(),
                "THEOLIZER_INTERNAL_ANNOTATE(%0) cannot be applied to this.")
                << ret.c_str();
return AnnotationInfo(AnnotationInfo::Error);
        }
        break;

    case AnnotateType::Class:
        if ((ret.mAnnotate != AnnotationInfo::CM)
         && (ret.mAnnotate != AnnotationInfo::CS)
         && (ret.mAnnotate != AnnotationInfo::CN))
        {
            gCustomDiag.ErrorReport(aTheolizerAttr->getLocation(),
                "THEOLIZER_INTERNAL_ANNOTATE(%0) cannot be applied to class/struct.")
                << ret.c_str();
return AnnotationInfo(AnnotationInfo::Error);
        }
        break;

    case AnnotateType::Field:
        if ((ret.mAnnotate != AnnotationInfo::FS)
         && (ret.mAnnotate != AnnotationInfo::FN))
        {
            gCustomDiag.ErrorReport(aTheolizerAttr->getLocation(),
                "THEOLIZER_INTERNAL_ANNOTATE(%0) cannot be applied to field.")
                << ret.c_str();
return AnnotationInfo(AnnotationInfo::Error);
        }
        break;

    case AnnotateType::Enum:
        if (ret.mAnnotate != AnnotationInfo::ES)
        {
            gCustomDiag.ErrorReport(aTheolizerAttr->getLocation(),
                "THEOLIZER_INTERNAL_ANNOTATE(%0) cannot be applied to base class.")
                << ret.c_str();
return AnnotationInfo(AnnotationInfo::Error);
        }
        break;

    default:
        break;
    }

    return ret;
}

// ***************************************************************************
//      AST解析から抽出したクラス／enum情報
// ***************************************************************************

//----------------------------------------------------------------------------
//      シリアライズ・クラス／enum
//----------------------------------------------------------------------------

//      ---<<< 状態 >>>---

enum SerializeStat
{
    esSerializeOnly =1,
    esSaveLoadOnly  =2,
    esBoth          =3
};

//      ---<<< シリアライズ・クラス情報 >>>---

template<class tDecl>
struct SerializeInfo
{
    typedef tDecl           Decl;

    tDecl const*            mTheolizerTarget;       // class/struct/class-template/enum
    tDecl const*            mUniqueClass;           // テンプレートのUniqueClass(enumの時はnullptr)
    unsigned                mLastVersionNo;         // 最新版のバージョン番号
    unsigned                mAdditionalInfo;        // ElementsMapping/EnumSaveType
    AnnotationInfo          mAnnotationInfo;        // クラス・アノテーション情報
    AnnotationInfo          mAnnotationInfoTS;      // 内部処理用アノテーション
    CXXRecordDecl const*    mNonIntrusive;          // TheolizerNonIntrusive<>のDecl(非侵入型手動)
    bool                    mIsManual;              // 手動型
    bool                    mIsFullAuto;            // 完全自動型
    bool                    mIsChanged;             // 完全自動型から半自動型へ変更
    bool                    mIsRegisteredClass;     // THEOLIZER_REGISTER_CLASS()指定クラス
    CXXRecordDecl const*    mTheolizerVersionLast;  // 最新版のTheolizerVersion<>
    CXXRecordDecl const*    mTheolizerVersionPrev;  // １つ前のTheolizerVersion<>
    std::forward_list<CXXRecordDecl const*>         // 全てのTheolizerVersion<>のリスト
                            mTheolizerVersionList;
    std::string const       mClassName;             // クラス名
    FullSourceLoc           mLastVersionNoLoc;      // kLastVersionNo定義位置
    FullSourceLoc           mVersionNoLastLoc;      // 最新版のkVersionNo定義位置
    FullSourceLoc           mVersionNoPrevLoc;      // １つ前のkVersionNo定義位置
    SerializeStat           mSerializeStat;

    SerializeInfo(tDecl const* iTheolizerTarget,
                  tDecl const* iUniqueClass,
                  unsigned iLastVersionNo,
                  unsigned iAdditionalInfo,
                  AnnotationInfo iAnnotationInfo,
                  AnnotationInfo iAnnotationInfoTS,
                  CXXRecordDecl const* iNonIntrusive,
                  bool iIsManual,
                  bool iIsFullAuto,
                  SerializeStat iSerializeStat)
          : mTheolizerTarget(iTheolizerTarget),
            mUniqueClass(iUniqueClass),
            mLastVersionNo(iLastVersionNo),
            mAdditionalInfo(iAdditionalInfo),
            mAnnotationInfo(iAnnotationInfo),
            mAnnotationInfoTS(iAnnotationInfoTS),
            mNonIntrusive(iNonIntrusive),
            mIsManual(iIsManual),
            mIsFullAuto(iIsFullAuto),
            mIsChanged(false),
            mIsRegisteredClass(false),
            mTheolizerVersionLast(nullptr),
            mTheolizerVersionPrev(nullptr),
            mClassName(getQualifiedName(iTheolizerTarget)),
            mSerializeStat(iSerializeStat)
    { }
};

//      ---<<< プライマリー・テンプレート取り出し >>>---

CXXRecordDecl const* getPrimary(CXXRecordDecl const* iTheolizerTarget)
{
    // プライマリー・テンプレート展開を試みる
    ClassTemplateSpecializationDecl const* ctsd = 
        dyn_cast<ClassTemplateSpecializationDecl>(iTheolizerTarget);
    if (ctsd)
    {
        ClassTemplateDecl* ctd = ctsd->getSpecializedTemplate();
        iTheolizerTarget = ctd->getMostRecentDecl()->getTemplatedDecl();
        ASTANALYZE_OUTPUT("      CXXRecordDecl -> Primary ctd=", ctd,
                          " : ", iTheolizerTarget->getQualifiedNameAsString(),
                          " : ", ctd->getMostRecentDecl()->getTemplatedDecl(),
                          " : ", iTheolizerTarget->getCanonicalDecl ());
    }
    return iTheolizerTarget;
}

// EnumDecl用のダミー

EnumDecl const* getPrimary(EnumDecl const* iTheolizerTarget)
{
    return iTheolizerTarget;
}

//      ---<<< シリアライズ・クラスのリスト >>>---

static const unsigned kInvalidIndex=std::numeric_limits<unsigned>::max();

template<class tDecl>
class SerializeList
{
private:
    // シリアライズ候補のリスト（生成順を決めるため、出現順序を記録する)
    unsigned    mDeclIndex;
    typedef std::map<tDecl const*, unsigned>    DeclMapType;
    DeclMapType mDeclMap;

    // シリアライズ対象のリスト
    typedef std::map<tDecl const*, SerializeInfo<tDecl> > MapType;
    MapType mMap;

public:
    SerializeList() : mDeclIndex(0) { }

    // clear
    void clear() { mDeclIndex=0; mDeclMap.clear(); mMap.clear(); }

    // シリアライズ候補登録
    void addCandidate(tDecl const* iCandidate)
    {
        ASTANALYZE_OUTPUT("addCandidate(", iCandidate->getQualifiedNameAsString(),
                          " : ", iCandidate,
                          ") : ", mDeclIndex);
        mDeclMap.emplace(iCandidate, mDeclIndex++);
    }
    // 指定シリアライズ候補のインデックス獲得
    unsigned getIndex(tDecl const* iCandidate)
    {
        auto pos=mDeclMap.find(iCandidate);
        if (pos == mDeclMap.end())
        {
            ASTANALYZE_OUTPUT("getIndex(", iCandidate->getQualifiedNameAsString(),
                              " : ", iCandidate,
                              ") Error");
            gCustomDiag.FatalReport(iCandidate->getLocation(),
                "Can not find decl. %0") << iCandidate->getQualifiedNameAsString();
    return kInvalidIndex;
        }
        return pos->second;
    }

    // シリアライズ指定クラス登録(重複していたら、エラー)
    void addSerializable(tDecl const* iTheolizerTarget,
                         tDecl const* iUniqueClass,
                         unsigned iLastVersionNo,
                         unsigned iAdditionalInfo,
                         AnnotationInfo iAnnotationInfo,
                         AnnotationInfo iAnnotationInfoTS,
                         CXXRecordDecl const* iNonIntrusive,
                         bool iIsManual,
                         bool iIsFullAuto)
    {
        auto pos = mMap.lower_bound(iTheolizerTarget);
        if ((pos != mMap.end()) && (pos->first == iTheolizerTarget))
        {
            // 完全自動型から半自動のVersion.2へ変わった場合の完全自動型はNOP。
            //  エラーにせず、登録もしない。（Version.2の侵入型で処理されるので)
            //  また、手動型Version.2への直接変更はできない。
            //  なお、enum型はNonIntrusiveのiIsManual=falseにて登録している。
            if (iIsFullAuto && !pos->second.mIsManual && (pos->second.mLastVersionNo <= 2))
            {
                ASTANALYZE_OUTPUT("addSerializable(",
                                  iTheolizerTarget->getQualifiedNameAsString(), ")"
                                  " is changed FullAuto to HalfAuto.");
                  pos->second.mIsChanged=true;
            }
            else
            {
                Decl const* aTargetClass=iNonIntrusive;
                if (!aTargetClass) aTargetClass = iTheolizerTarget;
                gCustomDiag.ErrorReport(aTargetClass->getLocation(),
                    "Multiple THEOLIZER_INTRUSIVE_XXX() or THEOLIZER_NON_INTRUSIVE_XXX()"
                    " for same class.");
            }
    return;
        }
        ASTANALYZE_OUTPUT("SerializeList::addSerializable(",
                                iTheolizerTarget->getQualifiedNameAsString(),
                          " ",  iTheolizerTarget,
                          ", ", iAnnotationInfo.c_str(),
                          ", ", iAnnotationInfoTS.c_str(),
                          ", ", (iNonIntrusive)?iNonIntrusive->getQualifiedNameAsString():"<NULL>",
                          ", iIsManual=", iIsManual,
                          ", iIsFullAuto=", iIsFullAuto,
                          ");");

        mMap.emplace_hint
        (
            pos,
            iTheolizerTarget,
            SerializeInfo<tDecl>
            (
                iTheolizerTarget,
                iUniqueClass,
                iLastVersionNo,
                iAdditionalInfo,
                iAnnotationInfo,
                iAnnotationInfoTS,
                iNonIntrusive,
                iIsManual,
                iIsFullAuto,
                esSerializeOnly
            )
        );
    }

    // save/loadクラス登録
    //  重複の場合、登録しない。
    //  登録したクラスの内容を追跡処理するならtrue返却。(esSerializeOnly、かつ、手動型でない)
    bool addSaveLoad(tDecl const* iTheolizerTarget, bool iIsRegisteredClass=false)
    {
        auto pos = mMap.lower_bound(iTheolizerTarget);
        // 見つからなかった時、プライマリーを取り出して再トライ
        if ((pos == mMap.end()) || (pos->first != iTheolizerTarget))
        {
            tDecl const* aTheolizerTarget=getPrimary(iTheolizerTarget);
            if (aTheolizerTarget != iTheolizerTarget)
            {
                iTheolizerTarget=aTheolizerTarget;
                pos = mMap.lower_bound(iTheolizerTarget);
            }
        }

        // 登録済の時の処理
        if ((pos != mMap.end()) && (pos->first == iTheolizerTarget))
        {
            ASTANALYZE_OUTPUT("      SerializeList::addSaveLoad(",
                              iTheolizerTarget->getQualifiedNameAsString(),
                              " ", (void*)iTheolizerTarget,
                              ") : alreay registered. Stat=",
                              static_cast<int>(pos->second.mSerializeStat));
            pos->second.mIsRegisteredClass=iIsRegisteredClass;
            if (pos->second.mSerializeStat == esSerializeOnly)
            {
                ASTANALYZE_OUTPUT("      set esBoth");
                pos->second.mSerializeStat=esBoth;
    return !pos->second.mIsManual;
            }

            ASTANALYZE_OUTPUT("      return false;");
    return false;
        }
        ASTANALYZE_OUTPUT("      SerializeList::addSaveLoad(",
                          iTheolizerTarget->getQualifiedNameAsString(),
                          " ", (void*)iTheolizerTarget,
                          ") : register.");

        // 未登録時の処理
        //  シリアライズ指定がないので、完全自動型である。
        //  従ってLastVersionNo=1/iAdditionalInfo=0/mIsFullAuto=true
        pos=mMap.emplace_hint
            (
                pos,
                iTheolizerTarget,
                SerializeInfo<tDecl>
                (
                    iTheolizerTarget,
                    nullptr,
                    1,
                    0,
                    AnnotationInfo(),
                    AnnotationInfo(),
                    nullptr,
                    false,
                    true,
                    esSaveLoadOnly
                )
            );
        pos->second.mIsRegisteredClass=iIsRegisteredClass;
        return !pos->second.mIsManual;
    }

    // TheolizerVersion情報の付加
    void annexVersion(tDecl const* iTheolizerTarget,
                      CXXRecordDecl const* iTheolizerVersion, unsigned iVersionNo,
                      FullSourceLoc iLastVersionNoLoc, FullSourceLoc iVersionNoLoc)
    {
        auto pos = mMap.find(iTheolizerTarget);

        // 削除されていたら登録しない
        if (pos == mMap.end())
    return;

        if (pos->second.mLastVersionNo == iVersionNo)
        {
            ASTANALYZE_OUTPUT("SerializeList::annexVersion(",
                              iTheolizerTarget->getQualifiedNameAsString(),
                              " ", (void*)iTheolizerTarget,
                              ", ", iTheolizerVersion->getQualifiedNameAsString(), ", ",
                              iVersionNo, ") : last ", (void*)iTheolizerVersion);
            pos->second.mTheolizerVersionLast=iTheolizerVersion;
            pos->second.mVersionNoLastLoc=iVersionNoLoc;
            if (iLastVersionNoLoc.isValid())
            {
                pos->second.mLastVersionNoLoc=iLastVersionNoLoc;
            }
        }
        else if ((pos->second.mLastVersionNo-1) == iVersionNo)
        {
            ASTANALYZE_OUTPUT("SerializeList::annexVersion(",
                              iTheolizerTarget->getQualifiedNameAsString(),
                              " ", (void*)iTheolizerTarget,
                              ", ", iTheolizerVersion->getQualifiedNameAsString(), ", ",
                              iVersionNo, ") : prev ", (void*)iTheolizerVersion);
            pos->second.mTheolizerVersionPrev=iTheolizerVersion;
            pos->second.mVersionNoPrevLoc=iVersionNoLoc;
            if (iLastVersionNoLoc.isValid())
            {
                pos->second.mLastVersionNoLoc=iLastVersionNoLoc;
            }
        }
        // 最新版より1つだけ新しいものは登録しないことにより、削除される。
        //  それ以上新しいものがあるとエラー
        else if ((pos->second.mLastVersionNo+1) < iVersionNo)
        {
            Decl const* aTargetClass=pos->second.mNonIntrusive;
            if (!aTargetClass) aTargetClass = pos->second.mTheolizerTarget;

            gCustomDiag.ErrorReport(aTargetClass->getLocation(),
                                    "Can not down plural versions in once.");
        }

        // TheolizerVersion<>のリストに登録する
        if (iVersionNo <= pos->second.mLastVersionNo)
        {
            pos->second.mTheolizerVersionList.emplace_front(iTheolizerVersion);
        }
    }

    // 内部のmapを返却する
    MapType& getList() {return mMap;}

    // 指定Declを検索する
    Iterator<MapType> find(tDecl const* iTheolizerTarget)
    {
        auto pos = mMap.find(iTheolizerTarget);
        // 見つからなかった時、プライマリーを取り出して再トライ
        if (pos == mMap.end())
        {
            tDecl const* aTheolizerTarget=getPrimary(iTheolizerTarget);
            if (aTheolizerTarget != iTheolizerTarget)
            {
                iTheolizerTarget=aTheolizerTarget;
                pos = mMap.find(iTheolizerTarget);
            }
        }
        return Iterator<MapType>(pos, &mMap);
    }
};

//----------------------------------------------------------------------------
//      AST解析処理からの出力
//----------------------------------------------------------------------------

struct AstInterface
{
//      ---<<< プリプロセスからの出力 >>>---
//          first : #undef、second : #defineとする
//          #define～#undefの間にあるキーを見つけるようにするため。

    bool    mNotParse;      // 解析処理を行わない時true THEOLIZER_NO_ANALYZE
    bool    mDefineGVNT;    // グローバル・バージョン番号テーブル定義する時true

    struct UpdateInfo
    {
        FullSourceLoc   mStart;             // 開始位置
        bool            mIsTheolizerHpp;    // 自動生成ソース・ファイル内ならtrue
        bool            mIsUpdateAllowed;   // 更新許可
        UpdateInfo
        (
            FullSourceLoc iStart,
            bool iIsTheolizerHpp,
            bool iIsUpdateAllowed
        ) : mStart(iStart),
            mIsTheolizerHpp(iIsTheolizerHpp),
            mIsUpdateAllowed(iIsUpdateAllowed)
        { }
    };

    // THEOLIZER_GENERATING_LOCATIONマクロ展開の位置リスト
    std::map<FullSourceLoc, bool>           mGeneratingLocationsMacro;

    // THEOLIZER_GENERATED_LAST_VERSION_NOマクロ定義／解除の位置リスト
    // THEOLIZER_GENERATED_VERSION_NOマクロ定義／解除の位置リスト
    typedef std::map<FullSourceLoc, UpdateInfo> LocationMap;
    LocationMap                             mLastVersionNoLocations;
    LocationMap                             mVersionNoLocations;

    // 自動生成デフォルト位置
    FullSourceLoc                           mLocationDefault;

//      ---<<< デフォルトの自動生成ソース関連 >>>---

    bool                                    mDoSaveTheolizerHpp;

//      ---<<< AST抽出情報 >>>---

    // グローバル・バージョン番号テーブルのDecl
    CXXRecordDecl*                          mGlobalVersionNoTableDecl;

    // BaseTypeInfoのDecl
    ClassTemplateDecl*                      mMidSerializerDecl;

    // シリアライズ対象クラスのDeclのリスト
    SerializeList<CXXRecordDecl>            mSerializeListClass;

    // シリアライズ対象enumのDeclのリスト
    SerializeList<EnumDecl>                 mSerializeListEnum;

    // #ifdef THEOLIZER_WRITE_CODE～#endif位置リスト(終了位置, 開始位置)
    std::map<FullSourceLoc, FullSourceLoc>  mLocations;

//      ---<<< コンストラクタ >>>---

    AstInterface() : 
        mNotParse(false),
        mDefineGVNT(false),
        mDoSaveTheolizerHpp(false),
        mGlobalVersionNoTableDecl(nullptr),
        mMidSerializerDecl(nullptr)
    { }
};

// ***************************************************************************
//      グローバル変数群
// ***************************************************************************

//      ---<<< 開発主体名 >>>---

std::string             gDeveloper;

//      ---<<< コンパイル対象ファイル >>>---

std::string             gCompilingSourceName;

//      ---<<< 自動ソース生成ファイル >>>---

boostF::path            gDefaultSourceName;
bool                    gMadeDefaultSource;

//      ---<<< AST処理用 >>>---

CompilerInstance*       gCompilerInstance=nullptr;
ASTContext*             gASTContext=nullptr;
SourceManager*          gSourceManager=nullptr;

// AST解析をリトライする
//  バージョンアップを検出した時、最新版のマクロが未生成の状態でAST解析するため、
//  旧バージョンのみで保存されているクラスのsave/loadが生成されない。
//  その場合にTHEOLIZER_GENERATED_NO_COMPILEが付いてしまう問題に対処する
// 2016/09/12 これだけではダメだった。
//  派生クラスからのみ保存されていた基底クラスがあり、
//  それをバーションアップ時に派生クラスから解除した場合、
//  save/load検出がうまく行っていない。
//  暫定処置として、THEOLIZER_GENERATED_NO_COMPILEを付けないようにしている。
bool                    gRetryAST;

//      ---<<< ソース・ファイルの排他制御 >>>---

ExclusiveControl        gExclusiveControl(kTheolizerFileLock);

//      ---<<< 固定文字列 >>>---

char const*const kElementsMapping[]={"emName", "emOrder"};
char const*const kEnumSaveType[]={"estName", "estValue"};

//      ---<<< 改行 >>>---

#if defined(_WIN32)
    static StringRef const  kLineEnding{"\r\n"};
#else
    static StringRef const  kLineEnding{"\n"};
#endif

#endif

// Logファイルの自動削除経過秒数(デフォルトは10分)
unsigned            gAutoDeleteSec=600;

//      ---<<< TMPファイル削除用 >>>---

#if defined(_WIN32)
    std::string     gTempFile;
#endif

