//############################################################################
//      report.*のテスト
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

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#if defined(_MSC_VER)
    #pragma warning(disable:4100)
#endif

// ***************************************************************************
//      通常のインクルード
// ***************************************************************************

#if defined(_WIN32)
    #include <windows.h>
    #include <mmsystem.h>               // for timeBeginPeriod/timeEndPeriod
#endif

#include <sstream>
#include <fstream>
using namespace std;

#include <theolizer/test_tool.h>
#include <theolizer/report.h>

//############################################################################
//      ErrorReporterのテスト
//          ErrorReporterはtheolizer専用
//          そのテスト用スタブなので、theolizer名前空間に入れる
//############################################################################

namespace theolizer
{
namespace internal
{

// ***************************************************************************
//      エラー付加情報管理クラス
// ***************************************************************************

class APISimulator;

class AdditionalInfo : public BaseAdditionalInfo
{
    // 付加情報記録領域(深さ)
    int                     mNestingCount;

    // setErrorによるエラー報告先
    APISimulator&           mAPISimulator;

public:
    // コンストラクタ／デストラクタ
    AdditionalInfo(APISimulator& iAPISimulator) noexcept :
        mNestingCount(0),
        mAPISimulator(iAPISimulator)
    { }
    ~AdditionalInfo() noexcept { }

    // インスタンス情報登録／解除
    void pushInstanceInfo()
    {
        ++mNestingCount;
    }

    void popInstanceInfo()
    {
        mNestingCount--;
    }

    // メッセージ取り出し
    u8string getString()
    {
        return std::to_string(mNestingCount);
    }

    // エラー情報伝達
    void setError(ErrorInfo const& iErrorInfo, bool iConstructor);
};

// ***************************************************************************
//      エラー付加情報の追加／削除用RAIIクラス
// ***************************************************************************

struct AutoRestoreAdditionalInfo
{
    AdditionalInfo&     mAdditionalInfo;

    AutoRestoreAdditionalInfo(AdditionalInfo& iAdditionalInfo) noexcept :
        mAdditionalInfo(iAdditionalInfo)
    {
        mAdditionalInfo.pushInstanceInfo();
    }
    ~AutoRestoreAdditionalInfo() noexcept
    {
        mAdditionalInfo.popInstanceInfo();
    }
};

// ***************************************************************************
//      APIシミュレート用のクラス
//          このクラスのインスタンスがAPIを持つ。
//          そのAPIを異なるスレッドから呼び出す。
//              ErrorReporterはスレッド毎にエラー情報を記録する
//          このクラスのインスタンスも上記と同じエラー情報を記録する
//              こちらはスレッドを問わず、インタンス内に記録する
// ***************************************************************************

class APISimulator : public ErrorBase
{
    // エラー追加情報の管理用インスタンス
    friend class AdditionalInfo;
    AdditionalInfo  mAdditionalInfo;

    // エラー追加情報の登録／解除用RAIIクラス
    struct AutoRestoreAdditionalInfo
    {
        AdditionalInfo& mAdditionalInfo;

        AutoRestoreAdditionalInfo(AdditionalInfo& iAdditionalInfo) noexcept : 
            mAdditionalInfo(iAdditionalInfo)
        {
            mAdditionalInfo.pushInstanceInfo();
        }
        ~AutoRestoreAdditionalInfo() noexcept
        {
            mAdditionalInfo.popInstanceInfo();
        }
    };

// ---------------------------------------------------------------------------
//      以下APIスタブ
// ---------------------------------------------------------------------------

    // デストラクタ内でエラー発生テスト用クラス
    struct ThrowInDestructor
    {
        bool    mThrow;

        ThrowInDestructor(bool iThrow) noexcept : mThrow(iThrow) { }
        ~ThrowInDestructor() noexcept(false)
        {
          Releasing   aReleasing;     // 例外保護

          if (mThrow) THEOLIZER_INTERNAL_ERROR(u8"In destructor");
        }
    };

