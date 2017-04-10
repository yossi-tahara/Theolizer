//############################################################################
//      Theolizerライブラリのコア部(基底シリアライザ)
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

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#if defined(_MSC_VER)
    #pragma warning(disable:4100)
#endif

//############################################################################
//      インクルード
//############################################################################

#include "avoid-trouble.h"

#include <algorithm>
#include <tuple>        // for tie
#include <map>
#include <set>

#include "internal.h"
#include "core.h"

//############################################################################
//      Begin
//############################################################################

namespace theolizer
{
namespace internal
{

//############################################################################
//      基底Serializer(型チェック処理)
//############################################################################

// ***************************************************************************
//      型チェック用の各種クラス定義
// ***************************************************************************

//----------------------------------------------------------------------------
//      シリアライズ・データ上のTypeIndexリストの要素
//----------------------------------------------------------------------------

//      ---<<< クラスが含む要素のリスト >>>---

// 要素
struct SerializedElementTypeIndex
{
    std::string mName;
    std::size_t mTypeIndex;

    SerializedElementTypeIndex(std::string iName, size_t iTypeIndex) :
        mName(iName), mTypeIndex(iTypeIndex)
    { }
};

// 要素リスト
typedef std::vector<SerializedElementTypeIndex> SerializedElementListI;

//      ---<<< クラスの型情報 >>>---

// 型情報
struct SerializedTypeIndex
{
    std::string                 mTypeName;              // 型名
    TypeIndexList*              mProgramTypeIndex;      // 対応するプログラム側TypeIndex
    SerializedElementListI      mSerializedElementList; // 要素リスト

    SerializedTypeIndex() : mProgramTypeIndex(nullptr) { }
};

// 型情報のリスト
struct BaseSerializer::SerializedTypeListI : public std::vector<SerializedTypeIndex>
{ };

//----------------------------------------------------------------------------
//      シリアライズ・データ上の型名リストの要素
//----------------------------------------------------------------------------

//      ---<<< クラスが含む要素のリスト >>>---

// 要素
struct SerializedElementTypeName
{
    std::string mName;
    std::string mTypeName;

    SerializedElementTypeName(std::string iName, std::string iTypeName) :
        mName(iName), mTypeName(iTypeName)
    { }
};

// 要素リスト
typedef std::vector<SerializedElementTypeName>  SerializedElementListN;

//      ---<<< クラスの型情報 >>>---

// 型情報
struct SerializedTypeName
{
    std::string                 mTypeName;              // 型名
    TypeIndexList*              mProgramTypeIndex;      // 対応するプログラム側TypeIndex
    SerializedElementListN      mSerializedElementList; // 要素リスト

    SerializedTypeName(std::string&& iTypeName,
                       TypeIndexList* iProgramTypeIndex,
                       SerializedElementListN iSerializedElementListN) :
        mTypeName(iTypeName),
        mProgramTypeIndex(iProgramTypeIndex),
        mSerializedElementList(iSerializedElementListN)
    { }
};

// 型情報のリスト
struct BaseSerializer::SerializedTypeListN : public std::vector<SerializedTypeName>
{ };

// ***************************************************************************
//      バージョン番号対応表生成
// ***************************************************************************

void BaseSerializer::createVersionNoTable()
{
//----------------------------------------------------------------------------
//      TypeIndex対応表
//----------------------------------------------------------------------------

    // 最新版でない時のみ生成する
    if (mLastGlobalVersionNo != mGlobalVersionNo)
    {
        // バージョン番号領域確保
        mVersionNoList.resize(mTypeIndexCount);
        // バージョン番号リスト生成
        auto aVersionNoIterator = mVersionNoList.begin();
        for (auto&& aTypeInfo : mTypeInfoList)
        {
            *aVersionNoIterator++ = mGlobalVersionNoTable->
                getLocalVersionNo(mGlobalVersionNo, aTypeInfo->getStdTypeIndex());
        }
    }
    else if (mVersionNoList.size())
    {
        mVersionNoList.resize(0);
    }

//----------------------------------------------------------------------------
//      派生Serializerのバージョン番号
//----------------------------------------------------------------------------

    // 最新版でない時
    if (mLastGlobalVersionNo != mGlobalVersionNo)
    {
        mSerializerVersionNo = mGlobalVersionNoTable->
            getLocalVersionNo
            (
                mGlobalVersionNo,
                BaseTypeFunctions::getInstance().getStdTypeIndex()
            );
    }
    else
    {
        mSerializerVersionNo=BaseTypeFunctions::getInstance().getLastVersionNo();
    }
}

// ***************************************************************************
//      ヘッダ内型情報保存（通常シリアライズ）
// ***************************************************************************

void BaseSerializer::writeHeaderTypeInfo()
{
//----------------------------------------------------------------------------
//      準備
//----------------------------------------------------------------------------

    // 開始マーク
    AutoRestoreSave aAutoRestoreSave(*this, emOrder,
                                     mCheckMode == CheckMode::NoTypeCheck);

//----------------------------------------------------------------------------
//      型チェック処理分岐
//----------------------------------------------------------------------------

    // メタ・データ保存
    if (mIsMetaMode)
    {
        writeMetaData();
return;
    }

    // 型チェックのモード出力
    writePreElement();
    saveControl(static_cast<typename std::underlying_type<CheckMode>::type>(mCheckMode));

    // 型チェック無しなら終了
    if ((mCheckMode != CheckMode::TypeCheck)
     && (mCheckMode != CheckMode::TypeCheckByIndex))
return;

#if 0
    for (auto&& aLoop : getRBForIndexer(mTypeInfoList))
    {
        std::cout << "[" << aLoop.getIndex() << "] "
                  << aLoop.front()->getTypeName(mVersionNoList)
                  << " isTopLevel()=" << aLoop.front()->isTopLovel()
                  << " isManual()=" << aLoop.front()->isManual() << "\n";
    }
    std::cout.flush();
#endif

//----------------------------------------------------------------------------
//      ヘッダ内の型情報保存
//----------------------------------------------------------------------------

    // 保存状況クリア
    std::vector<SaveStat> aSaveStatList(mTypeIndexCount, essIdle);
    {
        auto aSaveStatIterator = aSaveStatList.begin();
        for (auto&& aTypeInfo : mTypeInfoList)
        {
            if (aTypeInfo->isTopMatch(mDestinations) || aTypeInfo->isManual())
            {
                *aSaveStatIterator=essSaving;
            }
            ++aSaveStatIterator;
        }
    }

    // TypeIndexによるチェックなら、TypeIndex最大値出力
    if (mCheckMode == CheckMode::TypeCheckByIndex)
    {
        writePreElement();
        saveControl(mTypeIndexCount);
    }

    // 型情報出力(追加がなくなるまで繰り返す)
    bool aAgain=false;
    do
    {
        aAgain=false;
        for (auto&& aLoop : getRBForIndexer(mTypeInfoList))
        {
            auto aTypeIndex =aLoop.getIndex();
            auto aTypeInfo=aLoop.front();

            if (aSaveStatList[aTypeIndex] != essSaving)
        continue;
            aSaveStatList[aTypeIndex] = essSaved;

            // ポインタ／参照で派生クラスが登録されていたら、その派生クラスの型情報を出力指示する
            if (aTypeInfo->mTypeCategory == etcPointerType)
            {
                BaseTypeInfo* aPointeeTypeInfo = aTypeInfo->getPointeeTypeInfo();
                if (aPointeeTypeInfo->setSaving(*this, aSaveStatList)) {
                    aAgain=true;   // 自分より前のものなら、再ループ
                }
            }

            // 配列ならば、基本型について出力指示する
            if (aTypeInfo->mTypeCategory == etcArrayType)
            {
                std::size_t aUnderlyingTypeIndex=aTypeInfo->getUnderlyingTypeIndex();
                aSaveStatList[aUnderlyingTypeIndex]=essSaving;
                // 自分より前のものなら、再ループ
                if (aUnderlyingTypeIndex <= aTypeIndex)
                {
                    aAgain=true;
                }
            }

            // TypeCheckの時はClassTypeのみ出力する
            if ((mCheckMode == CheckMode::TypeCheck)
             && (aTypeInfo->mTypeCategory != etcClassType))
        continue;

            // 開始マーク
            writePreElement();
            AutoRestoreSave aAutoRestoreSave2(*this, emOrder, true);

            // TypeIndex
            if (mCheckMode == CheckMode::TypeCheckByIndex) {
                writePreElement();
                saveControl(aTypeIndex);
            }

            // TypeName
            writePreElement();
            std::string aTypeName=aTypeInfo->getTypeName(mVersionNoList);
            saveControl(aTypeName);

            if (aTypeInfo->mTypeCategory != etcClassType)
        continue;

            // バージョン番号と対応方法獲得
            unsigned aVersionNo=mVersionNoList.at(aTypeIndex);
            ElementsMapping aElementsMapping = aTypeInfo->getElementsMapping(aVersionNo);

            // 手動型なら出力せず、クラス要素のヘッダ出力を指示する
            if (aTypeInfo->getTypeKind() & etkManualFlag)
            {
                for (auto&& aElement : aTypeInfo->getElementRange(aVersionNo))
                {
                    // 保存先一致確認
                    if (!mDestinations.isMatch(aElement.getDestinations()))
                continue;

                    // まだ保存されてないなら、保存する
                    size_t aElementTypeIndex=aElement.getTypeIndex();
                    if (aSaveStatList[aElementTypeIndex] == essIdle)
                    {
                        aSaveStatList[aElementTypeIndex]=essSaving;
                        // 自分より前のものなら、再ループ
                        if (aElementTypeIndex <= aTypeIndex)
                        {
                            aAgain=true;
                        }
                    }
                }
        continue;
            }

            // ElementListを出力
            writePreElement();
            AutoRestoreSave aAutoRestoreSave3(*this, aElementsMapping);

            for (auto&& aElement : aTypeInfo->getElementRange(aVersionNo))
            {
                // 保存先一致確認
                if (!mDestinations.isMatch(aElement.getDestinations()))
            continue;

                writePreElement();

                // 名前対応時のみ要素名を記録する
                saveElementName(aElementsMapping, aElement.getName());

                // 要素の形名、もしくは、TypeIndex
                size_t aElementTypeIndex=aElement.getTypeIndex();
                if (mCheckMode == CheckMode::TypeCheck)
                {
                    // 処理中要素の型名取出し
                    BaseTypeInfo* aElementInfo = mTypeInfoList[aElementTypeIndex];
                    std::string aElementTypeName=aElementInfo->getTypeName(mVersionNoList);
                    saveControl(aElementTypeName);
                }
                else
                {
                    saveControl(aElementTypeIndex);
                }

                // まだ保存されてないなら、保存する
                if (aSaveStatList[aElementTypeIndex] == essIdle)
                {
                    aSaveStatList[aElementTypeIndex]=essSaving;
                    // 自分より前のものなら、再ループ
                    if (aElementTypeIndex <= aTypeIndex)
                    {
                        aAgain=true;
                    }
                }
            }
        }
    }
    while(aAgain);
}

// ***************************************************************************
//      トップ・レベル保存前後処理
// ***************************************************************************

//----------------------------------------------------------------------------
//      前処理
//----------------------------------------------------------------------------

void BaseSerializer::saveProcessStart(size_t iTypeIndex)
{
    saveClassStart(true);

    BaseTypeInfo* aTypeInfo=mTypeInfoList[iTypeIndex];

    switch(mCheckMode)
    {
    case CheckMode::InMemory:
    case CheckMode::NoTypeCheck:
        break;

    case CheckMode::TypeCheck:
        {
            writePreElement();
            saveControl(aTypeInfo->getTypeName(mVersionNoList));
        }
        break;

    case CheckMode::TypeCheckByIndex:
        writePreElement();
        saveControl(iTypeIndex);
        break;
    default:
        THEOLIZER_INTERNAL_ABORT("mCheckMode=%d", static_cast<int>(mCheckMode));
        break;
    }

    writePreElement();
}

//----------------------------------------------------------------------------
//      後処理
//----------------------------------------------------------------------------

void BaseSerializer::saveProcessEnd()
{
    saveClassEnd(true);
}

// ***************************************************************************
//      型名と型／バージョン番号対応表生成
// ***************************************************************************

//----------------------------------------------------------------------------
//      型名と型Index対応表（デシリアライズ処理用）
//          シリアライズ・データ上のグローバル・バージョン番号に該当する型名と
//          その型名を持つ型のTypeIndexの対応表である。
//          下記要因により１つの型名に複数のTypeIndexが対応することがある。
//              互換性のあるプリミティブ化クラスについて同じ型名を割り当てる場合
//              intとlongがint32のように同じ形名となる場合
//              参照と非参照は形名が異なるが最後の&を外して同じ形名として処理する
//----------------------------------------------------------------------------

struct BaseSerializer::TypeNameMap
{
    std::map<std::string const, TypeIndexList>   mMap;
    void add(std::string const& iTypeName, std::size_t iTypeIndex)
    {
//std::cout << "TypeNameMap::add(" << iTypeName << ", " << iTypeIndex << ");";
        auto pos = mMap.lower_bound(iTypeName);
        if ((pos != mMap.end()) && (pos->first == iTypeName))
        {
//std::cout << " -> appended\n";
            pos->second.emplace_back(iTypeIndex);
        }
        else
        {
//std::cout << " -> add\n";
            TypeIndexList aTypeIndexList;
            aTypeIndexList.emplace_back(iTypeIndex);
            mMap.emplace_hint(pos, iTypeName, aTypeIndexList);
        }
    }
};

//----------------------------------------------------------------------------
//      対応表生成処理
//----------------------------------------------------------------------------

void BaseSerializer::createTypeNameMap(bool iIsClassOnly)
{
    // TypeIndexとInMemoryの時は生成不要
    if ((mCheckMode == CheckMode::TypeCheckByIndex)
     || (mCheckMode == CheckMode::InMemory))
return;

    mTypeNameMap=std::unique_ptr<TypeNameMap>(new TypeNameMap);

    for (auto&& aIndexer : getRBForIndexer(mTypeInfoList))
    {
        if (iIsClassOnly && (aIndexer.front()->mTypeCategory != etcClassType))
    continue;

        mTypeNameMap->
            add
            (
                aIndexer.front()->getTypeName(mVersionNoList),
                aIndexer.getIndex()
            );
    }
}

// ***************************************************************************
//      トップ・レベル回復前後処理
// ***************************************************************************

//----------------------------------------------------------------------------
//      前処理
//----------------------------------------------------------------------------

void BaseSerializer::loadProcessStart(std::size_t iTypeIndex)
{
    loadClassStart(true);

    switch(mCheckMode)
    {
    case CheckMode::InMemory:
    case CheckMode::NoTypeCheck:
        break;

    case CheckMode::TypeCheck:
        {
            if (!readPreElement())
            {
                THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
            }

            std::string  aTypeName;
            loadControl(aTypeName);

            // 型名から、現在のTypeIndexListを求める
            TypeIndexList& aTypeIndexList=mTypeNameMap->mMap[aTypeName];
            bool aIsMatch=false;
            for (auto aTypeIndex : aTypeIndexList)
            {
                if (aTypeIndex == iTypeIndex)
                {
                    aIsMatch=true;
            break;
                }
            }
            if (!aIsMatch)
            {
                THEOLIZER_INTERNAL_DATA_ERROR(u8"Unmatch type.(%1%)", aTypeName);
            }
        }
        break;

    case CheckMode::TypeCheckByIndex:
        if (!readPreElement())
        {
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
        }

        size_t aTypeIndex;
        loadControl(aTypeIndex);
        if (!isMatchTypeIndex(aTypeIndex, iTypeIndex))
        {
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Unmatch type.");
        }
        break;

    default:
        THEOLIZER_INTERNAL_ABORT("mCheckMode=%d", static_cast<int>(mCheckMode));
        break;
    }

    if (!readPreElement())
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
    }
}

//----------------------------------------------------------------------------
//      後処理
//----------------------------------------------------------------------------

void BaseSerializer::loadProcessEnd()
{
    loadClassEnd(true);
}

//############################################################################
//      現在処理中のSerializer
//############################################################################

thread_local BaseSerializer* xNowSerializer=nullptr;

bool isLastVersion()
{
    return xNowSerializer->isLastVersion();
}

bool isSaver()
{
    return xNowSerializer->isSaver();
}

bool duringBackup()
{
    return xNowSerializer->duringBackup();
}

unsigned& getUpVersionCount()
{
    return xNowSerializer->mUpVersionCount;
}

//############################################################################
//      基底Serializer(シリアライズ／デシリアライズ実処理)
//############################################################################

// ***************************************************************************
//      オブジェクト追跡用オブジェクト管理テーブル
// ***************************************************************************

struct SerializeKey
{
    void*           mAddress;
    std::type_index mStdTypeIndex;

