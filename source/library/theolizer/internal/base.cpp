//############################################################################
//      Theolizer全体のベース部
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
//      型情報取得(getNameByTypeInfo)
// ***************************************************************************

// ---------------------------------------------------------------------------
//      GCC用
// ---------------------------------------------------------------------------

#if defined(__GNUC__)

std::string getNameByTypeInfo(std::type_index aTypeIndex)
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

std::string getNameByTypeInfo(std::type_index aTypeIndex)
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
