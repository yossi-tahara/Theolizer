//############################################################################
//      Theolizerライブラリの連携処理クラス(C++側コア部)
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

#if !defined(THEOLIZER_INTERNAL_CORE_INTEGRATOR_H)
#define THEOLIZER_INTERNAL_CORE_INTEGRATOR_H

#include <map>
#include <mutex>
#include <theolizer/serializer_binary.h>
#include <theolizer/serializer_json.h>

#include "memory_stream.h"

THEOLIZER_PROVIDED_BY("Theoride Technology");

//############################################################################
//      各種ヘルパー
//############################################################################

namespace theolizer
{

namespace internal
{
    class BaseIntegrator;
    using DelegateNotifySharedObject = void(*)(int, bool);
}

// ***************************************************************************
//      スレッドで使用するインテグレータを管理
// ***************************************************************************

class ThreadIntegrator
{
    static thread_local internal::BaseIntegrator*   mIntegrator;
public:
    static void setIntegrator(internal::BaseIntegrator* iIntegrator)
    {
        mIntegrator = iIntegrator;
    }
    static internal::BaseIntegrator* getIntegrator()
    {
        return mIntegrator;
    }
};

// ***************************************************************************
//      シリアライザの指定
//          C++/C#共通
// ***************************************************************************

enum class SerializerType
{
    Binary=1,           // Binary
    Json  =2            // Json
};

// ***************************************************************************
//      ユーザ・プログラムでの共有インスタンス保持用クラス
// ***************************************************************************

template<typename tType>
class SharedPointer
{
    internal::BaseIntegrator*   mIntegrator;    // インテグレータ
    std::size_t                 mIndex;         // 共有テーブルのインデックス番号
    std::shared_ptr<tType>      mInstance;      // インスタンス管理
public:
    // コンストラクタ
    SharedPointer() :
        mIntegrator(nullptr),
        mIndex(theolizer::internal::kInvalidSize),
        mInstance()
    { }
    SharedPointer
    (
        internal::BaseIntegrator&       iIntegrator,
        std::size_t                     iIndex,
        std::shared_ptr<tType> const&   iSharedPtr
    );
    SharedPointer(SharedPointer const& iSharedPointer) : 
        mIntegrator(iSharedPointer.mIntegrator),
        mIndex(iSharedPointer.mIndex),
        mInstance(iSharedPointer.mInstance)
    { }
    SharedPointer(SharedPointer&& iSharedPointer) noexcept : // std::vector<>管理用にnoexcept
        mIntegrator(iSharedPointer.mIntegrator),
        mIndex(iSharedPointer.mIndex),
        mInstance(std::move(iSharedPointer.mInstance))
    {
        iSharedPointer.mIndex = theolizer::internal::kInvalidSize;
    }

    // デストラクタ
    ~SharedPointer();

    // 代入演算子
    SharedPointer& operator=(SharedPointer const& iSharedPointer)       // コピー
    {
        mIntegrator = iSharedPointer.mIntegrator;
        mIndex      = iSharedPointer.mIndex;
        mInstance   = iSharedPointer.mInstance;
        return *this;
    }
    SharedPointer& operator=(SharedPointer&& iSharedPointer) noexcept   // ムーブ
    {
        mIntegrator = iSharedPointer.mIntegrator;
        mIndex      = iSharedPointer.mIndex;
        iSharedPointer.mIndex = theolizer::internal::kInvalidSize;
        mInstance   = std::move(iSharedPointer.mInstance);
        return *this;
    }

    // 有効性
    operator bool() const
    {
        return static_cast<bool>(mInstance);    // 登録済ならtrue(operator bool())
    }

    // オブジェクト獲得
    tType& get()
    {
        return *mInstance.get();
    }

    // オブジェクト開放
    void release()
    {
        mInstance.reset();
    }
};

//############################################################################
//      内部用（ユーザプログラムから使用不可）
//############################################################################

namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

// ***************************************************************************
//      基底インテグレータ
// ***************************************************************************

class BaseIntegrator
{
//----------------------------------------------------------------------------
//      関数クラス処理
//----------------------------------------------------------------------------

//      ---<<< ホルダ >>>---

    struct FuncClassHolderBase
    {
        virtual void processFunction
        (
            BaseSerializer& iISerializer,
            BaseSerializer& iOSerializer,
            ApiBoundarySerializer&& iApiBoundary,
            BaseSerializer::AutoRestoreLoadProcess&& iAutoRestoreLoadProcess
        )=0;
        virtual ~FuncClassHolderBase() { }
    };

