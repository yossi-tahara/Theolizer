//############################################################################
//      Theolizerライブラリの生配列用TheolizerVersion<>
//          他のTheolizerVersion<>と異なり、全ての配列用に１つのテンプレートで対応する
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

#if !defined(THEOLIZER_INTERNAL_VERSION_ARRAY_H)
#define THEOLIZER_INTERNAL_VERSION_ARRAY_H

//############################################################################
//      Begin
//############################################################################

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable:4127)
#endif

namespace theolizer
{
namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//      生配列の型定義
//          Array<型, 要素数>にて生配列の型になる
//          下記2つは同じ定義である。
//              theolizer::Array<int, 10, 20>   aIntArray;
//              int                             aIntArray[10][20];
//          このテンプレートで配列の参照を下記のように定義することができる。
//              theolizer::Array<int, 10>&      aInt10;
//              int                             (&aInt)[10];
// ***************************************************************************

//----------------------------------------------------------------------------
//      デバッグ・ツール
//----------------------------------------------------------------------------

struct ArrayDebugBase
{
    thread_local static unsigned mtIndent;
    ArrayDebugBase() { }
    ArrayDebugBase(unsigned)
    {
        mtIndent++;
    }
};

#if 1
template<typename... tTypes>
void outputArrayDebugImpl(tTypes...)
{
}

template<typename tFirst, typename... tTypes>
void outputArrayDebugImpl(tFirst const& iFirst, tTypes const&... iTypes)
{
    std::cout << std::string(ArrayDebugBase::mtIndent*2+2, ' ')
              << "[" << &iFirst << "] "
              << THEOLIZER_INTERNAL_TYPE_NAME(tFirst) << std::endl;
    if (sizeof...(tTypes))
    {
        return outputArrayDebugImpl(iTypes...);
    }
}

template<typename... tTypes>
unsigned outputArrayDebug(std::string const& iTitle, tTypes const&... iTypes)
{
    std::cout << std::string(ArrayDebugBase::mtIndent*2, ' ') << iTitle << std::endl;
    if (sizeof...(tTypes))
    {
        outputArrayDebugImpl(iTypes...);
    }
    return 0;
}

#define THEOLIZER_INTERNAL_DEBUG_ARRAY_BASE  : public ArrayDebugBase
#define THEOLIZER_INTERNAL_DEBUG_ARRAY(...)  ArrayDebugBase(outputArrayDebug(__VA_ARGS__)),
#define THEOLIZER_INTERNAL_DEBUG_ARRAY_END() (--ArrayDebugBase::mtIndent)
#else
#define THEOLIZER_INTERNAL_DEBUG_ARRAY_BASE
#define THEOLIZER_INTERNAL_DEBUG_ARRAY(...)
#define THEOLIZER_INTERNAL_DEBUG_ARRAY_END()
#endif

//----------------------------------------------------------------------------
//      配列初期化用のインデックスのリスト生成
//          IndicesHolder<0, 1, 2, ..., tIndices-1>を生成する
//----------------------------------------------------------------------------

template<std::size_t... tIndices>
struct IndicesHolder
{
    typedef IndicesHolder<tIndices..., sizeof...(tIndices)> Next;
};

template<std::size_t tIndexCount>
struct MakeIndices
{
    typedef typename MakeIndices<tIndexCount-1>::Type::Next Type;
};

template<>
struct MakeIndices<0>
{
    typedef IndicesHolder<> Type;
};

//----------------------------------------------------------------------------
//      ターゲット型取り出し
//          クラスやenum(参照がついていない)    TheolizerTarget
//          そうでないなら、                    tUnderlyingTypeの参照解除
//----------------------------------------------------------------------------

template<typename tUnderlyingType>
struct GetTheolizerTarget
{
    typedef typename tUnderlyingType::TheolizerTarget  Type;
};

template<typename tUnderlyingType>
struct GetTheolizerTarget< ::TheolizerNonKeepStep<tUnderlyingType, void> >
{
    typedef tUnderlyingType   Type;
};

//----------------------------------------------------------------------------
//      型とサイズの異なる配列から、コンストラクトされる配列型
//          型には、基本型::TheolizerVersion<>が入る。
//          基本型がKeep-stepなクラスやenumの場合、
//              基本型::TheolizerVersion<, N+1>から基本型::TheolizerVersion<, N>
//              をコントスラクト可能。
//          特記事項：
//              配列初期化子は、型変換後にリスト生成される。
//              つまり、ArrayBodyの場合、
//                  mData{tType(iData[tIndices])...}となっていることになる
//              そして、tType(これは基本型::TheolizerVersion<>)の同じバージョンからの
//                  コンストラクタへ与えられる。
//              それがtType&引数の場合、tType(iData[tIndices])が一時オブジェクトで
//                  あるため、gccではエラーになる。
//              そこで、version*.incの同じバージョンからのコンストラクタは
//                  tType const&引数のコピー・コンストラクタとする。
//              TheolizerVersion<>内の参照で問題が出そうな気がする。
//                  ターゲットからのコンストラクタはターゲット内部の要素への参照
//                  １つ１つがconstとなるため、問題が出る。
//                  しかし、同じバージョン同士の場合、TheolizerVersion<>全体への
//                  参照がconstになるだけであり、中身は非constのままなので、
//                  問題ない。
//----------------------------------------------------------------------------

//      ---<<< Keep-step(TheolizerVersion<>が指定されている) or Non-keep-stepの非最下位次元 >>>---

template<typename tElementType, std::size_t tDim>
struct ArrayBody
{
    tElementType    mElements[tDim];

