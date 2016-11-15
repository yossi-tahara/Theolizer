#[[###########################################################################
        Theolizer Document Project

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

]]############################################################################

#-----------------------------------------------------------------------------
#       setup Doxygen target
#-----------------------------------------------------------------------------

#       ---<<< 多言語用サブ >>>---

function(set_language ISO639)

    set(DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.${ISO639})

    if("${ISO639}" STREQUAL "ja")
        set(LANGEAGE "japanese")
    else()
        message(SEND_ERROR "${ISO639} is unkown language.")
    endif()

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${DOXYFILE})
    configure_file(index.html ${CMAKE_CURRENT_BINARY_DIR}/${ISO639}/index.html)

    add_custom_target(${ISO639} ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${ISO639}
        COMMENT "Creating documentation using Doxygen")

    add_dependencies(${ISO639} VersionOf${TheolizerLib})
    if(BUILD_DRIVER)
        add_dependencies(${ISO639} VersionOfTheolizerDriver)
    endif()

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${ISO639} DESTINATION ${DOC_DIR})

endfunction()

#       ---<<< 設定 >>>---

find_package(Doxygen)

if(NOT DOXYGEN_FOUND)
message(SEND_ERROR "Doxygen is needed. Please install Doxygen, or set BUILD_DOCUMENT to TRUE.")
endif()

if(NOT DOXYGEN_DOT_FOUND)
message(SEND_ERROR "Graphviz is needed. Please install Graphviz, or set BUILD_DOCUMENT to TRUE.")
endif()

# setup DOT_PATH
get_filename_component(DOT_PATH ${DOXYGEN_DOT_EXECUTABLE} DIRECTORY)
message(STATUS "DOT_PATH=${DOT_PATH}")

# 日本語用
set_language(ja)
