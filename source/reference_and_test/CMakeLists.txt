#[[###########################################################################
        Theolizer Test Project

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
#       Sub Functions
#-----------------------------------------------------------------------------

# 準備：フォルダ削除→フォルダ生成→コピー
#       NOW_VERSION  : 現バージョン指定
#       PREV_VERSION : 前バージョン指定  (先頭バージョンは"")
macro(add_build_and_test TARGET_NAME NOW_VERSION PREV_VERSION ERROR_CHECK TEST_FL MS RS)
    set(TEST_NAME ${TARGET_NAME}${NOW_VERSION}${ERROR_CHECK})
    set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${NOW_VERSION}")
    set(BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}${ERROR_CHECK}")
    if("${PREV_VERSION}" STREQUAL "")
        set(PREV_DIR "${SOURCE_DIR}")
    else()
        set(PREV_NAME ${TARGET_NAME}${PREV_VERSION})
        set(PREV_DIR "${CMAKE_CURRENT_BINARY_DIR}/${PREV_VERSION}")
    endif()

    # ビルド・パラメータ生成
    if (MSVC)
        # コンパイル・フラグ設定
        set(PARAMETERS ${CMAKE_CXX_FLAGS})
        string(REPLACE " " ";" PARAMETERS "${PARAMETERS}")

        set(TEMP ${CMAKE_CXX_FLAGS_RELEASE})
        string(REPLACE " " ";" TEMP "${TEMP}")
        foreach(PARAM IN LISTS TEMP)
            set(PARAMETERS ${PARAMETERS} $<$<CONFIG:Release>:${PARAM}>)
        endforeach()

        set(TEMP ${CMAKE_CXX_FLAGS_DEBUG})
        string(REPLACE " " ";" TEMP "${TEMP}")
        foreach(PARAM IN LISTS TEMP)
            set(PARAMETERS ${PARAMETERS} $<$<CONFIG:Debug>:${PARAM}>)
        endforeach()

        # コンパイル・エラー発生用マクロ定義
        if (NOT "${ERROR_CHECK}" STREQUAL "")
            set(PARAMETERS ${PARAMETERS} /D${ERROR_CHECK})
        endif()

        # 追加パラメータ設定
        set(PARAMETERS ${PARAMETERS} ${ARGN})
        set(PARAMETERS ${PARAMETERS} /Fe${TEST_NAME})
        set(PARAMETERS ${PARAMETERS} /I${CMAKE_SOURCE_DIR}/library)
        set(PARAMETERS ${PARAMETERS} /link /LIBPATH:${CMAKE_BINARY_DIR}/library/$<CONFIG>)
        set(PARAMETERS ${PARAMETERS} ${TheolizerLib}$<$<CONFIG:Debug>:d>.lib)
        set(PARAMETERS ${PARAMETERS} ${TheolizerTest}$<$<CONFIG:Debug>:d>.lib)

        # Staticの時(Boostのリンクが必要)
        if(("${STATIC_WITH_BOOST}" STREQUAL "OFF") AND ("${BUILD_SHARED_LIBS}" STREQUAL "OFF"))
            set(PARAMETERS ${PARAMETERS} /LIBPATH:${Boost_INCLUDE_DIR})
            set(IS_RELEASE "")
            foreach(LIB IN LISTS Boost_LIBRARIES)
                if("${LIB}" STREQUAL "optimized")
                    set(IS_RELEASE "TRUE")
                elseif("${LIB}" STREQUAL "debug")
                    set(IS_RELEASE "FALSE")
                elseif("${IS_RELEASE}" STREQUAL "TRUE")
                    set(PARAMETERS ${PARAMETERS} $<$<CONFIG:Release>:${LIB}>)
                elseif("${IS_RELEASE}" STREQUAL "FALSE")
                    set(PARAMETERS ${PARAMETERS} $<$<CONFIG:Debug>:${LIB}>)
                else()
                    message(SEND_ERROR "IS_RELEASE=${IS_RELEASE} : ${Boost_LIBRARIES}")
                endif()
            endforeach()
        endif()
    else()
        # コンパイル・フラグ設定
        set(PARAMETERS ${CMAKE_CXX_FLAGS})
        if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
            set(PARAMETERS ${PARAMETERS} ${CMAKE_CXX_FLAGS_RELEASE})
        elseif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            set(PARAMETERS ${PARAMETERS} ${CMAKE_CXX_FLAGS_DEBUG})
        else()
            message(SEND_ERROR "unknown BUILD_TYPE : ${CMAKE_BUILD_TYPE}")
        endif()
        set(PARAMETERS "${PARAMETERS} -v")
        STRING(REPLACE " " ";" PARAMETERS "${PARAMETERS}")

        # Theolizerへ解析要求
        set(PARAMETERS ${PARAMETERS} -DTHEOLIZER_ANALYZE)

        # コンパイル・エラー発生用マクロ定義
        if (NOT "${ERROR_CHECK}" STREQUAL "")
            set(PARAMETERS ${PARAMETERS} -D${ERROR_CHECK})
        endif()

        # 追加パラメータ設定
        set(PARAMETERS ${PARAMETERS} ${ARGN})
        set(PARAMETERS ${PARAMETERS} -o${TEST_NAME})
        set(PARAMETERS ${PARAMETERS} -I${CMAKE_SOURCE_DIR}/library)
        set(PARAMETERS ${PARAMETERS} -L${CMAKE_BINARY_DIR}/library)
        set(PARAMETERS ${PARAMETERS} -l${TheolizerLib})
        set(PARAMETERS ${PARAMETERS} -l${TheolizerTest})

        # Staticの時(Boostのリンクが必要)
        if(("${STATIC_WITH_BOOST}" STREQUAL "OFF") AND ("${BUILD_SHARED_LIBS}" STREQUAL "OFF"))

            if(NOT "${Boost_LIBRARY_DIR}" STREQUAL "")
                set(PARAMETERS ${PARAMETERS} -L${Boost_LIBRARY_DIR})
            endif()

            if(WIN32)
                set(IS_RELEASE "")
                foreach(LIB IN LISTS Boost_LIBRARIES)
                    if("${LIB}" STREQUAL "optimized")
                        set(IS_RELEASE "TRUE")
                    elseif("${LIB}" STREQUAL "debug")
                        set(IS_RELEASE "FALSE")
                    elseif("${IS_RELEASE}" STREQUAL "TRUE")
                        if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
                             set(PARAMETERS ${PARAMETERS} ${LIB})
                        endif()
                    elseif("${IS_RELEASE}" STREQUAL "FALSE")
                        if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
                             set(PARAMETERS ${PARAMETERS} ${LIB})
                        endif()
                    else()
                        message(SEND_ERROR "IS_RELEASE=${IS_RELEASE} : ${Boost_LIBRARIES}")
                    endif()
                endforeach()
            else()
                set(PARAMETERS ${PARAMETERS} ${Boost_LIBRARIES})
            endif()

        endif()
    endif()

    # 共通ファイル群
    file(GLOB COMMON_LIST *.h *.inc)
    message(STATUS "COMMON_LIST=${COMMON_LIST}")

    # ビルド準備とビルド実行(custom_target)
    if (MSVC)
        set(PARAMETERS ${PARAMETERS} "/DTHEOLIZER_DO_PROCESS")
        set(PARAMETERS ${PARAMETERS} "/Dtheolizer_original_compiler=\"${CMAKE_CXX_COMPILER}\"")
    else()
        set(PARAMETERS ${PARAMETERS} "-DTHEOLIZER_DO_PROCESS")
        set(PARAMETERS ${PARAMETERS} "--theolizer_original_compiler=\"${CMAKE_CXX_COMPILER}\"")
    endif()

    # デバッグ用
    if(TRUE)
    message(STATUS "----------------------------------------------------")
    message(STATUS "PARAMETERS=${PARAMETERS}")
    foreach(PARAM IN LISTS PARAMETERS)
        message(STATUS "${PARAM}")
    endforeach()
    message(STATUS "BINARY_DIR=${BINARY_DIR}")
    message(STATUS "PREV_DIR=${PREV_DIR}")
    message(STATUS "----------------------------------------------------")
    endif()

    if(BUILD_DRIVER)
        add_custom_target(${TEST_NAME}
            COMMAND ${CMAKE_COMMAND} -E echo "${TEST_NAME}"
            COMMAND ${CMAKE_COMMAND} -E echo "CONFIG=$<CONFIG>"
            COMMAND ${CMAKE_COMMAND} -E echo "PARAMETERS=\"${PARAMETERS}\""
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${BINARY_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_DIR} ${BINARY_DIR}
            COMMAND ${CMAKE_COMMAND}
                \"-DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}\"
                \"-DPREV_DIR=${PREV_DIR}\"
                \"-DBINARY_DIR=${BINARY_DIR}\"
                \"-DMATCH_STRING=${MS}\"
                \"-DREPLACE_STRING=${RS}\"
                -P "${CMAKE_CURRENT_SOURCE_DIR}/copy.cmake"
            COMMAND $<TARGET_FILE:TheolizerDriver> ${PARAMETERS})
    else()
        add_custom_target(${TEST_NAME}
            COMMAND ${CMAKE_COMMAND} -E echo "${TEST_NAME}"
            COMMAND ${CMAKE_COMMAND} -E echo "CONFIG=$<CONFIG>"
            COMMAND ${CMAKE_COMMAND} -E echo "PARAMETERS=\"${PARAMETERS}\""
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${BINARY_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_DIR} ${BINARY_DIR}
            COMMAND ${CMAKE_COMMAND}
                \"-DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}\"
                \"-DPREV_DIR=${PREV_DIR}\"
                \"-DBINARY_DIR=${BINARY_DIR}\"
                \"-DMATCH_STRING=${MS}\"
                \"-DREPLACE_STRING=${RS}\"
                -P "${CMAKE_CURRENT_SOURCE_DIR}/copy.cmake"
            COMMAND ${CMAKE_INSTALL_PREFIX}/bin/TheolizerDriver ${PARAMETERS})
    endif()
    set_target_properties(${TEST_NAME} PROPERTIES FOLDER "TestPrograms")
    #exclude_all_target(${TEST_NAME})

    # ビルド
    if ("${ERROR_CHECK}" STREQUAL "")
        add_test_sub_base(${TEST_NAME}Build ${CMAKE_COMMAND}
            --build ${CMAKE_BINARY_DIR} --config $<CONFIG> --target ${TEST_NAME})
    else()
        add_test_sub_base(${TEST_NAME}Build ${CMAKE_COMMAND}
            "-DBINARY_DIR=${CMAKE_BINARY_DIR}"
            -DCONFIG=$<CONFIG>
            -DTARGET_NAME=${TEST_NAME}
            "-DERROR_MESSAGE=${ERROR_MESSAGE}"
            -DERROR_NUMBER=${ERROR_NUMBER}
            -P "${CMAKE_SOURCE_DIR}/tools/build_error_test.cmake"
        )
    endif()
    set_tests_properties(${TEST_NAME}Build PROPERTIES LABELS ${TEST_FL})

    # 前バージョンがあるなら、前バージョンのビルドへ依存
    if(NOT "${PREV_VERSION}" STREQUAL "")
        set_tests_properties(${TEST_NAME}Build PROPERTIES DEPENDS ${PREV_NAME}Build)
        message(STATUS "DEPENDS : ${TEST_NAME}Build depends to ${PREV_NAME}Build")
    endif()

    # 追加パラメータがない時のみ、実行する
    #  (追加パラメータが有るときはビルド・エラーを発生させているため)
    if ("${ERROR_CHECK}" STREQUAL "")
        # 1回目の実行
        add_test_sub_base(${TEST_NAME}Exec ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME})
        set_tests_properties(${TEST_NAME}Exec PROPERTIES LABELS ${TEST_FL})
        set_tests_properties(${TEST_NAME}Exec PROPERTIES FOLDER "TestTargets")
        # 前バージョンがあるなら、1回目実行の前バージョンへ依存
        if(NOT "${PREV_VERSION}" STREQUAL "")
            set_tests_properties(${TEST_NAME}Exec PROPERTIES DEPENDS ${PREV_NAME}Exec)
            message(STATUS "DEPENDS : ${TEST_NAME}Exec depends to ${PREV_NAME}Exec")
        endif()

        # 2回目実行
        add_test_sub_base(${TEST_NAME}Exec2 ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME} Exec2)
        set_tests_properties(${TEST_NAME}Exec2 PROPERTIES LABELS ${TEST_FL})
        set_tests_properties(${TEST_NAME}Exec2 PROPERTIES FOLDER "TestTargets")
        # 前バージョンがあるなら、2回目実行の前バージョンへ依存
        if(NOT "${PREV_VERSION}" STREQUAL "")
            set_tests_properties(${TEST_NAME}Exec2 PROPERTIES DEPENDS ${PREV_NAME}Exec2)
            message(STATUS "DEPENDS : ${TEST_NAME}Exec2 depends to ${PREV_NAME}Exec2")
        endif()
    endif()
