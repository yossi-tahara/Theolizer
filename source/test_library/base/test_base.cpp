//############################################################################
//      base.*のテスト
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

#if defined(_MSC_VER)   // disabling MSVC warnings
    #pragma warning(disable:4100 4127)
#endif

// ***************************************************************************
//      通常のインクルード
// ***************************************************************************

#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <unordered_set>
#include <vector>

#include <map>
#include <unordered_map>

#include <type_traits>
#include <iostream>
using namespace std;

#include <theolizer/rbfor.h>
#include <theolizer/test_tool.h>

// ***************************************************************************
//      範囲ベースfor拡張機能テスト用サブ
// ***************************************************************************

struct TestClass
{
    int         first;
    unsigned    second;

TestClass(int f, unsigned s) : first(f), second(s) { }
};
// set<>でのテストを行うため
bool operator<(TestClass const& lhs, TestClass const& rhs)
{
    return lhs.first < rhs.first;
}
#define INITIALIZER_TEST_CLASS  {{0,8}, {1,3}, {2,9}, {3,6}, {4,4}, {5,3}}
#define INITIALIZER_INT         {0, 1, 12, 13, 24, 25, 36, 37, 48, 49}

TestClass const kTestDataClass[] = INITIALIZER_TEST_CLASS;
int       const kTestDataInt[]   = INITIALIZER_INT;

#define TEST_CLASS_COUNT        (std::extent<decltype(kTestDataClass)>::value)
#define INT_COUNT               (std::extent<decltype(kTestDataInt)>::value)

#define STOP_INDEX              3

//----------------------------------------------------------------------------
//      テスト用機能クラス
//----------------------------------------------------------------------------

template<class tIterator>
class AnotherRange : public theolizer::BasicRange<tIterator>
{
    ptrdiff_t   mRemaining;
public:
    AnotherRange(tIterator&& iBegin, tIterator&& iEnd) :
        theolizer::BasicRange<tIterator>
        (
            std::forward<tIterator>(iBegin),
            std::forward<tIterator>(iEnd)
        ),
        mRemaining(std::distance(iBegin, iEnd)-1)
    { }
    void drop_front()
    {
        --mRemaining;
        theolizer::BasicRange<tIterator>::drop_front();
    }
    ptrdiff_t getRemaining()
    {
        return mRemaining;
    }

    // イテレータ的にアクセスできるようにする
    AnotherRange& operator++()
    {
        drop_front();
        return *this;
    }
    AnotherRange operator++(int)
    {
        AnotherRange ret(*this);
        drop_front();
        return ret;
    }
};

//----------------------------------------------------------------------------
//      TestClassとmapのテスト用
//----------------------------------------------------------------------------

