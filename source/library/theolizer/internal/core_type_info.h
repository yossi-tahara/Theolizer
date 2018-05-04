//############################################################################
//      Theolizerライブラリのコア部(型情報リスト)
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

#if !defined(THEOLIZER_INTERNAL_CORE_TYPE_INFO_H)
#define THEOLIZER_INTERNAL_CORE_TYPE_INFO_H

#include "core_assorted.h"

//############################################################################
//      Begin
//############################################################################

// ***************************************************************************
//          警告抑止
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable:4100 4127 4251)
#endif

namespace theolizer
{
namespace internal
{
#ifndef THEOLIZER_INTERNAL_DOXYGEN

//############################################################################
//      各種シンボル定義
//############################################################################

// ***************************************************************************
//      フィールド対応方法指定
// ***************************************************************************

enum ElementsMapping
{
    emName,                 // フィールド名(=0:完全自動時)
    emOrder                 // 順序
};

// ***************************************************************************
//      シリアライズ・データ読出状態
// ***************************************************************************

enum ReadStat {Terminated=0, Continue, DontCare};

// ***************************************************************************
//      オブジェクト追跡指定
//          動的生成領域：newで獲得したエリア／デシリアライズ時newが必要になる
//                        複数のポインタが同じ動的生成領域をポイントし、かつ、
//                        それぞれのポインタが獲得／開放するケースがある。(shared_ptr<>等)
//                        仮想基底クラスは同じ領域を複数からポイントされるため、
//                        動的生成領域と同じ追跡処理を行う。
//          静的定義領域：グローバル変数、ローカル変数、メンバー変数等、
//                        明示的解放が不要な領域
//                        参照はある種のポインタであるが、別名として運用するので
//                        静的定義領域として処理する。
// ***************************************************************************

enum TrackingMode
{
    etmDefault,     // デフォルト：ポインタならアドレスのみ回復、その他はオブジェクト追跡無し
    etmPointee,     // ポイントすることはないオブジェクト追跡対象(静的領域と参照)
    etmOwner        // ポイントすることもあるオブジェクト追跡対象(動的生成領域と仮想基底クラス)
};

//############################################################################
//      TypeIndexクラス定義
//############################################################################

//----------------------------------------------------------------------------
//      TypeIndex本体
//----------------------------------------------------------------------------

class TypeIndex
{
    unsigned    mTypeIndexImpl;

    // オブジェクト追跡
    enum TypeIndexTracking : unsigned
    {
        etipMask                =0x0c,
        etipNone                =0x00,
        etipPointee             =0x04,
        etipNormalPointer       =0x08,
        etipOwnerPointer        =0x0c
    };
public:
    TypeIndex() : mTypeIndexImpl(kInvalidUnsigned) { }
    TypeIndex(short iIndex)    : mTypeIndexImpl(iIndex*TypeIndexRadix) { } // primitive用
    TypeIndex(unsigned iIndex) : mTypeIndexImpl(iIndex*TypeIndexRadix) { }
    TypeIndex(TypeIndex const& iTypeIndex) : mTypeIndexImpl(iTypeIndex.mTypeIndexImpl) { }
    TypeIndex& operator=(unsigned iIndex)
    {
        mTypeIndexImpl = iIndex*TypeIndexRadix;
        return *this;
    }
    TypeIndex& operator=(TypeIndex const& iTypeIndex)
    {
        mTypeIndexImpl = iTypeIndex.mTypeIndexImpl;
        return *this;
    }
    void set(unsigned iIndex, unsigned iAdditional)
    {
        mTypeIndexImpl = iIndex*TypeIndexRadix + iAdditional;
    }
    void setRaw(unsigned iTypeIndexImpl)
    {
        mTypeIndexImpl = iTypeIndexImpl;
    }
    unsigned getRaw() const
    {
        return mTypeIndexImpl;
    }
    bool isValid() const
    {
        return mTypeIndexImpl != kInvalidUnsigned;
    }
    bool operator==(TypeIndex iTypeIndex) const
    {
        return mTypeIndexImpl == iTypeIndex.mTypeIndexImpl;
    }
    bool operator!=(TypeIndex iTypeIndex) const
    {
        return mTypeIndexImpl != iTypeIndex.mTypeIndexImpl;
    }
    unsigned getIndex() const
    {
        return mTypeIndexImpl/TypeIndexRadix;
    }

    // 追加情報
    void setPointer()
    {
        mTypeIndexImpl |= etipNormalPointer;
    }
    bool isPointer() const
    {
        return !!(mTypeIndexImpl & etipNormalPointer);
    }
    unsigned getAdditional() const
    {
        return mTypeIndexImpl & ~TypeIndexMask;
    }
    // 追加文字列
    //  付加情報有りの時、$に続けて
    //      etipPointee:"e"     etipNormalPointer:"p"   etipOwnerPointer:"o"
    //      etiaArray1:"1"      etiaArray2:"2"          etiaArray3:"3"
    //  通常ポインタの1次元配列:$p1
    std::string getAdditionalString() const
    {
        std::string ret;
        if (mTypeIndexImpl & (etipMask|etiaMask))
        {
            ret="$";
        }
        switch(mTypeIndexImpl & etipMask)
        {
        case etipNone:                      break;
        case etipPointee:       ret+='e';   break;
        case etipNormalPointer: ret+='p';   break;
        case etipOwnerPointer:  ret+='o';   break;
        }
        switch(mTypeIndexImpl & etiaMask)
        {
        case etiaNone:                      break;
        case etiaArray1:        ret+='1';   break;
        case etiaArray2:        ret+='2';   break;
        case etiaArray3:        ret+='3';   break;
        }
        return ret;
    }
    // 追加文字列分離
    //  戻り値は付加情報
    static unsigned splitAdditional(std::string& ioTypeName)
    {
        std::size_t pos = ioTypeName.find('$');

        // 付加文字列無し
        if (pos == std::string::npos)
    return 0;

        char    first  = ioTypeName[pos+1];

        unsigned    ret=0;
        bool        skip=true;
        switch(first)
        {
        case 'e':   ret += etipPointee;         break;
        case 'p':   ret += etipNormalPointer;   break;
        case 'o':   ret += etipOwnerPointer;    break;
        default:    skip=false;                 break;
        }
        // ポインタ情報があったら、配列情報が続いているかも知れない
        if (skip)
        {
            // 配列情報が無かったら解析終了
            if (pos+2 >= ioTypeName.size())
            {
                ioTypeName.resize(pos);
    return ret;
            }
            first=ioTypeName[pos+2];
        }

        switch(first)
        {
        case '1':   ret += etiaArray1;          break;
        case '2':   ret += etiaArray2;          break;
        case '3':   ret += etiaArray3;          break;
        default:    THEOLIZER_INTERNAL_DATA_ERROR(u8"Unknown additional.(%1%)", ioTypeName);
        }

        ioTypeName.resize(pos);
        return ret;
    }

    // 配列種別
    enum TypeIndexArray : unsigned
    {
        etiaMask                =0x03,
        etiaNone                =0x00,      // 非配列
        etiaArray1              =0x01,      // 1次元配列
        etiaArray2              =0x02,      // 2次元配列
        etiaArray3              =0x03,      // 3次元配列
    };
    unsigned getRank() const
    {
        return mTypeIndexImpl & etiaMask;
    }
    void setRank(unsigned iRank)
    {
        if (3 < iRank)
        {
            THEOLIZER_INTERNAL_ERROR(u8"too large array rank(%1%)", iRank);
        }
        mTypeIndexImpl |= iRank;
    }

    // オブジェクト追跡種別
    void setTracking(TrackingMode iTrackingMode)
    {
        if (mTypeIndexImpl & etipNormalPointer)
        {
            switch(iTrackingMode)
            {
            case etmDefault:
    return;

            case etmOwner:
                mTypeIndexImpl |= etipOwnerPointer;
    return;

            default:
                break;
            }
        }
        else
        {
            switch(iTrackingMode)
            {
            case etmDefault:
    return;

            case etmPointee:
                mTypeIndexImpl |= etipPointee;
    return;

            default:
                break;
            }
        }
        THEOLIZER_INTERNAL_ERROR("setTracking() error");
    }

    // TypeIndexの基数
    constexpr static unsigned   TypeIndexRadix = 0x10;
    constexpr static unsigned   TypeIndexMask  = (~0xfu);

    // 文字列型への出力
    friend std::ostream& operator<<(std::ostream& iOStream, TypeIndex iTypeIndex)
    {
        iOStream << (iTypeIndex.mTypeIndexImpl/TypeIndexRadix) << "e"
                 << (iTypeIndex.mTypeIndexImpl&(~TypeIndexMask));
        return iOStream;
    }