    void first()
    {
        AutoRestoreAdditionalInfo aAutoRestoreAdditionalInfo(mAdditionalInfo);

        ThrowInDestructor aThrowInDestructor(mMode >= Destructor);

        if (mMode == First) THEOLIZER_INTERNAL_ERROR(u8"In first");

        second();
    }

    void second()
    {
        AutoRestoreAdditionalInfo aAutoRestoreAdditionalInfo(mAdditionalInfo);

        if (mMode == Second) THEOLIZER_INTERNAL_ERROR(u8"In second");

        stubAPI(CallThird);
    }

    void third()
    {
        AutoRestoreAdditionalInfo aAutoRestoreAdditionalInfo(mAdditionalInfo);

        if ((mMode == Third) || (mMode == DestructorThird))
        {
            THEOLIZER_INTERNAL_ERROR(u8"In third");
        }
    }

public:
    APISimulator() : ErrorBase(), mAdditionalInfo(*this) { }

    // APIスタブの動作モード
    enum Mode
    {
        NoError,        // 例外無し
        First,          // first関数で例外
        Second,         // second関数で例外
        Third,          // thirdで例外
        CallThird,      // 再度ApiBoundaryインスンタス生成テスト用
        Destructor,     // デストラクタ内で例外を投げるテスト
        DestructorThird // 例外処理中にデストラクタ内でエラー発生テスト
    };

    // APIスタブ関数
    void stubAPI(Mode iMode)
    {
        ApiBoundary aApiBoundary(&mAdditionalInfo);
        AutoRestoreAdditionalInfo aAutoRestoreAdditionalInfo(mAdditionalInfo);

        checkError();

        if (iMode == CallThird)
        {
            third();
    return;
        }

        mMode=iMode;
        first();
    }

private:
    using ErrorBase::setError;  // ErrorBaseにAdditionalInfoをfriend指定したくないので

// ---------------------------------------------------------------------------
//      非API
// ---------------------------------------------------------------------------

private:
    // 動作モード
    Mode    mMode;
};

// ***************************************************************************
//      AdditionalInfoの実装の一部
// ***************************************************************************

// エラー情報伝達
void AdditionalInfo::setError(ErrorInfo const& iErrorInfo, bool iConstructor)
{
    mAPISimulator.setError(iErrorInfo, iConstructor);
}

}   // namespace theolizer
}   // namespace internal


//############################################################################
//      テストのメイン関数
//############################################################################

THEOLIZER_ERRORLOG_FILE("ErrorReporter%1%.log");

