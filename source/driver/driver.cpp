//############################################################################
//      Theolizerドライバー
//          ドライバーは、インクルードが非常に重いため、
//          コンパイル単位を１つだけとする。
//          当ファイルがコンパイル単位の中心である。
//
//          役割：
//              関連ソースのインクルード
//              パラメータ解析
//              AST解析-ソース修正呼び出し
//              元のコンパイラ呼び出し
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

#include "header_includes.h"    // 必要な外部ヘッダのインクルード
#include "utility.h"            // ユーティリティ
#include "modify.h"             // ソース修正
#include "parse.h"              // AST解析と必要情報収集

#if defined(_MSC_VER)   // disabling MSVC warnings
    #pragma warning(disable:4996)
#endif

// ***************************************************************************
//      ドライバー・モード
// ***************************************************************************

enum DriverMode {
    none,               // 対応無し
    gpp,                // g++
    cl,                 // cl
    cpp                 // cpp
};

DriverMode getDriverMode(std::string const& iStem, std::string& oDefineParam)
{
    std::string  aStem = StringRef(iStem).rtrim("-0123456789.").lower();

    DriverMode  ret = none;
    oDefineParam="-D";
    if (StringRef(aStem).endswith("g++"))
    {
       ret = gpp;
    }
    if (StringRef(aStem).endswith("cl"))
    {
       ret = cl;
       oDefineParam="/D";
    }
    if (StringRef(aStem).endswith("cpp"))
    {
       ret = cpp;
    }
    DRIVER_OUTPUT("getDriverMode(", iStem, ") -> ", aStem, " ret=", ret);

    return ret;
}

// ***************************************************************************
//      元コンパイラのリネーム時フル・パス生成
// ***************************************************************************

string makeRenamePath(string const& iExePath)
{
    string aRenamePath = llvmS::path::parent_path(iExePath);
    aRenamePath.append("/");
    aRenamePath.append(llvmS::path::stem(iExePath));
    aRenamePath.append("RenamedByTheolizer");
    aRenamePath.append(llvmS::path::extension(iExePath));
    DRIVER_OUTPUT("aRenamePath = ", aRenamePath);

    return aRenamePath;
}

// ***************************************************************************
//      実行管理クラス
//          指定exeを実行し、結果を一時ファイルへ保存する。
//          デストラクタで一時ファイルを削除する。
// ***************************************************************************

class ExecuteMan
{
    SmallString<128>    mTempFilePath;
    bool                mError;
    string              mErrorMessage;
    string              mResult;
    char*               mCurrentDir;

public:
    // コンストラクタ(指定コマンド実行)
    ExecuteMan(string const& iExePath, llvm::opt::ArgStringList& ioArgv)
    {

        mError=true;
        mErrorMessage="Unknown error";
        mResult = "";
        mCurrentDir=GetCurrentDirName();

        // 一時ファイル名生成
        std::error_code ec = llvmS::fs::createTemporaryFile("Theolizer", "txt", mTempFilePath);
        if (ec)
        {
            mErrorMessage=ec.message();
    return;
        }

        DRIVER_OUTPUT("ExecuteMan");
        DRIVER_OUTPUT("    iExePath = ", iExePath);
        DRIVER_OUTPUT("    mTempFilePath = ", mTempFilePath.begin());

        // リダイレクト
        const StringRef *aRedirects[3];
        StringRef       aEmpty;
        aRedirects[0] = &aEmpty;
        StringRef       aTempFilePath(mTempFilePath);
        aRedirects[1] = &aTempFilePath;
        aRedirects[2] = &aTempFilePath;

        ioArgv.push_back(nullptr);

        // 実行
        string aCurrentDir = llvmS::path::parent_path(iExePath);
        if (chdir(aCurrentDir.c_str()))
        {
            mErrorMessage="Can not chdir.";
    return;
        }

        bool execution_failed;
        int ret = llvmS::ExecuteAndWait(iExePath, ioArgv.data(), nullptr,
                                        aRedirects, 0, 0, &mErrorMessage,
                                        &execution_failed);
        DRIVER_OUTPUT("chdir(", aCurrentDir, ")");
        DRIVER_OUTPUT("ExecuteAndWait(", iExePath, ") = ", ret);
        DRIVER_OUTPUT("    execution_failed = ", execution_failed);
        DRIVER_OUTPUT("    mErrorMessage = ", mErrorMessage);
        if (execution_failed)
    return;

        // 結果読み出し
        {
            std::ifstream ifs(mTempFilePath.begin());
            if (ifs.fail())
            {
                mErrorMessage="Temporary file read error!";
    return;
            }
            mError=false;

            std::istreambuf_iterator<char> it(ifs);
            std::istreambuf_iterator<char> last;
            mResult = string(it, last);
        }

        DRIVER_OUTPUT("------------------------- mResult");
        DRIVER_OUTPUT(mResult);
        DRIVER_OUTPUT("-------------------------");
    }

