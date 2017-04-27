//############################################################################
//      範囲ベースfor拡張
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
//          RBForAdaptor：範囲ベースfor専用の便利ツール
//              コンテナやレンジを受け取り、指定の「レンジ」へ設定して保持する。
//              begin(), end()はRangeReferencerを返却する
//          RBForReferencer:範囲ベースfor専用のiterator(かなり特殊）
//              RBForAdaptorへの参照のみを保持する
//                  operator*()は「レンジ」を返却
//                  operator++()はRBForAdaptorのdrop_front()を呼ぶ
//                  operator!=()はRBForAdaptorのempty()を呼ぶ
//          特記事項
//              RBForAdaptor::end()が返却したインスタンスのメンバ関数は呼び出し禁止。
//
//      注意事項：名前空間internalのアイテムは直接使わないこと
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

#define THEOLIZER_INCREMENT_OPERATOR(dThis)                                 \
    dThis& operator++() {                                                   \
        drop_front();                                                       \
        return *this;                                                       \
    }                                                                       \
    dThis operator++(int) {                                                 \
        dThis ret(*this);                                                   \
        drop_front();                                                       \
        return ret;                                                         \
    }

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
//      基本的なレンジ・クラス(公開クラス)
/*!
    @todo   T.B.D.
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
/*!
    @todo   T.B.D.
*/
    BasicRange(tIterator&& iBegin, tIterator&& iEnd) :
        mBegin(std::forward<tIterator>(iBegin)),
        mEnd  (std::forward<tIterator>(iEnd))
    { }
/*!
    @todo   T.B.D.
*/
    void drop_front()
    {
        if (empty())
        {
    throw std::out_of_range("BasicRange::drop_front()");
        }
        ++mBegin;
    }
/*!
    @todo   T.B.D.
*/
    ValueType& front()      {return *mBegin;}
/*!
    @todo   T.B.D.
*/
    bool empty()      const {return mBegin == mEnd;}
/*!
    @todo   T.B.D.
*/
    tIterator begin() const {return mBegin;}
/*!
    @todo   T.B.D.
*/
    tIterator end()   const {return mEnd;}

    // イテレータ的I/F
/*!
    @todo   T.B.D.
*/
    tIterator operator->()  {return mBegin;}
/*!
    @todo   T.B.D.
*/
    ValueType& operator*()  {return front();}
/*!
    @todo   T.B.D.
*/
    THEOLIZER_INCREMENT_OPERATOR(BasicRange)
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

    // イテレータ的I/F
    THEOLIZER_INCREMENT_OPERATOR(Indexer)
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
//      汎用
/*!
    @todo   T.B.D.
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
//      分割専用
/*!
    @todo   T.B.D.
*/
// ---------------------------------------------------------------------------

template<class tContainer, class tNextFunc=internal::NopFunctor>
auto getRBForSeparator(tContainer&& iContainer, tNextFunc&& iNextFunc=internal::NopFunctor()) ->
    typename internal::MakeRBForAdaptor<BasicRange, tContainer, tNextFunc>::Type
{
    return getRBForAdaptor<BasicRange>(iContainer, iNextFunc);
}

// ---------------------------------------------------------------------------
//      分割とインデックス用
/*!
    @todo   T.B.D.
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
