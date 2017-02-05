//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          STLサポート
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

#include "disable_test.h"
#ifndef DISABLE_SUPPORT_STL_TEST

namespace std
{
    template<class _Ty>
    struct default_delete;
}

// ***************************************************************************
//      インクルード
// ***************************************************************************

// 標準ライブラリ
#include <iostream>
#include <fstream>
#include <ctime>

// theolizerライブラリ
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_json.h>

// プロジェクト・ヘッダ
#include "common.h"
#include "test_support_stl.h"

// 自動生成ソース
#include "test_support_stl.cpp.theolizer.hpp"

//############################################################################
//      使い方の説明
//############################################################################

// ***************************************************************************
//      動作確認
// ***************************************************************************

void tutoriseSupportStl()
{
    std::cout << "tutoriseSupportStl() start" << std::endl;

//----------------------------------------------------------------------------
//      保存
//----------------------------------------------------------------------------

    {
//      ---<<< シリアライズ対象変数定義 >>>---


//      ---<<< 保存処理 >>>---

        std::ofstream   aStream("tutorise_support_stl.json");
        theolizer::JsonOSerializer<> aSerializer(aStream);

        // オーナーとして保存／回復する

        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();
    }

//----------------------------------------------------------------------------
//      回復
//----------------------------------------------------------------------------

    {

//      ---<<< 回復先変数定義 >>>---


//      ---<<< 回復処理 >>>---

        std::ifstream   aStream("tutorise_support_stl.json");
        theolizer::JsonISerializer<> aSerializer(aStream);

        // オーナーとして保存／回復する

        // オブジェクトIDテーブルのクリア
        aSerializer.clearTracking();

//      ---<<< 結果のチェック >>>---

    }

    std::cout << "tutoriseSupportStl() end\n" << std::endl;
}

//############################################################################
//      網羅テスト
//############################################################################

// ***************************************************************************
//      サブ処理
// ***************************************************************************

//      ---<<< 固定長のもののテスト >>>---

template<class tSerializer, class tContainer, typename tType>
void saveContainerFixed(tSerializer& iSerializer, tContainer& iContainer, tType const& iFirst)
{
    // 保存データ生成
    std::vector<tType*> aPtr;
    aPtr.resize(iContainer.size());
    for (std::size_t i=0; i < iContainer.size(); ++i)
    {
        iContainer[i]=iFirst+static_cast<int>(i);
        aPtr[i]=&iContainer[i];
    }

    // 保存
    for (std::size_t i=0; i < iContainer.size(); ++i)
    {
        THEOLIZER_PROCESS(iSerializer, aPtr[i]);
    }
    THEOLIZER_PROCESS(iSerializer, iContainer);
}

template<class tSerializer, class tContainer, typename tType>
void loadContainerFixed(tSerializer& iSerializer, tContainer& iContainer, tType const& iFirst)
{
    // 回復先生成
    std::vector<tType*> aPtr;
    aPtr.resize(iContainer.size());
    for (std::size_t i=0; i < iContainer.size(); ++i)
    {
        aPtr[i]=nullptr;
    }

    // 回復
    for (std::size_t i=0; i < iContainer.size(); ++i)
    {
        THEOLIZER_PROCESS(iSerializer, aPtr[i]);
    }
    THEOLIZER_PROCESS(iSerializer, iContainer);

    // チェック
    for (std::size_t i=0; i < iContainer.size(); ++i)
    {
        THEOLIZER_EQUAL_PTR(aPtr[i], &iContainer[i]);
        THEOLIZER_EQUAL(iContainer[i], iFirst+static_cast<int>(i));
    }
}

//      ---<<< キーがないもののテスト >>>---

// プライマリ
template<class tContainer, typename tType, class tEnable=void>
struct Emplace
{
    void operator()(tContainer& iContainer, tType const& iFirst, std::size_t iCount)
    {
        for (std::size_t i=0; i < iCount; ++i)
        {
            iContainer.emplace(iContainer.end(), iFirst+static_cast<int>(i));
        }
    }
};

// forward_list用(先頭への追加が自然なので、逆順で追加する)
template<class tContainer, typename tType>
struct Emplace
<
    tContainer,
    tType,
    typename std::enable_if<std::is_convertible<tContainer,std::forward_list<tType>>::value>::type
>
{
    void operator()(tContainer& iContainer, tType const& iFirst, std::size_t iCount)
    {
        for (std::size_t i=iCount; 0 < i; --i)
        {
            iContainer.emplace_front(iFirst+static_cast<int>(i-1));
        }
    }
};

