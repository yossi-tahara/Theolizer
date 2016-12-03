//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          メイン
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

// ***************************************************************************
//      インクルード
// ***************************************************************************

// 標準ライブラリ
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

// theolizerライブラリ
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_json.h>

// プロジェクト・ヘッダ
#include "common.h"

// 自動生成ソース
#include "main.cpp.theolizer.hpp"

//############################################################################
//      テスト基本部
//############################################################################

// ***************************************************************************
//      各種テスト関数の宣言
// ***************************************************************************

//----------------------------------------------------------------------------
//      THEOLIZER_PROCESS()の基本的な使い方
//----------------------------------------------------------------------------

template<class tSerializer>
void saveBasicProcess(tSerializer& iSerializer);

template<class tSerializer>
void loadBasicProcess(tSerializer& iSerializer);

//----------------------------------------------------------------------------
//      classの様々なバリエーション
//----------------------------------------------------------------------------

//      ---<<< 使い方のサンプル・コード >>>---

void tutoriseClassVariation();

//      ---<<< 自動テスト >>>---

template<class tSerializer>
void saveClassVariation(tSerializer& iSerializer);

template<class tSerializer>
void loadClassVariation(tSerializer& iSerializer);

// ***************************************************************************
//      各テスト呼び出し
// ***************************************************************************

template<class tSerializer>
void saveBasic(tSerializer& iSerializer)
{
    saveBasicProcess(iSerializer);
    saveClassVariation(iSerializer);
}

template<class tSerializer>
void loadBasic(tSerializer& iSerializer)
{
    loadBasicProcess(iSerializer);
    loadClassVariation(iSerializer);
}

//############################################################################
//      派生Serializerテスト
//############################################################################

// ***************************************************************************
//      CheckModeを振って呼ばれ、テスト基本部を呼ぶ
// ***************************************************************************

void varyCheckMode
(
    bool                    iLoadOnly,
    unsigned                iGlobalVersionNo,
    theolizer::CheckMode    iCheckMode
)
{
    std::cout << "varyCheckMode("
              << iLoadOnly << ", "
              << iGlobalVersionNo << ", "
              << iCheckMode << "); ----------" << std::endl;

//----------------------------------------------------------------------------
//      JsonSerializer
//----------------------------------------------------------------------------

    // デフォルト処理
    if (iGlobalVersionNo == 0)
    {
        std::string filename="json-default.json";
        std::cout << "    JsonSerializer(" << filename << ")\n";

        // 保存処理
        if (!iLoadOnly)
        {
            std::ofstream   aStream(filename);
            theolizer::JsonOSerializer<> jos(aStream);
            saveBasic(jos);
        }

        // 回復処理
        {
            std::ifstream   aStream(filename);
            theolizer::JsonISerializer<> jis(aStream);
            loadBasic(jis);
        }
    }

    // パラメータ指定
    else
    {
        std::stringstream ss;
        std::string filename;

//      ---<<< PretyPrint >>>---

        ss << "json-pp-ver" << iGlobalVersionNo << "-"<< iCheckMode << ".json";
        filename=ss.str();
        std::cout << "    JsonSerializer(" << filename << ")\n";

        // 保存処理
        if (!iLoadOnly)
        {
            std::ofstream   aStream(filename);
            theolizer::JsonOSerializer<> jos(aStream, iGlobalVersionNo, iCheckMode);
            saveBasic(jos);
        }

        // 回復処理
        {
            std::ifstream   aStream(filename);
            theolizer::JsonISerializer<> jis(aStream);
            loadBasic(jis);
        }

//      ---<<< 非PretyPrint >>>---

        ss.str("");
        ss << "json-np-ver" << iGlobalVersionNo << "-"<< iCheckMode << ".json";
        filename=ss.str();
        std::cout << "    JsonSerializer(" << filename << ")\n";

        // 保存処理
        if (!iLoadOnly)
        {
            std::ofstream   aStream(filename);
            theolizer::JsonOSerializer<> jos(aStream, iGlobalVersionNo, iCheckMode, true);
            saveBasic(jos);
        }

        // 回復処理
        {
            std::ifstream   aStream(filename);
            theolizer::JsonISerializer<> jis(aStream);
            loadBasic(jis);
        }
    }

//----------------------------------------------------------------------------
//      BinarySerializer
//----------------------------------------------------------------------------

    // デフォルト処理
    if (iGlobalVersionNo == 0)
    {
        std::string filename="binary-default.bin";
        std::cout << "    BinarySerializer(" << filename << ")\n";

        // 保存処理
        if (!iLoadOnly)
        {
            std::ofstream   aStream(filename, std::ios_base::binary);
            theolizer::BinaryOSerializer<> bos(aStream);
            saveBasic(bos);
        }

        // 回復処理
        {
            std::ifstream   aStream(filename, std::ios_base::binary);
            theolizer::BinaryISerializer<> bis(aStream);
            loadBasic(bis);
        }
    }

    // パラメータ指定
    else
    {
        std::stringstream ss;
        std::string filename;
        ss << "binary-ver" << iGlobalVersionNo << "-"<< iCheckMode << ".bin";
        filename=ss.str();
        std::cout << "    BinaryOSerializer(" << filename << ")\n";

        // 保存処理
        if (!iLoadOnly)
        {
            std::ofstream   aStream(filename, std::ios_base::binary);
            theolizer::BinaryOSerializer<> bos(aStream, iGlobalVersionNo, iCheckMode);
            saveBasic(bos);
        }

        // 回復処理
        {
            std::ifstream   aStream(filename, std::ios_base::binary);
            theolizer::BinaryISerializer<> bis(aStream);
            loadBasic(bis);
        }
    }

//----------------------------------------------------------------------------
//      FastSerializer
//----------------------------------------------------------------------------

    // デフォルト処理
    if (iGlobalVersionNo == 0)
    {
        std::string filename="fast-default.bin";
        std::cout << "    FastSerializer(" << filename << ")\n";

        // 保存処理
        if (!iLoadOnly)
        {
            std::ofstream   aStream(filename, std::ios_base::binary);
            theolizer::FastOSerializer<> fos(aStream);
            saveBasic(fos);
        }

        // 回復処理
        {
            std::ifstream   aStream(filename, std::ios_base::binary);
            theolizer::FastISerializer<> fis(aStream);
            loadBasic(fis);
        }
    }

    std::cout << "\n";
}

