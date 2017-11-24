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
#include <theolizer/serializer.h>

#include "memory_stream.h"

//############################################################################
//      各種ヘルパー
//############################################################################

namespace theolizer
{

namespace internal
{
class BaseIntegrator;
}

// ***************************************************************************
//      スレッドで使用するインテグレータを管理
// ***************************************************************************

class ThreadIntegrator
{
    static thread_local internal::BaseIntegrator  *mIntegrator;
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
// ***************************************************************************

enum class SerializerType
{
    Binary,             // Binary
    Json                // Json
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

    struct HolderBase
    {
        virtual void processFunction
        (
            BaseSerializer& iISerializer,
            BaseSerializer& iOSerializer
        )=0;
        virtual ~HolderBase() { }
    };

    // 派生クラスのsaveTypeInstance/loadTypeInstance呼び出し用クラス
    template<class tFuncClass>
    struct Holder : public HolderBase
    {
        Holder() { }
        ~Holder() = default;
        void processFunction
        (
            BaseSerializer& iISerializer,
            BaseSerializer& iOSerializer
        )
        {
            exchange::func0Theolizer    afunc0Theolizer;
            THEOLIZER_INTERNAL_LOAD(iISerializer, afunc0Theolizer, etmDefault);
            afunc0Theolizer.callFunc();
            // 暫定的に要求クラスを返却するが、ここは返却クラスを返すのが正しい
            BaseSerializer::AutoRestoreSaveProcess
                aAutoRestoreSaveProcess(iOSerializer, 0);
            THEOLIZER_INTERNAL_SAVE(iOSerializer, afunc0Theolizer, etmDefault);
        }
    };

    // 関数クラス・リスト
    typedef std::map<std::size_t, std::unique_ptr<HolderBase> > FuncClassList;
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

        getFuncClassList().emplace(aTypeIndex, new Holder<tFuncClass>);
    }

//      ---<<< 関数クラスを呼び出す >>>---

    void callFunc(BaseSerializer& iISerializer, BaseSerializer& iOSerializer)
    {
        // CheckMode変更はヘッダ処理を実装するまでの仮実装
        //  (仮でかなりいい加減だがデバッグには使える)
        iISerializer.mCheckMode = CheckMode::TypeCheckByIndex;
        iOSerializer.mCheckMode = CheckMode::TypeCheckByIndex;

        TypeIndexList*  aTypeIndexList = nullptr;
        BaseSerializer::AutoRestoreLoadProcess
            aAutoRestoreLoadProcess(iISerializer, aTypeIndexList);
        std::size_t aTypeIndex = (*aTypeIndexList)[0];      // 先頭のみ使用する(１つしかないので)

        // 受信→関数呼び出し→送信処理
        getFuncClassList()[aTypeIndex]->processFunction(iISerializer, iOSerializer);
        iOSerializer.clearTracking();   // flush
    }

//----------------------------------------------------------------------------
//      インテグレータ管理
//----------------------------------------------------------------------------
public:
    bool            mTerminated;        // サービス終了

    BaseIntegrator() : mTerminated(false) { }
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

    void deleteSerializer(BaseSerializer* iBaseSerializer)
    {
        delete iBaseSerializer;
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

//############################################################################
//      End
//############################################################################

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal

}   // namespace theolizer

#endif  // THEOLIZER_INTERNAL_CORE_INTEGRATOR_H