// 本体
template<class tSerializer, class tContainer, typename tType>
void saveContainer3(tSerializer& iSerializer, tContainer& iContainer, tType const& iFirst)
{
    // 保存データ生成
    tType   *aPtr0;
    tType   *aPtr1;
    tType   *aPtr2;
    Emplace<tContainer, tType> emplace;
    emplace(iContainer, iFirst, 3);
    auto it=iContainer.begin();
    aPtr0=&*(it++);
    aPtr1=&*(it++);
    aPtr2=&*(it++);

    // 保存
    THEOLIZER_PROCESS(iSerializer, aPtr0);
    THEOLIZER_PROCESS(iSerializer, aPtr1);
    THEOLIZER_PROCESS(iSerializer, aPtr2);
    THEOLIZER_PROCESS(iSerializer, iContainer);
}

// 事前準備要素数=0
template<class tSerializer, class tContainer, typename tType>
void loadContainer0(tSerializer& iSerializer, tContainer& iContainer, tType const& iFirst)
{
    // 回復先生成
    tType   *aPtr0=nullptr;
    tType   *aPtr1=nullptr;
    tType   *aPtr2=nullptr;
    iContainer.clear();

    // 回復
    THEOLIZER_PROCESS(iSerializer, aPtr0);
    THEOLIZER_PROCESS(iSerializer, aPtr1);
    THEOLIZER_PROCESS(iSerializer, aPtr2);
    THEOLIZER_PROCESS(iSerializer, iContainer);

    // チェック
    auto itr=iContainer.begin();
    THEOLIZER_EQUAL_PTR(aPtr0, &*itr);  ++itr;
    THEOLIZER_EQUAL_PTR(aPtr1, &*itr);  ++itr;
    THEOLIZER_EQUAL_PTR(aPtr2, &*itr);  ++itr;
    itr=iContainer.begin();
    THEOLIZER_EQUAL(*itr, iFirst+0);    ++itr;
    THEOLIZER_EQUAL(*itr, iFirst+1);    ++itr;
    THEOLIZER_EQUAL(*itr, iFirst+2);    ++itr;
}

// 事前準備要素数=2
template<class tSerializer, class tContainer, typename tType>
void loadContainer2(tSerializer& iSerializer, tContainer& iContainer, tType const& iFirst)
{
    // 回復先生成
    tType   *aPtr0=nullptr;
    tType   *aPtr1=nullptr;
    tType   *aPtr2=nullptr;
    iContainer.resize(2);

    // 回復
    THEOLIZER_PROCESS(iSerializer, aPtr0);
    THEOLIZER_PROCESS(iSerializer, aPtr1);
    THEOLIZER_PROCESS(iSerializer, aPtr2);
    THEOLIZER_PROCESS(iSerializer, iContainer);

    // チェック
    auto itr=iContainer.begin();
    THEOLIZER_EQUAL_PTR(aPtr0, &*itr);  ++itr;
    THEOLIZER_EQUAL_PTR(aPtr1, &*itr);  ++itr;
    THEOLIZER_EQUAL_PTR(aPtr2, &*itr);  ++itr;
    itr=iContainer.begin();
    THEOLIZER_EQUAL(*itr, iFirst+0);    ++itr;
    THEOLIZER_EQUAL(*itr, iFirst+1);    ++itr;
    THEOLIZER_EQUAL(*itr, iFirst+2);    ++itr;
}

// 事前準備要素数=4
template<class tSerializer, class tContainer, typename tType>
void loadContainer4(tSerializer& iSerializer, tContainer& iContainer, tType const& iFirst)
{
    // 回復先生成
    tType   *aPtr0=nullptr;
    tType   *aPtr1=nullptr;
    tType   *aPtr2=nullptr;
    iContainer.resize(4, iFirst+3);

    // 回復
    THEOLIZER_PROCESS(iSerializer, aPtr0);
    THEOLIZER_PROCESS(iSerializer, aPtr1);
    THEOLIZER_PROCESS(iSerializer, aPtr2);
    THEOLIZER_PROCESS(iSerializer, iContainer);

    // チェック
    auto itr=iContainer.begin();
    THEOLIZER_EQUAL_PTR(aPtr0, &*itr);  ++itr;
    THEOLIZER_EQUAL_PTR(aPtr1, &*itr);  ++itr;
    THEOLIZER_EQUAL_PTR(aPtr2, &*itr);  ++itr;
    itr=iContainer.begin();
    THEOLIZER_EQUAL(*itr, iFirst+0);    ++itr;
    THEOLIZER_EQUAL(*itr, iFirst+1);    ++itr;
    THEOLIZER_EQUAL(*itr, iFirst+2);    ++itr;
    THEOLIZER_EQUAL(*itr, iFirst+3);    ++itr;
}