    SerializeKey() :
        mAddress(nullptr),
        mStdTypeIndex(typeid(nullptr))
    { }

    SerializeKey
    (
        void* iAddress,
        std::type_info const& iTypeInfo
    ) : mAddress(iAddress),
        mStdTypeIndex(iTypeInfo)
    { }

    bool operator<(SerializeKey const& iRhs) const
    {
        return std::tie(mAddress, mStdTypeIndex) < std::tie(iRhs.mAddress, iRhs.mStdTypeIndex);
    }
};

struct BaseSerializer::SerializeList : public std::map<SerializeKey, SerializeInfo> { };
struct BaseSerializer::DeserializeList : public std::vector<DeserializeInfo> { };

// ***************************************************************************
//      コンストラクタ
// ***************************************************************************

BaseSerializer::BaseSerializer
(
    Destinations const& iDestinations,
    GlobalVersionNoTableBase const* iGlobalVersionNoTable,
    unsigned iGlobalVersionNo,
    unsigned iLastGlobalVersionNo,
    CheckMode iCheckMode,
    bool iIsSaver,
    std::ostream* iOStream,
    bool iNoThrowException
) : ErrorBase(),
    mSerializerBack(nullptr),
    mDestinations(iDestinations),
    mIsSaver(iIsSaver),
    mNoThrowException(iNoThrowException),
    mGlobalVersionNoTable(iGlobalVersionNoTable),
    mGlobalVersionNo(iGlobalVersionNo),
    mLastGlobalVersionNo(iLastGlobalVersionNo),
    mDuringBackup(false),
    mTypeInfoList(TypeInfoList::getInstance().getList()),
    mTypeIndexCount(mTypeInfoList.size()),
    mRequireCheckTracking(false),
    mAdditionalInfo(*this),
    mBaseProcessing(false),
    mClassTracking(false),
    mRefProcessing(false),
    mUpVersionCount(0),
    mCheckMode(iCheckMode),
    mElementsMapping(emOrder),
    mIsShared(false),
    mIndent(0),
    mCancelPrettyPrint(false),
    mSerializerVersionNo(0),
    mVersionNoList(),
    mSerializedTypeListI(new SerializedTypeListI),
    mSerializedTypeListN(new SerializedTypeListN),
    mTypeNameMap(nullptr),
    mTypeIndexTable(),
    mObjectId(0),
    mSerializeList(nullptr),
    mDeserializeList(nullptr),
    mOStream(iOStream),
    mIsMetaMode((iCheckMode == CheckMode::MetaMode) || (iOStream != nullptr))
{
    THEOLIZER_INTERNAL_ASSERT(mGlobalVersionNoTable != nullptr,
        "GlobalVersionNoTable is illegal.");

    if (mIsSaver)
    {
        mSerializeList=std::unique_ptr<SerializeList>(new SerializeList);
        initSerializeList();
    }
    else
    {
        mDeserializeList=std::unique_ptr<DeserializeList>(new DeserializeList);
        mDeserializeList->emplace_back();
        (*mDeserializeList)[0].mStatus=etsNullPtr;
    }

    // 処理中のSerializerをバックアップし、自分に切り替える。
    //  例外が怖いので最後で設定
    mSerializerBack=xNowSerializer;
    xNowSerializer=this;
}

//----------------------------------------------------------------------------
//      補助関数
//----------------------------------------------------------------------------

void BaseSerializer::initSerializeList()
{
    mObjectId=0;
    mSerializeList->emplace
    (
        std::piecewise_construct,
        std::forward_as_tuple(nullptr, typeid(nullptr)),
        std::forward_as_tuple(mObjectId, etsNullPtr)
    );
    mObjectId++;
}

// ***************************************************************************
//      デストラクタ
//          mSerializeListとmDeserializeListのデストラクトのため、
//          ヘッダで定義するとエラーとなる。
// ***************************************************************************

BaseSerializer::~BaseSerializer() noexcept
{
    // エラー情報登録準備
    theolizer::internal::ApiBoundary aApiBoundary(&mAdditionalInfo);

    // エラー／警告が残っていたら、プロセス停止する。
    //  ただし、コンストラクト中と例外有りの時を除く
    auto&& aErrorInfo=getErrorInfo();
    if (!mConstructorError && mNoThrowException && aErrorInfo)
    {
        std::cout << aErrorInfo.getMessage() << "\n";
        THEOLIZER_INTERNAL_ABORT
            (u8"Error occured, please call resetError() before destructing serializer.");
    }

    // clearTracking()が必要なのに呼ばれて無く、
    // かつ、例外処理中でなければ、プロセス停止する。
    //  なお、FastSerializer(ImMemory)についてはチェックしない。
    if (!std::uncaught_exception()
     && mRequireCheckTracking
     && (mCheckMode != CheckMode::InMemory))
    {
        THEOLIZER_INTERNAL_ABORT
            (u8"For object tracking, please call clearTracking() before destructing serializer.");
    }

    // 処理中のSerializerを回復する
    xNowSerializer=mSerializerBack;
}

// ***************************************************************************
//      シリアライズ・データの指定TypeIndexのクラスの指定番目の要素の名前獲得
// ***************************************************************************

std::string BaseSerializer::getDataElementName(std::size_t iDataTypeIndex, std::size_t iDataIndex)
{
    std::string ret;
    switch(mCheckMode)
    {
    case CheckMode::TypeCheck:
        ret=mSerializedTypeListN->at(iDataTypeIndex).mSerializedElementList[iDataIndex].mName;
        break;

    case CheckMode::TypeCheckByIndex:
        ret=mSerializedTypeListI->at(iDataTypeIndex).mSerializedElementList[iDataIndex].mName;
        break;

    default:
        THEOLIZER_INTERNAL_ABORT("mCheckMode=%d", static_cast<int>(mCheckMode));
        break;
    }

    return ret;
}

// ***************************************************************************
//      シリアライズ機能群
// ***************************************************************************

//----------------------------------------------------------------------------
//      トップ・レベル処理補助クラス
//----------------------------------------------------------------------------

BaseSerializer::AutoRestoreSaveProcess::AutoRestoreSaveProcess
(
    BaseSerializer& iSerializer,
    size_t iTypeIndex
) : mSerializer(iSerializer),
    mElementsMapping(iSerializer.mElementsMapping),
    mIndent(iSerializer.mIndent),
    mCancelPrettyPrint(iSerializer.mCancelPrettyPrint)
{
    mSerializer.mElementsMapping=emOrder;
    mSerializer.mCancelPrettyPrint=true;
    mSerializer.saveProcessStart(iTypeIndex);
}

BaseSerializer::AutoRestoreSaveProcess::~AutoRestoreSaveProcess() noexcept
{
    theolizer::internal::Releasing aReleasing{};
    try
    {
        mSerializer.mIndent=mIndent;
        mSerializer.saveProcessEnd();
        mSerializer.mCancelPrettyPrint=mCancelPrettyPrint;
        mSerializer.mElementsMapping=mElementsMapping;
    }
    catch (std::exception& e)
    {
        THEOLIZER_INTERNAL_ERROR(e.what());
    }
    catch (...)
    {
        THEOLIZER_INTERNAL_ERROR(u8"Unknown exception");
    }
}

//----------------------------------------------------------------------------
//      クラス(配列／侵入型／非侵入型)処理補助クラス
//----------------------------------------------------------------------------

BaseSerializer::AutoRestoreSave::AutoRestoreSave(
            BaseSerializer& iSerializer,
            ElementsMapping iElementsMapping,
            bool iCancelPrettyPrint) :
                mSerializer(iSerializer),
                mElementsMapping(iSerializer.mElementsMapping),
                mIndent(iSerializer.mIndent),
                mCancelPrettyPrint(iSerializer.mCancelPrettyPrint)
{
    mSerializer.mElementsMapping=iElementsMapping;
    mSerializer.mCancelPrettyPrint=iCancelPrettyPrint;
    mSerializer.saveClassStart();
}

BaseSerializer::AutoRestoreSave::~AutoRestoreSave() noexcept
{
    theolizer::internal::Releasing aReleasing{};
    try
    {
        mSerializer.mIndent=mIndent;
        mSerializer.saveClassEnd();
        mSerializer.mCancelPrettyPrint=mCancelPrettyPrint;
        mSerializer.mElementsMapping=mElementsMapping;
    }
    catch (std::exception& e)
    {
        THEOLIZER_INTERNAL_ERROR(e.what());
    }
    catch (...)
    {
        THEOLIZER_INTERNAL_ERROR(u8"Unknown exception");
    }
}

//----------------------------------------------------------------------------
//      トップ・レベル処理補助クラス
//----------------------------------------------------------------------------

BaseSerializer::AutoRestoreLoadProcess::AutoRestoreLoadProcess
(
    BaseSerializer& iSerializer,
    size_t iTypeIndex
) : mSerializer(iSerializer)
{
    mSerializer.loadProcessStart(iTypeIndex);
}

BaseSerializer::AutoRestoreLoadProcess::~AutoRestoreLoadProcess() noexcept
{
    theolizer::internal::Releasing aReleasing{};
    try
    {
        mSerializer.loadProcessEnd();
    }
    catch (std::exception& e)
    {
        THEOLIZER_INTERNAL_ERROR(e.what());
    }
    catch (...)
    {
        THEOLIZER_INTERNAL_ERROR(u8"Unknown exception");
    }
}

//----------------------------------------------------------------------------
//      クラス(配列／侵入型／非侵入型)処理補助クラス
//----------------------------------------------------------------------------

BaseSerializer::AutoRestoreLoad::AutoRestoreLoad(
            BaseSerializer& iSerializer,
            ElementsMapping iElementsMapping) :
                mSerializer(iSerializer),
                mElementsMapping(iSerializer.mElementsMapping)
{
    mSerializer.mElementsMapping=iElementsMapping;
    mSerializer.loadClassStart();
}

BaseSerializer::AutoRestoreLoad::~AutoRestoreLoad() noexcept
{
    theolizer::internal::Releasing aReleasing{};
    try
    {
        mSerializer.loadClassEnd();
        mSerializer.mElementsMapping=mElementsMapping;
    }
    catch (std::exception& e)
    {
        THEOLIZER_INTERNAL_ERROR(e.what());
    }
    catch (...)
    {
        THEOLIZER_INTERNAL_ERROR(u8"Unknown exception");
    }
}

// ***************************************************************************
//      オブジェクト追跡機能群
// ***************************************************************************

//----------------------------------------------------------------------------
//      シリアライズ用オブジェクト登録
//----------------------------------------------------------------------------

SerializeInfo& BaseSerializer::registerObject
(
    void*                   iAddress,
    std::type_info const&   iTypeInfo,
    TrackingMode            iTrackingMode,
    bool*                   oIsSaved
)
{
//std::cout << "registerObject(" << iTypeInfo.name() << ")" << std::endl;

    if (oIsSaved) *oIsSaved=false;

    TrackingStatus aTrackingStatus=etsInit;
    switch(iTrackingMode)
    {
    case etmDefault:    aTrackingStatus=etsPointed;     break;
    case etmPointee:    aTrackingStatus=etsProcessed;   break;
    case etmOwner:      aTrackingStatus=etsPointed;     break;
    }

    // テーブルにあるか探す
    SerializeKey aSerializeKey(iAddress, (iAddress)?iTypeInfo:typeid(nullptr));
    auto found = mSerializeList->find(aSerializeKey);

    // 見つかった時
    if ((found != mSerializeList->end())
     && (!(mDuringBackup && (iTrackingMode == etmPointee)))) // バックアップ中のPointeeは強制不一致
    {
        // 既にターゲット保存(etsProcessed)済みなら状態を更新しない
        if (found->second.mStatus != etsPointed)
        {
            if (oIsSaved) *oIsSaved=true;
//std::cout << "    saved : mObjectId=" << found->second.mObjectId 
//          << " mStatus=" << found->second.mStatus << " " << iAddress << "\n";
        }

        // ポインタからの参照有り
        else
        {
            found->second.mStatus=aTrackingStatus;
//std::cout << "    found : mObjectId=" << found->second.mObjectId
//        << " mStatus=" << found->second.mStatus << " " << iAddress << "\n";
        }
return found->second;
    }

//std::cout << "    mObjectId=" << mObjectId << " aTrackingStatus=" << aTrackingStatus 
//          << " " << iAddress << std::endl;

    auto inserted = mSerializeList->
        emplace(std::move(aSerializeKey), SerializeInfo(mObjectId++, aTrackingStatus));
    return inserted.first->second;
}

//----------------------------------------------------------------------------
//      デシリアライズ用回復済チェック
//----------------------------------------------------------------------------

bool BaseSerializer::isLoadedObject(size_t iObjectId, void*& oPointer)
{
    if (mDeserializeList->size() <= iObjectId)
return false;

    if (((*mDeserializeList)[iObjectId].mStatus == etsInit)
     || ((*mDeserializeList)[iObjectId].mStatus == etsPointed))
return false;

    oPointer=(*mDeserializeList)[iObjectId].mAddress;
    return true;
}

//----------------------------------------------------------------------------
//      デシリアライズ用アドレス登録／回復
//----------------------------------------------------------------------------

void BaseSerializer::recoverObject
(
    size_t iObjectId,
    void*& oPointer,
    std::type_info const& iTypeInfo,
    TrackingMode iTrackingMode,
    bool* oIsLoaded
)
{
//std::cout << "recoverObject(" << iTypeInfo.name() << ") iObjectId = " << iObjectId << std::endl;
//std::cout << "    oPointer=" << oPointer << std::endl;
    if (oIsLoaded) *oIsLoaded=false;

    if (mDeserializeList->capacity() <= iObjectId)
    {
        mDeserializeList->reserve(iObjectId+16);
    }
    if (mDeserializeList->size() <= iObjectId)
    {
        mDeserializeList->resize(iObjectId+1);
//std::cout << "    resize() iObjectId = " << iObjectId << std::endl;
    }

    switch((*mDeserializeList)[iObjectId].mStatus)
    {
    case etsInit:               // 未登録
//std::cout << "    etsInit : " << std::endl;
        if (iTrackingMode == etmDefault)
        {
            // 追跡指定でないなら、
            (*mDeserializeList)[iObjectId].mStatus=etsPointed;

            // ポインタ自身のアドレスを登録し、ポインタに番兵としてnullptr設定
            (*mDeserializeList)[iObjectId].mAddress=&oPointer;
            oPointer=nullptr;
        }
        else
        {
            // 追跡指定されていたら、回復済設定し、
            (*mDeserializeList)[iObjectId].mStatus = etsProcessed;

            // アドレスを登録する
            (*mDeserializeList)[iObjectId].mAddress=oPointer;
        }
        break;

    case etsNullPtr:        // アドレス登録済／nullptr
//std::cout << "    etsNullPtr : " << std::endl;
        if (oIsLoaded) *oIsLoaded=true;
        oPointer=nullptr;
        break;

    case etsPointed:         // アドレス登録済／オブジェクト未回復
//std::cout << "    etsPointed : " << std::endl;
        if (iTrackingMode == etmDefault)
        {
            // 追跡指定でないなら、自分自身のアドレスをリンク・リストへ登録する
            oPointer=(*mDeserializeList)[iObjectId].mAddress;
            (*mDeserializeList)[iObjectId].mAddress=&oPointer;
//std::cout << "    oPointer = " << oPointer << std::endl;
        }
        else
        {
            // 追跡指定されていたら、回復済設定し、
            (*mDeserializeList)[iObjectId].mStatus = etsProcessed;
//std::cout << "    -> etsProcessed" << std::endl;

            // リンク・リストを解決する
            void** p=reinterpret_cast<void**>((*mDeserializeList)[iObjectId].mAddress);
            (*mDeserializeList)[iObjectId].mAddress=oPointer;
            while(p != nullptr)
            {
                void **q=reinterpret_cast<void**>(*p);  // 次のポインタを一旦退避
                *p=oPointer;            // ポインタへオブジェクトのアドレスを設定
                p=q;                    // 次のポインタ処理へ
            }
        }
        break;

    case etsProcessed:      // アドレス登録済／オブジェクト回復済
//std::cout << "    etsProcessed : " << std::endl;
        if (oIsLoaded) *oIsLoaded=true;

        // 通常ポインタ以外（領域管理する）なら、アドレスを更新する
        //  複数回同じ領域のシリアライズを許可するが、回復時も同じアドレスとは限らないため。
        //  ただし、ポインタ保存する時（オブジェクト追跡する時）に、
        //  同じオブジェクトのアドレスが変化するような処理をしては行けない。
        //  この事態はオブジェクト追跡単位中にシリアライズした領域を開放すると発生する。
        if (iTrackingMode == etmPointee)
        {
//std::cout << "    etmPointee : mAddress=" << oPointer << std::endl;
            if ((*mDeserializeList)[iObjectId].mAddress != oPointer)
            {
                THEOLIZER_INTERNAL_WRONG_USING(
                    "This instance(%1%) is different from saved.",
                    getNameByTypeInfo(iTypeInfo));
            }
        }
        else
        {
            oPointer=(*mDeserializeList)[iObjectId].mAddress;
//std::cout << "    !etmPointee : oPointer=" << oPointer << std::endl;
        }
        break;
    }
}

//----------------------------------------------------------------------------
//      オブジェクト追跡テーブルをクリアする
//          プロセス内で保存／回復する時(CheckMode::InMemory)は、
//          追跡が必要なオブジェクトが保存されなかった場合、
//          対象アドレスをそのまま保存し、回復する。
//          プロセス内なので矛盾は生じない。
//          これは、ポインタをそのままコピーするのと同じ操作である。
//----------------------------------------------------------------------------

void BaseSerializer::clearTrackingImpl()
{
    if (mSerializeList)
    {
        // ObjectId順で処理する
        for (std::size_t i=0; i < mSerializeList->size(); ++i)
        {
            for (auto&& element : *mSerializeList)
            {
                if (element.second.mObjectId != i)
            continue;

                if ((element.second.mStatus == etsInit)
                 || (element.second.mStatus == etsPointed))
                {
                    if (mCheckMode != CheckMode::InMemory)
                    {
                        THEOLIZER_INTERNAL_ERROR
                        (
                            u8"Some pointed data does not save.(%1%)",
                            getNameByTypeInfo(element.first.mStdTypeIndex)
                        );
                    }
                    else
                    {
                        std::uintptr_t  aPointer=
                            reinterpret_cast<std::uintptr_t>(element.first.mAddress);
                        saveControl(aPointer);
                    }
                }
            }
        }
        // 初期状態へ戻す(先頭はnullptr用)
        mSerializeList->clear();
        initSerializeList();
    }

    if (mDeserializeList)
    {
        for (auto&& aIndexer : theolizer::getRBForIndexer(*mDeserializeList))
        {
            DeserializeInfo& aDeserializeInfo = aIndexer.front();
            auto aObjectId = aIndexer.getIndex();
            if ((aDeserializeInfo.mStatus == etsInit)
             || (aDeserializeInfo.mStatus == etsPointed))
            {
                if (mCheckMode != CheckMode::InMemory)
                {
                    THEOLIZER_INTERNAL_ERROR(u8"Can not resolve the address of pointer.");
                }
                else
                {
                    std::uintptr_t aPointer;
                    loadControl(aPointer);
                    void* aPointer2=reinterpret_cast<void*>(aPointer);
                    recoverObject(aObjectId, aPointer2, typeid(aPointer2), etmOwner);
                }
            }
        }
        // 初期状態へ戻す(先頭はnullptr用)
        mDeserializeList->resize(1);
        (*mDeserializeList)[0].mStatus=etsNullPtr;
    }
}

void BaseSerializer::clearTracking()
{
    // エラー情報登録準備
    theolizer::internal::ApiBoundary aApiBoundary(&mAdditionalInfo);

    mRequireCheckTracking=false;
    if (mNoThrowException)
    {
        try
        {
            clearTrackingImpl();
        }
        catch (ErrorInfo&)
        {
        }
    }
    else
    {
        clearTrackingImpl();
    }
}

//----------------------------------------------------------------------------
//      SharedPtrTable登録テーブル
//----------------------------------------------------------------------------

struct BaseSerializer::SharedPtrTables : public std::map<std::type_index, SharedPtrTable>
{ };

SharedPtrTable& BaseSerializer::registerSharedPtrTable(std::type_index iTypeIndex)
{
    // テーブル未生成なら生成する
    if (!mSharedPtrTables)
    {
        mSharedPtrTables.reset(new SharedPtrTables);
    }

    // 対象が未登録なら登録する
    auto aFound=mSharedPtrTables->find(iTypeIndex);
    if (aFound == mSharedPtrTables->end())
    {
        aFound=mSharedPtrTables->insert(aFound, std::make_pair(iTypeIndex, SharedPtrTable()));
    }

    return aFound->second;
}

// ***************************************************************************
//      ClassTypeの破棄処理
// ***************************************************************************

void BaseSerializer::disposeClass(ElementsMapping iElementsMapping)
{
    AutoRestoreLoad aAutoRestoreLoad(*this, iElementsMapping);

    while(true)
    {
        if (!readPreElement())
    break;

        loadElementName(iElementsMapping);
        disposeElement();
    }
}

// ***************************************************************************
//      ポリモーフィズム対応
// ***************************************************************************

//----------------------------------------------------------------------------
//      型名／TypeIndexを読み込み、対応するプログラム側のTypeIndexを求める
//----------------------------------------------------------------------------

TypeIndexList& BaseSerializer::getProgramTypeIndex()
{
    if (mCheckMode != CheckMode::TypeCheckByIndex)
    {
        // 型名回復
        std::string  aTypeName;
        loadControl(aTypeName);

        // 型名から、現在のTypeIndexListを求める
return mTypeNameMap->mMap[aTypeName];
    }
    else
    {
        // TypeIndex回復
        size_t aTypeIndex=0;
        loadControl(aTypeIndex);
        auto& aElementType = mSerializedTypeListI->at(aTypeIndex);
return *(aElementType.mProgramTypeIndex);
    }
}

// ***************************************************************************
//      ヘッダ回復と型チェック
// ***************************************************************************

void BaseSerializer::readHeaderTypeInfo()
{
//----------------------------------------------------------------------------
//      準備
//----------------------------------------------------------------------------

    // 開始マーク
    AutoRestoreLoad aAutoRestoreLoad(*this);

//----------------------------------------------------------------------------
//      型チェックのモード情報回復
//----------------------------------------------------------------------------

    // メタ・データ保存
    if (mIsMetaMode)
    {
        readMetaData();
return;
    }

    std::underlying_type<CheckMode>::type   aCheckModeValue;

    // 型チェックのモード入力
    readPreElement();
    loadControl(aCheckModeValue);
    mCheckMode = static_cast<CheckMode>(aCheckModeValue);

    // 型チェック無しなら終了
    if ((mCheckMode != CheckMode::TypeCheck)
     && (mCheckMode != CheckMode::TypeCheckByIndex))
return;

//----------------------------------------------------------------------------
//      ヘッダ内の型情報回復
//----------------------------------------------------------------------------

    // TypeIndex対応表確保
    mTypeIndexTable.resize(mTypeIndexCount, kInvalidSize);

    // TypeIndexによるチェックなら、TypeIndex最大値入力し、mSerializedTypeListI領域確保
    if (mCheckMode == CheckMode::TypeCheckByIndex)
    {
        readPreElement();
        std::size_t aTypeIndexCount;
        loadControl(aTypeIndexCount);
        mSerializedTypeListI->resize(aTypeIndexCount);
    }

    // 型情報取出し
    while (readPreElement())
    {
        // 開始マーク
        AutoRestoreLoad aAutoRestoreLoad2(*this);

        // TypeIndex取出し
        size_t aDataTypeIndex=0;
        if (mCheckMode == CheckMode::TypeCheckByIndex)
        {
            if (!readPreElement())
    break;
            loadControl(aDataTypeIndex);
        }
        else
        {
            aDataTypeIndex=mSerializedTypeListN->size();
        }

        if (!readPreElement())
    break;
        std::string  aTypeName;
        loadControl(aTypeName);

        // プログラム側のTypeIndexList獲得
        //  プログラム側に存在しない時はTypeIndexListのsize()==0
//std::cout << "[" << aDataTypeIndex << ", " << aTypeName << "]\n";
        TypeIndexList& aTypeIndexList=mTypeNameMap->mMap[aTypeName];

        // 要素記録エリア
        SerializedElementListI  aSerializedElementListI;
        SerializedElementListN  aSerializedElementListN;

        // ClassType処理
        if (readPreElement())
        {
//std::cout << "    {\n";
            // 知らない型なら廃棄する
            if (aTypeIndexList.empty())
            {
                disposeElement();
    continue;
            }

            // この先頭の型のElementsMappingを取り出す
            std::size_t aProgramTypeIndex = *(aTypeIndexList.begin());
            unsigned aVersionNo = mVersionNoList.at(aProgramTypeIndex);
            BaseTypeInfo* aTypeInfo = mTypeInfoList[aProgramTypeIndex];
            ElementsMapping aElementsMapping = aTypeInfo->getElementsMapping(aVersionNo);

            // 手動型なら要素情報なし
              if (aTypeInfo->getTypeKind() & etkManualFlag)
      continue;

            // 開始マーク
            AutoRestoreLoad aAutoRestoreLoad3(*this, aElementsMapping);

            // 要素の型取出し
            while(readPreElement())
            {
                // 名前対応時のみ要素名を取り出す
                std::string aElementName=loadElementName(aElementsMapping);

                if (mCheckMode == CheckMode::TypeCheck)
                {
                    std::string aElementTypeName;
                    loadControl(aElementTypeName);
//std::cout << "        " << aElementName
//          << ":"        << aElementTypeName << "\n";
                    aSerializedElementListN.emplace_back(std::move(aElementName),
                                                         std::move(aElementTypeName));
                }
                else
                {
                    size_t aElementTypeIndex;
                    loadControl(aElementTypeIndex);
//std::cout << "        " << aElementName
//          << ":"        << aElementTypeIndex << "\n";
                    aSerializedElementListI.emplace_back(std::move(aElementName),
                                                         aElementTypeIndex);
                }
            }
//std::cout << "    }\n";
        }

        // TypeIndex対応表へ反映
        for (auto aTypeIndex : aTypeIndexList)
        {
//std::cout << "mTypeIndexTable[" << aTypeIndex << "]=" << aDataTypeIndex << "\n";
            mTypeIndexTable[aTypeIndex]=aDataTypeIndex;
        }
//std::cout << "   ended...\n";

        // 結果を設定
        if (mCheckMode == CheckMode::TypeCheckByIndex)
        {
#if 0
            if (mSerializedTypeListI->size() <= aDataTypeIndex)
            {
                mSerializedTypeListI->resize(aDataTypeIndex+1);
            }
#endif
            SerializedTypeIndex& aSerializedTypeIndex
                                    =mSerializedTypeListI->at(aDataTypeIndex);
            aSerializedTypeIndex.mTypeName=std::move(aTypeName);
            aSerializedTypeIndex.mProgramTypeIndex=&aTypeIndexList;
            aSerializedTypeIndex.mSerializedElementList
                                            =std::move(aSerializedElementListI);
        }
        else
        {
            mSerializedTypeListN->emplace_back(std::move(aTypeName),
                                               &aTypeIndexList,
                                               std::move(aSerializedElementListN));
        }
    }

//----------------------------------------------------------------------------
//      ヘッダ内の型チェック(型名基準)
//----------------------------------------------------------------------------

//std::cout << "mSerializedTypeListN : size=" << mSerializedTypeListN->size() << "\n";
    for (auto& aSerializedTypeName : *mSerializedTypeListN.get())
    {

// size(対応する型の数) == 0の時、無視
//      他で保存されたが、回復処理されていない型があると、ここに該当する。
// size(対応する型の数) >= 2の時、
//      各型はシリアライズ・データ上、互換性があるので先頭の型で評価する。
//      2015/10/29現在、手動型のみを想定し、それはElementCount=0である。

//      ---<<< 対応する型が存在しない時、無視 >>>---

        if ((!aSerializedTypeName.mProgramTypeIndex)
         || (aSerializedTypeName.mProgramTypeIndex->size() == 0))
    continue;

//      ---<<< クラス名をエラー位置情報として登録 >>>---

        AutoRestoreSerializeInfo aAutoRestoreSerializeInfo
            (*this, aSerializedTypeName.mTypeName.c_str());

//      ---<<< プログラム側情報獲得 >>>---

        // 先頭のBaseTypeInfoを用いる
        std::size_t aProgramTypeIndex = *(aSerializedTypeName.mProgramTypeIndex->begin());
        unsigned aVersionNo = mVersionNoList.at(aProgramTypeIndex);
        BaseTypeInfo* aTypeInfo = mTypeInfoList[aProgramTypeIndex];

//      size_t aElementCount = aTypeInfo->getElementRange(aVersionNo).size();
//std::cout << "    [" << aProgramTypeIndex << "]"
//          << " ElementCount(" << aVersionNo << ")"
//          << "=" << aElementCount << "\n";

//std::cout << "  --- program ---\n";
        std::vector<std::pair<char const*, std::string> > aProgramElements;

        // 各要素情報を抽出
        for (auto&& aElement : aTypeInfo->getElementRange(aVersionNo))
        {
            // 要素名
            char const* aElementName=aElement.getName();
            // 要素のTypeIndex
            size_t aElementTypeIndex=aElement.getTypeIndex();
            // 要素BaseTypeInfo
            BaseTypeInfo* aElementTypeInfo = mTypeInfoList[aElementTypeIndex];
            // 要素の型名
            std::string aElementTypeName=aElementTypeInfo->getTypeName(mVersionNoList);
//std::cout << "    " << aElementName
//          << " : "  << aElementTypeName << "\n";
            // 登録
            aProgramElements.emplace_back(aElementName, std::move(aElementTypeName));
        }

//      ---<<< 要素の型が一致することを確認する >>>---

//std::cout << "  --- checking ---\n";
        auto aProgramElementItr = aProgramElements.begin();
        for (auto&& aSerializedElement : aSerializedTypeName.mSerializedElementList)
        {
            // 名前対応
            if (aTypeInfo->getElementsMapping(aVersionNo) == emName)
            {
//std::cout << "    " << aSerializedElement.mName << " : " << aSerializedElement.mTypeName << "\n";
                for (auto&& aProgramElement : aProgramElements)
                {
                    if (aSerializedElement.mName != aProgramElement.first)
        continue;

//std::cout << "        " << aSerializedElement.mTypeName << " : "
//          << aProgramElement.second << "\n";
                // 対応する要素の型が一致しない時、エラー
                    if (aSerializedElement.mTypeName != aProgramElement.second)
                    {
                        AutoRestoreSerializeInfo aAutoRestoreSerializeInfo2
                            (*this, aProgramElement.first);
                        THEOLIZER_INTERNAL_DATA_ERROR(u8"Unmatch type.");
                    }
                }
            }

            // 順序対応
            else
            {
                if (aSerializedElement.mTypeName != aProgramElementItr->second)
                {
                    AutoRestoreSerializeInfo aAutoRestoreSerializeInfo2
                        (*this, aProgramElementItr->first);
                    THEOLIZER_INTERNAL_DATA_ERROR(u8"Unmatch type.");
                }
                ++aProgramElementItr;
                if (aProgramElementItr == aProgramElements.end())
        break;
            }
        }
//std::cout << "\n";
    }

//----------------------------------------------------------------------------
//      ヘッダ内の型チェック(TypeIndex基準)
//----------------------------------------------------------------------------

//std::cout << "mSerializedTypeListI : size=" << mSerializedTypeListI->size() << "\n";
    for (auto&& aSerializedTypeIndex : *mSerializedTypeListI.get())
    {

//      ---<<< 対応する型が存在しない時、無視 >>>---

        if ((!aSerializedTypeIndex.mProgramTypeIndex)
         || (aSerializedTypeIndex.mProgramTypeIndex->size() == 0))
    continue;

//      ---<<< クラス名を位置情報として登録 >>>---

        AutoRestoreSerializeInfo aAutoRestoreSerializeInfo
            (*this, aSerializedTypeIndex.mTypeName.c_str());

//      ---<<< プログラム側情報獲得 >>>---

        // 先頭のBaseTypeInfoを用いる
        std::size_t aProgramTypeIndex = *(aSerializedTypeIndex.mProgramTypeIndex->begin());
        unsigned aVersionNo = mVersionNoList.at(aProgramTypeIndex);
        BaseTypeInfo* aTypeInfo = mTypeInfoList[aProgramTypeIndex];

//std::cout << "    [" << aProgramTypeIndex << "]"
//          << " getElementRange(" << aVersionNo << ").size()="
//          << aTypeInfo->getElementRange(aVersionNo).size() << "\n";
        std::vector<std::pair<char const*, size_t> > aProgramElements;
        for (auto&& aElement : aTypeInfo->getElementRange(aVersionNo))
        {
            // 要素名
            char const* aElementName=aElement.getName();
            // 要素のTypeIndex
            size_t aElementTypeIndex=aElement.getTypeIndex();
            // 登録
            aProgramElements.emplace_back(aElementName, aElementTypeIndex);
//std::cout << "    " << aElementName
//          << " : "  << aElementTypeIndex << "\n";
        }

//      ---<<< 要素の型が一致することを確認する >>>---

//std::cout << "  --- checking ---\n";
        auto aProgramElementItr = aProgramElements.begin();
        for (auto&& aSerializedElement : aSerializedTypeIndex.mSerializedElementList)
        {
            // 名前対応
            if (aTypeInfo->getElementsMapping(aVersionNo) == emName)
            {
                for (auto&& aProgramElement : aProgramElements)
                {
                    if (aSerializedElement.mName != aProgramElement.first)
        continue;

//      ---<<< 対応する要素の型が一致しない時、エラー >>>---

                    if (!isMatchTypeIndex(aSerializedElement.mTypeIndex, aProgramElement.second))
                    {
                        AutoRestoreSerializeInfo aAutoRestoreSerializeInfo2
                            (*this, aProgramElement.first);
                        THEOLIZER_INTERNAL_DATA_ERROR(u8"Unmatch type.");
                    }
                }
            }

            // 順序対応
            else
            {
                if (!isMatchTypeIndex(aSerializedElement.mTypeIndex, aProgramElementItr->second))
                {
                    AutoRestoreSerializeInfo aAutoRestoreSerializeInfo2
                            (*this, aProgramElementItr->first);
                    THEOLIZER_INTERNAL_DATA_ERROR(u8"Unmatch type.");
                }
                ++aProgramElementItr;
                if (aProgramElementItr == aProgramElements.end())
        break;
            }
        }
//std::cout << "\n";
    }
}

//----------------------------------------------------------------------------
//      TypeIndex一致判定
//----------------------------------------------------------------------------

bool BaseSerializer::isMatchTypeIndex(size_t iSerializedTypeIndex, size_t iProgramTypeIndex)
{
    bool aIsMatch=false;
    // リストにある時だけチェックする(ないなら不一致)
    if (iSerializedTypeIndex < mSerializedTypeListI->size())
    {
        auto& aElementType = mSerializedTypeListI->at(iSerializedTypeIndex);
        for (auto aProgramTypeIndex : *(aElementType.mProgramTypeIndex))
        {
//std::cout << "        " << aProgramTypeIndex << " : " << iProgramTypeIndex << "\n";
            if (aProgramTypeIndex == iProgramTypeIndex)
            {
//std::cout << "        Match!!\n";
                aIsMatch=true;
    break;
            }
        }
    }
    return aIsMatch;
}

// ***************************************************************************
//      RegisterTypeコンストラクト時の余分なstruct定義回避
// ***************************************************************************

Destinations const BaseSerializer::kDestinations=Destinations();

//############################################################################
//      メタ処理
//          メタ・シリアライズは通常のシリアライズと同じ情報があるので、
//          通常処理と同じ流れで処理する
//
//          メタ・デシリアライズはシリアライズ時と異なり、型情報を一切持たない
//          そのため、各種ローカル変数を用いて、型定義を生成する。
//          それが通常のシリアライザへ影響しないよう構造を分離している。
//############################################################################

//----------------------------------------------------------------------------
//      UniqueName記録
//----------------------------------------------------------------------------

struct BaseSerializer::UniqueNameSet : public std::set<char const*>
{
};

// ***************************************************************************
//      メタ・シリアライズ
// ***************************************************************************

void BaseSerializer::writeMetaData()
{

//----------------------------------------------------------------------------
//      ローカル・バージョン番号テーブル生成
//----------------------------------------------------------------------------

//      ---<<< シリアライザ用 >>>---

    std::vector<VersionNoList>  aVersionNoLists;
    for (unsigned aGlobalVersionNo=1; aGlobalVersionNo <= mGlobalVersionNo; ++aGlobalVersionNo)
    {
        unsigned index=aGlobalVersionNo-1;

        aVersionNoLists.emplace_back(true);     // mIsMetaMode=true
        aVersionNoLists[index].resize(mTypeIndexCount);
        // バージョン番号リスト生成
        {
            auto aVersionNoIterator = aVersionNoLists[index].begin();
            for (auto&& aTypeInfo : mTypeInfoList)
            {
                *aVersionNoIterator++ = mGlobalVersionNoTable->
                    getLocalVersionNo(aGlobalVersionNo, aTypeInfo->getStdTypeIndex());
            }
        }
    }

//----------------------------------------------------------------------------
//      保存対象抽出
//          デシリアライザ側は、シリアライザ側で保存するものは保存済とする
//----------------------------------------------------------------------------

//      ---<<< 初期化 >>>---

    std::vector<SaveStat> aSaveStatList(mTypeIndexCount, essIdle);
    {
        auto aSaveStatIterator = aSaveStatList.begin();
        for (auto&& aTypeInfo : mTypeInfoList)
        {
            if (aTypeInfo->isTopMatch(mDestinations))
                *aSaveStatIterator=essSaving;
            ++aSaveStatIterator;
        }
    }

//      ---<<< 保存判定処理 >>>---

    for (auto&& aLoop : getRBForIndexer(mTypeInfoList))
    {
        auto aTypeIndex=aLoop.getIndex();
        auto aTypeInfo =aLoop.front();

        // 保存先が不一致なら非対象
        if (!aTypeInfo->isTopMatch(mDestinations))
    continue;

        judgeWriteTypes(aTypeIndex, aTypeInfo, mTypeInfoList, aSaveStatList);
    }

//----------------------------------------------------------------------------
//      グローバル・バージョン番号テーブル名処理
//----------------------------------------------------------------------------

    {
        writePreElement();
        AutoRestoreSave aAutoRestoreSave(*this, emOrder, true);

        // 型種別
        writePreElement();
        saveControl(etkGlobalTable);

        // テーブル名
        writePreElement();
        saveControl(mGlobalVersionNoTable->getTableName());
    }

//----------------------------------------------------------------------------
//      プリミティブ(派生Serializer)処理
//----------------------------------------------------------------------------

//      ---<<< シリアライザ用 >>>---

    writeMetaPrimitive();

//----------------------------------------------------------------------------
//      enum型処理
//----------------------------------------------------------------------------

    writeMetaEnum(aVersionNoLists, mTypeInfoList, aSaveStatList);

//----------------------------------------------------------------------------
//      クラス
//----------------------------------------------------------------------------

    UniqueNameSet   aUniqueNameSet;

    writeMetaClass(aVersionNoLists, mTypeInfoList, aSaveStatList, aUniqueNameSet);
}

//----------------------------------------------------------------------------
//      保存判定
//          トップ・レベルなら保存先が一致するなら保存マーク
//          保存対象がクラスなら、全てのenum型とクラス要素をチェックし
//              保存対象なら、保存マーク
//                  enum型は保存先が一致するなら保存対象
//                  クラスは保存先が一致するなら保存対象
//----------------------------------------------------------------------------

void BaseSerializer::judgeWriteTypes
(
    std::size_t iTypeIndex,
    BaseTypeInfo* iTypeInfo,
    TypeInfoListImpl const& iTypeInfoList,
    std::vector<SaveStat>& ioSaveStatList
)
{
    // 既に保存済なら、処理しない
    if (ioSaveStatList[iTypeIndex] == essSaved)
return;

    ioSaveStatList[iTypeIndex]=essSaved;    // 非対象のものは「保存済」とする。

    // enum型は保存する
    if (iTypeInfo->mTypeCategory == etcEnumType)
    {
        ioSaveStatList[iTypeIndex]=essSaving;
return;
    }

    // (enum型と)クラス以外は保存しない
    if (iTypeInfo->mTypeCategory != etcClassType)
return;

    // 保存指定する
    ioSaveStatList[iTypeIndex]=essSaving;

    // 全てのローカル・バージョンの要素について判定する
    for (unsigned aVersionNo=iTypeInfo->getLastVersionNoV(); 1 <= aVersionNo; --aVersionNo)
    {
        for (auto&& element : iTypeInfo->getElementRange(aVersionNo))
        {
            // 保存先が不一致なら非対象
            if (!mDestinations.isMatch(element.getDestinations()))
        continue;

            auto aElementTypeIndex=element.getTypeIndex();
            auto aElementTypeInfo=iTypeInfoList[aElementTypeIndex];

            // ポインタの場合、その被ポイント型で判定する
            if (aElementTypeInfo->mTypeCategory == etcPointerType)
            {
                aElementTypeInfo=aElementTypeInfo->getPointeeTypeInfo();
                aElementTypeIndex=aElementTypeInfo->mTypeIndex;
            }

            // 生配列の場合、その基本型で判定する
            else if (aElementTypeInfo->mTypeCategory == etcArrayType)
            {
                aElementTypeIndex=aElementTypeInfo->getUnderlyingTypeIndex();
                aElementTypeInfo=iTypeInfoList[aElementTypeIndex];
            }
            judgeWriteTypes(aElementTypeIndex, aElementTypeInfo, iTypeInfoList, ioSaveStatList);
        }
    }
}

//----------------------------------------------------------------------------
//      プリミティブ(派生Serializer)のメタ・シリアライズ処理
//----------------------------------------------------------------------------

void BaseSerializer::writeMetaPrimitive()
{
    writePreElement();
    AutoRestoreSave aAutoRestoreSave(*this, emOrder);

    // 型種別
    writePreElement();
    saveControl(etkPrimitive);

    // 各バージョン
    for (unsigned aVersionNo=BaseTypeFunctions::getInstance().getLastVersionNo();
         1 <= aVersionNo;
         --aVersionNo)
    {
        // ローカル・バージョン毎の塊
        writePreElement();
        AutoRestoreSave aAutoRestoreSave2(*this, emOrder);

        // ローカル・バージョン番号
        writePreElement();
        saveControl(aVersionNo);

        // グローバル・バージョン番号
        writePreElement();
        unsigned aGlobalVersionNo=mGlobalVersionNoTable->
            getGlobalVersionNo(aVersionNo, BaseTypeFunctions::getInstance().getStdTypeIndex());
        saveControl(aGlobalVersionNo);

        // シリアライザ名
        writePreElement();
        saveControl(BaseTypeFunctions::getInstance().getSerializerName());
    }
}

//----------------------------------------------------------------------------
//      enum型のメタ・シリアライズ処理
//----------------------------------------------------------------------------

void BaseSerializer::writeMetaEnum
(
    std::vector<VersionNoList> const& iVersionNoLists,
    TypeInfoListImpl const& iTypeInfoList,
    std::vector<SaveStat>& ioSaveStatList
)
{
    for (auto&& aLoop : getRBForIndexer(iTypeInfoList))
    {
        auto aTypeIndex=aLoop.getIndex();
        auto aTypeInfo =aLoop.front();

        // 保存しないものはスキップ
        if (ioSaveStatList[aTypeIndex] != essSaving)
    continue;

        // enum型のみ処理する
        if (aTypeInfo->mTypeCategory != etcEnumType)
    continue;

        // 保存済を記録
        ioSaveStatList[aTypeIndex]=essSaved;

        // enum毎の塊
        writePreElement();
        AutoRestoreSave aAutoRestoreSave(*this, emOrder);

        // 型種別
        writePreElement();
        saveControl(aTypeInfo->getTypeKind());

        for (unsigned aVersionNo=aTypeInfo->getLastVersionNoV(); 1 <= aVersionNo; --aVersionNo)
        {
            // ローカル・バージョン毎の塊
            writePreElement();
            AutoRestoreSave aAutoRestoreSave2(*this, emOrder);

            // ローカル・バージョン番号
            writePreElement();
            saveControl(aVersionNo);

            // グローバル・バージョン番号
            writePreElement();
            unsigned aGlobalVersionNo=mGlobalVersionNoTable->
                getGlobalVersionNo(aVersionNo, aTypeInfo->getStdTypeIndex());
            saveControl(aGlobalVersionNo);

            // enum種別
            writePreElement();
            saveControl(aTypeInfo->getTypeFlags(aVersionNo));

            // 形名
            writePreElement();
            saveControl(aTypeInfo->getTypeName(iVersionNoLists[aGlobalVersionNo-1]));

            // 各シンボル情報を出力
            for (auto&& symbol : aTypeInfo->getElementRange(aVersionNo))
            {
                symbol.writeMetaData(*this);
            }
        }
    }
}

//----------------------------------------------------------------------------
//      クラスのメタ・シリアライズ処理
//----------------------------------------------------------------------------

void BaseSerializer::writeMetaClass
(
    std::vector<VersionNoList> const& iVersionNoLists,
    TypeInfoListImpl const& iTypeInfoList,
    std::vector<SaveStat>& ioSaveStatList,
    UniqueNameSet& ioUniqueNameSet
)
{
    // 型情報出力(追加がなくなるまで繰り返す)
    bool aAgain=false;
    do
    {
        aAgain=false;

        for (auto&& aLoop : getRBForIndexer(iTypeInfoList))
        {
            auto aTypeIndex=aLoop.getIndex();
            auto aTypeInfo =aLoop.front();

//      --- 保存するかどうか判定する(保留する場合含む) ---

            // 保存対象でないなら処理しない
            if (ioSaveStatList[aTypeIndex] != essSaving)
        continue;

            // 全てのローカル・バージョンについて未保存のクラス型要素が残っている ?
            bool aDefer = false;    // 保留フラグ
            for (unsigned aVersionNo=aTypeInfo->getLastVersionNoV(); 1 <= aVersionNo; --aVersionNo)
            {
                for (auto&& element : aTypeInfo->getElementRange(aVersionNo))
                {
                    // 保存先が不一致なら非対象
                    if (!mDestinations.isMatch(element.getDestinations()))
                continue;

                    auto aElementTypeIndex=element.getTypeIndex();
                    auto aElementTypeInfo=iTypeInfoList[aElementTypeIndex];

                    // 生配列の場合、その基本型で判定する
                    if (aElementTypeInfo->mTypeCategory == etcArrayType)
                    {
                        aElementTypeIndex=aElementTypeInfo->getUnderlyingTypeIndex();
                        aElementTypeInfo=iTypeInfoList[aElementTypeIndex];
                    }

                    // 未保存なクラス型要素があるなら保留し、再ループする
                    //  自分を再度処理する必要が有るため、再ループが必要
                    if (ioSaveStatList[aElementTypeIndex] == essSaving)
                    {
                        aAgain=true;
                        aDefer=true;
                    }
                }
            }
            if (aDefer)
        continue;

//      --- 保存したことを記録する ---

            // 保存済を記録
            ioSaveStatList[aTypeIndex]=essSaved;

            // テンプレートなら、同一のUniqueClassについて保存していない時のみ出力する
            if (aTypeInfo->getTypeKind() & etkTemplateFlag)
            {
                auto result=ioUniqueNameSet.emplace(aTypeInfo->getUniqueName());
                // 登録済なら、スキップ
                if (!result.second)
        continue;
            }

//      --- クラス情報保存 ---

            // クラス毎の塊
            writePreElement();
            AutoRestoreSave aAutoRestoreSave(*this, emOrder);

            // 型種別
            writePreElement();
            saveControl(aTypeInfo->getTypeKind());

            for (unsigned aVersionNo=aTypeInfo->getLastVersionNoV(); 1 <= aVersionNo; --aVersionNo)
            {
                // ローカル・バージョン毎の塊
                writePreElement();
                AutoRestoreSave aAutoRestoreSave2(*this, emOrder);

                // ローカル・バージョン番号
                writePreElement();
                saveControl(aVersionNo);

                // グローバル・バージョン番号
                writePreElement();
                unsigned aGlobalVersionNo=mGlobalVersionNoTable->
                    getGlobalVersionNo(aVersionNo, aTypeInfo->getStdTypeIndex());
                saveControl(aGlobalVersionNo);

                // Class種別
                writePreElement();
                saveControl(aTypeInfo->getTypeFlags(aVersionNo));

                // 形名

                // テンプレート(ユニーク名を出力する)
                if (aTypeInfo->getTypeKind() & etkTemplateFlag)
                {
                    writePreElement();
                    saveControl(aTypeInfo->getUniqueName());
            continue;   // 要素は出力しない
                }

                // 通常クラス
                else
                {
                    writePreElement();
                    saveControl(aTypeInfo->getTypeName(iVersionNoLists[aGlobalVersionNo-1]));
                }

                // 手動型は要素を出力しない
                if (aTypeInfo->getTypeKind() & etkManualFlag)
            continue;

                // 各要素情報を出力
                for (auto&& element : aTypeInfo->getElementRange(aVersionNo))
                {
                    // 保存先が不一致なら非対象
                    if (!mDestinations.isMatch(element.getDestinations()))
                continue;

                    element.writeMetaData(*this, &iVersionNoLists[aGlobalVersionNo-1]);
                }
            }
        }
    }
    while(aAgain);
}

// ***************************************************************************
//      メタ・デシリアライズ処理用基底クラス
//          クラス定義生成に必要な情報の受け取り
//          クラス定義生成
// ***************************************************************************

struct MetaDeserializerBase
{
protected:
    MetaDeserializerBase(std::ostream& iOStream) : mOStream(iOStream) { }
    std::ostream&   mOStream;

