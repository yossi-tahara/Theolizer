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
#include "disable_test.h"
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

//----------------------------------------------------------------------------
//      enum型の様々なバリエーション
//----------------------------------------------------------------------------

//      ---<<< 使い方のサンプル・コード >>>---

void tutoriseEnumVariation();

//      ---<<< 自動テスト >>>---

template<class tSerializer>
void saveEnumVariation(tSerializer& iSerializer);

template<class tSerializer>
void loadEnumVariation(tSerializer& iSerializer);

//----------------------------------------------------------------------------
//      オブジェクト追跡
//----------------------------------------------------------------------------

//      ---<<< 使い方のサンプル・コード >>>---

void tutoriseObjectTracking();

//      ---<<< 自動テスト >>>---

template<class tSerializer>
void saveObjectTracking(tSerializer& iSerializer);

template<class tSerializer>
void loadObjectTracking(tSerializer& iSerializer);

template<class tSerializer>
void saveObjectTracking3(tSerializer& iSerializer);

template<class tSerializer>
void loadObjectTracking3(tSerializer& iSerializer);

//----------------------------------------------------------------------------
//      ポリモーフィズム
//----------------------------------------------------------------------------

//      ---<<< 使い方のサンプル・コード >>>---

void tutorisePolymorphism();

//      ---<<< 自動テスト >>>---

template<class tSerializer>
void savePolymorphism(tSerializer& iSerializer);

template<class tSerializer>
void loadPolymorphism(tSerializer& iSerializer);

//----------------------------------------------------------------------------
//      保存の有無と保存先
//----------------------------------------------------------------------------

#ifndef DISABLE_DESTINATIONS_TEST

//      ---<<< 使い方のサンプル・コード >>>---

void tutoriseDestinations();

//      ---<<< 自動テスト >>>---

template<class tSerializer>
void saveSpecifySaving(tSerializer& iSerializer);

template<class tSerializer>
void loadSpecifySaving(tSerializer& iSerializer);

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void saveDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB& iSerializerAB
);

template<class tSerializerA, class tSerializerB, class tSerializerAB>
void loadDestinations
(
    tSerializerA&  iSerializerA,
    tSerializerB&  iSerializerB,
    tSerializerAB& iSerializerAB
);
#endif // DISABLE_DESTINATIONS_TEST

// ***************************************************************************
//      各テスト呼び出し
// ***************************************************************************

template<class tSerializer>
void saveBasic(tSerializer& iSerializer)
{
#ifndef DISABLE_BASIC_PROCESS_TEST
    saveBasicProcess(iSerializer);
#endif
#ifndef DISABLE_CLASS_VARIATION_TEST
    saveClassVariation(iSerializer);
#endif
#ifndef DISABLE_ENUM_VARIATION_TEST
    saveEnumVariation(iSerializer);
#endif
#ifndef DISABLE_OBJECT_TRACKING2_TEST
    saveObjectTracking(iSerializer);
#endif
#ifndef DISABLE_OBJECT_TRACKING3_TEST
    saveObjectTracking3(iSerializer);
#endif
#ifndef DISABLE_POLYMORPHISM_TEST
    savePolymorphism(iSerializer);
#endif
#ifndef DISABLE_DESTINATIONS_TEST
    saveSpecifySaving(iSerializer);
#endif
}

template<class tSerializer>
void loadBasic(tSerializer& iSerializer)
{
#ifndef DISABLE_BASIC_PROCESS_TEST
    loadBasicProcess(iSerializer);
#endif
#ifndef DISABLE_CLASS_VARIATION_TEST
    loadClassVariation(iSerializer);
#endif
#ifndef DISABLE_ENUM_VARIATION_TEST
    loadEnumVariation(iSerializer);
#endif
#ifndef DISABLE_OBJECT_TRACKING2_TEST
    loadObjectTracking(iSerializer);
#endif
#ifndef DISABLE_OBJECT_TRACKING3_TEST
    loadObjectTracking3(iSerializer);
#endif
#ifndef DISABLE_POLYMORPHISM_TEST
    loadPolymorphism(iSerializer);
#endif
#ifndef DISABLE_DESTINATIONS_TEST
    loadSpecifySaving(iSerializer);
#endif
}

//############################################################################
//      派生Serializerテスト
//############################################################################

// ***************************************************************************
//      保存先指定の網羅テスト用
//          コンストラクトに必要な処理はこちらで行う
// ***************************************************************************

