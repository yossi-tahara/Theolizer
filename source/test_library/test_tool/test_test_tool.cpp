//############################################################################
//      test_tool.*のテスト
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
//      通常のインクルード
// ***************************************************************************

#include <exception>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <locale>
using namespace std;

#define THEOLIZER_TEST_MAIN
#include <theolizer/test_tool.h>

//############################################################################
//      ユーティリティ
//############################################################################

// ***************************************************************************
//      出力チェックし、正しく<<<FAIL>>>が出ていたら、
//          Failカウンタをデクリメントし、かつ、出力を(((FAIL)))へ置き換える。
//      そうでないなら、
//          Failカウンタをインクリメントし、かつ、<<<FAIL>>>を出力する。
// ***************************************************************************

void judgeAndReplase(ostream& iOut, bool iFailIsPass, string iResult, bool iAnd=true)
{
    bool aIsPass=true;
    size_t  aPos = iResult.find(THEOLIZER_INTERNAL_FAIL);
    if (aPos != string::npos) {
        // <<<FAIL>>文字列有り
        theolizer::internal::gFailCount--;
        aIsPass = iFailIsPass & iAnd;
        iResult.replace(aPos, string(THEOLIZER_INTERNAL_FAIL).length(), "(((FAIL)))");
    } else {
        // <<<FAIL>>文字列無し
        aIsPass = !iFailIsPass & iAnd;
    }
    if (!aIsPass || theolizer::gDoDisplayPass) {
        theolizer::internal::gFailCount++;
        iOut << "\n" << ((aIsPass)?THEOLIZER_INTERNAL_PASS:THEOLIZER_INTERNAL_FAIL)
             << iResult;
    }
}

//############################################################################
//      テストのメイン関数
//############################################################################