    // デストラクタ(一時ファイル削除)
    ~ExecuteMan()
    {
        // 一時ファイル削除
        if (!mError)
            remove(mTempFilePath.begin());

        // カレント・フォルダを元に戻しておく
        int ret=chdir(mCurrentDir);
        free(mCurrentDir);
    }

    // 結果返却
    string const& GetResult()       const { return mResult; }
    bool GetError()                 const { return mError; }
    string const& GetErrorMessage() const { return mErrorMessage; }
};

// ***************************************************************************
//      TheolizerDriverであることを確認する
// ***************************************************************************

enum CheckResult
{
    ecrExecError,
    ecrIsTheolizer,
    ecrNotTheolizer
};

CheckResult CheckTheolizer(const string &iExePath)
{
    // パラメータ生成
    llvm::opt::ArgStringList aArgv;
    aArgv.push_back(iExePath.c_str());
    aArgv.push_back(kTheolizerVersionParam);

    // iExePath実行
    ExecuteMan  aExecuteMan(iExePath, aArgv);
    if (aExecuteMan.GetError())
    {
        llvm::errs() << kDiagMarker << aExecuteMan.GetErrorMessage() << "\n";
return ecrExecError;
    }

    // 結果判定
    if (aExecuteMan.GetResult().find(kTheolizerMarker) != string::npos)
return ecrIsTheolizer;

    return ecrNotTheolizer;
}

// ***************************************************************************
//      シンボリック・リンクを展開する
// ***************************************************************************

std::string getCanonicalPath(std::string const& iPath)
{
    boost::system::error_code ec;
    boostF::path aCanonicalPath = boostF::canonical(iPath, ec);

    if (ec)
    {
        llvm::errs() << kDiagMarker << "Not found " << iPath << "\n";
    }

    return aCanonicalPath.string();
}

// ***************************************************************************
//      TheolizerDriver専用処理
// ***************************************************************************

