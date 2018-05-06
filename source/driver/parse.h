//############################################################################
//      AST解析
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

#if !defined(THEOLIZER_PARSE_H)
#define THEOLIZER_PARSE_H

// ***************************************************************************
//      デバッグ用グローバル変数
// ***************************************************************************

StringRef   gProcessingFile="";     // 処理中ソース・ファイル名
FineTimer   gProcessingTime;        // 解析／修正処理時間計測用

// ***************************************************************************
//      Decl処理
// ***************************************************************************

class PPCallbacksTracker : public PPCallbacks
{
public:
    PPCallbacksTracker(AstInterface& oAstInterface) :
        mAstInterface(oAstInterface),
        mTheolizerHpp(0),
        mLastVersionNoLoc(),
        mVersionNoLoc()
    {
        mUpdateEnabled.push(true);
    }

private:
    AstInterface&       mAstInterface;      // ソース修正処理とのI/F
    int                 mTheolizerHpp;      // *.theolizer.hpp中#includeネスト・レベル
    std::stack<bool>    mUpdateEnabled;     // 更新許可ヘッダ

//----------------------------------------------------------------------------
//      AST解析ファイル変化(#include)
//----------------------------------------------------------------------------

    void FileChanged
    (
        SourceLocation iLoc,
        FileChangeReason iReason,
        clang::SrcMgr::CharacteristicKind iFileType,
        FileID iPrevFID
    )
    {
        // ファイル名取り出し
        FullSourceLoc aLoc = gASTContext->getFullLoc(iLoc).getSpellingLoc();
        boostF::path aFileName = boostF::absolute(gSourceManager->getFilename(aLoc).str());
        aFileName=boostF::canonical(aFileName);

        int aTheolizerHpp=mTheolizerHpp;
        // 新規ファイルへ入った
        if (iReason == EnterFile)
        {
            mUpdateEnabled.push(true);
            ASTANALYZE_OUTPUT("Enter file : ", aFileName.string(),
                              " size()=", mUpdateEnabled.size());

            // 既に解析中なら、++ネスト
            if (mTheolizerHpp)
            {
                ++mTheolizerHpp;
            }
            // 解析中でなく*.theolizer.hppなら、解析中とする
            else if (aFileName == gDefaultSourceName)
            {
                ++mTheolizerHpp;
                mAstInterface.mLocationDefault=aLoc;
            }
        }
        // 元のファイルへ戻った
        else if (iReason == ExitFile )
        {
            if (mTheolizerHpp)
            {
                --mTheolizerHpp;
            }
            ASTANALYZE_OUTPUT("Exit file : ", aFileName.string(),
                              " size()=", mUpdateEnabled.size());
            mUpdateEnabled.pop();
        }

        // 変化した時のみ解析用出力
        if (aTheolizerHpp != mTheolizerHpp)
        {
            ASTANALYZE_OUTPUT("--- FileChanged :", aFileName.string(),
                " iReason=", iReason, " mTheolizerHpp=", mTheolizerHpp );
        }
    }

//----------------------------------------------------------------------------
//      自動生成ソース内の#ifdef THEOLIZER_WRITE_CODE位置獲得
//----------------------------------------------------------------------------

    SourceLocation  mTheolizerSTarget;

    void Ifdef(SourceLocation iLoc, Token const& iMacroNameTok, MacroDefinition const& iMD)
    {
        if (mTheolizerHpp == 0)
    return;

        StringRef aMacroName=StringRef(iMacroNameTok.getIdentifierInfo()->getNameStart());
        if (aMacroName.equals("THEOLIZER_WRITE_CODE"))
        {
            ASTANALYZE_OUTPUT("--- Ifdef : ", aMacroName);
            mTheolizerSTarget=iLoc;
        }
    }

//----------------------------------------------------------------------------
//      自動生成ソース内の#ifdef THEOLIZER_WRITE_CODEに対応する#endif位置獲得
//----------------------------------------------------------------------------

    void Endif(SourceLocation iLoc, SourceLocation iIfLoc)
    {
        if (mTheolizerHpp == 0)
    return;

        if (iIfLoc == mTheolizerSTarget)
        {
            ASTANALYZE_OUTPUT("--- Endif : ");

            // 開始位置
            SourceLocation aStart = iIfLoc.getLocWithOffset(-1);

            // 終了位置の次の次の行の先頭
            FullSourceLoc aLoc = gASTContext->getFullLoc(iLoc).getSpellingLoc();
            FileID aFileId=aLoc.getFileID();
            unsigned aLineNo=aLoc.getSpellingLineNumber();
            SourceLocation aEnd=gSourceManager->translateLineCol(aFileId, aLineNo+1, 1);

            ASTANALYZE_OUTPUT("    ", aStart.printToString(*gSourceManager));
            ASTANALYZE_OUTPUT("    ", aEnd.printToString(*gSourceManager));

            mAstInterface.mLocations.emplace
            (
                gASTContext->getFullLoc(aEnd),
                gASTContext->getFullLoc(aStart)
            );

            mTheolizerSTarget=SourceLocation();
        }
    }

//----------------------------------------------------------------------------
//      更新許可取得／THEOLIZER_GENERATING_LOCATION位置の更新許可取得
//----------------------------------------------------------------------------

    void MacroExpands
    (
        Token const& iMacroNameTok,
        MacroDefinition const& iMD,
        clang::SourceRange iRange,
        clang::MacroArgs const* iArgs
    )
    {
        StringRef aMacroName=StringRef(iMacroNameTok.getIdentifierInfo()->getNameStart());
        if (aMacroName.equals("THEOLIZER_PROVIDED_BY"))
        {
            Token const* arg=iArgs->getUnexpArgument(0);
            if (arg && arg->isLiteral())
            {
                StringRef aArg=StringRef(arg->getLiteralData(), arg->getLength());
                if (!gDeveloper.empty() && aArg.startswith(gDeveloper))
                {
                    mUpdateEnabled.top()=true;
                }
                else
                {
                    mUpdateEnabled.top()=false;
                }
                ASTANALYZE_OUTPUT("--- MacroExpands : ", aMacroName,
                                  " mUpdateEnabled=", mUpdateEnabled.top());
            }
            else
            {
                gCustomDiag.ErrorReport(iRange.getBegin(), "Illegal format.");
            }
        }
        else if (aMacroName.equals("THEOLIZER_GENERATING_LOCATION"))
        {
            FullSourceLoc aLoc = gASTContext->getFullLoc(iRange.getBegin());
            mAstInterface.mGeneratingLocationsMacro.emplace
            (
                aLoc, 
                mTheolizerHpp || mUpdateEnabled.top()
            );
        }
    }

//----------------------------------------------------------------------------
//      #define位置獲得
//----------------------------------------------------------------------------

    SourceLocation  mLastVersionNoLoc;  // #define位置
    SourceLocation  mVersionNoLoc;      // #define位置