    // 文字列型からの入力
    friend std::istream& operator>>(std::istream& iIStream, TypeIndex& iTypeIndex)
    {
        unsigned aIndex;
        iIStream >> aIndex;
        if (iIStream.get() != 'e')
        {
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
        }
        unsigned aAdditional;
        iIStream >> aAdditional;
        if (TypeIndexRadix <= aAdditional)
        {
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Format Error.");
        }
        iTypeIndex.set(aIndex, aAdditional);
//std::cout << "{" << iTypeIndex << "}\n";

        // I/Oエラーチェック
        auto aStat = iIStream.rdstate();
        if (aStat & std::istream::eofbit)
        {
            THEOLIZER_INTERNAL_DATA_ERROR(u8"EOF occured.");
        }
        else if (aStat & std::istream::failbit)
        {
            THEOLIZER_INTERNAL_DATA_ERROR(u8"Logical Error on stream.");
        }
        else if (aStat & std::istream::badbit)
        {
            THEOLIZER_INTERNAL_IO_ERROR(u8"I/O Error.");
        }
        return iIStream;
    }
};

//############################################################################
//      internal化のための前方宣言
//############################################################################

template<class tBaseSerializer, typename tType, bool tIsRegister=false,
         TrackingMode tTrackingMode=etmDefault, class tEnable=void>
struct Switcher;

template
<
    TrackingMode tTrackingMode,
    class tTheolizerVersion,
    class tSerializer,
    typename tType,
    class tEnable=void
>
struct BranchedProcess;

template
<
    TrackingMode tTrackingMode,
    class tTheolizerVersion,
    class tSerializer,
    typename tType
>
void process
(
    tSerializer&    iSerializer,
    tType const&    iInstance,
    char const*     iName,
    char const*     iFileName,
    size_t          iLineNo
);

template
<
    TrackingMode tTrackingMode,
    class tTheolizerVersion,
    class tSerializer,
    typename tType
>
void process
(
    tSerializer&    iSerializer,
    tType&          ioInstance,
    char const*     iName,
    char const*     iFileName,
    size_t          iLineNo
);

//############################################################################
//      型管理
//############################################################################

// ***************************************************************************
//      型リスト(シングルトン)
//          派生シリアライザー毎に型リストを生成し、型IDを決定する。
// ***************************************************************************

class THEOLIZER_INTERNAL_DLL BaseTypeInfo;

//----------------------------------------------------------------------------
//      型種別
//----------------------------------------------------------------------------

enum TypeCategory
{
    etcPointerType,     // ポインタ型と参照型の両方
    etcArrayType,
    etcClassType,
    etcEnumType,
    etcPrimitiveType
};

//----------------------------------------------------------------------------
//      型リスト
//----------------------------------------------------------------------------

typedef std::vector<TypeIndex>      TypeIndexList;
typedef std::vector<BaseTypeInfo*>  TypeInfoListImpl;

//----------------------------------------------------------------------------
//      型リスト本体
//----------------------------------------------------------------------------

class THEOLIZER_INTERNAL_DLL TypeInfoList
{
private:
    template<class, class, class, bool, bool> friend class RegisterType;

    TypeInfoList() { }

    // 型のリスト
    TypeInfoListImpl    mTypeInfoList;

public:
    static TypeInfoList& getInstance();

    // コピー／ムーブ禁止(std::vector<>を使っているのでis_trivially_copyableにならない)
    TypeInfoList(const TypeInfoList&)  = delete;
    TypeInfoList(      TypeInfoList&&) = delete;
    TypeInfoList& operator=(const TypeInfoList&)  = delete;
    TypeInfoList& operator=(      TypeInfoList&&) = delete;

    // 型登録(TypeIndex返却)
    TypeIndex registerType(BaseTypeInfo* iTypeInfo);

    // リスト返却
    TypeInfoListImpl& getList() {return mTypeInfoList;}

    // BaseTypeInfo*返却
    BaseTypeInfo* operator[](TypeIndex iTypeIndex)
    {
        unsigned aIndex = iTypeIndex.getIndex();
        return mTypeInfoList[aIndex];
    }
};

// ***************************************************************************
//      型情報管理用基底クラス
// ***************************************************************************

//----------------------------------------------------------------------------
//      ポリモーフィズム対応用
//----------------------------------------------------------------------------

//      ---<<< ヘッダ・シリアライズ中の各型の出力状態 >>>---

enum SaveStat
{
    essIdle,
    essSaving,
    essSaved
};

//----------------------------------------------------------------------------
//      enum型の保存方法指定
//          名前、もしくは、値を保存するので、その指定
//----------------------------------------------------------------------------

enum EnumSaveType
{
    estName,                // enumシンボル名(=0:完全自動時)
    estValue                // enum値
};

//----------------------------------------------------------------------------
//      クラス要素／Enumシンボル枚挙用Range
//----------------------------------------------------------------------------

struct ElementBase;
using ElementRange = PolyRange<ElementBase const>;

//----------------------------------------------------------------------------
//      型種別(メタ・シリアライズ用)
//----------------------------------------------------------------------------

enum TypeKind
{
    // フラグ
    etkPrimitiveFlag        =1,                 // Primitive型
    etkEnumFlag             =2,                 // enum型
    etkClassFlag            =4,                 // クラス型
    etkNonIntrusiveFlag     =8,                 //   非侵入型
    etkManualFlag           =16,                //   手動型
    etkTemplateFlag         =32,                //   テンプレート

    // 値
    etkGlobalTable          =0,                 // グローバルバージョン番号テーブル名
    etkPrimitive            =etkPrimitiveFlag,  // プリミティブ(派生Serialzier)
    etkEnum                 =etkEnumFlag | etkNonIntrusiveFlag,
    etkIntrusiveAuto        =etkClassFlag,
    etkNonIntrusiveAuto     =etkClassFlag | etkNonIntrusiveFlag,
    etkIntrusiveManual      =etkClassFlag                       | etkManualFlag, 
    etkNonIntrusiveManual   =etkClassFlag | etkNonIntrusiveFlag | etkManualFlag
};

//      ---<<< クラス種別 >>>---

enum ClassFlag
{
    ecfFullAuto             =1,     // 0:Normal     1:完全自動
    ecfOrder                =2      // 0:Name       1:Order     (要素対応方式)
};

//      ---<<< enum種別 >>>---

enum EnumFlag
{
    eefUnsigned             =1,     // 0:符号付き   1:符号無し
    eefScoped               =2,     // 0:Unscoped   1:Scoped
    eefValue                =4      // 0:名前保存   1:値保存
};

//----------------------------------------------------------------------------
//      基底クラス本体
//----------------------------------------------------------------------------

class BaseSerializer;

class BaseTypeInfo
{
private:
    template<class, class, class, bool, bool>               friend class RegisterType;
                                                            friend class BaseSerializer;
                                                            friend class TypeInfoList;
    template<class>                                         friend class PointerTypeInfo;
    template<class>                                         friend class ArrayTypeInfo;
    template<class>                                         friend class ClassTypeInfo;
    template<class, typename, bool, TrackingMode, class>    friend struct Switcher;
    template<typename, bool, class>                         friend struct Switcher2;
    template<typename...>                                   friend struct ParameterName;
    template<class>                                         friend class Element;
    friend class BaseIntegrator;

    // トップ・レベルの保存先
    bool                    mIsTopLevel;
    Destinations            mTopDestinations;       // 通常のインスタンス
    Destinations            mPointerDestinations;   // ポインタ経由

    // 手動型からの保存
    bool                    mIsManual;

    // 型種別
    TypeCategory            mTypeCategory;

protected:
    BaseTypeInfo(TypeCategory iTypeCategory) :
        mIsTopLevel(false),
        mIsManual(false),
        mTypeCategory(iTypeCategory)
    { }

    // TypeInfoList<>::mTypeInfoListへの登録先インデックス
protected:
    TypeIndex               mTypeIndex;

public:
    TypeIndex   getTypeIndex() { return mTypeIndex; }

//      ---<<< トップ・レベルの保存先関連 >>>---

    void addDestination(Destinations const& iDestinations)
    {
        mIsTopLevel=true;
        mTopDestinations.add(iDestinations);
    }

    // トップ・レベルの保存先一致判定
    bool isTopMatch
    (
        Destinations const& iSerializerDestinations
    )
    {
        // トップ・レベルでないならfalse
        if (!mIsTopLevel)
    return false;

        // 保存先がAllも含めて一致するなら保存する
        return iSerializerDestinations.isMatch(mTopDestinations, true);
    }

//      ---<<< トップ・レベルのポインタの保存先関連 >>>---
//      派生クラスについて追跡が必要であるため、別途記録すｒ

