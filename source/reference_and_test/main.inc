//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          全バージョン共通ルーチン
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

using theolizer::CheckMode;

//############################################################################
//      派生Serializerテスト
//############################################################################

// ***************************************************************************
//      処理中データのバージョン・テーブル・インデックス
// ***************************************************************************

std::size_t gMyIndex;           // 自分自身のバージョン・インデックス
std::size_t gIndex;             // データのバージョン・インデックス(指定値)
std::size_t gDataIndex;         // データのバージョン・インデックス(実値)
std::size_t gProgramIndex;      // 保存したプログラムのバージョン・インデックス

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
//      テスト基本部を呼ぶ
// ***************************************************************************

void callTests
(
    bool                    iDoSave,
    theolizer::CheckMode    iCheckMode
)
{
    std::cout << "---------- callTests("
              << ((iDoSave)?"save":"load") << ", ";
    std::cout << "gMyIndex=" << gVersionList[gMyIndex].mName << ", gIndex=";
    gDataIndex=gIndex;
    if (gIndex == kDefaultIndex)
    {
        std::cout << "<Default>, ";
        gDataIndex=gProgramIndex;
    }
    else
    {
        std::cout << gVersionList[gIndex].mName << ", ";
    }
    std::cout << "gDataIndex=" << gVersionList[gDataIndex].mName << ", ";
    std::cout << "gProgramIndex=" << gVersionList[gProgramIndex].mName << ", "
              << iCheckMode << ");" << std::endl;

//----------------------------------------------------------------------------
//      バージョン名生成
//----------------------------------------------------------------------------

    std::string aVersionName;
    unsigned    aGlobalVersionNo=0;

    // デフォルト処理
    if (gIndex == kDefaultIndex)
    {
        aVersionName=gVersionList[gProgramIndex].mName;
    }

    // バージョン指定
    else
    {
        aVersionName=gVersionList[gIndex].mName+"-"+gVersionList[gProgramIndex].mName;
        aGlobalVersionNo=gVersionList[gIndex].mVersionNo;

        // 変更テスト対象外の時は0なので、初期バージョンの1を設定する
        if (aGlobalVersionNo == 0)
        {
            aGlobalVersionNo=1;
        }
    }

//----------------------------------------------------------------------------
//      XmlSerializer
//----------------------------------------------------------------------------

    std::string prefix;

#ifdef ENABLE_XML_TEST
    prefix="xml-";

    // デフォルト処理
    if (gIndex == kDefaultIndex)
    {
        std::string filename = prefix + aVersionName + ".xml";
        std::cout << "  XmlSerializer(" << filename << ")\n";

        // 保存処理
        if (iDoSave)
        {
            {
                std::ofstream   aStream(filename);
                theolizer::XmlOSerializer<> xos(aStream);
                saveBasic(xos);
            }

            // 保存先指定テスト
            {
                std::ofstream   aStreamA(makeDestFileName(filename, "A"));
                theolizer::XmlOSerializer<theolizerD::DestA> xosA(aStreamA);
                std::ofstream   aStreamB(makeDestFileName(filename, "B"));
                theolizer::XmlOSerializer<theolizerD::DestB> xosB(aStreamB);
                std::ofstream   aStreamAB(makeDestFileName(filename, "AB"));
                theolizer::XmlOSerializer<theolizerD::DestA, theolizerD::DestB> xosAB(aStreamAB);
                callSaveDestinations(xosA, xosB, xosAB);
            }
        }

        // 回復処理
        else
        {
            {
                std::ifstream   aStream(filename);
                theolizer::XmlISerializer<> xis(aStream);
                loadBasic(xis);
            }

            // 保存先指定テスト
            {
                std::ifstream   aStreamA(makeDestFileName(filename, "A"));
                theolizer::XmlISerializer<theolizerD::DestA> xisA(aStreamA);
                std::ifstream   aStreamB(makeDestFileName(filename, "B"));
                theolizer::XmlISerializer<theolizerD::DestB> xisB(aStreamB);
                std::ifstream   aStreamAB(makeDestFileName(filename, "AB"));
                theolizer::XmlISerializer<theolizerD::DestA, theolizerD::DestB> xisAB(aStreamAB);
                callLoadDestinations(xisA, xisB, xisAB);
            }
        }
    }

    // パラメータ指定
    else if (iCheckMode == CheckMode::NoTypeCheck)
    {
        std::stringstream   ss;
        std::string         filename;

//      ---<<< PretyPrint >>>---

        ss << prefix << "pp-" << aVersionName << "-" << iCheckMode << ".xml";
        filename=ss.str();
        std::cout << "  XmlSerializer(" << filename << ")\n";

        // 保存処理
        if (iDoSave)
        {
            {
                std::ofstream   aStream(filename);
                theolizer::XmlOSerializer<> xos(aStream, aGlobalVersionNo);
                saveBasic(xos);
            }

            // 保存先指定テスト
            {
                std::ofstream   aStreamA(makeDestFileName(filename, "A"));
                theolizer::XmlOSerializer<theolizerD::DestA>
                    xosA(aStreamA, aGlobalVersionNo);
                std::ofstream   aStreamB(makeDestFileName(filename, "B"));
                theolizer::XmlOSerializer<theolizerD::DestB>
                    xosB(aStreamB, aGlobalVersionNo);
                std::ofstream   aStreamAB(makeDestFileName(filename, "AB"));
                theolizer::XmlOSerializer<theolizerD::DestA, theolizerD::DestB>
                    xosAB(aStreamAB, aGlobalVersionNo);
                callSaveDestinations(xosA, xosB, xosAB);
            }
        }

        // 回復処理
        else
        {
            std::ifstream   aStream(filename);
            theolizer::XmlISerializer<> xis(aStream);
            loadBasic(xis);

            // 保存先指定テスト
            {
                std::ifstream   aStreamA(makeDestFileName(filename, "A"));
                theolizer::XmlISerializer<theolizerD::DestA> xisA(aStreamA);
                std::ifstream   aStreamB(makeDestFileName(filename, "B"));
                theolizer::XmlISerializer<theolizerD::DestB> xisB(aStreamB);
                std::ifstream   aStreamAB(makeDestFileName(filename, "AB"));
                theolizer::XmlISerializer<theolizerD::DestA, theolizerD::DestB> xisAB(aStreamAB);
                callLoadDestinations(xisA, xisB, xisAB);
            }
        }

//      ---<<< 非PretyPrint >>>---

        ss.str("");
        ss << prefix << "np-" << aVersionName << "-" << iCheckMode << ".xml";
        filename=ss.str();
        std::cout << "  XmlSerializer(" << filename << ")\n";

        // 保存処理
        if (iDoSave)
        {
            {
                std::ofstream   aStream(filename);
                theolizer::XmlOSerializer<> xos(aStream, aGlobalVersionNo, true);
                saveBasic(xos);
            }

            // 保存先指定テスト
            {
                std::ofstream   aStreamA(makeDestFileName(filename, "A"));
                theolizer::XmlOSerializer<theolizerD::DestA>
                    xosA(aStreamA, aGlobalVersionNo, true);
                std::ofstream   aStreamB(makeDestFileName(filename, "B"));
                theolizer::XmlOSerializer<theolizerD::DestB>
                    xosB(aStreamB, aGlobalVersionNo, true);
                std::ofstream   aStreamAB(makeDestFileName(filename, "AB"));
                theolizer::XmlOSerializer<theolizerD::DestA, theolizerD::DestB>
                    xosAB(aStreamAB, aGlobalVersionNo, true);
                callSaveDestinations(xosA, xosB, xosAB);
            }
        }

        // 回復処理
        else
        {
            std::ifstream   aStream(filename);
            theolizer::XmlISerializer<> xis(aStream);
            loadBasic(xis);

            // 保存先指定テスト
            {
                std::ifstream   aStreamA(makeDestFileName(filename, "A"));
                theolizer::XmlISerializer<theolizerD::DestA> xisA(aStreamA);
                std::ifstream   aStreamB(makeDestFileName(filename, "B"));
                theolizer::XmlISerializer<theolizerD::DestB> xisB(aStreamB);
                std::ifstream   aStreamAB(makeDestFileName(filename, "AB"));
                theolizer::XmlISerializer<theolizerD::DestA, theolizerD::DestB> xisAB(aStreamAB);
                callLoadDestinations(xisA, xisB, xisAB);
            }
        }
    }
#endif  // ENABLE_XML_TEST

//----------------------------------------------------------------------------
//      JsonSerializer
//----------------------------------------------------------------------------

    prefix="json-";

    // デフォルト処理
    if (gIndex == kDefaultIndex)
    {
        std::string filename = prefix + aVersionName + ".json";
        std::cout << "  JsonSerializer(" << filename << ")\n";

        // 保存処理
        if (iDoSave)
        {
            {
                std::ofstream   aStream(filename);
                theolizer::JsonOSerializer<> jos(aStream);
                saveBasic(jos);
            }

            // 保存先指定テスト
            {
                std::ofstream   aStreamA(makeDestFileName(filename, "A"));
                theolizer::JsonOSerializer<theolizerD::DestA> josA(aStreamA);
                std::ofstream   aStreamB(makeDestFileName(filename, "B"));
                theolizer::JsonOSerializer<theolizerD::DestB> josB(aStreamB);
                std::ofstream   aStreamAB(makeDestFileName(filename, "AB"));
                theolizer::JsonOSerializer<theolizerD::DestA, theolizerD::DestB> josAB(aStreamAB);
                callSaveDestinations(josA, josB, josAB);
            }
        }

        // 回復処理
        else
        {
            {
                std::ifstream   aStream(filename);
                theolizer::JsonISerializer<> jis(aStream);
                loadBasic(jis);
            }

            // 保存先指定テスト
            {
                std::ifstream   aStreamA(makeDestFileName(filename, "A"));
                theolizer::JsonISerializer<theolizerD::DestA> jisA(aStreamA);
                std::ifstream   aStreamB(makeDestFileName(filename, "B"));
                theolizer::JsonISerializer<theolizerD::DestB> jisB(aStreamB);
                std::ifstream   aStreamAB(makeDestFileName(filename, "AB"));
                theolizer::JsonISerializer<theolizerD::DestA, theolizerD::DestB> jisAB(aStreamAB);
                callLoadDestinations(jisA, jisB, jisAB);
            }
        }
    }

    // パラメータ指定
    else
    {
        std::stringstream   ss;
        std::string         filename;

//      ---<<< PretyPrint >>>---

        ss << prefix << "pp-" << aVersionName << "-" << iCheckMode << ".json";
        filename=ss.str();
        std::cout << "  JsonSerializer(" << filename << ")\n";

        // 保存処理
        if (iDoSave)
        {
            {
                std::ofstream   aStream(filename);
                theolizer::JsonOSerializer<> jos(aStream, aGlobalVersionNo, iCheckMode);
                saveBasic(jos);
            }

            // 保存先指定テスト
            {
                std::ofstream   aStreamA(makeDestFileName(filename, "A"));
                theolizer::JsonOSerializer<theolizerD::DestA>
                    josA(aStreamA, aGlobalVersionNo, iCheckMode);
                std::ofstream   aStreamB(makeDestFileName(filename, "B"));
                theolizer::JsonOSerializer<theolizerD::DestB>
                    josB(aStreamB, aGlobalVersionNo, iCheckMode);
                std::ofstream   aStreamAB(makeDestFileName(filename, "AB"));
                theolizer::JsonOSerializer<theolizerD::DestA, theolizerD::DestB>
                    josAB(aStreamAB, aGlobalVersionNo, iCheckMode);
                callSaveDestinations(josA, josB, josAB);
            }
        }

        // 回復処理
        else
        {
            std::ifstream   aStream(filename);
            theolizer::JsonISerializer<> jis(aStream);
            loadBasic(jis);

            // 保存先指定テスト
            {
                std::ifstream   aStreamA(makeDestFileName(filename, "A"));
                theolizer::JsonISerializer<theolizerD::DestA> jisA(aStreamA);
                std::ifstream   aStreamB(makeDestFileName(filename, "B"));
                theolizer::JsonISerializer<theolizerD::DestB> jisB(aStreamB);
                std::ifstream   aStreamAB(makeDestFileName(filename, "AB"));
                theolizer::JsonISerializer<theolizerD::DestA, theolizerD::DestB> jisAB(aStreamAB);
                callLoadDestinations(jisA, jisB, jisAB);
            }
        }

//      ---<<< 非PretyPrint >>>---

        ss.str("");
        ss << prefix << "np-" << aVersionName << "-" << iCheckMode << ".json";
        filename=ss.str();
        std::cout << "  JsonSerializer(" << filename << ")\n";

        // 保存処理
        if (iDoSave)
        {
            {
                std::ofstream   aStream(filename);
                theolizer::JsonOSerializer<> jos(aStream, aGlobalVersionNo, iCheckMode, true);
                saveBasic(jos);
            }

            // 保存先指定テスト
            {
                std::ofstream   aStreamA(makeDestFileName(filename, "A"));
                theolizer::JsonOSerializer<theolizerD::DestA>
                    josA(aStreamA, aGlobalVersionNo, iCheckMode, true);
                std::ofstream   aStreamB(makeDestFileName(filename, "B"));
                theolizer::JsonOSerializer<theolizerD::DestB>
                    josB(aStreamB, aGlobalVersionNo, iCheckMode, true);
                std::ofstream   aStreamAB(makeDestFileName(filename, "AB"));
                theolizer::JsonOSerializer<theolizerD::DestA, theolizerD::DestB>
                    josAB(aStreamAB, aGlobalVersionNo, iCheckMode, true);
                callSaveDestinations(josA, josB, josAB);
            }
        }

        // 回復処理
        else
        {
            std::ifstream   aStream(filename);
            theolizer::JsonISerializer<> jis(aStream);
            loadBasic(jis);

            // 保存先指定テスト
            {
                std::ifstream   aStreamA(makeDestFileName(filename, "A"));
                theolizer::JsonISerializer<theolizerD::DestA> jisA(aStreamA);
                std::ifstream   aStreamB(makeDestFileName(filename, "B"));
                theolizer::JsonISerializer<theolizerD::DestB> jisB(aStreamB);
                std::ifstream   aStreamAB(makeDestFileName(filename, "AB"));
                theolizer::JsonISerializer<theolizerD::DestA, theolizerD::DestB> jisAB(aStreamAB);
                callLoadDestinations(jisA, jisB, jisAB);
            }
        }
    }

//----------------------------------------------------------------------------
//      BinarySerializer
//----------------------------------------------------------------------------

#ifdef ENABLE_XML_TEST
    prefix="binary-";

    // デフォルト処理
    if (gIndex == kDefaultIndex)
    {
        std::string filename = prefix + aVersionName + ".bin";
        std::cout << "  BinarySerializer(" << filename << ")\n";

        // 保存処理
        if (iDoSave)
        {
            {
                std::ofstream   aStream(filename, std::ios_base::binary);
                theolizer::BinaryOSerializer<> bos(aStream);
                saveBasic(bos);
            }

            // 保存先指定テスト
            {
                std::ofstream   aStreamA(makeDestFileName(filename, "A"), std::ios_base::binary);
                theolizer::BinaryOSerializer<theolizerD::DestA> bosA(aStreamA);
                std::ofstream   aStreamB(makeDestFileName(filename, "B"), std::ios_base::binary);
                theolizer::BinaryOSerializer<theolizerD::DestB> bosB(aStreamB);
                std::ofstream   aStreamAB(makeDestFileName(filename, "AB"), std::ios_base::binary);
                theolizer::BinaryOSerializer<theolizerD::DestA, theolizerD::DestB>
                    bosAB(aStreamAB);
                callSaveDestinations(bosA, bosB, bosAB);
            }
        }

        // 回復処理
        else
        {
            {
                std::ifstream   aStream(filename, std::ios_base::binary);
                theolizer::BinaryISerializer<> bis(aStream);
                loadBasic(bis);
            }

            // 保存先指定テスト
            {
                std::ifstream   aStreamA(makeDestFileName(filename, "A"), std::ios_base::binary);
                theolizer::BinaryISerializer<theolizerD::DestA> bisA(aStreamA);
                std::ifstream   aStreamB(makeDestFileName(filename, "B"), std::ios_base::binary);
                theolizer::BinaryISerializer<theolizerD::DestB> bisB(aStreamB);
                std::ifstream   aStreamAB(makeDestFileName(filename, "AB"), std::ios_base::binary);
                theolizer::BinaryISerializer<theolizerD::DestA, theolizerD::DestB>
                    bisAB(aStreamAB);
                callLoadDestinations(bisA, bisB, bisAB);
            }
        }
    }

    // パラメータ指定
    else
    {
        std::stringstream   ss;
        std::string         filename;

        ss << prefix << aVersionName << "-"<< iCheckMode << ".bin";
        filename=ss.str();
        std::cout << "  BinaryOSerializer(" << filename << ")\n";

        // 保存処理
        if (iDoSave)
        {
            {
                std::ofstream   aStream(filename, std::ios_base::binary);
                theolizer::BinaryOSerializer<> bos(aStream, aGlobalVersionNo, iCheckMode);
                saveBasic(bos);
            }

            // 保存先指定テスト
            {
                std::ofstream   aStreamA(makeDestFileName(filename, "A"), std::ios_base::binary);
                theolizer::BinaryOSerializer<theolizerD::DestA>
                    bosA(aStreamA, aGlobalVersionNo, iCheckMode);
                std::ofstream   aStreamB(makeDestFileName(filename, "B"), std::ios_base::binary);
                theolizer::BinaryOSerializer<theolizerD::DestB>
                    bosB(aStreamB, aGlobalVersionNo, iCheckMode);
                std::ofstream   aStreamAB(makeDestFileName(filename, "AB"), std::ios_base::binary);
                theolizer::BinaryOSerializer<theolizerD::DestA, theolizerD::DestB>
                    bosAB(aStreamAB, aGlobalVersionNo, iCheckMode);
                callSaveDestinations(bosA, bosB, bosAB);
            }
        }

        // 回復処理
        else
        {
            std::ifstream   aStream(filename, std::ios_base::binary);
            theolizer::BinaryISerializer<> bis(aStream);
            loadBasic(bis);

            // 保存先指定テスト
            {
                std::ifstream   aStreamA(makeDestFileName(filename, "A"), std::ios_base::binary);
                theolizer::BinaryISerializer<theolizerD::DestA> bisA(aStreamA);
                std::ifstream   aStreamB(makeDestFileName(filename, "B"), std::ios_base::binary);
                theolizer::BinaryISerializer<theolizerD::DestB> bisB(aStreamB);
                std::ifstream   aStreamAB(makeDestFileName(filename, "AB"), std::ios_base::binary);
                theolizer::BinaryISerializer<theolizerD::DestA, theolizerD::DestB>
                    bisAB(aStreamAB);
                callLoadDestinations(bisA, bisB, bisAB);
            }
        }
    }

//----------------------------------------------------------------------------
//      FastSerializer
//----------------------------------------------------------------------------

    prefix="fast-";

    // 全く同じ者同士のデフォルトのみサポート
    if ((gMyIndex == gProgramIndex) && (gIndex == kDefaultIndex))
    {
        std::string filename = prefix + aVersionName + ".bin";
        std::cout << "  FastSerializer(" << filename << ")\n";

        // 保存処理
        if (iDoSave)
        {
            {
                std::ofstream   aStream(filename, std::ios_base::binary);
                theolizer::FastOSerializer<> fos(aStream);
                saveBasic(fos);
            }

            // 保存先指定テスト
            {
                std::ofstream   aStreamA(makeDestFileName(filename, "A"), std::ios_base::binary);
                theolizer::FastOSerializer<theolizerD::DestA> fosA(aStreamA);
                std::ofstream   aStreamB(makeDestFileName(filename, "B"), std::ios_base::binary);
                theolizer::FastOSerializer<theolizerD::DestB> fosB(aStreamB);
                std::ofstream   aStreamAB(makeDestFileName(filename, "AB"), std::ios_base::binary);
                theolizer::FastOSerializer<theolizerD::DestA, theolizerD::DestB> fosAB(aStreamAB);
                callSaveDestinations(fosA, fosB, fosAB);
            }
        }

        // 回復処理
        else
        {
            {
                std::ifstream   aStream(filename, std::ios_base::binary);
                theolizer::FastISerializer<> fis(aStream);
                loadBasic(fis);
            }

            // 保存先指定テスト
            {
                std::ifstream   aStreamA(makeDestFileName(filename, "A"), std::ios_base::binary);
                theolizer::FastISerializer<theolizerD::DestA> fisA(aStreamA);
                std::ifstream   aStreamB(makeDestFileName(filename, "B"), std::ios_base::binary);
                theolizer::FastISerializer<theolizerD::DestB> fisB(aStreamB);
                std::ifstream   aStreamAB(makeDestFileName(filename, "AB"), std::ios_base::binary);
                theolizer::FastISerializer<theolizerD::DestA, theolizerD::DestB>
                    fisAB(aStreamAB);
                callLoadDestinations(fisA, fisB, fisAB);
            }
        }
    }
#endif  // ENABLE_XML_TEST

    std::cout << "\n";
}

