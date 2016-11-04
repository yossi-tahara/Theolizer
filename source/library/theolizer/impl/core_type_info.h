//############################################################################
//      Theolizerライブラリのコア部(型情報リスト)
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

#if !defined(THEOLIZER_INTERNAL_CORE_TYPE_INFO_H)
#define THEOLIZER_INTERNAL_CORE_TYPE_INFO_H

#include "core_assorted.h"

//############################################################################
//      Begin
//############################################################################

// ***************************************************************************
//          DLL用の警告禁止
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable:4251)
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
//      型チェック用定義
// ***************************************************************************

//----------------------------------------------------------------------------
//      シリアライズ・データ上のTypeIndexリストの要素
//----------------------------------------------------------------------------

struct SerializedTypeIndex;

//----------------------------------------------------------------------------
//      シリアライズ・データ上の型名リストの要素
//----------------------------------------------------------------------------

struct SerializedTypeName;

// ***************************************************************************
//      型リスト(シングルトン)
//          派生シリアライザー毎に型リストを生成し、型IDを決定する。
// ***************************************************************************

class BaseTypeInfo;

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
//      型名リストの要素(型名→TypeIndex)
//----------------------------------------------------------------------------

struct TypeName
{
    char const* mTypeName;
    std::size_t mTypeIndex;

    TypeName(char const* iTypeName, std::size_t iTypeIndex) :
        mTypeName(iTypeName),
        mTypeIndex(iTypeIndex)
    { }
};
typedef std::vector<TypeName>   TypeNameList;

TypeNameList::const_iterator findTypeName(TypeNameList const& iTypeNameList,
                                          char const* iTypeName);

//----------------------------------------------------------------------------
//      リスト型
//----------------------------------------------------------------------------

typedef std::vector<std::size_t>    TypeIndexList;

using TypeInfoListImpl=std::vector<BaseTypeInfo*>;

//----------------------------------------------------------------------------
//      型リスト本体
//----------------------------------------------------------------------------

class BaseGlobalVersions;

class THEOLIZER_INTERNAL_DLL TypeInfoList
{
private:
    template<class, class, class, bool, bool> friend class RegisterType;

    TypeInfoList() : mPrimitiveTypeIndex(-1)    // 無効値
    { }

    // 型のリスト
    TypeInfoListImpl                        mList;

    // プリミティブのTypeIndex
    std::size_t                             mPrimitiveTypeIndex;

public:
    static TypeInfoList& getInstance();

    // コピー／ムーブ禁止(std::vector<>を使っているのでis_trivially_copyableにならない)
    TypeInfoList(const TypeInfoList&)  = delete;
    TypeInfoList(      TypeInfoList&&) = delete;
    TypeInfoList& operator=(const TypeInfoList&)  = delete;
    TypeInfoList& operator=(      TypeInfoList&&) = delete;

    // 型登録(TypeIndex返却)
    std::size_t registerType(BaseTypeInfo* iTypeInfo);

    // リスト返却
    TypeInfoListImpl& getList() {return mList;}

    // プリミティブのTypeIndex返却
    std::size_t getPrimitiveTypeIndex() {return mPrimitiveTypeIndex;}
};

//----------------------------------------------------------------------------
//      現在のグローバル・バージョン番号に対応したローカル・バージョン番号
//          下記パラメータをハンドリングする
//              特定のグローバル・バージョン番号に対する
//                  各TypeIndexのローカル・バージョン番号表
//              メタ・シリアライズ／デシリアライズ・モード・フラグ
//----------------------------------------------------------------------------

class VersionNoList : public std::vector<unsigned>
{
    bool    mIsMetaMode;
public:
    VersionNoList(bool iIsMetaMode=false) : mIsMetaMode(iIsMetaMode) { }
    bool isMetaMode() const {return mIsMetaMode;}
    THEOLIZER_INTERNAL_DLL unsigned at(std::size_t iTypeIndex) const;
};

// ***************************************************************************
//      型情報管理用基底クラス
// ***************************************************************************

//----------------------------------------------------------------------------
//      ポリモーフィズム対応用
//----------------------------------------------------------------------------