    void addPointerDestination(Destinations const& iDestinations)
    {
        mIsTopLevel=true;
        mPointerDestinations.add(iDestinations);
    }

    // トップ・レベルのポインタの保存先一致判定
    bool isPointerMatch
    (
        Destinations const& iSerializerDestinations
    )
    {
        // トップ・レベルでないならfalse
        if (!mIsTopLevel)
    return false;

        // 保存先がAllも含めて一致するなら保存する
        return iSerializerDestinations.isMatch(mPointerDestinations, true);
    }

//      ---<<< 手動型からの保存 >>>---

    bool isManual() {return mIsManual;}

//      ---<<< 型チェック用 >>>---

public:

    // 下記は各TypeInfoで定義する
    virtual std::type_index getStdTypeIndex(bool iRaw=false) const = 0;
    virtual unsigned        getLastVersionNoV() const = 0;
    virtual std::string     getTypeName(unsigned) = 0;

    // 下記はClassTypeInfoのみ有効
    virtual char const*     getUniqueName()
    {THEOLIZER_INTERNAL_ABORT("BaseTypeInfo::getUniqueName()");}
    virtual ElementsMapping getElementsMapping(unsigned iVersionNo)
    {THEOLIZER_INTERNAL_ABORT("BaseTypeInfo::getElementsMapping()");}

    // 下記はClassTypeInfoとEnumTypeInfoのみ有効
    virtual ElementRange getElementRange(unsigned iVersionNo)
    {return ElementRange();}

    // デバッグ用
    virtual char const*     getCName() const = 0;
    bool isTopLevel() {return mIsTopLevel;}

//      ---<<< ポリモーフィズム対応 >>>---

private:
    // 当クラスから派生したクラスについてヘッダへ型情報記録するよう指示する
    //  自分より前にあるクラスについて保存要求したら、true返却
    virtual bool setSaving(BaseSerializer& iSerializer, std::vector<SaveStat>& ioSaveStatList)
    {return false;}

//      ---<<< メタ・シリアライズ用 >>>---

    // enum型の時、iVersionNoは有効。その他はDon't care
    virtual unsigned getTypeFlags(unsigned iVersionNo) {return 0;}

    virtual TypeKind getTypeKind()
    {THEOLIZER_INTERNAL_ABORT("BaseTypeInfo::getTypeKind()");}
};

// ***************************************************************************
//      グローバル・バージョン・テーブル検索用type_index返却
//          ポインタ、参照、配列の大元の型によりtype_indexを求める
// ***************************************************************************

// 大元の型
template<typename tType, class tEnable=void>
class StdTypeIndex
{
    typedef typename std::remove_cv<tType>::type    Type;
public:
    static std::type_index getStdTypeIndex()
    {
        return std::type_index(typeid(Type));
    }
};

// ポインタ解除
template<typename tType>
class StdTypeIndex<tType, EnableIf<std::is_pointer<tType>::value>>
{
    typedef typename std::remove_pointer<tType>::type   Type;
public:
    static std::type_index getStdTypeIndex()
    {
        return StdTypeIndex<Type>::getStdTypeIndex();
    }
};
// 参照解除
template<typename tType>
class StdTypeIndex<tType, EnableIf<std::is_lvalue_reference<tType>::value>>
{
    typedef typename std::remove_reference<tType>::type Type;
public:
    static std::type_index getStdTypeIndex()
    {
        return StdTypeIndex<Type>::getStdTypeIndex();
    }
};
// 配列解除
template<typename tType>
class StdTypeIndex<tType, EnableIf<std::is_array<tType>::value>>
{
    typedef typename std::remove_extent<tType>::type    Type;
public:
    static std::type_index getStdTypeIndex()
    {
        return StdTypeIndex<Type>::getStdTypeIndex();
    }
};

// ***************************************************************************
//      ポインタ型管理クラス(シングルトン)
//          ポインタ型を型リストへ登録する
// ***************************************************************************

template<typename tPointerType>
class PointerTypeInfo : public BaseTypeInfo
{
private:
    // コンストラクタ／デストラクタ
    PointerTypeInfo() : BaseTypeInfo(etcPointerType)
    {
        typedef typename std::remove_pointer<tPointerType>::type    PointeeType;
        static_assert(!std::is_pointer<PointeeType>::value, "Not support pointer of pointer");
        typedef typename GetTypeInfo<PointeeType>::Type TypeInfo;
        auto& aTypeInfo=TypeInfo::getInstance();
        mTypeIndex = aTypeInfo.getTypeIndex();
        mTypeIndex.setPointer();
//std::cout << "PointerTypeInfo() : " << getCName() << "\n";
//std::cout << "    " << THEOLIZER_INTERNAL_TYPE_NAME(PointeeType) << "\n";
//std::cout << "    " << mTypeIndex << "\n";
    }

public:
    static PointerTypeInfo& getInstance()
    {
        static PointerTypeInfo instance;
        return instance;
    }

    // コピー／ムーブ禁止(仮想関数があるのでis_trivially_copyableにならない)
    PointerTypeInfo(const PointerTypeInfo&)  = delete;
    PointerTypeInfo(      PointerTypeInfo&&) = delete;
    PointerTypeInfo& operator=(const PointerTypeInfo&)  = delete;
    PointerTypeInfo& operator=(      PointerTypeInfo&&) = delete;

//      ---<<< 型チェック用 >>>---

    // type_index返却
    std::type_index getStdTypeIndex(bool iRaw) const
    {
        return StdTypeIndex<tPointerType>::getStdTypeIndex();
    }
    // 最新版のバージョン番号返却
    static unsigned getLastVersionNo()
    {THEOLIZER_INTERNAL_ERROR("bug(PointerTypeInfo::getLastVersionNo)");return 0;}
    unsigned getLastVersionNoV() const {return getLastVersionNo();}
    // 型名返却
    std::string getTypeName(unsigned)
    {THEOLIZER_INTERNAL_ERROR("bug(PointerTypeInfo::getTypeName)");return "";}
    // C言語名返却(デバッグ用)
    char const* getCName() const
    {
        return THEOLIZER_INTERNAL_TYPE_NAME(tPointerType);
    }

//      ---<<< メタ・シリアライズ用 >>>---

    TypeKind    getTypeKind()
    {
        BaseTypeInfo* aTypeInfo =
            TypeInfoList::getInstance().getList().at(mTypeIndex.getIndex());
        return aTypeInfo->getTypeKind();
    }
};

// ***************************************************************************
//      生配列管理クラス(シングルトン)
//          生配列型を型リストへ登録する
//          型名は"型名[]"とし、型名に要素数は含まない。
//              要素数が異なっても同じ型として認識する。
//              これはClassTypeの要素が変化しても同じ型と認識することと同じ。
// ***************************************************************************

//----------------------------------------------------------------------------
//      基本型取り出し
//----------------------------------------------------------------------------

template<typename tUnderlyingType, class tEnable=void>
struct GetUnderlyingTypeImpl
{
    typedef tUnderlyingType   Type;
};

template<typename tArrayType>
struct GetUnderlyingTypeImpl<tArrayType, EnableIf<std::is_array<tArrayType>::value> >
{
    typedef typename 
        GetUnderlyingTypeImpl<typename std::remove_extent<tArrayType>::type>::Type  Type;
};

template<typename tArrayType>
using GetUnderlyingType = typename GetUnderlyingTypeImpl<tArrayType>::Type;

//----------------------------------------------------------------------------
//      本体
//----------------------------------------------------------------------------

template<typename tArrayType>
class ArrayTypeInfo : public BaseTypeInfo
{
private:
    // コンストラクタ／デストラクタ
    ArrayTypeInfo() : BaseTypeInfo(etcArrayType)
    {
        typedef GetUnderlyingType<tArrayType>               UnderlyingType;
        typedef typename GetTypeInfo<UnderlyingType>::Type  TypeInfo;
        auto& aTypeInfo=TypeInfo::getInstance();
        mTypeIndex=aTypeInfo.getTypeIndex();
        mTypeIndex.setRank(std::rank<tArrayType>::value);
//std::cout << "ArrayTypeInfo() : " << getCName() << "\n";
//std::cout << "    " << THEOLIZER_INTERNAL_TYPE_NAME(UnderlyingType) << "\n";
//std::cout << "    " << mTypeIndex << "\n";
    }

public:
    static ArrayTypeInfo& getInstance()
    {
        static ArrayTypeInfo instance;
        return instance;
    }