    bool            mIsLastVersion;         // 最新版処理中
    bool            mIsDeferedLast;         // 最新版保留中
    std::string     mGlobalTableName;       // グローバル・バージョン番号テーブル名

    TypeKind        mTypeKind;              // 型種別
    std::string     mTypeName;              // 形名
    unsigned        mVersionNo;             // ローカル・バージョン番号
    unsigned        mGlobalVersionNo;       // グローバル・バージョン番号
    unsigned        mLastGlobalVersionNo;   // 最新版のグローバル・バージョン番号
    unsigned        mTypeFlags;             // enum/クラス種別フラグ
    unsigned        mNamespaceCount;        // namespaceの多重度

//      ---<<< プリミティブ(シリアライザ)用 >>>---

    std::size_t     mPrimitiveIndex;        // mGlobalVersionNoTableのインデックス
                                            //  最後のものが設定される。
                                            //  全て同じものなので、最後である必要は特にない。
    unsigned mSerializerVersionNoMeta;      // メタ処理時のシリアライザ・バージョン番号

//      ---<<< enum用 >>>---

    unsigned openNamespace(std::string& ioTypeName);
    void closeNamespace(unsigned iNamespaceCount);

    struct EnumSymbol
    {
        std::vector<std::string> mSymbols;
        std::vector<std::string> mValues;
        std::string              mPrevValue;
        EnumSymbol
        (
            std::vector<std::string>&& iSymbols,
            std::vector<std::string>&& iValues,
            std::string&&              iPrevValue
        ) : mSymbols(iSymbols), mValues(iValues), mPrevValue(iPrevValue)
        { }
    };
    std::vector<EnumSymbol> mEnumSymbols;
    std::string             mBaseType;

