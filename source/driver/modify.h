//############################################################################
//      ソース修正
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

#if !defined(THEOLIZER_MODIFY_H)
#define THEOLIZER_MODIFY_H

//############################################################################
//      ソース修正用クラス
//############################################################################

class ModifySource
{
// ***************************************************************************
//      ユーティリティ
// ***************************************************************************

private:

//----------------------------------------------------------------------------
//      文字列の改行を正規化する
//----------------------------------------------------------------------------

    std::string normalizeLF(std::string&& iString)
    {
        if (kLineEnding.equals("\n"))
    return iString;

        std::string ret;
        StringRef aString(iString);
        bool aEndIsLF=aString.endswith(kLineEnding);
        for (std::pair<StringRef, StringRef> aParsing=aString.split(kLineEnding);
             true;
             aParsing=aParsing.second.split(kLineEnding))
        {
            ret.append(aParsing.first);
            if (aParsing.second.empty())
        break;
            ret.append("\n");
        }
        if (aEndIsLF) ret.append("\n");
        return ret;
    }

//----------------------------------------------------------------------------
//      文字列の改行をOSに合わせる
//----------------------------------------------------------------------------

    std::string reverseLF(std::string&& iString)
    {
        if (kLineEnding.equals("\n"))
    return iString;

        std::string ret;
        StringRef aString(iString);
        bool aEndIsLF=aString.endswith("\n");
        for (std::pair<StringRef, StringRef> aParsing=aString.split('\n');
             true;
             aParsing=aParsing.second.split('\n'))
        {
            ret.append(aParsing.first);
            if (aParsing.second.empty())
        break;
            ret.append(kLineEnding);
        }
        if (aEndIsLF) ret.append(kLineEnding);
        return ret;
    }

//----------------------------------------------------------------------------
//      指定位置の現ソース獲得
//----------------------------------------------------------------------------

    std::string getNowSource(SourceLocation iBegin, SourceLocation iEnd)
    {
         return normalizeLF(
            std::move(mRewriter.getRewrittenText(clang::SourceRange(iBegin, iEnd))));
    }

//----------------------------------------------------------------------------
//      指定Declの現ソース獲得
//----------------------------------------------------------------------------

    std::string getDeclSource(Decl const* iDecl)
    {
        std::string ret;
        if (iDecl)
        {
            ret = normalizeLF(std::move(mRewriter.getRewrittenText(iDecl->getSourceRange())));
        }
        return ret;
    }

//----------------------------------------------------------------------------
//      FileID記録
//----------------------------------------------------------------------------

    void addFileID(const SourceLocation iLocation)
    {
        FileID aFileId = gASTContext->getFullLoc(iLocation).getFileID();
        auto found = std::lower_bound(mModifiedFiles.begin(),
                                      mModifiedFiles.end(),
                                      aFileId);
        if ((found == mModifiedFiles.end()) || (*found != aFileId))
        {
            mModifiedFiles.insert(found, aFileId);
        }
    }

//----------------------------------------------------------------------------
//      文字列置き換え
//----------------------------------------------------------------------------

    void replaceString(SourceLocation iBegin, SourceLocation iEnd, std::string&& iString)
    {
        if (mRewriter.ReplaceText(clang::SourceRange(iBegin, iEnd), reverseLF(std::move(iString))))
        {
            gCustomDiag.FatalReport(iBegin,
                "Fatal error. Can not replace source.");
    return;
        }
        addFileID(iBegin);
    }

//----------------------------------------------------------------------------
//      文字列挿入
//----------------------------------------------------------------------------

    void instertString(SourceLocation iLocation, std::string&& iString)
    {
        if (mRewriter.InsertText(iLocation, reverseLF(std::move(iString))))
        {
            gCustomDiag.FatalReport(iLocation,
                "Fatal error. Can not replace source.");
    return;
        }
        addFileID(iLocation);
    }

// ***************************************************************************
//      最新版生成用
// ***************************************************************************

//----------------------------------------------------------------------------
//      基底クラス生成
//----------------------------------------------------------------------------

    void createBaseClass
    (
        clang::AccessSpecifier  iAccSpec,
        CXXRecordDecl const*    iBase,
        std::string const&      iBaseName,
        unsigned                iId,
        bool                    iIsManual
    )
    {
        // Keep-step or Non-keep-step判定
        bool aIsKeepStep=false;
        auto found = mAstInterface.mSerializeListClass.find(iBase);
        // 派生先クラスが手動型の時は強制的にNon-keep-step
        if (found && !iIsManual)
        {
            // 侵入型と完全自動型のみKeep-step
            if (found->second.mIsFullAuto || !found->second.mNonIntrusive)
            {
                aIsKeepStep=true;
            }
        }

        if (aIsKeepStep)
        {
            if (found->second.mIsFullAuto)
            {
                mLastVersion << "    THEOLIZER_INTERNAL_BASE_KN(";
            }
            else
            {
                mLastVersion << "    THEOLIZER_INTERNAL_BASE_KI(";
            }
        }
        else
        {
            mLastVersion << "    THEOLIZER_INTERNAL_BASE_N(";
        }

        // public/protected/private
        switch(iAccSpec)
        {
        case clang::AS_public :
            mLastVersion << "public";
            break;

        case clang::AS_protected:
            mLastVersion << "protected";
            break;

        case clang::AS_private:
            mLastVersion << "private";
            break;

        default:
            // 省略時、structならpublic、classならprivate
            if (iBase->getTagKind() == clang::TTK_Struct)
            {
                mLastVersion << "public";
            }
            else
            {
                mLastVersion << "private";
            }
        }
        // 追跡モード(暫定的に非追跡)
        mLastVersion << ",etmDefault";

        // Id番号
        mLastVersion << "," << iId;

        // 型名
        mLastVersion << ",(" << iBaseName << ")";

        // バージョン番号(ある時だけ)
        if (aIsKeepStep)
        {
            mLastVersion << "," << found->second.mLastVersionNo;
        }
        mLastVersion << ",u8\"" << iBaseName << "\")";
    }

//----------------------------------------------------------------------------
//      生配列要素生成
//          サポートしない形式の場合false返却
//
//          下記は両方ともTypeClass==IncompleteArrayだった
//              int imcomplete[];
//              int star[*];
//          テンプレート・パラメータでサイズを指定すると
//              TypeClass==DependentSizedArrayだった
//----------------------------------------------------------------------------

    bool createArray(StringRef iName, QualType iType, clang::ArrayType const* iArray)
    {
        ASTANALYZE_OUTPUT("  Array : ",
                          iType.getDesugaredType(*gASTContext).getAsString(mPrintingPolicy),
                          " SizeModifier=", iArray->getSizeModifier(),
                          " TypeClass=" ,iArray->getTypeClass());

        // 不完全型([])やCの可変長配列はサポートしない
        if ((iArray->getTypeClass() == Type::IncompleteArray)
         || (iArray->getTypeClass() == Type::VariableArray))
    return false;

        // 基本型を見つける
        QualType aUnderlyingType;
        aUnderlyingType=iArray->getElementType();
        while(clang::ArrayType const* aType
            =dyn_cast<clang::ArrayType const>(aUnderlyingType.getTypePtr()))
        {
            aUnderlyingType=aType->getElementType();
        }

        // 不完全型([])やCの可変長配列はサポートしない
        if ((aUnderlyingType->getTypeClass() == Type::IncompleteArray)
         || (aUnderlyingType->getTypeClass() == Type::VariableArray))
    return false;

        // 基本型をNon-keep-step, keep-step(侵入型), keep-step(非侵入型:enum)に分類する
        //  下記はNon-keep-stepとする
        //      ポインタ型
        //      組み込み型
        //      テンプレート引数(どんな型に展開されるのか決定できない)
        enum {NonKeepStep, KeepStepIntrusive, KeepStepNonIntrusive}
        aKind = NonKeepStep;

        // クラス／構造体
        unsigned aVersionNo=0;
        if (clang::CXXRecordDecl const* crd = aUnderlyingType->getAsCXXRecordDecl())
        {
            // Keep-step or Non-keep-step判定
            auto found = mAstInterface.mSerializeListClass.find(crd);
            if (found)
            {
                // 侵入型のみKeep-step
                //  （完全自動型でもmNonIntrusiveがnullptrのケース有り）
                if (!found->second.mNonIntrusive && !found->second.mIsFullAuto)
                {
                    aKind=KeepStepIntrusive;
                    aVersionNo=found->second.mLastVersionNo;
                }
            }
        }

        // enum型
        else if (clang::EnumType const* et = aUnderlyingType->getAs<clang::EnumType>())
        {
            clang::EnumDecl const* ed = et->getDecl();
            auto found = mAstInterface.mSerializeListEnum.find(ed);
            if (found)
            {
                aKind=KeepStepNonIntrusive;
                aVersionNo=found->second.mLastVersionNo;
            }
        }

        // THEOLIZER_GENERATED_ELEMENT_A*生成
        switch (aKind)
        {
        case NonKeepStep:
            mLastVersion << "    THEOLIZER_INTERNAL_ELEMENT_AN((";
            break;

        case KeepStepIntrusive:
            mLastVersion << "    THEOLIZER_INTERNAL_ELEMENT_AKI((";
            break;

        case KeepStepNonIntrusive:
            mLastVersion << "    THEOLIZER_INTERNAL_ELEMENT_AKN((";
            break;
        }
        mLastVersion << iName.str() << ")," << iName.str() << "," << mObjectTracking << ",\\\n"
                     << "        "   << mDestination << ",\\\n"
                     << "        ("  << aUnderlyingType.getAsString(mPrintingPolicy) << ")";

        // バージョン番号
        if (aKind != NonKeepStep)
        {
            mLastVersion << "," << aVersionNo;
        }

        // 各次元の要素数
        for (clang::ArrayType const* aArrayType=iArray;
             aArrayType;
             aArrayType=dyn_cast<clang::ArrayType const>(
                aArrayType->getElementType().getTypePtr()))
        {
            // 固定長
            if (aArrayType->getTypeClass() == Type::ConstantArray)
            {
                clang::ConstantArrayType const* aConstantArrayType=
                    cast<clang::ConstantArrayType>(aArrayType);
                mLastVersion << "," << aConstantArrayType->getSize().getZExtValue();
            }
            // テンプレート・パラメータ依存長
            else
            {
                clang::DependentSizedArrayType  const* aDependentSizedArrayType =
                    cast<clang::DependentSizedArrayType>(aArrayType);
                std::string expr;
                llvm::raw_string_ostream aRawSS(expr);
                // llvm\tools\clang\lib\AST\TypePrinter.cppの
                // printDependentSizedArrayAfter()によると
                // printPretty()のPrinterHelper*パラメータはnullptrで良い模様
                aDependentSizedArrayType->getSizeExpr()->
                    printPretty(aRawSS, nullptr, mPrintingPolicy);
                mLastVersion << "," << aRawSS.str();
            }
        }
        mLastVersion << ")";
        return true;
    }

//----------------------------------------------------------------------------
//      組み込み型要素生成
//----------------------------------------------------------------------------

    void createBuiltin(StringRef iName, QualType iType)
    {
        mLastVersion << "    THEOLIZER_INTERNAL_ELEMENT_N(("
                     << iName.str() << ")," << iName.str() << "," << mObjectTracking << ",\\\n"
                     << "        "   << mDestination << ",\\\n"
                     << "        ("  << iType.getAsString(mPrintingPolicy) << "))";
    }

//----------------------------------------------------------------------------
//      テンプレート引数要素生成
//----------------------------------------------------------------------------

