//############################################################################
/*!
    @brief      ドキュメント・ファイル－使用方法（個別）
    @ingroup    Documents
    @file       7e.support_stl.h
    @author     Yoshinori Tahara
    @date       2017/02/08 Created
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
    @page SupportSTL スマート・ポインタ、および、標準コンテナ・サポートの使い方
ここでは、スマート・ポイントと標準コンテナをシリアライズするための機能について説明します。

基本は単にシリアライズするだけです。特別な操作は必要ありませんが、コンテナについては２つ応用的な使い方があります。

- コンテナへ保存する要素をオブジェクト追跡する場合
- 保存先指定機能(@ref Destinations )により分割保存したコンテナのデータを合成する場合

<br>
//############################################################################
@section HowToUseSmartPointer 1.スマート・ポインタのシリアライズ方法
//############################################################################

Theolizerはスマート・ポインタが管理するポインタをオーナー指定ポインタとしてシリアライズします。シリアライズする手順は通常のクラスと同じですが、少し注意事項があります。

スマート・ポインタをシリアライズするために、theolizer/memory.hをインクルードして下さい。

@code
#include <theolizer/memory.h>
@endcode

- unique_ptr<br>
unique_ptrは通常通りシリアライズ可能です。<br>
なお、`std::unique_ptr<T[]>`はサポートしていません。T[]をシリアライズするためには、その要素数が必要ですが管理されていないため、対応できないのです。<br>

- shared_ptr<br>
同じインスタンスを管理するshared_ptrを保存し回復すると、その状況を回復します。<br>
なお、同じインスタンスを管理するshared_ptrは同じオジェクト追跡単位内でシリアライズするようにして下さい。ポリモーフィズムにより回復処理中にポインタが置き換わった場合、シリアライズされていなかったshared_ptrとの共有状態が解除されます。<br>

- weak_ptr<br>
バインドしているshared_ptrをシリアライズします。ですので、shared_ptrと同様同じインスタンスを管理するものは同じオブジェクト追跡単位内でシリアライズして下さい。

<b>スマート・ポインタの保存と回復処理（source/reference_and_test/basic2/test_support_stl.cpp）</b><br>
unique_ptr、shared_ptr、weak_ptrの保存／回復のサンプル・ソースです。

@snippet  basic2/test_support_stl.cpp SmartPtr

//############################################################################
@section HowToUseContainer 2.標準コンテナのシリアライズ方法
//############################################################################

Theolizerは各種標準コンテナのシリアライズにも対応しています。<br>

- それぞれ対応するインクルードが必要です。
- 一部の標準コンテナについて<br>
保存先指定による合成回復、および、各要素を被ポインタとすること（他のシリアライズ対象のポインタから指す）に対応しています。

下記にその対応表を示します。

|標準コンテナ|ヘッダ|被ポインタ対応|合成回復|
|------------|------|--------------|--------|
|`array`|`<theolizer/array.h>`|YES|YES|
|`vector`|同上|YES|YES|
|`vector<bool>`|`<theolizer/vector.h>`|NO|NO|
|`deque`|`<theolizer/deque.h>`|YES|YES|
|`forward_list`|`<theolizer/forward_list.h>`|YES|YES|
|`list`|`<theolizer/list.h>`|YES|YES|
|`set`|`<theolizer/set.h>`|NO|NO|
|`multiset`|同上|NO|NO|
|`map`|`<theolizer/map.h>`|YES|YES|
|`multimap`|同上|NO|NO|
|`unordered_set`|`<theolizer/unordered_set.h>`|NO|NO|
|`unordered_multiset`|同上|NO|NO|
|`unordered_map`|`<theolizer/unordered_map.h>`|YES|YES|
|`unordered_multimap`|同上|NO|NO|

特記事項が幾つかあります。

- キー無しコンテナは先頭から順番にデータを回復します。<br>
arrayはコンテナを拡張できないので、余分なシリアライス・データは破棄されます。<br>
それ以外のキー無しコンテナ（vector, deque, forward_list, list）はシリアライズ・データの方が多い場合、コンテナを拡張して回復します。<br>
両者ともシリアライズ・データの方が少ない場合、コンテナの余分な要素はそのまま保持されます。<br>
<br>
- mapとunordered_mapは合成回復に対応しています。<br>
これらはキー基準で回復しますので、保存先指定機能で分割したファイルを別々に編集した場合でも、同じキーのデータを同じインスタンスへ合成回復できます。<br>
<br>
- setとunordered_setについて<br>
これらは回復する際、シリアライズ・データを新規にコンストラクトしたインスタンスへ回復してからコンテナへ追加します。ですので、対応するシリアライズ・データがあった要素は単純回復（非合成回復）します。対応するシリアライズ・データがなかったコンテナの要素はそのまま維持されます。<br>
<br>
- キーの重複を許すコンテナについて<br>
multiset, multimap, unordered_multiset, unordered_multimapは、回復する際、一旦コンテナをクリアしてから、シリアライズ・データを回復します。これはキーの重複を許すため、シリアライズ・データと対応するコンテナの要素を特定できないためです。<br>

@subsection HowToUseContainer21 2-1.標準コンテナを通常の使い方で保存／回復するサンプル・ソース

<b>サンプル・ソース（source/reference_and_test/basic2/test_support_stl.cpp）</b><br>

@snippet  basic2/test_support_stl.cpp ContaierNomal

@subsection HowToUseContainer22 2-2.標準コンテナの要素を被ポインタとする保存／回復するサンプル・ソース

<b>サンプル・ソース（source/reference_and_test/basic2/test_support_stl.cpp）</b><br>

@snippet  basic2/test_support_stl.cpp ContaierPointee

@subsection HowToUseContainer23 2-3.標準コンテナの要素を合成回復サンプル・ソース

<b>サンプル・ソースsource/reference_and_test/basic2/test_support_stl.cpp）</b><br>

@snippet  basic2/test_support_stl.cpp ContaierDestinations

<br>
//############################################################################
@section TestContainer 3.網羅的な使用例（自動テスト）の説明
//############################################################################

スマート・ポインタと標準コンテナのサポートについて、全てのシリアライザ、全ての書式でテストしています。
また、保存先指定で分割保存後、合成回復のテストも同様に全てのシリアライザ、全ての書式でテストしています。

@subsection TestContainer31 3-1.スマート・ポインタ

@subsubsection TestContainer311 3-1-1.通常

unique_ptrとweak_ptrについては、使い方説明にて保存／回復テストしています。

オーナー指定ポインタを回復する時、例えばある派生クラスAのインスタンスを指しているポインタへ異なる派生クラスBを回復する場合があります。その時、元の派生クラスAのインスタンスは解放し、派生クラスBのインスタンスをコンストラクトして回復します。
しかし、shared_ptrは他のshared_ptrとポイント先を共有しているため元のインスタンスを解放できません。そのための対策を実装していいます。（shared_ptr回復中フラグを設け、元のインスタンスはshared_ptr処理側で管理しています。)
その機能のテストのため、回復先のポインタが下記の３種類についてテストします。
- nullptrの時
- 回復するものと同じインスタンスを指している時
- 回復するものと異なるインスタンスを指している時

この時のシリアライズ対象クラスはSmartBaseとそれを派生したSmartDerivedです。

そして、それぞれについて、手動(トップ・レベル)、自動、手動(非トップ・レベル)による保存／回復をテストしています。
そのための補助クラスとして、SmartTestAutoとSmartTestManualを使っています。

<b>source/reference_and_test/basic2/test_support_stl.cpp</b>でテスト関数を定義してます。<br>

1. 保存処理<br>
template<class tSerializer><br>
void saveSupportStl(tSerializer& iSerializer)の前半<br>
<br>

2. 回復処理<br>
template<class tSerializer><br>
void loadSupportStl(tSerializer& iSerializer)の前半<br>

@subsubsection TestContainer312 3-1-2.合成回復

unique_ptrとshared_ptrについて、それが管理するクラスをDestAとDestBに分割して保存し合成回復できることを確認しています。
<b>source/reference_and_test/basic2/test_support_stl.cpp</b>でテスト関数を定義してます。<br>

1. 保存処理<br>
template<class tSerializer><br>
void saveSupportStlDestinations(tSerializer& iSerializer)の前半<br>
<br>

2. 回復処理<br>
template<class tSerializer><br>
void saveSupportStlDestinations(tSerializer& iSerializer)の前半<br>

@subsection TestContainer32 3-2.標準コンテナのテスト

@subsubsection TestContainer321 3-2-1.通常

サポートしている全てのコンテナについて、下記について正しく保存／回復できることをテストしています。

- 要素の型はint型とクラス（TestStl）
- 被ポインタをサポートしているコンテナについてはオブジェクト追跡

unorderedコンテナに対応するため、TestStlはstd::hashクラスを特殊化しています。

下記関数テンプレートを使っています。

|対象コンテナ|関数テンプレート|
|------------|----------------|
|array|saveContainerFixed, loadContainerFixed|
|vector(bool以外), deque, list, forward_list|saveContainer, loadContainer|
|set, multiset, unordered_set, unordered_multiset|saveContainerSet, loadContainerSet|
|map, multimap, unordered_map, unordered_multimap|saveContainerMap, loadContainerMap|

<b>source/reference_and_test/basic2/test_support_stl.cpp</b>でテスト関数を定義してます。<br>

1. 保存処理<br>
template<class tSerializer><br>
void saveSupportStl(tSerializer& iSerializer)の後半<br>
<br>

2. 回復処理<br>
template<class tSerializer><br>
void loadSupportStl(tSerializer& iSerializer)の後半<br>

@subsubsection TestContainer322 3-2-2.合成回復

合成回復をサポートしている全てのコンテナについて、下記について正しく保存／回復できることをテストしています。

- 要素の型はクラス（TestStlDestinations）<br>
TestStlDestinationsは保存先としてDestAとDestBを指定したメンバを定義しています。

下記関数テンプレートを使っています。

|対象コンテナ|関数テンプレート|
|------------|----------------|
|array|saveFixed, loadFixed|
|vector(bool以外), deque, list, forward_list|saveNoKey, loadNoKey|
|map, unordered_map|saveKey, loadeKey|

<b>source/reference_and_test/basic2/test_support_stl.cpp</b>でテスト関数を定義してます。<br>

1. 保存処理<br>
template<class tSerializer><br>
void saveSupportStlDestinations(tSerializer& iSerializer)の後半<br>
<br>

2. 回復処理<br>
template<class tSerializer><br>
void loadSupportStlDestinations(tSerializer& iSerializer)の後半<br>

*/