//      ---<<< std::setのテスト >>>---

template<class tSerializer, class tContainer, typename tType>
void saveContainerSet3(tSerializer& iSerializer, tType const& iFirst)
{
    for (int i=0; i < 2; ++i)
    {
        // 保存データ生成
        tContainer  aContainer;
        aContainer.insert(iFirst+i*100+0);
        aContainer.insert(iFirst+i*100+1);
        aContainer.insert(iFirst+i*100+2);

        // 保存
        THEOLIZER_PROCESS(iSerializer, aContainer);
    }
}

template<class tSerializer, class tContainer, typename tType>
void loadContainerSet3(tSerializer& iSerializer, tType const& iFirst)
{
    {
        // 回復領域生成
        tContainer  aContainer;

        // 回復
        THEOLIZER_PROCESS(iSerializer, aContainer);

        // チェック
        THEOLIZER_EQUAL(aContainer.size(), 3);
        THEOLIZER_CHECK(aContainer.find(iFirst+0) != aContainer.end(), iFirst+0);
        THEOLIZER_CHECK(aContainer.find(iFirst+1) != aContainer.end(), iFirst+1);
        THEOLIZER_CHECK(aContainer.find(iFirst+2) != aContainer.end(), iFirst+2);
    }
    {
        // 回復領域生成
        tContainer  aContainer;
        aContainer.insert(iFirst+100+0);
        aContainer.insert(iFirst+100+1);
        aContainer.insert(iFirst+100+2);
        aContainer.insert(iFirst+100+3);

        // 回復
        THEOLIZER_PROCESS(iSerializer, aContainer);

        // チェック
        THEOLIZER_EQUAL(aContainer.size(), 4);
        THEOLIZER_CHECK(aContainer.find(iFirst+100+0) != aContainer.end(), iFirst+100+0);
        THEOLIZER_CHECK(aContainer.find(iFirst+100+1) != aContainer.end(), iFirst+100+1);
        THEOLIZER_CHECK(aContainer.find(iFirst+100+2) != aContainer.end(), iFirst+100+2);
        THEOLIZER_CHECK(aContainer.find(iFirst+100+3) != aContainer.end(), iFirst+100+3);
    }
}

// ***************************************************************************
//      保存
// ***************************************************************************

template<class tSerializer>
void saveSupportStl(tSerializer& iSerializer)
{
//----------------------------------------------------------------------------
//      スマート・ポインタ対応
//----------------------------------------------------------------------------

//      ---<<< 手動(トップ・レベル)による保存 >>>---

#if 1
    {
        std::cout << "        saveSupportStl() : Manual(Top Level)" << std::endl;

        // 保存情報生成
        std::shared_ptr<SmartBase>  aSharedNull{};
        std::shared_ptr<SmartBase>  aForWeakNull{};
        std::shared_ptr<SmartBase>  aShared{new SmartDerived{100}};
        std::shared_ptr<SmartBase>  aForWeak{new SmartDerived{110}};

        // nullptrチェック用
        SmartTestAuto               aSmartTestAutoNull{0, aSharedNull, aForWeakNull};
        // nullptrへの回復チェック用
        SmartTestAuto               aSmartTestAuto0{120, aShared, aForWeak};
        // 同じ型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto1{130, aShared, aForWeak};
        // 異なる型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto2{140, aShared, aForWeak};

        // 保存
        THEOLIZER_PROCESS(iSerializer, aShared);
        THEOLIZER_PROCESS(iSerializer, aForWeak);
        processSmart(iSerializer, aSmartTestAutoNull);
        processSmart(iSerializer, aSmartTestAuto0);
        processSmart(iSerializer, aSmartTestAuto1);
        processSmart(iSerializer, aSmartTestAuto2);

        iSerializer.clearTracking();
    }

//      ---<<< 自動メンバ・リスト生成による保存 >>>---

    {
        std::cout << "        saveSupportStl() : Auto" << std::endl;

        // 保存情報生成
        std::shared_ptr<SmartBase>  aSharedNull{};
        std::shared_ptr<SmartBase>  aForWeakNull{};
        std::shared_ptr<SmartBase>  aShared{new SmartDerived{200}};
        std::shared_ptr<SmartBase>  aForWeak{new SmartDerived{210}};

        // nullptrチェック用
        SmartTestAuto               aSmartTestAutoNull{0, aSharedNull, aForWeakNull};
        // nullptrへの回復チェック用
        SmartTestAuto               aSmartTestAuto0{220, aShared, aForWeak};
        // 同じ型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto1{230, aShared, aForWeak};
        // 異なる型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto2{240, aShared, aForWeak};

        // 保存
        THEOLIZER_PROCESS(iSerializer, aShared);
        THEOLIZER_PROCESS(iSerializer, aForWeak);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAutoNull);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAuto0);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAuto1);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAuto2);

        iSerializer.clearTracking();
    }