    void createTemplateParm(StringRef iName, QualType iType,
                            clang::TemplateTypeParmType const* iTemplateParm)
    {
        mLastVersion << "    THEOLIZER_INTERNAL_ELEMENT_N(("
                     << iName.str() << ")," << iName.str() << "," << mObjectTracking << ",\\\n"
                     << "        "   << mDestination << ",\\\n"
                     << "        ("  << iType.getAsString(mPrintingPolicy) << "))";
    }

//----------------------------------------------------------------------------
//      クラス／構造体要素生成
//----------------------------------------------------------------------------

    void createClass(StringRef iName, QualType iType, clang::CXXRecordDecl const* iClass)
    {
        // Keep-step or Non-keep-step判定
        bool aIsKeepStep=false;
        auto found = mAstInterface.mSerializeListClass.find(iClass);
        if (found)
        {
            // 侵入型と完全自動型のみKeep-step
            if (found->second.mIsFullAuto || !found->second.mNonIntrusive)
            {
                aIsKeepStep=true;
            }
        }

        if (aIsKeepStep)
        {
            if (found->second.mIsFullAuto)
            {
                mLastVersion << "    THEOLIZER_INTERNAL_ELEMENT_KN((";
            }
            else
            {
                mLastVersion << "    THEOLIZER_INTERNAL_ELEMENT_KI((";
            }
        }
        else
        {
            mLastVersion << "    THEOLIZER_INTERNAL_ELEMENT_N((";
        }

        mLastVersion << iName.str() << ")," << iName.str() << "," << mObjectTracking << ",\\\n"
                     << "        "   << mDestination << ",\\\n"
                     << "        (" << iType.getAsString(mPrintingPolicy) << ")";

        // バージョン番号(ある時だけ)
        if (aIsKeepStep)
        {
            mLastVersion << "," << found->second.mLastVersionNo << ")";
        }
        else
        {
            mLastVersion << ")";
        }
    }

//----------------------------------------------------------------------------
//      enum型要素生成
//          エラー時false
//----------------------------------------------------------------------------

    bool createEnum(StringRef iName, QualType iType, clang::EnumType const* iEnum)
    {
        clang::EnumDecl const* ed = iEnum->getDecl();
        auto found = mAstInterface.mSerializeListEnum.find(ed);
        if (!found)
    return false;

        mLastVersion << "    THEOLIZER_INTERNAL_ELEMENT_KN(("
                     << iName.str() << ")," << iName.str() << "," << mObjectTracking << ",\\\n"
                     << "        "   << mDestination << ",\\\n"
                     << "        ("  << iType.getAsString(mPrintingPolicy) << ")"
                     << "," << found->second.mLastVersionNo << ")";

        return true;
    }

// ***************************************************************************
//      ソース修正状態(戻り値)
// ***************************************************************************

    enum SourceStatus
    {
        eAborted,
        eNew,
        eVersionUp,
        eUpdate
    };

// ***************************************************************************
//      最新版の新規部分生成
//          ここは最新版と１つ前の版のTheolizerVersion<>がない時に呼ばれる
//          なお、完全自動型用マクロについては、既に存在していても
//          TheolizerVersion<>を無視するのでここに来る。
// ***************************************************************************

    void genarateEnumLastVersion
    (
        SerializeInfo<EnumDecl> const& iSerializeInfo
    )
    {
ASTANALYZE_OUTPUT("genarateClassLastVersion");

        mNamespaceNestCount=0;          // 不要だが念のため

//----------------------------------------------------------------------------
//      自動生成開始マーク生成
//----------------------------------------------------------------------------

        mLastVersion << "#ifdef  THEOLIZER_WRITE_CODE // ###### "
                     << iSerializeInfo.mClassName << " ######\n";
        mLastVersion << "\n";

//----------------------------------------------------------------------------
//      ターゲットのソース生成
//----------------------------------------------------------------------------

        mLastVersion << "#define THEOLIZER_GENERATED_LAST_VERSION_NO "
                        "THEOLIZER_INTERNAL_DEFINE(";
    }

// ***************************************************************************
//      enum用ソース文字列生成
//          パターンとして、下記がある
//              完全自動(Version.0のみ)
//                  ソース未生成(THEOLIZER_ENUM無し)
//                  ソース生成済(THEOLIZER_ENUM有り)
//              指定有り(Version.0以外もあり)
//                  ソース未生成(THEOLIZER_ENUM有り)
//                  ソース生成済(THEOLIZER_ENUM有り)
// ***************************************************************************

    SourceStatus modifyEnum(SerializeInfo<EnumDecl> const& iSerializeInfo)
    {
        std::string const aClassName(iSerializeInfo.mClassName);
        ASTANALYZE_OUTPUT("modifyEnum(", aClassName, ")");

        // 処理中のターゲット設定
        mTarget=iSerializeInfo.mNonIntrusive;
        if (!mTarget)
        {
            mTarget = iSerializeInfo.mTheolizerTarget;
        }

        // バージョンが2以上なのに前バージョン無しは禁止
        if ((iSerializeInfo.mLastVersionNo >= 2)
         && (!iSerializeInfo.mTheolizerVersionPrev))
        {
            gCustomDiag.ErrorReport(mTarget->getLocation(), "No previous version(%0, %1).")
                << iSerializeInfo.mClassName << iSerializeInfo.mLastVersionNo;
return eAborted;
        }

//----------------------------------------------------------------------------
//      最新版生成
//----------------------------------------------------------------------------

        SourceStatus aSourceStatus=eUpdate; // 戻り値

        mLastVersion.str("");
////    bool aIsTheolizerHpp;

        // 最新版のTheolizerVersion無し
        if (!iSerializeInfo.mTheolizerVersionLast)
        {
            aSourceStatus=eVersionUp;
            mRetryAST=true;

            // １つ前の版も無し(新規生成)
            if (!iSerializeInfo.mTheolizerVersionPrev)
            {
                aSourceStatus=eNew;
                genarateEnumLastVersion
                (
                    iSerializeInfo
                );
            }
            else
            {
                // クラス定義マクロの定義位置
                mLastVersionNoLoc=mAstInterface.mLastVersionNoLocations.upper_bound(
                    iSerializeInfo.mLastVersionNoLoc);
                ERROR(mLastVersionNoLoc == mAstInterface.mLastVersionNoLocations.end(),
                    mTarget, eAborted);
////            aIsTheolizerHpp=mLastVersionNoLoc->second.mIsTheolizerHpp;
            }
        }
        else
        {
            // 最新版のTheolizerVersion定義マクロの定義位置
            mLastVersionNoLoc=mAstInterface.mVersionNoLocations.upper_bound(
                iSerializeInfo.mVersionNoLastLoc);
            ERROR(mLastVersionNoLoc == mAstInterface.mVersionNoLocations.end(),
                iSerializeInfo.mTheolizerVersionLast, eAborted);
////        aIsTheolizerHpp=mLastVersionNoLoc->second.mIsTheolizerHpp;
        }

        mLastVersion << "kLastVersionNo,"
                     << iSerializeInfo.mLastVersionNo << ")\n";

        // 自動生成ソース内、かつ、save/loadしてない時は、コンパイルしないように指示する
#if 0   // 最新版ではsave/loadしてなくても旧バージョンでしている時、ビルドできなくなるので保留
        if (aIsTheolizerHpp && (iSerializeInfo.mSerializeStat == esSerializeOnly))
        {
            mLastVersion << "#define THEOLIZER_GENERATED_NO_COMPILE\n";
        }
#endif

        if (iSerializeInfo.mIsFullAuto)
        {
            mLastVersion << "#define THEOLIZER_GENERATED_FULL_AUTO "
                         << aClassName << "\n\n";
        }
        else
        {
            mLastVersion << "#define THEOLIZER_GENERATED_ENUM_TYPE "
                         << aClassName << "\n\n";
        }

        mLastVersion << "//      ---<<< Version."
                     << iSerializeInfo.mLastVersionNo << " >>>---\n\n";

        mLastVersion << "#define THEOLIZER_GENERATED_VERSION_NO "
                        "THEOLIZER_INTERNAL_DEFINE(kVersionNo,"
                     << iSerializeInfo.mLastVersionNo << ")\n";

        mLastVersion << "#define THEOLIZER_GENERATED_ENUM_NAME u8\""
                     << aClassName << "\"\n";

        mLastVersion << "#define THEOLIZER_GENERATED_SAVE_TYPE "
                     << kEnumSaveType[iSerializeInfo.mAdditionalInfo] << "\n";

        mLastVersion << "#define THEOLIZER_GENERATED_BASE_TYPE "
                     << iSerializeInfo.mTheolizerTarget->
                            getIntegerType().getAsString(mPrintingPolicy) << "\n";

        mLastVersion << "#define THEOLIZER_GENERATED_ENUM_LIST()";
        int64_t aDefaultValue=0;
        bool aIsFirst=true;
        for (auto symbol : iSerializeInfo.mTheolizerTarget->enumerators())
        {
            // アノテーション獲得
            AnnotationInfo aEnumAnno=getAnnotationInfo(symbol,AnnotateType::Enum);

            // シンボル・リスト生成
            mLastVersion << "\\\n"
                         << "    THEOLIZER_GENERATED_ELEMENT((u8\""
                         << symbol->getName().str() << "\"";

            // first は現バージョンの代替文字列群
            // serondは現バージョンの代替値群＋前バージョン時の値
            std::pair<StringRef, StringRef> param;
            param=StringRef(aEnumAnno.mParameter).split(':');

            // 現バージョンの代替文字列出力
            for (std::pair<StringRef, StringRef> temp=param.first.split(',');
                 !temp.first.empty();
                 temp=temp.second.split(','))
            {
                mLastVersion << ",u8\"" << temp.first.trim().str() << "\"";
            }

            // 現バージョンの値出力
            int64_t aNowValue=symbol->getInitVal().getExtValue();
            mLastVersion << "),(" << aNowValue;

            // デフォルト値獲得
            if (aIsFirst)
            {
                aIsFirst=false;
                aDefaultValue=aNowValue;
            }

            // firstは現バージョンの代替値群
            // secondは前バージョン時の値
            param = param.second.split(':');

            // 現バージョンの代替値群出力
            for (std::pair<StringRef, StringRef> temp=param.first.split(',');
                 !temp.first.empty();
                 temp=temp.second.split(','))
            {
                std::string::size_type len=0;
                long long value=0;
                try
                {
                    value=std::stoll(temp.first.str(), &len, 0);
                }
                catch(...)
                {
                }
                if (len != temp.first.size())
                {
                    gCustomDiag.ErrorReport(symbol->getLocation(),
                        "Previous version's value(%0) is invalid.")
                        << temp.first;
            continue;
                }
                mLastVersion << "," << value;
            }
            mLastVersion << "),(";

            // 前バージョンの値出力(無指定なら、変更無し)
            if (param.second.empty())
            {
                mLastVersion << aNowValue;
            }
            else
            {
                mLastVersion << param.second.str();
            }
            mLastVersion << "))";
        }
        mLastVersion << "\n";
        mLastVersion << "#define THEOLIZER_GENERATED_DEFAULT_VALUE " << aDefaultValue << "\n";
        mLastVersion << "#include <theolizer/internal/version_enum.inc>\n";
        mLastVersion << "#undef  THEOLIZER_GENERATED_VERSION_NO";
        // 新規生成時は、#endifを定義する
        if (aSourceStatus == eNew)
        {
            mLastVersion << "\n\n";

            // 終了
            mLastVersion << "#endif//THEOLIZER_WRITE_CODE // ###### "
                         << aClassName << " ######\n\n";
        }

        ASTANALYZE_OUTPUT("--------mLastVersion\n", mLastVersion.str(), "\n----------");

//----------------------------------------------------------------------------
//      １つ前の版生成
//----------------------------------------------------------------------------

        mPrevVersion.str("");

        CXXRecordDecl const* aTheolizerVersionPrev = iSerializeInfo.mTheolizerVersionPrev;
        if (aTheolizerVersionPrev)
        {
            ASTANALYZE_OUTPUT("Prev Version : ",
                              aTheolizerVersionPrev->getQualifiedNameAsString());

            // struct Theolizerを探す(ついでにBaseTypeを取り出す)
            CXXRecordDecl const* aTheolizer=nullptr;
            for (auto&& decl : aTheolizerVersionPrev->decls())
            {
                CXXRecordDecl const* crd=dyn_cast<clang::CXXRecordDecl>(decl);
                if (!crd || !crd->getIdentifier())
            continue;

                if (crd->getName().equals("Theolizer"))
                {
                    aTheolizer=crd;
            break;
                }
            }
            ERROR(!aTheolizer, aTheolizerVersionPrev, eAborted);

            // kVersionNo、kEnumSaveType、クラス名取り出し
            auto&& aDecls = theolizer::getRBForIndexer(aTheolizer->decls());
            for (auto&& aDecl : aDecls)
            {
                VarDecl* vd = dyn_cast<VarDecl>(aDecl.front());
                if (!vd)
            continue;

                // kVersionNo
                if (vd->getName().equals("kVersionNo"))
                {
                    unsigned    aVersionNo;
                    APValue* apvalue = vd->getEvaluatedValue();
                    if (!apvalue)  apvalue=vd->evaluateValue();
                    ERROR(!getVersionNumber(apvalue, aVersionNo), vd, eAborted);

                    mPrevVersion << "kVersionNo," << aVersionNo << ")\n";
                }

                // kEnumSaveType
                else if (vd->getName().equals("kEnumSaveType"))
                {
                    unsigned    aEnumSaveType;
                    APValue* apvalue = vd->getEvaluatedValue();
                    if (!apvalue)  apvalue=vd->evaluateValue();
                    ERROR(!getVersionNumber(apvalue, aEnumSaveType), vd, eAborted);
                    ERROR(aEnumSaveType > 1, vd, eAborted);

                    mPrevVersion << "#define THEOLIZER_GENERATED_ENUM_NAME ";
                    AnnotationInfo aAnnotation=getAnnotationInfo(vd, AnnotateType::TS);
                    mPrevVersion << aAnnotation.mParameter << "\n";
                    // 手動型は順序固定なので出力しない
                    mPrevVersion << "#define THEOLIZER_GENERATED_SAVE_TYPE "
                                 << kEnumSaveType[aEnumSaveType] << "\n";
                }

                // kDefaultValue
                else if (vd->getName().equals("kDefaultValue"))
                {
                    mPrevVersion << "#define THEOLIZER_GENERATED_BASE_TYPE "
                                 << vd->getType().getDesugaredType(*gASTContext).
                                                  getUnqualifiedType().getAsString()
                                 << "\n";

                    APValue* apvalue = vd->getEvaluatedValue();
                    if (!apvalue)  apvalue=vd->evaluateValue();
                    ERROR(!apvalue, vd, eAborted);
                    int64_t aDefaultValue2=apvalue->getInt().getExtValue();

                    mPrevVersion << "#define THEOLIZER_GENERATED_ENUM_LIST()";
                    AnnotationInfo aAnnotation=getAnnotationInfo(vd, AnnotateType::TS);
                    std::pair<StringRef, StringRef> temp=StringRef(aAnnotation.mParameter).
                            split(StringRef("THEOLIZER_GENERATED_ELEMENT"));
                    for (temp=temp.second.split(StringRef(" THEOLIZER_GENERATED_ELEMENT"));
                         !temp.first.empty();
                         temp=temp.second.split(StringRef(" THEOLIZER_GENERATED_ELEMENT")))
                    {
                        mPrevVersion << "\\\n    THEOLIZER_GENERATED_ELEMENT" << temp.first.str();
                    }
                    mPrevVersion << "\n";

                    mPrevVersion << "#define THEOLIZER_GENERATED_DEFAULT_VALUE "
                                 << aDefaultValue2 << "\n";
            break;
                }
            }
            mPrevVersion << "#include <theolizer/internal/version_enum.inc>\n"
                         << "#undef  THEOLIZER_GENERATED_VERSION_NO";

            ASTANALYZE_OUTPUT("-------- mPrevVersion\n", mPrevVersion.str());
        }

        return aSourceStatus;
    }

// ***************************************************************************
//      最新版の新規部分生成
//          ここは最新版と１つ前の版のTheolizerVersion<>がない時に呼ばれる
//          なお、完全自動型用マクロについては、既に存在していても
//          TheolizerVersion<>を無視するのでここに来る。
// ***************************************************************************