    // コピー／ムーブ禁止(仮想関数があるのでis_trivially_copyableにならない)
    ArrayTypeInfo(const ArrayTypeInfo&)  = delete;
    ArrayTypeInfo(      ArrayTypeInfo&&) = delete;
    ArrayTypeInfo& operator= (const ArrayTypeInfo&)  = delete;
    ArrayTypeInfo& operator= (      ArrayTypeInfo&&) = delete;

//      ---<<< 型チェック用 >>>---

    // type_index返却
    std::type_index getStdTypeIndex(bool iRaw) const
    {return StdTypeIndex<tArrayType>::getStdTypeIndex();}
    // 最新版のバージョン番号返却
    static unsigned getLastVersionNo()
    {THEOLIZER_INTERNAL_ERROR("bug(ArrayTypeInfo::getLastVersionNo)");return 0;}
    unsigned getLastVersionNoV() const {return getLastVersionNo();}
    // 型名返却
    std::string getTypeName(unsigned)
    {THEOLIZER_INTERNAL_ERROR("bug(ArrayTypeInfo::getTypeName)");return "";}
    // C言語名返却(デバッグ用)
    char const* getCName() const
    {
        return THEOLIZER_INTERNAL_TYPE_NAME(tArrayType);
    }

//      ---<<< メタ・シリアライズ用 >>>---

    TypeKind    getTypeKind()
    {
        BaseTypeInfo* aTypeInfo =
            TypeInfoList::getInstance().getList().at(mTypeIndex.getIndex());
        return aTypeInfo->getTypeKind();
    }
};

// ***************************************************************************
//      ClassType管理クラス(シングルトン)
//          struct/class型を型リストへ登録する
//          tClassType
//              侵入型      ターゲット・クラス
//              非侵入型    TheolizerNonIntrusive<ターゲット・クラス>
//
//          注意事項(http://jumble-note.blogspot.jp/2015/01/c.html)
//              クラス・テンプレートの名前探索はテンプレート・パラメータが
//              全て確定後に行われるとC++規格にて定められている。
//                  →部分特殊化や特殊化を考慮？
//              そのため、クラス・テンプレートを継承した場合、そのままでは、
//              基底クラスのメンバは名前探索対象にならない。
//              しかし、明示的に基底クラスのメンバであることを指定すればよい。
//                  →部分特殊化や特殊化で無くなっていたら、その時エラーか？
//              注意深くやれと言うことかも。
// ***************************************************************************

template<class tClassType>
class ClassTypeInfo : public BaseTypeInfo
{
    template<class, typename, class>    friend struct SavePointer;
    template<class, typename, class>    friend struct LoadPointer;

    typedef typename tClassType::TheolizerTarget    TheolizerTarget;
public:
    typedef typename tClassType::TheolizerClass     TheolizerClass;

private:
    // コンストラクタ／デストラクタ
    ClassTypeInfo() : BaseTypeInfo(etcClassType)
    {
//std::cout << "ClassTypeInfo() : " << getCName() << "\n";
        mTypeIndex = TypeInfoList::getInstance().registerType(this);
    }
public:
    static ClassTypeInfo& getInstance()
    {
        static ClassTypeInfo instance;
        return instance;
    }

    // コピー／ムーブ禁止(仮想関数があるのでis_trivially_copyableにならない)
    ClassTypeInfo(const ClassTypeInfo&)  = delete;
    ClassTypeInfo(      ClassTypeInfo&&) = delete;
    ClassTypeInfo& operator=(const ClassTypeInfo&)  = delete;
    ClassTypeInfo& operator=(      ClassTypeInfo&&) = delete;

//      ---<<< 型チェック用 >>>---

    // type_index返却
    //  iRaw==true  : ターゲット・クラスのもの
    //  iRaw==false : グローバル・バージョン番号テーブルに登録されているもの
    std::type_index getStdTypeIndex(bool iRaw) const
    {
        if (iRaw)
        {
    return std::type_index(typeid(TheolizerTarget));
        }

        typedef typename tClassType::Theolizer::DefineUniqueName::UniqueClass UniqueClass;
        return std::type_index(typeid(UniqueClass));
    }
    // 最新版のバージョン番号返却
    static unsigned getLastVersionNo() {return tClassType::Theolizer::kLastVersionNo;}
    unsigned getLastVersionNoV() const {return getLastVersionNo();}
    // 型名返却
    std::string getTypeName(unsigned iVersionNo)
    {
        THEOLIZER_INTERNAL_ASSERT(BaseTypeInfo::mTypeIndex.isValid(),
            "Not registered class %1%.", getCName());
        return tClassType::Theolizer::getClassName(iVersionNo);
    }
    char const* getUniqueName()
    {
        return tClassType::Theolizer::getUniqueName();
    }
    // メンバ変数対応方法返却
    ElementsMapping getElementsMapping(unsigned iVersionNo)
    {
        return tClassType::Theolizer::getElementsMapping(iVersionNo);
    }

    // C言語名返却(デバッグ用)
    char const* getCName() const
    {
        return THEOLIZER_INTERNAL_TYPE_NAME(typename tClassType::TheolizerTarget);
    }

    // 要素枚挙用Range返却
    ElementRange getElementRange(unsigned iVersionNo)
    {
        return tClassType::Theolizer::getElementRange(iVersionNo);
    }

//      ---<<< ポリモーフィズム対応 >>>---

private:
    // 派生クラスのsaveTypeInstance/loadTypeInstance呼び出し登録用クラス
    struct HolderBase
    {
        virtual bool saveTypeInstance
        (
            BaseSerializer& iSerializer,
            tClassType*& iPointer,
            std::type_index iStdTypeIndex
        )=0;
        virtual bool loadTypeInstance
        (
            BaseSerializer& iSerializer,
            tClassType*& ioPointer,
            TypeIndexList& iTypeIndexList
        )=0;
        virtual bool setSaving
        (
            BaseSerializer& iSerializer,
            std::vector<SaveStat>& ioSaveStatList
        )=0;
        virtual void const* getDerivedPointer(TheolizerTarget* iBasePointer)=0;
        virtual TypeIndex getTypeIndex()=0;
        virtual ~HolderBase() { }
    };

    // 派生クラスのsaveTypeInstance/loadTypeInstance呼び出し用クラス
    template<class tDrivedClassType>
    struct Holder : public HolderBase
    {
        Holder() { }
        ~Holder() = default;

        bool saveTypeInstance
        (
            BaseSerializer& iSerializer,
            tClassType*& iPointer,
            std::type_index iStdTypeIndex
        )
        {
            typedef typename tClassType::TheolizerTarget    TheolizerTarget;
            tDrivedClassType* aPointer=
                static_cast<tDrivedClassType*>(reinterpret_cast<TheolizerTarget*>(iPointer));
            return ClassTypeInfo<tDrivedClassType>::getInstance().
                saveTypeInstance
                (
                    iSerializer,
                    aPointer,
                    iStdTypeIndex
                );
        }

        bool loadTypeInstance
        (
            BaseSerializer& iSerializer,
            tClassType*& ioPointer,
            TypeIndexList& iTypeIndexList
        )
        {
            tDrivedClassType* aPointer=
                static_cast<tDrivedClassType*>(reinterpret_cast<TheolizerTarget*>(ioPointer));
            bool ret=ClassTypeInfo<tDrivedClassType>::getInstance().
                loadTypeInstance
                (
                    iSerializer,
                    aPointer,
                    iTypeIndexList
                );
            ioPointer=reinterpret_cast<tClassType*>(static_cast<TheolizerTarget*>(aPointer));
            return ret;
        }

        bool setSaving(BaseSerializer& iSerializer, std::vector<SaveStat>& ioSaveStatList)
        {
            return ClassTypeInfo<tDrivedClassType>::getInstance().
                setSaving(iSerializer, ioSaveStatList);
        }

        void const* getDerivedPointer(TheolizerTarget* iBasePointer)
        {
            void* aPointer=static_cast<tDrivedClassType*>(iBasePointer);
            return aPointer;
        }

        TypeIndex getTypeIndex()
        {
            return ClassTypeInfo<tDrivedClassType>::getInstance().mTypeIndex;
        }
    };

    // 派生クラス・リスト
    std::vector<std::unique_ptr<HolderBase> >   mDrivedClassList;

public:
    // 派生クラスをmDrivedClassListへ登録する
    template<class tDrivedClassType>
    void registerDrivedClass()
    {
//std::cout << "registerDrivedClass<" << THEOLIZER_INTERNAL_TYPE_NAME(tDrivedClassType)
//          << " base=" << THEOLIZER_INTERNAL_TYPE_NAME(typename tClassType::TheolizerTarget)
//          << "\n";
        mDrivedClassList.emplace_back(new Holder<tDrivedClassType>);
    }