//      ---<<< 手動(非トップ・レベル)による保存 >>>---

    {
        std::cout << "        saveSupportStl() : Manul(Non-Top)" << std::endl;

        // 保存情報生成
        std::shared_ptr<SmartBase>  aSharedNull{};
        std::shared_ptr<SmartBase>  aForWeakNull{};
        std::shared_ptr<SmartBase>  aShared{new SmartDerived{300}};
        std::shared_ptr<SmartBase>  aForWeak{new SmartDerived{310}};

        // nullptrチェック用
        SmartTestManual             aSmartTestManualNull{0, aSharedNull, aForWeakNull};
        // nullptrへの回復チェック用
        SmartTestManual             aSmartTestManual0{320, aShared, aForWeak};
        // 同じ型ポインタへの回復チェック用
        SmartTestManual             aSmartTestManual1{330, aShared, aForWeak};
        // 異なる型ポインタへの回復チェック用
        SmartTestManual             aSmartTestManual2{340, aShared, aForWeak};

        // 保存
        THEOLIZER_PROCESS(iSerializer, aShared);
        THEOLIZER_PROCESS(iSerializer, aForWeak);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManualNull);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManual0);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManual1);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManual2);

        iSerializer.clearTracking();
    }
#endif

//----------------------------------------------------------------------------
//      コンテナ対応
//----------------------------------------------------------------------------

//      ---<<< std::array >>>---

#if 1
    {
        std::cout << "        saveContainerFixed() : std::array<TestStl, 3>" << std::endl;
        std::array<TestStl, 3>  aArrayTestStl0;
        saveContainerFixed(iSerializer, aArrayTestStl0, TestStl(100));

        std::cout << "        saveContainerFixed() : std::array<TestStl, 4>" << std::endl;
        std::array<TestStl, 4>  aArrayTestStl1;
        saveContainerFixed(iSerializer, aArrayTestStl1, TestStl(200));

        std::cout << "        saveContainerFixed() : theolizer::ArrayPointee<int, 3>" << std::endl;
        theolizer::ArrayPointee<int, 3> aArrayInt0;
        saveContainerFixed(iSerializer, aArrayInt0, 500);

        std::cout << "        saveContainerFixed() : theolizer::ArrayPointee<int, 4>" << std::endl;
        theolizer::ArrayPointee<int, 4> aArrayInt1;
        saveContainerFixed(iSerializer, aArrayInt1, 600);

        iSerializer.clearTracking();
    }

//      ---<<< std::vector >>>---

    {
        std::cout << "        saveContainer3() : std::vector<TestStl>" << std::endl;
        std::vector<TestStl>    aVectorTestStl0;
        saveContainer3(iSerializer, aVectorTestStl0, TestStl(100));

        std::cout << "        saveContainer3() : std::vector<TestStl>" << std::endl;
        std::vector<TestStl>    aVectorTestStl1;
        saveContainer3(iSerializer, aVectorTestStl1, TestStl(200));

        std::cout << "        saveContainer3() : std::vector<TestStl>" << std::endl;
        std::vector<TestStl>    aVectorTestStl2;
        saveContainer3(iSerializer, aVectorTestStl2, TestStl(300));

        std::cout << "        saveContainer3() : theolizer::VectorPointee<int>" << std::endl;
        theolizer::VectorPointee<int>  aVectorInt0;
        saveContainer3(iSerializer, aVectorInt0, 500);

        std::cout << "        saveContainer3() : theolizer::VectorPointee<int>" << std::endl;
        theolizer::VectorPointee<int>  aVectorInt1;
        saveContainer3(iSerializer, aVectorInt1, 600);

        std::cout << "        saveContainer3() : theolizer::VectorPointee<int>" << std::endl;
        theolizer::VectorPointee<int>  aVectorInt2;
        saveContainer3(iSerializer, aVectorInt2, 700);

        iSerializer.clearTracking();
    }