    void genarateClassLastVersion
    (
        SerializeInfo<CXXRecordDecl> const& iSerializeInfo,
        std::string const& iClassName,
        std::string const& iParameterList
    )
    {
        ASTANALYZE_OUTPUT("genarateClassLastVersion");

        mNamespaceNestCount=0;

//----------------------------------------------------------------------------
//      自動生成開始マークとnamespace生成
//----------------------------------------------------------------------------

        mLastVersion << "#ifdef  THEOLIZER_WRITE_CODE // ###### "
                     << iClassName << " ######\n";

        // namespace対応(侵入型のみ)
        if (!iSerializeInfo.mIsFullAuto
         && !iSerializeInfo.mNonIntrusive)
        {
            for (auto& aNameSpace : NameSpaces(mTheolizerClass, false))
            {
                ++mNamespaceNestCount;
                if (aNameSpace.mIsInline) mLastVersion << "inline ";
                mLastVersion << "namespace ";
                mLastVersion << aNameSpace.mName.str() << "{";
            }
            if (mNamespaceNestCount) mLastVersion << "\n";
        }
        mLastVersion << "\n";

//----------------------------------------------------------------------------
//      半自動型用up/downVersion雛形生成
//----------------------------------------------------------------------------

        if (!iSerializeInfo.mIsFullAuto
         && !iSerializeInfo.mNonIntrusive)
        {
            mLastVersion << "#if false // Sample of up/downVersion function.\n"
             << iParameterList <<
                "template<class tTheolizerVersion, class tNextVersion>\n"
                "struct " << iClassName <<
                "::TheolizerUserDefine<tTheolizerVersion, tNextVersion, 1>\n"
                "{\n"
                "    // Members version down.\n"
                "    static void downVersion(tNextVersion const& iNextVersion,"
                    " tTheolizerVersion& oNowVersion)\n"
                "    {\n"
                "    }\n\n"
                "    // Members version up.\n"
                "    static void upVersion(tTheolizerVersion const& iNowVersion,"
                    " tNextVersion& oNextVersion)\n"
                "    {\n"
                "    }\n"
                "};\n"
                "#endif // Sample of up/downVersion function.\n\n";
        }

//----------------------------------------------------------------------------
//      手動型用save/load雛形
//----------------------------------------------------------------------------

        if (iSerializeInfo.mNonIntrusive)
        {
            mLastVersion << "#if false // Sample of save/load function.\n"
                << iParameterList <<
                "template<class tBaseSerializer, class tTheolizerVersion>\n"
                "struct TheolizerNonIntrusive<" << iClassName << ">"
                "::TheolizerUserDefine<tBaseSerializer, tTheolizerVersion, 1>\n"
                "{\n"
                "    // Save members.\n"
                "    static void saveClassManual\n"
                "    (\n"
                "        tBaseSerializer& iSerializer,\n"
                "        typename tTheolizerVersion::TheolizerTarget const*const& iInstance\n"
                "    )\n"
                "    {\n"
                "        \n"
                "        \n"
                "    }\n\n"
                "    // Load members.\n"
                "    static void loadClassManual\n"
                "    (\n"
                "        tBaseSerializer& iSerializer,\n"
                "        typename tTheolizerVersion::TheolizerTarget*& oInstance\n"
                "    )\n"
                "    {\n"
                "        if (!oInstance) oInstance="
                    "new typename tTheolizerVersion::TheolizerTarget();\n"
                "        \n"
                "        \n"
                "    }\n"
                "};\n"
                "#endif // Sample of save/load function.\n\n";
        }

//----------------------------------------------------------------------------
//      ターゲットのソース生成
//----------------------------------------------------------------------------

        mLastVersion << "#define THEOLIZER_GENERATED_LAST_VERSION_NO "
                        "THEOLIZER_INTERNAL_DEFINE(";
    }

// ***************************************************************************
//      保存フィールド(FS)のパラメータ取り出し
//          下記を取り出す
//              mPrevName       前バージョンの要素名
//              mDestination    保存先指定
//              mObjectTracking オブジェクト追跡指定
// ***************************************************************************

    void getFieldParameter(std::string const& iParameter, clang::FieldDecl const* iFieldDecl)
    {
        std::pair<StringRef, StringRef> temp=StringRef(iParameter).split('<');
        if (!temp.first.empty())
        {
            mPrevName=temp.first;
        }
        temp=temp.second.rsplit('>');
        if (!temp.first.empty())
        {
            mDestination="(";
            mDestination.append(temp.first);
            mDestination.push_back(')');
        }
        if (temp.second.equals("Owner"))
        {
            mObjectTracking="etmOwner";
        }
        else if (temp.second.equals("Pointee"))
        {
            mObjectTracking="etmPointee";
        }
        else if (temp.second.empty())
        {
        }
        else
        {
            gCustomDiag.ErrorReport(iFieldDecl->getLocation(),
                "Object tracking mode(%0) is invalid.")
                << temp.second;
        }
    }

// ***************************************************************************
//      クラス用ソース文字列生成
// ***************************************************************************