endmacro(add_build_and_test)

#-----------------------------------------------------------------------------
#       Test-Projects
#-----------------------------------------------------------------------------

# MinGWの不具合(https://sourceforge.net/p/mingw-w64/discussion/723797/thread/c6b70624/#7f0a)暫定対策
# 巨大なテスト用コンパイル単位を縮小する
if(CMAKE_COMPILER_IS_MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSHRINK_FOR_MINGW")
    message(STATUS "add(CMAKE_CXX_FLAGS -DSHRINK_FOR_MINGW)")
##  通常数分のビルドが1時間以上経過しても終了しない
##  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wa,-mbig-obj")
##  message(STATUS "add(CMAKE_CXX_FLAGS -O3 -Wa,-mbig-obj)")
endif()

set(TARGET_NAME external_test_)

#-----------------------------------------------------------------------------
#       基本的網羅テスト
#-----------------------------------------------------------------------------

if(TRUE)

set(FIRST_VERSION "")
set(LAST_VERSION  "")
set(PREV_VERSION  "")

set(NOW_VERSION basic)
set(TEST_LABEL "TestF1L")
set(SOURCE_LIST                ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/main.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_basic_process.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_class_variation.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_enum_variation.cpp)

message(STATUS "start of add_build_and_test("
    "${TARGET_NAME}${NOW_VERSION} \"${PREV_VERSION}\" \"${TEST_LABEL}\")")
add_build_and_test(${TARGET_NAME} ${NOW_VERSION} "${PREV_VERSION}" "" ${TEST_LABEL}
    "" "" ${SOURCE_LIST})

set(NOW_VERSION basic2)
set(TEST_LABEL "TestF2L")
set(SOURCE_LIST                ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/main.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_object_tracking.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_object_tracking2.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_object_tracking3.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_polymorphism.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_destinations.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_support_stl.cpp)

message(STATUS "start of add_build_and_test("
    "${TARGET_NAME}${NOW_VERSION} \"${PREV_VERSION}\" \"${TEST_LABEL}\")")
add_build_and_test(${TARGET_NAME} ${NOW_VERSION} "${PREV_VERSION}" "" ${TEST_LABEL}
    "" "" ${SOURCE_LIST})

endif()

#-----------------------------------------------------------------------------
#       バージョン変更テスト
#-----------------------------------------------------------------------------

if(TRUE)

set(FIRST_VERSION "ver1a")
set(LAST_VERSION  "ver3b")
set(TEST_LABEL "TestF3L")

#       ---<<< ver1a >>>---

set(PREV_VERSION "")
set(NOW_VERSION  "ver1a")
set(SOURCE_LIST                ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/main.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_modify_enum.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_modify_class.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_modify_complex.cpp)

message(STATUS "start of add_build_and_test("
    "${TARGET_NAME}${NOW_VERSION} \"${PREV_VERSION}\" \"${TEST_LABEL}\")")
add_build_and_test(${TARGET_NAME} ${NOW_VERSION} "${PREV_VERSION}" "" ${TEST_LABEL}
    "" "" ${SOURCE_LIST})

#       ---<<< ver1b >>>---

set(PREV_VERSION "${NOW_VERSION}")
set(NOW_VERSION ver1b)
set(SOURCE_LIST                ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/main.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_modify_enum.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_modify_class.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_modify_complex.cpp)

message(STATUS "start of add_build_and_test("
    "${TARGET_NAME}${NOW_VERSION} \"${PREV_VERSION}\" \"${TEST_LABEL}\")")
add_build_and_test(${TARGET_NAME} ${NOW_VERSION} "${PREV_VERSION}" "" ${TEST_LABEL}
    "" "" ${SOURCE_LIST})

#       ---<<< ver1c >>>---

set(PREV_VERSION "${NOW_VERSION}")
set(NOW_VERSION ver1c)
set(SOURCE_LIST                ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/main.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_modify_enum.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_modify_class.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_modify_complex.cpp)

message(STATUS "start of add_build_and_test("
    "${TARGET_NAME}${NOW_VERSION} \"${PREV_VERSION}\" \"${TEST_LABEL}\")")
add_build_and_test(${TARGET_NAME} ${NOW_VERSION} "${PREV_VERSION}" "" ${TEST_LABEL}
    "" "" ${SOURCE_LIST})

#       ---<<< ver2a >>>---

set(PREV_VERSION "${NOW_VERSION}")
set(NOW_VERSION ver2a)
set(SOURCE_LIST                ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/main.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_modify_enum.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_modify_class.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_modify_complex.cpp)

message(STATUS "start of add_build_and_test("
    "${TARGET_NAME}${NOW_VERSION} \"${PREV_VERSION}\" \"${TEST_LABEL}\")")
add_build_and_test(${TARGET_NAME} ${NOW_VERSION} "${PREV_VERSION}" "" ${TEST_LABEL}
    "(ModifyHalfAutoX)" "(ModifyHalfAutoY)" ${SOURCE_LIST})

#       ---<<< ver3a >>>---

set(PREV_VERSION "${NOW_VERSION}")
set(NOW_VERSION ver3a)
set(SOURCE_LIST                ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/main.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_modify_enum.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_modify_class.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_modify_complex.cpp)

message(STATUS "start of add_build_and_test("
    "${TARGET_NAME}${NOW_VERSION} \"${PREV_VERSION}\" \"${TEST_LABEL}\")")
add_build_and_test(${TARGET_NAME} ${NOW_VERSION} "${PREV_VERSION}" "" ${TEST_LABEL}
    "" "" ${SOURCE_LIST})

#       ---<<< ver3b >>>---

set(PREV_VERSION "${NOW_VERSION}")
set(NOW_VERSION ver3b)
set(SOURCE_LIST                ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/main.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_modify_enum.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_modify_class.cpp)
set(SOURCE_LIST ${SOURCE_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${NOW_VERSION}/test_modify_complex.cpp)

message(STATUS "start of add_build_and_test("
    "${TARGET_NAME}${NOW_VERSION} \"${PREV_VERSION}\" \"${TEST_LABEL}\")")
add_build_and_test(${TARGET_NAME} ${NOW_VERSION} "${PREV_VERSION}" "" ${TEST_LABEL}
    "" "" ${SOURCE_LIST})

#       ---<<< ver3bERROR1 >>>---

if (MSVC)
    set(ERROR_MESSAGE " C2679")
    set(ERROR_NUMBER 8)
else()
    set(ERROR_MESSAGE " no match for ")
    set(ERROR_NUMBER 4)
endif()

message(STATUS "start of add_build_and_test("
    "${TARGET_NAME}${NOW_VERSION} \"${PREV_VERSION}\" \"ERROR1\" \"${TEST_LABEL}\")")
add_build_and_test(${TARGET_NAME} ${NOW_VERSION} "${PREV_VERSION}" "ERROR1" ${TEST_LABEL}
    "" "" ${SOURCE_LIST})

#       ---<<< ver3bERROR2 >>>---

if (MSVC)
    set(ERROR_MESSAGE " C2679")
    set(ERROR_NUMBER 8)
else()
    set(ERROR_MESSAGE " no match for ")
    set(ERROR_NUMBER 4)
endif()

message(STATUS "start of add_build_and_test("
    "${TARGET_NAME}${NOW_VERSION} \"${PREV_VERSION}\" \"ERROR2\" \"${TEST_LABEL}\")")
add_build_and_test(${TARGET_NAME} ${NOW_VERSION} "${PREV_VERSION}" "ERROR2" ${TEST_LABEL}
    "" "" ${SOURCE_LIST})

#       ---<<< ver3bERROR3 >>>---

set(ERROR_MESSAGE "Please check deleted class or enum.")
set(ERROR_NUMBER 1)
if (MSVC)
    if (MSVC_VERSION GREATER_EQUAL 1910)
        set(ERROR_NUMBER 2)
    endif()
endif()

message(STATUS "start of add_build_and_test("
    "${TARGET_NAME}${NOW_VERSION} \"${PREV_VERSION}\" \"ERROR2\" \"${TEST_LABEL}\")")
add_build_and_test(${TARGET_NAME} ${NOW_VERSION} "${PREV_VERSION}" "ERROR3" ${TEST_LABEL}
    "" "" ${SOURCE_LIST})

#-----------------------------------------------------------------------------
#       最初のバージョンの1回目実行を最後のバージョンのビルドへ依存させる
#       最初のバージョンの2回目実行を最後のバージョンの1回目実行へ依存させる
#-----------------------------------------------------------------------------

set_tests_properties(${TARGET_NAME}${FIRST_VERSION}Exec
    PROPERTIES DEPENDS ${TARGET_NAME}${LAST_VERSION}Build)
message(STATUS "DEPENDS : ${TARGET_NAME}${FIRST_VERSION}Exec depends to ${TARGET_NAME}${LAST_VERSION}Build")

set_tests_properties(${TARGET_NAME}${FIRST_VERSION}Exec2
    PROPERTIES DEPENDS ${TARGET_NAME}${LAST_VERSION}Exec)
message(STATUS "DEPENDS : ${TARGET_NAME}${FIRST_VERSION}Exec2 depends to ${TARGET_NAME}${LAST_VERSION}Exec")

endif()

#-----------------------------------------------------------------------------
#       make hash for check Pass
#-----------------------------------------------------------------------------

add_custom_target(HashOfReferenceAndTest
    COMMAND ${CMAKE_COMMAND}
        -D CURRENT_SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"
        -P \"${CMAKE_SOURCE_DIR}/tools/sources_hash.cmake\"
         > \"${CMAKE_CURRENT_BINARY_DIR}/sources_hash.txt\"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
add_dependencies(makeHashTxt HashOfReferenceAndTest)