    void MacroDefined(Token const& iMacroNameTok, MacroDirective const* iMD) override
    {
        StringRef aMacroName=StringRef(iMacroNameTok.getIdentifierInfo()->getNameStart());
        if (!aMacroName.startswith("THEOLIZER"))
    return;

        if (aMacroName.startswith("THEOLIZER_GENERATED"))
        {
            if (aMacroName.equals("THEOLIZER_GENERATED_VERSION_NO"))
            {
                ASTANALYZE_OUTPUT("--- MacroDefined : ", aMacroName);
                mVersionNoLoc=iMD->getMacroInfo()->getDefinitionLoc();
    return;
            }

            if (aMacroName.equals("THEOLIZER_GENERATED_LAST_VERSION_NO"))
            {
                ASTANALYZE_OUTPUT("--- MacroDefined : ", aMacroName);
                mLastVersionNoLoc=iMD->getMacroInfo()->getDefinitionLoc();
    return;
            }

            if (aMacroName.equals("THEOLIZER_GENERATED_CLASS_TYPE")
             || aMacroName.equals("THEOLIZER_GENERATED_FULL_AUTO")
             || aMacroName.equals("THEOLIZER_GENERATED_ENUM_TYPE"))
            {
                ASTANALYZE_OUTPUT("--- MacroDefined : ", aMacroName);
                // getDefinitionEndLoc()は最後のトークン先頭位置を返す
#if 0           // しかし、ReplaceTextはトークンの最後まで置き換えてくれる
                mAstInterface.mLastVersionNoLocations.emplace
                (
                    gASTContext->getFullLoc(iMD->getMacroInfo()->getDefinitionEndLoc()),
                    AstInterface::UpdateInfo
                    (
                        gASTContext->getFullLoc(mLastVersionNoLoc),
                        (!!mTheolizerHpp),
                        (!!mTheolizerHpp) || mUpdateEnabled.top()
                    )
                );
#else
                // 次の行の先頭を求め、そこから-1して、現在行の最後とする
                SourceLocation aMacroLoc = iMD->getMacroInfo()->getDefinitionEndLoc();
                FullSourceLoc aLoc = gASTContext->getFullLoc(aMacroLoc).getSpellingLoc();
                FileID aFileId=aLoc.getFileID();
                unsigned aLineNo=aLoc.getSpellingLineNumber();
                aMacroLoc=gSourceManager->translateLineCol(aFileId, aLineNo+1, 1);
                aMacroLoc=aMacroLoc.getLocWithOffset(-1);
                mAstInterface.mLastVersionNoLocations.emplace
                (
                    gASTContext->getFullLoc(aMacroLoc),
                    AstInterface::UpdateInfo
                    (
                        gASTContext->getFullLoc(mLastVersionNoLoc),
                        (!!mTheolizerHpp),
                        (!!mTheolizerHpp) || mUpdateEnabled.top()
                    )
                );
#endif
    return;
            }
        }

        // 解析しない
        if (aMacroName.equals("THEOLIZER_NO_ANALYZE"))
        {
            mAstInterface.mNotParse=true;
        }
    }

//----------------------------------------------------------------------------
//      #undef位置獲得
//----------------------------------------------------------------------------

    void MacroUndefined(Token const& iMacroNameTok, MacroDefinition const& iMD) override
    {
        StringRef aMacroName=StringRef(iMacroNameTok.getIdentifierInfo()->getNameStart());
        if (aMacroName.equals("THEOLIZER_GENERATED_VERSION_NO"))
        {
            ASTANALYZE_OUTPUT("--- MacroUndefined :", aMacroName);
            mAstInterface.mVersionNoLocations.emplace
            (
                gASTContext->getFullLoc(iMacroNameTok.getEndLoc()),
                AstInterface::UpdateInfo
                (
                    gASTContext->getFullLoc(mVersionNoLoc),
                    (!!mTheolizerHpp),
                    (!!mTheolizerHpp) || mUpdateEnabled.top()
                )
            );
        }
    }
};

// ***************************************************************************
//      ASTConsumer派生クラス
// ***************************************************************************

class ASTVisitor : public DeclVisitor<ASTVisitor, bool>
{
private:

//----------------------------------------------------------------------------
//      非侵入型のTheolizerTarget;からCXXRecordDecl/EnumDeclを取り出す
//          テンプレートなら、プライマリー定義まで展開する
//          エラー時false
//----------------------------------------------------------------------------