    void    generateEnum();

//      ---<<< クラス用 >>>---

    typedef std::vector<std::size_t>    ArrayExtents;
    struct ClassElement
    {
        std::string     mName;          // 要素名
        std::string     mNextName;      // 次バージョンの要素名
        TypeKind        mTypeKind;      // 型種別
        std::string     mTypeName;      // 型名
        ArrayExtents    mArrayExtents;  // 配列要素数の列
        bool            mIsArrayPointer;// 配列へのポインタ
        unsigned        mVersionNo;     // バージョン番号
        TrackingMode    mTrackingMode;  // オブジェクト追跡指定

        ClassElement
        (
            std::string const&  iName,
            std::string const&  iNextName,
            TypeKind            iTypeKind,
            std::string const&  iTypeName,
            ArrayExtents&       iArrayExtent,
            bool                iIsArrayPointer,
            unsigned            iVersionNo,
            TrackingMode        iTrackingMode
        ) : mName(iName),
            mNextName(iNextName),
            mTypeKind(iTypeKind),
            mTypeName(iTypeName),
            mArrayExtents(iArrayExtent),
            mIsArrayPointer(iIsArrayPointer),
            mVersionNo(iVersionNo),
            mTrackingMode(iTrackingMode)
        { }
    };
    std::vector<ClassElement> mClassElements;   // 次バージョンの要素リスト