int TheolizerProc(string const& iExePath, char const* iArg)
{
    bool    aDoReplace=false;
    bool    aDoRestore=false;

//----------------------------------------------------------------------------
//      TheolizerDriverであることを応答する(バージョン表示を兼ねる)
//----------------------------------------------------------------------------

    if (StringRef(iArg).equals(kTheolizerVersionParam))
    {

        llvm::outs() << theolizer::getVersionString() << "\n";
return 0;
    }

//----------------------------------------------------------------------------
//      バラメータ取り出し
//----------------------------------------------------------------------------

    aDoReplace = StringRef(iArg).startswith(kTheolizerReplaceParam);
    aDoRestore = StringRef(iArg).startswith(kTheolizerRestoreParam);

    if (!aDoReplace && !aDoRestore)
    {
        llvm::errs() << kDiagMarker 
                     << " Unknown theolizer parameter(" << iArg << ")\n";
return 1;
    }

//----------------------------------------------------------------------------
//      元コンパイラの置換／回復
//----------------------------------------------------------------------------

    std::pair<StringRef, StringRef> aCurrent = StringRef(iArg).split('=');
    while (true)
    {
        if (aCurrent.second.empty())
    break;

        aCurrent = aCurrent.second.split(';');

        string aTargetPath = aCurrent.first;
        DRIVER_OUTPUT("aTargetPath                = ", aTargetPath);
        aTargetPath=getCanonicalPath(aTargetPath);
    if (aTargetPath.empty())
return 1;
        DRIVER_OUTPUT("aTargetPath(CanonicalPath) = ", aTargetPath);
        string aReplacePath = makeRenamePath(aTargetPath);
        CheckResult cr=CheckTheolizer(aTargetPath);
        std::error_code ec;
        switch (cr)
        {
        case ecrExecError:
            break;

        case ecrNotTheolizer:
            // 相手方がTheolizerDriver.exeでないなら置換する
            if (aDoReplace)
            {
                llvm::outs() << "Replacing " << aTargetPath << " ...\n";

                // 元のコンパイラのファイル名を変える
                ec=llvmS::fs::rename(aTargetPath, aReplacePath);
                if (ec)
                {
                    llvm::errs() << kDiagMarker << "Can not rename "
                                 << aTargetPath << " to " << aReplacePath
                                 << "(" << ec.message() << ")\n";
return 1;
                }
                llvm::outs() << "    Renamed " << aTargetPath << " to "
                             << llvmS::path::filename(aReplacePath).begin() << "\n";

                // TheolizerDriver.exeをターゲットへコピーする
                ec=llvmS::fs::copy_file(iExePath, aTargetPath);
                if (ec)
                {
                    llvm::errs() << kDiagMarker << "Can not copy to "
                                 << aTargetPath << "(" << ec.message() << ")\n";
return 1;
                }
                // パーミッションをTheolizerDriver.exeと合わせる。
                boostF::file_status file_status=boostF::status(iExePath);
                boostF::permissions(aTargetPath, file_status.permissions());
                llvm::outs() << "    Copied " << iExePath << " to "
                             << llvmS::path::filename(aTargetPath).begin() << "\n";
                llvm::outs() << "Completed !\n";
            }
            else
            {
                llvm::outs() << "Already restored " << aTargetPath << "\n";
            }
            break;

        case ecrIsTheolizer:
            // 相手方がTheolizerDriver.exeなら回復する
            if (aDoRestore)
            {
                llvm::outs() << "Restoring " << aTargetPath << " ...\n";

                // ターゲットがTheolizerDriver.exeなので削除する
                ec=llvmS::fs::remove(aTargetPath);
                if (ec)
                {
                    llvm::errs() << kDiagMarker << "Can not remove "
                                 << aTargetPath << "(" << ec.message() << ")\n";
return 1;
                }
                llvm::outs() << "    Removed " << aTargetPath << "\n";

                // 元のコンパイラを元の名前へ回復する
                ec=llvmS::fs::rename(aReplacePath, aTargetPath);
                if (ec)
                {
                    llvm::errs() << kDiagMarker << "Can not rename "
                                 << aReplacePath << " to " << aTargetPath
                                 << "(" << ec.message() << ")\n";
return 1;
                }
                llvm::outs() << "    Renamed " << aReplacePath << " to "
                             << llvmS::path::filename(aTargetPath).begin() << "\n";
                llvm::outs() << "Completed !\n";
            }
            else
            {
                llvm::outs() << "Already replaced " << aTargetPath << " by "
                             << llvmS::path::filename(iExePath).begin() << "\n";
            }
            break;

        default:
            DRIVER_ABORT("Unknown return code : CheckTheolizer() = %d\n", cr);
        }
    }
    return 0;
}

// ***************************************************************************
//      g++の情報取り出し
// ***************************************************************************

vector<string> getGppInfo(string const& iExePath, string& oTarget)
{
    vector<string> includes;

    // パラメータ生成
    llvm::opt::ArgStringList aArgv;
    aArgv.push_back(iExePath.c_str());
    aArgv.push_back("-v");
    aArgv.push_back("-x");
    aArgv.push_back("c++");
    aArgv.push_back("-E");
    aArgv.push_back("-");

    ExecuteMan  aExecuteMan(iExePath, aArgv);
    DRIVER_ASSERT(!aExecuteMan.GetError(),
                        "Execution %s error : %s\n", 
                        llvmS::path::filename(iExePath).begin(),
                        aExecuteMan.GetErrorMessage().c_str());

    // システム・インクルード開始位置
    StringRef aResult = aExecuteMan.GetResult();
    size_t start = aResult.find("#include <...>");
    if (start == StringRef::npos)
return includes;

    size_t end = aResult.find("End of search list.", start);
    if (end == StringRef::npos)
return includes;

    // パスを取り出す
    std::pair<StringRef, StringRef> aCurrent = aResult.substr(start, end-start).split('\n');
    while (true) {
        if (aCurrent.second.empty())
    break;
        aCurrent = aCurrent.second.split('\n');
        StringRef aPath = aCurrent.first.trim();
        if (aPath.size() == 0)
    continue;

        includes.push_back(aPath);
    }

    // ターゲットを取り出す
    static const char aTargetString[]="Target: ";
    static const size_t aTargetSringLen=sizeof(aTargetString)-1;
    size_t aTargetPos = aResult.find(aTargetString);
    if (aTargetPos != StringRef::npos)
    {
        aCurrent = aResult.substr(aTargetPos+aTargetSringLen).split('\n');
        oTarget=aCurrent.first;
    }

    return includes;
}

