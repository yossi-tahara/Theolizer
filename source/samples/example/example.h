//############################################################################
/*!
    @brief      Theolizer紹介用サンプル・プログラム
    @ingroup    Documents
    @file       example.h
    @author     Yoshinori Tahara
    @date       2016/11/03 Created
*/
//############################################################################

#if !defined(EXAMPLE_H)
#define EXAMPLE_H

// ***************************************************************************
//      インクルード
// ***************************************************************************

#include <string>
#include <theolizer/serializer_json.h>

// ***************************************************************************
//      型定義
// ***************************************************************************

enum EnumType
{
    None,
    EnumA,
    EnumB,
    EnumC
};

struct StructType
{
    EnumType    mEnum;
    int         mInt;
    std::string mString;
    StructType() : mEnum(None), mInt(0), mString("") { }
};

#endif  // EXAMPLE_H