    void generateClass
    (
        std::vector<ClassElement>& iClassElements,
        unsigned iVersionNo,
        bool iIsLastVersion
    );

//      ---<<< グローバル・バージョン番号テーブル >>>---

    struct LocalVersionNoList
    {
        std::string             mTypeName;
        std::vector<unsigned>   mVersions;
    };
    std::vector<LocalVersionNoList> mMetaGlobalVersionNoTable;
    void generateGlobalVersionNoTable();
};

// ***************************************************************************
//      メタ・デシリアライズ処理用クラス
//          クラス定義生成情報をメタ・シリアライズ・データから回復
//          クラス定義生成を呼び出す
// ***************************************************************************

struct MetaDeserializer : public MetaDeserializerBase
{
    MetaDeserializer(BaseSerializer& iSerializer, std::ostream& iOStream) : 
        MetaDeserializerBase(iOStream), mSerializer(iSerializer)
    { }

    BaseSerializer& mSerializer;

//----------------------------------------------------------------------------
//      メタ・データ回復実体
//----------------------------------------------------------------------------

    void readMetaData()
    {
        // 型ループ
        while(mSerializer.readPreElement())
        {
            BaseSerializer::AutoRestoreLoad aAutoRestoreLoad(mSerializer, emOrder);

            // 型種別
            mSerializer.readPreElement();
            std::underlying_type<TypeKind>::type    aTypeKind;
            mSerializer.loadControl(aTypeKind);
            mTypeKind=static_cast<TypeKind>(aTypeKind);
//std::cout << "mTypeKind=" << mTypeKind << "\n";

            // グローバル・バージョン番号テーブル名
            if (mTypeKind == etkGlobalTable)
            {
                // 形名
                mSerializer.readPreElement();
                mSerializer.loadControl(mGlobalTableName);
//std::cout << "      mGlobalTableName=" << mGlobalTableName << "\n";
        continue;
            }

            // ローカル・バージョンのループ
            LocalVersionNoList  aLocalVersionNoList;
            mIsLastVersion=true;
            mIsDeferedLast=false;
            unsigned aPrevGlobalVersionNo=0;
            while(mSerializer.readPreElement())
            {
                BaseSerializer::AutoRestoreLoad aAutoRestoreLoad2(mSerializer, emOrder);

                // ローカル・バージョン番号
                mSerializer.readPreElement();
                mSerializer.loadControl(mVersionNo);
//std::cout << "    mVersionNo=" << mVersionNo << "\n";

                // 対応するグローバル・バージョン番号
                mSerializer.readPreElement();
                mSerializer.loadControl(mGlobalVersionNo);
//std::cout << "    mGlobalVersionNo=" << mGlobalVersionNo << "\n";

                if (mIsLastVersion) mLastGlobalVersionNo=mGlobalVersionNo;

//      ---<<< プリミティブ処理 >>>---

                if (mTypeKind == etkPrimitive)
                {
                    readMetaPrimitive();
                    mPrimitiveIndex=mMetaGlobalVersionNoTable.size();
//std::cout << "    mPrimitiveIndex=" << mPrimitiveIndex << "\n";
                }

//      ---<<< enum型処理 >>>---

                else if (mTypeKind == etkEnum)
                {
                    readMetaEnum();
                }

//      ---<<< クラス処理 >>>---

                else if ((mTypeKind & etkClassFlag))
                {
                    readMetaClass();
                }

//      ---<<< グローバル・バージョン番号テーブル生成 >>>---

                if (mIsLastVersion)
                {
                    aLocalVersionNoList.mTypeName = mTypeName;
                    aLocalVersionNoList.mVersions.resize(mGlobalVersionNo, 0);
//std::cout << "    mTypeName=" << mTypeName << " mGlobalVersionNo=" << mGlobalVersionNo << "\n";
                }
                // グローバル・バージョン番号が同じなら上書きしない
                if (aPrevGlobalVersionNo != mGlobalVersionNo)
                {
                    aPrevGlobalVersionNo=mGlobalVersionNo;
                    for (unsigned i=0; i < mGlobalVersionNo; ++i)
                    {
                        aLocalVersionNoList.mVersions[i]=mVersionNo;
//std::cout << "    aLocalVersionNoList.mVersions[" << i << "]=" << mVersionNo << "\n";
                    }
                }

                mIsLastVersion=false;
            }
#if 0
std::cout << "aLocalVersionNoList : " 
          << aLocalVersionNoList.mVersions.size() << "\n    ";
std::cout << aLocalVersionNoList.mTypeName << " : ";
for (auto i : aLocalVersionNoList.mVersions) std::cout << i << " ";
std::cout << " mPrimitiveIndex=" << mPrimitiveIndex << "\n";
#endif
            // 完全自動型は追加しない
            if ((mTypeKind & etkClassFlag) && (mTypeFlags & ecfFullAuto))
        continue;

            mMetaGlobalVersionNoTable.push_back(aLocalVersionNoList);
        }
        generateGlobalVersionNoTable();
    }

//----------------------------------------------------------------------------
//      プリミティブ(派生Serializer)のメタ・デシリアライズ処理
//----------------------------------------------------------------------------

private:
    void readMetaPrimitive()
    {
//std::cout << "  readMetaPrimitive()\n";

        // 形名
        mSerializer.readPreElement();
        mSerializer.loadControl(mTypeName);
//std::cout << "      mTypeName=" << mTypeName << "\n";
    }

//----------------------------------------------------------------------------
//      enum型のメタ・デシリアライズ処理
//----------------------------------------------------------------------------