    // 通常のコンストラクタ
    template<typename tArraySingleDimension, std::size_t... tIndices>
    ArrayBody(tArraySingleDimension& iData, IndicesHolder<tIndices...>) :
        mElements{iData[tIndices]...}
    { }

    // デフォルト・コンストラクタ
    //  バージョンアップで削除された要素をバージョンダウン時に用意する時使用される
    ArrayBody() : mElements{}
    { }

    // 次元数減少対応用(1つの要素を先頭へムーブする)
    ArrayBody(tElementType&& iData) :
        mElements{std::move(iData)}
    { }
};

//----------------------------------------------------------------------------
//      msvc 2015不具合対応
//          問題
//              EnableIf<>内で関数を直接呼び出すとfalseと決めつけてしまい、
//              適切にコンパイルできない。
//              →別クラス・テンプレートで間接的に処理すればOKだった。
//          他の対策も2016/02/11 12:30頃のコミットは実装している。
//              コンストラクタから、mArrayBodyの初期化子を呼ぶ際に
//              クラス・テンプレートを特殊化して分岐する方法。
//----------------------------------------------------------------------------

template<unsigned tVersionNo, class tTheolizerVersion, class tEnable=void>
struct Judge
{
    template<std::size_t... tDims>
    struct Dimensions
    {
        static const bool kFulfil=false;
        static const bool kHigher=false;
        static const bool kLower =false;
    };
};
template<unsigned tVersionNo, class tTheolizerVersion>
struct Judge<tVersionNo, tTheolizerVersion, typename tTheolizerVersion::TheolizerArrayVersion >
{
    template<std::size_t... tDims>
    struct Dimensions
    {
        static const bool kFulfil=tTheolizerVersion::isFulfil(tVersionNo, sizeof...(tDims)+1);
        static const bool kHigher=tTheolizerVersion::isHigher(tVersionNo, sizeof...(tDims)+1);
        static const bool kLower =tTheolizerVersion::isLower (tVersionNo, sizeof...(tDims)+1);
    };
};

//----------------------------------------------------------------------------
//      本体
//          tUnderlyingTypeは、
//              Non-keep-stepな型は 基本型&     (参照があるので注意)
//              Keep-stepな型は     基本型::TheolizerVersion<>
//          VersionTypeは、自分自身のTheolizerVersion<>を定義する
//          VersionElementは、配下のTheolizerVersion<>を定義する
//          VersionArrayは、配下のTheolizerVersion<>の配列を定義する
//----------------------------------------------------------------------------

template<typename tUnderlyingType, std::size_t... tDims>
struct ArrayManager
{
};

//      ---<<< 基本型のTheolizerVersion<>(プライマリー:Keep-step型用) >>>---

template<typename tUnderlyingType>
struct ArrayManager<tUnderlyingType>
{
    template<class tMidSerializer, unsigned tVersionNo>
    struct TheolizerVersion THEOLIZER_INTERNAL_DEBUG_ARRAY_BASE
    {
        typedef void TheolizerArrayUnderlying;  // Switcher<>分岐用
        typedef tUnderlyingType UnderlyingType; // Switcher<>処理用

        // Switcher<>分岐用
        struct Theolizer
        {
            static const bool kIsVersion=true;
            static const bool kIsArray=true;
        };

//      --- 型定義 ---