    bool getNonIntrusiveTarget(TypedefDecl* iTypedefDecl,
                               CXXRecordDecl const*& oTargetClass,
                               EnumDecl const*& oTargetEnum)
    {
        QualType qt=iTypedefDecl->getUnderlyingType().getDesugaredType(*gASTContext);
        switch (qt->getTypeClass())
        {
        case Type::Enum:
            {
                EnumType const* et = qt->getAs<EnumType>();
                if (!et)
    return false;
                oTargetEnum = et->getDecl();
                if (!oTargetEnum)
    return false;
            }
    return true;

        // 通常クラスと特殊化テンプレート
        case Type::Record:
            {
                oTargetClass = qt->getAsCXXRecordDecl();
                if (!oTargetClass)
    return false;

                // プライマリー・テンプレート展開を試みる
                oTargetClass = getPrimary(oTargetClass);
                ERROR(!oTargetClass, oTargetClass, false);
            }
            break;

        // テンプレート(プライマリーと部分特殊化)
        case Type::TemplateSpecialization:
            {
                // プライマリー・テンプレートを得る
                //  部分特殊化や特殊化のDeclを得ることができない。
                //  C++言語仕様上、部分特殊化や特殊化に一致するとは限らないので
                //  仕方がないと思う。
                TemplateSpecializationType const* tst=qt->getAs<TemplateSpecializationType>();

#if 0   // テンプレート仮パラメータ表示
                for (unsigned i=0; i < tst->getNumArgs(); ++i)
                {
                    ASTANALYZE_OUTPUT("     [", i, "]=",
                            tst->getArg(i).getAsType().getAsString());
                }
#endif
                TemplateDecl* td = tst->getTemplateName().getAsTemplateDecl();
                if (!td)
    return false;
                ClassTemplateDecl* ctd = dyn_cast<ClassTemplateDecl>(td);
                if (!ctd)
    return false;
                oTargetClass = searchDefinition(ctd);;
                if (!oTargetClass)
    return false;
            }
            break;

            // その他
            default:
    return false;
        }

        return true;
    }

//----------------------------------------------------------------------------
//      class/structの処理
//          enumは非侵入型処理なので、structで実装されることになる。
//          2015/05/14現在enumは非対応。
//
//          2016/01/06
//          TheolizerVersion<>も直接呼ばれる。
//              恐らく、クラス外定義しているから呼ばれると思う。
//          また、TheolizerVersion<>のdecls()に再度TheolizerVersion<>が現れる。
//              こちらはgetQualifiedNameAsString()でテンプレート・パラメータも
//              含まれる。取り除く方法がないか幾つかトライしたが失敗。
//          入口でTheolizerVersion<>判定して処理する。
//----------------------------------------------------------------------------

public:
    virtual bool VisitCXXRecordDecl(CXXRecordDecl *iCXXRecordDecl)
    {
ASTANALYZE_OUTPUT("VisitCXXRecordDecl(", iCXXRecordDecl->getQualifiedNameAsString(), ")");
ASTANALYZE_OUTPUT("    Def=" ,iCXXRecordDecl->isThisDeclarationADefinition(),
                  " mIsTheolizerSpace=" ,mIsTheolizerSpace);
        getAnnotationInfo(iCXXRecordDecl, AnnotateType::None);

//      ---<<< 解析準備 >>>---

        // 定義でないなら処理しない
        if (!iCXXRecordDecl->isThisDeclarationADefinition())
    return true;

        // unionや名前無し自身はシリアライズ対象としない
        if ((iCXXRecordDecl->isUnion())
         || (!iCXXRecordDecl->getIdentifier()))
    return true;

        // std空間なら登録だけする
        if (mIsStdSpace)
        {
            mAstInterface.mSerializeListClass.addCandidate(iCXXRecordDecl);
    return true;
        }

        // クラス名取り出し
        std::string aClassName=iCXXRecordDecl->getQualifiedNameAsString();

        // ::Theolizerを含むクラス(TheolizerVersionを除く)は処理しない
        {
            std::size_t pos=aClassName.find("::Theolizer");
            if (pos != std::string::npos)
            {
                if (aClassName.compare(
                    pos+sizeof("::Theolizer")-1, sizeof("Version")-1, "Version") != 0)
                {
                    ASTANALYZE_OUTPUT("    ::Theolizer does not process.");
    return true;
                }
            }
        }

//      ---<<< グローバル・バージョン番号テーブル処理 >>>---
//       theolizer::internal::GlobalVersionNoTableを継承していたら、それを設定する(追跡しない)

        for (auto base : iCXXRecordDecl->bases())
        {
            Type const* type = base.getType().split().Ty;
            CXXRecordDecl* aBaseDecl = type->getAsCXXRecordDecl();
            if (aBaseDecl
             && (aBaseDecl->getName().equals("GlobalVersionNoTable")))
            {
                if (mAstInterface.mGlobalVersionNoTableDecl)
                {
                    gCustomDiag.ErrorReport(iCXXRecordDecl->getLocation(),
                        "Multiple definition of THEOLIZER_DEFINE_GLOBAL_VERSION_TABLE().");

                    gCustomDiag.NoteReport(mAstInterface.mGlobalVersionNoTableDecl->getLocation(),
                        "previous definition here");
                }
                mAstInterface.mGlobalVersionNoTableDecl = iCXXRecordDecl;
                ASTANALYZE_OUTPUT("    mGlobalVersionNoTableDecl=",
                                  iCXXRecordDecl->getQualifiedNameAsString());
    return true;
            }
        }

//      ---<<< theolizer名前空間内なら追跡しない >>>---

        if (mIsTheolizerSpace)
    return true;

//      ---<<< シリアライズ対象判定 >>>---
//      アノテーションの有無チェックとバージョン取り出し

        bool                    aIsSerialize=false;
        bool                    aIsNonIntrusive=false;
        bool                    aIsVertion=false;
        bool                    aIsFullAuto=false;
        AnnotationInfo          aAnnotation;
        AnnotationInfo          aAnnotationTS;
        unsigned                aVersionNo=0;
        unsigned                aLastVersionNo=0;
        unsigned                aElementsMapping=kInvalidNumber;
        unsigned                aEnumSaveType=kInvalidNumber;
        CXXRecordDecl const*    aUniqueClass=nullptr;
        CXXRecordDecl const*    aTargetClass=nullptr;
        EnumDecl const*         aTargetEnum=nullptr;
        FullSourceLoc           aLastVersionNoLoc;
        FullSourceLoc           aVersionNoLoc;

        // TheolizerTarget(シリアライズ対象クラス)とTheolizerClass(処理用クラス)取り出し
        //  TheolizerTargetが定義されていたら、シリアライズ指定有りとする
        TypedefDecl* aTheolizerTarget=nullptr;
        auto&& aDecls = theolizer::getRBForIndexer(iCXXRecordDecl->decls());
        for (auto&& aDecl : aDecls)
        {
            if (aDecl.front()->getKind() != Decl::Typedef)
        continue;

            TypedefDecl* td = cast<TypedefDecl>(aDecl.front());
            if (td->getName().equals("TheolizerTarget"))
            {
                aTheolizerTarget=td;
                aIsSerialize = true;
                aAnnotationTS=getAnnotationInfo(aTheolizerTarget, AnnotateType::TS);
        break;
            }
            // 自動生成禁止なら処理中断
            else if (td->getName().equals("NoGenerate"))
            {
        break;
            }
        }
        ASTANALYZE_OUTPUT("    aIsSerialize=", aIsSerialize);

        // iCXXRecordDeclの種別判定
        if (aIsSerialize)
        {
            aTargetClass=iCXXRecordDecl;
            if (aTargetClass->getName() == "TheolizerVersion")
            {
                ASTANALYZE_OUTPUT("++++Version : ", aClassName);
                aIsVertion=true;

                // 親クラス取り出し
                DeclContext const* memberof = iCXXRecordDecl->getDeclContext();
                CXXRecordDecl const* crd = dyn_cast<CXXRecordDecl>(memberof);
                if (crd)
                {
                    aTargetClass=crd;
                }
            }
            if (aTargetClass->getName() == "TheolizerNonIntrusive")
            {
                ASTANALYZE_OUTPUT("++++NonIntrusive");
                aIsNonIntrusive=true;
            }
            else
            {
                ASTANALYZE_OUTPUT("++++Intrusive : ", aClassName);
                // class/structのみサポート
                if ((aTargetClass->getTagKind() != clang::TTK_Struct)
                 && (aTargetClass->getTagKind() != clang::TTK_Class))
                {
                    gCustomDiag.ErrorReport(iCXXRecordDecl->getLocation(),
                            "Not support union.");
return true;
                }
            }

            if (aIsNonIntrusive)
            {
                // 非侵入型対象のDeclを取り出す
                aTargetClass=nullptr;   // 非侵入型はTheolizerTargetから生成する
                ERROR(!getNonIntrusiveTarget(aTheolizerTarget, aTargetClass, aTargetEnum),
                     aTheolizerTarget, true);

                if (aTargetClass)
                {
                    ASTANALYZE_OUTPUT("    aTargetClass=",
                        aTargetClass->getQualifiedNameAsString(), " ", aTargetClass);
                }
                if (aTargetEnum)
                {
                    ASTANALYZE_OUTPUT("    aTargetEnum=",
                        aTargetEnum->getQualifiedNameAsString(), " ", aTargetEnum);
                }
            }

            // Theolizerクラスまで送る(途中TheolizerClassがあったらアノテーションを取り出す)
            CXXRecordDecl* aTheolizer=nullptr;
            for (auto&& aDecl : aDecls)
            {
                if (aDecl.front()->getKind() == Decl::Typedef)
                {
                    TypedefDecl* td = cast<TypedefDecl>(aDecl.front());
                    if (td->getName().equals("TheolizerClass"))
                    {
                        // クラスへのアノテーション取り出し
                        aAnnotation = getAnnotationInfo(td, AnnotateType::Class);
                    }
            continue;
                }

                if (aDecl.front()->getKind() != Decl::CXXRecord)
            continue;

                CXXRecordDecl* crd = cast<CXXRecordDecl>(aDecl.front());
                if (!crd->getIdentifier())
            continue;

                if (crd->getName().equals("Theolizer"))
                {
                    aTheolizer=crd;
            break;
                }
            }
            // Theolizerがなければエラー
           ERROR(!aTheolizer, iCXXRecordDecl, true);

            // アノテーションとバージョン番号取り出し
            for (auto decl : aTheolizer->decls())
            {
                // 定数獲得
                if (decl->getKind() == Decl::Var)
                {
                    VarDecl* vd = cast<VarDecl>(decl);
                    if (!vd->getIdentifier())
            continue;
                    // kVersionNo
                    if ((aVersionNo == 0)
                     && (vd->getName().equals("kVersionNo")))
                    {
                        aVersionNoLoc = gASTContext->getFullLoc(vd->getLocation()).
                            getSpellingLoc();

                        APValue* apvalue = vd->getEvaluatedValue();
                        if (!apvalue)  apvalue=vd->evaluateValue();
                        ERROR(!getVersionNumber(apvalue, aVersionNo),iCXXRecordDecl,true);
                        ASTANALYZE_OUTPUT("    aVersionNo=", aVersionNo);
                    }

                    // kLastVersionNo
                    else if ((aLastVersionNo == 0)
                            && (vd->getName().equals("kLastVersionNo")))
                    {
                        aLastVersionNoLoc = gASTContext->getFullLoc(vd->getLocation()).
                            getSpellingLoc();

                        APValue* apvalue = vd->getEvaluatedValue();
                        if (!apvalue)  apvalue=vd->evaluateValue();
                        ERROR(!getVersionNumber(apvalue, aLastVersionNo),iCXXRecordDecl,true);
                    }

                    // kIsFullAuto(完全自動なら登録しない)
                    else if (vd->getName().equals("kIsFullAuto"))
                    {
                        APValue* apvalue = vd->getEvaluatedValue();
                        if (!apvalue)  apvalue=vd->evaluateValue();
                        ERROR(!getBool(apvalue, aIsFullAuto),iCXXRecordDecl,true);
                        ASTANALYZE_OUTPUT("    aIsFullAuto=", aIsFullAuto);
                    }

                    // kElementsMapping
                    else if (vd->getName().equals("kElementsMapping"))
                    {
                        APValue* apvalue = vd->getEvaluatedValue();
                        if (!apvalue)  apvalue=vd->evaluateValue();
                        ERROR(!getVersionNumber(apvalue, aElementsMapping),iCXXRecordDecl,true);
                        ASTANALYZE_OUTPUT("    aElementsMapping=", aElementsMapping);
            break;
                    }

                    // kEnumSaveType
                    else if (vd->getName().equals("kEnumSaveType"))
                    {
                        APValue* apvalue = vd->getEvaluatedValue();
                        if (!apvalue)  apvalue=vd->evaluateValue();
                        ERROR(!getVersionNumber(apvalue, aEnumSaveType),iCXXRecordDecl,true);
                        ASTANALYZE_OUTPUT("    aEnumSaveType=", aEnumSaveType);
            break;
                    }
                }

                // キー用のクラス獲得(friend指定)
                else if (decl->getKind() == Decl::Friend)
                {
                    FriendDecl* fd = cast<FriendDecl>(decl);
                    clang::TypeSourceInfo* tsi = fd->getFriendType();
                    ERROR(!tsi, fd, true);
                    QualType qt = tsi->getType();
                    aUniqueClass = qt->getAsCXXRecordDecl();
                    ERROR(!aUniqueClass, fd, true);
                    ASTANALYZE_OUTPUT("    aUniqueClass=",
                                      aUniqueClass->getQualifiedNameAsString(),
                                      " ", aUniqueClass,
                                      " : Definition=", aUniqueClass->getDefinition());
                    // もし、friend指定クラスが未定義なら、getDefinition()は0になる
                }
            }
        }

//      ---<<< シリアライズ候補の登録 >>>---

        if (!aIsNonIntrusive && !aIsVertion)
        {
            mAstInterface.mSerializeListClass.addCandidate(iCXXRecordDecl);
        }

//      ---<<< シリアライズ対象の登録 >>>---

        if (aIsSerialize)
        {
            ERROR(!aTargetClass && !aTargetEnum, iCXXRecordDecl, true);

//      ---<<< 非侵入型のターゲット・クラス登録 >>>---

            if (aIsNonIntrusive && !aIsVertion)
            {
                if (aLastVersionNo == 0)
                {
                    gCustomDiag.ErrorReport(iCXXRecordDecl->getLocation(),
                        "LastVersionNo(0) is illegal.");
    return true;
                }

                if (aTargetClass)
                {
                    ERROR(aElementsMapping == kInvalidNumber, iCXXRecordDecl, true);
                    mAstInterface.mSerializeListClass.addSerializable
                    (
                        aTargetClass,
                        aUniqueClass,
                        aLastVersionNo,
                        aElementsMapping,
                        aAnnotation,
                        aAnnotationTS,
                        iCXXRecordDecl,
                        !aIsFullAuto,
                        aIsFullAuto
                    );
                    ASTANALYZE_OUTPUT("----NonIntrusive aTargetClass=", aTargetClass,
                                      " aUniqueClass=", aUniqueClass);
                }
                if (aTargetEnum)
                {
                    ERROR(aEnumSaveType == kInvalidNumber, iCXXRecordDecl, true);
                    mAstInterface.mSerializeListEnum.addSerializable
                    (
                        aTargetEnum,
                        nullptr,
                        aLastVersionNo,
                        aEnumSaveType,
                        aAnnotation,
                        aAnnotationTS,
                        iCXXRecordDecl,
                        false,
                        aIsFullAuto
                    );
                    ASTANALYZE_OUTPUT("----NonIntrusive aTargetEnum=", aTargetEnum);
                }
            }

//      ---<<< 侵入型登録 >>>---
//          現時点では半自動型のみ

            else if (!aIsVertion)
            {
                ERROR(aLastVersionNo == 0, iCXXRecordDecl, true);
                ERROR(aElementsMapping == kInvalidNumber, iCXXRecordDecl, true);
                mAstInterface.mSerializeListClass.addSerializable
                (
                    iCXXRecordDecl,
                    aUniqueClass,
                    aLastVersionNo,
                    aElementsMapping,
                    aAnnotation,
                    aAnnotationTS,
                    nullptr,
                    false,
                    aIsFullAuto
                );
                ASTANALYZE_OUTPUT("----Intrusive aTargetClass=", aTargetClass,
                                  " aUniqueClass=", aUniqueClass);
            }

//      ---<<< TheolizerVersion<>追加登録 >>>---
//      最新版とその１つ前について追加登録する

            else
            {
                if (aLastVersionNo == 0)
                {
                    gCustomDiag.ErrorReport(iCXXRecordDecl->getLocation(),
                        "LastVersionNo(0) is illegal.");
    return true;
                }

                ERROR(aVersionNo == 0, iCXXRecordDecl, true);
                if (aTargetClass)
                {
                    mAstInterface.mSerializeListClass.annexVersion(
                        aTargetClass, iCXXRecordDecl, aVersionNo,
                        aLastVersionNoLoc, aVersionNoLoc);
                    ASTANALYZE_OUTPUT("----Version aTargetClass=", aTargetClass);
                }
                if (aTargetEnum)
                {
                    mAstInterface.mSerializeListEnum.annexVersion(
                        aTargetEnum, iCXXRecordDecl, aVersionNo,
                        aLastVersionNoLoc, aVersionNoLoc);
                    ASTANALYZE_OUTPUT("----Version aTargetEnum=", aTargetEnum);
                }
            }
        }

        return true;
    }

//----------------------------------------------------------------------------
//      クラス・テンプレートの処理（プライマリ）
//          NamedDeclではあるが、TagDeclではない。
//          つまり、CXXRecordDeclの派生クラスでないので注意
//
//          CXXRecordDecl::getDescribedClassTemplate()を使えば、
//          そのCXXRecordDeclオブジェクトを定義したClassTemplateDecl
//          を取り出すことができる。
//----------------------------------------------------------------------------