    // 派生クラスのsaveTypeInstance/loadTypeInstance呼び出し用クラス
    template<class tFuncClass>
    struct FuncClassHolder : public FuncClassHolderBase
    {
        FuncClassHolder() { }
        ~FuncClassHolder() = default;

        void processFunction
        (
            BaseSerializer& iISerializer,
            BaseSerializer& iOSerializer,
            ApiBoundarySerializer&& iApiBoundary,
            BaseSerializer::AutoRestoreLoadProcess&& iAutoRestoreLoadProcess
        )
        {
            tFuncClass  aFuncClass;
            {
                // エラー情報登録
                ApiBoundarySerializer aApiBoundary(std::move(iApiBoundary));

                BaseSerializer::AutoRestoreLoadProcess
                    AutoRestoreLoadProcess(std::move(iAutoRestoreLoadProcess));
                THEOLIZER_INTERNAL_LOAD(iISerializer, aFuncClass, etmDefault);
            }
            iISerializer.clearTracking();   // flush

            auto ret = aFuncClass.callFunc();

            typedef decltype(ret)   ReturnClass;
            RegisterType<BaseSerializer, ReturnClass, tTheolizerVersion>::getInstance();

            {
                // エラー情報登録
                ApiBoundarySerializer aApiBoundary(&iOSerializer,iOSerializer.getAdditionalInfo());

                BaseSerializer::AutoRestoreSaveProcess
                    aAutoRestoreSaveProcess(iOSerializer, getTypeIndex<ReturnClass>());
                THEOLIZER_INTERNAL_SAVE(iOSerializer, ret, etmDefault);
            }
        }
    };

    // 関数クラス・リスト
    typedef std::map<std::size_t, std::unique_ptr<FuncClassHolderBase> > FuncClassList;
    static FuncClassList& getFuncClassList()
    {
        static FuncClassList    sFuncClassList;
        return sFuncClassList;
    }

public:

//      ---<<< 関数クラスをsFuncClassListへ登録する >>>---

    template<class tFuncClass>
    static void registerFuncClass()
    {
        // ここで渡すシリアライザが保存先指定を持っている必要がある!!
        RegisterType<BaseSerializer, tFuncClass, tTheolizerVersion>::getInstance();
        std::size_t aTypeIndex = ClassTypeInfo<tFuncClass>::getInstance().mTypeIndex;

DEBUG_PRINT("registerDrivedClass<", aTypeIndex, ", ",
    THEOLIZER_INTERNAL_TYPE_NAME(tFuncClass), ">()");

        getFuncClassList().emplace(aTypeIndex, new FuncClassHolder<tFuncClass>);
    }

//      ---<<< 要求を受信して処理する >>>---

    void processRequest(BaseSerializer& iISerializer, BaseSerializer& iOSerializer)
    {
        // CheckMode変更はヘッダ処理を実装するまでの仮実装
        //  (仮でかなりいい加減だがデバッグには使える)
        iISerializer.mCheckMode = CheckMode::TypeCheckByIndex;
        iOSerializer.mCheckMode = CheckMode::TypeCheckByIndex;

        // エラー情報登録
        ApiBoundarySerializer aApiBoundary(&iISerializer, iISerializer.getAdditionalInfo());

        TypeIndexList*  aTypeIndexList = nullptr;
        BaseSerializer::AutoRestoreLoadProcess
            aAutoRestoreLoadProcess(iISerializer, aTypeIndexList);
        std::size_t aTypeIndex = (*aTypeIndexList)[0];      // 先頭のみ使用する(１つしかないので)

        // 受信→関数呼び出し→送信処理
        getFuncClassList()[aTypeIndex]->processFunction
        (
            iISerializer,
            iOSerializer,
            std::move(aApiBoundary),
            std::move(aAutoRestoreLoadProcess)
        );
        iOSerializer.clearTracking();   // flush
    }

//----------------------------------------------------------------------------
//      共有テーブル管理
//----------------------------------------------------------------------------

private:
    //      ---<<< 共有テーブルの要素 >>>---

    // 基底クラス
    class SharedHolderBase
    {
    public:
        virtual void* getVoidPointer() = 0;
        virtual long getUseCount() = 0;
    };

    // 派生クラス
    template<typename tType>
    class SharedHolder : public SharedHolderBase
    {
        std::shared_ptr<tType>      mInstance;      // インスタンス管理
    public:
        SharedHolder() : mInstance(std::make_shared<tType>())
        { };
        SharedHolder(tType* iPointer) : mInstance(std::shared_ptr<tType>(iPointer))
        { };
        tType* get() { return mInstance.get(); }
        std::shared_ptr<tType>& getSharedPointer() { return mInstance; }

