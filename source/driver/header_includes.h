//############################################################################
//      ヘッダのインクルード
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

#if !defined(THEOLIZER_HEADER_INCLUDES_H)
#define THEOLIZER_HEADER_INCLUDES_H

#define THEOLIZER_INTERNAL_DRIVER

// ***************************************************************************
//      インクルード
// ***************************************************************************

#ifdef _MSC_VER     // start of disabling MSVC warnings
    #pragma warning(push)
    #pragma warning(disable:4100 4141 4146 4180 4244 4245 4258 4267 4291 4310 4324 4345 4351 4355 4389 4456 4457 4458 4459 4503 4624 4702 4722 4800 4913 4996)
#endif

#include <theolizer/internal/avoid-trouble.h>

THEOLIZER_INTERNAL_DISABLE_WARNING()
#if defined(__GNUC__)
    #pragma GCC diagnostic push
    THEOLIZER_INTERNAL_PRAGMA(GCC diagnostic ignored "-Wstrict-aliasing")
    THEOLIZER_INTERNAL_PRAGMA(GCC diagnostic ignored "-Wparentheses")
#endif

//----------------------------------------------------------------------------
//      standard library
//----------------------------------------------------------------------------

#include <sstream>
#include <iomanip>
#include <fstream>
#include <sys/stat.h>
#include <bitset>
#include <stack>
#include <chrono>
#include <thread>
#include <map>
#include <forward_list>
#include <limits>

//----------------------------------------------------------------------------
//      for Driver
//----------------------------------------------------------------------------

#include <clang/Driver/Compilation.h>
#include <clang/Driver/Driver.h>
#include <clang/Driver/Options.h>
#include <clang/Parse/ParseDiagnostic.h>
#include <clang/Frontend/ChainedDiagnosticConsumer.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Frontend/SerializedDiagnosticPrinter.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Option/ArgList.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/Process.h>
#include <llvm/Support/Program.h>
#include <llvm/Support/StringSaver.h>
#include <llvm/Support/raw_ostream.h>

#define BOOST_ALL_NO_LIB
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/upgradable_lock.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include <boost/interprocess/detail/os_thread_functions.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/filesystem.hpp>

//----------------------------------------------------------------------------
//      for AST Parse
//----------------------------------------------------------------------------

#include <clang/AST/AST.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/DeclVisitor.h>
#include <clang/Frontend/ASTConsumers.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Lex/MacroArgs.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

//----------------------------------------------------------------------------
//      for Source modify
//----------------------------------------------------------------------------

#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Rewrite/Frontend/Rewriters.h>
#include <clang/Lex/Lexer.h>        // マクロ追跡用

#ifdef _MSC_VER         // end of disabling MSVC warnings
    #pragma warning(pop)
#endif
#if defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif

//----------------------------------------------------------------------------
//      theolizer
//----------------------------------------------------------------------------

#include <theolizer/internal/base.h>

#if defined(_WIN32)
#include <theolizer/test_tool.h>    // getFileList()
#endif

//----------------------------------------------------------------------------
//      頻繁に使う名称を短縮する
//          当該名称の調査を容易にするため、namespace指令を使わないこと。
//----------------------------------------------------------------------------

using clang::DiagnosticsEngine;
using clang::DiagnosticBuilder;
using clang::DiagnosticIDs;
using clang::DiagnosticConsumer;
using clang::SourceLocation;
using clang::FullSourceLoc;
using clang::FileID;
using clang::FileEntry;
using clang::IntrusiveRefCntPtr;
using clang::SyntaxOnlyAction;
using clang::DiagnosticOptions;

using clang::CompilerInstance;
using clang::ASTContext;
using clang::SourceManager;
using clang::Preprocessor;
using clang::PPCallbacks;
using clang::ASTConsumer;
using clang::DeclVisitor;
using clang::Rewriter;

using clang::NamespaceDecl;
using clang::LinkageSpecDecl;
using clang::DeclContext;
using clang::DeclGroupRef;

using clang::Token;
using clang::MacroDirective;
using clang::MacroDefinition;
using clang::MacroInfo;
using clang::IdentifierTable;
using clang::IdentifierInfo;

using clang::EnumDecl;
using clang::CXXRecordDecl;
using clang::TemplateDecl;
using clang::ClassTemplateDecl;
using clang::ClassTemplateSpecializationDecl;
using clang::FunctionTemplateDecl;

using clang::Decl;
using clang::NamedDecl;
using clang::TagDecl;
using clang::RecordDecl;
using clang::TypedefDecl;
using clang::VarDecl;
using clang::FriendDecl;
using clang::EnumConstantDecl;
using clang::QualType;
using clang::Type;
using clang::RecordType;
using clang::EnumType;
using clang::TemplateTypeParmType;
using clang::TemplateSpecializationType;
using clang::InjectedClassNameType;
using clang::APValue;
using clang::TemplateName;
using clang::AnnotateAttr;
using clang::AttrVec;
using clang::FunctionTemplateSpecializationInfo;
using clang::TemplateArgument;
using clang::TemplateArgumentList;
using clang::Lexer;

using clang::SmallString;
using clang::SmallVector;
using clang::cast;
using clang::dyn_cast;

namespace llvmS=llvm::sys;
namespace clangD=clang::driver;
namespace boostI=boost::interprocess;
namespace boostF=boost::filesystem;

using theolizer::internal::FineTimer;

#endif
