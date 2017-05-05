#[[###########################################################################
        Theolizer Library Project

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

#       ---<<< make source list >>>---

set(CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
include(sources_list.cmake)
set(VERSION_SOURCE "${VERSION_SOURCE}" PARENT_SCOPE)

message(STATUS "Source of TheolizerLib")
foreach(SOURCE IN LISTS THEOLIZER_LIB_LIST)
    message(STATUS "${SOURCE}")
endforeach()

message(STATUS "Source of TheolizerTest")
foreach(SOURCE IN LISTS THEOLIZER_TEST_LIST)
    message(STATUS "${SOURCE}")
endforeach()

message(STATUS "Source of library's for hash")
foreach(SOURCE IN LISTS VERSION_SOURCE)
    message(STATUS "${SOURCE}")
endforeach()

#       ---<<< install header files >>>---

file(GLOB_RECURSE LIBRARY_HEADERS *.h *.inc)
file(GLOB_RECURSE REMOVE_LIST avoid-trouble.h internal.h version.h)
list(REMOVE_ITEM LIBRARY_HEADERS ${REMOVE_LIST})
list(SORT LIBRARY_HEADERS)
message(STATUS "Source of library's header")
foreach(SOURCE IN LISTS LIBRARY_HEADERS)
    message(STATUS "${SOURCE}")
endforeach()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/theolizer
        DESTINATION ${INCLUDE_DIR}
        FILES_MATCHING
             PATTERN *.h
             PATTERN *.inc
             PATTERN avoid-trouble.h EXCLUDE
             PATTERN internal.h      EXCLUDE
)

# 親へ返却する
set(LIBRARY_HEADERS "${LIBRARY_HEADERS}" PARENT_SCOPE)

#       ---<<< tool >>>---

