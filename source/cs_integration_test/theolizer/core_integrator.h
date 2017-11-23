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

//#include <memory>
//#include <thread>
//#include <utility>

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
// ***************************************************************************
//      基底インテグレータ
// ***************************************************************************

class BaseIntegrator
{
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
    }

    void deleteSerializer(BaseSerializer* iBaseSerializer)
    {
        delete iBaseSerializer;
    }
public:
    virtual ~BaseIntegrator()
    {
    }

};

//############################################################################
//      End
//############################################################################

}   // namespace internal

}   // namespace theolizer

#endif  // THEOLIZER_INTERNAL_CORE_INTEGRATOR_H