//----------------------------------------------------------------------------
//      ファイル名生成
//          最後の.の直前に指定文字列を挿入する
//----------------------------------------------------------------------------

std::string makeDestFileName(std::string const& iFileName, char const* iDestName)
{
    std::string ret(iFileName);
    std::size_t pos=ret.rfind('.');
    if (pos != std::string::npos)
    {
        ret.insert(pos, iDestName);
    }
    return ret;
}

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
            {
                std::ofstream   aStream(filename);
                theolizer::JsonOSerializer<> jos(aStream);
                saveBasic(jos);
            }

#ifndef DISABLE_DESTINATIONS_TEST
            // 保存先指定テスト
            {
                std::ofstream   aStreamA(makeDestFileName(filename, "A"));
                theolizer::JsonOSerializer<theolizerD::DestA> josA(aStreamA);
                std::ofstream   aStreamB(makeDestFileName(filename, "B"));
                theolizer::JsonOSerializer<theolizerD::DestB> josB(aStreamB);
                std::ofstream   aStreamAB(makeDestFileName(filename, "AB"));
                theolizer::JsonOSerializer<theolizerD::DestA, theolizerD::DestB> josAB(aStreamAB);
                saveDestinations(josA, josB, josAB);
            }
#endif
        }

        // 回復処理
        {
            {
                std::ifstream   aStream(filename);
                theolizer::JsonISerializer<> jis(aStream);
                loadBasic(jis);
            }

#ifndef DISABLE_DESTINATIONS_TEST
            // 保存先指定テスト
            {
                std::ifstream   aStreamA(makeDestFileName(filename, "A"));
                theolizer::JsonISerializer<theolizerD::DestA> jisA(aStreamA);
                std::ifstream   aStreamB(makeDestFileName(filename, "B"));
                theolizer::JsonISerializer<theolizerD::DestB> jisB(aStreamB);
                std::ifstream   aStreamAB(makeDestFileName(filename, "AB"));
                theolizer::JsonISerializer<theolizerD::DestA, theolizerD::DestB> jisAB(aStreamAB);
                loadDestinations(jisA, jisB, jisAB);
            }
#endif
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
            {
                std::ofstream   aStream(filename);
                theolizer::JsonOSerializer<> jos(aStream, iGlobalVersionNo, iCheckMode);
                saveBasic(jos);
            }

#ifndef DISABLE_DESTINATIONS_TEST
            // 保存先指定テスト
            {
                std::ofstream   aStreamA(makeDestFileName(filename, "A"));
                theolizer::JsonOSerializer<theolizerD::DestA>
                    josA(aStreamA, iGlobalVersionNo, iCheckMode);
                std::ofstream   aStreamB(makeDestFileName(filename, "B"));
                theolizer::JsonOSerializer<theolizerD::DestB>
                    josB(aStreamB, iGlobalVersionNo, iCheckMode);
                std::ofstream   aStreamAB(makeDestFileName(filename, "AB"));
                theolizer::JsonOSerializer<theolizerD::DestA, theolizerD::DestB>
                    josAB(aStreamAB, iGlobalVersionNo, iCheckMode);
                saveDestinations(josA, josB, josAB);
            }
#endif
        }

        // 回復処理
        {
            std::ifstream   aStream(filename);
            theolizer::JsonISerializer<> jis(aStream);
            loadBasic(jis);

#ifndef DISABLE_DESTINATIONS_TEST
            // 保存先指定テスト
            {
                std::ifstream   aStreamA(makeDestFileName(filename, "A"));
                theolizer::JsonISerializer<theolizerD::DestA> jisA(aStreamA);
                std::ifstream   aStreamB(makeDestFileName(filename, "B"));
                theolizer::JsonISerializer<theolizerD::DestB> jisB(aStreamB);
                std::ifstream   aStreamAB(makeDestFileName(filename, "AB"));
                theolizer::JsonISerializer<theolizerD::DestA, theolizerD::DestB> jisAB(aStreamAB);
                loadDestinations(jisA, jisB, jisAB);
            }
#endif
        }