    virtual bool VisitClassTemplateDecl(ClassTemplateDecl *iClassTemplateDecl)
    {
        ASTANALYZE_OUTPUT("VisitClassTemplateDecl(",
                          iClassTemplateDecl->getQualifiedNameAsString(),
                          " : ",  iClassTemplateDecl,
                          " : ",  iClassTemplateDecl->getCanonicalDecl(), ")");
        getAnnotationInfo(iClassTemplateDecl, AnnotateType::None);

        if (mIsTheolizerSpace)
        {
            if (iClassTemplateDecl->getName().equals("Switcher"))
            {
                mSwicher = iClassTemplateDecl;
            }

            else if (iClassTemplateDecl->getName().equals("RegisterToBaseClass"))
            {
                mRegisterToBaseClass = iClassTemplateDecl;
            }

            else if (iClassTemplateDecl->getName().equals("SerializerVersion"))
            {
                mAstInterface.mMidSerializerDecl = iClassTemplateDecl;
            }

            else
            {
                CXXRecordDecl* crd=iClassTemplateDecl->getTemplatedDecl();
                AnnotationInfo ret=getAnnotationInfo(crd, AnnotateType::Class);
                if (ret.mAnnotate == AnnotationInfo::CS)
                {
                    mAstInterface.mSerializeListClass.addCandidate(crd);
                }
            }
return true;
        }

        // 定義なら追跡する
        if (iClassTemplateDecl->isThisDeclarationADefinition())
        {
            Visit(iClassTemplateDecl->getTemplatedDecl());
        }

        return true;
    }

//----------------------------------------------------------------------------
//      関数テンプレート
//          下記関数を抽出する
//              theolizer::internal::save
//              theolizer::internal::load
//----------------------------------------------------------------------------

    virtual bool VisitFunctionTemplateDecl(FunctionTemplateDecl* iFunctionTemplateDecl)
    {
        if (!mIsTheolizerSpace)
    return true;

        // 既にsave/loadが見つかっていたら、それ以上チェック不要
        if (mHasSaveLoad)
    return true;

        if ((iFunctionTemplateDecl->getName() != "save")
         && (iFunctionTemplateDecl->getName() != "load"))
    return true;

        // spec(特殊化)はclang::FunctionDecl*型
        for (auto spec : iFunctionTemplateDecl->specializations())
        {
            // Bodyがあるものだけ
            if (!spec->hasBody())
        continue;

            FunctionTemplateSpecializationInfo* ftsi = spec->getTemplateSpecializationInfo();
            TemplateArgumentList const* tal = ftsi->TemplateArguments;
            ERROR(tal->size() != 4, iFunctionTemplateDecl, true);
            // テンプレート・パラメータ取り出し
            TemplateArgument const& ta = tal->get(3);
            QualType qt = ta.getAsType();
            switch (qt->getTypeClass())
            {
            case Type::Record:
                mHasSaveLoad=true;
                break;

            case Type::Enum:
                mHasSaveLoad=true;
                break;

            default:
                break;
            }
        }

        return true;
    }

//----------------------------------------------------------------------------
//      各種追跡処理
//----------------------------------------------------------------------------

//      ---<<< namespace >>>---