//      ---<<< std::vector<bool> >>>---

    {
        std::cout << "        save : std::vector<bool>" << std::endl;
        std::vector<bool>   aVectorBool;
        aVectorBool.resize(3, true);
        THEOLIZER_PROCESS(iSerializer, aVectorBool);
        THEOLIZER_PROCESS(iSerializer, aVectorBool);
        THEOLIZER_PROCESS(iSerializer, aVectorBool);
    }

//      ---<<< std::deque >>>---

    {
        std::cout << "        saveContainer3() : std::deque<TestStl>" << std::endl;
        std::deque<TestStl> aDequeTestStl0;
        saveContainer3(iSerializer, aDequeTestStl0, TestStl(100));

        std::cout << "        saveContainer3() : std::deque<TestStl>" << std::endl;
        std::deque<TestStl> aDequeTestStl1;
        saveContainer3(iSerializer, aDequeTestStl1, TestStl(200));

        std::cout << "        saveContainer3() : std::deque<TestStl>" << std::endl;
        std::deque<TestStl> aDequeTestStl2;
        saveContainer3(iSerializer, aDequeTestStl2, TestStl(300));

        std::cout << "        saveContainer3() : theolizer::DequePointee<int>" << std::endl;
        theolizer::DequePointee<int>    aDequeInt0;
        saveContainer3(iSerializer, aDequeInt0, 500);

        std::cout << "        saveContainer3() : theolizer::DequePointee<int>" << std::endl;
        theolizer::DequePointee<int>    aDequeInt1;
        saveContainer3(iSerializer, aDequeInt1, 600);

        std::cout << "        saveContainer3() : theolizer::DequePointee<int>" << std::endl;
        theolizer::DequePointee<int>    aDequeInt2;
        saveContainer3(iSerializer, aDequeInt2, 700);

        iSerializer.clearTracking();
    }

//      ---<<< std::list >>>---

    {
        std::cout << "        saveContainer3() : std::list<TestStl>" << std::endl;
        std::list<TestStl>  aListTestStl0;
        saveContainer3(iSerializer, aListTestStl0, TestStl(100));

        std::cout << "        saveContainer3() : std::list<TestStl>" << std::endl;
        std::list<TestStl>  aListTestStl1;
        saveContainer3(iSerializer, aListTestStl1, TestStl(200));

        std::cout << "        saveContainer3() : std::list<TestStl>" << std::endl;
        std::list<TestStl>  aListTestStl2;
        saveContainer3(iSerializer, aListTestStl2, TestStl(300));

        std::cout << "        saveContainer3() : theolizer::ListPointee<int>" << std::endl;
        theolizer::ListPointee<int> aListInt0;
        saveContainer3(iSerializer, aListInt0, 500);

        std::cout << "        saveContainer3() : theolizer::ListPointee<int>" << std::endl;
        theolizer::ListPointee<int> aListInt1;
        saveContainer3(iSerializer, aListInt1, 600);

        std::cout << "        saveContainer3() : theolizer::ListPointee<int>" << std::endl;
        theolizer::ListPointee<int> aListInt2;
        saveContainer3(iSerializer, aListInt2, 700);

        iSerializer.clearTracking();
    }

//      ---<<< std::forward_list >>>---

    {
        std::cout << "        saveContainer3() : std::forward_list<TestStl>" << std::endl;
        std::forward_list<TestStl>  aForwardListTestStl0;
        saveContainer3(iSerializer, aForwardListTestStl0, TestStl(100));

        std::cout << "        saveContainer3() : std::forward_list<TestStl>" << std::endl;
        std::forward_list<TestStl>  aForwardListTestStl1;
        saveContainer3(iSerializer, aForwardListTestStl1, TestStl(200));

        std::cout << "        saveContainer3() : std::forward_list<TestStl>" << std::endl;
        std::forward_list<TestStl>  aForwardListTestStl2;
        saveContainer3(iSerializer, aForwardListTestStl2, TestStl(300));

        std::cout << "        saveContainer3() : theolizer::ForwardListPointee<int>" << std::endl;
        theolizer::ForwardListPointee<int> aForwardListInt0;
        saveContainer3(iSerializer, aForwardListInt0, 500);

        std::cout << "        saveContainer3() : theolizer::ForwardListPointee<int>" << std::endl;
        theolizer::ForwardListPointee<int> aForwardListInt1;
        saveContainer3(iSerializer, aForwardListInt1, 600);

        std::cout << "        saveContainer3() : theolizer::ForwardListPointee<int>" << std::endl;
        theolizer::ForwardListPointee<int> aForwardListInt2;
        saveContainer3(iSerializer, aForwardListInt2, 700);

        iSerializer.clearTracking();
    }