//      ---<<< 非PretyPrint >>>---

        ss.str("");
        ss << "json-np-ver" << iGlobalVersionNo << "-"<< iCheckMode << ".json";
        filename=ss.str();
        std::cout << "    JsonSerializer(" << filename << ")\n";

        // 保存処理
        if (!iLoadOnly)
        {
            {
                std::ofstream   aStream(filename);
                theolizer::JsonOSerializer<> jos(aStream, iGlobalVersionNo, iCheckMode, true);
                saveBasic(jos);
            }

#ifndef DISABLE_DESTINATIONS_TEST
            // 保存先指定テスト
            {
                std::ofstream   aStreamA(makeDestFileName(filename, "A"));
                theolizer::JsonOSerializer<theolizerD::DestA>
                    josA(aStreamA, iGlobalVersionNo, iCheckMode, true);
                std::ofstream   aStreamB(makeDestFileName(filename, "B"));
                theolizer::JsonOSerializer<theolizerD::DestB>
                    josB(aStreamB, iGlobalVersionNo, iCheckMode, true);
                std::ofstream   aStreamAB(makeDestFileName(filename, "AB"));
                theolizer::JsonOSerializer<theolizerD::DestA, theolizerD::DestB>
                    josAB(aStreamAB, iGlobalVersionNo, iCheckMode, true);
                saveDestinations(josA, josB, josAB);
            }
#endif
        }

        // 回復処理
        {
            std::ifstream   aStream(filename);
            theolizer::JsonISerializer<> jis(aStream);
            loadBasic(jis);

#ifndef DISABLE_DESTINATIONS_TEST
            // 保存先指定テスト
            {
                std::ifstream   aStreamA(makeDestFileName(filename, "A"));
                theolizer::JsonISerializer<theolizerD::DestA> jisA(aStreamA);
                std::ifstream   aStreamB(makeDestFileName(filename, "B"));
                theolizer::JsonISerializer<theolizerD::DestB> jisB(aStreamB);
                std::ifstream   aStreamAB(makeDestFileName(filename, "AB"));
                theolizer::JsonISerializer<theolizerD::DestA, theolizerD::DestB> jisAB(aStreamAB);
                loadDestinations(jisA, jisB, jisAB);
            }
#endif
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
            {
                std::ofstream   aStream(filename, std::ios_base::binary);
                theolizer::BinaryOSerializer<> bos(aStream);
                saveBasic(bos);
            }

#ifndef DISABLE_DESTINATIONS_TEST
            // 保存先指定テスト
            {
                std::ofstream   aStreamA(makeDestFileName(filename, "A"), std::ios_base::binary);
                theolizer::BinaryOSerializer<theolizerD::DestA> bosA(aStreamA);
                std::ofstream   aStreamB(makeDestFileName(filename, "B"), std::ios_base::binary);
                theolizer::BinaryOSerializer<theolizerD::DestB> bosB(aStreamB);
                std::ofstream   aStreamAB(makeDestFileName(filename, "AB"), std::ios_base::binary);
                theolizer::BinaryOSerializer<theolizerD::DestA, theolizerD::DestB>
                    bosAB(aStreamAB);
                saveDestinations(bosA, bosB, bosAB);
            }
#endif
        }

        // 回復処理
        {
            {
                std::ifstream   aStream(filename, std::ios_base::binary);
                theolizer::BinaryISerializer<> bis(aStream);
                loadBasic(bis);
            }

#ifndef DISABLE_DESTINATIONS_TEST
            // 保存先指定テスト
            {
                std::ifstream   aStreamA(makeDestFileName(filename, "A"), std::ios_base::binary);
                theolizer::BinaryISerializer<theolizerD::DestA> bisA(aStreamA);
                std::ifstream   aStreamB(makeDestFileName(filename, "B"), std::ios_base::binary);
                theolizer::BinaryISerializer<theolizerD::DestB> bisB(aStreamB);
                std::ifstream   aStreamAB(makeDestFileName(filename, "AB"), std::ios_base::binary);
                theolizer::BinaryISerializer<theolizerD::DestA, theolizerD::DestB>
                    bisAB(aStreamAB);
                loadDestinations(bisA, bisB, bisAB);
            }
#endif
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
            {
                std::ofstream   aStream(filename, std::ios_base::binary);
                theolizer::BinaryOSerializer<> bos(aStream, iGlobalVersionNo, iCheckMode);
                saveBasic(bos);
            }

#ifndef DISABLE_DESTINATIONS_TEST
            // 保存先指定テスト
            {
                std::ofstream   aStreamA(makeDestFileName(filename, "A"), std::ios_base::binary);
                theolizer::BinaryOSerializer<theolizerD::DestA>
                    bosA(aStreamA, iGlobalVersionNo, iCheckMode);
                std::ofstream   aStreamB(makeDestFileName(filename, "B"), std::ios_base::binary);
                theolizer::BinaryOSerializer<theolizerD::DestB>
                    bosB(aStreamB, iGlobalVersionNo, iCheckMode);
                std::ofstream   aStreamAB(makeDestFileName(filename, "AB"), std::ios_base::binary);
                theolizer::BinaryOSerializer<theolizerD::DestA, theolizerD::DestB>
                    bosAB(aStreamAB, iGlobalVersionNo, iCheckMode);
                saveDestinations(bosA, bosB, bosAB);
            }
#endif
        }

        // 回復処理
        {
            std::ifstream   aStream(filename, std::ios_base::binary);
            theolizer::BinaryISerializer<> bis(aStream);
            loadBasic(bis);

#ifndef DISABLE_DESTINATIONS_TEST
            // 保存先指定テスト
            {
                std::ifstream   aStreamA(makeDestFileName(filename, "A"), std::ios_base::binary);
                theolizer::BinaryISerializer<theolizerD::DestA> bisA(aStreamA);
                std::ifstream   aStreamB(makeDestFileName(filename, "B"), std::ios_base::binary);
                theolizer::BinaryISerializer<theolizerD::DestB> bisB(aStreamB);
                std::ifstream   aStreamAB(makeDestFileName(filename, "AB"), std::ios_base::binary);
                theolizer::BinaryISerializer<theolizerD::DestA, theolizerD::DestB>
                    bisAB(aStreamAB);
                loadDestinations(bisA, bisB, bisAB);
            }
#endif
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
            {
                std::ofstream   aStream(filename, std::ios_base::binary);
                theolizer::FastOSerializer<> fos(aStream);
                saveBasic(fos);
            }

#ifndef DISABLE_DESTINATIONS_TEST
            // 保存先指定テスト
            {
                std::ofstream   aStreamA(makeDestFileName(filename, "A"), std::ios_base::binary);
                theolizer::FastOSerializer<theolizerD::DestA> fosA(aStreamA);
                std::ofstream   aStreamB(makeDestFileName(filename, "B"), std::ios_base::binary);
                theolizer::FastOSerializer<theolizerD::DestB> fosB(aStreamB);
                std::ofstream   aStreamAB(makeDestFileName(filename, "AB"), std::ios_base::binary);
                theolizer::FastOSerializer<theolizerD::DestA, theolizerD::DestB> fosAB(aStreamAB);
                saveDestinations(fosA, fosB, fosAB);
            }
#endif
        }

        // 回復処理
        {
            {
                std::ifstream   aStream(filename, std::ios_base::binary);
                theolizer::FastISerializer<> fis(aStream);
                loadBasic(fis);
            }

#ifndef DISABLE_DESTINATIONS_TEST
            // 保存先指定テスト
            {
                std::ifstream   aStreamA(makeDestFileName(filename, "A"), std::ios_base::binary);
                theolizer::FastISerializer<theolizerD::DestA> fisA(aStreamA);
                std::ifstream   aStreamB(makeDestFileName(filename, "B"), std::ios_base::binary);
                theolizer::FastISerializer<theolizerD::DestB> fisB(aStreamB);
                std::ifstream   aStreamAB(makeDestFileName(filename, "AB"), std::ios_base::binary);
                theolizer::FastISerializer<theolizerD::DestA, theolizerD::DestB>
                    fisAB(aStreamAB);
                loadDestinations(fisA, fisB, fisAB);
            }
#endif
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

    try
    {
        // デフォルト起動時のみ呼び出す
        if (aGlobalVersionNo == 0)
        {
            #ifndef DISABLE_CLASS_VARIATION_TEST
                tutoriseClassVariation();
            #endif
            #ifndef DISABLE_ENUM_VARIATION_TEST
                tutoriseEnumVariation();
            #endif
            #ifndef DISABLE_OBJECT_TRACKING_TEST
                tutoriseObjectTracking();
            #endif
            #ifndef DISABLE_POLYMORPHISM_TEST
                tutorisePolymorphism();
            #endif
            #ifndef DISABLE_DESTINATIONS_TEST
                tutoriseDestinations();
            #endif

            // ここまでの中間結果表示
            theolizer::printResult("------------- Result of tutorial samples ----------");
            std::cerr << "\n\n";
        }
    }
    catch(theolizer::ErrorInfo& e)
    {
        std::cerr << e.getString() << "\n";

        theolizer::incrementFailCount();
        theolizer::printResult();
return 2;
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
return 3;
    }

    // Failが0でないならエラー終了
    if (!theolizer::printResult())
return 4;

    return 0;
}
