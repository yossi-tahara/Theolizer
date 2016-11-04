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

#include <deque>

#include "internal.h"
#include "core.h"

//############################################################################
//      Begin
//############################################################################

namespace theolizer
{
namespace internal
{

//############################################################################
//      エラー報告サービス群
//############################################################################

// ***************************************************************************
//      シリアライズ情報管理領域
// ***************************************************************************

//----------------------------------------------------------------------------
//      処理中インスタンス情報
//----------------------------------------------------------------------------

struct InstanceInfo
{
    char const* mName;                  // メンバ名(配列の時nullptr)
    size_t      mIndex;                 // 配列時のインデックス
    bool        mIsPointer;             // ポインタの時true
    char const* mFileName;              // 定義場所のファイル名と行番号
    size_t      mLineNo;                // 　無効の時、mFileName == nullptr

    InstanceInfo
    (
        char const* iName,
        size_t      iIndex,
        bool        iIsPointer,
        char const* iFileName,
        size_t      iLineNo
    ) : mName(iName),
        mIndex(iIndex),
        mIsPointer(iIsPointer),
        mFileName(iFileName),
        mLineNo(iLineNo)
    { }
};

//----------------------------------------------------------------------------
//      ヘッダ依存回避部
//----------------------------------------------------------------------------

struct AdditionalInfo::Indep
{
    std::deque<InstanceInfo>    mInstances; // 処理中インスタンス情報パス
};

//----------------------------------------------------------------------------
//      コンストラクタ
//----------------------------------------------------------------------------

AdditionalInfo::AdditionalInfo(BaseSerializer& iBaseSerializer) noexcept :
        mIndep(std::unique_ptr<Indep>(new Indep())),
        mBaseSerializer(iBaseSerializer)
{
}

//----------------------------------------------------------------------------
//      デストラクタ
//----------------------------------------------------------------------------

AdditionalInfo::~AdditionalInfo() noexcept
{
}

//----------------------------------------------------------------------------
//      インスタンス情報登録
//----------------------------------------------------------------------------

void AdditionalInfo::pushInstanceInfo
(
    char const* iName,
    size_t      iIndex,
    bool        iIsPointer,
    char const* iFileName,
    size_t      iLineNo
)
{
    mIndep->mInstances.push_back(
        InstanceInfo(iName, iIndex, iIsPointer, iFileName, iLineNo));
}

//----------------------------------------------------------------------------
//      インスタンス情報解除
//----------------------------------------------------------------------------

void AdditionalInfo::popInstanceInfo()
{
    mIndep->mInstances.pop_back();
}

//----------------------------------------------------------------------------
//      メッセージ取り出し
//----------------------------------------------------------------------------

u8string AdditionalInfo::getString()
{
    std::string aInstancePath;

    bool aFirst=true;
    bool aIsPointer=false;
    for (auto&& instance : mIndep->mInstances)
    {
        if (aFirst)
        {
            aFirst=false;
        }
        else
        {
            if (aIsPointer)
            {
                aInstancePath.append("->");
            }
            else
            {
                aInstancePath.push_back('.');
            }
        }

        if (instance.mName)
        {
            aInstancePath.append(instance.mName);
        }
        else
        {
            aInstancePath.push_back('[');
            aInstancePath.append(std::to_string(instance.mIndex));
            aInstancePath.push_back(']');
        }

        if (instance.mFileName)
        {
            aInstancePath.push_back('{');
            aInstancePath.append(instance.mFileName);
            aInstancePath.push_back('(');
            aInstancePath.append(std::to_string(instance.mLineNo));
            aInstancePath.push_back(')');
            aInstancePath.push_back('}');
        }

        aIsPointer=instance.mIsPointer;
    }

    return aInstancePath;
}

//----------------------------------------------------------------------------
//      エラー情報伝達
//----------------------------------------------------------------------------

void AdditionalInfo::setError(ErrorInfo const& iErrorInfo, bool iConstructor)
{
    mBaseSerializer.setError(iErrorInfo, iConstructor);
}

}   // namespace internal
}   // namespace theolizer