        void* getVoidPointer()
        {
            return mInstance.get();
        }
        long getUseCount()
        {
            return mInstance.use_count();
        }
    };

    //      ---<<< 共有テーブル >>>---

    typedef std::unique_ptr<SharedHolderBase>   SharedElement;
    typedef std::vector<SharedElement>          SharedTable;

    SharedTable                                 mSharedTable;
    std::mutex                                  mMutex;

public:
    // 受信時に使用
    //  Indexを受け取り、
    //      登録済ならそのインスタンスを返却
    //      未登録ならデフォルト・コンストラクタで生成して返却
    template<typename tType>
    tType* registerSharedInstance(std::size_t iIndex)
    {
        std::lock_guard<std::mutex> aLock(mMutex);

std::cout << "(1)registerSharedInstance<" << THEOLIZER_INTERNAL_TYPE_NAME(tType) << "> index=" << iIndex << "\n";
        if (mSharedTable.size() <= iIndex)
        {
            mSharedTable.resize(iIndex+1);
        }

        if (mSharedTable[iIndex] == nullptr)
        {
            auto aSharedHolder = new SharedHolder<tType>();
            mSharedTable[iIndex].reset(aSharedHolder);
    return aSharedHolder->get();
        }

        return reinterpret_cast<tType*>(mSharedTable[iIndex]->getVoidPointer());
    }

    // 送信処理用：指定領域を共有テーブルへ登録。
    //  未登録なら新規登録し、そのIndex返却
    //  登録済ならそのIndex返却。
    template<typename tType>
    std::size_t registerSharedInstance(tType* iInstance)
    {
        std::lock_guard<std::mutex> aLock(mMutex);
        std::size_t ret;

        // 既に登録済チェック
        for (ret=0; ret < mSharedTable.size(); ++ret)
        {
            if ((mSharedTable[ret] != nullptr)
             && (mSharedTable[ret]->getVoidPointer() == iInstance))
            {
std::cout << "(2)registerSharedInstance<" << THEOLIZER_INTERNAL_TYPE_NAME(tType) << "> index=" << ret << "\n";
    return ret;
            }
        }

        // 登録済でないなら先頭のnullptrを探す
        for (ret=0; ret < mSharedTable.size(); ++ret)
        {
            if (mSharedTable[ret] == nullptr)
            {
                mSharedTable[ret].reset(new SharedHolder<tType>(iInstance));
std::cout << "(3)registerSharedInstance<" << THEOLIZER_INTERNAL_TYPE_NAME(tType) << "> index=" << ret << "\n";
    return ret;
            }
        }

        // 新たに領域を増やす
        mSharedTable.emplace_back(new SharedHolder<tType>(iInstance));
std::cout << "(4)registerSharedInstance<" << THEOLIZER_INTERNAL_TYPE_NAME(tType) << "> index=" << ret << "\n";
        return ret;
    }

    //      ---<<< ユーザ側共有オブジェクト保持用クラス返却 >>>---

    template<typename tType>
    SharedPointer<tType> getSharedPointer(tType* iInstance)
    {
#if 0
        std::lock_guard<std::mutex> aLock(mMutex);

        // 登録済サーチ
        for (std::size_t index=0; index < mSharedTable.size(); ++index)
        {
            if ((mSharedTable[index] != nullptr)
             && (mSharedTable[index]->getVoidPointer() == iInstance))
            {
                auto& aSharedHolder=static_cast<SharedHolder<tType>& >(*mSharedTable[index].get());
    return SharedPointer<tType>(*this, index, aSharedHolder.getSharedPointer());
            }
        }
        throw new std::invalid_argument("Not registered C# shared-object.");
#else
        std::size_t aIndex = registerSharedInstance(iInstance);
        auto& aSharedHolder=static_cast<SharedHolder<tType>& >(*mSharedTable[aIndex].get());
        return std::move(SharedPointer<tType>(*this, aIndex, aSharedHolder.getSharedPointer()));
#endif
    }

    //      ---<<< 共有オブジェクト状態通知 >>>---

public:
    void notifySharedObject(std::size_t iIndex, bool iUserPresaved)
    {
        mDelegateNotifySharedObject(static_cast<int>(iIndex), iUserPresaved);
    }