        // ユーザ定義配列の「基本型」
        typedef typename GetTheolizerTarget<tUnderlyingType>::Type  TheolizerTarget;

        // ユーザ定義配列用内部配列の型
        typedef theolizer::internal::CoveredArray<TheolizerTarget> TargetCoveredArray;

//      --- 実体定義 ---

        tUnderlyingType mElement;

//      --- アクセス用関数 ---

        // 中身へのアクセスを提供する
        typedef typename tUnderlyingType::AsElementOfArray  AsElementOfArray;
        AsElementOfArray&       getAccess()       {return mElement.getAccess();}
        AsElementOfArray const& getAccess() const {return mElement.getAccess();}

        // 自分自身の配列を返却する(save/loadArrayへ渡すためと次元数削減時に使用)
        tUnderlyingType& get() {return mElement;}

//      --- 構築用関数 ---

        // コンストラクタ(ターゲットから)
        TheolizerVersion(TargetCoveredArray& iElement) :
            THEOLIZER_INTERNAL_DEBUG_ARRAY(u8"KS :Target  ", *this, iElement)
            mElement(iElement.getElement())
        { THEOLIZER_INTERNAL_DEBUG_ARRAY_END(); }

        // コンストラクタ(次バージョンから)
        template<class tTheolizerVersion>
        TheolizerVersion(tTheolizerVersion& iElement) :
            THEOLIZER_INTERNAL_DEBUG_ARRAY(u8"KS :Next    ", *this, iElement)
            mElement(iElement.mElement)
        { THEOLIZER_INTERNAL_DEBUG_ARRAY_END(); }

        // コンストラクタ(同バージョンから)
        TheolizerVersion(TheolizerVersion& iElement) :
            THEOLIZER_INTERNAL_DEBUG_ARRAY(u8"KS :Same    ", *this, iElement)
            mElement(iElement.mElement)
        { THEOLIZER_INTERNAL_DEBUG_ARRAY_END(); }

        // ムーブ・コンストラクタ(同バージョンから)
        TheolizerVersion(TheolizerVersion&& iElement) :
            THEOLIZER_INTERNAL_DEBUG_ARRAY(u8"KS :Move    ", *this, iElement)
            mElement(std::move(iElement.mElement))
        { THEOLIZER_INTERNAL_DEBUG_ARRAY_END(); }

        // デフォルト・コンストラクタ
        TheolizerVersion() : 
            THEOLIZER_INTERNAL_DEBUG_ARRAY(u8"KS :Default ", *this)
            mElement()
        { THEOLIZER_INTERNAL_DEBUG_ARRAY_END(); }

//      --- コピー演算子 ---

#if 0
        TheolizerVersion& operator=(TheolizerVersion const& iRhs)
        {
            if (&iRhs != this) copyData(mElement, iRhs.mElement);
            return *this;
        }
#endif
    };
};

//      ---<<< 基本型のTheolizerVersion<>(Non-keep-step型用) >>>---

template<typename tPrimitiveType>
struct ArrayManager< ::TheolizerNonKeepStep<tPrimitiveType, void> >
{
    typedef ::TheolizerNonKeepStep<tPrimitiveType, void>   TheolizerNonKeepStep;

    template<class tMidSerializer, unsigned tVersionNo>
    struct TheolizerVersion THEOLIZER_INTERNAL_DEBUG_ARRAY_BASE
    {
        typedef void TheolizerArrayUnderlying;          // Switcher<>分岐用
        typedef TheolizerNonKeepStep UnderlyingType;    // Switcher<>処理用

        // Switcher<>分岐用
        struct Theolizer
        {
            static const bool kIsVersion=true;
            static const bool kIsArray=true;
        };

//      --- 型定義 ---

        // ユーザ定義配列の基本型
        typedef tPrimitiveType TheolizerTarget;

        // ユーザ定義配列用内部配列の型
        typedef theolizer::internal::CoveredArray<TheolizerTarget> TargetCoveredArray;

//      --- 実体定義 ---

        TheolizerNonKeepStep    mElement;   // 実体

//      --- アクセス用関数 ---

        // 中身へのアクセスを提供する
        typedef TheolizerNonKeepStep AsElementOfArray;
        AsElementOfArray&       getAccess()       {return mElement;}
        AsElementOfArray const& getAccess() const {return mElement;}