#endif

//      ---<<< std::set >>>---

    {
        std::cout << "        saveContainerSet3() : int" << std::endl;
        saveContainerSet3<tSerializer, std::set<int>, int>
        (
            iSerializer, 100
        );

        saveContainerSet3<tSerializer, std::set<TestStl>, TestStl>
        (
            iSerializer, 500
        );
    }
}

INSTANTIATION_ALL(saveSupportStl);

// ***************************************************************************
//      回復
// ***************************************************************************

template<class tSerializer>
void loadSupportStl(tSerializer& iSerializer)
{
//----------------------------------------------------------------------------
//      スマート・ポインタ対応
//----------------------------------------------------------------------------

//      ---<<< 手動(トップ・レベル)による回復 >>>---

#if 1
    {
        std::cout << "        loadSupportStl() : Manual(Top Level)" << std::endl;

        // 回復先生成
        std::shared_ptr<SmartBase>  aSharedNull{};
        std::shared_ptr<SmartBase>  aForWeakNull{};
        std::shared_ptr<SmartBase>  aShared{new SmartDerived{}};
        std::shared_ptr<SmartBase>  aForWeak{new SmartDerived{}};

        // nullptrチェック用
        SmartTestAuto               aSmartTestAutoNull{};
        // nullptrへの回復チェック用
        SmartTestAuto               aSmartTestAuto0{};
        // 同じ型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto1{};
        // 異なる型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto2{};

        // 回復
        THEOLIZER_PROCESS(iSerializer, aShared);
        THEOLIZER_PROCESS(iSerializer, aForWeak);
        processSmart(iSerializer, aSmartTestAutoNull);
        processSmart(iSerializer, aSmartTestAuto0);
        processSmart(iSerializer, aSmartTestAuto1);
        processSmart(iSerializer, aSmartTestAuto2);

        iSerializer.clearTracking();

        // チェック
        aShared.get()->check(100);
        aForWeak.get()->check(110);
        THEOLIZER_EQUAL(aSharedNull, false);
        THEOLIZER_EQUAL(aForWeakNull, false);
        aSmartTestAuto0.check(120, aShared, aForWeak);
        aSmartTestAuto1.check(130, aShared, aForWeak);
        aSmartTestAuto2.check(140, aShared, aForWeak);
    }

//      ---<<< 自動メンバ・リスト生成による回復 >>>---

    {
        std::cout << "        loadSupportStl() : Auto" << std::endl;

        // 回復先生成
        std::shared_ptr<SmartBase>  aSharedNull{};
        std::shared_ptr<SmartBase>  aForWeakNull{};
        std::shared_ptr<SmartBase>  aShared{new SmartDerived{}};
        std::shared_ptr<SmartBase>  aForWeak{new SmartDerived{}};

        // nullptrチェック用
        SmartTestAuto               aSmartTestAutoNull{};
        // nullptrへの回復チェック用
        SmartTestAuto               aSmartTestAuto0{};
        // 同じ型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto1{};
        // 異なる型ポインタへの回復チェック用
        SmartTestAuto               aSmartTestAuto2{};

        // 回復
        THEOLIZER_PROCESS(iSerializer, aShared);
        THEOLIZER_PROCESS(iSerializer, aForWeak);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAutoNull);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAuto0);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAuto1);
        THEOLIZER_PROCESS(iSerializer, aSmartTestAuto2);

        iSerializer.clearTracking();

        // チェック
        aShared.get()->check(200);
        aForWeak.get()->check(210);
        THEOLIZER_EQUAL(aSharedNull, false);
        THEOLIZER_EQUAL(aForWeakNull, false);
        aSmartTestAuto0.check(220, aShared, aForWeak);
        aSmartTestAuto1.check(230, aShared, aForWeak);
        aSmartTestAuto2.check(240, aShared, aForWeak);
    }

