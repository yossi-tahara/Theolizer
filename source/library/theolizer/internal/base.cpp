//############################################################################
//      Theolizer全体のベース部
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
//      インクルード
//############################################################################

#include "avoid-trouble.h"

#include "internal.h"
#include "base.h"

//############################################################################
//      Begin
//############################################################################

namespace theolizer
{
namespace internal
{

// ***************************************************************************
//      型情報取得(getNameByTypeId)
// ***************************************************************************

// ---------------------------------------------------------------------------
//      GCC用
// ---------------------------------------------------------------------------

#if defined(__GNUC__)

std::string getNameByTypeId(std::type_index aTypeIndex)
{
    char*   aName;
    int status = 0;
    aName = abi::__cxa_demangle(aTypeIndex.name(), 0, 0, &status);
    std::string ret(aName);
    std::free(aName);
    return ret;
}
// ---------------------------------------------------------------------------
//      その他のコンパイラ用
// ---------------------------------------------------------------------------

#else

std::string getNameByTypeId(std::type_index aTypeIndex)
{
    return aTypeIndex.name();
}

#endif

}   // namespace internal

//############################################################################
//      バージョン管理情報
//############################################################################

std::string getVersionString()
{
    std::string ret(THEOLIZER_INTERNAL_PRODUCT_NAME " version ");
    ret += getVersionNo();
    ret += " " THEOLIZER_INTERNAL_LICENSE "\n";
    ret += THEOLIZER_INTERNAL_COPYRIGHT "\n";
    ret += "    SourcesHash : ";
    ret += getSourcesHash();

#error "Error for tring travis CI"

    return ret;
}

std::string getVersionNo()
{
    return kTheolizerVersion;
}

std::string getSourcesHash()
{
    return kTheolizerSourcesHash;
}

std::string getLibraryHash()
{
    return kTheolizerLibraryHash;
}

bool checkLibraryHash(char const* iLibraryHash)
{
    return getLibraryHash() == iLibraryHash;
}

//############################################################################
//      End
//############################################################################

}   // namespace theolizer
