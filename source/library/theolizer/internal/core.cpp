//############################################################################
//      Theolizerライブラリのコア部
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

#define THEOLIZER_INTERNAL_EXCLUDE_VERSION_H

//############################################################################
//      インクルード
//############################################################################

#include "avoid-trouble.h"

#include "internal.h"
#include "core.h"

//############################################################################
//      Begin
//############################################################################

//############################################################################
//      バージョン対応用定義群
//############################################################################

// ***************************************************************************
//      TheolizerNonKeepStepクラス
//          プリミティブ    部分特殊化
//          その他          プライマリ
//              ポインタ
//              非侵入型
// ***************************************************************************

//----------------------------------------------------------------------------
//      TheolizerNonKeepStepプリミティブ用部分特殊化の実体部
//----------------------------------------------------------------------------

//      ---<<< ターゲットからのコンストラクタ >>>---

template<typename tPrimitiveType>
TheolizerNonKeepStep
<
    tPrimitiveType,
    theolizer::internal::EnableIf<theolizer::internal::IsPrimitive<tPrimitiveType>::value>
>::TheolizerNonKeepStep(tPrimitiveType& iTarget) :
    Base(iTarget),
    mValue(Base::getData()),
    mBackup(),
    mTarget(&iTarget),
    mNextPtr(nullptr),
    mDoSucceed(false),
    mIsChanged(false),
    mUpVersionCount(0)
{ }

//      ---<<< 次からのコンストラクタ >>>---

template<typename tPrimitiveType>
TheolizerNonKeepStep
<
    tPrimitiveType,
    theolizer::internal::EnableIf<theolizer::internal::IsPrimitive<tPrimitiveType>::value>
>::TheolizerNonKeepStep(This& iNext) :
    Base(iNext.mValue),
    mValue(Base::getData()),
    mBackup(),
    mTarget(iNext.mTarget),
    mNextPtr(&iNext),
    mDoSucceed(false),
    mIsChanged(false),
    mUpVersionCount(0)
{ }

//      ----<<< ムーブ・コンストラクタ(配列処理用) >>>---

template<typename tPrimitiveType>
TheolizerNonKeepStep
<
    tPrimitiveType,
    theolizer::internal::EnableIf<theolizer::internal::IsPrimitive<tPrimitiveType>::value>
>::TheolizerNonKeepStep(This&& iNext) :
    Base(std::move(iNext.mValue)),
    mValue(Base::getData()),
    mBackup(),
    mTarget(iNext.mTarget),
    mNextPtr(iNext.mNextPtr),
    mDoSucceed(false),
    mIsChanged(false),
    mUpVersionCount(0)
{ }

//      ---<<< デフォルト・コンストラクタ(次で削除されていた場合) >>>---

template<typename tPrimitiveType>
TheolizerNonKeepStep
<
    tPrimitiveType,
    theolizer::internal::EnableIf<theolizer::internal::IsPrimitive<tPrimitiveType>::value>
>::TheolizerNonKeepStep() :
    Base(),
    mValue(Base::getData()),
    mBackup(),
    mTarget(nullptr),
    mNextPtr(nullptr),
    mDoSucceed(false),
    mIsChanged(false),
    mUpVersionCount(0)
{ }

//      ---<<< 当プリミティブを含むクラスやその上位クラスのupVersionからの通常設定 >>>---

template<typename tPrimitiveType>
TheolizerNonKeepStep<tPrimitiveType>& TheolizerNonKeepStep
<
    tPrimitiveType,
    theolizer::internal::EnableIf<theolizer::internal::IsPrimitive<tPrimitiveType>::value>
>::operator=(tPrimitiveType iRhs)
{
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "TheolizerNonKeepStep<" << THEOLIZER_INTERNAL_TYPE_NAME(tPrimitiveType)
              << ">::operator=(" << iRhs << "); " << (void*)mTarget << std::endl;
    std::cout << "    mUpVersionCount    =" << mUpVersionCount << "" << std::endl;
    std::cout << "    getUpVersionCount()=" << theolizer::internal::getUpVersionCount() << ""
          << std::endl;
));

    unsigned aUpVersionCount=theolizer::internal::getUpVersionCount();

    // バージョン・ダウン中
    if (aUpVersionCount == 0)
    {
        mValue=iRhs;
    }

    // 回復後のバージョン・アップ中、かつ、有効なset呼び出し無し
    else if (!mDoSucceed)
    {
        // 最初の呼び出しなら、バックアップする
        if (mUpVersionCount == 0)
        {
            mBackup=mValue;
            mUpVersionCount=aUpVersionCount;
        }

        // 最初の呼び出しと同じupVersion()からの呼び出し
        if (mUpVersionCount == aUpVersionCount)
        {
            mValue=iRhs;
            mIsChanged=true;
        }
    }

THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "    mValue               =" << mValue << "" << std::endl;
    std::cout << "    mIsChanged           =" << mIsChanged << "" << std::endl;
    std::cout << "    mDoSucceed           =" << mDoSucceed << "" << std::endl;
));
    return *this;
}