macro(extract FOLDER ARCHIVE_FILE)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${FOLDER})
    execute_process(COMMAND ar xo ${ARCHIVE_FILE} ${ARGN}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${FOLDER})
    file(GLOB FILE_LIST RELATIVE ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${FOLDER}/*)
endmacro(extract)

#-----------------------------------------------------------------------------
#       make target
#-----------------------------------------------------------------------------

#       ---<<< Common >>>---

# Header
include_directories("${Boost_INCLUDE_DIR}")

# Library
link_directories("${Boost_LIBRARY_DIR}")

#       ---<<< static-link boost to TheolizerLib >>>---

# boostをTheolizerLibにパッケージする場合(非MSVC)

set(LIB_LIST ${Boost_LIBRARIES})

# Separate lib-list into Release and Debug
set(CONFIG_MODE "general")
set(RELEASE_LIBS "")
set(DEBUG_LIBS "")
foreach(LINK_LIB IN LISTS LIB_LIST)
    if(${LINK_LIB} STREQUAL optimized)
        set(CONFIG_MODE "optimized")
    elseif(${LINK_LIB} STREQUAL debug)
        set(CONFIG_MODE "debug") 
    elseif(${LINK_LIB} STREQUAL general)
        set(CONFIG_MODE "general") 
    elseif(${CONFIG_MODE} STREQUAL optimized)
        set(RELEASE_LIBS ${RELEASE_LIBS} ${LINK_LIB})
    elseif(${CONFIG_MODE} STREQUAL debug)
        set(DEBUG_LIBS ${DEBUG_LIBS} ${LINK_LIB})
    else()
        set(RELEASE_LIBS ${RELEASE_LIBS} ${LINK_LIB})
        set(DEBUG_LIBS   ${DEBUG_LIBS}   ${LINK_LIB})
    endif()
endforeach()
message(STATUS "RELEASE_LIBS=${RELEASE_LIBS}")
message(STATUS "DEBUG_LIBS  =${DEBUG_LIBS}")

# extract *.o files from libboost*.a and make list(OBJECT_FILES) for add_library
set(OBJECT_FILES "")
if (NOT ${IS_MSVC} AND ${STATIC_WITH_BOOST})
    if ((${CMAKE_BUILD_TYPE} STREQUAL Release) OR (${CMAKE_BUILD_TYPE} STREQUAL MinSizeRel))
        set(LINK_LIB_LIST ${RELEASE_LIBS})
    else()
        set(LINK_LIB_LIST ${DEBUG_LIBS})
    endif()
    message(STATUS "LINK_LIB_LIST=${LINK_LIB_LIST}")
    foreach(LINK_LIB IN LISTS LINK_LIB_LIST)
        if (LINK_LIB MATCHES "filesystem")
            extract(filesystem ${LINK_LIB})
            STRING(REPLACE "filesystem" "${CMAKE_CURRENT_BINARY_DIR}/filesystem" FILE_LIST "${FILE_LIST}")
            set(OBJECT_FILES ${OBJECT_FILES} ${FILE_LIST})
            message(STATUS "    extract ${LINK_LIB} to filesystem : ${FILE_LIST}")
        else()
            extract(system ${LINK_LIB})
            STRING(REPLACE "system" "${CMAKE_CURRENT_BINARY_DIR}/system" FILE_LIST "${FILE_LIST}")
            set(OBJECT_FILES ${OBJECT_FILES} ${FILE_LIST})
            message(STATUS "    extract ${LINK_LIB} to system : ${FILE_LIST}")
        endif()
    endforeach()
endif()

#       ---<<< for shared library >>>---

if(${BUILD_SHARED_LIBS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTHEOLIZER_DYN_LINK")

    if (NOT ${IS_MSVC})
        if(WIN32)
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
        endif()
    endif()
endif()

#       ---<<< TheolizerLib >>>---

message(STATUS "OBJECT_FILES=${OBJECT_FILES}")
add_library(${TheolizerLib} ${THEOLIZER_LIB_LIST} ${OBJECT_FILES})
add_dependencies(${TheolizerLib} remove_crlf)
add_dependencies(${TheolizerLib} checkBuildSystem)

# version.h生成
version_at_build(${TheolizerLib} "/theolizer/internal" "${VERSION_SOURCE}")

# boostをTheolizerLibにパッケージする場合(MSVC)
if (${IS_MSVC} AND ${STATIC_WITH_BOOST})
    # This is a measure to fix the trouble.
    #    POST_BUILD run always when cmake "--build ."
    set_target_properties(${TheolizerLib} PROPERTIES OUTPUT_NAME ${TheolizerLib}Tmp)

    # Link Boost-library to TheolizerLib
    #   target_link_libraries() do not work for Visual Studio at static link.
    add_custom_command(TARGET ${TheolizerLib} POST_BUILD
        COMMAND if "$(Configuration)" == "Release"
                lib.exe /OUT:$(TargetDir)$(ProjectName).lib
                    $(TargetDir)$(TargetFileName) ${RELEASE_LIBS}
        COMMAND if "$(Configuration)" == "Debug"
                lib.exe /OUT:$(TargetDir)$(ProjectName)d.lib
                    $(TargetDir)$(TargetFileName) ${DEBUG_LIBS})

    #ターゲット指定すると、${TheolizerLib}Tmpがインストールされてしまうため、対処。
    install(FILES $<$<CONFIG:Release>:${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${TheolizerLib}.lib>
            DESTINATION ${LIB_DIR})
    install(FILES $<$<CONFIG:Debug>:${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${TheolizerLib}d.lib>
            DESTINATION ${LIB_DIR})
else()
    target_link_libraries(${TheolizerLib} PUBLIC ${LIB_LIST})
    install(TARGETS ${TheolizerLib} DESTINATION ${LIB_DIR})
endif()

#       ---<<< TheolizerTest >>>---

add_library(${TheolizerTest} ${THEOLIZER_TEST_LIST})
add_dependencies(${TheolizerTest} checkBuildSystem)
target_link_libraries(${TheolizerTest} ${TheolizerLib})

# for boost test
if (${IS_MSVC})
    set_target_properties(${TheolizerTest} PROPERTIES COMPILE_FLAGS "/EHa")
endif()

install(TARGETS ${TheolizerTest} DESTINATION ${LIB_DIR})
