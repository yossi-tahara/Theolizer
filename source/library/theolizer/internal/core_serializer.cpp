//############################################################################
//      Theolizerライブラリのコア部(基底シリアライザ)
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
#include "internal.h"

#include <algorithm>
#include <tuple>        // for tie
#include <map>
#include <set>

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
    TypeIndex   mTypeIndex;

    SerializedElementTypeIndex(std::string iName, TypeIndex iTypeIndex) :
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
//      バージョン番号取得
// ***************************************************************************

unsigned BaseSerializer::getLocalVersionNo(TypeIndex iTypeIndex)
{
    unsigned aIndex = iTypeIndex.getIndex();

    // プリミティブなら派生シリアライザのバージョン番号
    if (mTypeInfoList[aIndex]->mTypeCategory == etcPrimitiveType)
    {
        aIndex=getSerializerTypeIndex().getIndex();
    }
    return mGlobalVersionNoTable->getLocalVersionNo(mGlobalVersionNo, aIndex);
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
    AutoRestoreSave aAutoRestoreSave(*this, emOrder, (mCheckMode <= CheckMode::NoTypeCheck));

//----------------------------------------------------------------------------
//      型チェック処理分岐
//----------------------------------------------------------------------------

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
        unsigned aVersionNo = getLocalVersionNo(aLoop.front()->getTypeIndex());
        std::cout << "[" << aLoop.getIndex() << "] "
                  << aLoop.front()->getTypeName(aVersionNo)
                  << " isTopLevel()=" << aLoop.front()->isTopLevel()
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
        for (auto& aLoop : getRBForIndexer(mTypeInfoList))
        {
            auto aIndex =aLoop.getIndex();
            auto aTypeInfo=aLoop.front();
            auto aTypeIndex = aTypeInfo->getTypeIndex();
            unsigned aVersionNo = getLocalVersionNo(aTypeIndex);

            if (aSaveStatList[aIndex] != essSaving)
        continue;
            aSaveStatList[aIndex] = essSaved;

            // TypeCheckの時はClassTypeのみ出力する
            if ((mCheckMode == CheckMode::TypeCheck)
             && (aTypeInfo->mTypeCategory != etcClassType))
        continue;

            // 開始マーク
            writePreElement();
            AutoRestoreSave aAutoRestoreSave2(*this, emOrder, true);

            // TypeIndex
            if (mCheckMode == CheckMode::TypeCheckByIndex)
            {
                writePreElement();
                saveControl(aTypeIndex);
            }

            // TypeName
            writePreElement();
            std::string aTypeName=aTypeInfo->getTypeName(aVersionNo);
            saveControl(aTypeName);
//std::cout << "aTypeName=" << aTypeName << "\n";

            if (aTypeInfo->mTypeCategory != etcClassType)
        continue;

            // バージョン番号と対応方法獲得
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
                    unsigned aElementIndex=aElement.getTypeIndex().getIndex();
                    if (aSaveStatList[aElementIndex] == essIdle)
                    {
                        aSaveStatList[aElementIndex]=essSaving;
                        // 自分より前のものなら、再ループ
                        if (aElementIndex <= aIndex)
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
//std::cout << "    ElementName=" << aElement.getName() << "\n";
                // 保存先一致確認
                if (!mDestinations.isMatch(aElement.getDestinations()))
            continue;

                writePreElement();

                // 名前対応時のみ要素名を記録する
                saveElementName(aElementsMapping, aElement.getName());

                // 要素の形名、もしくは、TypeIndex
                TypeIndex aElementTypeIndex=aElement.getTypeIndex();
                unsigned aElementIndex = aElementTypeIndex.getIndex();
                if (mCheckMode == CheckMode::TypeCheck)
                {
                    // 処理中要素の型名取出し
                    BaseTypeInfo* aElementInfo = mTypeInfoList[aElementIndex];
                    unsigned aVersionNo2 = getLocalVersionNo(aElementInfo->getTypeIndex());
                    std::string aElementTypeName=aElementInfo->getTypeName(aVersionNo2);
                    saveControl(aElementTypeName);
                }
                else
                {
                    saveControl(aElementTypeIndex);
                }

                // まだ保存されてないなら、保存する
                if (aSaveStatList[aElementIndex] == essIdle)
                {
                    aSaveStatList[aElementIndex]=essSaving;
                    // 自分より前のものなら、再ループ
                    if (aElementIndex <= aIndex)
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

std::string BaseSerializer::getTypeName(TypeIndex iTypeIndex)
{
    auto aTypeInfo = mTypeInfoList[iTypeIndex.getIndex()];
    unsigned aVersionNo = getLocalVersionNo(iTypeIndex);
    return aTypeInfo->getTypeName(aVersionNo);
}

void BaseSerializer::saveProcessStart(TypeIndex iTypeIndex)
{
    if (mDoCheck)
    {
        saveGroupStart(true);

        switch(mCheckMode)
        {
        case CheckMode::InMemory:
        case CheckMode::TypeCheckInData:
        case CheckMode::NoTypeCheck:
            break;

        case CheckMode::TypeCheck:
            {
                writePreElement();
                saveControl(getTypeName(iTypeIndex));
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
}

//----------------------------------------------------------------------------
//      後処理
//----------------------------------------------------------------------------

void BaseSerializer::saveProcessEnd()
{
    if (mDoCheck)
    {
        saveGroupEnd(true);
    }
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

    void add(std::string const& iTypeName, TypeIndex iTypeIndex)
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

void BaseSerializer::createTypeNameMap()
{
//std::cout << "createTypeNameMap()\n";
    // TypeIndexとInMemoryの時は生成不要
    if ((mCheckMode == CheckMode::TypeCheckByIndex)
     || (mCheckMode == CheckMode::InMemory))
return;

    mTypeNameMap=std::unique_ptr<TypeNameMap>(new TypeNameMap);

    for (auto const& aIndexer : getRBForIndexer(mTypeInfoList))
    {
        TypeIndex aTypeIndex = aIndexer.front()->getTypeIndex();
        unsigned aVersionNo = getLocalVersionNo(aTypeIndex);
        mTypeNameMap->add(aIndexer.front()->getTypeName(aVersionNo), aTypeIndex);
//std::cout << "    [" << aIndexer.getIndex() << "] "
//          << aIndexer.front()->getTypeName(aVersionNo) << "\n";
    }
}

// ***************************************************************************
//      トップ・レベル回復前後処理
// ***************************************************************************

//----------------------------------------------------------------------------
//      前処理
//----------------------------------------------------------------------------

TypeIndexList* BaseSerializer::loadProcessStart(TypeIndex iTypeIndex)
{
    TypeIndexList* ret = nullptr;

    if (mDoCheck)
    {

        loadGroupStart(true);

        switch(mCheckMode)
        {
        case CheckMode::InMemory:
        case CheckMode::TypeCheckInData:
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
                if (iTypeIndex.isValid())
                {
                    bool aIsMatch=false;
                    for (auto aTypeIndex : aTypeIndexList)
                    {
                        if (aTypeIndex.getIndex() == iTypeIndex.getIndex())
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
                else
                {
                    ret = &aTypeIndexList;
                }
            }
            break;

        case CheckMode::TypeCheckByIndex:
            if (!readPreElement())
            {
                THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
            }

            {
                TypeIndex aTypeIndex;
                loadControl(aTypeIndex);
                if (iTypeIndex.isValid())
                {
                    if (!isMatchTypeIndex(aTypeIndex, iTypeIndex))
                    {
                        THEOLIZER_INTERNAL_DATA_ERROR(u8"Unmatch type.");
                    }
                }
                else
                {
#if 1   // ヘッダ処理を実装するまでの仮実装(仮なのでかなりいい加減だがデバッグには使える)
                    static TypeIndexList    aTemp;  // 本来staticではダメだが、仮実装なので手抜き
                    aTemp.clear();
                    aTemp.push_back(aTypeIndex);
                    ret = &aTemp;
#else
                    auto& aElementType = mSerializedTypeListI->at(aTypeIndex);
                    ret = aElementType.mProgramTypeIndex;
#endif
                }
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

    return ret;
}

//----------------------------------------------------------------------------
//      後処理
//----------------------------------------------------------------------------

void BaseSerializer::loadProcessEnd()
{
    if (mDoCheck)
    {
        loadGroupEnd(true);
    }
}

//############################################################################
//      現在処理中のSerializer
//############################################################################

// ***************************************************************************
//      現在処理中のSerializerの状態獲得
// ***************************************************************************

thread_local BaseSerializer* xNowSerializer=nullptr;

bool isLastVersion()
{
    return xNowSerializer->isLastVersion();
}

bool isSaver()
{
    return xNowSerializer->isSaver();
}

unsigned& getUpVersionCount()
{
    return xNowSerializer->mUpVersionCount;
}

// ***************************************************************************
//      API境界にてxNowSerializer設定／回復
// ***************************************************************************

ApiBoundarySerializer::ApiBoundarySerializer
(
    BaseSerializer* iBaseSerializer,
    BaseAdditionalInfo* iAdditionalInfo,
    bool iConstructor
) noexcept : ApiBoundary(iAdditionalInfo, iConstructor)
{
    mSerializerBack=xNowSerializer;
    xNowSerializer=iBaseSerializer;
}

ApiBoundarySerializer::ApiBoundarySerializer
(
    ApiBoundarySerializer&& iApiBoundarySerializer
) noexcept : ApiBoundary(std::move(iApiBoundarySerializer))
{ }

ApiBoundarySerializer::~ApiBoundarySerializer()
{
    xNowSerializer=mSerializerBack;
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
    mDestinations(iDestinations),
    mIsSaver(iIsSaver),
    mNoThrowException(iNoThrowException),
    mGlobalVersionNoTable(iGlobalVersionNoTable),
    mGlobalVersionNo(iGlobalVersionNo),
    mLastGlobalVersionNo(iLastGlobalVersionNo),
    mCharIsMultiByte(false),
    mTypeInfoList(TypeInfoList::getInstance().getList()),
    mTypeIndexCount(mTypeInfoList.size()),
    mRequireClearTracking(false),
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
    mDoCheck(true),
    mSerializedTypeListI(new SerializedTypeListI),
    mSerializedTypeListN(new SerializedTypeListN),
    mTypeNameMap(nullptr),
    mTypeIndexTable(),
    mObjectId(0),
    mSerializeList(nullptr),
    mDeserializeList(nullptr)
{
    // エラー情報登録準備
    theolizer::internal::ApiBoundarySerializer aApiBoundary(this, &mAdditionalInfo);

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
    theolizer::internal::ApiBoundarySerializer aApiBoundary(this, &mAdditionalInfo);

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
     && mRequireClearTracking
     && (mCheckMode != CheckMode::InMemory))
    {
        THEOLIZER_INTERNAL_ABORT
            (u8"For object tracking, please call clearTracking() before destructing serializer.");
    }
}

// ***************************************************************************
//      シリアライズ・データの指定TypeIndexのクラスの指定番目の要素の名前獲得
// ***************************************************************************

std::string BaseSerializer::getDataElementName(TypeIndex iDataTypeIndex, std::size_t iDataIndex)
{
    std::string ret;
    unsigned aIndex = iDataTypeIndex.getIndex();
    switch(mCheckMode)
    {
    case CheckMode::TypeCheck:
        ret=mSerializedTypeListN->at(aIndex).mSerializedElementList[iDataIndex].mName;
        break;

    case CheckMode::TypeCheckByIndex:
        ret=mSerializedTypeListI->at(aIndex).mSerializedElementList[iDataIndex].mName;
        break;

    default:
        THEOLIZER_INTERNAL_ABORT("mCheckMode=%d", static_cast<int>(mCheckMode));
        break;
    }

    return ret;
}

// ***************************************************************************
//      エラー処理
// ***************************************************************************

//----------------------------------------------------------------------------
//      プリミティブ処理後のエラー・チェック
//----------------------------------------------------------------------------

void BaseSerializer::checkStreamError(std::ios::iostate iIoState)
{
    if (iIoState & std::istream::eofbit)
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"EOF occured.");
    }
    else if (iIoState & std::istream::failbit)
    {
        THEOLIZER_INTERNAL_DATA_ERROR(u8"Logical Error on stream.");
    }
    else if (iIoState & std::istream::badbit)
    {
        THEOLIZER_INTERNAL_IO_ERROR(u8"I/O Error.");
    }
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
    TypeIndex iTypeIndex
) : mSerializer(iSerializer),
    mElementsMapping(iSerializer.mElementsMapping),
    mIndent(iSerializer.mIndent),
    mCancelPrettyPrint(iSerializer.mCancelPrettyPrint)
{
    mSerializer.mElementsMapping=emOrder;
    mSerializer.mCancelPrettyPrint=true;
    mSerializer.saveProcessStart(iTypeIndex);
}

BaseSerializer::AutoRestoreSaveProcess::~AutoRestoreSaveProcess() noexcept(false)
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
//      グループ化処理補助クラス
//----------------------------------------------------------------------------

BaseSerializer::AutoRestoreSave::AutoRestoreSave
(
    BaseSerializer& iSerializer,
    ElementsMapping iElementsMapping,
    bool iCancelPrettyPrint
) : mSerializer(iSerializer),
    mElementsMapping(iSerializer.mElementsMapping),
    mIndent(iSerializer.mIndent),
    mCancelPrettyPrint(iSerializer.mCancelPrettyPrint)
{
    mSerializer.mElementsMapping=iElementsMapping;
    mSerializer.mCancelPrettyPrint=iCancelPrettyPrint;
    mSerializer.saveGroupStart();
}

BaseSerializer::AutoRestoreSave::~AutoRestoreSave() noexcept(false)
{
    theolizer::internal::Releasing aReleasing{};
    try
    {
        mSerializer.mIndent=mIndent;
        mSerializer.saveGroupEnd();
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
//      各種構造処理補助クラス
//----------------------------------------------------------------------------

BaseSerializer::AutoRestoreSaveStructure::AutoRestoreSaveStructure
(
    BaseSerializer&     iSerializer,
    ElementsMapping     iElementsMapping,
    Structure           iStructure,
    TypeIndex           iTypeIndex,
    std::size_t         iObjectId,
    bool                iCancelPrettyPrint
) : mSerializer(iSerializer),
    mElementsMapping(iSerializer.mElementsMapping),
    mStructure(iStructure),
    mIndent(iSerializer.mIndent),
    mCancelPrettyPrint(iSerializer.mCancelPrettyPrint)
{
    mSerializer.mElementsMapping=iElementsMapping;
    mSerializer.mCancelPrettyPrint=iCancelPrettyPrint;
    // データ内に型名を記録
    if (mSerializer.mCheckMode == CheckMode::TypeCheckInData)
    {
        if (iTypeIndex.isValid())
        {
            mTypeName.reset(new std::string(mSerializer.getTypeName(iTypeIndex)));
        }
        else
        {
            mTypeName.reset(new std::string);
        }
    }
    mSerializer.saveStructureStart(mStructure, *mTypeName.get(), iObjectId);
}

BaseSerializer::AutoRestoreSaveStructure::~AutoRestoreSaveStructure() noexcept(false)
{
    theolizer::internal::Releasing aReleasing{};
    try
    {
        mSerializer.mIndent=mIndent;
        mSerializer.saveStructureEnd(mStructure, *mTypeName.get());
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

// TypeCheckする
BaseSerializer::AutoRestoreLoadProcess::AutoRestoreLoadProcess
(
    BaseSerializer& iSerializer,
    TypeIndex iTypeIndex
) : mSerializer(&iSerializer)
{
    mSerializer->loadProcessStart(iTypeIndex);
}

// TypeIndex返却
BaseSerializer::AutoRestoreLoadProcess::AutoRestoreLoadProcess
(
    BaseSerializer& iSerializer,
    TypeIndexList*& oTypeIndexList
) : mSerializer(&iSerializer)
{
    oTypeIndexList=mSerializer->loadProcessStart(TypeIndex());
}

BaseSerializer::AutoRestoreLoadProcess::~AutoRestoreLoadProcess() noexcept(false)
{
    // ムーブ対応
    if (!mSerializer)
return;

    theolizer::internal::Releasing aReleasing{};
    try
    {
        mSerializer->loadProcessEnd();
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
//      グループ化処理補助クラス
//----------------------------------------------------------------------------

BaseSerializer::AutoRestoreLoad::AutoRestoreLoad
(
    BaseSerializer& iSerializer,
    ElementsMapping iElementsMapping
) : mSerializer(iSerializer),
    mElementsMapping(iSerializer.mElementsMapping)
{
//std::cout << "AutoRestoreLoad(" << iElementsMapping << ")\n";
    mSerializer.mElementsMapping=iElementsMapping;
    mSerializer.loadGroupStart();
}

BaseSerializer::AutoRestoreLoad::~AutoRestoreLoad() noexcept(false)
{
    theolizer::internal::Releasing aReleasing{};
    try
    {
        mSerializer.loadGroupEnd();
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
//std::cout << "~AutoRestoreLoad(" << mSerializer.mElementsMapping << ")\n";
}

//----------------------------------------------------------------------------
//      各種構造処理補助クラス
//----------------------------------------------------------------------------

BaseSerializer::AutoRestoreLoadStructure::AutoRestoreLoadStructure
(
    BaseSerializer&     iSerializer,
    ElementsMapping     iElementsMapping,
    Structure           iStructure,
    TypeIndex           iTypeIndex,
    std::size_t*        oObjectId
) : mSerializer(iSerializer),
    mElementsMapping(iSerializer.mElementsMapping),
    mStructure(iStructure)
{
//std::cout << "AutoRestoreLoadStructure(" << iElementsMapping << ")\n";
    mSerializer.mElementsMapping=iElementsMapping;
    // データ内に型名を記録
    if (mSerializer.mCheckMode == CheckMode::TypeCheckInData)
    {
        if (iTypeIndex.isValid())
        {
            mTypeName.reset(new std::string(mSerializer.getTypeName(iTypeIndex)));
        }
        else
        {
            mTypeName.reset(new std::string);
        }
    }
    mSerializer.loadStructureStart(mStructure, *mTypeName.get(), oObjectId);
}

BaseSerializer::AutoRestoreLoadStructure::~AutoRestoreLoadStructure() noexcept(false)
{
    theolizer::internal::Releasing aReleasing{};
    try
    {
        mSerializer.loadStructureEnd(mStructure, *mTypeName.get());
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
//std::cout << "~AutoRestoreLoadStructure(" << mSerializer.mElementsMapping << ")\n";
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
    if (found != mSerializeList->end())
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
                        THEOLIZER_INTERNAL_WRONG_USING
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
                    THEOLIZER_INTERNAL_WRONG_USING(u8"Can not resolve the address of pointer.");
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
    // save時はバッファ内データのフラッシュ
    // load時はバッファ内の無効データの破棄
    flush();
}

void BaseSerializer::clearTracking()
{
    // エラー情報登録準備
    theolizer::internal::ApiBoundarySerializer aApiBoundary(this, &mAdditionalInfo);

    mRequireClearTracking=false;
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

SharedPtrTable& BaseSerializer::registerSharedPtrTable(std::type_index iStdTypeIndex)
{
    // テーブル未生成なら生成する
    if (!mSharedPtrTables)
    {
        mSharedPtrTables.reset(new SharedPtrTables);
    }

    // 対象が未登録なら登録する
    auto aFound=mSharedPtrTables->find(iStdTypeIndex);
    if (aFound == mSharedPtrTables->end())
    {
        aFound=mSharedPtrTables->insert(aFound, std::make_pair(iStdTypeIndex, SharedPtrTable()));
    }

    return aFound->second;
}

// ***************************************************************************
//      ClassTypeの破棄処理
// ***************************************************************************

void BaseSerializer::disposeClass(ElementsMapping iElementsMapping)
{
    AutoRestoreLoadStructure aAutoRestoreLoadStructure(*this, iElementsMapping, Structure::Class);

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
        loadTypeName(aTypeName);

        // 型名から、現在のTypeIndexListを求める
return mTypeNameMap->mMap[aTypeName];
    }
    else
    {
        // TypeIndex回復
        TypeIndex aTypeIndex;
        loadControl(aTypeIndex);
        auto& aElementType = mSerializedTypeListI->at(aTypeIndex.getIndex());
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
    mTypeIndexTable.resize(mTypeIndexCount);

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
        TypeIndex aDataTypeIndex;
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
        TypeIndexList& aTypeIndexList=mTypeNameMap->mMap[aTypeName];
//std::cout << "aTypeIndexList : " << aDataTypeIndex << ", " << aTypeName
//          << " size()=" << aTypeIndexList.size() << " addr=" << &aTypeIndexList << "\n";

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
            TypeIndex aProgramTypeIndex = *(aTypeIndexList.begin());
            unsigned aVersionNo = getLocalVersionNo(aProgramTypeIndex);
            BaseTypeInfo* aTypeInfo = mTypeInfoList[aProgramTypeIndex.getIndex()];
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
                    TypeIndex aElementTypeIndex;
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
            mTypeIndexTable[aTypeIndex.getIndex()]=aDataTypeIndex;
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
                                    =mSerializedTypeListI->at(aDataTypeIndex.getIndex());
            aSerializedTypeIndex.mTypeName=std::move(aTypeName);
            aSerializedTypeIndex.mProgramTypeIndex=&aTypeIndexList;
//std::cout << "   &aTypeIndexList=" << &aTypeIndexList << "\n";
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
        TypeIndex aProgramTypeIndex = *(aSerializedTypeName.mProgramTypeIndex->begin());
        unsigned aVersionNo = getLocalVersionNo(aProgramTypeIndex);
        BaseTypeInfo* aTypeInfo = mTypeInfoList[aProgramTypeIndex.getIndex()];

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
            TypeIndex aElementTypeIndex=aElement.getTypeIndex();
            // 要素BaseTypeInfo
            BaseTypeInfo* aElementTypeInfo = mTypeInfoList[aElementTypeIndex.getIndex()];
            // 要素のバージョン番号
            unsigned aElementVersionNo = getLocalVersionNo(aElementTypeInfo->getTypeIndex());
            // 要素の型名
            std::string aElementTypeName=aElementTypeInfo->getTypeName(aElementVersionNo);
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
        TypeIndex aProgramTypeIndex = *(aSerializedTypeIndex.mProgramTypeIndex->begin());
        unsigned aVersionNo = getLocalVersionNo(aProgramTypeIndex);
        BaseTypeInfo* aTypeInfo = mTypeInfoList[aProgramTypeIndex.getIndex()];

//std::cout << "    [" << aProgramTypeIndex << "]"
//          << " getElementRange(" << aVersionNo << ").size()="
//          << aTypeInfo->getElementRange(aVersionNo).size() << "\n";
        std::vector<std::pair<char const*, TypeIndex> > aProgramElements;
        for (auto&& aElement : aTypeInfo->getElementRange(aVersionNo))
        {
            // 要素名
            char const* aElementName=aElement.getName();
            // 要素のTypeIndex
            TypeIndex aElementTypeIndex=aElement.getTypeIndex();
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
//std::cout << "----------------------- readHeaderTypeInfo()\n";
}

//----------------------------------------------------------------------------
//      TypeIndex一致判定
//----------------------------------------------------------------------------

bool BaseSerializer::isMatchTypeIndex(TypeIndex iSerializedTypeIndex, TypeIndex iProgramTypeIndex)
{
    // 付加情報(オブジェクト追跡と配列)が異なれば不一致
    if (iSerializedTypeIndex.getAdditional() != iProgramTypeIndex.getAdditional())
return false;

    bool aIsMatch=false;
    // リストにある時だけチェックする(ないなら不一致)
    if (iSerializedTypeIndex.getIndex() < mSerializedTypeListI->size())
    {
        auto& aElementType = mSerializedTypeListI->at(iSerializedTypeIndex.getIndex());
//std::cout << "   Index=" << iSerializedTypeIndex.getIndex() 
//          << " size()=" << aElementType.mProgramTypeIndex->size()
//          << " mProgramTypeIndex=" << aElementType.mProgramTypeIndex << "\n";
        for (auto aProgramTypeIndex : *(aElementType.mProgramTypeIndex))
        {
//std::cout << "        " << aProgramTypeIndex << " : " << iProgramTypeIndex << "\n";
            if (aProgramTypeIndex.getIndex() == iProgramTypeIndex.getIndex())
            {
//std::cout << "        Match!!\n";
                aIsMatch=true;
    break;
            }
        }
//std::cout << "    aIsMatch = " << aIsMatch << "\n";
    }
    return aIsMatch;
}

//############################################################################
//      End
//############################################################################

}   // namespace internal
}   // namespace theolizer
