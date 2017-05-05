//############################################################################
/*!
    @brief      範囲ベースfor拡張
    @details    ①RBForAdaptor：範囲ベースfor専用の便利ツール
                　　コンテナやレンジを受け取り、指定の「レンジ」へ設定して保持する。<br>
                　　begin(), end()はRangeReferencerを返却する<br>
                ②RBForReferencer:範囲ベースfor専用のiterator(かなり特殊）<br>
                　　RBForAdaptorへの参照のみを保持する<br>
                　　　　operator*()は「レンジ」を返却<br>
                　　　　operator++()はRBForAdaptorのdrop_front()を呼ぶ<br>
                　　　　operator!=()はRBForAdaptorのempty()を呼ぶ<br>
                ※特記事項<br>
                　　RBForAdaptor::end()が返却したインスタンスのメンバ関数は呼び出し禁止。<br>
                ※注意事項：名前空間internalのアイテムは直接使わないこと<br>
    @ingroup    TheolizerLib
    @file       rbfor.h
    @author     Yoshinori Tahara
    @date       2017/07/14 Created
*/
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

#if !defined(THEOLIZER_INTERNAL_RBFOR_H)
#define THEOLIZER_INTERNAL_RBFOR_H

#include <stdexcept>
#include <utility>

//############################################################################
//      Begin
//############################################################################

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable:4100)
#endif

namespace theolizer
{
namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

//############################################################################
//      範囲ベースfor拡張(ループ分割とインデックス番号他)
//############################################################################

// ***************************************************************************
//      RBForReferencer
// ***************************************************************************

template<class tRBForAdaptor>
class RBForReferencer
{
    typedef typename tRBForAdaptor::Range  Range;
    tRBForAdaptor& mRBForAdaptor;
public:
    RBForReferencer(tRBForAdaptor& iRBForAdaptor) : mRBForAdaptor(iRBForAdaptor)
    { }
    Range& operator*()
    {
        return mRBForAdaptor;
    }
    void operator++()
    {
        mRBForAdaptor.drop_front();
    }
    bool operator!=(RBForReferencer const& iRhs) const
    {
        return !mRBForAdaptor.empty();
    }
};

// ***************************************************************************
//      RBForAdaptor
// ***************************************************************************

template<class tRange, class tIterator, class tNextFunc>
class RBForAdaptor : private tRange
{
    tNextFunc   mNextFunc;  // drop_front時に呼び出す関数
public:
    // コンストラクタ
    RBForAdaptor(tIterator&& iBegin, tIterator&& iEnd, tNextFunc const& iNextFunc) :
        tRange(std::forward<tIterator>(iBegin), std::forward<tIterator>(iEnd)),
        mNextFunc(iNextFunc)
    { }
private:
    // RBForReferencer I/F
    template<class> friend class internal::RBForReferencer;
    typedef tRange  Range;
    typedef internal::RBForReferencer<RBForAdaptor>   RBForReferencer;
    void drop_front()
    {
        mNextFunc(tRange::front());
        tRange::drop_front();
    }
public:
    // レンジ取り出し
    tRange& getRange()      {return *this;}

    // 範囲ベースfor I/F(プログラマーは使用禁止)
    RBForReferencer begin() {return RBForReferencer(*this);}
    RBForReferencer end()   {return RBForReferencer(*this);}
};

// ***************************************************************************
//      ユーティリティ
// ***************************************************************************

// ---------------------------------------------------------------------------
//      デフォルトのtNextFunc処理
// ---------------------------------------------------------------------------

struct NopFunctor
{
    void operator()(...) { }
};

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

// ---------------------------------------------------------------------------
/*!
@brief      範囲ベースfor用のRageを管理する基底クラス
@details    for(auto&& aLoop : getRBForIndexer(コンテナ))
            とした時のaLoopの型の基底クラスである。
*/
// ---------------------------------------------------------------------------

template<class tIterator>
class BasicRange
{
    typedef typename
        std::remove_reference<decltype(*(std::declval<tIterator>()))>::type ValueType;

    tIterator   mBegin;
    tIterator   mEnd;
public:
    //! @ingroup RbFor
    //! コンストラクタ（範囲開始と終了を設定する）
    BasicRange(tIterator&& iBegin, tIterator&& iEnd) :
        mBegin(std::forward<tIterator>(iBegin)),
        mEnd  (std::forward<tIterator>(iEnd))
    { }

    //! @ingroup RbFor
    //! １つ次へ進める
    void drop_front()
    {
        if (empty())
        {
    throw std::out_of_range("BasicRange::drop_front()");
        }
        ++mBegin;
    }

    //! @ingroup RbFor
    //! 先頭要素を取り出す
    ValueType& front()      {return *mBegin;}