//############################################################################
//      メイン
//############################################################################

int main(int argc, char* argv[])
{
    theolizer::initResult();

    for (std::size_t i=0; i < std::extent<decltype(gVersionList)>::value; ++i)
    {
        std::cout << "gVersionList[" << i << "]="
            << gVersionList[i].mName << ", "
            << gVersionList[i].mVersionNo << "\n";
    }

// ***************************************************************************
//      自身のインデックス番号獲得
//          ファイル名を使って検出
//          一致しない時はエラー
// ***************************************************************************

    std::string aMyVersionName=argv[0];
    std::size_t pos=aMyVersionName.rfind('_');
    if (pos == std::string::npos)
    {
        std::cerr << "Exe name error.(" << aMyVersionName << ")\n";
return 2;
    }
    ++pos;
    std::size_t len=aMyVersionName.rfind('.')-pos;
    aMyVersionName=aMyVersionName.substr(pos, len);

    gMyIndex=kDefaultIndex;
    for (std::size_t i=0; i < std::extent<decltype(gVersionList)>::value; ++i)
    {
        if (gVersionList[i].mName == aMyVersionName)
        {
            gMyIndex=i;
        }
    }
    if (gMyIndex == kDefaultIndex)
    {
        std::cerr << "Not found VersionName.(" << aMyVersionName << ")\n";
return 2;
    }

// ***************************************************************************
//      バラメータ解析
// ***************************************************************************

    bool        aDoSave=true;

    if (1 < argc)
    {
        aDoSave=false;
    }

    std::cout << theolizer::print("VersionName:%s MyIndex:%u aDoSave:%u\n",
        aMyVersionName, gMyIndex, aDoSave);

// ***************************************************************************
//      IndexとCheckModeを振って処理を呼び出す
// ***************************************************************************

    try
    {
        // 使い方解説用ルーチン呼び出し
        if (!aDoSave)
        {
            callTutorial();
        }

        // 保存時は同じグローバルバージョン番号の最新版のみ保存する
        if (aDoSave)
        {
            // デフォルト出力
            gIndex=kDefaultIndex;
            gProgramIndex=gMyIndex;
            callTests(aDoSave, CheckMode::NoTypeCheck);

            // バラメータ指定出力
            for (gIndex=0; gIndex <= gMyIndex; ++gIndex)
            {
                // 自身未満の時は、
                //  変更テストしないならcontinue
                //  次のグローバル・バージョン番号が同じならcontinue
                if (gIndex < gMyIndex)
                {
                    if (gVersionList[gIndex].mVersionNo == 0)
        continue;
                    if (gVersionList[gIndex].mVersionNo == 
                        gVersionList[gIndex+1].mVersionNo)
        continue;
                }

                callTests(aDoSave, CheckMode::NoTypeCheck);
                callTests(aDoSave, CheckMode::TypeCheckByIndex);
                callTests(aDoSave, CheckMode::MetaMode);
            }
        }

        // 回復時はプログラムIndexも振る
        else
        {
            // 各プログラムのデフォルト出力データから回復する
            gIndex=kDefaultIndex;
            for (gProgramIndex=0; gProgramIndex <= gMyIndex; ++gProgramIndex)
            {
                // 非変更／変更が一致していない時はcontinue
                if ((gVersionList[gMyIndex].mVersionNo == 0)
                 != (gVersionList[gProgramIndex].mVersionNo == 0))
            continue;

                // 非変更の時は他のプログラムIndexのデータを回復しない
                if ((gVersionList[gMyIndex].mVersionNo == 0)
                 && (gMyIndex != gProgramIndex))
            continue;

                callTests(aDoSave, CheckMode::NoTypeCheck);
            }

            // 各プログラムのパラメータ指定出力データから回復する
            for (gIndex=0; gIndex <= gMyIndex; ++gIndex)
            {
std::cout << "gMyIndex=" << gMyIndex
          << " gIndex=" << gIndex << "\n";

                for (gProgramIndex=gIndex;
                     gProgramIndex < std::extent<decltype(gVersionList)>::value;
                     ++gProgramIndex)
                {
std::cout << "    gProgramIndex=" << gProgramIndex << "\n";

                    // 非変更／変更が一致していない時はcontinue
                    if (((gVersionList[gMyIndex].mVersionNo == 0)
                      != (gVersionList[gIndex].mVersionNo == 0))
                     || ((gVersionList[gProgramIndex].mVersionNo == 0)
                      != (gVersionList[gIndex].mVersionNo == 0)))
            continue;

                    // 非変更の時は他のプログラムIndexのデータを回復しない
                    if ((gVersionList[gMyIndex].mVersionNo == 0)
                     && (gMyIndex != gProgramIndex))
            continue;

                    // 対象プログラム未満の時は、次のグローバル・バージョン番号が同じならcontinue
                    if (gIndex < gProgramIndex)
                    {
                        if (gVersionList[gIndex].mVersionNo == 
                            gVersionList[gIndex+1].mVersionNo)
        continue;
                    }

                    callTests(aDoSave, CheckMode::NoTypeCheck);
                    callTests(aDoSave, CheckMode::TypeCheckByIndex);
                    callTests(aDoSave, CheckMode::MetaMode);
                }
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