int main(int argc, char** argv)
{
//  theolizer::DisplayPass aDisplayPass;

    using theolizer::ErrorType;
    using theolizer::ErrorKind;
    using theolizer::ErrorInfo;
    using theolizer::internal::APISimulator;

// ***************************************************************************
//      print()関数のテスト
// ***************************************************************************

    THEOLIZER_EQUAL(theolizer::print(u8"テスト%2% 表示%1%", 101, 202), u8"テスト202 表示101");

// ***************************************************************************
//      ErrorType, ErrorKindのテスト
// ***************************************************************************

    stringstream ss;
    ss << ErrorType::None << " "
       << ErrorType::Warning << " "
       << ErrorType::Error;
    THEOLIZER_EQUAL(ss.str(), "None Warning Error");

    ss.str("");
    ss << ErrorKind::Unclassified << " "
       << ErrorKind::WrongUsing << " "
       << ErrorKind::IOError << " "
       << ErrorKind::UnknownData << " "
       << ErrorKind::UnknownVerson;
    THEOLIZER_EQUAL(ss.str(), "Unclassified WrongUsing IOError UnknownData UnknownVerson");

// ***************************************************************************
//      ErrorInfoのテスト
// ***************************************************************************

    {
        // 空の時、エラーにならない(boolへのキャストがfalseになる)ことを確認
        ErrorInfo    aErrorInfo;
        THEOLIZER_CHECK(!aErrorInfo, aErrorInfo);
    }

// ***************************************************************************
//      警告とエラー出力チェック
// ***************************************************************************

    {
//      ---<<< 準備(エラー・ログ・ファイルを削除) >>>---

        theolizer::u8string aErrorLog = "ErrorReporter0.log";
        removeFile(aErrorLog);
        removeFile(theolizer::u8string("ErrorReporter1.log"));

//      ---<<< ログ出力 >>>---

        // ErrorReporter例外発行しログ確認
        unsigned line0, line1, line2, line3, line4;

        {
            theolizer::internal::ApiBoundary aApiBoundary(nullptr);

            THEOLIZER_CHECK_EXCEPTION2(
                THEOLIZER_INTERNAL_IO_ERROR(u8"IO_ERROR"),              // dStatements
                ErrorInfo& e,                                           // dException
                (e.getErrorType() == ErrorType::Error) &&               // dJudge
                (e.getErrorKind() == ErrorKind::IOError),
                e.getMessage()                                          // dResult
            );  line0=__LINE__;
        }

        {
            theolizer::internal::ApiBoundary aApiBoundary(nullptr);

            THEOLIZER_CHECK_EXCEPTION2(
                THEOLIZER_INTERNAL_WRONG_USING(u8"WRONG_USING"),        // dStatements
                ErrorInfo& e,                                           // dException
                (e.getErrorType() == ErrorType::Error) &&               // dJudge
                (e.getErrorKind() == ErrorKind::WrongUsing),
                e.getMessage()                                          // dResult
            );  line1=__LINE__;
        }

        {
            theolizer::internal::ApiBoundary aApiBoundary(nullptr);

            THEOLIZER_CHECK_EXCEPTION2(
                THEOLIZER_INTERNAL_DATA_ERROR(u8"DATA_ERROR"),          // dStatements
                ErrorInfo& e,                                           // dException
                (e.getErrorType() == ErrorType::Error) &&               // dJudge
                (e.getErrorKind() == ErrorKind::UnknownData),
                e.getMessage()                                          // dResult
            );  line2=__LINE__;
        }

        {
            theolizer::internal::ApiBoundary aApiBoundary(nullptr);

            THEOLIZER_CHECK_EXCEPTION2(
                THEOLIZER_INTERNAL_VERSION_ERROR(u8"VERSION_ERROR"),    // dStatements
                ErrorInfo& e,                                           // dException
                (e.getErrorType() == ErrorType::Error) &&               // dJudge
                (e.getErrorKind() == ErrorKind::UnknownVerson),
                e.getMessage()                                          // dResult
            );  line3=__LINE__;
        }

        {
            theolizer::internal::ApiBoundary aApiBoundary(nullptr);

            THEOLIZER_CHECK_EXCEPTION2(
                THEOLIZER_INTERNAL_ERROR(u8"ERROR"),                    // dStatements
                ErrorInfo& e,                                           // dException
                (e.getErrorType() == ErrorType::Error) &&               // dJudge
                (e.getErrorKind() == ErrorKind::Unclassified),
                e.getMessage()                                          // dResult
            );  line4=__LINE__;
        }

//      ---<<< 確認 >>>---

        std::ifstream ifs(aErrorLog.get_fstring());
        THEOLIZER_REQUIRE(ifs, "");

        std::string aString;
        bool        aResult;
        theolizer::WorkingLog::LineHeader aLineHeader;
        size_t      aPos;
        std::stringstream ss2;

        // 先頭行チェック
        unsigned    aNumber;
        ifs >> aNumber;
        THEOLIZER_REQUIRE(aNumber==0, aNumber);
        aResult = static_cast<bool>(getline(ifs, aString));
        THEOLIZER_REQUIRE(aResult, aResult);

        // ログ内容チェック

        // メッセージ取り出しと確認
        aResult = static_cast<bool>(getline(ifs, aString));
        THEOLIZER_REQUIRE(aResult, aResult);
        aResult=theolizer::WorkingLog::getLineHeader(aString, aLineHeader, &aPos);
        THEOLIZER_REQUIRE(aResult, aResult);
        ss2.str("");
        ss2 << "Error(IOError), : IO_ERROR{test_report5.cpp(" << line0 << ")}";
        THEOLIZER_EQUAL(aString.substr(aPos), ss2.str());

        // メッセージ取り出しと確認
        aResult = static_cast<bool>(getline(ifs, aString));
        THEOLIZER_REQUIRE(aResult, aResult);
        aResult=theolizer::WorkingLog::getLineHeader(aString, aLineHeader, &aPos);
        THEOLIZER_REQUIRE(aResult, aResult);
        ss2.str("");
        ss2 << "Error(WrongUsing), : WRONG_USING{test_report5.cpp(" << line1 << ")}";
        THEOLIZER_EQUAL(aString.substr(aPos), ss2.str());

        // メッセージ取り出しと確認
        aResult = static_cast<bool>(getline(ifs, aString));
        THEOLIZER_REQUIRE(aResult, aResult);
        aResult=theolizer::WorkingLog::getLineHeader(aString, aLineHeader, &aPos);
        THEOLIZER_REQUIRE(aResult, aResult);
        ss2.str("");
        ss2 << "Error(UnknownData), : DATA_ERROR{test_report5.cpp(" << line2 << ")}";
        THEOLIZER_EQUAL(aString.substr(aPos), ss2.str());

        // メッセージ取り出しと確認
        aResult = static_cast<bool>(getline(ifs, aString));
        THEOLIZER_REQUIRE(aResult, aResult);
        aResult=theolizer::WorkingLog::getLineHeader(aString, aLineHeader, &aPos);
        THEOLIZER_REQUIRE(aResult, aResult);
        ss2.str("");
        ss2 << "Error(UnknownVerson), : VERSION_ERROR{test_report5.cpp(" << line3 << ")}";
        THEOLIZER_EQUAL(aString.substr(aPos), ss2.str());

        // メッセージ取り出しと確認
        aResult = static_cast<bool>(getline(ifs, aString));
        THEOLIZER_REQUIRE(aResult, aResult);
        aResult=theolizer::WorkingLog::getLineHeader(aString, aLineHeader, &aPos);
        THEOLIZER_REQUIRE(aResult, aResult);
        ss2.str("");
        ss2 << "Error(Unclassified), : ERROR{test_report5.cpp(" << line4 << ")}";
        THEOLIZER_EQUAL(aString.substr(aPos), ss2.str());
    }

// ***************************************************************************
//     ErrorReporterのテスト
// ***************************************************************************

    {
        APISimulator    aAPISimulator;

        // 例外無し
        aAPISimulator.stubAPI(APISimulator::NoError);

        // first関数で例外
        THEOLIZER_CHECK_EXCEPTION2
        (
            aAPISimulator.stubAPI(APISimulator::First);,            // dStatements
            ErrorInfo& e,                                           // dException
            e.getAdditionalInfo() == "2",                           // dJudge
            e.getAdditionalInfo()                                   // dResult
        );

        // エラー取り出し(ErrorReporter)
        THEOLIZER_EQUAL
        (
            theolizer::ErrorReporter::getError().getAdditionalInfo(),
            "2"
        );

        // エラー取り出し(aAPISimulator)
        THEOLIZER_EQUAL
        (
            aAPISimulator.getError().getAdditionalInfo(),
            "2"
        );

        // エラー状態のためWrongUsingエラー発生(checkError)確認(aAPISimulator)
        THEOLIZER_CHECK_EXCEPTION2
        (
            aAPISimulator.stubAPI(APISimulator::NoError);,          // dStatements
            ErrorInfo& e,                                           // dException
            e.getErrorKind() == ErrorKind::WrongUsing,              // dJudge
            e.getErrorKind()                                        // dResult
        );

        // エラー・リセットと確認(aAPISimulatorのみ)
        aAPISimulator.resetError();
        THEOLIZER_CHECK
        (
            !aAPISimulator.getError(),
            aAPISimulator.getError().getString()
        );
        THEOLIZER_EQUAL     // ErrorReporterのエラーはクリアされない
        (
            theolizer::ErrorReporter::getError().getErrorKind(),
            ErrorKind::WrongUsing
        );

        // エラー・クリアと確認(ErrorReporter)
        aAPISimulator.stubAPI(APISimulator::NoError);
        THEOLIZER_CHECK
        (
            !theolizer::ErrorReporter::getError(),
            theolizer::ErrorReporter::getError().getString()
        );

        // second関数で例外
        THEOLIZER_CHECK_EXCEPTION2
        (
            aAPISimulator.stubAPI(APISimulator::Second);,           // dStatements
            ErrorInfo& e,                                           // dException
            e.getAdditionalInfo() == "3",                           // dJudge
            e.getAdditionalInfo()                                   // dResult
        );
        aAPISimulator.resetError();

        // third関数で例外
        THEOLIZER_CHECK_EXCEPTION2
        (
            aAPISimulator.stubAPI(APISimulator::Third);,            // dStatements
            ErrorInfo& e,                                           // dException
            e.getAdditionalInfo() == "5",                           // dJudge
            e.getAdditionalInfo()                                   // dResult
        );
        aAPISimulator.resetError();

        // デストラクタで例外
        THEOLIZER_CHECK_EXCEPTION2
        (
            aAPISimulator.stubAPI(APISimulator::Destructor);,       // dStatements
            ErrorInfo& e,                                           // dException
            e.getAdditionalInfo() == "2",                           // dJudge
            e.getAdditionalInfo()                                   // dResult
        );
        aAPISimulator.resetError();

        // 例外処理中にデストラクタでエラー検出
        THEOLIZER_CHECK_EXCEPTION2
        (
            aAPISimulator.stubAPI(APISimulator::DestructorThird);,  // dStatements
            ErrorInfo& e,                                           // dException
            e.getAdditionalInfo() == "5",                           // dJudge
            e.getAdditionalInfo()                                   // dResult
        );
        aAPISimulator.resetError();
    }

// ***************************************************************************
//     ErrorReporterのマルチスレッド・テスト(内部機能のテスト)
// ***************************************************************************

#if defined(_WIN32)
    timeBeginPeriod(1); // これが無いと、開放したCPUの獲得に時間が掛かる
#endif

//      ---<<< 準備 >>>---

    // 並列実行用関数オブジェクト
    struct Thread
    {
        void operator()(ErrorKind iErrorKind)
        {
            for(unsigned i=0; i < 1000; ++i)
            {
                {
                    theolizer::internal::ApiBoundary aApiBoundary(nullptr);

                    THEOLIZER_CHECK_EXCEPTION2
                    (
                        theolizer::internal::throwDeferred
                        (
                            theolizer::print(u8"dummy"),
                            iErrorKind,
                            THEOLIZER_INTERNAL_FILE,
                            __LINE__
                        );,                                             // dStatements
                        ErrorInfo& e,                                   // dException
                        e.getErrorKind() == iErrorKind,                 // dJudge
                        e.getErrorKind()                                // dResult
                    );
                }

                // 他のスレッドのエラー記録を確実に割り込ませる
                //  他のスレッドの値へ化けてなければスレッド毎管理できている
                std::this_thread::sleep_for(std::chrono::microseconds(1));

                THEOLIZER_EQUAL
                (
                    theolizer::ErrorReporter::getError().getErrorKind(),
                    iErrorKind
                );
            }
        }
    };

//      ---<<< テスト >>>---

    {
        auto aUnclassified =theolizer::ThreadGuard(Thread(), ErrorKind::Unclassified);
        auto aWrongUsing   =theolizer::ThreadGuard(Thread(), ErrorKind::WrongUsing);
        auto aIOError      =theolizer::ThreadGuard(Thread(), ErrorKind::IOError);
        auto aUnknownData  =theolizer::ThreadGuard(Thread(), ErrorKind::UnknownData);
        auto aUnknownVerson=theolizer::ThreadGuard(Thread(), ErrorKind::UnknownVerson);
    }   // ←ThreadGuardのデストラクタで終了待ち

#if defined(_WIN32)
    timeEndPeriod(1);
#endif

    return 0;
}
