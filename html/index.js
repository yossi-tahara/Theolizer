var index =
[
    [ "概要", "_abstract.html", [
      [ "1.Theolizerとは", "_abstract.html#What", null ],
      [ "2.対応環境", "_abstract.html#Platforms", null ],
      [ "3.Theolizerの特長", "_abstract.html#Features", [
        [ "3-1.自動シリアライズ", "_abstract.html#Features31", null ],
        [ "3-2.保存先指定", "_abstract.html#Features32", null ],
        [ "3-3.オブジェクト追跡", "_abstract.html#Features33", null ],
        [ "3-4.プログラム変更対応", "_abstract.html#Features34", null ]
      ] ],
      [ "4.Theolizerの仕組み", "_abstract.html#Mechanism", [
        [ "4-1.ドライバ", "_abstract.html#Mechanism41", null ],
        [ "4-2.ライブラリ", "_abstract.html#Mechanism42", null ]
      ] ]
    ] ],
    [ "仕様", "_specification.html", [
      [ "1.基本仕様", "_specification.html#Basic", [
        [ "1-1.シリアライズ可能な型", "_specification.html#Basic11", null ],
        [ "1-2.シリアライズ可能な型についての補足事項", "_specification.html#Basic12", [
          [ "1-2-1.クラスについて", "_specification.html#Basic121", null ],
          [ "1-2-2. enum型について", "_specification.html#Basic122", null ]
        ] ],
        [ "1-3.保存先指定", "_specification.html#Basic13", null ],
        [ "1-4.現在対応しているフォーマット、STLなどについて", "_specification.html#Basic14", null ],
        [ "1-5.その他", "_specification.html#Basic15", [
          [ "1-5-1.自動生成コードの生成場所", "_specification.html#Basic151", null ],
          [ "1-5-2.シリアライズ処理を行っていないコンパイル単位について", "_specification.html#Basic152", null ],
          [ "1-5-3.エラー報告について", "_specification.html#Basic153", null ],
          [ "1-5-4.スレッド安全性", "_specification.html#Basic154", null ],
          [ "1-5-5.処理系間でのデータ交換について", "_specification.html#Basic155", null ]
        ] ],
        [ "1-6.制約事項", "_specification.html#Basic16", [
          [ "1-6-1.デフォルト・コンストラクタが必要", "_specification.html#Basic161", null ],
          [ "1-6-2.クラスのstatic変数について", "_specification.html#Basic162", null ],
          [ "1-6-3.仮想基底クラス", "_specification.html#Basic163", null ]
        ] ]
      ] ],
      [ "2.オブジェクト追跡", "_specification.html#Tracking", [
        [ "2-1.オブジェクト追跡する領域について", "_specification.html#Basic21", null ],
        [ "2-2.ポインタ型のアドレス回復について", "_specification.html#Basic22", null ],
        [ "2-3.ポリモーフィズム対応", "_specification.html#Basic23", null ]
      ] ],
      [ "3.バージョン変更", "_specification.html#Modifying", [
        [ "3-1.クラス定義とenum定義の変更", "_specification.html#Basic31", null ],
        [ "3-2.バージョン番号管理について", "_specification.html#Basic32", null ],
        [ "3-3.クラス・メンバ変数のキープ・ステップ", "_specification.html#Basic33", null ],
        [ "3-4.保存先指定等の要因で一部のメンバが回復されていない時", "_specification.html#Basic34", null ],
        [ "3-5.旧バージョン処理時、メンバが回復されなかった時に発生する問題点対応", "_specification.html#Basic35", null ],
        [ "3-6.up/downVersion関数でポインタの先を変更してはいけません", "_specification.html#Basic36", null ]
      ] ]
    ] ],
    [ "インストール／アンインストール", "_prepare.html", [
      [ "1.インストール手順", "_prepare.html#Install", null ],
      [ "2.アンインストール手順", "_prepare.html#Uninstall", null ]
    ] ],
    [ "Theolizerの組み込み方とファイル構成", "_how_to_make_project.html", [
      [ "1.Theolizerライブラリの選択", "_how_to_make_project.html#Selecting", null ],
      [ "2.CMakeを使う場合", "_how_to_make_project.html#CMake", null ],
      [ "3.exampleのビルド例(CMake使用)", "_how_to_make_project.html#Example", null ],
      [ "4.CMakeを使わない場合", "_how_to_make_project.html#NonCMake", null ],
      [ "5.ファイル構成", "_how_to_make_project.html#FileStructure", null ]
    ] ],
    [ "Theolizerのビルド手順", "_how_to_build_theolizer.html", [
      [ "1.Theolizerライブラリのビルド方法", "_how_to_build_theolizer.html#HowToBuildLibrary", [
        [ "1-1.使用するツールとリンクするライブラリ", "_how_to_build_theolizer.html#HowToBuildLibrary11", null ],
        [ "1-2.ビルド手順", "_how_to_build_theolizer.html#HowToBuildLibrary12", [
          [ "1-2-1.boostのsystemとfilesystemをビルドしてインストール", "_how_to_build_theolizer.html#HowToBuildLibrary121", null ],
          [ "1-2-2.Theolizerライブラリをビルドしてインストール", "_how_to_build_theolizer.html#HowToBuildLibrary122", null ]
        ] ]
      ] ],
      [ "2.Theolizerドライバのビルド方法", "_how_to_build_theolizer.html#HowToBuildDriver", [
        [ "2-1.使用するツールとリンクするライブラリ", "_how_to_build_theolizer.html#HowToBuildLibrary21", null ],
        [ "2-2.ビルド手順", "_how_to_build_theolizer.html#HowToBuildLibrary22", [
          [ "2-2-1.boostのsystemとfilesystemをビルドしてインストール", "_how_to_build_theolizer.html#HowToBuildLibrary221", null ],
          [ "2-2-2.LLVM(libTooling含む)をビルドしてインストール", "_how_to_build_theolizer.html#HowToBuildLibrary222", null ],
          [ "2-2-3.Theolizerライブラリをビルドしてインストール", "_how_to_build_theolizer.html#HowToBuildLibrary223", null ]
        ] ]
      ] ],
      [ "3.バージョン番号の付け方", "_how_to_build_theolizer.html#VersionNo", null ],
      [ "4.ドキュメントについて補足", "_how_to_build_theolizer.html#Documents", null ]
    ] ],
    [ "使用方法（全体）", "_usage_total.html", [
      [ "1.名前の付け方", "_usage_total.html#Naming", [
        [ "1-1.名前空間", "_usage_total.html#Namespace", null ],
        [ "1-2.マクロ名", "_usage_total.html#MacroName", null ]
      ] ],
      [ "2.テスト・プログラムの構造", "_usage_total.html#TestProgram", [
        [ "2-1.構造について", "_usage_total.html#TestProgramStructure", null ],
        [ "2-2.説明で用いるマクロについて", "_usage_total.html#MacroForDescription", null ]
      ] ],
      [ "3.基本的な使い方", "_usage_total.html#BasicUsage", [
        [ "3-1.Theolizerヘッダをインクルード", "_usage_total.html#TheolizerHeader", null ],
        [ "3-2.シリアライズするクラスとenum型の定義", "_usage_total.html#DefineClassEnum", null ],
        [ "3-3.Theolizerの自動生成ファイルをインクルード", "_usage_total.html#IncludeAutoGeneratedFile", null ],
        [ "3-4.シリアライズ処理", "_usage_total.html#SerializingProcess", [
          [ "3-4-1.シリアライザ・インスタンスの生成", "_usage_total.html#ConstructSerializer", null ],
          [ "3-4-2.シリアライズ処理要求", "_usage_total.html#Request", null ],
          [ "3-4-3.シリアライザ・インスタンスの破棄", "_usage_total.html#DestructSerializer", null ]
        ] ]
      ] ]
    ] ],
    [ "使用方法（個別）", "_usage_individual.html", "_usage_individual" ],
    [ "APIリファレンス", "_api_reference.html", null ]
];