//      ---<<< registerToBaseClassImpl()をfriend指定するため >>>---

template<class tClassType>
class Element;

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
    friend THEOLIZER_INTERNAL_DLL 
    void registerToBaseClassImpl(ElementRange&&, BaseTypeInfo*, unsigned, unsigned);

    // TypeIndexを記録する
    void registerTypeIndex(std::size_t iTypeIndex)
    {
        mTypeIndex=iTypeIndex;
        mIsRegistered=true;
    }

    // トップ・レベルの保存先
    bool                    mIsTopLevel;
    Destinations            mTopDestinations;

    // 手動型からの保存
    bool                    mIsManual;

    // 被ポインタ
    bool                    mIsPointee;

    // 型種別
    TypeCategory            mTypeCategory;

protected:
    BaseTypeInfo(TypeCategory iTypeCategory) :
        mIsTopLevel(false),
        mIsManual(false),
        mIsPointee(false),
        mTypeCategory(iTypeCategory),
        mTypeIndex(-1), // Max値設定
        mIsRegistered(false)
    { }

    // TypeInfoList<>::mListへの登録先インデックス
protected:
    std::size_t             mTypeIndex;
private:
    bool                    mIsRegistered;

public:

//      ---<<< トップ・レベルの保存先関連 >>>---

    void addDestination
    (
        Destinations const& iDestinations
    )
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

        // 保存先が一致するなら保存する
        return iSerializerDestinations.isMatch(mTopDestinations);
    }

//      ---<<< 手動型からの保存 >>>---

    bool isManual() {return mIsManual;}

//      ---<<< 型チェック用 >>>---

public:

    // 下記は各TypeInfoで定義する
    virtual std::type_index getStdTypeIndex(bool iRaw=false) const = 0;
    virtual unsigned        getLastVersionNoV() const = 0;
    virtual std::string     getTypeName(VersionNoList const& iVersionNoList) = 0;

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
    bool isTopLovel() {return mIsTopLevel;}

//      ---<<< 被ポインタ・チェック用 >>>---

    bool isPointee() { return mIsPointee; }

//      ---<<< ポリモーフィズム対応 >>>---

private:
    // 派生クラスのリスト
    typedef std::vector<BaseTypeInfo*>  DrivedClassList;
    std::vector<DrivedClassList>        mDrivedClassListVersions;

    // 基底クラスのmDrivedClassListVersionsへ自分(派生クラスのBaseTypeInfo)を登録する
    virtual void registerToBaseClass() { }

    // 当クラスから派生したクラスについてヘッダへ型情報記録するよう指示する
    //  自分より前にあるクラスについて保存要求したら、true返却
    bool setSaving(BaseSerializer& iSerializer, std::vector<SaveStat>& ioSaveStatList);

    // 被ポインタのBaseTypeInfo*獲得
    virtual BaseTypeInfo* getPointeeTypeInfo() {return nullptr;}

    // 型、および、インスタンス保存(保存したらtrue返却)
    virtual bool saveTypeInstance
    (
        BaseSerializer& iSerializer,
        void*& iPointer,
        std::type_index iStdTypeIndex
    )
    {return false;}

    // 型、および、インスタンス回復(回復したらtrue返却)
    virtual bool loadTypeInstance
    (
        BaseSerializer& iSerializer,
        void*& ioPointer,
        TypeIndexList& iTypeIndexList
    )
    {return false;}

//      ---<<< メタ・シリアライズ用 >>>---

    // enum型の時、iVersionNoは有効。その他はDon't care
    virtual unsigned getTypeFlags(unsigned iVersionNo) {return 0;}

    virtual TypeKind getTypeKind() = 0;
    virtual std::size_t getUnderlyingTypeIndex()
    {THEOLIZER_INTERNAL_ABORT("BaseTypeInfo::getUnderlyingTypeIndex()");}

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
    PointerTypeInfo() : BaseTypeInfo(etcPointerType) { }
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
    static unsigned getLastVersionNo();
    unsigned getLastVersionNoV() const {return getLastVersionNo();}
    // 型名返却
    std::string getTypeName(VersionNoList const& iVersionNoList);
    // C言語名返却(デバッグ用)
    char const* getCName() const
    {
        return THEOLIZER_INTERNAL_TYPE_NAME(tPointerType);
    }

    // 被ポインタのBaseTypeInfo*獲得
    BaseTypeInfo* getPointeeTypeInfo();