    virtual bool VisitNamespaceDecl(NamespaceDecl *iNamespaceDecl)
    {
        getAnnotationInfo(iNamespaceDecl, AnnotateType::None);
        StringRef name = iNamespaceDecl->getName();

        // theolizer名前空間処理
        if (name.equals("theolizer"))
        {
            AutoFalse auto_false(mIsTheolizerSpace);
            enumerateDecl(iNamespaceDecl);
    return true;
        }

        // std名前空間は処理しない
        // 他にも不要な名前空間は処理しない方がよいが、特定できない。
        if (name.equals("std"))
        {
            AutoFalse auto_false(mIsStdSpace);
            enumerateDecl(iNamespaceDecl);
    return true;
        }

        enumerateDecl(iNamespaceDecl);
        return true;
    }

//      ---<<< extern "C"/"C++" >>>---

    virtual bool VisitLinkageSpecDecl(LinkageSpecDecl *iLinkageSpecDecl)
    {
        getAnnotationInfo(iLinkageSpecDecl, AnnotateType::None);

        switch (iLinkageSpecDecl->getLanguage())
        {
        case LinkageSpecDecl::lang_c:           // "C" : NOP
            enumerateDecl(iLinkageSpecDecl);
            break;

        case LinkageSpecDecl::lang_cxx:         // "C++"
            enumerateDecl(iLinkageSpecDecl);
            break;
        }
        return true;
    }

    virtual bool VisitEnumDecl(EnumDecl *iEnumDecl)
    {
        // 定義でないなら非対象
        if (!iEnumDecl->isThisDeclarationADefinition())
    return true;

        // 名前無しは非対象
        if (!iEnumDecl->getIdentifier())
    return true;

        // theolizer名前空間内なら登録
        if (mIsTheolizerSpace)
    return true;

        mAstInterface.mSerializeListEnum.addCandidate(iEnumDecl);
        return true;
    }

//      ---<<< 以下は追跡しない >>>---

#if 0
    virtual bool VisitClassTemplateSpecializationDecl(
        ClassTemplateSpecializationDecl * iClassTemplateSpecializationDecl )
    {
ASTANALYZE_OUTPUT("VisitClassTemplateSpecializationDecl(",
                  iClassTemplateSpecializationDecl->getQualifiedNameAsString(), ")");
        return true;
    }
#endif

#if 0   // たぶん、追跡不要
    // その他のレコード
    virtual bool VisitTagDecl(TagDecl *iTagDecl)
    {
        getAnnotationInfo(iTagDecl, AnnotateType::None);

        enumerateDecl(iTagDecl);
        return true;
    }
#endif

#if 0   // 名前無し関数のボディ等。恐らく、関数内の{}ブロックと思われる。
    // Block    http://clang.llvm.org/doxygen/classclang_1_1BlockDecl.html#details
    virtual bool VisitBlockDecl (BlockDecl *iBlockDecl)
    {
        getAnnotationInfo(iBlockDecl, AnnotateType::None);
        enumerateDecl(iBlockDecl);
        return true;
    }
#endif

//----------------------------------------------------------------------------
//      アノテーション・エラー・チェック
//----------------------------------------------------------------------------

    virtual bool VisitDecl(Decl *iDecl)
    {
        if (iDecl->hasAttrs())
            getAnnotationInfo(iDecl, AnnotateType::None);
        return true;
    }

//----------------------------------------------------------------------------
//      Declリスト1レベルの枚挙処理
//----------------------------------------------------------------------------

public:
    void enumerateDecl(DeclContext *iDeclContext)
    {
        for (auto decl : iDeclContext->decls())
        {
            Visit(decl);
        }
    }

//----------------------------------------------------------------------------
//      基本
//----------------------------------------------------------------------------

//      ---<<< 出力 >>>---

public:
    ClassTemplateDecl*  mSwicher;               // Switcher<>のDecl
    ClassTemplateDecl*  mRegisterToBaseClass;   // RegisterToBaseClass<>のDecl
    bool                mHasSaveLoad;           // save/load有り
    bool                mHasGlobalVersionTable; // グローバル・バージョン番号テーブル有り

private:
    AstInterface&       mAstInterface;          // ソース修正処理とのI/F
    CXXRecordDecl*      mNonTemplateMarker;     // NonTemplateMarkerクラスのDecl

//      ---<<< 内部処理 >>>---

    bool    mIsStdSpace;                        // std名前空間処理中
    bool    mIsTheolizerSpace;                  // theolizer名前空間処理中

public:
    ASTVisitor(AstInterface& iAstInterface) :
        mSwicher(nullptr),
        mRegisterToBaseClass(nullptr),
        mHasSaveLoad(false),
        mHasGlobalVersionTable(false),
        mAstInterface(iAstInterface),
        mNonTemplateMarker(nullptr),
        mIsStdSpace(false),
        mIsTheolizerSpace(false)
    { }
};

// ***************************************************************************
//      プリプロセッサからのコールバック処理
//          AST解析を実行するかどうか判定する
// ***************************************************************************

class TheolizerASTConsumer : public ASTConsumer
{
private:

//----------------------------------------------------------------------------
//      解析実行判定
//----------------------------------------------------------------------------

    // AST解析結果の受取(トップレベル毎に呼ばれる)
    virtual bool HandleTopLevelDecl (DeclGroupRef iDeclGroupRef)
    {
        // 致命的エラーが発生していたら解析しない
        if (gCustomDiag.getDiags().hasFatalErrorOccurred())
        {
            DOANALYZE_OUTPUT("HandleTopLevelDecl() aborted parsing by Fatal Error.");
            mAstInterface.mNotParse=true;
        }

        return !mAstInterface.mNotParse;
    }

//----------------------------------------------------------------------------
//      std::string, std::wstring, std::u16string, std::u32stringのどれかか?
//----------------------------------------------------------------------------

    bool isString(QualType iQualType, Decl const* iErrorPos)
    {
        if (iQualType->getTypeClass() != Type::Record)
    return false;

        // 名前確認
        CXXRecordDecl const* aInstanciatedClass = iQualType->getAsCXXRecordDecl();
        ERROR(!aInstanciatedClass, iErrorPos, false);

        // gccの場合、"std::__cxx11::basic_string"となっていた。
        //  名前空間でバージョンを切り替えている。
        if (aInstanciatedClass->getQualifiedNameAsString().find("std::") != 0)
    return false;
        if (aInstanciatedClass->getName() != "basic_string")
    return false;

        // テンプレート情報取り出し
        ClassTemplateSpecializationDecl const* ctsd=
            dyn_cast<ClassTemplateSpecializationDecl>(aInstanciatedClass);
        if (!ctsd)
    return false;

        // 先頭のテンプレート・パラメータ確認
        TemplateArgumentList const& tal=ctsd->getTemplateInstantiationArgs();
        if (tal.size() < 1)
    return false;

        TemplateArgument const& ta = tal.get(0);
        QualType qt = ta.getAsType();
        Type const* type = qt.getTypePtr();
        clang::BuiltinType const* bt=dyn_cast<clang::BuiltinType>(type);
        if (!bt)
    return false;

        switch(bt->getKind())
        {
        case clang::BuiltinType::Char_U:
        case clang::BuiltinType::WChar_U:
        case clang::BuiltinType::Char_S:
        case clang::BuiltinType::WChar_S:
        case clang::BuiltinType::Char16:
        case clang::BuiltinType::Char32:
    return true;
        default:
            break;
        }
        return false;
    }

//----------------------------------------------------------------------------
//      save/loadする要素のQualTypeを処理し、
//          PointerとLValueReferenceの展開
//          Enumの登録
//          Recordの登録と枚挙
//----------------------------------------------------------------------------

