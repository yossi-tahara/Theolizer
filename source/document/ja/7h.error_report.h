//############################################################################
/*!
    @brief      ドキュメント・ファイル－使用方法（個別）
    @ingroup    Documents
    @file       7h.error_report.h
    @author     Yoshinori Tahara
    @date       2017/05/03 Created
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
    @page ErrorReport エラー報告
ここではTheolizerが検出するエラーの受け取り方とその後処理について説明します。

<br>
//############################################################################
@section ErrorReport1 1.Theolizerエラー・クラス
//############################################################################

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

<b>theolizer::ErrorType</b>はenum型で以下のシンボルを持ちます。
|シンボル名|意味|
|----------|----|
|None|エラーでも警告でも無い|
|Warning|警告|
|Error|エラー|

<b>theolizer::ErrorKind</b>はenum型で以下のシンボルを持ちます。
|シンボル名|意味|
|----------|----|
|Unclassified|未分類|
|WrongUsing|使い方間違い|
|IOError|I/O動作エラー|
|UnknownData|非対応シリアライズ・データ・フォーマット|
|UnknownVerson|非対応バージョン（未知のグローバル・バージョン番号）|



<br>
//############################################################################
@section ErrorReport2 2.Theolizerで検出したエラーの記録場所
//############################################################################

Theolizerは大きく２つのエラー記録場所を持っています。

1. シリアライザのインスタンス<br>
シリアライザ・インスタンス.getError()で取得できます。<br>

2. theolizer::ErrorReporterのstatic領域<br>
theolizer::ErrorReproter::getError()で取得できます。<br>
このstatic領域はthread_localですのでスレッド毎に唯一存在します。<br>





<br>
@subsection ErrorNotify11 1-1.

    エラー状態の解除は、下記のように、resetError()で行って下さい。<br>

@code
{
    std::ofstream   aStream("example.json");
    theolizer::JsonOSerializer<> js(aStream, theolizer::CheckMode::NoTypeCheck, false, true);

    THEOLIZER_PROCESS(js, ...);

    // エラー・チェック(例外を禁止している場合に必要)
    theolizer::ErrorInfo aErrorInfo=js.getError();
    if (aErrorInfo)
    {
        std::cout << aErrorInfo.getMessage() << std::endl;
        js.resetError();    // エラー状態を解除する
    }
}
@endcode

# T.B.D.
*/