    //! @ingroup RbFor
    //! 開始位置と終了位置が一致している時empty
    bool empty()      const {return mBegin == mEnd;}

    //! @ingroup RbFor
    //! 開始位置取り出し(範囲ベースfor専用)
    tIterator begin() const {return mBegin;}

    //! @ingroup RbFor
    //! 終了位置取り出し(範囲ベースfor専用)
    tIterator end()   const {return mEnd;}

    //! @ingroup RbFor
    //! ポイント先メンバ取り出し(イテレータ的I/F)
    tIterator operator->()  {return mBegin;}

    //! @ingroup RbFor
    //! ポイント先取り出し(イテレータ的I/F)
    ValueType& operator*()  {return front();}

    //! @ingroup RbFor
    //! 前置インクリメント(イテレータ的I/F)
    BasicRange& operator++()
    {
        drop_front();
        return *this;
    }

    //! @ingroup RbFor
    //! 後置インクリメント(イテレータ的I/F)
    BasicRange operator++(int)
    {
        BasicRange ret(*this);
        drop_front();
        return ret;
    }
};

// ---------------------------------------------------------------------------
//      インデックス機能クラス
// ---------------------------------------------------------------------------

namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

template<class tIterator>
class Indexer : public BasicRange<tIterator>
{
    std::size_t mIndex;
public:
    Indexer(tIterator&& iBegin, tIterator&& iEnd) :
        BasicRange<tIterator>
        (
            std::forward<tIterator>(iBegin),
            std::forward<tIterator>(iEnd)
        ),
        mIndex(0)
    { }
    void drop_front()
    {
        ++mIndex;
        BasicRange<tIterator>::drop_front();
    }
    std::size_t getIndex() const    {return mIndex;}

    //! @ingroup RbFor
    //! 前置インクリメント(イテレータ的I/F)
    Indexer& operator++()
    {
        drop_front();
        return *this;
    }

    //! @ingroup RbFor
    //! 後置インクリメント(イテレータ的I/F)
    Indexer operator++(int)
    {
        Indexer ret(*this);
        drop_front();
        return ret;
    }
};

// ---------------------------------------------------------------------------
//      RBForAdaptor生成ヘルパ
// ---------------------------------------------------------------------------

template
<
    template<class tIterator> class tRange,
    class tContainer,
    class tNextFunc=internal::NopFunctor
>
class MakeRBForAdaptor
{
    typedef decltype(std::begin(std::declval<tContainer>()))    Iterator;
    typedef typename std::remove_reference<tNextFunc>::type     NextFunc;
public:
    typedef RBForAdaptor<tRange<Iterator>, Iterator, NextFunc>  Type;
};

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

// ***************************************************************************
//      API関数
// ***************************************************************************

// ---------------------------------------------------------------------------
/*!
    @brief      汎用の範囲ベースfor用レンジ生成
*/
// ---------------------------------------------------------------------------

template
<
    template<class tIterator> class tRange,
    class tContainer,
    class tNextFunc=internal::NopFunctor
>
auto getRBForAdaptor(tContainer&& iContainer, tNextFunc&& iNextFunc=internal::NopFunctor()) ->
    typename internal::MakeRBForAdaptor<tRange, tContainer, tNextFunc>::Type
{
    typedef decltype(getRBForAdaptor<tRange>(iContainer, iNextFunc))   RBForAdaptor;
    return RBForAdaptor(std::begin(iContainer), std::end(iContainer), iNextFunc);
}

// ---------------------------------------------------------------------------
/*!
    @brief      範囲ベースforを分割できるようにするためのレンジ生成
*/
// ---------------------------------------------------------------------------

template<class tContainer, class tNextFunc=internal::NopFunctor>
auto getRBForSeparator(tContainer&& iContainer, tNextFunc&& iNextFunc=internal::NopFunctor()) ->
    typename internal::MakeRBForAdaptor<BasicRange, tContainer, tNextFunc>::Type
{
    return getRBForAdaptor<BasicRange>(iContainer, iNextFunc);
}

// ---------------------------------------------------------------------------
/*!
    @brief      範囲ベースforを分割できるようにし、かつ、インデックス番号用レンジ生成
*/
// ---------------------------------------------------------------------------

template<class tContainer, class tNextFunc=internal::NopFunctor>
auto getRBForIndexer(tContainer&& iContainer, tNextFunc&& iNextFunc=internal::NopFunctor()) ->
    typename internal::MakeRBForAdaptor<internal::Indexer, tContainer, tNextFunc>::Type
{
    return getRBForAdaptor<internal::Indexer>(iContainer, iNextFunc);
}

//############################################################################
//      End
//############################################################################

}   // namespace theolizer

// ***************************************************************************
//          警告抑止解除
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#endif  // THEOLIZER_INTERNAL_BASE_H
