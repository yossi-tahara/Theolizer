//############################################################################
//      各serializerのテスト
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

#define THEOLIZER_INTERNAL_EXCLUDE_VERSION_H

//############################################################################
//      通常のインクルード
//############################################################################

#ifdef _MSC_VER
    #pragma warning(disable:4100 4127)
#endif

#include "test_serializer_base02.h"
#include "test_serializer_base_version.h"

#include <sstream>
#include <fstream>
#include <typeindex>
using namespace std;

#include <theolizer/test_tool.h>
using theolizer::u8string;

//############################################################################
//      テストのメイン関数
//          ・エラー検出
//              トップ・レベルの型違い
//              クラス内の型違い
//              多重例外→データを途中でなくす
//
//          ・エラー報告
//              mError
//              例外
//              エラー・ログ
//############################################################################

THEOLIZER_ERRORLOG_FILE("ErrorLogFile%1%.log");

int main(int argc, char** argv)
{
//  theolizer::DisplayPass aDisplayPass;

    try
    {

// ***************************************************************************
//      エラー検出
// ***************************************************************************

        u8string aMessage(u8"型が異なります。");
        theolizer::removeFile("ErrorLogFile0.log");

// ---------------------------------------------------------------------------
//      型違い(TypeCheck)
// ---------------------------------------------------------------------------

//      ---<<< ヘッダ(例外有り) >>>---

        {
            std::ifstream   aStream("test_change_check1.log");
            THEOLIZER_CHECK_EXCEPTION(
                theolizer::JsonISerializer<>   js(aStream);
                IntrusiveBase2  mIntrusiveBase2;
                THEOLIZER_PROCESS(js, mIntrusiveBase2); ,
                theolizer::ErrorInfo);
        }

//      ---<<< ヘッダ(例外無し) >>>---

        unsigned line0;
        {
            std::ifstream   aStream("test_change_check1.log");
            theolizer::JsonISerializer<>   js(aStream, true);
            IntrusiveBase2  mIntrusiveBase2;
            THEOLIZER_PROCESS(js, mIntrusiveBase2); line0=__LINE__;
            u8string aAdditionalInfo=string("mIntrusiveBase2{test_serializer_base02.cpp(")
                                    +std::to_string(line0)+")}";
            auto aError=js.getErrorInfo();
            THEOLIZER_CHECK(aError, aError.getErrorType());
            THEOLIZER_EQUAL(aError.getMessage(), theolizer::print(u8"Unmatch type."));
            THEOLIZER_EQUAL(aError.getAdditionalInfo(), aAdditionalInfo);
            js.resetError();
        }

// ---------------------------------------------------------------------------
//      型違い(TypeCheckByIndex)
// ---------------------------------------------------------------------------

//      ---<<< ヘッダ(例外有り) >>>---

        {
            std::ifstream   aStream("test_change_check_index1.log");
            THEOLIZER_CHECK_EXCEPTION(
                theolizer::JsonISerializer<>   js(aStream);
                IntrusiveBase2  mIntrusiveBase2;
                THEOLIZER_PROCESS(js, mIntrusiveBase2); ,
                theolizer::ErrorInfo);
        }

//      ---<<< ヘッダ(例外無し) >>>---

        unsigned line1;
        {
            std::ifstream   aStream("test_change_check_index1.log");
            theolizer::JsonISerializer<>   js(aStream, true);
            IntrusiveBase2  mIntrusiveBase2;
            THEOLIZER_PROCESS(js, mIntrusiveBase2); line1=__LINE__;
            js.resetError();
        }

//      ---<<< 確認 >>>---

// ---------------------------------------------------------------------------
//      エラー・ログ内容チェック
// ---------------------------------------------------------------------------

        {
            std::stringstream ss;
            ss << "Error(" << theolizer::ErrorKind::UnknownData << "),IntrusiveBase2.mULongLong : ";
            ss << theolizer::print(u8"Unmatch type.");

            std::stringstream ss0;
            ss0 << "Error(" << theolizer::ErrorKind::WrongUsing << "),"
                << "mIntrusiveBase2{test_serializer_base02.cpp(" << line0 << ")}"
                << " : Unmatch type.";

            std::stringstream ss1;
            ss1 << "Error(" << theolizer::ErrorKind::WrongUsing << "),"
                << "mIntrusiveBase2{test_serializer_base02.cpp(" << line1 << ")}"
                << " : Unmatch type.";

            theolizer::u8string aErrorLog = "ErrorLogFile0.log";
            std::ifstream ifs(aErrorLog.get_fstring());
            THEOLIZER_REQUIRE(ifs, "");

            std::string aString;
            bool        aResult;

            // 先頭行読み捨て
            aResult = static_cast<bool>(getline(ifs, aString));
            THEOLIZER_REQUIRE(aResult, aResult);

            for (int i=0; i < 6; ++i)
            {
                // ログ内容チェック
                aResult = static_cast<bool>(getline(ifs, aString));
                THEOLIZER_REQUIRE(aResult, aResult);

                theolizer::WorkingLog::LineHeader aLineHeader;
                size_t  aPos;
                aResult=theolizer::WorkingLog::getLineHeader(aString, aLineHeader, &aPos);
                THEOLIZER_REQUIRE(aResult, aResult);

                // メッセージは正しいか?
                switch(i)
                {
                case 2:
                    THEOLIZER_EQUAL(aString.substr(aPos), ss0.str());
                    break;

                case 5:
                    THEOLIZER_EQUAL(aString.substr(aPos), ss1.str());
                    break;

                default:
                    {
                        size_t  aEnd=aString.rfind('{');
                        THEOLIZER_EQUAL(aString.substr(aPos, aEnd-aPos), ss.str());
                    }
                    break;
                }
            }
        }
    }
    catch(theolizer::ErrorInfo& e)
    {
        std::cout << e.getMessage() << std::endl;
    }
    catch(...)
    {
        std::cout << "unknown exception!!" << std::endl;
throw;
    }

    return 0;
}
