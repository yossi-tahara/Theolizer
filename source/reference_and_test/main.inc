//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          全バージョン共通ルーチン
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

//############################################################################
//      定数等
//############################################################################

// デフォルト指定用インデックス番号
const std::size_t   kDefaultIndex=std::numeric_limits<std::size_t>::max();

using theolizer::CheckMode;

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
//      テスト基本部を呼ぶ
// ***************************************************************************

void callTests
(
    bool                    iDoSave,
    std::size_t             iIndex,
    std::size_t             iProgramIndex,
    theolizer::CheckMode    iCheckMode
)
{
    std::cout << "---------- callTests("
              << ((iDoSave)?"save":"load") << ", ";
    if (iIndex == kDefaultIndex)
    {
        std::cout << "<Default>, ";
    }
    else
    {
        std::cout << iIndex << ", ";
    }
    std::cout << iProgramIndex << ", "
              << iCheckMode << ");" << std::endl;

//----------------------------------------------------------------------------
//      バージョン名生成
//----------------------------------------------------------------------------

    std::string aVersionName;
    unsigned    aGlobalVersionNo=0;

    // デフォルト処理
    if (iIndex == kDefaultIndex)
    {
        aVersionName=gVersionList[iProgramIndex].mName;
    }

    // バージョン指定
    else
    {
        aVersionName=gVersionList[iIndex].mName+"-"+gVersionList[iProgramIndex].mName;
        aGlobalVersionNo=gVersionList[iIndex].mVersionNo;

        // 変更テスト対象外の時は0なので、初期バージョンの1を設定する
        if (aGlobalVersionNo == 0)
        {
            aGlobalVersionNo=1;
        }
    }

//----------------------------------------------------------------------------
//      JsonSerializer
//----------------------------------------------------------------------------

    std::string prefix;
    prefix="json-";

    // デフォルト処理
    if (iIndex == kDefaultIndex)
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

    prefix="binary-";

    // デフォルト処理
    if (iIndex == kDefaultIndex)
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

    // デフォルト処理
    if (iIndex == kDefaultIndex)
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

    std::size_t aMyIndex=kDefaultIndex;
    for (std::size_t i=0; i < std::extent<decltype(gVersionList)>::value; ++i)
    {
        if (gVersionList[i].mName == aMyVersionName)
        {
            aMyIndex=i;
        }
    }
    if (aMyIndex == kDefaultIndex)
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
        aMyVersionName, aMyIndex, aDoSave);

// ***************************************************************************
//      IndexとCheckModeを振って処理を呼び出す
// ***************************************************************************

    try
    {
        // 使い方解説用ルーチン呼び出し
        if (aDoSave)
        {
            callTutorial();
        }

        // デフォルト呼び出し
        callTests(aDoSave, kDefaultIndex, aMyIndex, CheckMode::NoTypeCheck);

        // バラメータ指定呼び出し
        // 保存時は同じグローバルバージョン番号の最新版のみ保存する
        if (aDoSave)
        {
            for (std::size_t aIndex=0; aIndex <= aMyIndex; ++aIndex)
            {
                // 自身未満の時は、
                //  変更テストしないならcontinue
                //  次のグローバル・バージョン番号が同じならcontinue
                if (aIndex < aMyIndex)
                {
                    if (gVersionList[aIndex].mVersionNo == 0)
        continue;
                    if (gVersionList[aIndex].mVersionNo == 
                        gVersionList[aIndex+1].mVersionNo)
        continue;
                }

                callTests(aDoSave, aIndex, aMyIndex, CheckMode::NoTypeCheck);
                callTests(aDoSave, aIndex, aMyIndex, CheckMode::TypeCheck);
                callTests(aDoSave, aIndex, aMyIndex, CheckMode::TypeCheckByIndex);
            }
        }

        // 回復時はプログラムIndexも振る
        else
        {
            for (std::size_t aIndex=0; aIndex <= aMyIndex; ++aIndex)
            {
                // 自身未満の時は、
                //  変更テストしないならcontinue
                if (aIndex < aMyIndex)
                {
                    if (gVersionList[aIndex].mVersionNo == 0)
        continue;
                }

                for (std::size_t aProgramIndex=aIndex; aProgramIndex <= aMyIndex; ++aProgramIndex)
                {
                    // 対象プログラム未満の時は、次のグローバル・バージョン番号が同じならcontinue
                    if (aIndex < aProgramIndex)
                    {
                        if (gVersionList[aIndex].mVersionNo == 
                            gVersionList[aIndex+1].mVersionNo)
        continue;
                    }

                    callTests(aDoSave, aIndex, aProgramIndex, CheckMode::NoTypeCheck);
                    callTests(aDoSave, aIndex, aProgramIndex, CheckMode::TypeCheck);
                    callTests(aDoSave, aIndex, aProgramIndex, CheckMode::TypeCheckByIndex);
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