    SourceStatus modifyClass(SerializeInfo<CXXRecordDecl> const& iSerializeInfo)
    {
        std::string aClassName(iSerializeInfo.mClassName);
        ASTANALYZE_OUTPUT("modifyClass(", aClassName, ")##########\n",
                          " mTheolizerTarget=", iSerializeInfo.mTheolizerTarget,
                          " mUniqueClass=", iSerializeInfo.mUniqueClass,
                          " NumTemplateParameterLists=",
                          iSerializeInfo.mTheolizerTarget->getNumTemplateParameterLists());
        ASTANALYZE_OUTPUT("    mAnnotationInfo=", iSerializeInfo.mAnnotationInfo.c_str(),
                          ":", iSerializeInfo.mAnnotationInfo.mParameter);
        ASTANALYZE_OUTPUT("    mAnnotationInfoTS=", iSerializeInfo.mAnnotationInfoTS.mParameter);

        // 処理中のターゲット設定
        mTheolizerClass=iSerializeInfo.mNonIntrusive;
        if (!mTheolizerClass)
        {
            mTheolizerClass = iSerializeInfo.mTheolizerTarget;
        }
        mTarget=mTheolizerClass;

        // バージョンが2以上なのに前バージョン無しは禁止
        if ((iSerializeInfo.mLastVersionNo >= 2)
         && (!iSerializeInfo.mTheolizerVersionPrev))
        {
            gCustomDiag.ErrorReport(mTarget->getLocation(), "No previous version(%0, %1).")
                << iSerializeInfo.mClassName << iSerializeInfo.mLastVersionNo;
return eAborted;
        }

//----------------------------------------------------------------------------
//      アノテーション確認(手動なら処理しない)
//----------------------------------------------------------------------------

        switch(iSerializeInfo.mAnnotationInfo.mAnnotate)
        {
        case AnnotationInfo::CM:
            ASTANALYZE_OUTPUT("    Theolizer does not modify source "
                              "because this is manual serialization class.");
    return eAborted;

        case AnnotationInfo::None:
        case AnnotationInfo::CN:
        case AnnotationInfo::CS:
            break;

        default:
            DRIVER_ABORT("");
        }

//----------------------------------------------------------------------------
//      クラス・テンプレート対応
//----------------------------------------------------------------------------

        std::string aParameterList;      // template<...>
        std::string aAddClassName;       // <...>

        bool aIsTemplate=false;
        bool aMakeTempalteName=false;
        clang::TemplateParameterList* aTemplateParameterList=nullptr;

        // プライマリー
        //  getInjectedClassNameSpecialization()で得たQualTypeのgetAsString()では
        //  定義側ではなく宣言のパラメータ・リストになる模様。
        //  プライマリーの定義側に実引数(注入)リストはないためだろう。
        //  そこで、仮引数リスト(TemplateParameterList)から生成する。
        if (ClassTemplateDecl* ctd=iSerializeInfo.mTheolizerTarget->getDescribedClassTemplate())
        {
            aIsTemplate=true;
            aMakeTempalteName=true;
            aTemplateParameterList=ctd->getTemplateParameters();
        }

        // 部分特殊化
        //  特殊化はテンプレートではない
        if (ClassTemplateSpecializationDecl const* ctsd=
            dyn_cast<ClassTemplateSpecializationDecl>(iSerializeInfo.mTheolizerTarget))
        {
            clang::TypeSourceInfo* tsi = ctsd->getTypeAsWritten();
            ERROR(!tsi, iSerializeInfo.mTheolizerTarget, eAborted);
            QualType qt = tsi->getType();
            aClassName=qt.getAsString();
            clang::ClassTemplatePartialSpecializationDecl const* ctpsd=
            dyn_cast<clang::ClassTemplatePartialSpecializationDecl>(ctsd);
            if (ctpsd)
            {
                aIsTemplate=true;
                aTemplateParameterList=ctpsd->getTemplateParameters();
            }
            aMakeTempalteName=true;
        }

        // テンプレート・パラメータとクラス名生成
        std::stringstream aArgumentList;    // クラス<...>の...の部分
        if (aIsTemplate)
        {
            // 完全自動型以外は、ユーザ指定マクロから取り出す
            std::pair<StringRef, StringRef> temp;
            if (!iSerializeInfo.mIsFullAuto)
            {
                temp=StringRef(iSerializeInfo.mAnnotationInfoTS.mParameter).split(';');
                aClassName=temp.second;
                aParameterList=temp.first;
                aParameterList.append("\n");

                temp=temp.first.split('<');
                temp=temp.second.rsplit('>');
                temp=temp.first.split(',');
                ASTANALYZE_OUTPUT("    temp=[", temp.first, "],[", temp.second, "]");
            }
            else
            {
                aParameterList="template<";
                aAddClassName="<";
            }

            bool aIsFirst=true;
            for (unsigned i=0; i < aTemplateParameterList->size(); ++i)
            {
                NamedDecl* nd=aTemplateParameterList->getParam(i);
                if (!nd || !nd->getIdentifier())
            continue;

                StringRef aType;
                StringRef aName;
                if (!iSerializeInfo.mIsFullAuto)
                {
                    if (temp.first.empty())
            break;
                    StringRef temp2=temp.first.trim();
                    std::size_t pos=temp2.find_first_of(" \t\n\v\f\r");
                    aType=temp2.substr(0, pos).trim();
                    aName=temp2.substr(pos+1).trim();
                    ASTANALYZE_OUTPUT("    aType=", aType);
                    ASTANALYZE_OUTPUT("    aName=", aName);
                    temp=temp.second.split(',');
                }
                else
                {
                    aName=nd->getName();
                }

                if (aIsFirst)
                {
                    aIsFirst=false;
                }
                else
                {
                    aArgumentList << ",";
                    if (iSerializeInfo.mIsFullAuto)
                    {
                        aParameterList.append(",");
                        aAddClassName.append(",");
                    }
                }

                if (clang::TemplateTypeParmDecl* ttpd=dyn_cast<clang::TemplateTypeParmDecl>(nd))
                {
                    aArgumentList << aName.str();
                    if (iSerializeInfo.mIsFullAuto)
                    {
                        if (ttpd->wasDeclaredWithTypename())
                        {
                            aParameterList.append("typename ");
                        }
                        else
                        {
                            aParameterList.append("class ");
                        }
                        aParameterList.append(nd->getName());
                        aAddClassName.append(nd->getName());
                    }
                }
                else if (clang::NonTypeTemplateParmDecl* nttpd=
                    dyn_cast<clang::NonTypeTemplateParmDecl>(nd))
                {
                    std::string aTypeName=aType;
                    if (iSerializeInfo.mIsFullAuto)
                    {
                        aParameterList.append(nttpd->getType().getAsString());
                        aParameterList.append(" ");
                        aParameterList.append(nd->getName());
                        aAddClassName.append(nd->getName());
                        aTypeName=nttpd->getType().getAsString();
                    }
                    aArgumentList << "\\\n        theolizer::internal::NonType<"
                                  << aTypeName
                                  << "," << aName.str()
                                  << ">";
                }
            }
            if (iSerializeInfo.mIsFullAuto)
            {
                aParameterList.append(">\n");
                aAddClassName.append(">");
            }
            ASTANALYZE_OUTPUT("    aParameterList=", aParameterList);
            ASTANALYZE_OUTPUT("    aAddClassName=", aAddClassName);
        }
        else if (aMakeTempalteName)
        {
            std::pair<StringRef, StringRef> temp=StringRef(aClassName).split('<');
            temp=temp.second.rsplit('>');
            aArgumentList.str(temp.first);
        }

//----------------------------------------------------------------------------
//      最新版生成
//----------------------------------------------------------------------------

        SourceStatus aSourceStatus=eUpdate; // 戻り値

        mLastVersion.str("");
        mNamespaceNestCount=0;
////    bool aIsTheolizerHpp=true;

        ASTANALYZE_OUTPUT("    mTheolizerVersionLast=", iSerializeInfo.mTheolizerVersionLast);
        ASTANALYZE_OUTPUT("    mTheolizerVersionPrev=", iSerializeInfo.mTheolizerVersionPrev);
        // 最新版のTheolizerVersion無し、もしくは、完全自動型から半自動型へ変更
        if (!iSerializeInfo.mTheolizerVersionLast || iSerializeInfo.mIsChanged)
        {
            aSourceStatus=eVersionUp;
            mRetryAST=true;

            // １つ前の版も無し(新規生成)
            if (!iSerializeInfo.mTheolizerVersionPrev)
            {
                aSourceStatus=eNew;
                genarateClassLastVersion
                (
                    iSerializeInfo,
                    aClassName,
                    aParameterList
                );
            }
            else
            {
                // クラス定義マクロの定義位置
                mLastVersionNoLoc=mAstInterface.mLastVersionNoLocations.upper_bound(
                    iSerializeInfo.mLastVersionNoLoc);
                ERROR(mLastVersionNoLoc == mAstInterface.mLastVersionNoLocations.end(),
                    mTarget, eAborted);
////            aIsTheolizerHpp=mLastVersionNoLoc->second.mIsTheolizerHpp;
            }
        }
        else
        {
            // 最新版のTheolizerVersion定義マクロの定義位置
            mLastVersionNoLoc=mAstInterface.mVersionNoLocations.upper_bound(
                iSerializeInfo.mVersionNoLastLoc);
            ERROR(mLastVersionNoLoc == mAstInterface.mVersionNoLocations.end(),
                iSerializeInfo.mTheolizerVersionLast, eAborted);
////        aIsTheolizerHpp=mLastVersionNoLoc->second.mIsTheolizerHpp;
        }

        mLastVersion << "kLastVersionNo,"
                     << iSerializeInfo.mLastVersionNo << ")\n";

        // 自動生成ソース内、かつ、save/loadしてない時は、コンパイルしないように指示する
#if 0   // 最新版ではsave/loadしてなくても旧バージョンでしている時、ビルドできなくなるので一旦保留
ASTANALYZE_OUTPUT("    aIsTheolizerHpp=", aIsTheolizerHpp,
                  " mSerializeStat=", iSerializeInfo.mSerializeStat, " ", esSerializeOnly);
        if (aIsTheolizerHpp && (iSerializeInfo.mSerializeStat == esSerializeOnly))
        {
            mLastVersion << "#define THEOLIZER_GENERATED_NO_COMPILE\n";
        }
#endif

        if (iSerializeInfo.mIsFullAuto)
        {
            mLastVersion << "#define THEOLIZER_GENERATED_FULL_AUTO "
                         << aClassName << aAddClassName << "\n";
        }
        else
        {
            mLastVersion << "#define THEOLIZER_GENERATED_CLASS_TYPE THEOLIZER_INTERNAL_UNPAREN("
                         << aClassName << ")\n";
        }
        if (aIsTemplate)
        {
            mLastVersion << "#define THEOLIZER_GENERATED_PARAMETER_LIST "
                         << aParameterList;
            mLastVersion << "#define THEOLIZER_GENERATED_UNIQUE_NAME ";
            if (!iSerializeInfo.mIsFullAuto)
            {
                mLastVersion << iSerializeInfo.mUniqueClass->getQualifiedNameAsString() << "\n";
            }
            else
            {
                mLastVersion << iSerializeInfo.mTheolizerTarget->getName().str() << "Primary\n";
            }
        }

        mLastVersion << "\n//      ---<<< Version."
                     << iSerializeInfo.mLastVersionNo << " >>>---\n\n";

        mLastVersion << "#define THEOLIZER_GENERATED_VERSION_NO "
                        "THEOLIZER_INTERNAL_DEFINE(kVersionNo,"
                     << iSerializeInfo.mLastVersionNo << ")\n";

        mLastVersion << "#define THEOLIZER_GENERATED_CLASS_NAME()\\\n";
        if (aMakeTempalteName)
        {
            mLastVersion << "    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8\""
                         << iSerializeInfo.mClassName << "\"," << aArgumentList.str() << "))\n";
        }
        else
        {
            mLastVersion << "    THEOLIZER_INTERNAL_CLASS_NAME((u8\""
                         << aClassName << "\"))\n";
        }
        if (!iSerializeInfo.mIsManual)
        {
            mLastVersion << "#define THEOLIZER_GENERATED_ELEMENT_MAP "
                         << kElementsMapping[iSerializeInfo.mAdditionalInfo] << "\n";
        }

//      ---<<< 基底クラスと要素のリスト >>>---
//          基底クラスは@+形名がキー、要素は要素名がキー

        struct ElementInfo
        {
            StringRef   mNextName;      // 次バージョンの要素名
            QualType    mType;          // 要素の型

            ElementInfo(QualType iType) : mType(iType)
            { }
            ElementInfo(StringRef iNextName, QualType iType) : mNextName(iNextName), mType(iType)
            { }
        };
        // キーは前バージョンの要素名(通常は最新版の要素名と同じ)
        std::map<std::string, ElementInfo>   aElementList;

//      ---<<< 基底クラスのリスト(THEOLIZER_GENERATED_BASE_LIST)生成 >>>---

        bool aIsFirst=true;
        unsigned aId=0;     // 基底クラスのバックアップ領域と引き継ぎフラグに付けるID

        // 手動型はTHEOLIZER_REGISTER_CLASS()で指定されている時のみ生成する
        if (!iSerializeInfo.mIsManual || iSerializeInfo.mIsRegisteredClass)
        {
            for (auto&& base : iSerializeInfo.mTheolizerTarget->bases())
            {
                QualType qt = base.getType().getCanonicalType();
                CXXRecordDecl const* aBase = qt->getAsCXXRecordDecl();
                if (!aBase)
            continue;

                std::string aBaseName = qt.getCanonicalType().getAsString(mPrintingPolicy);
                ASTANALYZE_OUTPUT("    Base : ", aBaseName, " hasDefinition()=",
                                  aBase->hasDefinition());

                // 完全自動型のprivateなら、スキップする
                if ((iSerializeInfo.mIsFullAuto) && (base.getAccessSpecifier()==clang::AS_private))
                {
                    ASTANALYZE_OUTPUT("    Skip because FullAuto & private base class.");
            continue;
                }

                if (aIsFirst)
                {
                    aIsFirst=false;
                    mLastVersion << "#define THEOLIZER_GENERATED_BASE_LIST()\\\n";
                }
                else
                {
                    mLastVersion << "\\\n        THEOLIZER_GENERATED_SEP\\\n";
                }

                createBaseClass
                (
                    base.getAccessSpecifier(),
                    aBase,
                    aBaseName,
                    aId++,
                    iSerializeInfo.mIsManual
                );
                // 要素リストへ登録
                aElementList.emplace(std::string("@")+aBase->getName().str(), ElementInfo(qt));
            }
        }
        if (!aIsFirst)
        {
            mLastVersion << "\n";
        }

//      ---<<< 要素のリスト(THEOLIZER_GENERATED_ELEMENT_LIST)生成 >>>---

        // 完全自動型と半自動型の時のみ
        if (!iSerializeInfo.mIsManual)
        {
            aIsFirst=true;
            for (auto&& field : iSerializeInfo.mTheolizerTarget->fields())
            {
                ASTANALYZE_OUTPUT("  Element Name=", field->getName());
                mPrevName=field->getName();
                mDestination="(theolizerD::All)";
                mObjectTracking="etmDefault";

                // 保存判定と保存先取り出し
                // アノテーション獲得
                AnnotationInfo aFieldAnno=getAnnotationInfo(field,AnnotateType::Field);
                ASTANALYZE_OUTPUT("    Field : ", field->getName(),
                                  " ", aFieldAnno.c_str(), ":", aFieldAnno.mParameter);

                // 完全自動型のprivateなら、スキップする
                if ((iSerializeInfo.mIsFullAuto) && (field->getAccess() == clang::AS_private))
                {
                    ASTANALYZE_OUTPUT("    Skip because FullAuto & private member.");
            continue;
                }

                // 非保存フィールドなら、スキップ
                if (aFieldAnno.mAnnotate == AnnotationInfo::FN)
                {
                    ASTANALYZE_OUTPUT("    Skip FN");
            continue;
                }

                // 保存フィールドなら、パラメータ取り出し
                else if (aFieldAnno.mAnnotate == AnnotationInfo::FS)
                {
                    getFieldParameter(aFieldAnno.mParameter, field);
                }

                // 非保存でも、保存でも無い場合、クラス指定に従う
                else
                {
                    // デフォルト非保存クラスの場合、スキップ
                    if (iSerializeInfo.mAnnotationInfo.mAnnotate == AnnotationInfo::CN)
                    {
                        ASTANALYZE_OUTPUT("    Skip CN");
            continue;
                    }
                }
                ASTANALYZE_OUTPUT("    sugared type name=",
                                  field->getType().getAsString(mPrintingPolicy));
                ASTANALYZE_OUTPUT("    mPrevName=", mPrevName);
                ASTANALYZE_OUTPUT("    mDestination=", mDestination);
                ASTANALYZE_OUTPUT("    mObjectTracking=", mObjectTracking);

                // 先頭行 or 改行処理
                if (aIsFirst)
                {
                    aIsFirst=false;
                    mLastVersion << "#define THEOLIZER_GENERATED_ELEMENT_LIST()\\\n";
                }
                else
                {
                    mLastVersion << "\\\n";
                }

                // 型取り出し
                QualType qt=field->getType().getDesugaredType(*gASTContext);

                // TheolizerParameterなら、Type取り出し
                do
                {
                    CXXRecordDecl* crd=qt->getAsCXXRecordDecl();
                    if (!crd)
                break;

                    ClassTemplateSpecializationDecl const* ctsd
                        =dyn_cast<ClassTemplateSpecializationDecl >(crd);
                    if (!ctsd)
                break;
                    if (crd->getName().startswith("TheolizerParameter") == false)
                break;

                    TemplateArgumentList const& tal=ctsd->getTemplateArgs();
                    if (tal.size() < 1)
                break;

                    TemplateArgument const& ta = tal.get(0);
                    qt = ta.getAsType();
                }
                while(0);

                Type const* type = qt.getTypePtr();

                // 要素リストへ登録
                aElementList.emplace(mPrevName, ElementInfo(field->getName(), qt));

                // 生配列
                if (clang::ArrayType  const* at = dyn_cast<clang::ArrayType>(type))
                {
                    NOT_SUPPORT_CONT(!createArray(field->getName(), qt, at), field);
                }

                // ポインタ
                else if (clang::PointerType  const* pt=dyn_cast<clang::PointerType>(type))
                {
                    createBuiltin(field->getName(), qt);
                }

                // 参照
                else if (clang::ReferenceType const* rt=dyn_cast<clang::ReferenceType>(type))
                {
                    createBuiltin(field->getName(), qt);
                }

                // 組み込み型
                else if (clang::BuiltinType const* bt=dyn_cast<clang::BuiltinType>(type))
                {
                    createBuiltin(field->getName(), qt);
                }

                // テンプレート引数
                else if (TemplateTypeParmType const* ttpt=dyn_cast<TemplateTypeParmType>(type))
                {
                    createTemplateParm(field->getName(), qt, ttpt);
                }

                // クラス／構造体
                else if (clang::CXXRecordDecl const* crd=type->getAsCXXRecordDecl())
                {
                    createClass(field->getName(), qt, crd);
                }

                // enum型
                else if (clang::EnumType const* et=type->getAs<clang::EnumType>())
                {
                    WARNING_CONT(!createEnum(field->getName(), qt, et), field);
                }
            }
            if (!aIsFirst)
            {
                mLastVersion << "\n";
            }
            mLastVersion << "#include <theolizer/internal/version_auto.inc>\n";
        }

        // 手動型の時
        else
        {
            mLastVersion << "#include <theolizer/internal/version_manual.inc>\n";
        }
        mLastVersion << "#undef  THEOLIZER_GENERATED_VERSION_NO";

        // 新規生成時は、namespaceの終端、および、#endifを定義する
        if (aSourceStatus == eNew)
        {
            mLastVersion << "\n\n";
            // namespace対応
            for (int i=0; i < mNamespaceNestCount; ++i)
            {
                mLastVersion << "}";
            }
            if (mNamespaceNestCount) mLastVersion << "\n";

            // 終了
            mLastVersion << "#endif//THEOLIZER_WRITE_CODE // ###### "
                         << aClassName << " ######\n\n";
        }

        ASTANALYZE_OUTPUT("--------mLastVersion\n", mLastVersion.str(), "\n----------");
        for (auto element : aElementList)
        {
            ASTANALYZE_OUTPUT("    Element : ", element.first,
                              ", ", element.second.mNextName,
                              ", ", element.second.mType.getAsString());
        }

//----------------------------------------------------------------------------
//      １つ前の版生成
//----------------------------------------------------------------------------

        mPrevVersion.str("");

        CXXRecordDecl const* aTheolizerVersionPrev = iSerializeInfo.mTheolizerVersionPrev;
        if (aTheolizerVersionPrev)
        {
            ASTANALYZE_OUTPUT("Prev Version : ",
                              aTheolizerVersionPrev->getQualifiedNameAsString());

//      ---<<< 管理情報取り出し >>>---

            // struct Theolizerを探す
            CXXRecordDecl const* aTheolizer=nullptr;
            for (auto&& decl : aTheolizerVersionPrev->decls())
            {
                CXXRecordDecl const* crd=dyn_cast<clang::CXXRecordDecl>(decl);
                if (!crd || !crd->getIdentifier())
            continue;

                if (crd->getName().equals("Theolizer"))
                {
                    aTheolizer=crd;
            break;
                }
            }
            ERROR(!aTheolizer, aTheolizerVersionPrev, eAborted);

            // kVersionNo、kElementsMapping、クラス名取り出し
            auto&& aDecls = theolizer::getRBForIndexer(aTheolizer->decls());
            for (auto&& aDecl : aDecls)
            {
                VarDecl* vd = dyn_cast<VarDecl>(*aDecl);
                if (!vd)
            continue;

                // kVersionNo
                if (vd->getName().equals("kVersionNo"))
                {
                    unsigned    aVersionNo;
                    APValue* apvalue = vd->getEvaluatedValue();
                    if (!apvalue)  apvalue=vd->evaluateValue();
                    ERROR(!getVersionNumber(apvalue, aVersionNo), vd, eAborted);

                    mPrevVersion << "kVersionNo," << aVersionNo << ")\n";
                }

                // kElementsMapping
                else if (vd->getName().equals("kElementsMapping"))
                {
                    unsigned    aElementsMapping;
                    APValue* apvalue = vd->getEvaluatedValue();
                    if (!apvalue)  apvalue=vd->evaluateValue();
                    ERROR(!getVersionNumber(apvalue, aElementsMapping), vd, eAborted);
                    ERROR(aElementsMapping > 1, vd, eAborted);

                    mPrevVersion << "#define THEOLIZER_GENERATED_CLASS_NAME()\\\n";
                    AnnotationInfo aAnnotation=getAnnotationInfo(vd, AnnotateType::TS);
                    if (StringRef(aAnnotation.mParameter).startswith("TEMPLATE_NAME"))
                    {
                        mPrevVersion << "    THEOLIZER_INTERNAL_"
                                     << aAnnotation.mParameter << "\n";
                    }
                    else
                    {
                        mPrevVersion << "    THEOLIZER_INTERNAL_CLASS_NAME(("
                                     << aAnnotation.mParameter << "))\n";
                    }
                    // 手動型は順序固定なので出力しない
                    if (!iSerializeInfo.mIsManual)
                    {
                        mPrevVersion << "#define THEOLIZER_GENERATED_ELEMENT_MAP "
                                     << kElementsMapping[aElementsMapping] << "\n";
                    }
                    // 次のforループのため(forループの外で++すると終了判定に失敗する)
                    ++aDecl;
            break;
                }
            }

//      ---<<< 基底クラス情報取り出し >>>---

            aIsFirst=true;
            for (auto&& aDecl : aDecls)
            {
                clang::CXXMethodDecl const* cmd = 
                    dyn_cast<clang::CXXMethodDecl>(aDecl.front());
                // CXXMethodDecl以外のdeclなら、次はそのdeclから処理する
                if (!cmd)
            break;

                if (aIsFirst)
                {
                    aIsFirst=false;
                    mPrevVersion << "#define THEOLIZER_GENERATED_BASE_LIST()\\\n";
                }
                else
                {
                    mPrevVersion << "\\\n        THEOLIZER_GENERATED_SEP\\\n";
                }

                // 基底クラスが削除されているかどうか判定する
                ERROR(cmd->param_size() != 1, cmd, eAborted);
                clang::ParmVarDecl const* aParam=cmd->getParamDecl(0);
                QualType qt = aParam->getType().getCanonicalType();
                CXXRecordDecl const* aBase = qt->getAsCXXRecordDecl();
                std::string aKey = std::string("@")+aBase->getName().str();
                auto found = aElementList.find(aKey);           // 最新版にあるか確認
                bool aDeleted=true;
                if (found != aElementList.end())
                {
                    ERROR(found->second.mType != qt, aParam, eAborted);
                    aDeleted=false;
                    ASTANALYZE_OUTPUT("    Base = ", found->first, " : ",
                                      found->second.mType == qt, " : ", qt.getAsString());
                }

                // アノテーション文字列を加工して出力
                AnnotationInfo aAnnotation=getAnnotationInfo(cmd, AnnotateType::TS);
                std::size_t pos=aAnnotation.mParameter.find('(');
                ERROR(pos == StringRef::npos, cmd, eAborted);

                // クラスの完全自動型が半自動型に変化していたらKNをKIへ変更する。
                bool    aDoModify=false;
                do
                {
                    // クラスのDecl取り出し
                    clang::CXXRecordDecl const* crd=qt->getAsCXXRecordDecl();
                    if (!crd)
                break;

                    // クラス情報取り出し
                    auto found2 = mAstInterface.mSerializeListClass.find(crd);
                    if (!found2)
                break;

                    // 手動型はNOP
                    if (found2->second.mIsManual)
                break;

                    // 侵入型(KIへ変更する)
                    if (!found2->second.mNonIntrusive)
                    {
                        aDoModify=true;
                    }
                }
                while(0);
                if (aDoModify)
                {
                    mPrevVersion << "    THEOLIZER_INTERNAL_BASE_KI";
                }
                else
                {
                    mPrevVersion << "    " << aAnnotation.mParameter.substr(1, pos-1);
                }

                // 次で削除されていたら_DELを挿入する
                if (aDeleted)
                {
                    mPrevVersion << "_DEL" << aAnnotation.mParameter.substr(pos);
                }
                else
                {
                    mPrevVersion << aAnnotation.mParameter.substr(pos);
                }
            }
            if (!aIsFirst)
            {
                mPrevVersion << "\n";
            }

//      ---<<< メンバ変数情報取り出し >>>---

            aIsFirst=true;
            for (auto&& aDecl : aDecls)
            {
                clang::TypedefDecl const* td = dyn_cast<TypedefDecl>(aDecl.front());
                if (!td)
            continue;

                if (aIsFirst)
                {
                    aIsFirst=false;
                    mPrevVersion << "#define THEOLIZER_GENERATED_ELEMENT_LIST()\\\n";
                }
                else
                {
                    mPrevVersion << "\\\n";
                }

                // メンバ変数が削除されているかどうか判定する
                QualType qt=td->getUnderlyingType().getDesugaredType(*gASTContext);
                std::string aKey = td->getName().substr(1).str();    // 衝突防止用の'_'を取り除く
                auto found = aElementList.find(aKey);           // 最新版にあるか確認
                bool aDeleted=true;
                if (found != aElementList.end())
                {
                    aDeleted=false;
                    ASTANALYZE_OUTPUT("    Element = ", found->first,
                                      ", ",  found->second.mNextName,
                                      " : ", found->second.mType == qt,
                                      " : ", qt.getAsString());

#if 0
                    // 型が変化していたらエラー(配列なら基底クラスで比較する)
                    QualType aNewQt=getUnderlyingType(found->second.mType).getCanonicalType();
                    QualType aNowQt=getUnderlyingType(qt).getCanonicalType();
                    if (aNewQt != aNowQt)
                    {
                        gCustomDiag.ErrorReport(td->getLocation(),
                            "Does not change type(%0).") << found->first;
            continue;
                    }
#else
                    // 型が変化していたら、当変数が削除され、別変数が定義されたものとする
                    QualType aNewQt=getUnderlyingType(found->second.mType).getCanonicalType();
                    QualType aNowQt=getUnderlyingType(qt).getCanonicalType();
                    if (aNewQt != aNowQt)
                    {
                        found=aElementList.end();
                        aDeleted=true;
                    }
#endif
                }

                // アノテーション文字列を加工して出力
                AnnotationInfo aAnnotation=getAnnotationInfo(td, AnnotateType::TS);
                std::pair<StringRef, StringRef> temp =
                        StringRef(aAnnotation.mParameter).split('\\');
                std::size_t pos=temp.first.find('(');
                ERROR(pos == StringRef::npos, td, eAborted);

                // クラスの完全自動型が半自動型に変化していたらKNをKIへ変更する。
                bool    aDoModify=false;
                do
                {
                    // クラスのDecl取り出し
                    clang::CXXRecordDecl const* crd=qt->getAsCXXRecordDecl();
                    if (!crd)
                break;

                    // クラス情報取り出し
                    auto found2 = mAstInterface.mSerializeListClass.find(crd);
                    if (!found2)
                break;

                    // 手動型はNOP
                    if (found2->second.mIsManual)
                break;

                    // 侵入型(KIへ変更する)
                    if (!found2->second.mNonIntrusive)
                    {
                        aDoModify=true;
                    }
                }
                while(0);
                if (aDoModify)
                {
                    mPrevVersion << "    THEOLIZER_INTERNAL_ELEMENT_KI";
                }
                else
                {
                    mPrevVersion << "    " << temp.first.substr(1, pos-1).str();
                }

                // 次で削除されていたら_DELを追加してマクロ定義を"("まで生成する
                if (aDeleted)
                {
                    mPrevVersion << "_DEL(";
                }
                else
                {
                    mPrevVersion << "(";
                }
                // 第２パラメータ(dNextName)を置き換える
                std::size_t pos2=temp.first.find(',', pos+1);
                ERROR(pos2 == StringRef::npos, td, eAborted);
                mPrevVersion << temp.first.substr(pos+1, pos2-pos).str();
                std::size_t pos3=temp.first.find(',', pos2+1);
                ERROR(pos3 == StringRef::npos, td, eAborted);
                if (found == aElementList.end())
                {
                    mPrevVersion << temp.first.substr(pos3).str();
                }
                else
                {
                    mPrevVersion << found->second.mNextName.str()
                                 << temp.first.substr(pos3).str();
                }

                for (temp=temp.second.split('\\');
                     !temp.first.empty();
                     temp=temp.second.split('\\'))
                {
                    mPrevVersion << "\\\n        " << temp.first.str();
                }
            }

//      ---<<< クラス定義ソース生成 >>>---

            if (!aIsFirst)
            {
                mPrevVersion << "\n";
            }
            if (!iSerializeInfo.mIsManual)
            {
                mPrevVersion << "#include <theolizer/internal/version_auto.inc>\n";
            }
            else
            {
                mPrevVersion << "#include <theolizer/internal/version_manual.inc>\n";
            }
            mPrevVersion << "#undef  THEOLIZER_GENERATED_VERSION_NO";

            ASTANALYZE_OUTPUT("-------- mPrevVersion\n", mPrevVersion.str());
        }

        return aSourceStatus;
    }

// ***************************************************************************
//      clang内部バッファ上のソースを修正
// ***************************************************************************