//      ---<<< 強制引継ぎ設定（他の変数から引き継ぐ） >>>---

template<typename tPrimitiveType>
void TheolizerNonKeepStep
<
    tPrimitiveType,
    theolizer::internal::EnableIf<theolizer::internal::IsPrimitive<tPrimitiveType>::value>
>::set(tPrimitiveType iValue, bool iDoSucceed)
{
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "TheolizerNonKeepStep<" << THEOLIZER_INTERNAL_TYPE_NAME(tPrimitiveType)
          << ">::set(" << iValue << ", " << iDoSucceed << "); " << (void*)mTarget << std::endl;
    std::cout << "    mUpVersionCount    =" << mUpVersionCount << "" << std::endl;
    std::cout << "    getUpVersionCount()=" << theolizer::internal::getUpVersionCount() << ""
          << std::endl;
));

    unsigned aUpVersionCount=theolizer::internal::getUpVersionCount();

    // バージョン・ダウン中
    if (aUpVersionCount == 0)
    {   // 使用禁止
        THEOLIZER_INTERNAL_ABORT("Do not call <member>.set() in downVersion().");
    }

    // 回復後のバージョン・アップ中、かつ、有効なset呼び出し
    else if (iDoSucceed)
    {
        // 最初の呼び出しなら、バックアップする
        if (mUpVersionCount == 0)
        {
            mBackup=mValue;
            mUpVersionCount=aUpVersionCount;
        }

        // 最初の呼び出しと同じupVersion()からの呼び出し
        if (mUpVersionCount == aUpVersionCount)
        {
            mValue=iValue;
            mDoSucceed=true;
        }
    }
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "    mValue               =" << mValue << "" << std::endl;
    std::cout << "    mIsChanged           =" << mIsChanged << "" << std::endl;
    std::cout << "    mDoSucceed           =" << mDoSucceed << "" << std::endl;
));
}

//      ---<<< 前バージョンのデストラクタから呼ばれる引継ぎ処理 >>>---

template<typename tPrimitiveType>
void TheolizerNonKeepStep
<
    tPrimitiveType,
    theolizer::internal::EnableIf<theolizer::internal::IsPrimitive<tPrimitiveType>::value>
>::succeed(tPrimitiveType iValue, bool iDoSucceed)
{
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "TheolizerNonKeepStep<" << THEOLIZER_INTERNAL_TYPE_NAME(tPrimitiveType)
              << ">::succeed(" << iValue << "); " << (void*)mTarget << std::endl;
    std::cout << "    mIsChanged =" << mIsChanged << "" << std::endl;
));

    // setされていない時
    if (!mDoSucceed)
    {   // 引継ぎ無し
        if (!iDoSucceed)
        {   // 変更されていたらリストアする
            if (mIsChanged)
            {
                mValue=mBackup;
            }
        }

        // 引継ぎ有り
        else
        {
            mDoSucceed=true;

            // 変更されていないなら、引き継ぐ
            if (!mIsChanged)
            {
                mValue=iValue;
            }
        }
    }
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "    mValue               =" << mValue << "" << std::endl;
    std::cout << "    mIsChanged           =" << mIsChanged << "" << std::endl;
    std::cout << "    mDoSucceed           =" << mDoSucceed << "" << std::endl;
));
}

//      ---<<< デストラクタ(次バージョンへの引継ぎ実行) >>>---

template<typename tPrimitiveType>
TheolizerNonKeepStep
<
    tPrimitiveType,
    theolizer::internal::EnableIf<theolizer::internal::IsPrimitive<tPrimitiveType>::value>
>::~TheolizerNonKeepStep()
{
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "TheolizerNonKeepStep<" << THEOLIZER_INTERNAL_TYPE_NAME(tPrimitiveType)
              << ">::~TheolizerNonKeepStep(); " << (void*)mTarget << std::endl;
    std::cout << "    mValue               =" << mValue << "" << std::endl;
    std::cout << "    mIsChanged           =" << mIsChanged << "" << std::endl;
    std::cout << "    mDoSucceed           =" << mDoSucceed << "" << std::endl;
));

    // 引継ぎ処理
    if (mNextPtr)
    {
THEOLIZER_INTERNAL_VER_DEBUG((
    std::cout << "    mNextPtr->succeed(" << mValue << ");" << std::endl;
));
        mNextPtr->succeed(mValue, mDoSucceed);
    }
    else if (mTarget && mDoSucceed)
    {
THEOLIZER_INTERNAL_VER_DEBUG((std::cout << "    *mTarget=" << mValue << ";" << std::endl;));
        *mTarget=mValue;
    }
}

//----------------------------------------------------------------------------
//      明示的実体化
//----------------------------------------------------------------------------

#define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSimbol)                \
    template struct THEOLIZER_INTERNAL_DLL TheolizerNonKeepStep<dType>;
#include "primitive.inc"

//############################################################################
//      デバッグ用
//############################################################################

namespace theolizer
{
namespace internal
{

thread_local unsigned ArrayDebugBase::mtIndent=0;

}   // namespace internal
}   // namespace theolizer