    THEOLIZER_INTERNAL_DISABLE_WARNING_SIGN_COMPARE()
    void readMetaEnum()
    {
//std::cout << "  readMetaEnum()\n";

        // enum種別
        mSerializer.readPreElement();
        mSerializer.loadControl(mTypeFlags);
//std::cout << "    mTypeFlags=" << mTypeFlags << "\n";

        // 形名
        mSerializer.readPreElement();
        mSerializer.loadControl(mTypeName);
//std::cout << "    mTypeName=" << mTypeName << "\n";

        long long aMin = 0;
        long long aMax = 0;
        unsigned long long aUMax = 0;

        // シンボル・リスト回復
        mEnumSymbols.clear();
        while(mSerializer.readPreElement())
        {
            std::vector<std::string> aSymbols;
            std::vector<std::string> aValues;

            // シンボル名群回復
            {
                BaseSerializer::AutoRestoreLoad aAutoRestoreLoad(mSerializer, emOrder);
                while(mSerializer.readPreElement())
                {
                    std::string  aSymbol;
                    mSerializer.loadControl(aSymbol);
                    aSymbols.push_back(aSymbol);
                }
            }

            // シンボル値群回復
            mSerializer.readPreElement();
            {
                BaseSerializer::AutoRestoreLoad aAutoRestoreLoad(mSerializer, emOrder);
                while(mSerializer.readPreElement())
                {
                    if (!(mTypeFlags & eefUnsigned))
                    {
                        long long   aValue;
                        mSerializer.loadControl(aValue);
                        aValues.push_back(std::to_string(aValue));
                        if (aValue < aMin) aMin=aValue;
                        if (aMax < aValue) aMax=aValue;
                    }
                    else
                    {
                        unsigned long long  aValue;
                        mSerializer.loadControl(aValue);
                        aValues.push_back(std::to_string(aValue));
                        if (aUMax < aValue) aUMax=aValue;
                    }
                }
            }

            // 次バージョン・シンボル値回復
            std::string aNextValueStr;
            mSerializer.readPreElement();
            if (!(mTypeFlags & eefUnsigned))
            {
                long long   aNextValue;
                mSerializer.loadControl(aNextValue);
                aNextValueStr=std::to_string(aNextValue);
            }
            else
            {
                unsigned long long  aNextValue;
                mSerializer.loadControl(aNextValue);
                aNextValueStr=std::to_string(aNextValue);
            }

            mEnumSymbols.emplace_back
            (
                std::move(aSymbols),
                std::move(aValues),
                std::move(aNextValueStr)
            );
        }

        // 基底型判定
        mBaseType.clear();
        if (!(mTypeFlags & eefUnsigned))
        {
            if ((aMin < std::numeric_limits<long>::min())
             || (aMax > std::numeric_limits<long>::max()))
            {
                mBaseType="long long";
            }
            else if ((aMin < std::numeric_limits<int>::min())
                  || (aMax > std::numeric_limits<int>::max()))
            {
                mBaseType="long";
            }
        }
        else
        {
            if (aMax > std::numeric_limits<unsigned long>::max())
            {
                mBaseType="unsigned long long";
            }
            else
            if (aMax > std::numeric_limits<unsigned int>::max())
            {
                mBaseType="unsigned long";
            }
            else
            {
                mBaseType="unsigned";
            }
        }

        // enum定義出力
        generateEnum();
    }

//----------------------------------------------------------------------------
//      クラスのメタ・デシリアライズ処理
//          最新版は、前バージョンの要素名生成のため、
//          １つ前のバージョン獲得後に呼び出す
//----------------------------------------------------------------------------