        // 自分自身の配列を返却する(save/loadArrayへ渡すためと次元数削減時に使用)
        TheolizerNonKeepStep& get() {return mElement;}

//      --- 構築用関数 ---

        // コンストラクタ(ターゲットから)
        TheolizerVersion(TargetCoveredArray& iElement) :
            THEOLIZER_INTERNAL_DEBUG_ARRAY(u8"NKS:Target  ", *this, iElement)
            mElement(iElement.getElement())
        { THEOLIZER_INTERNAL_DEBUG_ARRAY_END(); }

        // コンストラクタ(次バージョンから)
        template<class tTheolizerVersion>
        TheolizerVersion(tTheolizerVersion& iElement) :
            THEOLIZER_INTERNAL_DEBUG_ARRAY(u8"NKS:Next    ", *this, iElement)
            mElement(iElement.mElement)
        { THEOLIZER_INTERNAL_DEBUG_ARRAY_END(); }

        // コンストラクタ(同バージョンから)
        TheolizerVersion(TheolizerVersion& iElement) :
            THEOLIZER_INTERNAL_DEBUG_ARRAY(u8"NKS:Same    ", *this, iElement)
            mElement(iElement.mElement)
        { THEOLIZER_INTERNAL_DEBUG_ARRAY_END(); }

        // ムーブ・コンストラクタ(同バージョンから)
        TheolizerVersion(TheolizerVersion&& iElement) :
            THEOLIZER_INTERNAL_DEBUG_ARRAY(u8"NKS:Move    ", *this, iElement)
            mElement(std::move(iElement.mElement))
        { THEOLIZER_INTERNAL_DEBUG_ARRAY_END(); }

        // デフォルト・コンストラクタ
        TheolizerVersion() :
            THEOLIZER_INTERNAL_DEBUG_ARRAY(u8"NKS :Default ", *this)
            mElement()
        { THEOLIZER_INTERNAL_DEBUG_ARRAY_END(); }

//      --- コピー演算子 ---

#if 0
        TheolizerVersion& operator=(TheolizerVersion const& iRhs)
        {
            if (&iRhs != this) copyData(mElement, iRhs.mElement);
            return *this;
        }
#endif
    };
};

//      ---<<< 配列のTheolizerVersion<>定義 >>>---

template<typename tUnderlyingType, std::size_t tFirst, std::size_t... tDims>
struct ArrayManager<tUnderlyingType, tFirst, tDims...>
{
    template<class tMidSerializer, unsigned tVersionNo>
    struct TheolizerVersion THEOLIZER_INTERNAL_DEBUG_ARRAY_BASE
    {
        // Switcher<>分岐用
        typedef void TheolizerArrayVersion; 

        // Switcher<>分岐用
        struct Theolizer
        {
            static const bool kIsVersion=true;
            static const bool kIsArray=true;
        };

//      --- 型定義 ---

        // ユーザ定義配列の「基本型」
        typedef typename GetTheolizerTarget<tUnderlyingType>::Type  TheolizerTarget;

        // ユーザ定義生配列の型(TypeInfoList登録へ使用する)
        typedef theolizer::internal::Array<TheolizerTarget, tFirst, tDims...>
            TargetArray;

        // ユーザ定義配列用内部配列の型
        typedef theolizer::internal::CoveredArray<TheolizerTarget, tFirst, tDims...>
            TargetCoveredArray;

        // 要素の型(VersionElement)とその配列の型(VersionArray)
        typedef typename ArrayManager<tUnderlyingType, tDims...>::template
            TheolizerVersion<tMidSerializer, tVersionNo> VersionElement;
        typedef VersionElement VersionArray[tFirst];

        // アクセス用の型(AsElementOfArray:自分自身、ElementType:要素のAsElementOfArray)
        typedef TheolizerVersion AsElementOfArray;
        typedef typename ArrayManager<tUnderlyingType, tDims...>::template
            TheolizerVersion<tMidSerializer, tVersionNo>::AsElementOfArray ElementType;

//      --- 実体定義 ---

        // 配列保持領域
        ArrayBody<VersionElement, tFirst>  mArrayBody;

//      --- アクセス用関数 ---

        // 要素数を返却する
        static std::size_t size() {return tFirst;}

        // 指定要素へのアクセスを提供する
        ElementType&       operator[](std::size_t iIndex)
        {return mArrayBody.mElements[iIndex].getAccess();}

        ElementType const& operator[](std::size_t iIndex) const
        {return mArrayBody.mElements[iIndex].getAccess();}