    //      ---<<< 共有オブジェクト破棄 >>>---
protected:
    // 破棄出来ない時、false返却(破棄した時と既に破棄されている時はtrue返却)
    bool disposeShared(std::size_t iIndex)
    {
        std::lock_guard<std::mutex> aLock(mMutex);

std::cout << "BaseIntegrator::disposeShared(" << iIndex << ")\n";
        if ((iIndex < mSharedTable.size())
         && (mSharedTable[iIndex]))
        {
            if (mSharedTable[iIndex]->getUseCount() <= 1)
            {
                mSharedTable[iIndex].reset();
                return true;
            }
            else
            {
                return false;
            }
        }

        return true;
    }

//----------------------------------------------------------------------------
//      インテグレータ管理
//----------------------------------------------------------------------------

public:
    // 共有オブジェクト状態通知用コールバック
    DelegateNotifySharedObject  mDelegateNotifySharedObject;

    // サービス終了
    bool                        mTerminated;

    BaseIntegrator(DelegateNotifySharedObject iCallback) :
        mDelegateNotifySharedObject(iCallback),
        mTerminated(false)
    { }
    virtual ~BaseIntegrator() { }

    bool isTerminated() { return mTerminated; }

//----------------------------------------------------------------------------
//      シリアライザの生成／破棄処理
//----------------------------------------------------------------------------

protected:
    template<Destination uDefault>
    BaseSerializer* makeISerializer(SerializerType iSerializerType, std::istream& iIStream)
    {
        switch(iSerializerType)
        {
        case SerializerType::Binary:
            return new BinaryISerializer<uDefault>(iIStream);

        case SerializerType::Json:
            return new JsonISerializer<uDefault>(iIStream);
        }
        return nullptr;
    }

    template<Destination uDefault>
    BaseSerializer*  makeOSerializer
    (
        SerializerType  iSerializerType,
        std::ostream&   iOStream,
        unsigned        iGlobalVersionNo
    )
    {
        switch(iSerializerType)
        {
        case SerializerType::Binary:
            return new BinaryOSerializer<uDefault>(iOStream, iGlobalVersionNo);

        case SerializerType::Json:
            return new JsonOSerializer<uDefault>(iOStream, iGlobalVersionNo);
        }
        return nullptr;
    }

//  ---<<< シリアライザ返却 >>>---

private:
    // 通知用シリアライザ返却
    virtual BaseSerializer* getNotifySerializer()
    {
        THEOLIZER_INTERNAL_WRONG_USING("Not Supported getNotify() by this derived-serializer.");
        return nullptr;
    }

public:
    // 通知処理
    template<typename tType>
    void notify(tType const& iInstance)
    {
        theolizer::internal::RegisterType
        <
            theolizer::internal::BaseSerializer,
            tType,
            tTheolizerVersion
        >::getInstance();

        auto& aSerializer = *getNotifySerializer();

        // エラー情報登録
        ApiBoundarySerializer aApiBoundary(&aSerializer, aSerializer.getAdditionalInfo());

        // CheckMode変更はヘッダ処理を実装するまでの仮実装
        //  (仮でかなりいい加減だがデバッグには使える)
        aSerializer.mCheckMode = CheckMode::TypeCheckByIndex;
        {

            theolizer::internal::BaseSerializer::AutoRestoreSaveProcess aAutoRestoreSaveProcess
                (
                    aSerializer,
                    theolizer::internal::getTypeIndex<tType>()
                );
            THEOLIZER_INTERNAL_SAVE(aSerializer, const_cast<tType&>(iInstance), etmDefault);
        }
        aSerializer.flush();
    }
};

// ***************************************************************************
//      関数クラス登録処理
// ***************************************************************************

#define THEOLIZER_INTERNAL_REGISTER_FUNC(dFuncClass)                        \
    namespace theolizer{namespace internal{namespace{                       \
    template                                                                \
    class RegisterFuncClass<THEOLIZER_INTERNAL_UNPAREN dFuncClass>;         \
    }}}

template<class tFuncClass>
class RegisterFuncClass
{
    static RegisterFuncClass& mInstance;
    static void use(RegisterFuncClass const&) {}

    RegisterFuncClass()
    {
        DEBUG_PRINT("RegisterFuncClass<", THEOLIZER_INTERNAL_TYPE_NAME(tFuncClass), ">();");
        theolizer::internal::BaseIntegrator::registerFuncClass<tFuncClass>();
    }
public:
    static RegisterFuncClass& getInstance()
    {
        static RegisterFuncClass instance;
        use(mInstance);
        return instance;
    }
};

template<class tFuncClass>
RegisterFuncClass<tFuncClass>&
    RegisterFuncClass<tFuncClass>::mInstance = RegisterFuncClass<tFuncClass>::getInstance();

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

// ***************************************************************************
//      ユーザ・プログラムでの共有インスタンス保持用クラスの後置定義
// ***************************************************************************

//----------------------------------------------------------------------------
//  コンストラクタ
//----------------------------------------------------------------------------

template<typename tType>
SharedPointer<tType>::SharedPointer
(
    internal::BaseIntegrator&       iIntegrator,
    std::size_t                     iIndex,
    std::shared_ptr<tType> const&   iSharedPtr
) : mIntegrator(&iIntegrator),
    mIndex(iIndex),
    mInstance(iSharedPtr)
{
    // C#側共有テーブルへ破棄禁止を通知
    mIntegrator->notifySharedObject(mIndex, true);
}

//----------------------------------------------------------------------------
//  デストラクタ
//----------------------------------------------------------------------------

template<typename tType>
SharedPointer<tType>::~SharedPointer()
{
    // 共有オブジェクトを管理していたら
    // C#側共有テーブルへ破棄許諾を通知
    if (mInstance)
    {
        mIntegrator->notifySharedObject(mIndex, false);
    }
}

}   // namespace theolizer