    void readMetaClass()
    {
//std::cout << "  readMetaClass()\n";

        // クラス種別
        mSerializer.readPreElement();
        mSerializer.loadControl(mTypeFlags);
//std::cout << "    mTypeFlags=" << mTypeFlags << "\n";

        // 形名
        mSerializer.readPreElement();
        mSerializer.loadControl(mTypeName);
//std::cout << "    mTypeName=" << mTypeName << "\n";

        // 要素リスト
        std::vector<ClassElement> aClassElements;

        // 要素リスト回復(手動の時は存在しない)
        if (!(mTypeKind & etkManualFlag))
        {
            while(mSerializer.readPreElement())
            {
                BaseSerializer::AutoRestoreLoad aAutoRestoreLoad(mSerializer, emOrder);

                // 要素名
                std::string     aName;
                mSerializer.readPreElement();
                mSerializer.loadControl(aName);
//std::cout << "    aName=" << aName << "\n";

                // 次バージョンの要素名
                std::string     aNextName;
                mSerializer.readPreElement();
                mSerializer.loadControl(aNextName);
//std::cout << "    aNextName=" << aNextName << "\n";

                // 型種別
                std::underlying_type<TypeKind>::type    aTypeKind;
                mSerializer.readPreElement();
                mSerializer.loadControl(aTypeKind);
//std::cout << "    aTypeKind=" << aTypeKind << "\n";

                // 型名
                std::string     aTypeName;
                mSerializer.readPreElement();
                mSerializer.loadControl(aTypeName);

                // バージョン番号
                unsigned        aVersionNo;
                mSerializer.readPreElement();
                mSerializer.loadControl(aVersionNo);
//std::cout << "    aVersionNo=" << aVersionNo << "\n";

                // オブジェクト追跡指定
                std::underlying_type<TrackingMode>::type    aTrackingMode;
                mSerializer.readPreElement();
                mSerializer.loadControl(aTrackingMode);
//std::cout << "    aTrackingMode=" << aTrackingMode << "\n";

                // 配列なら、その要素数獲得
                ArrayExtents aArrayExtents;
                bool aIsArrayPointer;
//std::cout << "    aTypeName=" << aTypeName << "\n";
                aTypeName=getExtentTypeName
                    (
                        aTypeName,
                        aArrayExtents,
                        aIsArrayPointer
                    );

                // プリミティブ名をC++名へ変換する
                mSerializerVersionNoMeta=mMetaGlobalVersionNoTable[mPrimitiveIndex].
                    mVersions[mGlobalVersionNo-1];
                aTypeName=convertTypeName(aTypeName);

                // 要素リストへ追加
                aClassElements.emplace_back
                (
                    aName,
                    aNextName,
                    static_cast<TypeKind>(aTypeKind),
                    aTypeName,
                    aArrayExtents,
                    aIsArrayPointer,
                    aVersionNo,
                    static_cast<TrackingMode>(aTrackingMode)
                );
            }
        }

        // 手動型とテンプレートは定義生成しない
        if ((mTypeKind & etkManualFlag) || (mTypeFlags & ecfFullAuto))
    return;

        // クラス定義出力

        // バージョンが1つしかない時
        if ((mVersionNo == 1) && mIsLastVersion)
        {
            generateClass(aClassElements, mVersionNo, true);
        }
        // バージョンが複数あり、かつ、最新版
        else if (mIsLastVersion)
        {
            swap(mClassElements, aClassElements);
            mIsDeferedLast=true;
        }
        // バージョンが複数あり、かつ、非最新版
        else
        {
            if (mIsDeferedLast)
            {
                mIsDeferedLast=false;
                swap(mClassElements, aClassElements);
                // swapの結果下記となる
                //  aClassElements : 最新版
                //  mClassElements : １つ前の版
                generateClass(aClassElements, mVersionNo+1, true);
                // 内部でswapされるので下記となっている
                //  mClassElements : 最新版
                //  aClassElements : １つ前の版
            }
            generateClass(aClassElements, mVersionNo, false);
        }
    }

//----------------------------------------------------------------------------
//      テンプレート・パラメータ読み飛ばし
//----------------------------------------------------------------------------

    std::size_t skipTemplateParameter(std::string const& iTypeName)
    {
        std::size_t pos=iTypeName.find('<');
        if (pos != std::string::npos)
        {
            unsigned cnt=1;
            while (cnt)
            {
                pos=iTypeName.find_first_of("><", pos+1);
                THEOLIZER_INTERNAL_ASSERT(pos != std::string::npos, THEOLIZER_INTERNAL_BUG);
                if (iTypeName[pos] == '<') {
                    ++cnt;
                } else {
                    --cnt;
                }
            }
            ++pos;
        }
        else
        {
            pos=0;
        }

        return pos;
    }

//----------------------------------------------------------------------------
//      型名から配列情報を獲得し、基本型名を返却する(変換無し)
//          シリアライズ・データ上の型を解析する
//              プリミティブなら、C++言語の型名へ変換する
//              生配列なら、配列部分を分離し、oArrayExtentsへ設定する。
//          戻り値
//              生配列なら、その基本型名
//              プリミティブは、C++言語型名
//----------------------------------------------------------------------------

    std::string getExtentTypeName
    (
        std::string const& iTypeName,
        ArrayExtents& oArrayExtents,
        bool& oIsArrayPointer
    )
    {
        // テンプレート・パラメータ読み飛ばし
        std::size_t pos=skipTemplateParameter(iTypeName);

        // 生配列処理
        std::size_t aExtentStart=iTypeName.find('[', pos);
        for (std::size_t pos2=aExtentStart;
             pos2 != std::string::npos;
             pos2=iTypeName.find('[', pos2+1))
        {
            std::size_t extent=static_cast<std::size_t>(std::stoull(iTypeName.substr(pos2+1)));
            oArrayExtents.emplace_back(extent);
        }
        oIsArrayPointer=(oArrayExtents.size() != 0) && (iTypeName.back() == '*');

        std::string aUnderlyingName=iTypeName.substr(0, aExtentStart);
        return aUnderlyingName;
    }

//----------------------------------------------------------------------------
//      型名内のプリミティブ名をC++名へ変換する
//----------------------------------------------------------------------------