        // 自分自身を返却する
        AsElementOfArray& getAccess()
        {return *this;}

        // 自分自身を配列として返却する(save/loadArrayへ渡すためと次元数削減時に使用)
        VersionArray& get() {return mArrayBody.mElements;}

//      --- 構築用関数 ---

        // コンストラクタ(ターゲットから：入り口の次元)
        TheolizerVersion(TargetArray& iTargetArray) : 
            THEOLIZER_INTERNAL_DEBUG_ARRAY(u8"---:Entry   ", *this, iTargetArray)
            mArrayBody(reinterpret_cast<TargetCoveredArray&>(iTargetArray).getElements(),
                       typename MakeIndices<tFirst>::Type())
        { THEOLIZER_INTERNAL_DEBUG_ARRAY_END(); }

        // コンストラクタ(ターゲットから：内側の次元)
        TheolizerVersion(TargetCoveredArray& iTargetCoveredArray) : 
            THEOLIZER_INTERNAL_DEBUG_ARRAY(u8"---:Inner   ", *this, iTargetCoveredArray)
            mArrayBody(iTargetCoveredArray.getElements(),
                       typename MakeIndices<tFirst>::Type())
        { THEOLIZER_INTERNAL_DEBUG_ARRAY_END(); }

        // コンストラクタ(同バージョンから)
        TheolizerVersion(TheolizerVersion& iTheolizerVersion) :
            THEOLIZER_INTERNAL_DEBUG_ARRAY(u8"---:Same    ", *this, iTheolizerVersion)
            mArrayBody(iTheolizerVersion.mArrayBody)
        { THEOLIZER_INTERNAL_DEBUG_ARRAY_END(); }

        // ムーブ・コンストラクタ(同バージョンから)
        TheolizerVersion(TheolizerVersion&& iTheolizerVersion) :
            THEOLIZER_INTERNAL_DEBUG_ARRAY(u8"---:Move    ", *this, iTheolizerVersion)
            mArrayBody(std::move(iTheolizerVersion.mArrayBody))
        { THEOLIZER_INTERNAL_DEBUG_ARRAY_END(); }

        // デフォルト・コンストラクタ
        TheolizerVersion() :
            THEOLIZER_INTERNAL_DEBUG_ARRAY(u8"---:Default ", *this)
            mArrayBody()
        { THEOLIZER_INTERNAL_DEBUG_ARRAY_END(); }

//      --- 次元数／要素数変更対応用関数定義 ---

        // 指定バージョン、かつ、指定次元であることを判定する
        constexpr static bool isFulfil(unsigned iVersionNo, std::size_t iDimsCount)
        {
            return ((iVersionNo == tVersionNo) && (iDimsCount == (sizeof...(tDims)+1)));
        }

        // 指定バージョン、かつ、指定次元より大きい次元であることを判定する
        constexpr static bool isHigher(unsigned iVersionNo, std::size_t iDimsCount)
        {
            return ((iVersionNo == tVersionNo) && (iDimsCount < (sizeof...(tDims)+1)));
        }

        // 指定バージョン、かつ、指定次元より小さい次元であることを判定する
        constexpr static bool isLower(unsigned iVersionNo, std::size_t iDimsCount)
        {
            return ((iVersionNo == tVersionNo) && (iDimsCount > (sizeof...(tDims)+1)));
        }

        // 前バージョンとの共通する要素数計算(前バージョンのコンストラクタから呼ばれる)
        constexpr static const std::size_t getInitCount(
            std::size_t iDstFirst, std::size_t iDstDimsCount)
        {
            return ((iDstDimsCount+1) <= ((sizeof...(tDims))+1))?
                ((iDstFirst<tFirst)?iDstFirst:tFirst)
                :
                1;
        }

        // 指定の(低)次元のTheolizerVersionを取り出す(クラス外で部分特殊化している)
        template<class tTheolizerVersion, class tEnable=void>
        struct GetTheolizerVersion
        {
            typedef VersionArray VersionArray_t;
            static VersionArray_t& getLower(TheolizerVersion& iTheolizerVersion)
            {
                return iTheolizerVersion.get();
            }
        };

        // 注意事項
        //  下記ヘルパー・クラスを作り、記述を簡略化しようとしたところ、
        //  MSVC 2015で"fatal error C1001"が発生する。
        #if 0
        // Judgeクラス展開用ヘルパー
        template<class tTheolizerVersion2>
        using JudgeVersion=typename Judge<tVersionNo+1, tTheolizerVersion2>::
            template Dimensions<tDims...>;
        #endif