    template<class tSerializeInfo>
    void modifySource(tSerializeInfo const& iSerializeInfo, SourceStatus iSourceStatus)
    {
        ASTANALYZE_OUTPUT("modifySource(", iSerializeInfo.mClassName, ");");

//      ---<<< 新規生成なら直接記録する >>>---

        if (iSourceStatus == eNew)
        {
            ASTANALYZE_OUTPUT("--------eNew : \n", mLastVersion.str(), "\n----------");
            mTheolizerHpp << mLastVersion.str();
    return;
        }

//      ---<<< 修正対象のソース位置獲得 >>>---

        // クラス定義部(LastVersionNo)
        //  バージョンアップの時はクラス定義範囲のみ
        //  アップデートの時はクラス定義部と最新版の定義部を含む範囲
        SourceLocation  aLastVersionNoBegin=iSerializeInfo.mLastVersionNoLoc;
        SourceLocation  aLastVersionNoEnd  =mLastVersionNoLoc->first;
        ASTANALYZE_OUTPUT("--- LastVersionNo ---");
        ASTANALYZE_OUTPUT("    ", aLastVersionNoBegin.printToString(*gSourceManager));
        ASTANALYZE_OUTPUT("    ", aLastVersionNoEnd.printToString(*gSourceManager));
        ASTANALYZE_OUTPUT("    mIsTheolizerHpp=", mLastVersionNoLoc->second.mIsTheolizerHpp);

        // 1つ前のバージョン定義部(VersionNo)
        SourceLocation  aPrevVersionNoBegin;
        SourceLocation  aPrevVersionNoEnd;
        bool aIsUpdateAllowedPrev=false;
        if (iSerializeInfo.mTheolizerVersionPrev)
        {
            aPrevVersionNoBegin=iSerializeInfo.mVersionNoPrevLoc;
            auto found=mAstInterface.mVersionNoLocations.upper_bound(
                iSerializeInfo.mVersionNoPrevLoc);
            ERROR(found == mAstInterface.mVersionNoLocations.end(),
                iSerializeInfo.mTheolizerVersionPrev);
            aPrevVersionNoEnd  =found->first;
            aIsUpdateAllowedPrev=found->second.mIsUpdateAllowed;
            ASTANALYZE_OUTPUT("--- PrevVersionNo ---");
            ASTANALYZE_OUTPUT("    ", aPrevVersionNoBegin.printToString(*gSourceManager));
            ASTANALYZE_OUTPUT("    ", aPrevVersionNoEnd.printToString(*gSourceManager));
            ASTANALYZE_OUTPUT("    ", aIsUpdateAllowedPrev);
            ASTANALYZE_OUTPUT("    mIsTheolizerHpp=", found->second.mIsTheolizerHpp);
        }

//      ---<<< 修正対象がユーザ・ソースならここで修正する >>>---

        if (!mLastVersionNoLoc->second.mIsTheolizerHpp)
        {
            // --- 最新版の処理 ---

            // バージョンアップなら最新版を置き換え
            if (iSourceStatus == eVersionUp)
            {
                ASTANALYZE_OUTPUT("--- eVersionUp ---");
                // ただし、変更禁止ならエラー
                if (!(mLastVersionNoLoc->second.mIsUpdateAllowed))
                {
                    gCustomDiag.ErrorReport(mTarget->getLocation(),
                        "Update is prohibited (%0).")
                        << iSerializeInfo.mTheolizerTarget->getName();
    return;
                }
                replaceString(aLastVersionNoBegin, aLastVersionNoEnd, mLastVersion.str());
            }

            // アップデートなら、最新版に変更があれば置き換え
            else if (getNowSource(aLastVersionNoBegin, aLastVersionNoEnd) != mLastVersion.str())
            {
                ASTANALYZE_OUTPUT("--- eUpdate ---");
                // ただし、変更禁止ならエラー
                if (!(mLastVersionNoLoc->second.mIsUpdateAllowed))
                {
                    gCustomDiag.ErrorReport(mTarget->getLocation(),
                        "Update is prohibited (%0).")
                        << iSerializeInfo.mTheolizerTarget->getName();
    return;
                }
                replaceString(aLastVersionNoBegin, aLastVersionNoEnd, mLastVersion.str());
            }

            // --- １つ前の版の処理 ---

            // １つ前の版が存在し、変更があれば置き換え
            if ((iSerializeInfo.mTheolizerVersionPrev)
             && (getNowSource(aPrevVersionNoBegin, aPrevVersionNoEnd) != mPrevVersion.str()))
            {
                ASTANALYZE_OUTPUT("--- modify prev-version ---");
                // ただし、変更禁止ならエラー
                if (!aIsUpdateAllowedPrev)
                {
                    gCustomDiag.ErrorReport(mTheolizerClass->getLocation(),
                        "Update is prohibited (%0).")
                        << iSerializeInfo.mTheolizerTarget->getName();
    return;
                }
                replaceString(aPrevVersionNoBegin, aPrevVersionNoEnd, mPrevVersion.str());
            }
    return;
        }

//      ---<<< 修正対象がデフォルト・ソース(mTheolizerHppへ出力する) >>>---

        // クラス定義全体(#ifdef THEOLIZER_WRITE_CODEと#endif)位置獲得
        FullSourceLoc   aLoc = iSerializeInfo.mLastVersionNoLoc.getExpansionLoc();
        ASTANALYZE_OUTPUT("    Location : ", aLoc.printToString(*gSourceManager));

        auto found=mAstInterface.mLocations.lower_bound(aLoc);
        if (found == mAstInterface.mLocations.end())
        {
            ASTANALYZE_OUTPUT("        not found");
        }
        else
        {
            ASTANALYZE_OUTPUT("        start : ", found->second.printToString(*gSourceManager));
            ASTANALYZE_OUTPUT("        end   : ", found->first.printToString(*gSourceManager));
        }
        ERROR_LOC((found == mAstInterface.mLocations.end())
           || (aLoc < found->second) || (found->first < aLoc), iSerializeInfo.mLastVersionNoLoc);
        SourceLocation  aTotalBegin=found->second;
        SourceLocation  aTotalEnd  =found->first;
        ASTANALYZE_OUTPUT("--- Total ---");
        ASTANALYZE_OUTPUT("    ", aTotalBegin.printToString(*gSourceManager));
        ASTANALYZE_OUTPUT("    ", aTotalEnd.printToString(*gSourceManager));

        // --- #ifdef THEOLIZER_WRITE_CODE～クラス用マクロ定義開始まで ---
        std::size_t offset=mTheolizerHpp.str().size();
        mTheolizerHpp << getNowSource(aTotalBegin, aLastVersionNoBegin.getLocWithOffset(-1));

        // --- 最新版 ---

        mTheolizerHpp << mLastVersion.str();

        // --- １つ前の版～#endif // THEOLIZER_WRITE_CODEまで ---

        // 1つ前の版がある時
        if (iSerializeInfo.mTheolizerVersionPrev)
        {
            mTheolizerHpp << getNowSource(aLastVersionNoEnd,
                aPrevVersionNoBegin.getLocWithOffset(-1));
            mTheolizerHpp << mPrevVersion.str();
            mTheolizerHpp << getNowSource(aPrevVersionNoEnd, aTotalEnd) << "\n";
        }

        // 1つ前の版がない時
        else
        {
            mTheolizerHpp << getNowSource(aLastVersionNoEnd, aTotalEnd) << "\n";
        }

        ASTANALYZE_OUTPUT("--- Source ---\n", mTheolizerHpp.str().substr(offset), "--------");
    }

// ***************************************************************************
//      グローバル・バージョン番号テーブル生成
// ***************************************************************************

