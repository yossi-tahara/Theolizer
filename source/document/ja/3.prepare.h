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

下記2つの手順で行います。

1. Theolizerの用意
2. ドライバのリプレース

// ***************************************************************************
@subsection PrepareTheolizer 1-1.Theolizerの用意
// ***************************************************************************
プリビルド版の場合は、GitHubの[relases](https://github.com/yossi-tahara/Theolizer/releases)からダウンロードしてお好きなフォルダへ解凍して下さい。

ソース版の場合は、GitHubからソースをダウンロード、もしくは、ローカルへclone後、@ref HowToBuildLibrary の手順に従ってビルドとインストールを行って下さい。指定のフォルダへTheolizerがインストールされます。

Theolizerを解凍、もしくは、インストールしたフォルダを以下<b>Theolizerルート・フォルダ</b>と呼びます。）

// ***************************************************************************
@subsection ReplaceDriver 1-2.ドライバのリプレース
// ***************************************************************************

<b>Theolizerルート・フォルダ</b>/binにてコマンド・プロンプト(Windows) かターミナル（linux）を起動し、下記を実行します。

    TheolizerDriver "--theolizer-replace=コンパイラのパス"

ターゲットのコンパイラが、<b>コンパイラ名RenamedByTheolizer</b>へリネームされ、TheolizerDriverが<b>コンパイラ名</b>へコピーされます。<br>
コンパイラを置き換えますが、これによる悪影響はない筈です。@ref Mechanism の「ドライバ動作の補足」をご参照下さい。<br>
（もし、悪影響か見つかった場合は、お手数ですがその再現手順を含めて当リボジトリのIssuesへご報告頂けますようお願い致します。）

例えば、<b>`C:\mingw-w64\x86_64-5.4.0-posix-seh-rt_v5-rev0\mingw64\bin`</b>にMinGW 5.4.0がインストールされている場合、下記コマンドでにてコンパイラをリプレースします。

    TheolizerDriver.exe "--theolizer-replace=C:\mingw-w64\x86_64-5.4.0-posix-seh-rt_v5-rev0\mingw64\bin\g++.exe"

<br>
<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
Visual Strudio C++については、<b>Theolizerルート・フォルダ</b>にreplace.batを用意しています。これを起動することで上記のリプレース操作を行います。
</div>

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
linuxにてコンパイラのパスとしてシンボリック・リンクを指定した場合、シンボリック・リンクを追跡してコンパイラ実体のファイルを置き換えます。
</div>


<br>
//############################################################################
@section Uninstall 2.アンインストール手順
//############################################################################

---

下記2つの手順で行います。

1. ドライバのリストア
2. Theolizerの削除

// ***************************************************************************
@subsection RestoreDriver 2-1.ドライバのリストア
// ***************************************************************************

リプレース時のreplaceをrestoreへ置き換えて操作して下さい。

    TheolizerDriver "--theolizer-restore=コンパイラのパス"

コンパイラのファイル名にてコピーされていた<b>TheolizerDriver</b>が削除され、<b>コンパイラ名RenamedByTheolizer</b>が元の<b>コンパイラ名</b>へリネームされます。また、もし、指定したコンパイラがTheolizerDriverでなかった場合は何もしません。

<br>
<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
 Visual Strudio C++については、<b>Theolizerルート・フォルダ</b>にrestore.batを用意しています。これを起動することで上記のリストア操作を行います。
</div>

// ***************************************************************************
@subsection DeleteTheolizer 2-2.Theolizerの削除
// ***************************************************************************
ドライバのリストア確認後、<b>Theolizerルート・フォルダ</b>をフォルダごと、削除して下さい。

*/
