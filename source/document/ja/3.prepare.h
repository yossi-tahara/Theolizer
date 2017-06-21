//############################################################################
/*!
    @brief      ドキュメント・ファイル－Theolizerによる開発の準備
    @ingroup    Documents
    @file       3.prepare.h
    @author     Yoshinori Tahara
    @date       2016/11/01 Created
*/
/*
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
*/
//############################################################################

/*!
    @page Prepare インストール／アンインストール

<br>
//############################################################################
@section Install 1.インストール手順
//############################################################################

---
プリビルド版の場合は、GitHubの[relases](https://github.com/yossi-tahara/Theolizer/releases)からダウンロードしてお好きなフォルダへ解凍して下さい。

ソース版の場合は、GitHubからソースをダウンロード、もしくは、ローカルへclone後、@ref HowToBuildLibrary の手順に従ってビルドとインストールを行って下さい。指定のフォルダへTheolizerがインストールされます。

Theolizerを解凍、もしくは、インストールしたフォルダを以下<b>Theolizerルート・フォルダ</b>と呼びます。）

<br>
//############################################################################
@section Uninstall 2.アンインストール手順
//############################################################################

---

<b>Theolizerルート・フォルダ</b>をフォルダごと、削除して下さい。

<br>
//############################################################################
@section UpdatedTeolizer 3.Theolizer v1.1.0からのアップデートについて
//############################################################################

---

Theolizer v1.1.0まではTheolizerドライバのリプレースが必要でしたが、v1.1.1以降リプレース不要となりました。その結果、コンパイラがリプレースされたままの場合、正常に動作しない可能性があります。<br>
Theolizer v1.1.0以前のものを使っていた方は<b>必ずリストアして下さい。</b>

@subsection RestoreTheolizer 3-1.リストア方法

旧Theolizerを削除する前に下記の手順を実行して下さい。

### Visual Studioの場合
旧Theolizerルート・フォルダのrestore.batをexplorerから起動して下さい。

### その他（gcc/mingw)の場合
コマンド・プロンプト、もしくは、ターミナルで下記コマンドを実行して下さい。

    $ cd <旧Theolizerルート・フォルダ>/bin
    $ TheolizerDriver "--theolizer-restore=コンパイラのパス"

*/