    // 型、および、インスタンス保存(privateだとHolderからの呼び出しができない。friend化が難しい。)
    bool saveTypeInstance
    (
        BaseSerializer& iSerializer,
        tClassType*& iPointer,
        std::type_index iStdTypeIndex
    );

    // 型、および、インスタンス回復
    bool loadTypeInstance
    (
        BaseSerializer& iSerializer,
        tClassType*& ioPointer,
        TypeIndexList& iTypeIndexList
    );

    // 当クラスから派生したクラスについてヘッダへ型情報記録するよう指示する
    //  自分より前にあるクラスについて保存要求したら、true返却
    bool setSaving(BaseSerializer& iSerializer, std::vector<SaveStat>& ioSaveStatList)
    {
        // 派生クラスが登録されていないなら、NOP
        if (!mDrivedClassList.size())
    return false;

//std::cout << "setSaving(" << getCName()
//          << " mDrivedClassList.size()=" << mDrivedClassList.size() << "\n";
        bool ret=false;
        for (auto&& aDrivedClass : mDrivedClassList)
        {
            TypeIndex aTypeIndex = aDrivedClass->getTypeIndex();
            SaveStat& aSaveStat = ioSaveStatList.at(aTypeIndex.getIndex());
//std::cout << "    " << aDrivedClass->getCName() << "\n";
            if (aSaveStat == essIdle)
            {
                aSaveStat=essSaving;
                if (aTypeIndex.getIndex() < mTypeIndex.getIndex())
                {
                    ret=true;
                }
            }
            if (aDrivedClass->setSaving(iSerializer, ioSaveStatList))
            {
                ret=true;
            }
        }
        return ret;
    }

    // 現インスタンスの先頭アドレス返却
    void const* getDerivedPointer(TheolizerTarget* iBasePointer);

private:
    // ターゲットクラスのstd::type_index返却
    std::type_index getTargetStdTypeIndex() const
    {
        return std::type_index(typeid(typename tClassType::TheolizerTarget));
    }

//      ---<<< メタ・シリアライズ用 >>>---

    unsigned getTypeFlags(unsigned iVersionNo)
    {
        // 自動型
        if (tClassType::Theolizer::kIsAuto)
    return tClassType::Theolizer::getTypeFlags(iVersionNo);

        // 手動型はここで求める(バージョン毎に異なる部分がない)
        unsigned ret=0;
        ret |= theolizer::internal::ecfOrder;
        return ret;
    }

    TypeKind getTypeKind()
    {
        std::underlying_type<TypeKind>::type ret = etkClassFlag;

        if (tClassType::Theolizer::kIsNonIntrusive)
        {
            ret |= etkNonIntrusiveFlag;
        }

        if (!tClassType::Theolizer::kIsAuto)
        {
            ret |= etkManualFlag;
        }

        if (!std::is_same
            <
                typename tClassType::Theolizer::DefineUniqueName::UniqueClass,
                typename tClassType::TheolizerTarget
            >::value)
        {
            ret |= etkTemplateFlag;
        }

        return static_cast<TypeKind>(ret);
    }
};

// ***************************************************************************
//      EnumType管理クラス(シングルトン)
//          enum型を型リストへ登録する
//          バージョン管理する。詳細T.B.D.
// ***************************************************************************

template<typename tEnumType>
class EnumTypeInfo : public BaseTypeInfo
{
private:
    typedef TheolizerNonIntrusive<tEnumType> EnumNonIntrusive;

    // コンストラクタ／デストラクタ
    EnumTypeInfo() : BaseTypeInfo(etcEnumType)
    {
//std::cout << "EnumTypeInfo() : " << getCName() << "\n";
        mTypeIndex = TypeInfoList::getInstance().registerType(this);
    }
public:
    static EnumTypeInfo& getInstance()
    {
        static EnumTypeInfo instance;
        return instance;
    }

    // コピー／ムーブ禁止(仮想関数があるのでis_trivially_copyableにならない)
    EnumTypeInfo(const EnumTypeInfo&)  = delete;
    EnumTypeInfo(      EnumTypeInfo&&) = delete;
    EnumTypeInfo& operator=(const EnumTypeInfo&)  = delete;
    EnumTypeInfo& operator=(      EnumTypeInfo&&) = delete;

//      ---<<< 型チェック用 >>>---

    // type_index返却
    std::type_index getStdTypeIndex(bool iRaw) const
    {return std::type_index(typeid(tEnumType));}
    // 最新版のバージョン番号返却
    static unsigned getLastVersionNo()
    {
        return EnumNonIntrusive::Theolizer::kLastVersionNo;
    }
    unsigned getLastVersionNoV() const {return getLastVersionNo();}
    // 型名返却
    std::string getTypeName(unsigned iVersionNo)
    {
        return EnumNonIntrusive::Theolizer::getEnumName(iVersionNo);
    }
    // C言語名返却(デバッグ用)
    char const* getCName() const
    {
        return THEOLIZER_INTERNAL_TYPE_NAME(tEnumType);
    }

    // 要素枚挙用Range返却
    ElementRange getElementRange(unsigned iVersionNo)
    {
        return EnumNonIntrusive::Theolizer::getElementRange(iVersionNo);
    }

//      ---<<< メタ・シリアライズ用 >>>---

    unsigned getTypeFlags(unsigned iVersionNo)
    {
        return EnumNonIntrusive::Theolizer::getTypeFlags(iVersionNo);
    }

//      ---<<< メタ・シリアライズ用 >>>---

    TypeKind getTypeKind() {return etkEnum;}
};

// ***************************************************************************
//      プリミティブ管理クラス(シングルトン)
//          プリミティブ型を型リストへ登録する
// ***************************************************************************

//----------------------------------------------------------------------------
//      プリミティブID生成
//----------------------------------------------------------------------------

template<typename tPrimitive, class tEnable=void>
struct PrimitiveId { };

#define THEOLIZER_INTERNAL_INTEGRAL(dSigned, dDigits, dName)                \
    template<typename tPrimitive>                                           \
    struct PrimitiveId                                                      \
    <                                                                       \
        tPrimitive,                                                         \
        EnableIf                                                            \
        <                                                                   \
               (std::numeric_limits<tPrimitive>::is_signed == dSigned)      \
            && (std::numeric_limits<tPrimitive>::radix == 2)                \
            && (std::numeric_limits<tPrimitive>::digits == dDigits)         \
            && (std::numeric_limits<tPrimitive>::max_exponent == 0)         \
        >                                                                   \
    >                                                                       \
    {                                                                       \
        static char const* getPrimitiveName()    {return dName;}            \
    }

#define THEOLIZER_INTERNAL_FLOAT(dDigits, dMaxExponent, dName)              \
    template<typename tPrimitive>                                           \
    struct PrimitiveId                                                      \
    <                                                                       \
        tPrimitive,                                                         \
        EnableIf                                                            \
        <                                                                   \
               (std::numeric_limits<tPrimitive>::is_signed == 1)            \
            && (std::numeric_limits<tPrimitive>::radix == 2)                \
            && (std::numeric_limits<tPrimitive>::digits == dDigits)         \
            && (std::numeric_limits<tPrimitive>::max_exponent == dMaxExponent)\
        >                                                                   \
    >                                                                       \
    {                                                                       \
        static char const* getPrimitiveName()    {return dName;}            \
    }

#define THEOLIZER_INTERNAL_STRING(dName)                                    \
    template<typename tPrimitive>                                           \
    struct PrimitiveId                                                      \
    <                                                                       \
        tPrimitive,                                                         \
        EnableIf                                                            \
        <                                                                   \
               (IsString<tPrimitive>::value)                                \
        >                                                                   \
    >                                                                       \
    {                                                                       \
        static char const* getPrimitiveName()    {return dName;}            \
    }

THEOLIZER_INTERNAL_INTEGRAL(0,  1,  "bool"    );
THEOLIZER_INTERNAL_INTEGRAL(0,  8,  "unit8"   );
THEOLIZER_INTERNAL_INTEGRAL(1,  7,   "int8"   );
THEOLIZER_INTERNAL_INTEGRAL(0, 16,  "uint16"  );
THEOLIZER_INTERNAL_INTEGRAL(1, 15,   "int16"  );
THEOLIZER_INTERNAL_INTEGRAL(0, 32,  "uint32"  );
THEOLIZER_INTERNAL_INTEGRAL(1, 31,   "int32"  );
THEOLIZER_INTERNAL_INTEGRAL(0, 64,  "uint64"  );
THEOLIZER_INTERNAL_INTEGRAL(1, 63,   "int64"  );
THEOLIZER_INTERNAL_FLOAT(24,   128, "float32" );
THEOLIZER_INTERNAL_FLOAT(53,  1024, "float64" );
THEOLIZER_INTERNAL_FLOAT(64, 16384, "float80" );
THEOLIZER_INTERNAL_FLOAT(113,16384, "float128");
THEOLIZER_INTERNAL_STRING(          "string"  );
//THEOLIZER_INTERNAL_BYTES(           "bytes"   );

#undef  THEOLIZER_INTERNAL_INTEGRAL
#undef  THEOLIZER_INTERNAL_FLOAT
#undef  THEOLIZER_INTERNAL_STRING

template<typename tType>
char const* getPrimitiveName()
{
    static_assert(Ignore<tType>::kFalse, "Unknown primitive name.");
    return "";
}

#define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType)                             \
    template<>                                                              \
    inline char const* getPrimitiveName<dType>()                            \
    {                                                                       \
        return PrimitiveId<dType>::getPrimitiveName();                      \
    }
#include "primitive.inc"

//----------------------------------------------------------------------------
//      本体
//----------------------------------------------------------------------------

template<typename tPrimitiveType>
class THEOLIZER_INTERNAL_DLL PrimitiveTypeInfo : public BaseTypeInfo
{
private:
    // コンストラクタ／デストラクタ
    PrimitiveTypeInfo() : BaseTypeInfo(etcPrimitiveType)
    {
//std::cout << "PrimitiveTypeInfo() : " << getPrimitiveName<tPrimitiveType>() << "\n";
        mTypeIndex = TypeInfoList::getInstance().registerType(this);
    }
public:
    static PrimitiveTypeInfo& getInstance();