// ***************************************************************************
//      コンパイラー情報獲得
//          g++ の場合、'echo "" | g++ -v -x c++ -E -'にて取り出す
//              システム・インクルード："#include <...>
//              ターゲット            ："Target: "
//          msvcの場合
//              システム・インクルード：環境変数INCLUDE
//              ターゲット            ："無し
// ***************************************************************************

vector<string> GetCompilerInfo(const string& iExePath,
                                    DriverMode iDriverMode,
                                    string& oTarget)
{
    vector<string> aIncludes;
    oTarget = "";

    // g++なら、-vにて取り出す
    if (iDriverMode == gpp)
    {
        aIncludes = getGppInfo(iExePath, oTarget);
    }

    // それ以外なら、環境変数INCLUDEから取り出す
    else
    {
        char const* env = ::getenv("INCLUDE");
        if (env) {
            std::pair<StringRef, StringRef> aCurrent = StringRef(env).split(';');
            while (!aCurrent.second.empty())
            {
                if (!aCurrent.first.empty())
                {
                    aIncludes.push_back(aCurrent.first);
                }
                aCurrent = aCurrent.second.split(';');
            }
        }
    }

    return aIncludes;
}

// ***************************************************************************
//      メイン
// ***************************************************************************

int main(int iArgc, const char **iArgv)
{
llvm::outs() << "Theolizer Driver started.\n";
//  DISABLE_OUTPUT();
//  ENABLE_OUTPUT(KIND(Time));
//  ENABLE_OUTPUT(KIND(Time)|KIND(Parameter));
    ENABLE_OUTPUT(KIND(Time)|KIND(Driver)|KIND(Parameter)|KIND(AstAnalyze));

    FineTimer   ft;
    int i;

    char const* developer=::getenv("THEOLIZER_DEVELOPER_NAME");
    if (developer)
    {
        gDeveloper=developer;
    }

    if (IS_PARAMETER_OUTPUT)
    {
        PARAMETER_OUTPUT("Theolizer Driver : Command line parameter");
        char* aCurrentDir = GetCurrentDirName();
        PARAMETER_OUTPUT("Current Dir : ", aCurrentDir);
        free(aCurrentDir);
        for (int i=0; i < iArgc; ++i)
        {
            std::stringstream ss;
            ss << "    iArgv[" << i << "] = ";
            if (iArgv[i] == nullptr)
            {
                ss << "<nullptr>";
            }
            else
            {
                ss << "\"" << iArgv[i] << "\"";
            }
            PARAMETER_OUTPUT(ss.str());
        }
        PARAMETER_OUTPUT("THEOLIZER_DEVELOPER_NAME : ", gDeveloper);
    }

//----------------------------------------------------------------------------
//      自exeパス名操作
//----------------------------------------------------------------------------

    // 自モジュールのパスを自exeのフル・パスとする
    void *aFuncPtr = (void*)(intptr_t)main;
    string aExePath = llvmS::fs::getMainExecutable(iArgv[0], &aFuncPtr);
    DRIVER_OUTPUT("aExePath                = ", aExePath);
    aExePath=getCanonicalPath(aExePath);
    if (aExePath.empty())
return 1;
    DRIVER_OUTPUT("aExePath(CanonicalPath) = ", aExePath);

//----------------------------------------------------------------------------
//      パラメータ取り出し
//----------------------------------------------------------------------------

    // パラメータをaArgvへ設定する
    SmallVector<char const*, 256> aArgv;
    llvm::SpecificBumpPtrAllocator<char> ArgAllocator;  // ここが実体を記録している
    {
        auto array_ref = llvm::makeArrayRef(iArgv, iArgc);

        // GetArgumentVector()
        //  aArgvにはアクセスしていない。
        //  コマンド・ラインをUnicodeで取り出し、
        //  ワイルド・カード展開後、char型で保存している。
        std::error_code ec = llvmS::Process::GetArgumentVector(aArgv, array_ref, ArgAllocator);
        if (ec)
        {
            llvm::errs() << kDiagMarker
                         << "error: couldn't get arguments = " << ec.message() << "\n";
            return 1;
        }
    }

    // 応答ファイルをaArgvへ展開する
#if 0   // 3.7.0
    llvm::BumpPtrAllocator aAllocator;
    llvm::BumpPtrStringSaver Saver(aAllocator);
#else   // 3.8.0(2015/12/20)
    llvm::BumpPtrAllocator A;
    llvm::StringSaver Saver(A);
#endif
#if defined(_WIN32)
    llvm::cl::ExpandResponseFiles(Saver, llvm::cl::TokenizeWindowsCommandLine, aArgv, true);
#else
    llvm::cl::ExpandResponseFiles(Saver, llvm::cl::TokenizeGNUCommandLine, aArgv, true);
#endif

//----------------------------------------------------------------------------
//      Theolizer用パラメータ処理
//----------------------------------------------------------------------------

    // ドライバー・モード設定とマクロ定義パラメータ形式決定
    std::string aDefineParam;
    DriverMode aDriverMode = getDriverMode(llvmS::path::stem(aExePath), aDefineParam);

    // Theolizerドライバ・モード判定準備
    std::string aTheolizerAnalyze = aDefineParam+kTheolizerAnalyzeParam;
    bool aDefining=false;

    // パラメータ・チェック
    bool aDoAnalyze = false;        // Theolizer解析実行
    bool aIsClangHelp = false;
    bool aIsVersion = false;
    bool aIsNostdinc = false;
    bool aIsNostdincpp = false;
    string aOriginalPath;

    for (auto&& arg : aArgv)
    {
        if (arg == nullptr)
    continue;

        // Theolizerドライバ・モード判定
        if ((aDefining && StringRef(arg).equals(kTheolizerAnalyzeParam))
         || (StringRef(arg).equals(aTheolizerAnalyze)))
        {
PARAMETER_OUTPUT("    aDoAnalyze");
            aDoAnalyze=true;
    continue;
        }

        if (StringRef(arg).equals(aDefineParam))
        {
            aDefining=true;
    continue;
        }
        aDefining=false;

        // デバッグ動作(元コンパイラのパスを設定する)
        if (StringRef(arg).startswith(kTheolizerDebugParam))
        {
            std::pair<StringRef, StringRef> aCurrent = StringRef(arg).split('=');
            if (aCurrent.second.empty())
            {
                llvm::errs() << kDiagMarker
                             << "error: debug mode path is null.\n";
return 1;
            }
PARAMETER_OUTPUT("    aDoAnalyze and Debug");
            aDoAnalyze=true;
            aOriginalPath=aCurrent.second;
            DRIVER_OUTPUT("aOriginalPath                = ", aOriginalPath);
            aOriginalPath=getCanonicalPath(aOriginalPath);
            if (aOriginalPath.empty())
return 1;
            DRIVER_OUTPUT("aOriginalPath(CanonicalPath) = ", aOriginalPath);
            aDriverMode = getDriverMode(llvmS::path::stem(aOriginalPath), aDefineParam);
            aTheolizerAnalyze = aDefineParam+kTheolizerAnalyzeParam;

            arg=nullptr;
    continue;
        }

        if (StringRef(arg).startswith(ARG_THEOLIZER))
        {
return TheolizerProc(aExePath, arg);
        }

        if (StringRef(arg).equals("-help"))         aIsClangHelp  = true;
        if (StringRef(arg).equals("--help"))        aIsClangHelp  = true;
        if (StringRef(arg).equals("--version"))     aIsVersion    = true;
        if (StringRef(arg).equals("-nostdinc"))     aIsNostdinc   = true;
        if (StringRef(arg).equals("-nostdinc++"))   aIsNostdincpp = true;
    }

    // 元コンパイラのパス名をaExePathから生成する
    if (aOriginalPath.empty())
    {
        aOriginalPath = makeRenamePath(aExePath);
    }
    DRIVER_OUTPUT("aOriginalPath = ", aOriginalPath);

//----------------------------------------------------------------------------
//      Theolizer解析実行判定
//----------------------------------------------------------------------------

    if (!aDoAnalyze)
    {
        DRIVER_OUTPUT("##### Pass through mode. #####");
    }
    else
    {
        // ライセンス表示
        if ((aDriverMode != gpp) || (aIsVersion))
        {
            llvm::outs() << theolizer::getVersionString() << "\n";
            llvm::outs() << "\n";
            llvm::outs().flush();
        }

//----------------------------------------------------------------------------
//      パラメータをclang用に変換する
//----------------------------------------------------------------------------

        // clang helpが含まれていたら、clangのhelp messageを出したくないので、
        // スキップする。
        int aRet=0;
        if (!aIsClangHelp)
        {
            // 警告・エラー表示環境準備(最低限)
            IntrusiveRefCntPtr<DiagnosticOptions> aDiagOpts = new DiagnosticOptions;
            DiagnosticConsumer* diag_client =
                            new TheolizerDiagnosticConsumer(llvm::errs(), &*aDiagOpts);
            IntrusiveRefCntPtr<DiagnosticIDs> aDiagId(new DiagnosticIDs());
            DiagnosticsEngine Diags(aDiagId, &*aDiagOpts, diag_client);

            // ドライバー生成
            clangD::Driver TheDriver(aExePath, llvmS::getDefaultTargetTriple(), Diags);

            // ドライバー・モード用パラメータ設定
            switch (aDriverMode)
            {
            case gpp:
                aArgv.insert(aArgv.begin()+1, "--driver-mode=g++");
                break;

            case cl:
                aArgv.insert(aArgv.begin()+1, "--driver-mode=cl");
                aArgv.push_back("-Wno-ignored-attributes");// 標準ライブラリ警告を防ぐ
                aArgv.push_back("-Wno-deprecated-declarations");
                aArgv.push_back("-Wno-implicit-exception-spec-mismatch");
                aArgv.push_back("-Wno-delete-incomplete");
                aArgv.push_back("-Wno-microsoft");
                aArgv.push_back("-Wno-undefined-inline");
                aArgv.push_back("-Wno-writable-strings");
                aArgv.push_back("-Wno-dllimport-static-field-def");
                aArgv.push_back("-Wno-dangling-else");
                break;

            case cpp:
                aArgv.insert(aArgv.begin()+1, "--driver-mode=cpp");
                break;

            default:
                break;
            }

            // システム・インクルード・パス追加
            //  msvcはclangに任せる(環境変数だけではうまくいかない)
            if ((aDriverMode != cl) && !aIsNostdinc)    aArgv.push_back("-nostdinc");
            if ((aDriverMode != cl) && !aIsNostdincpp)  aArgv.push_back("-nostdinc++");

            // システム・インクルード・パス生成
            string  aTarget;
            vector<string>  aIncludes = GetCompilerInfo(aOriginalPath, aDriverMode, aTarget);

            // インクルード・パス追加
            for(auto& include : aIncludes)
            {
                if (aDriverMode == cl)
                {
                    include.insert(0, "/I");
                    aArgv.push_back(include.c_str());
                }
                else
                {
                    aArgv.push_back("-I");
                    aArgv.push_back(include.c_str());
                }
            }

            if (aTarget.size())
            {
                aTarget.insert(0, "-aTarget=");
                aArgv.push_back(aTarget.c_str());
            }

            PARAMETER_OUTPUT("BuildCompilation");
            i=0;
            for(auto arg : aArgv ){
                if (arg != nullptr) PARAMETER_OUTPUT("    aArgv[", i, "] = ", arg);
                i++;
            }
            TIME_OUTPUT("pre  BuildCompilation() time=, ", ft.GetmSec(false), ", mSec");

            // Jobs分解する
            std::unique_ptr<clangD::Compilation> compilation(TheDriver.BuildCompilation(aArgv));
            TIME_OUTPUT("post BuildCompilation() time=, ", ft.GetmSec(false), ", mSec");
            FineTimer   ft2;

            if (aIsVersion) {
                llvm::outs() << "\n";
            }

//----------------------------------------------------------------------------
//      AST解析とソース修正呼び出し処理
//----------------------------------------------------------------------------

            clangD::JobList &jobs = compilation->getJobs();

            // ジョブ毎に処理
            i=-1;
            for(const auto &job : jobs)
            {
                ++i;

                // コマンド取り出し
                const clangD::Command *command = dyn_cast<clangD::Command>(&job);
                if (!command)
            continue;

                DRIVER_OUTPUT("job[", i, "] : ", command->getExecutable());

                // Theolizer Driver以外なら非対象
                if (aExePath != command->getExecutable())
            continue;

                // オプション取り出し
                const llvm::opt::ArgStringList &args = command->getArguments();

                // -cc1でなければ非対象
                if (!args.size() || (!StringRef(*args.begin()).equals("-cc1")))
            continue;

                int ret=0;
                do
                {
                    boostI::sharable_lock<ExclusiveControl> lock(gExclusiveControl);
                    gRetryAST=false;

                    // AST解析とソース修正
                    ret=parse(aExePath, args, aTarget);
                } while(gExclusiveControl.getRedoRequest() || gRetryAST);
                if (ret) aRet=ret;

                if (IS_TIME_OUTPUT)
                {
                    TIME_OUTPUT(*(args.end() - 1),
                                " : parse time=,, ", ft2.GetmSec(), ", mSec");
                }
            }

            TIME_OUTPUT("AST analysis(included modifying source) time=, ",
                        ft.GetmSec(), ", mSec");
        }
        if (aRet)
return aRet;
    }

//----------------------------------------------------------------------------
//      元コンパイラ呼び出し
//----------------------------------------------------------------------------

    llvm::opt::ArgStringList   aArgvForCall;
    aArgvForCall.push_back(aOriginalPath.c_str());
    for (int i=1; i < iArgc; ++i)
    {
        if (StringRef(iArgv[i]).startswith(ARG_THEOLIZER))
    continue;

        if (StringRef(iArgv[i]).equals(""))
    continue;

        aArgvForCall.push_back(iArgv[i]);
    }

    // 番兵
    aArgvForCall.push_back(nullptr);

    PARAMETER_OUTPUT("----- Calling original compiler -----");
    for (int i=0; aArgvForCall[i] != nullptr; ++i)
    {
        PARAMETER_OUTPUT("    aArgvForCall[", i, "] = ", aArgvForCall[i]);
    }

    char* aCurrentDir = GetCurrentDirName();
    PARAMETER_OUTPUT("Current Dir : ", aCurrentDir);
    string error_essage;
    bool execution_failed;

    int ret=0;
    {
        boostI::sharable_lock<ExclusiveControl> lock(gExclusiveControl);

        ret = llvmS::ExecuteAndWait(aOriginalPath, aArgvForCall.data(), nullptr,
                                    nullptr, 0, 0, &error_essage,
                                    &execution_failed);
    }
    TIME_OUTPUT("Compile time=, ", ft.GetmSec(), ", mSec");

    llvm::llvm_shutdown();

    return ret;
}

// ***************************************************************************
//      ソース不正修正エラー表示用マクロ(解放)
// ***************************************************************************

#undef ERROR
#undef ERROR_ST

//############################################################################
//      コンパイラの相違点を吸収する
//############################################################################

// ***************************************************************************
//      POSIX関係
// ***************************************************************************

#ifdef _MSC_VER
#include <windows.h>
int chdir(char const* iPath)
{
    BOOL ret = SetCurrentDirectoryA(iPath);
    return (ret)?0:-1;
}

char* GetCurrentDirName()
{
    DWORD   len;
    len = GetCurrentDirectory(0, nullptr);
    char* ret = (char*)malloc(len);
    GetCurrentDirectoryA(len, ret);

    return ret;
}
#else
char* GetCurrentDirName()
{
    return getcwd(nullptr, 0);
}
#endif