    typedef std::map<TagDecl const*, std::vector<unsigned> > GlobalTable;

//----------------------------------------------------------------------------
//      既存情報獲得
//----------------------------------------------------------------------------

    GlobalTable getGlobalTableInfo(clang::CXXConstructorDecl const* iConstructor)
    {
        // 結果返却エリア
        GlobalTable aGlobalTable;

        if (!iConstructor)
    return aGlobalTable;

        if (!iConstructor->hasBody())
    return aGlobalTable;

        clang::Stmt* stmt0=iConstructor->getBody();
        ERROR(stmt0->getStmtClass() != clang::Stmt::CompoundStmtClass,
            iConstructor, aGlobalTable);
        clang::CompoundStmt* compound_stmt = cast<clang::CompoundStmt>(stmt0);

        if (compound_stmt->body_empty())
    return aGlobalTable;

        // add()関数を枚挙する
        for (auto stmt : compound_stmt->body())
        {
            ERROR(stmt->getStmtClass() != clang::Stmt::CXXMemberCallExprClass,
                iConstructor, aGlobalTable);

            // 結果の一時生成エリア
            clang::TagDecl const* aType=nullptr;
            std::vector<unsigned> aLocalTable;

            // add()関数のパラメータを枚挙する
            clang::CXXMemberCallExpr* expr = cast<clang::CXXMemberCallExpr>(stmt);
            bool aIsFirst=true;
            for (auto arg : expr->arguments())
            {
                // クラス/enum型取り出し
                if (aIsFirst)
                {
                    aIsFirst=false;
                    ERROR(arg->getStmtClass() != clang::Stmt::CXXTypeidExprClass,
                        iConstructor, aGlobalTable);
                    clang::CXXTypeidExpr* cte = cast<clang::CXXTypeidExpr>(arg);
                    QualType qt=cte->getTypeOperand(*gASTContext);
                    aType=qt->getAsTagDecl();
                    ERROR(!aType, iConstructor, aGlobalTable);
            continue;
                }

                // ローカル・バージョン番号取り出し
                ERROR(arg->getStmtClass() != clang::Stmt::IntegerLiteralClass,
                    iConstructor, aGlobalTable);
                clang::IntegerLiteral* aValue = cast<clang::IntegerLiteral>(arg);
                unsigned aLocalVersionNo;
                ERROR(!getControlNumber(aValue->getValue(), aLocalVersionNo),
                    iConstructor, aGlobalTable);
                aLocalTable.emplace_back(aLocalVersionNo);
            }
            aGlobalTable.emplace(aType, aLocalTable);
        }

        return aGlobalTable;
    }

//----------------------------------------------------------------------------
//      指定TagDecl用add()関数を生成する
//----------------------------------------------------------------------------