template<class tContainer>
void testTestClass(tContainer&& iContainer, bool iIsConst=false)
{
//      ---<<< Separator通常ループのテスト >>>---

    {
        std::size_t aIndex=0;

        // auto&&をテスト
        for (auto&& aSeparator :
            theolizer::getRBForSeparator(iContainer))
        {
            auto&& aData=aSeparator.front();

            THEOLIZER_EQUAL(kTestDataClass[aIndex].first,  aData.first);
            THEOLIZER_EQUAL(kTestDataClass[aIndex].second, aData.second);
            ++aIndex;

            // const性のテスト
            typedef typename std::remove_reference<decltype(aData)>::type DataType;
            THEOLIZER_EQUAL(std::is_const<DataType>::value, iIsConst);
        }
    }

//      ---<<< Separator分割ループのテスト >>>---

    {
        std::size_t aIndex=0;

        auto&& aRBForSeparator=theolizer::getRBForSeparator(iContainer);
        // auto&をテスト
        for (auto& aSeparator : aRBForSeparator)
        {
            auto&& aData=aSeparator.front();

            THEOLIZER_EQUAL(kTestDataClass[aIndex].first,  aData.first);
            THEOLIZER_EQUAL(kTestDataClass[aIndex].second, aData.second);
            ++aIndex;

            if (aIndex == STOP_INDEX)
            {
                aSeparator.drop_front();
        break;
            }
        }

        // autoをテスト
        for (auto aSeparator : aRBForSeparator)
        {
            auto&& aData=aSeparator.front();

            THEOLIZER_EQUAL(kTestDataClass[aIndex].first,  aData.first);
            THEOLIZER_EQUAL(kTestDataClass[aIndex].second, aData.second);
            ++aIndex;
        }

        // これ以上回らないことを確認
        for (auto aSeparator : aRBForSeparator)
        {
            aSeparator.front();
            THEOLIZER_EQUAL(0, 1);
        }

        // 余分にpop_front()すると例外が発生することを確認
        THEOLIZER_CHECK_EXCEPTION2(
            aRBForSeparator.getRange().drop_front(),    // dStatement
            std::out_of_range e,                        // dException
            string(e.what()) == "BasicRange::drop_front()",// dJudge
            e.what()                                    // dResult
        );
    }

//      ---<<< Indexer通常ループのテスト >>>---

    {
        std::size_t aIndex=0;

        // auto&&をテスト
        for (auto&& aIndexer :
            theolizer::getRBForIndexer(iContainer))
        {
            auto&& aData=aIndexer.front();

            THEOLIZER_EQUAL(aIndex, aIndexer.getIndex());
            THEOLIZER_EQUAL(kTestDataClass[aIndex].first,  aData.first);
            THEOLIZER_EQUAL(kTestDataClass[aIndex].second, aData.second);
            ++aIndex;
        }
    }

//      ---<<< ラムダ式のテスト >>>---

    {
        std::size_t aIndex=0;
        std::size_t aLambdaIndex=0;

        for (auto&& aIndexer :
            theolizer::getRBForIndexer(iContainer, [&aLambdaIndex](...){++aLambdaIndex;}))
        {
            auto&& aData=aIndexer.front();

            THEOLIZER_EQUAL(aIndex, aIndexer.getIndex());
            THEOLIZER_EQUAL(aIndex, aLambdaIndex);
            THEOLIZER_EQUAL(kTestDataClass[aIndex].first,  aData.first);
            THEOLIZER_EQUAL(kTestDataClass[aIndex].second, aData.second);
            ++aIndex;
        }
    }

//      ---<<< 機能クラスのテスト >>>---

    {
        std::size_t aIndex=0;

        ptrdiff_t   aSize = std::distance(std::begin(iContainer), std::end(iContainer));
        for (auto&& aCountDown :
            theolizer::getRBForAdaptor<AnotherRange>(iContainer))
        {
            auto&& aData=aCountDown.front();

            THEOLIZER_EQUAL(static_cast<ptrdiff_t>(aSize-aIndex-1), aCountDown.getRemaining());
            THEOLIZER_EQUAL(kTestDataClass[aIndex].first,  aData.first);
            THEOLIZER_EQUAL(kTestDataClass[aIndex].second, aData.second);
            ++aIndex;
        }
    }

//      ---<<< SeparatorのイテレータI/Fのテスト >>>---

    {
        std::size_t aIndex=0;

        auto&& aRBForSeparator=theolizer::getRBForSeparator(iContainer);
        auto& aSeparator = aRBForSeparator.getRange();

        THEOLIZER_EQUAL(kTestDataClass[aIndex].first,  aSeparator->first);
        THEOLIZER_EQUAL(kTestDataClass[aIndex].second, aSeparator->second);
        ++aSeparator;
        ++aIndex;

        THEOLIZER_EQUAL(kTestDataClass[aIndex].first,  aSeparator->first);
        THEOLIZER_EQUAL(kTestDataClass[aIndex].second, aSeparator->second);
        aSeparator++;
        ++aIndex;

        auto&& aData=*aSeparator;
        THEOLIZER_EQUAL(aData.first,  kTestDataClass[aIndex].first);
        THEOLIZER_EQUAL(aData.second, kTestDataClass[aIndex].second);
    }

//      ---<<< IndexerのイテレータI/Fのテスト >>>---

    {
        std::size_t aIndex=0;

        auto&& aRBForIndexer=theolizer::getRBForIndexer(iContainer);
        auto& aIndexer = aRBForIndexer.getRange();

        THEOLIZER_EQUAL(aIndex, aIndexer.getIndex());
        THEOLIZER_EQUAL(kTestDataClass[aIndex].first,  aIndexer->first);
        THEOLIZER_EQUAL(kTestDataClass[aIndex].second, aIndexer->second);
        ++aIndexer;
        ++aIndex;

        THEOLIZER_EQUAL(aIndex, aIndexer.getIndex());
        THEOLIZER_EQUAL(kTestDataClass[aIndex].first,  aIndexer->first);
        THEOLIZER_EQUAL(kTestDataClass[aIndex].second, aIndexer->second);
        aIndexer++;
        ++aIndex;

        auto&& aData=*aIndexer;
        THEOLIZER_EQUAL(aIndex, aIndexer.getIndex());
        THEOLIZER_EQUAL(aData.first,  kTestDataClass[aIndex].first);
        THEOLIZER_EQUAL(aData.second, kTestDataClass[aIndex].second);
    }
}