//      ---<<< 手動(非トップ・レベル)による回復 >>>---

    {
        std::cout << "        loadSupportStl() : Manul(Non-Top)" << std::endl;

        // 回復先生成
        std::shared_ptr<SmartBase>  aSharedNull{};
        std::shared_ptr<SmartBase>  aForWeakNull{};
        std::shared_ptr<SmartBase>  aShared{new SmartDerived{}};
        std::shared_ptr<SmartBase>  aForWeak{new SmartDerived{}};

        // nullptrチェック用
        SmartTestManual             aSmartTestManualNull{};
        // nullptrへの回復チェック用
        SmartTestManual             aSmartTestManual0{};
        // 同じ型ポインタへの回復チェック用
        SmartTestManual             aSmartTestManual1{};
        // 異なる型ポインタへの回復チェック用
        SmartTestManual             aSmartTestManual2{};

        // 回復
        THEOLIZER_PROCESS(iSerializer, aShared);
        THEOLIZER_PROCESS(iSerializer, aForWeak);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManualNull);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManual0);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManual1);
        THEOLIZER_PROCESS(iSerializer, aSmartTestManual2);

        iSerializer.clearTracking();

        // チェック
        aShared.get()->check(300);
        aForWeak.get()->check(310);
        THEOLIZER_EQUAL(aSharedNull, false);
        THEOLIZER_EQUAL(aForWeakNull, false);
        aSmartTestManual0.check(320, aShared, aForWeak);
        aSmartTestManual1.check(330, aShared, aForWeak);
        aSmartTestManual2.check(340, aShared, aForWeak);
    }
#endif

//----------------------------------------------------------------------------
//      コンテナ対応
//----------------------------------------------------------------------------

//      ---<<< std::array >>>---

#if 1
    {
        std::cout << "        loadContainerFixed() : std::array<TestStl, 3>" << std::endl;
        std::array<TestStl, 3>  aArrayTestStl0;
        loadContainerFixed(iSerializer, aArrayTestStl0, TestStl(100));

        std::cout << "        loadContainerFixed() : std::array<TestStl, 4>" << std::endl;
        std::array<TestStl, 4>  aArrayTestStl1;
        loadContainerFixed(iSerializer, aArrayTestStl1, TestStl(200));

        std::cout << "        loadContainerFixed() : theolizer::ArrayPointee<int, 3>" << std::endl;
        theolizer::ArrayPointee<int, 3> aArrayInt0;
        loadContainerFixed(iSerializer, aArrayInt0, 500);

        std::cout << "        loadContainerFixed() : theolizer::ArrayPointee<int, 4>" << std::endl;
        theolizer::ArrayPointee<int, 4> aArrayInt1;
        loadContainerFixed(iSerializer, aArrayInt1, 600);

        iSerializer.clearTracking();
    }

//      ---<<< std::vector >>>---

    {
        std::cout << "        loadContainer0() : std::vector<TestStl>" << std::endl;
        std::vector<TestStl>    aVectorTestStl0;
        loadContainer0(iSerializer, aVectorTestStl0, TestStl(100));

        std::cout << "        loadContainer2() : std::vector<TestStl>" << std::endl;
        std::vector<TestStl>    aVectorTestStl1;
        loadContainer2(iSerializer, aVectorTestStl1, TestStl(200));

        std::cout << "        loadContainer4() : std::vector<TestStl>" << std::endl;
        std::vector<TestStl>    aVectorTestStl2;
        loadContainer4(iSerializer, aVectorTestStl2, TestStl(300));

        std::cout << "        loadContainer0() : theolizer::VectorPointee<int>" << std::endl;
        theolizer::VectorPointee<int>  aVectorInt0;
        loadContainer0(iSerializer, aVectorInt0, 500);

        std::cout << "        loadContainer2() : theolizer::VectorPointee<int>" << std::endl;
        theolizer::VectorPointee<int>  aVectorInt1;
        loadContainer2(iSerializer, aVectorInt1, 600);

        std::cout << "        loadContainer4() : theolizer::VectorPointee<int>" << std::endl;
        theolizer::VectorPointee<int>  aVectorInt2;
        loadContainer4(iSerializer, aVectorInt2, 700);

        iSerializer.clearTracking();
    }

//      ---<<< std::vector<bool> >>>---

    {
        std::cout << "        load : std::vector<bool>" << std::endl;
        std::vector<bool>   aVectorBool0;
        THEOLIZER_PROCESS(iSerializer, aVectorBool0);
        std::vector<bool>   aVectorBool1;
        aVectorBool1.resize(2, false);
        THEOLIZER_PROCESS(iSerializer, aVectorBool1);
        std::vector<bool>   aVectorBool2;
        aVectorBool2.resize(4, false);
        THEOLIZER_PROCESS(iSerializer, aVectorBool2);

        iSerializer.clearTracking();

        for (std::size_t i=0; i < 3; ++i)
        {
            THEOLIZER_EQUAL(aVectorBool0[i], true);
            THEOLIZER_EQUAL(aVectorBool1[i], true);
            THEOLIZER_EQUAL(aVectorBool2[i], true);
        }
        THEOLIZER_EQUAL(aVectorBool2[3], false);
    }