    void addAddFunc
    (
        std::string const& iTableName,
        GlobalTable const& iGlobalTable,
        unsigned iLastGlobalVersionNo,
        TagDecl const* iTagDecl,
        unsigned iLastVersionNo
    )
    {
        mTheolizerHpp << "        THEOLIZER_INTERNAL_ADD("
            << iTableName << ","
            << getQualifiedName(iTagDecl);

        auto found = iGlobalTable.find(iTagDecl);
        std::size_t aVersionCount=(found!= iGlobalTable.end())?found->second.size():0;
        for (std::size_t i=0; i < iLastGlobalVersionNo; ++i)
        {
            if (i < (iLastGlobalVersionNo-1)) 
            {
                if (i < aVersionCount)
                {
                    mTheolizerHpp << "," << found->second[i];
                }
                else
                {
                    // 完全自動型を半自動型としてバージョンを上げた場合、テーブルに登録されていない
                    // 従って、この場合のバージョンをとする。
                    mTheolizerHpp << ",1";
                }
            }
            else
            {
                mTheolizerHpp << "," << iLastVersionNo;
            }
            mTheolizerHpp << "u";
        }
        mTheolizerHpp << ");\n";
    }

//----------------------------------------------------------------------------
//      本体
//----------------------------------------------------------------------------

