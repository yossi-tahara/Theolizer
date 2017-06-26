#[[###########################################################################
        report.*のテスト

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
#       Base
#-----------------------------------------------------------------------------

if("${CMAKE_VERSION}" STREQUAL "")
    set(CMAKE_VERSION, 3.5.0)
endif()
cmake_minimum_required(VERSION ${CMAKE_VERSION})

# for MSVC
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
set(CMAKE_SUPPRESS_REGENERATION TRUE)

#-----------------------------------------------------------------------------
#       target definition(1)
#-----------------------------------------------------------------------------

#       <<< Child process >>>

set(TARGET_NAME Report1Child)
set(CHILD_NAME  Report1Child)

set(SOURCE_LIST test_report1_child.cpp)
set(LIBRARY_LIST ${TheolizerLib} ${TheolizerTest})

add_executable(${TARGET_NAME} ${SOURCE_LIST} ${HEADER_LIST})
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "TestPrograms")
if (NOT ${STATIC_WITH_BOOST})
    target_link_libraries(${TARGET_NAME} ${Boost_LIBRARIES})
endif()

if (MSVC)
    add_definitions(-D_UNICODE -DUNICODE)
    foreach(LINK_LIB IN LISTS LIBRARY_LIST)
        target_link_libraries(${TARGET_NAME} optimized ${LINK_LIB}.lib)
        target_link_libraries(${TARGET_NAME} debug     ${LINK_LIB}d.lib)
    endforeach()
    add_dependencies(${TARGET_NAME} ${LIBRARY_LIST})
else()
    target_link_libraries(${TARGET_NAME} ${LIBRARY_LIST})
endif()

#       <<< Target >>>

set(TARGET_NAME Report1)

set(SOURCE_LIST test_report1.cpp)
add_test_function(${TARGET_NAME} ${SOURCE_LIST})
add_dependencies(${TARGET_NAME} ${CHILD_NAME})

if(WIN32)
    message(STATUS "WIN32")
    # ("winmm" is for timeBegin/EndPeriod.)
    target_link_libraries(${TARGET_NAME} winmm)
endif()

if(UNIX)
    message(STATUS "UNIX")
    target_link_libraries(${TARGET_NAME} pthread)
endif()

#-----------------------------------------------------------------------------
#       target definition(2)
#-----------------------------------------------------------------------------

set(TARGET_NAME Report2)

set(SOURCE_LIST test_report2.cpp)
add_test_function(${TARGET_NAME} ${SOURCE_LIST})

#-----------------------------------------------------------------------------
#       target definition(3, 4)
#-----------------------------------------------------------------------------

set(TARGET_NAME Report3)

set(SOURCE_LIST test_report3.cpp test_report4.cpp)
add_test_function(${TARGET_NAME} "WILL_FAIL" ${SOURCE_LIST})

# exit code != 0 is PASS.
#set_tests_properties(Report3 PROPERTIES WILL_FAIL true)

#-----------------------------------------------------------------------------
#       target definition(5)
#-----------------------------------------------------------------------------

set(TARGET_NAME Report5)

set(SOURCE_LIST test_report5.cpp)
add_test_function(${TARGET_NAME} ${SOURCE_LIST})

if(WIN32)
    # ("winmm" is for timeBegin/EndPeriod.)
    target_link_libraries(${TARGET_NAME} winmm)
endif()

if(UNIX)
    message(STATUS "UNIX")
    target_link_libraries(${TARGET_NAME} pthread)
endif()