//      ---<<< std::deque >>>---

    {
        std::cout << "        loadContainer0() : std::deque<TestStl>" << std::endl;
        std::deque<TestStl> aDequeTestStl0;
        loadContainer0(iSerializer, aDequeTestStl0, TestStl(100));

        std::cout << "        loadContainer2() : std::deque<TestStl>" << std::endl;
        std::deque<TestStl> aDequeTestStl1;
        loadContainer2(iSerializer, aDequeTestStl1, TestStl(200));

        std::cout << "        loadContainer4() : std::deque<TestStl>" << std::endl;
        std::deque<TestStl> aDequeTestStl2;
        loadContainer4(iSerializer, aDequeTestStl2, TestStl(300));

        std::cout << "        loadContainer0() : theolizer::DequePointee<int>" << std::endl;
        theolizer::DequePointee<int>    aDequeInt0;
        loadContainer0(iSerializer, aDequeInt0, 500);

        std::cout << "        loadContainer2() : theolizer::DequePointee<int>" << std::endl;
        theolizer::DequePointee<int>    aDequeInt1;
        loadContainer2(iSerializer, aDequeInt1, 600);

        std::cout << "        loadContainer4() : theolizer::DequePointee<int>" << std::endl;
        theolizer::DequePointee<int>    aDequeInt2;
        loadContainer4(iSerializer, aDequeInt2, 700);

        iSerializer.clearTracking();
    }

//      ---<<< std::list >>>---

    {
        std::cout << "        loadContainer0() : std::list<TestStl>" << std::endl;
        std::list<TestStl>  aListTestStl0;
        loadContainer0(iSerializer, aListTestStl0, TestStl(100));

        std::cout << "        loadContainer2() : std::list<TestStl>" << std::endl;
        std::list<TestStl>  aListTestStl1;
        loadContainer2(iSerializer, aListTestStl1, TestStl(200));

        std::cout << "        loadContainer4() : std::list<TestStl>" << std::endl;
        std::list<TestStl>  aListTestStl2;
        loadContainer4(iSerializer, aListTestStl2, TestStl(300));

        std::cout << "        loadContainer0() : theolizer::ListPointee<int>" << std::endl;
        theolizer::ListPointee<int> aListInt0;
        loadContainer0(iSerializer, aListInt0, 500);

        std::cout << "        loadContainer2() : theolizer::ListPointee<int>" << std::endl;
        theolizer::ListPointee<int> aListInt1;
        loadContainer2(iSerializer, aListInt1, 600);

        std::cout << "        loadContainer4() : theolizer::ListPointee<int>" << std::endl;
        theolizer::ListPointee<int> aListInt2;
        loadContainer4(iSerializer, aListInt2, 700);

        iSerializer.clearTracking();
    }

//      ---<<< std::forward_list >>>---

    {
        std::cout << "        loadContainer0() : std::forward_list<TestStl>" << std::endl;
        std::forward_list<TestStl>  aForwardListTestStl0;
        loadContainer0(iSerializer, aForwardListTestStl0, TestStl(100));

        std::cout << "        loadContainer2() : std::forward_list<TestStl>" << std::endl;
        std::forward_list<TestStl>  aForwardListTestStl1;
        loadContainer2(iSerializer, aForwardListTestStl1, TestStl(200));

        std::cout << "        loadContainer4() : std::forward_list<TestStl>" << std::endl;
        std::forward_list<TestStl>  aForwardListTestStl2;
        loadContainer4(iSerializer, aForwardListTestStl2, TestStl(300));

        std::cout << "        loadContainer0() : theolizer::ForwardListPointee<int>" << std::endl;
        theolizer::ForwardListPointee<int>  aForwardListInt0;
        loadContainer0(iSerializer, aForwardListInt0, 500);

        std::cout << "        loadContainer2() : theolizer::ForwardListPointee<int>" << std::endl;
        theolizer::ForwardListPointee<int>  aForwardListInt1;
        loadContainer2(iSerializer, aForwardListInt1, 600);

        std::cout << "        loadContainer4() : theolizer::ForwardListPointee<int>" << std::endl;
        theolizer::ForwardListPointee<int>  aForwardListInt2;
        loadContainer4(iSerializer, aForwardListInt2, 700);

        iSerializer.clearTracking();
    }
#endif

//      ---<<< std::set >>>---

    {
        std::cout << "        loadContainerSet3() : int" << std::endl;
        loadContainerSet3<tSerializer, std::set<int>, int>
        (
            iSerializer, 100
        );
    }
}

INSTANTIATION_ALL(loadSupportStl);

#endif  // DISABLE_SUPPORT_STL_TEST