    void processElement(QualType iQualType, Decl const* iErrorPos, bool iIsRegisterdClass=false)
    {
        QualType qt=iQualType.getDesugaredType(*gASTContext);
        // TheolizerNonKeepStep or TheolizerParameterなら、Type取り出し
        do
        {
            CXXRecordDecl* crd=qt->getAsCXXRecordDecl();
            if (!crd)
        break;

            ClassTemplateSpecializationDecl const* ctsd
                =dyn_cast<ClassTemplateSpecializationDecl >(crd);
            if (!ctsd)
        break;
            if ((crd->getName().startswith("TheolizerNonKeepStep") == false)
             && (crd->getName().startswith("TheolizerParameter") == false))
        break;

            TemplateArgumentList const& tal=ctsd->getTemplateArgs();
            if (tal.size() < 1)
        break;

            TemplateArgument const& ta = tal.get(0);
            qt = ta.getAsType();
        }
        while(0);

        ASTANALYZE_OUTPUT("    processElement() ", qt.getAsString());
        ASTANALYZE_OUTPUT("      ", qt->getTypeClass(),
                          " : ", Type::Builtin,
                          ", ", Type::ConstantArray,
                          ", ", Type::Record,
                          ", ", Type::Enum,
                          ", ", Type::TemplateTypeParm,
                          ", ", Type::TemplateSpecialization,
                          ", ", Type::InjectedClassName,
                          ", ", Type::DependentName);

        switch (qt->getTypeClass())
        {
        case Type::Builtin:
        case Type::TemplateTypeParm:
        case Type::TemplateSpecialization:
        case Type::DependentTemplateSpecialization:
    return ;

        case Type::ConstantArray:
        {
            clang::ArrayType const* at=dyn_cast<clang::ArrayType>(qt.getTypePtr());
            QualType    at_qt=at->getElementType();
            processElement(at_qt, iErrorPos);
        }
    return ;

        case Type::Pointer:
            {
                // 参照を外して、再処理する
                clang::PointerType  const* pt = qt->getAs<clang::PointerType >();
                processElement(pt->getPointeeType(), iErrorPos);
                return;
            }

        case Type::LValueReference:
            {
                // 参照を外して、再処理する
                clang::LValueReferenceType const* lvrt = qt->getAs<clang::LValueReferenceType>();
                processElement(lvrt->getPointeeType(), iErrorPos);
                return;
            }

        // 右辺値参照はNOP(非対応)
        case Type::RValueReference:
    return;

        case Type::Enum:
            {
                EnumType const* et = qt->getAs<EnumType>();
                ERROR(!et, iErrorPos);

                EnumDecl const* aTargetEnum = et->getDecl();
                ERROR(!aTargetEnum, iErrorPos);
                mAstInterface.mSerializeListEnum.addSaveLoad(aTargetEnum);
            }
            break;

        // 通常クラスと全てのテンプレート
        case Type::Record:
            {
                // std::string等なら、プリミティブとして処理する。
                if (isString(qt, iErrorPos))
    return;

                // TheolizerBaseは処理しない、TheolizerVersionはターゲット取り出し
                CXXRecordDecl const* aInstanciatedClass = qt->getAsCXXRecordDecl();
                CXXRecordDecl const* aTargetClass=nullptr;
                if (qt.getBaseTypeIdentifier())
                {
                    if (qt.getBaseTypeIdentifier()->getName().equals("TheolizerBase"))
    return;
                    if (qt.getBaseTypeIdentifier()->getName().equals("TheolizerVersion"))
                    {
                        ASTANALYZE_OUTPUT("++++++++++++++++++processElement(TheolizerVersion)");
                        for (auto&& aDecl : aInstanciatedClass->decls())
                        {
                            if (aDecl->getKind() != Decl::Typedef)
                        continue;

                            TypedefDecl* td = cast<TypedefDecl>(aDecl);
                            if (td->getName().equals("TheolizerTarget"))
                            {
                                QualType qt2=td->getUnderlyingType().
                                                getDesugaredType(*gASTContext);

                                // std::string等なら、プリミティブとして処理する。
                                if (isString(qt2, iErrorPos))
    return;
                                aTargetClass=qt2->getAsCXXRecordDecl();
                                if (aTargetClass)
                        break;

                                // Enum型処理
                                EnumType const* et = qt2->getAs<EnumType>();
                                if (!et)
    return;
                                EnumDecl const* aTargetEnum = et->getDecl();
                                if (!aTargetEnum)
    return;

                                ASTANALYZE_OUTPUT("          aTargetEnum=",
                                                  aTargetEnum->getQualifiedNameAsString(),
                                                  " ", aTargetEnum);
                                mAstInterface.mSerializeListEnum.addSaveLoad(aTargetEnum);
    return;
                            }
                        }
                        ASTANALYZE_OUTPUT("------------------processElement(TheolizerVersion)"
                                          " aTargetClass=", aTargetClass);
                        if (!aTargetClass)
    return;
                    }
                }
                if (!aTargetClass)
                {
                    ERROR(!aInstanciatedClass, iErrorPos);

                    aTargetClass=aInstanciatedClass->getTemplateInstantiationPattern();
                    if (!aTargetClass)
                    {
                        aTargetClass=aInstanciatedClass;
                    }
                }
                ASTANALYZE_OUTPUT("          aTargetClass=",
                                  aTargetClass->getQualifiedNameAsString(),
                                  " ", aTargetClass);

                if (mAstInterface.mSerializeListClass.addSaveLoad(aTargetClass, iIsRegisterdClass))
                {
                    // 初めてのsave/loadなら、枚挙する
                    enumerateClass(aTargetClass);
                }
            }
            break;

        // コンストラクトの記述ミス時ここに来る
        //  デフォルト・コンストラクタを呼ぶつもりで()を付けると、関数として解釈されるため
        case Type::FunctionProto:
            ASTANALYZE_OUTPUT("Theolizer unkown pattern.(Type::FunctionProto)");
            gCustomDiag.ErrorReport(iErrorPos->getLocation(),
                "Illigal format. Please construct %0 by {} instead of ().(%1)")
                << qt.getAsString()
                << __LINE__;
    return;

        // 不明ケースも警告
        default:
            ASTANALYZE_OUTPUT("Theolizer unkown pattern.", qt->getTypeClass());
            WARNING(true, iErrorPos);
        }
    }

//----------------------------------------------------------------------------
//      保存／回復しているクラスに含まれるclassとenum型を枚挙し、登録する
//----------------------------------------------------------------------------

    void enumerateClass
    (
        CXXRecordDecl const* iClass,
        bool iIsVersion=false,
        AnnotationInfo const* iAnnotationInfo=nullptr
    )
    {
        ASTANALYZE_OUTPUT("++++++++++++ enumerateClass(", iClass->getQualifiedNameAsString(), ")");

//      ---<<< 基底クラス処理 >>>---

        for (auto base : iClass->bases())
        {
            const Type *type = base.getType().split().Ty;
            CXXRecordDecl* aBaseDecl = type->getAsCXXRecordDecl();
            if (aBaseDecl)
            {
                // TheolizerBaseは登録しない
                if ((aBaseDecl->getName().equals("TheolizerBase")))
            continue;

                ASTANALYZE_OUTPUT("          base : ", aBaseDecl->getQualifiedNameAsString());
                // 基底クラスを登録する
                if (mAstInterface.mSerializeListClass.addSaveLoad(aBaseDecl))
                {
                    // 初めてのsave/loadなら、更にクラス内のクラスとenum型を枚挙し、登録する
                    enumerateClass(aBaseDecl, iIsVersion);
                }
            }
        }

//      ---<<< フィールド処理 >>>---

        // 手動型ならメンバは処理しない
        auto found=mAstInterface.mSerializeListClass.find(iClass);
        if ((found) && (found->second.mIsManual))
    return;

        for (auto field : iClass->fields())
        {
            if (!field->getIdentifier())
        continue;

            // getCanonicalType()を取ることで、std::string→basic_stringへ展開される。
            ASTANALYZE_OUTPUT("          field : ",
                              field->getType().getCanonicalType().getAsString(),
                              " ", field->getName());

            // privateは処理しない
            if (field->getAccess() == clang::AS_private)
        continue;

            // TheolizerVersion<>のmTheolizerSpecialsを除く
            if (iIsVersion)
            {
                if (field->getName() == "mTheolizerSpecials")
        continue;
            }

            // アノテーション確認
            AnnotationInfo annotation = getAnnotationInfo(field, AnnotateType::Field);
            switch(annotation.mAnnotate)
            {
            case AnnotationInfo::None:  // 指定無しなら、登録する
                {
                    AnnotationInfo::Annotate    aAnnotate=AnnotationInfo::None;
                    if (iAnnotationInfo)
                    {
                        aAnnotate=iAnnotationInfo->mAnnotate;
                    }
                    else if (found)
                    {
                        aAnnotate=found->second.mAnnotationInfo.mAnnotate;
                    }
                    // デフォルト非保存なら登録しない
                    if (aAnnotate == AnnotationInfo::CN)
        continue;
                }
                break;

            case AnnotationInfo::FS:    // 保存指定有りなら、登録する
                break;

            default:                    // その他は、登録しない
        continue;
            }

            processElement(field->getType(), iClass);
        }
        ASTANALYZE_OUTPUT("------------ enumerateClass(", iClass->getQualifiedNameAsString(), ")");
    }

//----------------------------------------------------------------------------
//      シリアライズ・クラスの完全自動型以外の基底クラスをsave/load対象とする
//          保存／回復していない完全自動型をコード生成対象とするため
//          下記が対象である
//              侵入型半自動(基底クラスとメンバ変数)
//----------------------------------------------------------------------------