//############################################################################
//      共有インスタンス交換用クラス
//############################################################################

#ifndef THEOLIZER_INTERNAL_DOXYGEN

template<typename tType>
struct SharedHelperTheolizer
{
    std::size_t mIndex;         // 共有テーブルのインデックス番号
    tType*      mInstance;      // 交換対象インスタンスへのポインタ
    SharedHelperTheolizer() : mIndex(theolizer::internal::kInvalidSize), mInstance(nullptr) { }
    operator tType&() { return *mInstance; }
    tType* operator->() { return mInstance; }
};

THEOLIZER_NON_INTRUSIVE_TEMPLATE_ORDER((template<typename T>),
                                        (SharedHelperTheolizer<T>), 1,
                                        SharedExchangerTheolizer);

//----------------------------------------------------------------------------
//      ユーザ定義
//----------------------------------------------------------------------------

//      ---<<< Version.1 >>>---

template<typename T>
template<class tMidSerializer, class tTheolizerVersion>
struct TheolizerNonIntrusive<SharedHelperTheolizer<T>>::
    TheolizerUserDefine<tMidSerializer, tTheolizerVersion, 1>
{
    // 保存
    static void saveClassManual
    (
        tMidSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget const*const& iInstance
    )
    {
        iSerializer.mDoCheck = false;
        auto& aInstance = const_cast<typename tTheolizerVersion::TheolizerTarget*&>(iInstance);
        aInstance->mIndex = theolizer::ThreadIntegrator::getIntegrator()->
            registerSharedInstance<T>(aInstance->mInstance);
        THEOLIZER_PROCESS(iSerializer, aInstance->mIndex);
        THEOLIZER_PROCESS(iSerializer, *(aInstance->mInstance));
    }

    // 回復
    static void loadClassManual
    (
        tMidSerializer& iSerializer,
        typename tTheolizerVersion::TheolizerTarget*& oInstance
    )
    {
        // もし、nullptrなら、インスタンス生成
        if (!oInstance)   oInstance=new typename tTheolizerVersion::TheolizerTarget();

        iSerializer.mDoCheck = false;
        THEOLIZER_PROCESS(iSerializer, oInstance->mIndex);
        oInstance->mInstance = theolizer::ThreadIntegrator::getIntegrator()->
            registerSharedInstance<T>(oInstance->mIndex);
        THEOLIZER_PROCESS(iSerializer, *(oInstance->mInstance));
    }
};

//----------------------------------------------------------------------------
//      自動生成
//----------------------------------------------------------------------------

#ifdef  THEOLIZER_WRITE_CODE // ###### SharedHelperTheolizer<T> ######

#define THEOLIZER_GENERATED_LAST_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kLastVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_TYPE THEOLIZER_INTERNAL_UNPAREN(SharedHelperTheolizer<T>)
#define THEOLIZER_GENERATED_PARAMETER_LIST template<typename T>
#define THEOLIZER_GENERATED_UNIQUE_NAME SharedExchangerTheolizer

//      ---<<< Version.1 >>>---

#define THEOLIZER_GENERATED_VERSION_NO THEOLIZER_INTERNAL_DEFINE(kVersionNo,1)
#define THEOLIZER_GENERATED_CLASS_NAME()\
    THEOLIZER_INTERNAL_TEMPLATE_NAME((u8"SharedHelperTheolizer",T))
#include <theolizer/internal/version_manual.inc>
#undef  THEOLIZER_GENERATED_VERSION_NO

#endif//THEOLIZER_WRITE_CODE // ###### SharedHelperTheolizer<T> ######

#endif  // THEOLIZER_INTERNAL_DOXYGEN

//############################################################################
//      End
//############################################################################

#endif  // THEOLIZER_INTERNAL_CORE_INTEGRATOR_H