    void addGlobalVersionNoTable(CXXRecordDecl const* iGlobalVersionNoTableDecl)
    {
        mTheolizerHpp <<
            "#ifdef  THEOLIZER_WRITE_CODE // ###### Global VersionNo. Table ######\n\n";

//      ---<<< 最新のグローバル・バージョン番号取り出し >>>---

        ASTANALYZE_OUTPUT(iGlobalVersionNoTableDecl->getQualifiedNameAsString());
        unsigned aLastGlobalVersionNo=0;

        std::string aTableName=iGlobalVersionNoTableDecl->getName();
        for (auto decl : iGlobalVersionNoTableDecl->decls())
        {
            if (decl->getKind() != Decl::Var)
        continue;

            VarDecl* vd = cast<VarDecl>(decl);
            if (!vd->getIdentifier())
        continue;

            if (!vd->getName().equals("kLastGlobalVersionNo"))
        continue;

            APValue* apvalue = vd->getEvaluatedValue();
            if (!apvalue)  apvalue=vd->evaluateValue();
            ERROR(!getVersionNumber(apvalue, aLastGlobalVersionNo), iGlobalVersionNoTableDecl);
            ASTANALYZE_OUTPUT("aLastGlobalVersionNo=", aLastGlobalVersionNo);
        break;
        }

//      ---<<< デフォルト・コンストラクタ取り出し >>>---

        clang::CXXConstructorDecl const* aConstructor=nullptr;
        for (auto ctor : iGlobalVersionNoTableDecl->ctors())
        {
            // デフォルト・コンストラクタのみ処理する
            if (ctor->isDefaultConstructor())
            {
                aConstructor=ctor;
        break;
            }
        }

//      ---<<< 定義があるなら内容を取り出す >>>---

        GlobalTable aGlobalTable=getGlobalTableInfo(aConstructor);
        for (auto& data : aGlobalTable)
        {
            ASTANALYZE_OUTPUT(data.first->getQualifiedNameAsString());
            for (auto ver : data.second)
            {
                ASTANALYZE_OUTPUT("    ", ver);
            }
        }

//      ---<<< エラー・チェック >>>---

        // バージョン2以上なのに必要なGVNTがない
        if ((aGlobalTable.size() == 0) && (2 <= aLastGlobalVersionNo))
        {
            gCustomDiag.ErrorReport(
                (aConstructor)?
                    aConstructor->getLocation()
                    :
                    iGlobalVersionNoTableDecl->getLocation(),
                "Global Version No. Table error. Please check deleted class or enum.");
        }

//      ---<<< ソース生成 >>>---

        mTheolizerHpp <<
            "namespace theolizer{namespace internal{namespace{\n"
            "    RegisterLocalVersions::RegisterLocalVersions()\n"
            "    {\n";

        // 派生Serializer(BaseTypeInfo<>の特殊化を枚挙する)
        for (auto spec : mAstInterface.mMidSerializerDecl->specializations())
        {
            TemplateArgumentList const& tal=spec->getTemplateArgs();
            if (tal.size() != 1)
        continue;

            // 派生Serializer取り出し
            TemplateArgument const& ta = tal.get(0);
            QualType qt = ta.getAsType();

            CXXRecordDecl const* aSerializer = qt->getAsCXXRecordDecl();
            ERROR(aSerializer == nullptr, mAstInterface.mMidSerializerDecl);
            ASTANALYZE_OUTPUT("BaseTypeInfo<", aSerializer->getQualifiedNameAsString(), ">");

            // 最新版のバージョン番号取り出し
            unsigned aLastVersionNo=0;
            for (auto decl : aSerializer->decls())
            {
                if (decl->getKind() != Decl::Var)
            continue;

                VarDecl* vd = cast<VarDecl>(decl);
                if (!vd->getIdentifier())
            continue;

                if (!vd->getName().equals("kLastVersionNo"))
            continue;

                APValue* apvalue = vd->getEvaluatedValue();
                if (!apvalue)  apvalue=vd->evaluateValue();
                ERROR(!getVersionNumber(apvalue, aLastVersionNo),mAstInterface.mMidSerializerDecl);
                ASTANALYZE_OUTPUT("aLastVersionNo=", aLastVersionNo);
            break;
            }

            addAddFunc
            (
                aTableName,
                aGlobalTable,
                aLastGlobalVersionNo,
                aSerializer,
                aLastVersionNo
            );
        }

        // enum型
        for (auto&& aSerializeInfo : mAstInterface.mSerializeListEnum.getList())
        {
            // 完全自動なら、登録しない
            if (aSerializeInfo.second.mIsFullAuto)
        continue;

            addAddFunc
            (
                aTableName,
                aGlobalTable,
                aLastGlobalVersionNo,
                aSerializeInfo.second.mTheolizerTarget,
                aSerializeInfo.second.mLastVersionNo
            );
        }

        // クラス
        for (auto&& aSerializeInfo : mAstInterface.mSerializeListClass.getList())
        {
            // 完全自動なら、登録しない
            if (aSerializeInfo.second.mIsFullAuto)
        continue;

            CXXRecordDecl const* aClass = aSerializeInfo.second.mUniqueClass;
            if (!aClass) aClass = aSerializeInfo.second.mTheolizerTarget;

            addAddFunc
            (
                aTableName,
                aGlobalTable,
                aLastGlobalVersionNo,
                aClass,
                aSerializeInfo.second.mLastVersionNo
            );
        }
        mTheolizerHpp <<
            "    }\n"
            "}}} // namespace theolizer\n";

        mTheolizerHpp <<
            "\n#endif//THEOLIZER_WRITE_CODE // ###### Global VersionNo. Table ######\n";
    }

// ***************************************************************************
//      ソース修正全体
// ***************************************************************************

public:
    void process()
    {

//          ---<<< ソース修正 >>>---

        ASTANALYZE_OUTPUT("============ enum sorting ====================");
        std::map<unsigned, SerializeInfo<EnumDecl>* > aEnumList;
        for (auto&& aSerializeInfo : mAstInterface.mSerializeListEnum.getList())
        {
            // 完全自動でsave/load無しなら、処理しないことで削除する。
            if (aSerializeInfo.second.mIsFullAuto
             && (aSerializeInfo.second.mSerializeStat == esSerializeOnly))
        continue;

            unsigned index = mAstInterface.mSerializeListEnum.
                getIndex(aSerializeInfo.second.mTheolizerTarget);
            if (index == kInvalidIndex)
        continue;

            aEnumList.emplace(index, &(aSerializeInfo.second));
        }

        ASTANALYZE_OUTPUT("============ enum processing =================");
        for (auto&& aSerializeInfo : aEnumList)
        {
            ASTANALYZE_OUTPUT("enum processing : ",
                              aSerializeInfo.second->mTheolizerTarget->getQualifiedNameAsString(),
                              "=", aSerializeInfo.first);
            SourceStatus aSourceStatus=modifyEnum(*(aSerializeInfo.second));
            ASTANALYZE_OUTPUT("    aSourceStatus=", aSourceStatus);
            if (aSourceStatus != eAborted)
            {
                modifySource(*(aSerializeInfo.second), aSourceStatus);
            }
        }
        aEnumList.clear();  // 不要メモリ解放

        ASTANALYZE_OUTPUT("============ class sorting ===================");
        std::map<unsigned, SerializeInfo<CXXRecordDecl>* > aClassList;
        for (auto&& aSerializeInfo : mAstInterface.mSerializeListClass.getList())
        {
            // 完全自動でsave/load無しなら、処理しないことで削除する。
            if (aSerializeInfo.second.mIsFullAuto
             && (aSerializeInfo.second.mSerializeStat == esSerializeOnly))
        continue;

            unsigned index = mAstInterface.mSerializeListClass.
                getIndex(aSerializeInfo.second.mTheolizerTarget);
            if (index == kInvalidIndex)
        continue;

            aClassList.emplace(index, &(aSerializeInfo.second));
        }
        ASTANALYZE_OUTPUT("============ class processing ================");
        for (auto&& aSerializeInfo : aClassList)
        {
            ASTANALYZE_OUTPUT("class processing : ",
                              aSerializeInfo.second->mTheolizerTarget->getQualifiedNameAsString(),
                              "=", aSerializeInfo.first);
            SourceStatus aSourceStatus=modifyClass(*(aSerializeInfo.second));
            ASTANALYZE_OUTPUT("    aSourceStatus=", aSourceStatus);
            if (aSourceStatus != eAborted)
            {
                modifySource(*(aSerializeInfo.second), aSourceStatus);
            }
        }
        aClassList.clear(); // 不要メモリ解放

        ASTANALYZE_OUTPUT("============ GlobalVersionNoTable ============");
        ASTANALYZE_OUTPUT("mGlobalVersionNoTableDecl=", mAstInterface.mGlobalVersionNoTableDecl);
        ASTANALYZE_OUTPUT("mAstInterface.mLocationDefault=",
                          mAstInterface.mLocationDefault.isInvalid());
        // グローバル・バージョン番号テーブル無し
        if (mAstInterface.mGlobalVersionNoTableDecl == nullptr)
        {
            // インクルードされている時のみ追加
            if (!mAstInterface.mLocationDefault.isInvalid())
            {
                mTheolizerHpp <<
                    "#ifdef  THEOLIZER_WRITE_CODE // ###### Global VersionNo. Table ######\n"
                    "THEOLIZER_GENERATED_GLOBAL_TABLE();\n"
                    "#endif//THEOLIZER_WRITE_CODE // ###### Global VersionNo. Table ######\n";
            }
        }

        // グローバル・バージョン番号テーブル有り－要定義
        else //if (mAstInterface.mDefineGVNT)
        {
            addGlobalVersionNoTable(mAstInterface.mGlobalVersionNoTableDecl);
        }
        ASTANALYZE_OUTPUT("==============================================");

        //  デフォルト自動生成されたコードがないなら、ファイル保存なし
        mAstInterface.mDoSaveTheolizerHpp=true;
        ASTANALYZE_OUTPUT("hpp generation : tellp()=", mTheolizerHpp.tellp(),
                          " isInvalid()=", mAstInterface.mLocationDefault.isInvalid(),
                          " source=", gCompilingSourceName);
        // 空、かつ、インクルードされていないなら、保存しない
        if ((mTheolizerHpp.tellp() <= 0) && (mAstInterface.mLocationDefault.isInvalid()))
        {
            mAstInterface.mDoSaveTheolizerHpp=false;
        }

        // デフォルトの自動生成ソースを#includeしていないなら、エラー
        if (mAstInterface.mDoSaveTheolizerHpp && mAstInterface.mLocationDefault.isInvalid())
        {
            gCustomDiag.ErrorReport(SourceLocation(),
                    "Put #include \"%0\" for auto source generation.")
                    << gDefaultSourceName.filename().string();
    return;
        }

//          ---<<< 自動生成ソース反映（順序調整対応） >>>---

        FileID aFileId;
        if (mAstInterface.mDoSaveTheolizerHpp)
        {
            aFileId=mAstInterface.mLocationDefault.getFileID();
            SourceLocation start = gSourceManager->getLocForStartOfFile(aFileId);
            SourceLocation end   = gSourceManager->getLocForEndOfFile(aFileId);
            ASTANALYZE_OUTPUT("--------new\n", mTheolizerHpp.str(), "\n----------");
            ASTANALYZE_OUTPUT("--------now\n", getNowSource(start, end), "\n----------");
            if (getNowSource(start, end) != mTheolizerHpp.str())
            {
                ASTANALYZE_OUTPUT("    Not same!! ###############################");
                replaceString(start, end, mTheolizerHpp.str());
            }
        }

//          ---<<< ソース保存 >>>---

        ASTANALYZE_OUTPUT("mModifiedFiles.size()=", mModifiedFiles.size());

        // 致命的エラーが発生していたら保存しない
        if (gCustomDiag.getDiags().hasFatalErrorOccurred())
        {
            ASTANALYZE_OUTPUT("HandleTranslationUnit() aborted by Fatal Error.");
    return;
        }
        // Theolizerがエラー検出していたら保存しない
        if (gCustomDiag.hasErrorOccurred())
        {
            ASTANALYZE_OUTPUT("HandleTranslationUnit() aborted by Error.");
    return;
        }

        // 修正ファイルがないなら保存しない
        if (!mModifiedFiles.size())
    return;

//      ---<<< 保存処理 >>>---

        // 自動生成ファイルのみの更新ならば、排他制御なし
        if ((mModifiedFiles.size() == 1) && (*(mModifiedFiles.begin()) == aFileId))
        {
            bool error=mRewriter.overwriteChangedFiles();

#if defined(_WIN32)
            // Windowsで残るTMPファイル情報獲得
            gTempFile=gSourceManager->getFileEntryForID(aFileId)->getName();
#endif

            ASTANALYZE_OUTPUT("Update default file only. : overwriteChangedFiles()=", error);

            // 保存処理を実行したら、mRetryAST(最新版変更によるリトライ)をgRetryASTへ反映
            gRetryAST = mRetryAST;
            ASTANALYZE_OUTPUT("gRetryAST=", gRetryAST);
    return;
        }

        // 修正開始してみる。他が修正していたら、再処理する
        boostI::upgradable_lock<ExclusiveControl> lock_try(*gExclusiveControl,boostI::try_to_lock);
        ASTANALYZE_OUTPUT("pre lock_try");
        if (!lock_try)
        {
            ASTANALYZE_OUTPUT("post lock_try(1)");
    return;
        }
        ASTANALYZE_OUTPUT("post lock_try(2)");

        // 他のプロセスのファイル開放待ち
        boostI::scoped_lock<ExclusiveControl> lock(*gExclusiveControl);

        for (auto aFileId2 : mModifiedFiles)
        {
            const FileEntry* file = gSourceManager->getFileEntryForID(aFileId2);
            ASTANALYZE_OUTPUT(file->getName(), " ==============================================\n");
            std::string file_data;
            llvm::raw_string_ostream ss(file_data);
            mRewriter.getEditBuffer(aFileId2).write(ss);
            ss.flush();
            std::string file_body=normalizeLF(std::move(file_data));
            ASTANALYZE_OUTPUT(file_body);
            ASTANALYZE_OUTPUT("==============================================");
        }

        // 保存処理
        bool error=mRewriter.overwriteChangedFiles();
        ASTANALYZE_OUTPUT("overwriteChangedFiles()=", error);

        // 保存処理を実行したら、mRetryAST(最新版変更によるリトライ)をgRetryASTへ反映
        gRetryAST = mRetryAST;
        ASTANALYZE_OUTPUT("gRetryAST=", gRetryAST);
    }

// ***************************************************************************
//      生成と削除
// ***************************************************************************

private:
    // 入力メンバ
    Preprocessor&           mPreprocessor;          // プリプロセッサ
    AstInterface&           mAstInterface;          // AST解析処理とのI/F
    clang::PrintingPolicy const& mPrintingPolicy;

    // 処理用メンバ
    Rewriter                mRewriter;              // ソース修正用
    std::vector<FileID>     mModifiedFiles;         // 修正したソース

    std::stringstream       mLastVersion;           // 最新版マクロ定義ソース
    std::stringstream       mPrevVersion;           // １つ前の版マクロ定義ソース
    std::stringstream       mTheolizerHpp;          // デフォルトの自動生成ソース

    AstInterface::LocationMap::iterator    mLastVersionNoLoc;

    std::string             mPrevName;              // 前バージョンの要素名
    std::string             mDestination;           // 保存先指定
    std::string             mObjectTracking;        // オブジェクト追跡指定

    int                     mNamespaceNestCount;    // 名前空間のネスト数
    CXXRecordDecl const*    mTheolizerClass;        // 処理中のターゲット・クラス
    NamedDecl const*        mTarget;                // 処理中のターゲット(エラー表示用)

    bool                    mRetryAST;              // AST処理再実行要求(最新版を変更した時)

public:
    ModifySource(Preprocessor& iPreprocessor, AstInterface& iAstInterface) :
        mPreprocessor(iPreprocessor),
        mAstInterface(iAstInterface),
        mPrintingPolicy(gASTContext->getPrintingPolicy()),
        mRetryAST(false)

    {
        // リライター準備
        mRewriter.setSourceMgr(*gSourceManager, gASTContext->getLangOpts());
        mModifiedFiles.clear();
    }

    ~ModifySource()
    {
    }
};

#endif
