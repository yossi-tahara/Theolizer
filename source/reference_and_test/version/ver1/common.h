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

#endif  // COMMON_H