    // コピー／ムーブ禁止(仮想関数があるのでis_trivially_copyableにならない)
    PrimitiveTypeInfo(const PrimitiveTypeInfo&)  = delete;
    PrimitiveTypeInfo(      PrimitiveTypeInfo&&) = delete;
    PrimitiveTypeInfo& operator=(const PrimitiveTypeInfo&)  = delete;
    PrimitiveTypeInfo& operator=(      PrimitiveTypeInfo&&) = delete;

//      ---<<< 型チェック用 >>>---

    // type_index返却
    std::type_index getStdTypeIndex(bool iRaw) const
    {
        return BaseTypeFunctions::getInstance().getStdTypeIndex();
    }
    // 最新版のバージョン番号返却(シリアライザのバージョン番号を返却する)
    static unsigned getLastVersionNo()
    {
        return BaseTypeFunctions::getInstance().getLastVersionNo();
    }
    unsigned getLastVersionNoV() const {return getLastVersionNo();}
    // 型名返却
    std::string getTypeName(unsigned)
    {
        return getPrimitiveName<tPrimitiveType>();
    }
    // C言語名返却(デバッグ用)
    char const* getCName() const
    {
        return THEOLIZER_INTERNAL_TYPE_NAME(tPrimitiveType);
    }

//      ---<<< メタ・シリアライズ用 >>>---

    TypeKind    getTypeKind() {return etkPrimitive;}
};

// ***************************************************************************
//      追加のTypeIndex管理クラス(シングルトン)
// ***************************************************************************

template<typename tAdditionalClass>
class THEOLIZER_INTERNAL_DLL AdditionalTypeInfo : public BaseTypeInfo
{
private:
    // コンストラクタ／デストラクタ
    AdditionalTypeInfo() : BaseTypeInfo(etcPrimitiveType)
    {
//std::cout << "AdditionalTypeInfo() : " << getCName() << "\n";
        mTypeIndex = TypeInfoList::getInstance().registerType(this);
    }
public:
    static AdditionalTypeInfo& getInstance()
    {
        static AdditionalTypeInfo instance;
        return instance;
    }

    // コピー／ムーブ禁止(仮想関数があるのでis_trivially_copyableにならない)
    AdditionalTypeInfo(const AdditionalTypeInfo&)  = delete;
    AdditionalTypeInfo(      AdditionalTypeInfo&&) = delete;
    AdditionalTypeInfo& operator=(const AdditionalTypeInfo&)  = delete;
    AdditionalTypeInfo& operator=(      AdditionalTypeInfo&&) = delete;

//      ---<<< 型チェック用 >>>---

    // type_index返却
    std::type_index getStdTypeIndex(bool iRaw) const
    {return std::type_index(typeid(tAdditionalClass));}
    // 最新版のバージョン番号返却(シリアライザのバージョン番号を返却する)
    static unsigned getLastVersionNo() {return tAdditionalClass::kLastVersionNo;}
    unsigned getLastVersionNoV() const {return getLastVersionNo();}
    // 型名返却
    std::string getTypeName(unsigned)
    {return tAdditionalClass::kName;}

    // C言語名返却(デバッグ用)
    char const* getCName() const
    {
        return THEOLIZER_INTERNAL_TYPE_NAME(tAdditionalClass);
    }

//      ---<<< メタ・シリアライズ用 >>>---

//    TypeKind    getTypeKind() {return etkPrimitive;}
};

// ***************************************************************************
//      TypeをTypeInfoListへ登録する
// ***************************************************************************

template<typename tType>
TypeIndex getTypeIndex();

//----------------------------------------------------------------------------
//      TypeInfoを求める
//----------------------------------------------------------------------------

//      ---<<< プライマリ >>>---

template<typename tType, class tEnable=void>
struct GetTypeInfo
{
    static_assert(Ignore<tType>::kFalse, "This is not supported type.");
};

//      ---<<< ポインタ型 >>>---

template<typename tType>
struct GetTypeInfo<tType, EnableIf<std::is_pointer<tType>::value> >
{
    typedef PointerTypeInfo<tType>          Type;
};

//      ---<<< 生配列 >>>---

template<typename tType>
struct GetTypeInfo<tType, EnableIf<std::is_array<tType>::value> >
{
    typedef ArrayTypeInfo<tType>            Type;
};

//      ---<<< 侵入型 >>>---

template<typename tType>
struct GetTypeInfo<tType, EnableIf<IsIntrusive<tType>::value> >
{
    typedef ClassTypeInfo<tType>            Type;
};

//      ---<<< 非侵入型 >>>---

template<typename tType>
struct GetTypeInfo<tType, EnableIf<IsNonIntrusive<tType>::value> >
{
private:
    typedef TheolizerNonIntrusive<tType> TheolizerClass;
public:
    typedef ClassTypeInfo<TheolizerClass>   Type;
};

//      ---<<< enum型 >>>---

template<typename tType>
struct GetTypeInfo<tType, EnableIf<std::is_enum<tType>::value> >
{
    typedef EnumTypeInfo<tType>             Type;
};

//      ---<<< プリミティブ型 >>>---

template<typename tType>
struct GetTypeInfo<tType, EnableIf<IsPrimitive<tType>::value> >
{
    typedef PrimitiveTypeInfo<tType>        Type;
};

//      ---<<< 派生シリアライザ・バージョン定義クラス >>>---

template<typename tType>
struct GetTypeInfo<tType, EnableIf<IsAdditionalType<tType>::value> >
{
    typedef AdditionalTypeInfo<tType>       Type;
};

//      ---<<< 参照型 >>>---

template<typename tType>
struct GetTypeInfo<tType, EnableIf<std::is_lvalue_reference<tType>::value> >
{
private:
    typedef typename std::remove_reference<tType>::type RemovedType;
public:
    typedef typename GetTypeInfo<RemovedType>::Type Type;
};

//----------------------------------------------------------------------------
//      基底クラスへ登録する
//----------------------------------------------------------------------------

template<typename tType, bool uIsDerived>
struct RegisterToBaseClassEntrance
{
    void operator()(BaseTypeInfo*) { }
};

template<typename tType>
struct RegisterToBaseClassEntrance<tType, true>
{
    void operator()(BaseTypeInfo* iBaseTypeInfo)
    {
        typedef typename tType::TheolizerClass      TheolizerClass;
        typedef typename TheolizerClass::Theolizer  Theolizer;
        Theolizer::registerToBaseClass(iBaseTypeInfo);
    }
};

//----------------------------------------------------------------------------
//      登録本体
//----------------------------------------------------------------------------

template
<
    class tSerializer,
    typename tType,
    class tTheolizerVersion=::tTheolizerVersion,
    bool uIsDerived=false,
    bool uIsManual=false
>
class RegisterType
{
private:
    typedef typename RemoveCV<tType>::type      RemovedCVType;

