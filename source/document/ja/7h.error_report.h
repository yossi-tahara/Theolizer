//############################################################################
/*!
    @brief      ドキュメント・ファイル－使用方法（個別）
    @ingroup    Documents
    @file       7h.error_report.h
    @author     Yoshinori Tahara
    @date       2017/05/03 Created
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
    @page ErrorReport エラー報告
ここではTheolizerが検出するエラーの受け取り方とその後処理について説明します。

<br>
//############################################################################
@section ErrorReport1 1.Theolizerエラー・クラス
//############################################################################

@subsection ErrorInfo 1-1.ErrorInfoクラス
Theolzierのエラーは report.h で定義している theolizer::ErrorInfo クラスで表現します。<br>
主な公開メンバは以下の通りです。<br>

|メンバ名|意味|
|--------|----|
|theolizer::ErrorType getErrorType() const;|エラー種別コード返却|
|theolizer::ErrorKind getErrorKind() const;|エラー分類コード返却|
|std::string getString() const;|全エラー情報を含むエラー・メッセージ返却|
|bool isError() const;|エラーならtrue|
|operator bool() const;|エラーか警告ならtrue|
|bool operator!() const;|エラーも警告もないならtrue|


@subsection ErrorType 1-2.ErrorType枚挙型
<b>theolizer::ErrorType</b>はenum型で以下のシンボルを持ちます。
|シンボル名|意味|
|----------|----|
|None|エラーでも警告でも無い|
|Warning|警告|
|Error|エラー|

@subsection ErrorKind 1-3.ErrorKind枚挙型
<b>theolizer::ErrorKind</b>はenum型で以下のシンボルを持ちます。
|シンボル名|意味|
|----------|----|
|Unclassified|未分類|
|WrongUsing|使い方間違い|
|IOError|I/O動作エラー|
|UnknownData|非対応シリアライズ・データ・フォーマット|
|UnknownVerson|非対応バージョン（未知のグローバル・バージョン番号）|

@subsection getString 1-4.ErrorInfoのgetString()のフォーマット
ErrorInfoのgetString()で取得できる文字列は次のようなフォーマットです。<br>
@code
ErrorType(ErrorKind),エラーを検出したインスタンス{ソース・ファイル名(行番号)} : エラー・メッセージ
@endcode
エラーを検出したインスタンス直後の"{ソース・ファイル名(行番号)}"は可能な時のみ表示されます。

<b>シリアライザのコンストラクタでエラー検出した時（型チェック・エラー）の例</b><br>
@code
Error(UnknownData),IntrusiveBase2.mULongLong : Unmatch type.{core_serializer.cpp(1570)}
@endcode
なお、エラー・メッセージ含まれるソース・ファイルと行番号は、当該エラーを検出したTheolizerのソースと行番号です。

<b>THEOLIZER_PROCESS()等のマクロ内でエラー検出した時の例</b><br>
@code
Error(UnknownData),aInt{test_basic_process.cpp(212)} : Logical Error on stream.
@endcode

<br>
//############################################################################
@section ErrorReport2 2.Theolizerで検出したエラーの記録
//############################################################################

Theolizerは２種類のエラー記録を持っています。
どちらとも複数回エラーか警告が発生した場合、最後のエラーが記録されます。<br>
ただし、エラー記録を優先するため、エラー発生後の警告はエラーが解除(resetError)されるまで記録されません。

@subsection SerializerInstance 2-1.シリアライザのインスタンス
シリアライザ・インスタンス毎に当該シリアライザで発生したエラー（もしくは警告）を保持します。<br>
警告ではないエラーが発生している場合、シリアライザへの要求は全て処理されず、WrongUsingとなります。<br>
回復するためには、ストリームの再同期とresetError()呼び出しが必要です。<br>
<br>
エラー情報に関するメンバ関数は「@ref MemberFunctions 」を参照下さい。<br>

@subsection ErrorReporter 2-2.ErrorReporterシングルトン
theolizer::ErrorReporterはthread_localなシングルトンですのでスレッド毎に一つだけ存在します。<br>
当該スレッドで発生した最後のエラーか警告を保持します。<br>
シングルトンですのでコンストラクト、コピー、ムーブできません。<br>
<br>
以下のstatic関数が公開されています。<br>

|メンバ名|意味|
|--------|----|
|static theolizer::ErrorInfo const& getErrorInfo() noexcept;|エラー情報を返却します。|
|static bool isError() noexcept;|エラーが発生している時trueを返却します。|
|static void resetError() noexcept;|エラー状態を解除します。（@ref ErrorReport3 参照）|

<br>
//############################################################################
@section ErrorReport3 3.Theolizerで検出したエラーの通知方法
//############################################################################

シリアライザからのエラー通知方法は２つあります。

1. 例外許可<br>
iNoThrowExceptionバラメータをfalse（デフォルト）として、シリアライザをコンストラクトすると例外が許可されます。<br>
エラーを検出した時に theolizer::ErrorInfo を投げます。<br>
<br>
2. 例外禁止<br>
iNoThrowExceptionバラメータをtrueにして、シリアライザをコンストラクトすると例外が禁止されます。<br>
エラーを検出しても例外を投げません。<br>

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
<b>シリアライザにエラー状態が残ったままデストラクトしないで下さい</b><br>
例外を禁止している時は、必ずエラーを受け取りresetError()してからデストラクトするようにして下さい。「@ref ErrorReport6 」参照<br>
エラーの受け取り忘れを防止するため、エラーが発生しているのにresetError()せずにデストラクトするとプログラムを強制終了させています。<br>
なお、例外許可時はエラーの受け取り損ないは発生しないためresetError()してなくても強制終了しません。
</div>

<br>
//############################################################################
@section ErrorReport4 4.エラー状態の回復方法
//############################################################################

シリアライザのエラー状態を解除するには、下記の手順で行います。<br>

1. シリアライザへ渡しているストリームを同期する。<br>
次に読むべきデータ先頭から読み出せる状態にします。<br>
この処理はユーザ・プログラムにて行って下さい。<br>
<br>
2. シリアライザのエラーをリセットする。<br>
シリアライザ・インスタンス.resetError()により行います。<br>
<br>

ただし、一般にファイルからの回復時にエラーが発生した時の原因は下記ですので、ファイルへの保存／回復の場合はストリームをオープンしたままエラー回復する操作を行うことは原則としてありません。<br>

- プログラムのバグ(Theolizerが非対応な変更をしたプログラムで回復しようとした。)
- ファイル指定ミス(指定したフォーマットと実際のフォーマットの不一致)
- ファイル破壊

通信による受信時にエラーが発生した時は、再同期することでリカバリすることが可能です。<br>
なお、通信回線の同期はTheolizerによるサポートはありません。ユーザ・プログラムにて再同期して下さい。<br>
<br>

<br>
//############################################################################
@section ErrorReport5 5.エラー・ログ
//############################################################################

TheolizerはTheolizerで検出したエラーをログ・ファイルへ記録する機能をサポートしています。<br>

@subsection THEOLIZER_ERRORLOG_FILE 5-1.THEOLIZER_ERRORLOG_FILE()マクロの指定方法

THEOLIZER_ERRORLOG_FILE()マクロをユーザ・プログラム内で１箇所だけ書くことで有効になります。<br>

グローバル名前空間で以下を記述します。
@code
THEOLIZER_ERRORLOG_FILE("LogFilePath%1%.log");
@endcode

- "LogFilePath"の部分でエラー・ログのパスを指定します。<br>
相対パスで指定した場合、プログラム起動時のカレント・フォルダからの相対になります。<br>
<br>
- "%1%"はTheolizerがログ・ファイルの番号として使います。<br>
ログ・ファイル番号は0開始です。<br>
ログ・ファイルはデフォルトでは1MBytesを超えると次のログ・ファイルへ切り替わります。その時、ログ・ファイル番号をインクリメントします。<br>
ただし、デフォルトでは最大2個までに制限していますので、ログ・ファイル番号1の次は0のログ・ファイルへ「上書き」します。<br>
これにより、ほっておいてもログ・ファイルが無限に増えることを防止しています。<br>
<br>
- ".log"は自由に指定して下さい。<br>
<br>

@subsection LogFileFormat 5-2.ログ・ファイルのフォーマット

ログ・ファイルは後処理するのに便利のため簡易的なcsvフォーマットにしています。以下の通りです。<br>

- 先頭行<br>
シーケンシャル番号です。これはログ・ファイルを生成する度に1つ増えます。<br>
unsigned型で循環します。<br>
<br>
- ２行目以降<br>
@code
,年-月-日,時間:分:秒.mSec,同期待ち時間(uSec),スレッドID,ErrorType(ErrorKind),エラー検出インスタンス : エラー・メッセージ
@endcode

<b>ログ・ファイル例：</b><br>
@code
0
,2017-05-04,13:32:38.671,000001,___________12712,Error(UnknownData),IntrusiveBase2.mULongLong : Unmatch type.{core_serializer.cpp(1570)}
,2017-05-04,13:32:38.673,000000,___________12712,Error(UnknownData),IntrusiveBase2.mULongLong : Unmatch type.{core_serializer.cpp(1570)}
,2017-05-04,13:32:38.673,000000,___________12712,Error(WrongUsing),mIntrusiveBase2{test_serializer_base02.cpp(91)} : Unmatch type.
,2017-05-04,13:32:38.673,000000,___________12712,Error(UnknownData),IntrusiveBase2.mULongLong : Unmatch type.{core_serializer.cpp(1654)}
,2017-05-04,13:32:38.674,000000,___________12712,Error(UnknownData),IntrusiveBase2.mULongLong : Unmatch type.{core_serializer.cpp(1654)}
,2017-05-04,13:32:38.674,000000,___________12712,Error(WrongUsing),mIntrusiveBase2{test_serializer_base02.cpp(123)} : Unmatch type.
@endcode

@subsection WaitSync 5-3.同期待ち時間について
エラー・ログ出力は、マルチ・スレッドに対応しています。<br>
他のスレッドがエラー・ログ出力処理中に出力しようとした場合、後から出力する方は待たされます。<br>
その待ち時間を uSec で記録します。<br>
<br>

@subsection ThreadId 5-4.スレッドIDについて
当該ログを出力したスレッドIDを`std::this_thread::get_id()`で獲得して記録します。<br>

<br>
//############################################################################
@section ErrorReport6 6.サンプル・ソース
//############################################################################

<b>サンプル・ソース（source/reference_and_test/basic/test_basic_process.cpp）</b><br>
@snippet  basic/test_basic_process.cpp ErrorReport

これの出力は以下のようになります。

@code
exception    : Error(UnknownData),aInt{test_basic_process.cpp(241)} : Logical Error on stream.
no-exception : Error(UnknownData),ashort{test_basic_process.cpp(256)} : Logical Error on stream.
@endcode

*/