        // コンストラクタ(次バージョンの同一次元から)
        template
        <
            class tTheolizerVersion,
            THEOLIZER_INTERNAL_OVERLOAD
            (
                (Judge<tVersionNo+1, tTheolizerVersion>::template Dimensions<tDims...>::kFulfil)
            )
        >
        TheolizerVersion(tTheolizerVersion& iTheolizerVersion) :
            THEOLIZER_INTERNAL_DEBUG_ARRAY(u8"---:Next-same ", *this, iTheolizerVersion)
            mArrayBody(iTheolizerVersion.mArrayBody.mElements,
                typename MakeIndices<
                    tTheolizerVersion::getInitCount(tFirst, sizeof...(tDims))
                >::Type())
        { THEOLIZER_INTERNAL_DEBUG_ARRAY_END(); }

        // コンストラクタ(次バージョンの上位次元から)
        template
        <
            class tTheolizerVersion,
            THEOLIZER_INTERNAL_OVERLOAD
            (
                (Judge<tVersionNo+1, tTheolizerVersion>::template Dimensions<tDims...>::kHigher)
            )
        >
        TheolizerVersion(tTheolizerVersion& iTheolizerVersion) :
            THEOLIZER_INTERNAL_DEBUG_ARRAY(u8"---:Next-upper", *this, iTheolizerVersion)
            mArrayBody(tTheolizerVersion::template
                GetTheolizerVersion<TheolizerVersion>::getLower(iTheolizerVersion),
                //↑現次元と同じ次元の次バージョンのTheolizerVersion<>を取り出す
                typename MakeIndices<
                    tTheolizerVersion::getInitCount(tFirst, sizeof...(tDims))
                >::Type())
        { THEOLIZER_INTERNAL_DEBUG_ARRAY_END(); }

        // コンストラクタ(次バージョンの下位次元から)
        template
        <
            class tTheolizerVersion,
            THEOLIZER_INTERNAL_OVERLOAD
            (
                (Judge<tVersionNo+1, tTheolizerVersion>::template Dimensions<tDims...>::kLower)
            )
        >
        TheolizerVersion(tTheolizerVersion& iTheolizerVersion) :
            THEOLIZER_INTERNAL_DEBUG_ARRAY(u8"---:Next-lower", *this, iTheolizerVersion)
            mArrayBody(VersionElement(iTheolizerVersion))
        { THEOLIZER_INTERNAL_DEBUG_ARRAY_END(); }

//      --- コピー演算子 ---

#if 0
        TheolizerVersion& operator=(TheolizerVersion const& iRhs)
        {
            if (&iRhs != this) copyData(mArrayBody.mElements, iRhs.mArrayBody.mElements);
            return *this;
        }
#endif
    };
};

//      ---<<< 指定(低)次元のTheolizerVersionを取り出すための特殊化 >>>---
//          このArrayManagerは次バージョン側である

template<typename tUnderlyingType, std::size_t tFirst, std::size_t... tDims>
template<class tMidSerializer, unsigned tVersionNo>
template<class tTheolizerVersion>
struct ArrayManager<tUnderlyingType, tFirst, tDims...>::
        TheolizerVersion<tMidSerializer, tVersionNo>::
            GetTheolizerVersion<tTheolizerVersion, typename std::enable_if<
                Judge<tVersionNo-1, tTheolizerVersion, void>::template
                    Dimensions<tDims...>::kLower>::type>
{
    typedef typename ArrayManager<tUnderlyingType, tDims...>::template
        TheolizerVersion<tMidSerializer, tVersionNo> LowerTheolizerVersion;
    typedef typename LowerTheolizerVersion::template
        GetTheolizerVersion<tTheolizerVersion>::VersionArray_t VersionArray_t;
    static VersionArray_t& getLower(TheolizerVersion<tMidSerializer,tVersionNo>& iTheolizerVersion)
    {
        return LowerTheolizerVersion::template
            GetTheolizerVersion<tTheolizerVersion>::
                getLower(iTheolizerVersion.mArrayBody.mElements[0]);
    }
};

//############################################################################
//      End
//############################################################################

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal
}   // namespace theolizer

// ***************************************************************************
//          警告抑止解除
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#endif  // THEOLIZER_INTERNAL_VERSION_ARRAY_H