int TestMain(int argc, char** argv)
{
//  theolizer::AutoRestoreBool aBoolRestore(theolizer::gDoDisplayPass);

//  cout << "PATH=" << theolizer::getenv("PATH") << "\n";
//  cout << "THEOLIZER_LOCALE_DIR=" << theolizer::getenv("THEOLIZER_LOCALE_DIR") << "\n";

// ***************************************************************************
//      カウンタのテスト準備
// ***************************************************************************

    unsigned    aTestCounter=0;

    {   // gTotal, gFailCountは比較後にインクリメントしている
        THEOLIZER_EQUAL(theolizer::internal::gTotal, aTestCounter);
        ++aTestCounter;
        THEOLIZER_EQUAL(theolizer::internal::gFailCount, 0);
        ++aTestCounter;
    }

// ***************************************************************************
//      環境変数ツールのテスト
// ***************************************************************************

    {
        string aResult;

        theolizer::putenv("THEOLIZER_TEST=Environment_Test");
        aResult = theolizer::getenv("THEOLIZER_TEST");
        ++aTestCounter;
        THEOLIZER_EQUAL(aResult, "Environment_Test");

        theolizer::putenv("THEOLIZER_TEST=");
        aResult = theolizer::getenv("THEOLIZER_TEST");
        ++aTestCounter;
        THEOLIZER_EQUAL(aResult, "");
    }

// ***************************************************************************
//      gDoDisplayPass(AutoRestoreBool含む)とマクロのテスト
// ***************************************************************************

//----------------------------------------------------------------------------
//      各チェック・マクロのテスト準備
//----------------------------------------------------------------------------

    ostream& out = theolizer::getOStream();
    stringstream    ss;

//----------------------------------------------------------------------------
//      gDoDisplayPassとAutoRestoreBool
//----------------------------------------------------------------------------

    {
        theolizer::AutoRestoreBool aBoolRestore(theolizer::gDoDisplayPass);

//      ---<<< gDoDisplayPassがtrue時のテスト >>>---

        // AutoRestoreBoolの戻りで回復することをチェック
        {
            theolizer::AutoRestoreBool aBoolRestore(theolizer::gDoDisplayPass);

            // {}の中でtheolizer::gDoDisplayPassがtrueであることをチェック
            ++aTestCounter;
            THEOLIZER_CHECK(theolizer::gDoDisplayPass,
                theolizer::gDoDisplayPass);
        }
        // {}を抜けると、theolizer::gDoDisplayPassが{}直前に戻ることをチェック
        ++aTestCounter;
        THEOLIZER_CHECK(theolizer::gDoDisplayPass,
            theolizer::gDoDisplayPass);

        // gDoDisplayPass
        theolizer::setOStream(ss);                          // テスタのテスト
        ss.str("");                                         // Clear ss
        ++aTestCounter;
        THEOLIZER_CHECK(true, true);                        // <<<PASS>>が出力されること
                                                            // を以下でチェックする

        theolizer::setOStream(out);                         // テスト実施
        ++aTestCounter;
        THEOLIZER_CHECK((ss.str().find(THEOLIZER_INTERNAL_PASS) != string::npos), ss.str());

//      ---<<< gDoDisplayPassがfalse時のテスト >>>---

        theolizer::gDoDisplayPass = false;
        theolizer::setOStream(out);                          // テスト実施

        // AutoRestoreBoolの戻りで回復することをチェック
        {
            theolizer::AutoRestoreBool aBoolRestore(theolizer::gDoDisplayPass);

            // {}の中でgDoDisplayPassがtrueであることをチェック
            ++aTestCounter;
            THEOLIZER_EQUAL(theolizer::gDoDisplayPass, true);
        }
        // {}を抜けると、gDoDisplayPassが{}直前に戻ることをチェック
        ++aTestCounter;
        THEOLIZER_EQUAL(theolizer::gDoDisplayPass, false);

        // gDoDisplayPass
        theolizer::setOStream(ss);                          // テスタのテスト
        ss.str("");                                         // Clear ss
        ++aTestCounter;
        THEOLIZER_EQUAL(true, true);                        // 何も出力されないこと
                                                            // を以下でチェックする

        theolizer::setOStream(out);                         // テスト実施
        ++aTestCounter;
        THEOLIZER_EQUAL(ss.str().length(), 0);
    }

//----------------------------------------------------------------------------
//      THEOLIZER_EQUAL()
//----------------------------------------------------------------------------

    theolizer::setOStream(ss);                      // テスタのテスト
    {
//      ---<<< string >>>---

        const string result_string = "Test";

        // Pass
        ss.str("");                                 // Clear ss
        ++aTestCounter;
        THEOLIZER_EQUAL(result_string, "Test");
        judgeAndReplase(out, false, ss.str());

        // Fail
        ss.str("");                                 // Clear ss
        ++aTestCounter;
        THEOLIZER_EQUAL(result_string, "Test-Error");
        judgeAndReplase(out, true, ss.str());

//      ---<<< int >>>---

        int     result_int = 123;

        // Pass
        ss.str("");                                 // Clear ss
        ++aTestCounter;
        THEOLIZER_EQUAL(result_int, 123);
        judgeAndReplase(out, false, ss.str());

        // Fail
        ss.str("");                                 // Clear ss
        ++aTestCounter;
        THEOLIZER_EQUAL(result_int, 456);
        judgeAndReplase(out, true, ss.str());
    }

//----------------------------------------------------------------------------
//      THEOLIZER_CHECK()
//----------------------------------------------------------------------------

    {
//      ---<<< string >>>---

        const string result_string = "Test";

        // Pass
        ss.str("");                                 // Clear ss
        ++aTestCounter;
        THEOLIZER_CHECK(result_string == "Test", result_string);
        judgeAndReplase(out, false, ss.str());

        // Fail
        ss.str("");                                 // Clear ss
        ++aTestCounter;
        THEOLIZER_CHECK(result_string == "Test-Error", result_string);
        judgeAndReplase(out, true, ss.str());

//      ---<<< int >>>---

        int     result_int = 123;

        // Pass
        ss.str("");                                 // Clear ss
        ++aTestCounter;
        THEOLIZER_CHECK(result_int == 123, result_int);
        judgeAndReplase(out, false, ss.str());

        // Fail
        ss.str("");                                 // Clear ss
        ++aTestCounter;
        THEOLIZER_CHECK(result_int == 456, result_int);
        judgeAndReplase(out, true, ss.str());

//      ---<<< double >>>---

        double  result_double = 123.456;

        // Pass
        ss.str("");                                 // Clear ss
        ++aTestCounter;
        THEOLIZER_CHECK(abs(result_double-123.456)<0.001, result_double);
        judgeAndReplase(out, false, ss.str());

        // Fail
        ss.str("");                                 // Clear ss
        ++aTestCounter;
        THEOLIZER_CHECK(abs(result_double-456)<0.001, result_double);
        judgeAndReplase(out, true, ss.str());
    }

//----------------------------------------------------------------------------
//      THEOLIZER_REQUIRE()
//----------------------------------------------------------------------------

    {
//      ---<<< string >>>---

        const string result_string = "Test";

        // Pass
        bool    exception=false;
        ss.str("");                                 // Clear ss
        try
        {
            ++aTestCounter;
            THEOLIZER_REQUIRE(result_string == "Test", result_string);
        }
        catch(...)
        {
            exception=true;
        }
        judgeAndReplase(out, false, ss.str(), exception == false);

        // Fail
        exception=false;
        ss.str("");                                 // Clear ss
        try
        {
            ++aTestCounter;
            THEOLIZER_REQUIRE(result_string == "Test-Error", result_string);
        }
        catch(...)
        {
            exception=true;
        }
        judgeAndReplase(out, true, ss.str(), exception == true);
    }

//----------------------------------------------------------------------------
//      THEOLIZER_CHECK_EXCEPTION()
//----------------------------------------------------------------------------

    {
        // Pass
        ss.str("");                                 // Clear ss
        ++aTestCounter;
        THEOLIZER_CHECK_EXCEPTION(throw 1; , int);
        judgeAndReplase(out, false, ss.str());

        // Fail(no-exception)
        ss.str("");                                 // Clear ss
        ++aTestCounter;
        THEOLIZER_CHECK_EXCEPTION(;, int);
        judgeAndReplase(out, true, ss.str());
    }

//----------------------------------------------------------------------------
//      THEOLIZER_REQUIRE_EXCEPTION()
//----------------------------------------------------------------------------

    {
        bool    exception=false;

        // Pass
        ss.str("");                                 // Clear ss
        try
        {
            ++aTestCounter;
            THEOLIZER_REQUIRE_EXCEPTION(throw 1; , int);
        }
        catch(...)
        {
            exception=true;
        }
        judgeAndReplase(out, false, ss.str(), exception == false);

        // Fail(no-exception)
        exception=false;
        ss.str("");                                 // Clear ss
        try
        {
            ++aTestCounter;
            THEOLIZER_REQUIRE_EXCEPTION(;, int);
        }
        catch(...)
        {
            exception=true;
        }
        judgeAndReplase(out, true, ss.str(), exception == true);
    }

//----------------------------------------------------------------------------
//      THEOLIZER_CHECK_EXCEPTION2()
//----------------------------------------------------------------------------

    {
        // Pass
        ss.str("");                                 // Clear ss
        ++aTestCounter;
        THEOLIZER_CHECK_EXCEPTION2(
            throw 1,                                // dStatements
            int e,                                  // dException
            e == 1,                                 // dJudge
            e);                                     // dResult
        judgeAndReplase(out, false, ss.str());

        // Fail(no-exception)
        ss.str("");                                 // Clear ss
        ++aTestCounter;
        THEOLIZER_CHECK_EXCEPTION2(
            ;,                                      // dStatements
            int e,                                  // dException
            e == 1,                                 // dJudge
            e);                                     // dResult
        judgeAndReplase(out, true, ss.str());

        // Fail(unmatch result)
        ss.str("");                                 // Clear ss
        ++aTestCounter;
        THEOLIZER_CHECK_EXCEPTION2(
            THEOLIZER_INTERNAL_ERROR("Test"),       // dStatements
            theolizer::ErrorInfo& e,                // dException
            e.getMessage() == "Test-Error",         // dJudge
            e.getMessage());                        // dResult
        judgeAndReplase(out, true, ss.str());
    }

//----------------------------------------------------------------------------
//      THEOLIZER_REQUIRE_EXCEPTION2()
//----------------------------------------------------------------------------

    {
        bool    exception=false;

        // Pass
        ss.str("");                                 // Clear ss
        try
        {
            ++aTestCounter;
            THEOLIZER_REQUIRE_EXCEPTION2(
                throw 1,                            // dStatements
                int e,                              // dException
                e == 1,                             // dJudge
                e);                                 // dResult
        }
        catch(...)
        {
            exception=true;
        }
        judgeAndReplase(out, false, ss.str(), exception == false);

        // Fail(no-exception)
        exception=false;
        ss.str("");                                 // Clear ss
        try
        {
            ++aTestCounter;
            THEOLIZER_REQUIRE_EXCEPTION2(
                ;,                                  // dStatements
                int e,                              // dException
                e == 1,                             // dJudge
                e);                                 // dResult
        }
        catch(...)
        {
            exception=true;
        }
        judgeAndReplase(out, true, ss.str(), exception == true);

        // Fail(unmatch result)
        exception=false;
        ss.str("");                                 // Clear ss
        try
        {
            ++aTestCounter;
            THEOLIZER_REQUIRE_EXCEPTION2(
                THEOLIZER_INTERNAL_ERROR("Test"),   // dStatements
                theolizer::ErrorInfo& e,            // dException
                e.getMessage() == "Test-Error",     // dJudge
                e.getMessage());                    // dResult
        }
        catch(...)
        {
            exception=true;
        }
        judgeAndReplase(out, true, ss.str(), exception == true);
    }

//----------------------------------------------------------------------------
//      テストの後始末
//----------------------------------------------------------------------------

    theolizer::setOStream(out);

// ***************************************************************************
//      ファイル操作のテスト
// ***************************************************************************

    // フォルダを生成出来ない時、例外が投げられることを確認
    {
        ++aTestCounter;
        THEOLIZER_CHECK_EXCEPTION(
            theolizer::PrepareDir AutoDelete("///", true); ,    // dStatements
            std::runtime_error&);                               // dException
    }

    string aDirForTest = "./TheolizerTestFolder/";
    bool result;

    // PrepareDir, isExist, remove
    {
        // aDirForTestがもしあったら削除しておく
        {
            theolizer::PrepareDir AutoDelete(aDirForTest);
            std::vector<string> aFileList=theolizer::getFileList(aDirForTest);
            for (std::size_t i=0; i < aFileList.size(); ++i)
            {
                theolizer::setWritePermission(aDirForTest+aFileList[i], true);
            }
        }

        {
            // aDirForTest生成(終了時削除しない)
            theolizer::PrepareDir AutoDelete(aDirForTest, true);

            // aDirForTest存在確認
            result = theolizer::isExist(aDirForTest);
            ++aTestCounter;
            THEOLIZER_CHECK(result, result);

            // aDirForTest削除トライ
            theolizer::removeFile(aDirForTest);

            // aDirForTest存在確認(削除されないこと)
            result = theolizer::isExist(aDirForTest);
            ++aTestCounter;
            THEOLIZER_CHECK(result, result);

            // ファイル生成
            string  aFilePath=aDirForTest + "test0.txt";
            {
                std::ofstream ofs(aFilePath);
            }

            // aFilePath存在確認
            result = theolizer::isExist(aFilePath);
            ++aTestCounter;
            THEOLIZER_CHECK(result, result);

            // aFilePath削除トライ
            theolizer::removeFile(aFilePath);

            // aFilePath存在確認(削除されること)
            result = theolizer::isExist(aFilePath);
            ++aTestCounter;
            THEOLIZER_CHECK(!result, result);

            // 複数のファイル生成
            {
                theolizer::u8string aFileName;
                aFileName=aDirForTest+u8"Ｆｉｌｅ０.txt";
                std::ofstream ofs00(aFileName.get_fstring());
                aFileName=aDirForTest+u8"Ｆｉｌｅ１.txt";
                std::ofstream ofs01(aFileName.get_fstring());
                aFileName=aDirForTest+u8"Ｆｉｌｅ２２.txt";
                std::ofstream ofs02(aFileName.get_fstring());
            }

            // ファイル・リスト獲得(正規表現無し)
            vector<string> aFileList=theolizer::getFileList(aDirForTest);
            ++aTestCounter;
            THEOLIZER_EQUAL(aFileList[0], u8"Ｆｉｌｅ０.txt");
            ++aTestCounter;
            THEOLIZER_EQUAL(aFileList[1], u8"Ｆｉｌｅ１.txt");
            ++aTestCounter;
            THEOLIZER_EQUAL(aFileList[2], u8"Ｆｉｌｅ２２.txt");
            ++aTestCounter;
            THEOLIZER_EQUAL(aFileList.size(), 3);

            // ファイル・リスト獲得(正規表現有り)
            aFileList=theolizer::getFileList(aDirForTest, u8"(Ｆｉｌｅ.\\.txt)");
            ++aTestCounter;
            THEOLIZER_EQUAL(aFileList[0], u8"Ｆｉｌｅ０.txt");
            ++aTestCounter;
            THEOLIZER_EQUAL(aFileList[1], u8"Ｆｉｌｅ１.txt");
            ++aTestCounter;
            THEOLIZER_EQUAL(aFileList.size(), 2);
        }

        // aDirForTest存在確認(削除されないこと)
        result = theolizer::isExist(aDirForTest);
        ++aTestCounter;
        THEOLIZER_CHECK(result, result);

        {
            // aDirForTest再生成(終了時削除する)
            theolizer::PrepareDir AutoDelete(aDirForTest);
        }
        // aDirForTest存在確認(削除されていること)
        result = theolizer::isExist(aDirForTest);
        ++aTestCounter;
        THEOLIZER_CHECK(!result, result);
    }

    // setWritePermission
    {
        // aDirForTest生成
        theolizer::PrepareDir AutoDelete(aDirForTest);

        // ファイル生成
        string  aFilePath=aDirForTest + "test.txt";
        {
            std::ofstream ofs(aFilePath);
        }
        // 書き込み禁止
        theolizer::setWritePermission(aFilePath, false);

// ubuntuでは書き込み禁止しても削除できてしまう。
//  書き込み禁止には頼らないのでテスト省略
#if defined(_WIN32)
        // aFilePath削除トライ
        theolizer::removeFile(aFilePath);

        // aFilePath存在確認(削除されないこと)
        result = theolizer::isExist(aFilePath);
        ++aTestCounter;
        THEOLIZER_CHECK(result, result);
#endif

        // 書き込み許可
        theolizer::setWritePermission(aFilePath, true);
    }

// ***************************************************************************
//      カウンタのテスト
// ***************************************************************************

    {   // gTotal, gFailCountは比較後にインクリメントしている
        THEOLIZER_EQUAL(theolizer::internal::gFailCount, 0);
        ++aTestCounter;
        THEOLIZER_EQUAL(theolizer::internal::gTotal, aTestCounter);
        ++aTestCounter;
    }

    return theolizer::kExitSuccess;
}
