//############################################################################
/*!
@brief      バージョン番号を自動生成
@details    バージョン・アップ手順(タグ付けする)
                1. ルートCMakeLists.txtのproject(VERSION)を修正
                2. CMakeコンフィグ実行
                3. kTheolizerSourcesHashをCommitメッセージに入れて、Commitする。
                4. kTheolizerVersionでタグ付けする。

            バージョン・アップしないでCommitする時の手順(タグ付けしない)
                1. kTheolizerSourcesHashをCommitメッセージに入れて、Commitする。
@ingroup    TheolizerLib
@file       version.h
@author     Yoshinori Tahara(Theoride Technology)
@date       2015/09/20 Created
*/
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
/*!
    Theolizerバージョン文字列(ドライバ／ライブラリ共通)
*/
char const kTheolizerVersion[]    ="0.4.1-Prerelease.";
/*!
    Theolizerソースのハッシュ値(ドライバ／ライブラリ別)
*/
char const kTheolizerSourcesHash[]="4aa10a588b819331b6a39e5afba35fb4";
/*!
    Theolizerライブラリ・ヘッダ・ファイルのハッシュ値
*/
char const kTheolizerLibraryHash[]="97197bb96f6980c19c222542c8d89d54";
