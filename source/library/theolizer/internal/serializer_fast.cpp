//############################################################################
//      Fastシリアライザ
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

#define THEOLIZER_INTERNAL_EXCLUDE_VERSION_H

//############################################################################
//      インクルード
//############################################################################

#include "avoid-trouble.h"

#include <limits>
#include <sstream>

#include "internal.h"
#include "serializer_fast.h"

//############################################################################
//      Begin
//############################################################################

namespace theolizer
{
namespace internal
{

// ***************************************************************************
//      グローバル・バージョン番号管理
// ***************************************************************************

//----------------------------------------------------------------------------
//      ユーザ定義のグローバル・バージョン番号テーブルへのポインタ定義
//          Fastシリアライザは最新版のみ対応しているため、GVNT不要である。
//          なので、デフォルトのテーブルをここで実体化する。
//----------------------------------------------------------------------------

namespace{
    GlobalVersionNoTableBase const*const sGlobalVersionNoTable=
        GlobalVersionNoTable<1>::getInstance();;
}   // namespace

//############################################################################
//      保存
//############################################################################

// ***************************************************************************
//      シリアライザ名
// ***************************************************************************

char const* const   FastMidOSerializer::kSerializerName=
        "theolizer::internal::FastMidOSerializer";

// ***************************************************************************
//      コンストラクタ
// ***************************************************************************

FastMidOSerializer::FastMidOSerializer
(
    std::ostream& iOStream,
    Destinations const& iDestinations,
    GlobalVersionNoTableBase const*const iGlobalVersionNoTable,
    unsigned iLastGlobalVersionNo,
    bool mNoThrowException
) : BaseSerializer
    (
        iDestinations,
        iGlobalVersionNoTable,
        iLastGlobalVersionNo,
        iLastGlobalVersionNo,
        CheckMode::InMemory,
        true,
        nullptr,
        mNoThrowException
    ),
    mOStream(iOStream)
{
    // エラー情報登録準備
    theolizer::internal::ApiBoundary aApiBoundary(&mAdditionalInfo, true);

    // 型情報取得中継クラス登録
    TypeFunctions<FastMidOSerializer>   aTypeFunctions;

    // 派生Serializerのバージョン番号獲得
    mSerializerVersionNo=kLastVersionNo;

    if (getNoThrowException())
    {
        try
        {
            // ヘッダ処理
            writeHeader();
        }
        catch (ErrorInfo&)
        {
        }
    }
    else
    {
        // ヘッダ処理
        writeHeader();
    }
}

// ***************************************************************************
//      型情報保存
// ***************************************************************************

//      ---<<< ヘッダ保存 >>>---

void FastMidOSerializer::writeHeader()
{
    // バージョン番号対応表生成
    createVersionNoTable();
}

// ***************************************************************************
//      内部処理
// ***************************************************************************

//----------------------------------------------------------------------------
//      プリミティブ処理
//----------------------------------------------------------------------------

#define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSimbol)                    \
    void FastMidOSerializer::savePrimitive(dType const& iPrimitive)         \
    {                                                                       \
        mOStream.write(reinterpret_cast<char const*>(&iPrimitive), sizeof(iPrimitive));\
        if (!mOStream.good()) {                                             \
            THEOLIZER_INTERNAL_IO_ERROR(u8"I/O Error.");                    \
        }                                                                   \
    }

//      ---<<< 文字列型 >>>---

#define THEOLIZER_INTERNAL_DEF_STRING(dType, dSimbol)                       \
    void FastMidOSerializer::savePrimitive(dType const& iPrimitive)         \
    {                                                                       \
        unsigned aDataSize=sizeof(dType::value_type);                       \
        std::size_t size=iPrimitive.size()*aDataSize;                       \
        savePrimitive(size);                                                \
        mOStream.write(reinterpret_cast<char const*>(&iPrimitive[0]), size);\
    }

//      ---<<< 実体定義 >>>---

#include "primitive.inc"

// ***************************************************************************
//      ユーティリティ
// ***************************************************************************

//----------------------------------------------------------------------------
//      文字列保存
//----------------------------------------------------------------------------

void FastMidOSerializer::saveString(std::string const& iString)
{
    savePrimitive(iString.size());
    mOStream.write(iString.c_str(), iString.size());
//std::cout << "### saveString(" << iString.size() << ":" << iString << ");\n";
    if (!mOStream.good()) {
        THEOLIZER_INTERNAL_IO_ERROR(u8"保存時IOエラー");
    }
}

//############################################################################
//      回復
//############################################################################

// ***************************************************************************
//      シリアライザ名
// ***************************************************************************

char const* const   FastMidISerializer::kSerializerName=
        "theolizer::internal::FastMidISerializer";

// ***************************************************************************
//      コンストラクタ
// ***************************************************************************

FastMidISerializer::FastMidISerializer
(
    std::istream& iIStream,
    Destinations const& iDestinations,
    GlobalVersionNoTableBase const*const iGlobalVersionNoTable,
    unsigned iLastGlobalVersionNo,
    bool mNoThrowException
) : BaseSerializer
    (
        iDestinations,
        iGlobalVersionNoTable,
        iLastGlobalVersionNo,
        iLastGlobalVersionNo,
        CheckMode::InMemory,
        false,
        nullptr,
        mNoThrowException
    ),
    mIStream(iIStream)
{
    // エラー情報登録準備
    theolizer::internal::ApiBoundary aApiBoundary(&mAdditionalInfo);

    // 型情報取得中継クラス登録
    TypeFunctions<FastMidISerializer>   aTypeFunctions;

    // ヘッダ処理
    if (getNoThrowException())
    {
        try
        {
            // ヘッダ処理
            readHeader();
        }
        catch (ErrorInfo&)
        {
        }
    }
    else
    {
        // ヘッダ処理
        readHeader();
    }
}

// ***************************************************************************
//      ヘッダ回復と型チェック
// ***************************************************************************

//----------------------------------------------------------------------------
//      ヘッダ情報回復
//----------------------------------------------------------------------------

void FastMidISerializer::readHeader()
{
    // バージョン番号対応表生成
    createVersionNoTable();
}

//----------------------------------------------------------------------------
//      TypeIndex一致判定
//----------------------------------------------------------------------------

bool FastMidISerializer::isMatchTypeIndex(size_t iSerializedTypeIndex,
                                          size_t iProgramTypeIndex)
{
    return BaseSerializer::isMatchTypeIndex(iSerializedTypeIndex, iProgramTypeIndex);
}

// ***************************************************************************
//      データ回復
// ***************************************************************************

//----------------------------------------------------------------------------
//      プリミティブ処理
//----------------------------------------------------------------------------

#define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSimbol)                    \
    void FastMidISerializer::loadPrimitive(dType& oPrimitive)               \
    {                                                                       \
        mIStream.read(reinterpret_cast<char*>(&oPrimitive), sizeof(oPrimitive));\
        if (!mIStream.good()) {                                             \
            THEOLIZER_INTERNAL_IO_ERROR(u8"I/O Error.");                    \
        }                                                                   \
    }


//      ---<<< 文字列型 >>>---

#define THEOLIZER_INTERNAL_DEF_STRING(dType, dSimbol)                       \
    void FastMidISerializer::loadPrimitive(dType& oPrimitive)               \
    {                                                                       \
        unsigned aDataSize=sizeof(dType::value_type);                       \
        std::size_t size;                                                   \
        loadPrimitive(size);                                                \
        oPrimitive.resize(size/aDataSize);                                  \
        if (size) {                                                         \
            mIStream.read(reinterpret_cast<char*>(&oPrimitive[0]), size);   \
        }                                                                   \
        if (!mIStream.good()) {                                             \
            THEOLIZER_INTERNAL_IO_ERROR(u8"oPrimitive");                    \
        }                                                                   \
    }

//      ---<<< 実体定義 >>>---

#include "primitive.inc"

// ***************************************************************************
//      ユーティリティ
// ***************************************************************************

//----------------------------------------------------------------------------
//      文字列回復
//----------------------------------------------------------------------------

void FastMidISerializer::loadString(std::string& iString)
{
    std::size_t size;
    loadPrimitive(size);
    iString.resize(size);
    if (size) {
        mIStream.read(&(*iString.begin()), size);
    }
//std::cout << "### loadString(" << size << ":" << iString << ");\n";
    if (!mIStream.good()) {
        THEOLIZER_INTERNAL_IO_ERROR(u8"I/O Error.");
    }
}

//############################################################################
//      FastSerializerを用いたユーティリティ
//############################################################################

// ***************************************************************************
//      メモリ・ストリーム
// ***************************************************************************

//----------------------------------------------------------------------------
//      メモリ・ストリームの内部実装クラス
//          stringstreamを保持する
//----------------------------------------------------------------------------

struct MemoryStream::Impl
{
    std::stringstream   mStream;
    Impl() noexcept
    { }
};

//----------------------------------------------------------------------------
//      コンストラクタ／デストラクタ
//----------------------------------------------------------------------------

MemoryStream::MemoryStream() noexcept :
    mImpl(std::unique_ptr<Impl>(new Impl()))
{ }

MemoryStream::~MemoryStream() noexcept
{ }

//----------------------------------------------------------------------------
//      操作関数
//----------------------------------------------------------------------------

//      ---<<< iostream返却 >>>---

std::iostream& MemoryStream::get() noexcept
{
    return mImpl->mStream;
}

//      ---<<< 内容と状態のクリア >>>---

void MemoryStream::clear()
{
    mImpl->mStream.clear();
    mImpl->mStream.str("");
}

//      ---<<< 読出ポインタを先頭へ巻き戻す >>>---

void MemoryStream::rewind()
{
    mImpl->mStream.seekg(0u);
}

}   // namespace internal
}   // namespace theolizer