// ***************************************************************************
//      GlobalVersionNoを振って呼ばれ、CheckModeを振る
//          iGlobalVersionNo==0の時は、デフォルト・パラメータで呼び出す。
// ***************************************************************************

void varyGlobalVersionNo
(
    bool        iLoadOnly,
    unsigned    iGlobalVersionNo
)
{
    if (iGlobalVersionNo == 0)
    {
        varyCheckMode(iLoadOnly, iGlobalVersionNo, theolizer::CheckMode::NoTypeCheck);
    }
    else
    {
        varyCheckMode(iLoadOnly, iGlobalVersionNo, theolizer::CheckMode::NoTypeCheck);
        varyCheckMode(iLoadOnly, iGlobalVersionNo, theolizer::CheckMode::TypeCheck);
        varyCheckMode(iLoadOnly, iGlobalVersionNo, theolizer::CheckMode::TypeCheckByIndex);
    }
}

//############################################################################
//      メイン
//############################################################################

int main(int argc, char* argv[])
{
    theolizer::initResult();

// ***************************************************************************
//      バラメータ解析
// ***************************************************************************

    unsigned    aGlobalVersionNo=0;

    try
    {
        if (1 < argc)
        {
            unsigned long no=std::stoul(argv[1]);
            if (std::numeric_limits<unsigned>::max() < no)
    throw std::invalid_argument("GlobalVersionNo is too big.");
        }
    }
    catch(std::invalid_argument& e)
    {
        std::cerr << e.what() << "\n";

        theolizer::incrementFailCount();
        theolizer::printResult();
return 1;
    }

    std::cout << theolizer::print("aGlobalVersionNo:%d\n", aGlobalVersionNo);

// ***************************************************************************
//      使い方説明サンプル呼び出し
// ***************************************************************************

    // デフォルト起動時のみ呼び出す
    if (aGlobalVersionNo == 0)
    {
        tutoriseClassVariation();

        // ここまでの中間結果表示
        theolizer::printResult("------------- Result of tutorial samples ----------");
        std::cerr << "\n\n";
    }

// ***************************************************************************
//      GlobalVersionNoとCheckModeを振って処理を呼び出す
// ***************************************************************************

    try
    {
        if (aGlobalVersionNo)
        {
            varyGlobalVersionNo(true, aGlobalVersionNo);
        }
        else
        {
            for (unsigned aVersionNo=0; aVersionNo <= 1; ++aVersionNo)
            {
                varyGlobalVersionNo(false, aVersionNo);
            }
        }
    }
    catch(theolizer::ErrorInfo& e)
    {
        std::cerr << e.getString() << "\n";

        theolizer::incrementFailCount();
        theolizer::printResult();
return 2;
    }

    // Failが0でないならエラー終了
    if (!theolizer::printResult())
return 3;

    return 0;
}

void dummy()
{
}
