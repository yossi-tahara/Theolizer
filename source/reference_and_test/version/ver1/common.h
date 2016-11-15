//############################################################################
//      Theolizer仕様／テスト・プログラム
//
//          共通ヘッダ
/*
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
*/
//############################################################################

#if !defined(COMMON_H)
#define COMMON_H

// ***************************************************************************
//      共通のインクルード
// ***************************************************************************

#include <theolizer/test_tool.h>

//############################################################################
//      ユーティリティ
//############################################################################

// ***************************************************************************
//      テスト用関数テンプレート実体化マクロ
// ***************************************************************************

#if defined(__GNUC__)

    #define INSTANTIATION(dType, dFunc, dSerializer, ...)                   \
        template dType dFunc<dSerializer>(dSerializer&, ##__VA_ARGS__);

    #define INSTANTIATION_ALL(dType, dFunc, ...)                            \
        INSTANTIATION(dType, dFunc, theolizer::JsonOSerializer<>, ##__VA_ARGS__);\
        INSTANTIATION(dType, dFunc, theolizer::JsonISerializer<>, ##__VA_ARGS__);\
        INSTANTIATION(dType, dFunc, theolizer::BinaryOSerializer<>, ##__VA_ARGS__);\
        INSTANTIATION(dType, dFunc, theolizer::BinaryISerializer<>, ##__VA_ARGS__);\
        INSTANTIATION(dType, dFunc, theolizer::FastOSerializer<>, ##__VA_ARGS__);\
        INSTANTIATION(dType, dFunc, theolizer::FastISerializer<>, ##__VA_ARGS__)

#else

    #define INSTANTIATION(dType, dFunc, dSerializer, ...)                   \
        template dType dFunc<dSerializer>(dSerializer&, __VA_ARGS__);

    #define INSTANTIATION_ALL(dType, dFunc, ...)                            \
        INSTANTIATION(dType, dFunc, theolizer::JsonOSerializer<>, __VA_ARGS__);\
        INSTANTIATION(dType, dFunc, theolizer::JsonISerializer<>, __VA_ARGS__);\
        INSTANTIATION(dType, dFunc, theolizer::BinaryOSerializer<>, __VA_ARGS__);\
        INSTANTIATION(dType, dFunc, theolizer::BinaryISerializer<>, __VA_ARGS__);\
        INSTANTIATION(dType, dFunc, theolizer::FastOSerializer<>, __VA_ARGS__);\
        INSTANTIATION(dType, dFunc, theolizer::FastISerializer<>, __VA_ARGS__)

#endif

//############################################################################
//      配列テスト用関数テンプレート
//############################################################################

// ***************************************************************************
//      保存
// ***************************************************************************

//! [saveTestArray]
template<typename tType, std::size_t N, class tSerializer, typename tFunc>
void saveTestArray(tSerializer& iSerializer, tFunc iFunc)
{
    std::cout << "        saveTestArray<"
              << THEOLIZER_INTERNAL_TYPE_NAME(tType) << ", " << N << ">\n";

    // 1次元配列
    tType aArray1[N];
    for (std::size_t i=0; i < N; ++i)
    {
        aArray1[i]=iFunc(i);
    }
    THEOLIZER_PROCESS(iSerializer, aArray1);

    // 2次元配列
    tType aArray2[2][N];
    for (std::size_t j=0; j < 2; ++j)
    {
        for (std::size_t i=0; i < N; ++i)
        {
            aArray2[j][i]=iFunc(i+j);
        }
    }
    THEOLIZER_PROCESS(iSerializer, aArray2);

    // 3次元配列
    tType aArray3[3][2][N];
    for (std::size_t k=0; k < 3; ++k)
    {
        for (std::size_t j=0; j < 2; ++j)
        {
            for (std::size_t i=0; i < N; ++i)
            {
                aArray3[k][j][i]=iFunc(i+j+k);
            }
        }
    }
    THEOLIZER_PROCESS(iSerializer, aArray3);
}
//! [saveTestArray]

// ***************************************************************************
//      回復
// ***************************************************************************

//! [loadTestArray]
template<typename tType, std::size_t N, class tSerializer, typename tFunc>
void loadTestArray(tSerializer& iSerializer, tFunc iFunc)
{
    std::cout << "        loadTestArray<"
              << THEOLIZER_INTERNAL_TYPE_NAME(tType) << ", " << N << ">\n";

    // 1次元配列
    tType aArray1[N];
    THEOLIZER_PROCESS(iSerializer, aArray1);
    for (std::size_t i=0; i < N; ++i)
    {
        THEOLIZER_EQUAL(aArray1[i], iFunc(i), i);
    }

    // 2次元配列
    tType aArray2[2][N];
    THEOLIZER_PROCESS(iSerializer, aArray2);
    for (std::size_t j=0; j < 2; ++j)
    {
        for (std::size_t i=0; i < N; ++i)
        {
            THEOLIZER_EQUAL(aArray2[j][i], iFunc(i+j), i, j);
        }
    }

    // 3次元配列
    tType aArray3[3][2][N];
    THEOLIZER_PROCESS(iSerializer, aArray3);
    for (std::size_t k=0; k < 3; ++k)
    {
        for (std::size_t j=0; j < 2; ++j)
        {
            for (std::size_t i=0; i < N; ++i)
            {
                THEOLIZER_EQUAL(aArray3[k][j][i], iFunc(i+j+k), i, j, k);
            }
        }
    }
}
//! [loadTestArray]

#endif  // COMMON_H