    void enumerateNonFullAuto()
    {
        ASTANALYZE_OUTPUT("++++++++++++ enumerateNonFullAuto()");
        for (auto&& aSerializeInfo : mAstInterface.mSerializeListClass.getList())
        {
            if ((!aSerializeInfo.second.mIsFullAuto)
             && (!aSerializeInfo.second.mIsManual))
            {
                // ターゲット・クラスの枚挙
                enumerateClass
                (
                    aSerializeInfo.second.mTheolizerTarget,
                    false,
                    &(aSerializeInfo.second.mAnnotationInfo)
                );

                // TheolizerVersion<>の枚挙
                for (auto&& aTheolizerVersion : aSerializeInfo.second.mTheolizerVersionList)
                {
                    enumerateClass(aTheolizerVersion, true);
                    ASTANALYZE_OUTPUT("    enumerateClass(",
                                      aTheolizerVersion->getQualifiedNameAsString(), ")");
                }
            }
        }
        ASTANALYZE_OUTPUT("------------ enumerateNonFullAuto()");
    }

//----------------------------------------------------------------------------
//      RegisterToBaseClass<>を実体化しているクラス抽出する
//----------------------------------------------------------------------------

    void processRegisterToBaseClass(ClassTemplateDecl* iRegisterToBaseClass)
    {
        ASTANALYZE_OUTPUT("============ RegisterToBaseClass ===========================");

        // 特殊化を枚挙する
        for (auto spec : iRegisterToBaseClass->specializations())
        {
            TemplateArgumentList const& tal=spec->getTemplateArgs();
            if (tal.size() != 1)
        continue;

            // テンプレート・パラメータ取り出し
            TemplateArgument const& ta = tal.get(0);
            QualType qt = ta.getAsType();
            ASTANALYZE_OUTPUT("$$$$ RegisterToBaseClass() : ", qt.getAsString());

            processElement(qt, iRegisterToBaseClass, true);
        }
    }

//----------------------------------------------------------------------------
//      Swicher<>::save/loadを追跡し、保存／回復しているクラスとenum型を抽出する
//----------------------------------------------------------------------------

    void processSwitcher(ClassTemplateDecl* iSwitcher)
    {
        ASTANALYZE_OUTPUT("============ processSwitcher ===========================");

        // 特殊化を枚挙する
        for (auto spec : iSwitcher->specializations())
        {
            TemplateArgumentList const& tal=spec->getTemplateArgs();
            if (tal.size() != 5)
        continue;

            // テンプレート・パラメータ取り出し
            TemplateArgument const& ta = tal.get(1);
            QualType qt = ta.getAsType();
            ASTANALYZE_OUTPUT("$$$$ processSwitcher() : ", qt.getAsString());

            processElement(qt, iSwitcher);
        }
    }

//----------------------------------------------------------------------------
//      AST解析とソース修正処理呼び出し
//----------------------------------------------------------------------------

    virtual void HandleTranslationUnit(ASTContext &iContext)
    {
        TIME_OUTPUT("HandleTranslationUnit() time=,", gProcessingTime.GetmSec(), ", mSec");
        DOANALYZE_OUTPUT("TheolizerDriver is processing ", gProcessingFile);

//          ---<<< 情報収集 >>>---

        // 1Pass目(シリアライズ・クラス収集)
        mASTVisitor.enumerateDecl(iContext.getTranslationUnitDecl());

        // 2Pass目(他のクラスから使われている完全自動型を生成対象とするため)
        enumerateNonFullAuto();

        // 3Pass目(THEOLIZER_REGISTER_CLASSで登録された派生クラスをsave/load処理する)
        if (mASTVisitor.mRegisterToBaseClass)
        {
            processRegisterToBaseClass(mASTVisitor.mRegisterToBaseClass);
        }

        // 4Pass目(save/load処理)
        if (mASTVisitor.mSwicher)
        {
            processSwitcher(mASTVisitor.mSwicher);
        }

//          ---<<< ソース修正 >>>---

        // save/loadがなく、かつ、デフォルトの自動生成ソース指定もないなら
        // ソース修正無し。(非デフォルトへの修正が発生しないが許容する。)

        if (mASTVisitor.mSwicher || !mAstInterface.mLocationDefault.isInvalid())
        {
            ModifySource aModifySource(mPreprocessor, mAstInterface);
            aModifySource.process();

            TIME_OUTPUT("Collect AST and Modify sources time=,",
                        gProcessingTime.GetmSec(), ", mSec");
        }
        else
        {
            TIME_OUTPUT("Collect AST but no-Modify sources time=,",
                        gProcessingTime.GetmSec(), ", mSec");
        }
    }

//----------------------------------------------------------------------------
//      生成と削除
//----------------------------------------------------------------------------

private:
    Preprocessor&       mPreprocessor;      // プリプロセッサ
    ASTVisitor          mASTVisitor;        // Visitor処理用クラス
    AstInterface        mAstInterface;      // AST解析処理とのI/F
    bool                mMadeDefaultSource; // デフォルトの自動生成ソースを作成した

public:
    explicit TheolizerASTConsumer(CompilerInstance *iCompilerInstance) :
                    mPreprocessor(iCompilerInstance->getPreprocessor()),
                    mASTVisitor(mAstInterface),
                    mMadeDefaultSource(false)
    {
        TIME_OUTPUT("TheolizerASTConsumer(start) time=,", gProcessingTime.GetmSec(false),
                    ", mSec");

        // グローバル変数群の初期化
        gCompilerInstance = iCompilerInstance;
        gASTContext = &(gCompilerInstance->getASTContext());
        gSourceManager = &(gASTContext->getSourceManager());
        gCustomDiag.setDiagnosticsEngine(&(gCompilerInstance->getDiagnostics()));

        // プリプロセッサからのコールバック登録
        mPreprocessor.addPPCallbacks(std::unique_ptr<PPCallbacksTracker>(
            new PPCallbacksTracker(mAstInterface)));

        // デフォルトの自動生成ソース・ファイルがなければ作る
        if (llvmS::fs::access(gDefaultSourceName.string(), llvmS::fs::AccessMode::Exist))
        {
            std::error_code error;
            llvm::raw_fd_ostream temp(gDefaultSourceName.string(), error, llvmS::fs::F_None);
            ASTANALYZE_OUTPUT("hpp generation : make source:"
                              " source=", gDefaultSourceName.string());
            if (error)
            {
                gCustomDiag.FatalReport(SourceLocation(),
                    "Can not create file(%0).") << gDefaultSourceName.string();
            }
            else
            {
                mMadeDefaultSource=true;
            }
        }
        // パスを正規化する
        gDefaultSourceName=boostF::canonical(gDefaultSourceName);
    }
    ~TheolizerASTConsumer()
    {
        // デフォルトの自動生成ソースを作成し、
        if (mMadeDefaultSource)
        {
            // 保存されなければ削除する
            if (!mAstInterface.mDoSaveTheolizerHpp)
            {
                ASTANALYZE_OUTPUT("hpp generation : remove(", gDefaultSourceName.string(), ")");
                                llvmS::fs::remove(gDefaultSourceName.string());
            }
        }

        gCustomDiag.resetDiagnosticsEngine();
        DRIVER_OUTPUT("~TheolizerASTConsumer()");
    }
};

//----------------------------------------------------------------------------
//      メイン処理からの中継
//----------------------------------------------------------------------------