//      ---<<< メタ・シリアライズ用 >>>---

    TypeKind    getTypeKind()
    {
        return getPointeeTypeInfo()->getTypeKind();
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
//      ArrayType管理クラス本体
//----------------------------------------------------------------------------

template<typename tArrayType>
class ArrayTypeInfo : public BaseTypeInfo
{
private:
    // コンストラクタ／デストラクタ
    ArrayTypeInfo() : BaseTypeInfo(etcArrayType) { }
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
    static unsigned getLastVersionNo();
    unsigned getLastVersionNoV() const {return getLastVersionNo();}
    // 型名返却
    std::string getTypeName(VersionNoList const& iVersionNoList);
    // C言語名返却(デバッグ用)
    char const* getCName() const
    {
        return THEOLIZER_INTERNAL_TYPE_NAME(tArrayType);
    }

//      ---<<< メタ・シリアライズ用 >>>---

    // 基本型のTypeIndex返却
    std::size_t getUnderlyingTypeIndex();

    TypeKind    getTypeKind()
    {
        BaseTypeInfo* aTypeInfo =
            TypeInfoList::getInstance().getList().at(getUnderlyingTypeIndex());
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

private:
    // コンストラクタ／デストラクタ
    ClassTypeInfo() : BaseTypeInfo(etcClassType) { }
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
    return std::type_index(typeid(typename tClassType::TheolizerTarget));
        }

        typedef typename tClassType::Theolizer::DefineUniqueName::UniqueClass UniqueClass;
        return std::type_index(typeid(UniqueClass));
    }
    // 最新版のバージョン番号返却
    static unsigned getLastVersionNo() {return tClassType::Theolizer::kLastVersionNo;}
    unsigned getLastVersionNoV() const {return getLastVersionNo();}
    // 型名返却
    std::string getTypeName(VersionNoList const& iVersionNoList)
    {
        return tClassType::Theolizer::getClassName
                (
                    iVersionNoList,
                    iVersionNoList.at(BaseTypeInfo::mTypeIndex)
                );
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
    // 基底クラスのmDrivedClassListVersionsへ自分(派生クラスのBaseTypeInfo)を登録する
    virtual void registerToBaseClass()
    {
        tClassType::Theolizer::registerToBaseClass(this);
    }

    // 型、および、インスタンス保存
    bool saveTypeInstance
    (
        BaseSerializer& iSerializer,
        void*& iPointer,
        std::type_index iStdTypeIndex
    );

    // 型、および、インスタンス回復
    bool loadTypeInstance
    (
        BaseSerializer& iSerializer,
        void*& ioPointer,
        TypeIndexList& iTypeIndexList
    );

    // ターゲットクラスのstd::type_index返却
    std::type_index getTargetStdTypeIndex() const
    {
        return std::type_index(typeid(typename tClassType::TheolizerTarget));
    }

//      ---<<< メタ・シリアライズ用 >>>---

    unsigned getTypeFlags(unsigned iVersionNo)
    {
        // 自動型
        if (tClassType::Theolizer::kIsAuto) {
    return tClassType::Theolizer::getTypeFlags(iVersionNo);
        }

        // 手動型はここで求める(バージョン毎に異なる部分がない)
        unsigned ret=0;
        ret |= theolizer::internal::ecfOrder;
        return ret;
    }

    TypeKind getTypeKind()
    {
        std::underlying_type<TypeKind>::type ret = etkClassFlag;

        if (tClassType::Theolizer::kIsNonIntrusive) {
            ret |= etkNonIntrusiveFlag;
        }

        if (!tClassType::Theolizer::kIsAuto) {
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
    EnumTypeInfo() : BaseTypeInfo(etcEnumType) { }
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
    std::string getTypeName(VersionNoList const& iVersionNoList)
    {
        return EnumNonIntrusive::Theolizer::getEnumName
            (
                iVersionNoList.
                at(BaseTypeInfo::mTypeIndex)
            );
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
//      PrimitiveType管理クラス本体
//----------------------------------------------------------------------------

template<typename tPrimitiveType>
class PrimitiveTypeInfo : public BaseTypeInfo
{
private:
    // コンストラクタ／デストラクタ
    PrimitiveTypeInfo() : BaseTypeInfo(etcPrimitiveType) { }
public:
    static PrimitiveTypeInfo& getInstance()
    {
        static PrimitiveTypeInfo instance;
        return instance;
    }

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
    std::string getTypeName(VersionNoList const& iVersionNoList)
    {
        unsigned aVersionNo = iVersionNoList.at(BaseTypeInfo::mTypeIndex);
        return BaseTypeFunctions::getInstance().getPrimitiveName(aVersionNo, tPrimitiveType());
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
//      TypeをTypeInfoListへ登録する
// ***************************************************************************

template<typename tType>
std::size_t getTypeIndex();

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
//      ポインタか参照の場合、ポイント／参照先の型を登録する
//----------------------------------------------------------------------------

//      ---<<< プライマリ >>>---

template
<
    class tSerializer,
    typename tType,
    class tTheolizerVersion,
    bool uIsDerived,
    bool uIsManual=false,
    class tEnable=void
>
struct RegisterPointee
{
    static void registerType() { }
};

//      ---<<< ポインタ型 >>>---

template<class tSerializer,typename tType,class tTheolizerVersion,bool uIsDerived,bool uIsManual>
struct RegisterPointee
<
    tSerializer,
    tType,
    tTheolizerVersion,
    uIsDerived,
    uIsManual,
    EnableIf<std::is_pointer<tType>::value>
>
{
    static void registerType();
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

    // 登録した型情報
    BaseTypeInfo*   mBaseTypeInfo;

    // コンストラクタ
    //  型リストへ登録し、型IDを入手する。
    RegisterType()
    {
        // XxxTypeInfo生成
        typedef typename GetTypeInfo<RemovedCVType>::Type   TypeInfo;
        mBaseTypeInfo=&TypeInfo::getInstance();

#if 0
std::cout << "RegisterType<" << THEOLIZER_INTERNAL_TYPE_NAME(tSerializer) << ",\n"
          << "             " << THEOLIZER_INTERNAL_TYPE_NAME(tType) << ",\n"
          << "             " << THEOLIZER_INTERNAL_TYPE_NAME(TypeInfo) << ",\n"
          << "             " << THEOLIZER_INTERNAL_TYPE_NAME(tTheolizerVersion) << ",\n"
          << "             uIsDerived=" << uIsDerived << ">\n";
#endif

        // 未登録なら、TypeInfoListへBaseTypeInfoを登録する
        std::size_t aTypeIndex=0;
        if (mBaseTypeInfo->mIsRegistered)
        {
            aTypeIndex=mBaseTypeInfo->mTypeIndex;
//std::cout << "    aTypeIndex=" << aTypeIndex << " (Registered)\n";
        }
        else
        {
            aTypeIndex=TypeInfoList::getInstance().registerType(mBaseTypeInfo);
//std::cout << "    aTypeIndex=" << aTypeIndex << "\n";

            // TypeIndexをBaseTypeInfoへ設定する
            mBaseTypeInfo->registerTypeIndex(aTypeIndex);
        }

        // ポリモーフィズム用派生クラスの登録なら、基底クラスのリストへ追加する
        if (uIsDerived)
        {
            mBaseTypeInfo->registerToBaseClass();
        }

        // ポインタなら、その先の型を登録する
        RegisterPointee
        <
            tSerializer,
            RemovedCVType,
            tTheolizerVersion,
            uIsDerived,
            uIsManual
        >::registerType();

        // 手動型のkElementListへ登録する
        tTheolizerVersion::addElement
        (
            []{return theolizer::internal::getTypeIndex<tType>();}
        );
    }
    // デストラクタ
    ~RegisterType() { }
public:
    static RegisterType& getInstance()
    {
        static RegisterType instance;
        use(mInstance);

        // 保存先があるTopLevelシリアライザなら、TypeInfoに保存先を登録する
        if (tSerializer::kHasDestination)
        {
            instance.mBaseTypeInfo->addDestination(tSerializer::kDestinations);
        }

        // 手動型からの保存設定
        instance.mBaseTypeInfo->mIsManual=uIsManual;

        return instance;
    }

    // コピー／ムーブ禁止(デストラクタ定義がないとis_trivially_copyableになる)
    RegisterType(const RegisterType&)  = delete;
    RegisterType(      RegisterType&&) = delete;
    RegisterType& operator=(const RegisterType&)  = delete;
    RegisterType& operator=(      RegisterType&&) = delete;

    // 被ポインタ登録
    void setPointee() { mBaseTypeInfo->mIsPointee=true; }
};

// プロセス起動時にシングルトンを生成させる
template<class tSerializer, class tType, class tTheolizerVersion, bool uIsDerived, bool uIsManual>
RegisterType<tSerializer, tType, tTheolizerVersion, uIsDerived, uIsManual>&
        RegisterType<tSerializer, tType, tTheolizerVersion, uIsDerived, uIsManual>::mInstance
      = RegisterType<tSerializer, tType, tTheolizerVersion, uIsDerived, uIsManual>::getInstance();

//----------------------------------------------------------------------------
//      ポインタの場合、ポイント先の型を登録する関数本体
//----------------------------------------------------------------------------

//      ---<<< ポインタ型 >>>---

template<class tSerializer,typename tType,class tTheolizerVersion,bool uIsDerived,bool uIsManual>
void RegisterPointee
<
    tSerializer,
    tType,
    tTheolizerVersion,
    uIsDerived,
    uIsManual,
    EnableIf<std::is_pointer<tType>::value>
>::registerType()
{
    RegisterType
    <
        tSerializer,
        typename std::remove_pointer<tType>::type,
        tTheolizerVersion,
        uIsDerived,
        uIsManual
    >::getInstance().setPointee();
};

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
    static std::string getTypeName(VersionNoList const& iVersionNoList)
    {
        static_assert(Ignore<tType>::kFalse, "This is not serializable class.");
        return 0;
    }
    // TypeIndex返却
    static std::size_t getTypeIndex()
    {
        static_assert(Ignore<tType>::kFalse, "This is not serializable class.");
        return 0;
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
    static unsigned getLastVersionNo()
    {
        return PointerTypeInfo<PointerType>::getLastVersionNo();
    }
    // 型名返却
    static std::string getTypeName(VersionNoList const& iVersionNoList)
    {
        return PointerTypeInfo<PointerType>::getInstance().getTypeName(iVersionNoList);
    }
    // TypeIndex返却
    static std::size_t getTypeIndex()
    {
        return PointerTypeInfo<PointerType>::getInstance().mTypeIndex;
    }
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
    static unsigned getLastVersionNo()
    {
        return ArrayTypeInfo<ArrayType>::getLastVersionNo();
    }
    // 型名返却
    static std::string getTypeName(VersionNoList const& iVersionNoList)
    {
        return ArrayTypeInfo<ArrayType>::getInstance().getTypeName(iVersionNoList);
    }
    // TypeIndex返却
    static std::size_t getTypeIndex()
    {
        return ArrayTypeInfo<ArrayType>::getInstance().mTypeIndex;
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
    static std::string getTypeName(VersionNoList const& iVersionNoList)
    {
        return ClassTypeInfo<ClassType>::getInstance().getTypeName(iVersionNoList);
    }
    // TypeIndex返却
    static std::size_t getTypeIndex()
    {
        return ClassTypeInfo<ClassType>::getInstance().mTypeIndex;
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
    static std::string getTypeName(VersionNoList const& iVersionNoList)
    {
        return ClassTypeInfo<ClassType>::
            getInstance().getTypeName(iVersionNoList);
    }
    // TypeIndex返却
    static std::size_t getTypeIndex()
    {
        return ClassTypeInfo<ClassType>::getInstance().mTypeIndex;
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
    static std::string getTypeName(VersionNoList const& iVersionNoList)
    {
        return EnumTypeInfo<EnumType>::getInstance().getTypeName(iVersionNoList);
    }
    // TypeIndex返却
    static std::size_t getTypeIndex()
    {
        return EnumTypeInfo<EnumType>::getInstance().mTypeIndex;
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
    static std::string getTypeName(VersionNoList const& iVersionNoList)
    {
        return PrimitiveTypeInfo<PrimitiveType>::getInstance().getTypeName(iVersionNoList);
    }
    // TypeIndex返却
    static std::size_t getTypeIndex()
    {
        return PrimitiveTypeInfo<PrimitiveType>::getInstance().mTypeIndex;
    }
};

//----------------------------------------------------------------------------
//      TypeIndex取出し
//----------------------------------------------------------------------------

template<typename tType>
std::size_t getTypeIndex()
{
    typedef typename std::remove_reference<tType>::type Type;
    return Switcher2<Type>::getTypeIndex();
}

// ***************************************************************************
//      TypeInfoからのSwitcher2呼び出し
// ***************************************************************************

//----------------------------------------------------------------------------
//      PointerTypeInfo
//          aIndexについて
//              iVersionNoは
//                  型がClassTypeの場合、プリミティブ化されている時
//                      iSerializerVersionNoが変化する
//                  型がプリミティブの場合、iSerializerVersionNoは0固定
//                  型がenumの場合、iSerializerVersionNoは0固定
//              tMidSerializerがiVersionNoとiSerializerVersionNoの組み合わせを
//              決定する。
//              また、iVersionNoとiSerializerVersionNoは、両方共単調増加である。
//              従って、同じaIndex値となるiVersionNoとiSerializerVersionNoの
//              組み合わせはtMidSerializer毎に一意である。
//  例：
//  プリミティブ化状況  ClassType 派生Serialzier  GlobalVersionNo aIndex
//  ノーマル                0           0               0               0
//  プリミティブ化          0           1               1               1
//  クラスVersionUp         1           1               2               2
//  他クラスVersionUp       1           1               3               2
//  Serializer VersionUp    1           2               4               3
//  非プリミティブ化        1           3               5               4
//  クラスVersionUp         2           3               6               5
//----------------------------------------------------------------------------

//      ---<<< 型名獲得 >>>---

template<typename tPointerType>
unsigned PointerTypeInfo<tPointerType>::getLastVersionNo()
{
    typedef typename std::remove_pointer<tPointerType>::type    ParentType;
    return Switcher2<ParentType>::getLastVersionNo();
}

template<typename tPointerType>
std::string PointerTypeInfo<tPointerType>::
    getTypeName(VersionNoList const& iVersionNoList)
{
    typedef typename std::remove_pointer<tPointerType>::type    ParentType;
    return Switcher2<ParentType>::getTypeName(iVersionNoList)+"*";
}

//      ---<<< ポイントされている型のBaseTypeInfo*獲得 >>>---

template<typename tPointerType>
BaseTypeInfo* PointerTypeInfo<tPointerType>::getPointeeTypeInfo()
{
    typedef typename std::remove_pointer<tPointerType>::type    ParentType;
    std::size_t aTypeIndex = Switcher2<ParentType>::getTypeIndex();
    return TypeInfoList::getInstance().getList().at(aTypeIndex);
}

//----------------------------------------------------------------------------
//      ArrayTypeInfo
//----------------------------------------------------------------------------

template<typename tArrayType>
unsigned ArrayTypeInfo<tArrayType>::getLastVersionNo()
{
    typedef typename std::remove_extent<tArrayType>::type   ParentType;
    return Switcher2<ParentType>::getLastVersionNo();
}

template<typename tArrayType>
std::string ArrayTypeInfo<tArrayType>::
    getTypeName(VersionNoList const& iVersionNoList)
{
    typedef typename std::remove_extent<tArrayType>::type   ParentType;
    std::string ret = Switcher2<ParentType>::getTypeName(iVersionNoList);
    if (iVersionNoList.isMetaMode())
    {
        // []の順序が逆になるのでひっくり返す
        std::size_t pos=ret.find('[');
        std::string aExtent;
        if (pos != std::string::npos)
        {
            aExtent = ret.substr(pos);
            ret     = ret.substr(0, pos);
        }
        ret.push_back('[');
        ret.append(std::to_string(std::extent<tArrayType>::value));
        ret.push_back(']');
        ret.append(aExtent);
    }
    else
    {   // []の順序は逆だが、中身がないのでひっくり返す必要無し
        ret.append("[]");
    }
    return ret;
}

// 基本型のTypeIndex返却
template<typename tArrayType>
std::size_t ArrayTypeInfo<tArrayType>::
    getUnderlyingTypeIndex()
{
    typedef typename std::remove_all_extents<tArrayType>::type UnderlyingType;
    return getTypeIndex<UnderlyingType>();
}

//############################################################################
//      型名生成補助ツール
//############################################################################

// ***************************************************************************
//      派生Serializerが提供するプリミティブ名獲得用クラスのプライマリー定義
// ***************************************************************************

template<class tMidSerializer, typename tPrimitive, class tEnable=void>
struct PrimitiveName { };

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
    static std::string get(VersionNoList const& iVersionNoList)
    {
        return "";
    }
};

//      ---<<< 非プリミティブ用特殊化 >>>---

template<typename tClass, typename... tRest>
struct ParameterName<tClass, tRest...>
{
    static std::string get(VersionNoList const& iVersionNoList)
    {
        std::string aName=Switcher2<tClass>::getTypeName(iVersionNoList);
        if (sizeof...(tRest)) {
            aName = aName+","+ParameterName<tRest...>::get(iVersionNoList);
        }
        return aName;
    }
};

//      ---<<< 非型用特殊化 >>>---

template<typename tType, tType tValue, typename... tRest>
struct ParameterName<NonType<tType, tValue>, tRest...>
{
    static std::string get(VersionNoList const& iVersionNoList)
    {
        std::string aName=NonType<tType, tValue>::get();
        if (sizeof...(tRest)) {
            aName = aName+","+ParameterName<tRest...>::get(iVersionNoList);
        }
        return aName;
    }
};

//      ---<<< プリミティブ用特殊化 >>>---

#define THEOLIZER_INTERNAL_DEF_PRIMITIVE(dType, dSymbol)                    \
    template<typename... tRest>                                             \
    struct ParameterName<dType, tRest...>                                   \
    {                                                                       \
        static std::string get(VersionNoList const& iVersionNoList)         \
        {                                                                   \
            std::size_t aTypeIndex=PrimitiveTypeInfo<dType>::getInstance().mTypeIndex;\
            unsigned aSerializerVersionNo=iVersionNoList.at(aTypeIndex);    \
            static dType aType;                                             \
            std::string aName=BaseTypeFunctions::getInstance().             \
                getPrimitiveName(aSerializerVersionNo, aType);              \
            if (sizeof...(tRest)) {                                         \
                aName = aName+","+ParameterName<tRest...>::get(iVersionNoList);\
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
        if (sizeof...(tRest)) {
            aName = aName+","+ParameterName<tRest...>::get(iSerializer);
        }
        return aName;
    }
};

//----------------------------------------------------------------------------
//      テンプレート・パラメータ展開入口
//----------------------------------------------------------------------------

template<typename... tArgs>
char const* makeTemplateName(char const* iName, VersionNoList const& iVersionNoList)
{
    static std::string gStaticString;

    if (iVersionNoList.isMetaMode())
    {
        gStaticString = std::string(iName) + "<"
                     + ParameterName<tArgs...>::get(iVersionNoList)
                     + ">";
return gStaticString.c_str();
    }
    else
    {
        gStaticString = std::string(iName) + "<"
                     + ParameterName<tArgs...>::get(iVersionNoList)
                     + ">";
return gStaticString.c_str();
    }
}

#define THEOLIZER_INTERNAL_MAKE_TEMPLATE_NAME(dName, ...)                   \
    theolizer::internal::makeTemplateName<__VA_ARGS__>(dName, iVersionNoList)

//############################################################################
//      End
//############################################################################

#endif  // THEOLIZER_INTERNAL_DOXYGEN
}   // namespace internal
}   // namespace theolizer

// ***************************************************************************
//          DLL用の警告禁止解除
// ***************************************************************************

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#endif  // THEOLIZER_INTERNAL_CORE_TYPE_INFO_H