    static RegisterType& mInstance;
    static void use(const RegisterType&) {}

    // コンストラクタ
    //  型リストへ登録し、型IDを入手する。
    RegisterType()
    {
        // XxxTypeInfo生成
        typedef typename GetTypeInfo<RemovedCVType>::Type   TypeInfo;
        auto& aBaseTypeInfo=TypeInfo::getInstance();

#if 1
std::cout << "RegisterType<" << THEOLIZER_INTERNAL_TYPE_NAME(tSerializer) << ",\n"
          << "             " << THEOLIZER_INTERNAL_TYPE_NAME(tType) << ",\n"
          << "             " << THEOLIZER_INTERNAL_TYPE_NAME(TypeInfo) << ",\n"
          << "             " << THEOLIZER_INTERNAL_TYPE_NAME(tTheolizerVersion) << ",\n"
          << "             uIsDerived=" << uIsDerived << ">\n";
std::cout << "    mTypeIndex=" << aBaseTypeInfo.getTypeIndex() << "\n";
#endif

        // 保存先があるTopLevelシリアライザなら、TypeInfoに保存先を登録する
        if (tSerializer::kHasDestination)
        {
std::cout << "    addDestination(" << tSerializer::getDestinations() << ")\n";
            TypeIndex aTypeIndex = aBaseTypeInfo.getTypeIndex();
            unsigned  aIndex = aTypeIndex.getIndex();
            TypeInfoList& aTypeInfoList = TypeInfoList::getInstance();

            // ポインタならば、ポイント先とその派生クラスをトップ・レベル扱いとする
            if (aBaseTypeInfo.mTypeCategory == etcPointerType)
            {
                aTypeInfoList[aIndex]->addDestination(tSerializer::getDestinations());
                aTypeInfoList[aIndex]->addPointerDestination(tSerializer::getDestinations());
std::cout << "    " << aTypeInfoList[aIndex]->getCName() << ".addDestination()/addPointerDestination()\n";
            }

            // 配列ならば、基本型をトップ・レベル扱いとする
            else if (aBaseTypeInfo.mTypeCategory == etcArrayType)
            {
                aTypeInfoList[aIndex]->addDestination(tSerializer::getDestinations());
std::cout << "    " << aTypeInfoList[aIndex]->getCName() << ".addDestination()\n";
            }

            else
            {
std::cout << "    aBaseTypeInfo.addDestination()\n";
                aBaseTypeInfo.addDestination(tSerializer::getDestinations());
            }
        }

        // ポリモーフィズム用派生クラスの登録なら、基底クラスのリストへ追加する
        RegisterToBaseClassEntrance<TypeInfo, uIsDerived>()(&aBaseTypeInfo);

        // 手動型のkElementListへ登録する
        tTheolizerVersion::addElement
        (
            []{return theolizer::internal::getTypeIndex<tType>();}
        );

        // 手動型からの保存設定
        aBaseTypeInfo.mIsManual=uIsManual;
    }
    // デストラクタ
    ~RegisterType() { }
public:
    static RegisterType& getInstance()
    {
        static RegisterType instance;
        use(mInstance);
        return instance;
    }

