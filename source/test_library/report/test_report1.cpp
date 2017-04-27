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
    #pragma warning(disable:4100 4996)
#endif

// ***************************************************************************
//      通常のインクルード
// ***************************************************************************

#if defined(_WIN32)
    #include <windows.h>
    #include <mmsystem.h>               // for timeBeginPeriod/timeEndPeriod
#endif

#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <tuple>
using namespace std;

#include <theolizer/test_tool.h>
#include <theolizer/report.h>

// ***************************************************************************
//      子プロセス呼び出しと結果取り出し
// ***************************************************************************

std::string gLog("test.log");

theolizer::u8string execute(theolizer::u8string const& iCommand)
{
    int ret=system(iCommand.c_str());
    std::cout << "system() returned " << ret << "\n";

    ifstream ifs(gLog);
    std::string line;
    getline(ifs, line);
    getline(ifs, line);

    return theolizer::u8string(line, theolizer::MultiByte());
}

// ***************************************************************************
//      テストのメイン関数
// ***************************************************************************

THEOLIZER_ERRORLOG_FILE("ErrorLog%1%.log");

int main(int argc, char** argv)
{
//  theolizer::DisplayPass aDisplayPass;

//----------------------------------------------------------------------------
//      getLineHeaderのテスト(テストに使用するので最初にテストする)
//----------------------------------------------------------------------------

    {

//      ---<<< 正常系 >>>---

        theolizer::WorkingLog::LineHeader aLineHeader;
        bool        aRet;
        size_t      aPos;

        std::string aString(u8",2015-10-31,12:34:56.789,1234,_54321,誕生日おめでとう！！");
        aRet=theolizer::WorkingLog::getLineHeader(aString, aLineHeader, &aPos);
        THEOLIZER_CHECK(aRet, aRet);

        tm *aTm = localtime(&aLineHeader.mDateTime);
        THEOLIZER_EQUAL(aTm->tm_year+1900, 2015);
        THEOLIZER_EQUAL(aTm->tm_mon+1,     10);
        THEOLIZER_EQUAL(aTm->tm_mday,      31);
        THEOLIZER_EQUAL(aTm->tm_hour,      12);
        THEOLIZER_EQUAL(aTm->tm_min,       34);
        THEOLIZER_EQUAL(aTm->tm_sec,       56);
        THEOLIZER_EQUAL(aLineHeader.mMilliseconds, 789);
        THEOLIZER_EQUAL(aLineHeader.mWaitedMicroseconds, 1234);
        THEOLIZER_EQUAL(aLineHeader.mThereadId, "54321");
        THEOLIZER_EQUAL(aString.substr(aPos), u8"誕生日おめでとう！！");

//      ---<<< 異常系 >>>---

        // 最初の1つが既にエラー
        aString = u8"                                     ";
        aRet=theolizer::WorkingLog::getLineHeader(aString, aLineHeader, &aPos);
        THEOLIZER_CHECK(!aRet, aRet);

        // 最後のカンマ無し
        aString = u8",2015-10-31,12:34:56.789,1234,_54321 ";
        aRet=theolizer::WorkingLog::getLineHeader(aString, aLineHeader, &aPos);
        THEOLIZER_CHECK(!aRet, aRet);

        // 短すぎる行
        aString = u8",2015-10-31,12:34:56.789,1234,_54321";
        aRet=theolizer::WorkingLog::getLineHeader(aString, aLineHeader, &aPos);
        THEOLIZER_CHECK(!aRet, aRet);
    }

//----------------------------------------------------------------------------
//      エラー・ログの正常系テスト
//          概要
//              Local時間にて検証
//              下記を確認する
//                  THEOLIZER_ERRORLOG_FILEで指定したファイルができること
//                  警告出力が適切に書き込まれること
//                  時刻が書き込み開始～完了の範囲に入っていること
//              下記は別ソースでチェックする
//                  THEOLIZER_ERRORLOG_FILEがない時に例外が発生すること
//                  異常系
/*
異常系テスト
    ファイル名が指定されていない場合、エラーにより例外が発生すること。
        別のプロセスを作るべき。→CMakeLists.txtで2回includeすればOKの筈。

    エラー・ログ・パスが変化した時の例外
        THEOLIZER_ERRORLOG_FILE()に渡すパラメータを細工すれば出来る筈。
        →ここに不具合があっても健在化する可能性は低いのでパス。
*/
//----------------------------------------------------------------------------

    {
//      ---<<< 準備(エラー・ログ・ファイルを削除) >>>---

        theolizer::u8string aErrorLog = "ErrorLog0.log";
        removeFile(aErrorLog);

//      ---<<< ログ出力 >>>---

        time_t  aDateTime    =theolizer::WorkingLog::getTime();
        THEOLIZER_INTERNAL_WARNING("Error Message"); unsigned line=__LINE__;
        time_t  aLastDateTime=theolizer::WorkingLog::getTime();

//      ---<<< 確認 >>>---

        std::ifstream ifs(aErrorLog.get_fstring());
        THEOLIZER_REQUIRE(ifs, "");

        std::string aString;
        bool        aResult;

        // 先頭行チェック
        unsigned    aNumber;
        ifs >> aNumber;
        THEOLIZER_REQUIRE(aNumber==0, aNumber);
        aResult = static_cast<bool>(getline(ifs, aString));
        THEOLIZER_REQUIRE(aResult, aResult);

        // ログ内容チェック
        aResult = static_cast<bool>(getline(ifs, aString));
        THEOLIZER_REQUIRE(aResult, aResult);

        theolizer::WorkingLog::LineHeader aLineHeader;
        size_t  aPos;
        aResult=theolizer::WorkingLog::getLineHeader(
                                    aString, aLineHeader, &aPos);
        THEOLIZER_REQUIRE(aResult, aResult);

        // 時刻が範囲内か？
        THEOLIZER_CHECK((aDateTime<=aLineHeader.mDateTime)
                     || (aLineHeader.mDateTime <= aLastDateTime),
                     aLineHeader.mDateTime);

        // メッセージは正しいか?
        std::stringstream ss;
        ss << "Warning(Unclassified), : Error Message{test_report1.cpp(" << line << ")}";
        THEOLIZER_EQUAL(aString.substr(aPos), ss.str());

//      ---<<< 後始末 >>>---

//      remove(aErrorLog);
    }

//----------------------------------------------------------------------------
//      WorkingLogクラスの異常系テスト
//          abort()関数のテスト含む
//          原因も通知できていることをチェックする
//----------------------------------------------------------------------------

    // 子プロセス呼び出しコマンド生成
    std::string command(argv[0]);
#if defined(_WIN32)
    std::size_t pos=command.rfind('.');
    if (pos == std::string::npos)
    {
        std::cout << "argv[0] format error\n";
        exit(1);
    }
    command = command.substr(0, pos) + "Child" + command.substr(pos) + " >" + gLog + " ";
#else
    command = command + "Child >" + gLog + " ";
#endif

//      ---<<< iPathのフォーマット・チェック >>>---

    {
        theolizer::u8string expected;

        char const* aNoFile = "Folder/";
        expected=theolizer::print
            (
                u8"WorkingLog : iPath(%1%)にはファイル名も入れて下さい。", aNoFile
            );
        THEOLIZER_EQUAL(execute(command+aNoFile), expected);

        char const* aNoField = "Folder/Test.log";
        expected=theolizer::print
            (
                u8"WorkingLog : iPath(%1%)に%%1%%を１つ含めて下さい。", aNoField
            );
        THEOLIZER_EQUAL(execute(command+aNoField), expected);

        char const* aIllField = "Folder%1%/Test%1%.log";
        expected=theolizer::print
            (
                u8"WorkingLog : iPath(%1%)のフォルダ・パスに%%1%%を含めないで下さい。", aIllField
            );
        THEOLIZER_EQUAL(execute(command+aIllField), expected);
    }
 
//      ---<<< iPathフォルダ部のチェック >>>---

    std::string aDirForTest = "./TestFolder/";
    {
        theolizer::u8string expected;

        // テスト用フォルダ準備
        theolizer::PrepareDir AutoDelete(aDirForTest);

        // 念のため書き込み許可
        theolizer::setWritePermission(aDirForTest, true);

        // 指定パスのフォルダ部を作れない時のテスト
        // (指定パスのフォルダ部としてファイルを作っておく。)
        theolizer::u8string aFolder = aDirForTest+u8"Folder";
        {
            std::ofstream ofs(aFolder.get_fstring());
        }
        theolizer::u8string aPath = aFolder + u8"/Test%1%.log";

        // 指定パスにフォルダを作れないことを確認する
        expected=theolizer::print
            (
                u8"WorkingLog : %1%がフォルダではありません。", aFolder
            );
        THEOLIZER_EQUAL(execute(command+aPath), expected);

        // 後始末
        theolizer::removeFile(aFolder);

        // 不正なファイル有り(不正なフォルダでテストする)
        {
            theolizer::u8string aIllFile = aFolder + u8"/Test1.log";
            theolizer::PrepareDir AutoDelete2(aIllFile.str());

            // 不正なファイルを検知することを確認する
            expected=theolizer::print
                (
                    u8"WorkingLog : %1%がファイルではありません。取り除いて下さい。", aIllFile
                );
            THEOLIZER_EQUAL(execute(command+aPath), expected);
        }
    }

//----------------------------------------------------------------------------
//      WorkingLogクラスの正常系テスト
//
//          概要
//              UTC時間にて検証
//              ①何もない状態から、ログ記録を開始し、ログ・ファイル切替発生。
//              ②①が終了した状態から、ログ記録を開始し、ログ・ファイル切替発生。
//              ③複数スレッドから書き込みする。
//                  この時、書き込みカウンタを1行毎に+1して書き込む。
//              ④全てのログ行について下記をチェックする
//                  書き込みカウンタ
//                  Mutex待ち時間が異常でないこと
//                  スレッドID
//                  ログ時刻が昇順であること
//              ⑤トータルの書き込み行数とログの行数が一致していること。
//----------------------------------------------------------------------------

#if defined(_WIN32)
    timeBeginPeriod(1); // これが無いと、開放したCPUの獲得に時間が掛かる
#endif

    {

//      ---<<< 準備 >>>---

        // テスト用フォルダ準備
        theolizer::PrepareDir AutoDelete(aDirForTest, true);

        // 並列実行用関数オブジェクト
        struct Thread
        {
            // std::thread()へ参照を渡せない。
            void operator()(theolizer::WorkingLog* iWorkingLog,
                            int* iCounter, int iNumber)
            {
                while (1)
                {
                    {   // LogStreamインスタンスでMutex獲得/開放している。
                        // なので、この中であれば排他制御が有効
                        theolizer::WorkingLog::LogStream
                                    aLogStream(iWorkingLog->getLogStream());
                        int     count=(*iCounter)++;
                        aLogStream << "iCounter=" << count;

                        if (iNumber <= *iCounter)
                break;
                    }

                    // ↓0やyield()では何故かCPUが開放されない。
                    std::this_thread::sleep_for(std::chrono::microseconds(1));
                }
            }
        };

//      ---<<< パラメータ >>>---

        // ログ・パス名
        theolizer::u8string aLogPath = aDirForTest + u8"Test%1%.log";

        // ログ・ファイル数(2固定)
        const unsigned  kFileCount=2;

        // ログ・サイズ
//      const size_t    kFileSize=350000;
        const size_t    kFileSize=35000;

        // 1セットあたりの書き込み回数
//      const int       kNumber=10000;
        const int       kNumber=700;

        // kFileSizeとkNumberは、2セットで3ファイルになるよう調整すること

        // スレッド数
        const int       kThreadCount=20;

        // セット数(2固定)
        const int       kSetCount=2;

//      ---<<< テスト実行 >>>---
//      1セット目:何もない状態からの新規ログ(aLogNo=0→1)
//      2セット目:1セット完了状態からの追記 (aLogNo=1→0)

        // 書き込み用カウンタ
        int aCounter=0;

        // チェック用カウンタ
        int aCounterForChecking=0;

        // セット開始時のログ開始位置
        std::size_t aLogPos=0;
        bool        aUseLogPos=false;

        // ログ・シーケンシャル番号
        unsigned aLogNumber=0;

        // ログ・ファイル番号
        int aLogNo=0;

        // 現在時刻設定
        time_t      aDateTime=theolizer::WorkingLog::getTime(true);
        uint32_t    aMilliseconds=0;

        for (int aSetNo=0; aSetNo < kSetCount; ++aSetNo) 
        {
//std::cout << "aSetNo=" << aSetNo << "\n";
            // ThreadIdのリスト
            vector<string> aThreadIds;

            // ログ生成
            theolizer::WorkingLog aWorkingLog(aLogPath, kFileSize, kFileCount);
            // UTC時間へ設定変更
            aWorkingLog.setUTCTime(true);

//std::cout << "--- output log...\n";
            {
                // 複数生成するため、vector化
                vector<theolizer::ThreadGuard> aThreads;

                // 実行
                for (int i=0; i < kThreadCount; ++i)
                {
                    aThreads.emplace_back(
                        theolizer::ThreadGuard(Thread(), &aWorkingLog,
                                                &aCounter, kNumber*(aSetNo+1))
                    );
                    aThreadIds.emplace_back(aThreads[i].getThreadId());
                }
            }   // ←ThreadGuardのデストラクタで終了待ち
//std::cout << "--- output log ended\n";

            // 終了時刻設定
            time_t      aLastDateTime=theolizer::WorkingLog::getTime(true);

            //結果確認
            for (int no=0; no < 2; ++no)
            {
                std::string aString;
//std::cout << "aLogNo=" << aLogNo << "\n";
//std::cout << "aUseLogPos=" << aUseLogPos << " aLogPos=" << aLogPos << "\n";
//std::cout << "aCounter=" << aCounter << " aCounterForChecking=" << aCounterForChecking << "\n";
                theolizer::u8string aFilePath=theolizer::print(aLogPath, aLogNo);
                std::ifstream ifs(aFilePath.get_fstring());
//std::cout << "aFilePath" << aFilePath.str() << " ifs=" << (ifs?1:0) << "\n";
                if (aUseLogPos)
                {
                    aUseLogPos=false;
                    ifs.seekg(aLogPos);
                }
                else
                {
                    unsigned    aNumber;
                    ifs >> aNumber;
                    THEOLIZER_REQUIRE(aNumber==aLogNumber, aNumber);
                    ++aLogNumber;
                    bool aResult=static_cast<bool>(getline(ifs, aString));
                    THEOLIZER_REQUIRE(aResult, aResult);
                }

                while(ifs && getline(ifs, aString))
                {
                    theolizer::WorkingLog::LineHeader aLineHeader;
                    size_t  aPos;
                    bool aResult=theolizer::WorkingLog::getLineHeader(aString, aLineHeader, &aPos);
                    THEOLIZER_REQUIRE(aResult, aResult);
//std::cout << "aResult=" << aResult << " aPos=" << aPos << "\n";
                    if (!aResult)
                continue;

                    // カウンタ・チェック
                    int aCounterInLog = std::stoul(aString.substr(aPos+9));
                    THEOLIZER_REQUIRE(aCounterInLog==aCounterForChecking, aCounterInLog);
                    aCounterForChecking++;

                    // 待ち時間が1000mSecを超えたら何かおかしい。
                    THEOLIZER_REQUIRE(aLineHeader.mWaitedMicroseconds < 1000000,
                                aLineHeader.mWaitedMicroseconds);

                    // Thread Idをチェック
                    bool aFound=false;
                    for (int i=0; i < kThreadCount; ++i)
                    {
                        if (aThreadIds[i] == aLineHeader.mThereadId)
                        {
                            aFound=true;
                    break;
                        }
                    }
                    THEOLIZER_REQUIRE(aFound, aFound);

                    // 時間が昇順になっていることをチェック
                    aResult = false;
                    if (std::tie(aDateTime, aMilliseconds)
                     <= std::tie(aLineHeader.mDateTime, aLineHeader.mMilliseconds))
                        aResult = true;
                    THEOLIZER_CHECK(aResult, aResult);
                    if (!aResult)
                    {
                        struct tm now = *localtime(&aDateTime);
                        std::cout << "Now  : " << aMilliseconds << " " << asctime(&now);

                        struct tm next = *localtime(&aLineHeader.mDateTime);
                        std::cout << "Next : " << aLineHeader.mMilliseconds<<" "<<asctime(&next);
                    }

                    // 終了時刻を超えていないことをチェック
                    aResult = true;
                    if (aLastDateTime < aLineHeader.mDateTime)
                        aResult = false;
                    THEOLIZER_CHECK(aResult, aResult);
                    if (!aResult)
                    {
                        struct tm last = *localtime(&aLastDateTime);
                        std::cout << "Last : " << aLastDateTime << " " << asctime(&last);

                        struct tm head = *localtime(&aLineHeader.mDateTime);
                        std::cout << "Head : " << aLineHeader.mDateTime << " " << asctime(&head);
                    }

                    // 次のチェック用に現在時刻を更新
                    aDateTime=aLineHeader.mDateTime;
                    aMilliseconds =aLineHeader.mMilliseconds;
                }
                // ファイルの最後の位置を記録する
                ifs.clear();
                ifs.seekg(0, ios_base::end);
                aLogPos=static_cast<std::size_t>(ifs.tellg());
//std::cout << "aLogPos=" << aLogPos << "\n";

                // ログ番号更新
                aLogNo = (aLogNo+1) % kFileCount;
//std::cout << "aLogNo(1)=" << aLogNo << "\n";
            }
            // 次セットは現ログ・ファイルに追記するので、ログ番号を戻しておく
            aLogNo = (aLogNo+kFileCount-1) % kFileCount;
//std::cout << "aLogNo(2)=" << aLogNo << " aUseLogPos=" << aUseLogPos << "\n";
            // 次セットは、aLogPosからチェック開始
            aUseLogPos=true;
        }
        // カウンタの値が正しいことをチェック
        THEOLIZER_EQUAL(aCounterForChecking, kNumber*kSetCount+kThreadCount-1);
//std::cout << "aCounterForChecking=" << aCounterForChecking << "\n";
    }

#if defined(_WIN32)
    timeEndPeriod(1);
#endif

    return 0;
}