//----------------------------------------------------------------------------
//      int配列のテスト用
//----------------------------------------------------------------------------

template<class tContainer>
void testInt(tContainer&& iContainer, bool iIsConst=false)
{
//      ---<<< Separator通常ループのテスト >>>---

    {
        std::size_t aIndex=0;

        // auto&&をテスト
        for (auto&& aSeparator :
            theolizer::getRBForSeparator(iContainer))
        {
            THEOLIZER_EQUAL(*aSeparator, kTestDataInt[aIndex]);
            ++aIndex;

            // const性のテスト
            typedef typename std::remove_reference<decltype(*aSeparator)>::type DataType;
            THEOLIZER_EQUAL(std::is_const<DataType>::value, iIsConst);
        }
    }
}

// ***************************************************************************
//      テストのメイン関数
// ***************************************************************************

int main(int argc, char** argv)
{
//  theolizer::DisplayPass aDisplayPass;

//----------------------------------------------------------------------------
//      バージョン管理機能のテスト
//----------------------------------------------------------------------------

    {
        THEOLIZER_EQUAL(theolizer::getVersionNo(),   theolizer::kTheolizerVersion);
        THEOLIZER_EQUAL(theolizer::getSourcesHash(), theolizer::kTheolizerSourcesHash);
        THEOLIZER_EQUAL(theolizer::getLibraryHash(), theolizer::kTheolizerLibraryHash);

        std::string temp;
        temp=THEOLIZER_INTERNAL_PRODUCT_NAME " version ";
        temp += theolizer::getVersionNo();
        temp += " " THEOLIZER_INTERNAL_LICENSE "\n";
        temp += THEOLIZER_INTERNAL_COPYRIGHT "\n";
        temp += "    SourcesHash : ";
        temp += theolizer::getSourcesHash();
        THEOLIZER_EQUAL(theolizer::getVersionString(), temp);
        std::cout << "--------------------------------------------------------\n";
        std::cout << temp << "\n";
        std::cout << "--------------------------------------------------------\n";

        THEOLIZER_EQUAL(theolizer::checkLibraryHash(), true);
        THEOLIZER_EQUAL(theolizer::checkLibraryHash("850fbe2af013e3c06d1b251d8053adc2"), false);
    }

//----------------------------------------------------------------------------
//      範囲ベースfor拡張機能のテスト
//----------------------------------------------------------------------------

//      ---<<< 生配列 >>>---

    {
        std::cout << "RawArray0\n";
        TestClass aTestClass[]=INITIALIZER_TEST_CLASS;
        testTestClass(aTestClass, false);
        std::cout << "RawArray1\n";
        TestClass const aTestClassConst[]=INITIALIZER_TEST_CLASS;
        testTestClass(aTestClassConst, true);

        std::cout << "RawArray2\n";
        int aInt[]=INITIALIZER_INT;
        testInt(aInt, false);
        std::cout << "RawArray3\n";
        int const aIntConst[]=INITIALIZER_INT;
        testInt(aIntConst, true);
    }

//      ---<<< std::array >>>---

    {
        std::cout << "array0\n";
        testTestClass(std::array<TestClass, TEST_CLASS_COUNT>{INITIALIZER_TEST_CLASS});
        std::cout << "array1\n";
        testTestClass(std::array<TestClass const, TEST_CLASS_COUNT>{INITIALIZER_TEST_CLASS}, true);
        std::cout << "array2\n";
        std::array<TestClass, TEST_CLASS_COUNT> const aTestClassConst={INITIALIZER_TEST_CLASS};
        testTestClass(aTestClassConst, true);

        std::cout << "array3\n";
        testInt(std::array<int, INT_COUNT>{INITIALIZER_INT});
        std::cout << "array4\n";
        testInt(std::array<int const, INT_COUNT>{INITIALIZER_INT}, true);
        std::cout << "array5\n";
        std::array<int, INT_COUNT> const aIntConst={INITIALIZER_INT};
        testInt(aIntConst, true);
    }

//      ---<<< std::deque >>>---

    {
        std::cout << "deque0\n";
        testTestClass(std::deque<TestClass>{INITIALIZER_TEST_CLASS});
        std::cout << "deque1\n";
        std::deque<TestClass> const aTestClassConst={INITIALIZER_TEST_CLASS};
        testTestClass(aTestClassConst, true);

        std::cout << "deque2\n";
        testInt(std::deque<int>INITIALIZER_INT);
        std::cout << "deque3\n";
        std::deque<int> const aIntConst=INITIALIZER_INT;
        testInt(aIntConst, true);
    }

//      ---<<< std::forward_list >>>---

    {
        std::cout << "forward_list0\n";
        testTestClass(std::forward_list<TestClass>{INITIALIZER_TEST_CLASS});
        std::cout << "forward_list1\n";
        std::forward_list<TestClass> const aTestClassConst={INITIALIZER_TEST_CLASS};
        testTestClass(aTestClassConst, true);

        std::cout << "forward_list2\n";
        testInt(std::forward_list<int>INITIALIZER_INT);
        std::cout << "forward_list3\n";
        std::forward_list<int> const aIntConst=INITIALIZER_INT;
        testInt(aIntConst, true);
    }

//      ---<<< std::list >>>---

    {
        std::cout << "list0\n";
        testTestClass(std::list<TestClass>{INITIALIZER_TEST_CLASS});
        std::cout << "list1\n";
        std::list<TestClass> const aTestClassConst={INITIALIZER_TEST_CLASS};
        testTestClass(aTestClassConst, true);

        std::cout << "list2\n";
        testInt(std::list<int>INITIALIZER_INT);
        std::cout << "list3\n";
        std::list<int> const aIntConst=INITIALIZER_INT;
        testInt(aIntConst, true);
    }

//      ---<<< std::map >>>---

    {
        std::cout << "map0\n";
        testTestClass(std::map<int, unsigned>{INITIALIZER_TEST_CLASS});
        std::cout << "map1\n";
        std::map<int, unsigned> const aMap={INITIALIZER_TEST_CLASS};
        testTestClass(aMap, true);
    }

//      ---<<< std::multimap >>>---

    {
        std::cout << "multimap0\n";
        testTestClass(std::multimap<int, unsigned>{INITIALIZER_TEST_CLASS});
        std::cout << "multimap1\n";
        std::multimap<int, unsigned> const aMap={INITIALIZER_TEST_CLASS};
        testTestClass(aMap, true);
    }

//      ---<<< std::set >>>---

    {
        std::cout << "set0\n";
        testTestClass(std::set<TestClass>{INITIALIZER_TEST_CLASS}, true);
        std::cout << "set1\n";
        std::set<TestClass> const aTestClassConst={INITIALIZER_TEST_CLASS};
        testTestClass(aTestClassConst, true);

        std::cout << "set2\n";
        testInt(std::set<int>INITIALIZER_INT, true);
        std::cout << "set3\n";
        std::set<int> const aIntConst=INITIALIZER_INT;
        testInt(aIntConst, true);
    }

//      ---<<< std::multiset >>>---

    {
        std::cout << "multiset0\n";
        testTestClass(std::multiset<TestClass>{INITIALIZER_TEST_CLASS}, true);
        std::cout << "multiset1\n";
        std::multiset<TestClass> const aTestClassConst={INITIALIZER_TEST_CLASS};
        testTestClass(aTestClassConst, true);

        std::cout << "multiset2\n";
        testInt(std::multiset<int>INITIALIZER_INT, true);
        std::cout << "multiset3\n";
        std::multiset<int> const aIntConst=INITIALIZER_INT;
        testInt(aIntConst, true);
    }

//      ---<<< std::vector >>>---

    {
        std::cout << "vector0\n";
        testTestClass(std::vector<TestClass>{INITIALIZER_TEST_CLASS});
        std::cout << "vector1\n";
        std::vector<TestClass> const aTestClassConst={INITIALIZER_TEST_CLASS};
        testTestClass(aTestClassConst, true);

        std::cout << "vector2\n";
        testInt(std::vector<int>INITIALIZER_INT);
        std::cout << "vector3\n";
        std::vector<int> const aIntConst=INITIALIZER_INT;
        testInt(aIntConst, true);
    }

    return 0;
}
