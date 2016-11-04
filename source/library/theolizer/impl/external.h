//############################################################################
//      Theolizerをimportするための外部用ヘッダ
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

#if !defined(THEOLIZER_INTERNAL_EXTERNAL_H)
#define THEOLIZER_INTERNAL_EXTERNAL_H

//############################################################################
//      dll export関係
//############################################################################

#if !defined(THEOLIZER_INTERNAL_DLL)
    #if defined(THEOLIZER_DYN_LINK)
        #if defined(_MSC_VER)
            #define THEOLIZER_INTERNAL_DLL __declspec(dllimport)
        #else
            #define THEOLIZER_INTERNAL_DLL
        #endif
    #else
        #define THEOLIZER_INTERNAL_DLL
    #endif
#endif

//############################################################################
//      名前空間
//############################################################################

namespace theolizer
{

// ***************************************************************************
//      internal
//          theolizer貢献者以外の使用は非推奨
//              theolizer内部的な使用のみを想定している
//              仕様変更の頻度が高い
//          なお、派生Serializerはtheolizerへフィードバック頂けることを期待しているが、
//              フィードバック無しで、派生Serializer開発を止めるものではない。
//              使用方法の想定が狭いことと、仕様変更の可能性を考慮願いたい。
// ***************************************************************************

namespace internal
{
}   // namespace internal

}   // namespace theolizer

#endif  // THEOLIZER_INTERNAL_EXTERNAL_H
