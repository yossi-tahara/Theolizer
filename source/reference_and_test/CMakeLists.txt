#[[###########################################################################
        Theolizer Test Project

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
#       Sub Functions
#-----------------------------------------------------------------------------

# 準備：フォルダ削除→フォルダ生成→コピー
#       VERSION : バージョン指定
macro(add_build_and_test TARGET_NAME VERSION ERROR_CHECK)
    set(TEST_NAME ${TARGET_NAME}${VERSION})

    # ビルド・パラメータ生成
    if (${IS_MSVC})
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

        # Theolizerへ解析要求
        set(PARAMETERS ${PARAMETERS} /DTHEOLIZER_ANALYZE)

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
        STRING(REPLACE " " ";" PARAMETERS "${PARAMETERS}")

        # Theolizerへ解析要求
        set(PARAMETERS ${PARAMETERS} -DTHEOLIZER_ANALYZE)

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

    # デバッグ用
    if(TRUE)
    message(STATUS "----------------------------------------------------")
    message(STATUS "PARAMETERS=${PARAMETERS}")
    foreach(PARAM IN LISTS PARAMETERS)
        message(STATUS "${PARAM}")
    endforeach()
    message(STATUS "----------------------------------------------------")
    endif()

    # ビルド準備とビルド実行(custom_target)
    set(PARAMETERS ${PARAMETERS} "--theolizer-debug=${CMAKE_CXX_COMPILER}")
    if(BUILD_DRIVER)
        add_custom_target(${TEST_NAME}
            COMMAND ${CMAKE_COMMAND} -E echo "${TARGET_NAME}${VERSION}"
            COMMAND ${CMAKE_COMMAND} -E echo "CONFIG=$<CONFIG>"
            COMMAND ${CMAKE_COMMAND} -E echo "PARAMETERS=\"${PARAMETERS}\""
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/${VERSION}
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/${VERSION} ${CMAKE_CURRENT_BINARY_DIR}/${VERSION}
            COMMAND $<TARGET_FILE:TheolizerDriver> ${PARAMETERS})
    else()
        add_custom_target(${TEST_NAME}
            COMMAND ${CMAKE_COMMAND} -E echo "${TARGET_NAME}${VERSION}"
            COMMAND ${CMAKE_COMMAND} -E echo "CONFIG=$<CONFIG>"
            COMMAND ${CMAKE_COMMAND} -E echo "PARAMETERS=\"${PARAMETERS}\""
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/${VERSION}
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/${VERSION} ${CMAKE_CURRENT_BINARY_DIR}/${VERSION}
            COMMAND ${CMAKE_INSTALL_PREFIX}/bin/TheolizerDriver ${PARAMETERS})
    endif()
    set_target_properties(${TEST_NAME} PROPERTIES FOLDER "TestPrograms")
    #exclude_all_target(${TEST_NAME})

    # テスト
    add_test_sub_base(${TEST_NAME}Test ${CMAKE_COMMAND}
        --build ${CMAKE_BINARY_DIR} --config $<CONFIG> --target ${TEST_NAME})
    set_tests_properties(${TEST_NAME}Test PROPERTIES LABELS TestFL)

    # 追加パラメータがある時、ビルド・エラーが発生することをテストする
    if (NOT "${ERROR_CHECK}" STREQUAL "")
        set_tests_properties(${TEST_NAME}Test PROPERTIES WILL_FAIL TRUE)
        set_tests_properties(${TEST_NAME}Test PROPERTIES
                             FAIL_REGULAR_EXPRESSION ${ERROR_CHECK})

    # 追加パラメータがない時、実行する(test)
    else()
        add_test_sub_base(${TEST_NAME}Exec ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME})
        set_tests_properties(${TEST_NAME}Exec PROPERTIES LABELS TestFL)
        set_tests_properties(${TEST_NAME}Exec PROPERTIES FOLDER "TestTargets")
        set_tests_properties(${TEST_NAME}Exec PROPERTIES DEPENDS ${TEST_NAME}Test)
    endif()
endmacro(add_build_and_test)

#-----------------------------------------------------------------------------
#       Test-Projects
#-----------------------------------------------------------------------------

add_subdirectory(version)
