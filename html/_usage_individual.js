var _usage_individual =
[
    [ "THEOLIZER_PROCESS()の使用方法", "_basic_process.html", [
      [ "1.クラスとenum型の定義", "_basic_process.html#DefinitionEnumClass", null ],
      [ "2.網羅的な使用例（自動テスト）の説明", "_basic_process.html#TestBasicProcess", null ]
    ] ],
    [ "クラスのバリエーション", "_class_variation.html", [
      [ "1.非侵入型完全自動クラス", "_class_variation.html#FullAutoClass", null ],
      [ "2.侵入型半自動クラス", "_class_variation.html#HalfAutoClass", null ],
      [ "3.非侵入型手動クラス", "_class_variation.html#ManualClass", [
        [ "3-1.非侵入型手動クラスの定義例", "_class_variation.html#DefineManualClass", null ],
        [ "3-2.非侵入型手動クラスのシリアライズ指定", "_class_variation.html#SpecifyManualClass", null ],
        [ "3-3.自動生成される保存／回復処理用関数の雛形例", "_class_variation.html#TemplateManualClass", null ],
        [ "3-4.クラス定義雛形のコピー", "_class_variation.html#CopyManualClass", null ],
        [ "3-5.保存／回復処理関数の記述", "_class_variation.html#WriteManualClass", null ],
        [ "3-6.保存／回復処理例", "_class_variation.html#SaveLoadManualClass", null ]
      ] ],
      [ "4.それらを派生したり包含したりしたクラス", "_class_variation.html#DerivedIncluded", null ],
      [ "5.網羅的な使用例（自動テスト）の説明", "_class_variation.html#TestClassVariation", [
        [ "5-1.単独テスト", "_class_variation.html#SingleTest", null ],
        [ "5-2.二重組み合わせテスト", "_class_variation.html#DoubleTest", [
          [ "5-2-1.二重組み合わせテスト用のヘルパー・クラス", "_class_variation.html#DoubleTestHelper", null ],
          [ "5-2-2.二重組み合わせテスト本体", "_class_variation.html#DoubleTestImpl", null ]
        ] ],
        [ "5-3.三重組み合わせテスト", "_class_variation.html#TripledTest", null ]
      ] ]
    ] ],
    [ "enum型のバリエーション", "_enum_variation.html", [
      [ "1.非侵入型完全自動enum型", "_enum_variation.html#FullAutoEnum", null ],
      [ "2.非侵入型半自動enum型", "_enum_variation.html#HalfAutoEnum", null ],
      [ "3.網羅的な使用例（自動テスト）の説明", "_enum_variation.html#TestEnumVariation", null ]
    ] ],
    [ "オブジェクト追跡について", "_object_tracking.html", [
      [ "1.オブジェクト追跡の使い方", "_object_tracking.html#HowToObjectTracking", [
        [ "1-1.指定方法", "_object_tracking.html#HowToObjectTracking11", null ],
        [ "1-2.サンプル・ソース", "_object_tracking.html#HowToObjectTracking12", null ]
      ] ],
      [ "2.ポリモーフィズムの使い方", "_object_tracking.html#Polymorphism", [
        [ "3-1.使い方", "_object_tracking.html#Polymorphism31", null ],
        [ "3-2.参照を経由する場合", "_object_tracking.html#Polymorphism32", null ]
      ] ],
      [ "3.オブジェクト追跡の仕組み", "_object_tracking.html#ClearTracking", [
        [ "3-1.ポインタをシリアライズする仕組み", "_object_tracking.html#HowToObjectTracking31", null ],
        [ "3-2.オブジェクト追跡の課題", "_object_tracking.html#HowToObjectTracking32", null ],
        [ "3-3.オブジェクト追跡で用いる用語", "_object_tracking.html#HowToObjectTracking33", null ],
        [ "3-4.オブジェクト追跡対象について", "_object_tracking.html#HowToObjectTracking34", null ],
        [ "3-5.オブジェクト追跡単位について", "_object_tracking.html#HowToObjectTracking35", null ]
      ] ],
      [ "4.オブジェクト追跡の網羅的な使用例（自動テスト）の説明", "_object_tracking.html#TestObjectTracking", [
        [ "4-1.各種メモリへのポインタのテスト", "_object_tracking.html#TestObjectTracking41", [
          [ "4-1-1.概要", "_object_tracking.html#TestObjectTracking411", null ],
          [ "4-1-2.ソース・コード", "_object_tracking.html#TestObjectTracking412", null ]
        ] ],
        [ "4-2.ポインタ(非オーナー)のテスト", "_object_tracking.html#TestObjectTracking42", [
          [ "4-2-1.概要", "_object_tracking.html#TestObjectTracking421", null ],
          [ "4-2-2.ソース・コード", "_object_tracking.html#TestObjectTracking422", null ]
        ] ],
        [ "4-3.オーナー・ポインタのテスト", "_object_tracking.html#TestObjectTracking43", [
          [ "4-3-1.概要", "_object_tracking.html#TestObjectTracking431", null ],
          [ "4-3-2.ソース・コード", "_object_tracking.html#TestObjectTracking432", null ]
        ] ],
        [ "4-4.同じインスタンスを複数回シリアライズするテスト", "_object_tracking.html#TestObjectTracking44", null ]
      ] ],
      [ "5.ポリモーフィズムの網羅的な使用例（自動テスト）の説明", "_object_tracking.html#TestPolymorphism", null ]
    ] ],
    [ "保存先指定について", "_destinations.html", [
      [ "1.指定方法", "_destinations.html#HowToSpecifySaving", [
        [ "1-1.シリアライズの有無指定", "_destinations.html#HowToSpecifySaving11", null ],
        [ "1-2.保存先指定", "_destinations.html#HowToSpecifySaving12", null ],
        [ "1-3.指定方法", "_destinations.html#HowToSpecifySaving13", [
          [ "1-3-1.保存先の定義方法", "_destinations.html#HowToSpecifySaving131", null ],
          [ "1-3-2.デフォルト保存／非保存クラスの指定方法", "_destinations.html#HowToSpecifySaving132", null ],
          [ "1-3-3.メンバ変数への指定方法", "_destinations.html#HowToSpecifySaving133", null ],
          [ "1-3-4.シリアライザへの指定方法", "_destinations.html#HowToSpecifySaving134", null ]
        ] ],
        [ "1-4.サンプル・ソース", "_destinations.html#HowToSpecifySaving14", [
          [ "1-4-1.保存の有無指定", "_destinations.html#HowToSpecifySaving141", null ],
          [ "1-4-2.保存先指定", "_destinations.html#HowToSpecifySaving142", null ],
          [ "1-4-3.間違い易い指定について", "_destinations.html#HowToSpecifySaving143", null ]
        ] ]
      ] ],
      [ "2.網羅的な使用例（自動テスト）の説明", "_destinations.html#TestSpecifySaving", [
        [ "2-1.保存の有無指定のテスト", "_destinations.html#TestSpecifySaving21", null ],
        [ "2-2.保存先指定のテスト", "_destinations.html#TestSpecifySaving22", null ]
      ] ]
    ] ],
    [ "スマート・ポインタ、および、標準コンテナ・サポートの使い方", "_support_s_t_l.html", [
      [ "1.スマート・ポインタのシリアライズ方法", "_support_s_t_l.html#HowToUseSmartPointer", null ],
      [ "2.標準コンテナのシリアライズ方法", "_support_s_t_l.html#HowToUseContainer", [
        [ "2-1.標準コンテナを通常の使い方で保存／回復するサンプル・ソース", "_support_s_t_l.html#HowToUseContainer21", null ],
        [ "2-2.標準コンテナの要素を被ポインタとする保存／回復するサンプル・ソース", "_support_s_t_l.html#HowToUseContainer22", null ],
        [ "2-3.標準コンテナの要素を合成回復サンプル・ソース", "_support_s_t_l.html#HowToUseContainer23", null ]
      ] ],
      [ "3.網羅的な使用例（自動テスト）の説明", "_support_s_t_l.html#TestContainer", [
        [ "3-1.スマート・ポインタ", "_support_s_t_l.html#TestContainer31", [
          [ "3-1-1.通常", "_support_s_t_l.html#TestContainer311", null ],
          [ "3-1-2.合成回復", "_support_s_t_l.html#TestContainer312", null ]
        ] ],
        [ "3-2.標準コンテナのテスト", "_support_s_t_l.html#TestContainer32", [
          [ "3-2-1.通常", "_support_s_t_l.html#TestContainer321", null ],
          [ "3-2-2.合成回復", "_support_s_t_l.html#TestContainer322", null ]
        ] ]
      ] ]
    ] ],
    [ "enum型のアップデート／バージョン・アップ方法", "_changing_enum.html", [
      [ "1.バージョン番号を変えないでenum型を修正", "_changing_enum.html#HowToModifyEnum1", [
        [ "1-1.シンボル名保存の変更サンプル", "_changing_enum.html#HowToModifyEnum11", null ],
        [ "1-2.シンボル値保存の変更サンプル", "_changing_enum.html#HowToModifyEnum12", null ]
      ] ],
      [ "2.グローバル・バージョン番号テーブル生成", "_changing_enum.html#HowToMakeGlobalVersionNoTable", [
        [ "2-1.グローバル・バージョン番号テーブルの宣言", "_changing_enum.html#HowToMakeGlobalVersionNoTable21", null ],
        [ "2-2.グローバル・バージョン番号テーブル実体定義", "_changing_enum.html#HowToMakeGlobalVersionNoTable22", null ]
      ] ],
      [ "3.バージョン番号を変えてenum型を修正", "_changing_enum.html#HowToModifyEnum3", [
        [ "3-1.バージョン・アップ手順", "_changing_enum.html#HowToModifyEnum31", null ],
        [ "3-2.非侵入型半自動への変更方法", "_changing_enum.html#HowToModifyEnum32", null ],
        [ "3-3.バージョン・アップ時のシンボル対応", "_changing_enum.html#HowToModifyEnum33", [
          [ "3-3-1.内部処理概要", "_changing_enum.html#HowToModifyEnum331", null ],
          [ "3-3-2.以前使用したシンボルの再割当て", "_changing_enum.html#HowToModifyEnum332", null ],
          [ "3-4.シンボル名保存とシンボル値保存の切り替え", "_changing_enum.html#HowToModifyEnum34", null ]
        ] ]
      ] ],
      [ "4.網羅的な使用例（自動テスト）の説明", "_changing_enum.html#HowToModifyEnum4", null ]
    ] ],
    [ "クラスのアップデート／バージョン・アップ方法", "_changing_class.html", [
      [ "1.バージョン番号を変えないでクラスを修正", "_changing_class.html#HowToModifyClass1", [
        [ "1-1.名前対応の場合", "_changing_class.html#HowToModifyClass11", null ],
        [ "1-2.順序対応の場合", "_changing_class.html#HowToModifyClass12", null ],
        [ "1-3.配列の場合", "_changing_class.html#HowToModifyClass13", null ],
        [ "1-4.違反した場", "_changing_class.html#HowToModifyClass14", null ],
        [ "1-5.サンプル・ソース", "_changing_class.html#HowToModifyClass15", [
          [ "1-1-1.名前対応クラス", "_changing_class.html#HowToModifyClass111", null ],
          [ "1-5-2.順序対応クラス", "_changing_class.html#HowToModifyClass152", null ],
          [ "1-5-3.配列の要素数の変更サンプル", "_changing_class.html#HowToModifyClass153", null ]
        ] ]
      ] ],
      [ "2.グローバル・バージョン番号テーブル生成", "_changing_class.html#HowToMakeGlobalVersionNoTable2", null ],
      [ "3.バージョン番号を変えることで可能な修正", "_changing_class.html#HowToModifyClass3", [
        [ "3-1.名前対応の場合", "_changing_class.html#HowToModifyClass31", null ],
        [ "3-2.順序対応の場合", "_changing_class.html#HowToModifyClass32", null ],
        [ "3-3.配列の場合", "_changing_class.html#HowToModifyClass33", null ]
      ] ],
      [ "4.バージョン番号を変えてクラス修正する仕組み概要", "_changing_class.html#HowToModifyClass4", [
        [ "4-1.コア・データ構造", "_changing_class.html#HowToModifyClass41", null ],
        [ "4-2.補助データ構造", "_changing_class.html#HowToModifyClass42", null ],
        [ "4-3.侵入型半自動におけるバージョン・アップ／ダウン処理", "_changing_class.html#HowToModifyClass43", null ],
        [ "4-4.非侵入型手動におけるバージョン・アップ／ダウン処理", "_changing_class.html#HowToModifyClass44", null ],
        [ "4-5.バージョン・アップ時の注意事項", "_changing_class.html#HowToModifyClass45", null ]
      ] ],
      [ "5.バージョン番号を変えてクラスを修正する方法", "_changing_class.html#HowToModifyClass5", [
        [ "5-1.メンバ変数名の変更", "_changing_class.html#HowToModifyClass51", null ],
        [ "5-2.クラス名の変更", "_changing_class.html#HowToModifyClass52", null ],
        [ "5-3.配列の次元数の変更", "_changing_class.html#HowToModifyClass53", null ],
        [ "5-4.バージョン・アップ／ダウン処理の記述方法", "_changing_class.html#HowToModifyClass54", [
          [ "5-4-1.down/upVersion関数雛形のコピー&ペースト", "_changing_class.html#HowToModifyClass541", null ],
          [ "5-4-2.down/upVersion処理を記述する", "_changing_class.html#HowToModifyClass542", null ]
        ] ]
      ] ],
      [ "6.網羅的な使用例（自動テスト）の説明", "_changing_class.html#HowToModifyClass6", [
        [ "6-1.クラス変更のテスト", "_changing_class.html#HowToModifyClass61", null ],
        [ "6-2.クラス変更の複合テスト", "_changing_class.html#HowToModifyClass62", null ]
      ] ]
    ] ],
    [ "エラー報告", "_error_report.html", [
      [ "1.Theolizerエラー・クラス", "_error_report.html#ErrorReport1", [
        [ "1-1.ErrorInfoクラス", "_error_report.html#ErrorInfo", null ],
        [ "1-2.ErrorType枚挙型", "_error_report.html#ErrorType", null ],
        [ "1-3.ErrorKind枚挙型", "_error_report.html#ErrorKind", null ],
        [ "1-4.ErrorInfoのgetString()のフォーマット", "_error_report.html#getString", null ]
      ] ],
      [ "2.Theolizerで検出したエラーの記録", "_error_report.html#ErrorReport2", [
        [ "2-1.シリアライザのインスタンス", "_error_report.html#SerializerInstance", null ],
        [ "2-2.ErrorReporterシングルトン", "_error_report.html#ErrorReporter", null ]
      ] ],
      [ "3.Theolizerで検出したエラーの通知方法", "_error_report.html#ErrorReport3", null ],
      [ "4.エラー状態の回復方法", "_error_report.html#ErrorReport4", null ],
      [ "5.エラー・ログ", "_error_report.html#ErrorReport5", [
        [ "5-1.THEOLIZER_ERRORLOG_FILE()マクロの指定方法", "_error_report.html#THEOLIZER_ERRORLOG_FILE", null ],
        [ "5-2.ログ・ファイルのフォーマット", "_error_report.html#LogFileFormat", null ],
        [ "5-3.同期待ち時間について", "_error_report.html#WaitSync", null ],
        [ "5-4.スレッドIDについて", "_error_report.html#ThreadId", null ]
      ] ],
      [ "6.サンプル・ソース", "_error_report.html#ErrorReport6", null ],
      [ "7.エラー検出の網羅的テスト", "_error_report.html#ErrorReport7", [
        [ "7-1.エラー報告全体", "_error_report.html#ErrorReport71", null ],
        [ "7-2.downVersion/upVersionのコンパイル・エラー", "_error_report.html#ErrorReport72", null ],
        [ "7-3.グローバル・バージョン番号テーブルのエラー検出", "_error_report.html#ErrorReport73", null ],
        [ "7-4.エラー・リカバリ", "_error_report.html#ErrorReport74", null ]
      ] ]
    ] ]
];