    // コピー／ムーブ禁止(デストラクタ定義がないとis_trivially_copyableになる)
    RegisterType(const RegisterType&)  = delete;
    RegisterType(      RegisterType&&) = delete;
    RegisterType& operator=(const RegisterType&)  = delete;
    RegisterType& operator=(      RegisterType&&) = delete;
};

// プロセス起動時にシングルトンを生成させる
template<class tSerializer, class tType, class tTheolizerVersion, bool uIsDerived, bool uIsManual>
RegisterType<tSerializer, tType, tTheolizerVersion, uIsDerived, uIsManual>&
        RegisterType<tSerializer, tType, tTheolizerVersion, uIsDerived, uIsManual>::mInstance
      = RegisterType<tSerializer, tType, tTheolizerVersion, uIsDerived, uIsManual>::getInstance();

//----------------------------------------------------------------------------
//      GVNTと派生シリアライザ登録専用関数テンプレート
//----------------------------------------------------------------------------

template<typename tType>
TypeIndex registerTypeIndex()
{
    // XxxTypeInfo生成し、登録する
    typedef typename GetTypeInfo<tType>::Type   TypeInfo;
    auto& aBaseTypeInfo=TypeInfo::getInstance();

    return aBaseTypeInfo.getTypeIndex();
}

// ***************************************************************************
//      Switcher2(管理する型毎に特殊化する)
//          型管理における型毎の分岐を実現する
// ***************************************************************************

//----------------------------------------------------------------------------
//      デフォルトSwitcher(エラー)
//----------------------------------------------------------------------------

template<typename tType, bool tIsRegister=false, class tEnable=void>
struct Switcher2
{
    // 最新版のバージョン番号返却
    static unsigned getLastVersionNo()
    {
        static_assert(Ignore<tType>::kFalse, "This is not serializable class.");
        return 0;
    }
    // 型名返却
    static std::string getTypeName(unsigned)
    {
        static_assert(Ignore<tType>::kFalse, "This is not serializable class.");
        return 0;
    }
    // TypeIndex返却
    static TypeIndex getTypeIndex()
    {
        static_assert(Ignore<tType>::kFalse, "This is not serializable class.");
        return TypeIndex();
    }
    // そのまま返却
    static void const* getDerivedPointer(tType* iPointer)
    {
        static_assert(Ignore<tType>::kFalse, "This is not serializable class.");
        return iPointer;
    }
};

//----------------------------------------------------------------------------
//      ポインタ型(オブジェクト追跡)用Switcher2
//----------------------------------------------------------------------------

template<typename tPointerType, bool tIsRegister>
struct Switcher2
<
    tPointerType,
    tIsRegister,
    EnableIf<std::is_pointer<tPointerType>::value>
>
{
    typedef typename RemoveCV<tPointerType>::type   PointerType;

    // 最新版のバージョン番号返却
//  static unsigned getLastVersionNo()
//  {
//      return PointerTypeInfo<PointerType>::getLastVersionNo();
//  }
    // 型名返却
//  static std::string getTypeName(unsigned iVersionNo)
//  {
//      return PointerTypeInfo<PointerType>::getInstance().getTypeName(iVersionNo);
//  }
    // TypeIndex返却
    static TypeIndex getTypeIndex()
    {
        return PointerTypeInfo<PointerType>::getInstance().mTypeIndex;
    }
    // これは不要(多重ポインタは非サポート)
//  static void const* getDerivedPointer(tPointerType* iPointer)
//  {
//      return iPointer;
//  }
};

//----------------------------------------------------------------------------
//      生配列型用Switcher2
//----------------------------------------------------------------------------

template<typename tArrayType, bool tIsRegister>
struct Switcher2
<
    tArrayType,
    tIsRegister,
    EnableIf<std::is_array<tArrayType>::value>
>
{
    typedef typename RemoveCV<tArrayType>::type     ArrayType;

    // 最新版のバージョン番号返却
//  static unsigned getLastVersionNo()
//  {
//      return ArrayTypeInfo<ArrayType>::getLastVersionNo();
//  }
    // 型名返却
//  static std::string getTypeName(unsigned iVersionNo)
//  {
//      return ArrayTypeInfo<ArrayType>::getInstance().getTypeName(iVersionNo);
//  }
    // TypeIndex返却
    static TypeIndex getTypeIndex()
    {
        return ArrayTypeInfo<ArrayType>::getInstance().mTypeIndex;
    }
    // そのまま返却
    static void const* getDerivedPointer(tArrayType* iPointer)
    {
        return iPointer;
    }
};

//----------------------------------------------------------------------------
//      ClassType用Switcher2
//----------------------------------------------------------------------------

//      ---<<< 侵入型用Switcher2本体 >>>---

template<class tClassType, bool tIsRegister>
struct Switcher2
<
    tClassType,
    tIsRegister,
    EnableIf<IsIntrusive<tClassType>::value>
>
{
    typedef typename std::remove_cv<tClassType>::type ClassType;

    // 最新版のバージョン番号返却
    static unsigned getLastVersionNo()
    {
        return ClassTypeInfo<ClassType>::getLastVersionNo();
    }
    // 型名返却
    static std::string getTypeName(unsigned iVersionNo)
    {
        return ClassTypeInfo<ClassType>::getInstance().getTypeName(iVersionNo);
    }
    // TypeIndex返却
    static TypeIndex getTypeIndex()
    {
        return ClassTypeInfo<ClassType>::getInstance().mTypeIndex;
    }
    // 派生クラスのアドレスを返却
    static void const* getDerivedPointer(tClassType* iPointer)
    {
        return ClassTypeInfo<ClassType>::getInstance().getDerivedPointer(iPointer);
    }
};

//      ---<<< 非侵入型用Switcher2本体 >>>---
//          対象クラスがconstの場合、TheolizerNonIntrusive<>が別物になるので、
//          constを外したTheolizerNonIntrusive<>の有無をチェックする

template<class tClassType, bool tIsRegister>
struct Switcher2
<
    tClassType,
    tIsRegister,
    EnableIf<IsNonIntrusive<tClassType>::value>
>
{
    typedef typename std::remove_cv<tClassType>::type   ClassNoConst;
    typedef TheolizerNonIntrusive<ClassNoConst>         ClassType;

    // 最新版のバージョン番号返却
    static unsigned getLastVersionNo()
    {
        return ClassTypeInfo<ClassType>::getLastVersionNo();
    }
    // 型名返却
    static std::string getTypeName(unsigned iVersionNo)
    {
        return ClassTypeInfo<ClassType>::getInstance().getTypeName(iVersionNo);
    }
    // TypeIndex返却
    static TypeIndex getTypeIndex()
    {
        return ClassTypeInfo<ClassType>::getInstance().mTypeIndex;
    }
    // 派生クラスのアドレスを返却
    static void const* getDerivedPointer(tClassType* iPointer)
    {
        return ClassTypeInfo<ClassType>::getInstance().getDerivedPointer(iPointer);
    }
};

//----------------------------------------------------------------------------
//      EnumType用Switcher2
//----------------------------------------------------------------------------

template<typename tEnumType, bool tIsRegister>
struct Switcher2
<
    tEnumType,
    tIsRegister,
    EnableIf<std::is_enum<tEnumType>::value>
>
{
    typedef typename std::remove_cv<tEnumType>::type    EnumType;

    // 最新版のバージョン番号返却
    static unsigned getLastVersionNo()
    {
        return EnumTypeInfo<EnumType>::getLastVersionNo();
    }
    // 型名返却
    static std::string getTypeName(unsigned iVersionNo)
    {
        return EnumTypeInfo<EnumType>::getInstance().getTypeName(iVersionNo);
    }
    // TypeIndex返却
    static TypeIndex getTypeIndex()
    {
        return EnumTypeInfo<EnumType>::getInstance().mTypeIndex;
    }
    // そのまま返却
    static void const* getDerivedPointer(tEnumType* iPointer)
    {
        return iPointer;
    }
};

//----------------------------------------------------------------------------
//      プリミティブ型用Switcher2
//----------------------------------------------------------------------------

//      ---<<< Primitive用Switcher2本体 >>>---
//      プリミティブへの参照の場合でも、save/loadPrimitiveにマッチする。
//      なので、参照の場合はここにマッチしないように条件を追加している。

template<typename tPrimitiveType, bool tIsRegister>
struct Switcher2
<
    tPrimitiveType,
    tIsRegister,
    EnableIf<IsPrimitive<tPrimitiveType>::value>
>
{
    typedef typename std::remove_cv<tPrimitiveType>::type   PrimitiveType;

    // 最新版のバージョン番号返却
    static unsigned getLastVersionNo()
    {
        return PrimitiveTypeInfo<PrimitiveType>::getLastVersionNo();
    }
    // 型名返却
    static std::string getTypeName(unsigned iVersionNo)
    {
        return PrimitiveTypeInfo<PrimitiveType>::getInstance().getTypeName(iVersionNo);
    }
    // TypeIndex返却
    static TypeIndex getTypeIndex()
    {
        return PrimitiveTypeInfo<PrimitiveType>::getInstance().mTypeIndex;
    }
    // そのまま返却
    static void const* getDerivedPointer(tPrimitiveType* iPointer)
    {
        return iPointer;
    }
};

// ***************************************************************************
//      中継処理
// ***************************************************************************

//----------------------------------------------------------------------------
//      TypeIndex取出し
//----------------------------------------------------------------------------

template<typename tType>
TypeIndex getTypeIndex()
{
    typedef typename std::remove_reference<tType>::type Type;
    return Switcher2<Type>::getTypeIndex();
}

//----------------------------------------------------------------------------
//      派生クラスの先頭アドレス返却
//          クラスでない場合は、そのまま返却
//----------------------------------------------------------------------------

template<typename tType>
void const* getDerivedPointer(tType* iPointer)
{
    return Switcher2<tType>::getDerivedPointer(iPointer);
}

//############################################################################
//      型名生成補助ツール
//############################################################################

// ***************************************************************************
//      クラス・テンプレート型名
// ***************************************************************************

//----------------------------------------------------------------------------
//      非型パラメータ用補助テンプレート
//----------------------------------------------------------------------------

template<typename tType, tType tValue, class tEnable=void>
struct NonType
{
    static std::string get()
    {
        return std::to_string(tValue);
    }
};

template<typename tType, tType tValue>
struct NonType<tType, tValue, EnableIf<std::is_enum<tType>::value> >
{
    static std::string get()
    {
        typedef typename std::underlying_type<tType>::type  Type;
        Type mValue=static_cast<Type>(tValue);
        return std::to_string(mValue);
    }
};

//----------------------------------------------------------------------------
//      テンプレート・パラメータ展開処理
//----------------------------------------------------------------------------

//      ---<<< プライマリー >>>---

template<typename... tRest>
struct ParameterName
{
    static std::string get(unsigned iVersionNo)
    {
        return "";
    }
};

//      ---<<< 非プリミティブ用特殊化 >>>---

template<typename tClass, typename... tRest>
struct ParameterName<tClass, tRest...>
{
    static std::string get(unsigned iVersionNo)
    {
        std::string aName=Switcher2<tClass>::getTypeName(iVersionNo);
        if (sizeof...(tRest))
        {
            aName = aName+","+ParameterName<tRest...>::get(iVersionNo);
        }
        return aName;
    }
};

//      ---<<< 非型用特殊化 >>>---

template<typename tType, tType tValue, typename... tRest>
struct ParameterName<NonType<tType, tValue>, tRest...>
{
    static std::string get(unsigned iVersionNo)
    {
        std::string aName=NonType<tType, tValue>::get();
        if (sizeof...(tRest))
        {
            aName = aName+","+ParameterName<tRest...>::get(iVersionNo);
        }
        return aName;
    }
};

//      ---<<< プリミティブ用特殊化 >>>---

#define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType)                             \
    template<typename... tRest>                                             \
    struct ParameterName<dType, tRest...>                                   \
    {                                                                       \
        static std::string get(unsigned iVersionNo)                         \
        {                                                                   \
            std::string aName=getPrimitiveName<dType>();                    \
            if (sizeof...(tRest)) {                                         \
                aName = aName+","+ParameterName<tRest...>::get(iVersionNo); \
            }                                                               \
            return aName;                                                   \
        }                                                                   \
    };
#include "primitive.inc"

#define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType)                             \
    template<typename... tRest>                                             \
    struct ParameterName<dType const, tRest...>                             \
    {                                                                       \
        static std::string get(unsigned iVersionNo)                         \
        {                                                                   \
            std::string aName=getPrimitiveName<dType>();                    \
            if (sizeof...(tRest)) {                                         \
                aName = aName+","+ParameterName<tRest...>::get(iVersionNo); \
            }                                                               \
            return aName;                                                   \
        }                                                                   \
    };
#include "primitive.inc"

//      ---<<< void用特殊化 >>>---

/*!
 ... text ...
*/

template<typename... tRest>
struct ParameterName<void, tRest...>
{
    static std::string get(BaseSerializer& iSerializer)
    {
        std::string aName;
        aName="void";
        if (sizeof...(tRest))
        {
            aName = aName+","+ParameterName<tRest...>::get(iSerializer);
        }
        return aName;
    }
};

//----------------------------------------------------------------------------
//      テンプレート・パラメータ展開入口
//----------------------------------------------------------------------------

template<typename... tArgs>
char const* makeTemplateName(char const* iName, unsigned iVersionNo)
{
    static std::string gStaticString;

    gStaticString = std::string(iName) + "<"
                 + ParameterName<tArgs...>::get(iVersionNo)
                 + ">";
    return gStaticString.c_str();
}

#define THEOLIZER_INTERNAL_MAKE_TEMPLATE_NAME(dName, ...)                   \
    theolizer::internal::makeTemplateName<__VA_ARGS__>(dName, iVersionNo)

//############################################################################
//      End
//############################################################################

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal
}   // namespace theolizer

// ***************************************************************************
//          警告抑止解除
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#endif  // THEOLIZER_INTERNAL_CORE_TYPE_INFO_H
