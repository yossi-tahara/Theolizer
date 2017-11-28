#[[###########################################################################
        テスト

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
#       Test-Projects
#-----------------------------------------------------------------------------

add_subdirectory(base)
add_subdirectory(report)
add_subdirectory(serializer)
add_subdirectory(test_tool)
add_subdirectory(u8string)

#-----------------------------------------------------------------------------
#       make hash for check Pass
#-----------------------------------------------------------------------------

add_custom_target(HashOfTestLibrary
    COMMAND ${CMAKE_COMMAND}
        -D CURRENT_SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"
        -P \"${CMAKE_SOURCE_DIR}/tools/sources_hash.cmake\"
         > \"${CMAKE_CURRENT_BINARY_DIR}/sources_hash.txt\"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
add_dependencies(makeHashTxt HashOfTestLibrary)