    std::string convertTypeName
    (
        std::string const& iTypeName
    )
    {
        // テンプレート・パラメータ読み飛ばし
        std::size_t pos=skipTemplateParameter(iTypeName);
        std::size_t aExtentStart=iTypeName.find('[', pos);

        // ポインタ処理
        if (iTypeName.back() == '*')
        {   // " *"の空白は、clangの表記に合わせるため
            // 配列へのポインタ処理
            if (aExtentStart != std::string::npos)
            {
    return convertTypeName(iTypeName.substr(0, aExtentStart))
        +" (*)"+iTypeName.substr(aExtentStart, iTypeName.size()-aExtentStart-1);
            }

            // 通常のポインタ
            else
            {
    return convertTypeName(iTypeName.substr(0, iTypeName.size()-1))+" *";
            }
        }

        // テンプレート名なら、引数を変換する
        auto start=iTypeName.find('<');
        if (start != std::string::npos)
        {
            ++start;
            std::string  ret=iTypeName.substr(0, start);
            while(1)
            {
                auto end=iTypeName.find_first_of(",><", start);
                THEOLIZER_INTERNAL_ASSERT(end != std::string::npos, THEOLIZER_INTERNAL_BUG);

                // 更にテンプレート
                if (iTypeName[end] == '<')
                {
                    end=iTypeName.find('>', end);
                    THEOLIZER_INTERNAL_ASSERT(end != std::string::npos,
                        THEOLIZER_INTERNAL_BUG);
                    ret += convertTypeName(iTypeName.substr(start, end+1-start));

                    end=iTypeName.find_first_of(",>", end);
                    THEOLIZER_INTERNAL_ASSERT(end != std::string::npos,
                        THEOLIZER_INTERNAL_BUG);
                }
                // 通常の型
                else
                {
                    ret += convertTypeName(iTypeName.substr(start, end-start));
                }
                start=end+1;
                ret += iTypeName[end];
                if (iTypeName[end] == '>')
            break;
                ret += ' '; // clangの表記に合わせるため
            }
            // 残り(配列)を追加
            ret += iTypeName.substr(start);
    return ret;
        }

        // C++言語型名へ変換し、必要なら配列定義を追加する
        std::string ret = BaseTypeFunctions::getInstance().getCppName
            (
                iTypeName.substr(0, aExtentStart),
                mSerializerVersionNoMeta
            );
        if (aExtentStart != std::string::npos)
        {
            ret += ' '; // clangの表記に合わせるため
            ret += iTypeName.substr(aExtentStart);
        }
        return  ret;
    }
};

// ***************************************************************************
//      メタ・デシリアライズ
// ***************************************************************************

//----------------------------------------------------------------------------
//      メタ・データ回復
//----------------------------------------------------------------------------

void BaseSerializer::readMetaData()
{
    // メタ・デシリアライズ処理用クラス生成
    MetaDeserializer    aMetaDeserializer(*this, *mOStream);

    // メタ・デシリアライザ呼び出し
    aMetaDeserializer.readMetaData();
}

// ***************************************************************************
//      メタ・デシリアライズ機能群
// ***************************************************************************

//----------------------------------------------------------------------------
//      namespace開始出力
//          戻り値:namespaceのネスト数(0の時、namespace無し)
//          ioTypeNameにはnamespaceを除いた名前が返却される
//----------------------------------------------------------------------------

unsigned MetaDeserializerBase::openNamespace(std::string& ioTypeName)
{
    unsigned aNamespaceCount=0;
    std::size_t start=0;
    std::size_t end=ioTypeName.find("::");
    for ( ;
         end != std::string::npos;
         start=end+2, end=ioTypeName.find("::", start))
    {
        ++aNamespaceCount;
        mOStream << "namespace " << ioTypeName.substr(start, end-start) << "{";
    }
    if (aNamespaceCount)
    {
        mOStream << "\n";
    }
    ioTypeName=ioTypeName.substr(start);

    return aNamespaceCount;
}

//----------------------------------------------------------------------------
//      namespace終了出力
//----------------------------------------------------------------------------

void MetaDeserializerBase::closeNamespace(unsigned iNamespaceCount)
{
    if (iNamespaceCount)
    {
        for (unsigned i=0; i < iNamespaceCount; ++i)
        {
            mOStream << "}";
        }
        mOStream << "\n";
    }
}

//----------------------------------------------------------------------------
//      enum型のメタ・デシリアライズ処理
//----------------------------------------------------------------------------

void MetaDeserializerBase::generateEnum()
{
//      ---<<< 実定義 >>>---

    if (mIsLastVersion)
    {
        mOStream <<
            "//----------------------------------------------------------------------------\n"
            "//      " << mTypeName << "\n"
            "//----------------------------------------------------------------------------\n";

        // namespace処理
        //  enum定義部のみくくる
        std::string aEnumName=mTypeName;
        mNamespaceCount=openNamespace(aEnumName);

        // enum定義
        mOStream << "\nenum " << ((mTypeFlags & eefScoped)?"struct ":"") << aEnumName;
        if (!mBaseType.empty()) {
            mOStream << " : " << mBaseType;
        }
        mOStream << "\n{";

        bool aIsFirst=true;
        for (auto&& symbol : mEnumSymbols)
        {
            if (aIsFirst) {
                aIsFirst=false;
            } else {
                mOStream << ",";
            }
            mOStream << "\n    " << symbol.mSymbols[0];
            if ((symbol.mSymbols.size() >= 2)
             || (symbol.mValues.size() >= 2))
            {
                mOStream << " THEOLIZER_ANNOTATE(ES:";
                if (symbol.mSymbols.size() >= 2)
                {
                    bool aIsFirst2=true;
                    for (std::size_t i=1; i < symbol.mSymbols.size(); ++i)
                    {
                        if (aIsFirst2) {
                            aIsFirst2=false;
                        } else {
                            mOStream << ",";
                        }
                        mOStream << symbol.mSymbols[i];
                    }
                }
                mOStream << ":";
                if (symbol.mValues.size() >= 2)
                {
                    bool aIsFirst2=true;
                    for (std::size_t i=1; i < symbol.mValues.size(); ++i)
                    {
                        if (aIsFirst2) {
                            aIsFirst2=false;
                        } else {
                            mOStream << ",";
                        }
                        mOStream << symbol.mValues[i];
                    }
                }
                mOStream << ")";
            }
            mOStream << " = "    << symbol.mValues[0];
        }
        mOStream << "\n};\n\n";
        closeNamespace(mNamespaceCount);
        mOStream << "THEOLIZER_ENUM(" << mTypeName << "," << mVersionNo << ");\n\n";
    }

//      ---<<< バージョン定義マクロ群 >>>---

    if (mIsLastVersion)
    {
        mOStream << 
            "#ifdef  THEOLIZER_WRITE_CODE // ###### " << mTypeName << " ######\n\n"
            "#define THEOLIZER_GENERATED_LAST_VERSION_NO "
                "THEOLIZER_INTERNAL_DEFINE(kLastVersionNo," << mVersionNo << ")\n"
            "#define THEOLIZER_GENERATED_ENUM_TYPE " << mTypeName << "\n";
    }

    mOStream << 
        "\n//      ---<<< Version." << mVersionNo << " >>>---\n\n"
        "#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,"
            << mVersionNo << ")\n"
        "#define THEOLIZER_GENERATED_ENUM_NAME u8\"" << mTypeName << "\"\n"
        "#define THEOLIZER_GENERATED_SAVE_TYPE "
            << ((mTypeFlags & eefValue)?"estValue\n":"estName\n") << 
        "#define THEOLIZER_GENERATED_BASE_TYPE " << (mBaseType.empty()?"int":mBaseType) << "\n"
        "#define THEOLIZER_GENERATED_ENUM_LIST()\\\n";
    bool aIsFirst=true;
    for (auto&& symbol : mEnumSymbols)
    {
        if (aIsFirst) {
            aIsFirst=false;
        } else {
            mOStream << "\\\n";
        }

        mOStream << "    THEOLIZER_GENERATED_ELEMENT((";
        {
            bool aIsFirst2=true;
            for (std::size_t i=0; i < symbol.mSymbols.size(); ++i)
            {
                if (aIsFirst2) {
                    aIsFirst2=false;
                } else {
                    mOStream << ",";
                }
                mOStream << "u8\"" << symbol.mSymbols[i] << "\"";
            }
        }
        mOStream << "),(";
        {
            bool aIsFirst2=true;
            for (std::size_t i=0; i < symbol.mValues.size(); ++i)
            {
                if (aIsFirst2) {
                    aIsFirst2=false;
                } else {
                    mOStream << ",";
                }
                mOStream << symbol.mValues[i];
            }
        }
        mOStream << ")," << symbol.mPrevValue << ")";
    }
    std::string aDefault="0";
    if (mEnumSymbols.size()) {
        aDefault=mEnumSymbols[0].mValues[0];
    }
    mOStream << "\n"
        "#define THEOLIZER_GENERATED_DEFAULT_VALUE " << aDefault << "\n"
        "#include <theolizer/internal/version_enum.inc>\n"
        "#undef  THEOLIZER_GENERATED_VERSION_NO\n";

    if (mVersionNo == 1)
    {
        mOStream << "\n#endif//THEOLIZER_WRITE_CODE // ###### "
                  << mTypeName << " ######\n\n";
    }
}

//----------------------------------------------------------------------------
//      クラスのメタ・デシリアライズ処理
//----------------------------------------------------------------------------

void MetaDeserializerBase::generateClass
(
    std::vector<ClassElement>& iClassElements,
    unsigned iVersionNo,
    bool iIsLastVersion
)
{
//      ---<<< 実定義 >>>---

    if (iIsLastVersion)
    {
        mOStream <<
            "//----------------------------------------------------------------------------\n"
            "//      " << mTypeName << "\n"
            "//----------------------------------------------------------------------------\n\n";

        // ポインタにて参照しているクラス/enumの前方宣言
        bool aIsForwardDecl=false;
        for (auto&& element : iClassElements)
        {
            if (element.mName[0] == '(')
        continue;

            // ポインタ、かつ、非テンプレートで自動型クラスなら前方宣言
            if (((element.mTypeName.back() == '*') || (element.mIsArrayPointer))
             && ((element.mTypeKind & (etkClassFlag|etkManualFlag|etkTemplateFlag))
              == (etkClassFlag)))
            {
                aIsForwardDecl=true;
                std::string aTypeName = element.mTypeName;
                if (aTypeName.back() == '*')
                {
                    aTypeName.pop_back();
                    if (aTypeName.back() == ' ')
                    {
                        aTypeName.pop_back();
                    }
                }
                mNamespaceCount=openNamespace(aTypeName);

                if (mNamespaceCount) mOStream << "    ";
                mOStream << ((element.mTypeKind & etkManualFlag)?"class ":"struct ")
                         << aTypeName << ";\n";
                closeNamespace(mNamespaceCount);
            }
        }
        if (aIsForwardDecl) mOStream << "\n";

        // namespace処理
        // 侵入型のみ処理する
        std::string aClassName=mTypeName;
        if (!(mTypeKind & etkNonIntrusiveFlag))
        {
            mNamespaceCount=openNamespace(aClassName);
        }
        if (mNamespaceCount) mOStream << "\n";

        mOStream << ((mTypeKind & etkManualFlag)?"class ":"struct ") << aClassName;

        // 基底クラス処理
        bool aIsFirst=true;
        for (auto&& element : iClassElements)
        {
            if (element.mName[0] != '(')
        continue;

            if (aIsFirst)
            {
                mOStream << " : ";
                aIsFirst=false;
            }
            else
            {
                mOStream << ", ";
            }

            mOStream << element.mNextName << ' ' << element.mTypeName;
        }

        mOStream << "\n{\n";

        // メンバ処理
        for (auto&& element : iClassElements)
        {
            if (element.mName[0] == '(')
        continue;

            mOStream << "    " << element.mTypeName << ' ';
            if (element.mIsArrayPointer)
            {
                mOStream << "(*" << element.mName << ")";
            }
            else
            {
                mOStream << element.mName;
            }
            for (auto extent : element.mArrayExtents)
            {
                mOStream << '[' << extent << ']';
            }

            // １つ前のバージョンから要素名変更チェック
            std::string aPrevName;
            auto found = 
                find_if
                (
                    mClassElements.begin(),
                    mClassElements.end(),
                    [&](ClassElement const& iRhs)
                    {
                       return element.mName == iRhs.mNextName;
                    }
                );
            // 同じ要素名が見つかり、かつ、名前が異なるかチェック
            if ((found != mClassElements.end()) && (element.mName != found->mName))
            {
                aPrevName=found->mName;
            }

            if ((!aPrevName.empty()) || (element.mTrackingMode != etmDefault))
            {
                mOStream << " THEOLIZER_ANNOTATE(FS:" << aPrevName << "<>";
                switch (element.mTrackingMode)
                {
                case etmDefault:mOStream << ")";        break;
                case etmPointee:mOStream << "Pointee)"; break;
                case etmOwner:  mOStream << "Owner)";   break;
                }
            }
            mOStream << ";\n";
        }

        // 侵入型処理
        if (!(mTypeKind & etkNonIntrusiveFlag))
        {
            if (mTypeFlags & ecfOrder)
            {
                mOStream << "\n    THEOLIZER_INTRUSIVE_ORDER";
            }
            else
            {
                mOStream << "\n    THEOLIZER_INTRUSIVE";
            }
            mOStream << "(CS, (" << mTypeName << "), " << iVersionNo << ");\n";
        }

        mOStream << "};\n\n";

        // 非侵入型処理
        if ((mTypeKind & etkNonIntrusiveFlag) && !(mTypeFlags & ecfFullAuto))
        {
            mOStream << 
                "THEOLIZER_NON_INTRUSIVE_ORDER((" << aClassName << ")," << iVersionNo << ");\n\n";
        }
    }


//      ---<<< バージョン定義マクロ群 >>>---

    if (iIsLastVersion)
    {
        mOStream << 
            "#ifdef  THEOLIZER_WRITE_CODE // ###### " << mTypeName << " ######\n\n"
            "#define THEOLIZER_GENERATED_LAST_VERSION_NO "
                "THEOLIZER_INTERNAL_DEFINE(kLastVersionNo," << iVersionNo << ")\n";

        // 完全自動型
        if (mTypeFlags & ecfFullAuto)
        {
            mOStream << "#define THEOLIZER_GENERATED_FULL_AUTO " << mTypeName << "\n";
        }
        // 通常型
        else
        {
            mOStream << "#define THEOLIZER_GENERATED_CLASS_TYPE " << mTypeName << "\n\n";
        }
    }

    mOStream << 
        "//      ---<<< Version." << iVersionNo << " >>>---\n\n"
        "#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,"
            << iVersionNo << ")\n"
        "#define THEOLIZER_GENERATED_CLASS_NAME()\\\n"
            "    THEOLIZER_INTERNAL_CLASS_NAME((u8\"" << mTypeName << "\"))\n"
        "#define THEOLIZER_GENERATED_ELEMENT_MAP "
            << ((mTypeFlags & ecfOrder)?"emOrder":"emName");

    bool aIsBaseFirst=true;
    bool aIsElementFirst=true;
    for (auto&& element : iClassElements)
    {
        // 次バージョンでの削除判定
        std::string  aDel;
        if (!iIsLastVersion)
        {
            // 基底クラス
            if (element.mName[0] == '(')
            {
                auto found = 
                    find_if
                    (
                        mClassElements.begin(),
                        mClassElements.end(),
                        [&](ClassElement const& iRhs)
                        {
                           return element.mName == iRhs.mName;
                        }
                    );
                if (found == mClassElements.end()) {
                    aDel="_DEL";
                }
            }
            // 要素
            else
            {
                if (element.mNextName.empty()) {
                    aDel="_DEL";
                }
            }
        }

        // 基底クラス
        if (element.mName[0] == '(')
        {
            if (aIsBaseFirst)
            {
                aIsBaseFirst=false;
                mOStream << "\n#define THEOLIZER_GENERATED_BASE_LIST()\\\n";
            }
            else
            {
                mOStream << " THEOLIZER_GENERATED_SEP\\\n";
            }

            // 手動型
            if (element.mTypeKind & etkManualFlag)
            {
                mOStream << "    THEOLIZER_INTERNAL_BASE_N" << aDel << "(" 
                         << element.mNextName
                         << ",etmDefault,(" << element.mTypeName << "))";
            }
            // 自動型
            else
            {
                // 侵入型
                if (!(element.mTypeKind & etkNonIntrusiveFlag))
                {
                    mOStream << "    THEOLIZER_INTERNAL_BASE_KI" << aDel << "(";
                }
                // 非侵入型
                else
                {
                    mOStream << "    THEOLIZER_INTERNAL_BASE_KN" << aDel << "(";
                }
                mOStream << element.mNextName << ",etmDefault,(" << element.mTypeName
                          << ")," << element.mVersionNo << ")";
            }
        }
        // 要素
        else
        {
            if (aIsElementFirst)
            {
                aIsElementFirst=false;
                mOStream << "\n#define THEOLIZER_GENERATED_ELEMENT_LIST()\\\n";
            }
            else
            {
                mOStream << "\\\n";
            }

            // オブジェクト追跡モード
            std::string aTrackingMode = "etmDefault";
            switch (element.mTrackingMode)
            {
            case etmPointee:aTrackingMode="etmPointee";break;
            case etmOwner:  aTrackingMode="etmOwner";  break;
            default:                                   break;
            }

            // Non-keep-step判定(プリミティブ、ポインタ、手動型)
            bool aIsNonKeepStep=false;
            if (element.mTypeKind == etkPrimitive)  aIsNonKeepStep=true;
            if (element.mTypeName.back() == '*')    aIsNonKeepStep=true;
            if (element.mTypeKind & etkManualFlag)  aIsNonKeepStep=true;
            if (element.mIsArrayPointer)            aIsNonKeepStep=true;

            // 非配列
            if (!element.mArrayExtents.size() || element.mIsArrayPointer)
            {
                // 手動型
                if (aIsNonKeepStep)
                {
                    mOStream << "    THEOLIZER_INTERNAL_ELEMENT_N" << aDel << "(" 
                              << element.mName << "," << element.mNextName
                              << "," << aTrackingMode << ",\\\n"
                              << "        (theolizerD::All),\\\n";
                    if (element.mIsArrayPointer)
                    {
                        mOStream << "        (" << element.mTypeName << " (*)";
                        for (auto extent : element.mArrayExtents)
                        {
                            mOStream << '[' << extent << ']';
                        }
                        mOStream << "))";
                    }
                    else
                    {
                        mOStream << "        (" << element.mTypeName << "))";
                    }
                }
                // 自動型
                else
                {
                    // 侵入型
                    if (!(element.mTypeKind & etkNonIntrusiveFlag))
                    {
                        mOStream << "    THEOLIZER_INTERNAL_ELEMENT_KI" << aDel << "(";
                    }
                    // 非侵入型
                    else
                    {
                        mOStream << "    THEOLIZER_INTERNAL_ELEMENT_KN" << aDel << "(";
                    }
                    mOStream << element.mName << "," << element.mNextName
                              << "," << aTrackingMode << ",\\\n"
                              << "        (theolizerD::All),\\\n"
                              << "        (" << element.mTypeName << ")," 
                              << element.mVersionNo << ")";
                }
            }

            // 配列
            else
            {
                // 手動型
                if (aIsNonKeepStep)
                {
                    mOStream << "    THEOLIZER_INTERNAL_ELEMENT_AN" << aDel << "(" 
                              << element.mName << "," << element.mNextName
                              << "," << aTrackingMode << ",\\\n"
                              << "        (theolizerD::All),\\\n"
                              << "        (" << element.mTypeName << ")";
                }
                // 自動型
                else
                {
                    // 侵入型
                    if (!(element.mTypeKind & etkNonIntrusiveFlag))
                    {
                        mOStream << "    THEOLIZER_INTERNAL_ELEMENT_AKI" << aDel << "(";
                    }
                    // 非侵入型
                    else
                    {
                        mOStream << "    THEOLIZER_INTERNAL_ELEMENT_AKN" << aDel << "(";
                    }
                    mOStream << element.mName << "," << element.mNextName
                              << "," << aTrackingMode << ",\\\n"
                              << "        (theolizerD::All),\\\n"
                              << "        (" << element.mTypeName << ")," 
                              << element.mVersionNo;
                }
                for (auto extent : element.mArrayExtents)
                {
                    mOStream << "," << extent;
                }
                mOStream << ")";
            }
        }
    }
    mOStream << "\n"
        "#include <theolizer/internal/version_auto.inc>\n"
        "#undef  THEOLIZER_GENERATED_VERSION_NO\n\n";

    if (iVersionNo == 1)
    {
        closeNamespace(mNamespaceCount);
        mOStream << "#endif//THEOLIZER_WRITE_CODE // ###### "
                  << mTypeName << " ######\n\n";
    }

    // 前バージョン処理に備えて要素リストを残す
    swap(mClassElements, iClassElements);
}

//----------------------------------------------------------------------------
//      グローバル・バージョン番号テーブル生成
//----------------------------------------------------------------------------

void MetaDeserializerBase::generateGlobalVersionNoTable()
{
    mOStream << "\nTHEOLIZER_DEFINE_GLOBAL_VERSION_TABLE(" << mGlobalTableName
             << "," << mGlobalVersionNo << ");\n";

    mOStream << "\n#ifdef  THEOLIZER_WRITE_CODE // ###### Global VersionNo. Table ######\n";

    // グローバル・バージョン番号テーブルのユーザ定義有り
    if (!mGlobalTableName.empty())
    {
        mOStream <<
            "\nnamespace theolizer{namespace internal{\n"
            "namespace global_table{\n"
            "    " << mGlobalTableName << "::" << mGlobalTableName << "()\n"
            "    {\n";

        for (auto&& aLocalVersionNoList : mMetaGlobalVersionNoTable)
        {
            mOStream << "        add(typeid(" << aLocalVersionNoList.mTypeName << ")";
            for (auto i : aLocalVersionNoList.mVersions)
            {
                mOStream << "," << i << "u";
            }
            mOStream << ");\n";
        }

        mOStream <<
            "    }\n"
            "}   // namespace global_table\n"
            "}} // namespace theolizer\n";
    }

    // グローバル・バージョン番号テーブルのユーザ定義無し(デフォルトのみ)
    else
    {
        mOStream << "THEOLIZER_GENERATED_GLOBAL_TABLE();";
    }

    mOStream << "\n#endif//THEOLIZER_WRITE_CODE // ###### Global VersionNo. Table ######\n";
}

//############################################################################
//      End
//############################################################################

}   // namespace internal
}   // namespace theolizer
