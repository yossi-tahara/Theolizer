//############################################################################
//      Theolizerライブラリのメタ・シリアライザ呼び出し
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

#if defined(_WIN32)
    #define NOMINMAX
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

#include <iostream>
#include <fstream>

#define THEOLIZER_INTERNAL_META_SERIALIZER
#include "core_integrator.h"

// ***************************************************************************
//      動的ロード準備(Windowsのみテスト済)
// ***************************************************************************

//----------------------------------------------------------------------------
//      動的ロード用クラス
//----------------------------------------------------------------------------

class DynamicLoad
{
    using FuncType = void (*)
        (
            std::ostream&               iOStream,
            theolizer::SerializerType   iSerializerType
        );
    FuncType    mCppMetaSerialize;

    #if defined(_WIN32)
    HMODULE     mHandle;
    #else
    void*       mHandle;
    #endif

public:
    DynamicLoad(char const* iDllPath)
    {
        #if defined(_WIN32)
        mHandle=LoadLibraryA(iDllPath);
        mCppMetaSerialize=reinterpret_cast<FuncType>(GetProcAddress(mHandle, "CppMetaSerialize"));
        #else
        mHandle=dlopen(iDllPath, RTLD_LAZY);
        mCppMetaSerialize=reinterpret_cast<FuncType>(dlsym(mHandle, "CppMetaSerialize"));
        #endif
    }
    ~DynamicLoad()
    {
        #if defined(_WIN32)
        FreeLibrary(mHandle);
        #else
        dlclose(mHandle);
        #endif
    }
    void CppMetaSerialize
    (
        std::ostream&               iOStream,
        theolizer::SerializerType   iSerializerType
    )
    {
        mCppMetaSerialize(iOStream, iSerializerType);
    }
};

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Need a parameter : <dll path> <meta serialize file path>\n";
return 1;
    }

    DynamicLoad aDynamicLoad(argv[1]);

    std::ofstream ofs(argv[2]);
    aDynamicLoad.CppMetaSerialize(ofs, theolizer::SerializerType::Json);

    return 0;
}