class TheolizerFrontendAction : public SyntaxOnlyAction /*ASTFrontendAction*/
{
public:
    virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(
                        CompilerInstance &iCompilerInstance, StringRef file)
    {
        return llvm::make_unique<TheolizerASTConsumer>(&iCompilerInstance); 
    }
};

// ***************************************************************************
//      AST解析用パラメータのスキップ解析
// ***************************************************************************

struct SkipList
{
  const char *name;
  const int  skip_num;
};

int check_skip(char const* iArg)
{
    static const SkipList skip_list[] =
    {
        // unsupported
        {"--dependent-lib", 1},

        // unknowm
        {"-cc1", 1},
        {"-triple", 2},
        {"-main-file-name", 2},
        {"-mrelocation-model", 2},
        {"-mdisable-fp-elim", 1},
        {"-pic-level", 2},
        {"-relaxed-aliasing", 1},
        {"-masm-verbose", 1},
        {"-mconstructor-aliases", 1},
        {"-munwind-tables", 1},
        {"-target-cpu", 2},
        {"-fdiagnostics-format", 2},
        {"-fdeprecated-macro", 1},
        {"-fdebug-compilation-dir", 2},
//      {"-ferror-limit", 2},
        {"-fmessage-length", 2},
        {"-coverage-file", 2},  // Visual Studioからビルドする時、BuildCompilationで生成された。
        {"-vectorize-loops", 1},// Visual Studioでコンパイル・エラーになってしまうので取り除く。
        {"-vectorize-slp", 1},
        {"-fno-rtti-data", 1},

        // unused
        {"-emit-obj", 1},           // a.oファイルを出力する
        {"-mrelax-all", 1},         // 内部アセンブラ用オプション
        {"-disable-free", 1},       // 終了時、メモリ開放しない
        {"-fms-volatile", 1},       // 意味が分からない
        {"-dwarf-column-info", 1},  // DWARFなので恐らくリンカー・オプション
//      {"-internal-isystem", 2},   // システム・インクルード指定だが使ってない模様
    };

    for (std::size_t i = 0; i < llvm::array_lengthof(skip_list); ++i)
    {
        if (StringRef(iArg).startswith(skip_list[i].name))
return skip_list[i].skip_num;
    }

    return 0;
}

// ***************************************************************************
//      AST解析メイン処理
// ***************************************************************************

static llvm::cl::OptionCategory MyToolCategory("Theolizer Driver");
int parse
(
    std::string const&              iExecPath,
    llvm::opt::ArgStringList const& iArgs,
    std::string const&              iTarget
)
{
    gProcessingTime.GetmSec();  // 計測開始

//----------------------------------------------------------------------------
//      CommonOptionsParserへ渡すためのパラメータ生成
//----------------------------------------------------------------------------

    llvm::opt::ArgStringList aArgStringList;

    aArgStringList.push_back(iExecPath.c_str());    // argv[0]=exe

    auto aFileArg=iArgs.end()-1;
    gProcessingFile = StringRef(*aFileArg);
    aArgStringList.push_back(*aFileArg);            // argv[1]=ファイル名
    aArgStringList.push_back("--");

    // デフォルトの自動生成先ファイル名
    gCompilingSourceName=std::string(*aFileArg);
    gDefaultSourceName=boostF::absolute(gCompilingSourceName+".theolizer.hpp");
    DRIVER_OUTPUT("gDefaultSourceName=", gDefaultSourceName.string());

    // CommonOptionsParser::getSourcePathList()は、
    // 内部にstaticにソース・リストを保持しており、
    // コンストラクトされる度にソース・リストへ追加される。
    // 対策のため、別途ソース・リストを用意する。
    std::vector<std::string>  aSourceList;
    aSourceList.push_back(*aFileArg);

    bool        x_flag = false;
    int         skip_num = 0;                       // 解析スキップするargvの数
    std::string error_limit;                        // -error-limit記憶領域(最後のみ有効)
    std::string message_length;                     // -message-length記憶領域(最後のみ有効)
    for (auto arg = iArgs.begin(); arg != iArgs.end(); ++arg)
    {
        // ファイル名はスキップする。
        if (arg == aFileArg)
    continue;

        // -xフラグ確認(cppのみ処理する。プリプロセス済のものは処理しない。)
        if (x_flag)
        {
            x_flag = false;

            if ((!StringRef(*arg).equals("c++"))
             && (!StringRef(*arg).equals("c++-header")))
return 0;
        }

        if (StringRef(*arg).equals("-x"))
            x_flag=true;

        // '-ferror-limit'のフォーマットをCommonOptionsParser用に戻す
        if (StringRef(*arg).equals("-ferror-limit"))
        {
            // ユーザ・クラスのバージョンアップ時に多数のエラーが出るのでエラー停止無し
            error_limit = "-ferror-limit=0";
//          error_limit = error_limit + *(arg+1);
            aArgStringList.push_back(error_limit.c_str());
            skip_num=2;
        }

        // '-fmessage-length'のフォーマットをCommonOptionsParser用に戻す
        if (StringRef(*arg).equals("-fmessage-length"))
        {
            message_length = "-fmessage-length=";
            message_length = message_length + *(arg+1);
            aArgStringList.push_back(message_length.c_str());
            skip_num=2;
        }

        // これがないと、char16_t, char32_tについて未定義エラーとなる(llvm 3.7.0+MSVC 2015)
        if (StringRef(*arg).equals("-fms-compatibility-version=18"))
        {
            aArgStringList.push_back("-fms-compatibility-version=19");
            skip_num=1;
        }

        // 解析スキップ処理しつつ、パラメータ登録
        if (!skip_num)
        {
            skip_num = check_skip(*arg);
        }

        if (skip_num)
        {
            --skip_num;
        }
        else
        {
            aArgStringList.push_back(*arg);
        }
    }

    // ドライバー動作中マクロ定義
    aArgStringList.push_back("-D");
    aArgStringList.push_back("THEOLIZER_INTERNAL_DRIVER");

    // これがないと、tryとthrowで例外対応していないエラーが出る(llvm 3.7.0+MSVC 2015)
    aArgStringList.push_back("-fexceptions");

    // ターゲットをあわせる
    if (iTarget.size())
    {
        aArgStringList.push_back(iTarget.c_str());
    }

//----------------------------------------------------------------------------
//      AST解析実行
//----------------------------------------------------------------------------

    // パラメータ表示
    PARAMETER_OUTPUT("ClangTool::run()");

    int argc = static_cast<int>(aArgStringList.size());
    char const** argv = aArgStringList.begin();
    for (int j = 0; j < argc; ++j)
    {
        PARAMETER_OUTPUT("    argv[", j, "] :  ", argv[j]);
    }

#if defined(_WIN32) // TMPファイル削除準備
    gTempFile.clear();
#endif

    // パラメータ解析と実行
    clang::tooling::CommonOptionsParser op(argc, argv, MyToolCategory);
    clang::tooling::ClangTool Tool(op.getCompilations(), aSourceList);
    IntrusiveRefCntPtr<DiagnosticOptions>   diag_opts = new DiagnosticOptions;
    TheolizerDiagnosticConsumer    DiagClient(llvm::errs(), &*diag_opts);
    Tool.setDiagnosticConsumer(&DiagClient);
    int ret=Tool.run(clang::tooling::newFrontendActionFactory<TheolizerFrontendAction>().get());

#if defined(_WIN32) // TMPファイル削除
    if (!gTempFile.empty())
    {
        std::string aDirectory=llvmS::path::parent_path(gTempFile).str();
        std::string aFileName =llvmS::path::filename(gTempFile).str();
        std::string aRegex=aFileName+"~RF.*\\.TMP";
        ASTANALYZE_OUTPUT("aDirectory=", aDirectory);
        ASTANALYZE_OUTPUT("aFileName =", aFileName);
        ASTANALYZE_OUTPUT("aRegex    =", aRegex);
        auto aFileList=theolizer::getFileList(aDirectory+"/", aRegex);
        for (auto&& aFile : aFileList)
        {
            std::string aDelFile=aDirectory+"/"+aFile;
            ASTANALYZE_OUTPUT("    remove(", aDelFile, ")");
            boostF::remove(aDelFile);
        }
    }
#endif

    return ret;
}

#endif
