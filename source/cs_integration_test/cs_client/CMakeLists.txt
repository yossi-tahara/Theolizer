#[[###########################################################################
        Theolizer Test Project for C# Integration

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

]]############################################################################

#-----------------------------------------------------------------------------
#       target definition
#-----------------------------------------------------------------------------

set(CMAKE_CSharp_FLAGS "/langversion:6")
if ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(CMAKE_CSharp_FLAGS "${CMAKE_CSharp_FLAGS} /platform:x64")
endif()

set(CS_SERIALIZER
    ${THEOLIZER_ROOT}/theolizer/integrator.cs
    ${THEOLIZER_ROOT}/theolizer/memory_stream.cs
)

set(CS_MAIN     App.config Program.cs)
set(CS_FORMS    Form1.cs Form1.Designer.cs Form1.resx)
set(CS_RESOUCES
    Properties/AssemblyInfo.cs
    Properties/Resources.Designer.cs
    Properties/Resources.resx
    Properties/Settings.Designer.cs
    Properties/Settings.settings
)
set(CS_REFERENCES
    System
    System.Data
    System.Drawing
    System.Windows.Forms
    System.Xml
)

#-----------------------------------------------------------------------------
#       make target
#-----------------------------------------------------------------------------

enable_language(CSharp)
include(CSharpUtilities)

add_executable(cs_client ${CS_MAIN} ${CS_FORMS} ${CS_RESOUCES} ${CS_SERIALIZER})
csharp_set_windows_forms_properties(${CS_FORMS})
csharp_set_designer_cs_properties(${CS_RESOUCES})
set_property(TARGET cs_client PROPERTY VS_DOTNET_REFERENCES ${CS_REFERENCES})

set_target_properties(cs_client PROPERTIES
    WIN32_EXECUTABLE                    true
    VS_DOTNET_TARGET_FRAMEWORK_VERSION  "v4.6.2"
    VS_GLOBAL_ROOTNAMESPACE             "cs_client"
)

message(STATUS "CMAKE_GENERATOR_PLATFORM=${CMAKE_GENERATOR_PLATFORM}")
message(STATUS "CMAKE_VS_PLATFORM_NAME=${CMAKE_VS_PLATFORM_NAME}")
