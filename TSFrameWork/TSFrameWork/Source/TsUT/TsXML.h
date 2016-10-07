//!*******************************************************
//! TsXML.h
//!
//! XmlPerserのLapper
//!
//! © 2016 Yuki Tsuneyama

#pragma once

#include "../../Extern/XML/tinyxml2.h"
#include "TsType.h"
#include "TsArrayType.h"

//==================================================
//! xml アトリビュート
//==================================================
class TsXMLAttribute
{
    friend class TsXMLElement;
public:
    TsXMLAttribute();
    //==================================================
    //	テキストデータを適正なデータに変換し取得します。
    //==================================================
    TsInt4 GetInt4Value();
    TsInt3 GetInt3Value();
    TsInt2 GetInt2Value();
    TsInt  GetIntValue();

    TsFloat4 GetFloat4Value();
    TsFloat3 GetFloat3Value();
    TsFloat2 GetFloat2Value();
    TsF32	 GetFloatValue();
    TsBool	 GetBoolValue();

    TsString GetStringValue();
private:
    tinyxml2::XMLAttribute* m_attribute;
};

//==================================================
//! xml エレメント
//==================================================
class TsXMLElement
{
    friend class TsXML;
public:

    //! コンストラクタ
    TsXMLElement();
    //! デストラクタ
    virtual ~TsXMLElement();

    //! テキストブロックを取得
    TsString	  GetTextData();
    //! エレメントの名前を取得
    TsString	  GetName();

    //! 次のエレメントを取得
    TsXMLElement* Next();

    //! 第一子のエレメントを取得
    TsXMLElement* GetFirstChild();

    //! エレメントを名前で検索し、取得する
    TsXMLElement* FindChild( const TsString& name );

    //! 子エレメントを検索する
    //  指定されたアトリュビュート名を持つエレメントが存在しそのデータ要素がvalueである
    //	子エレメントを取得する
    TsXMLElement* FindChildForAttribute(const TsString& attributeName ,TsString value);

    //! 指定された名前を持つアトリビュートを取得する
    TsXMLAttribute* GetAttribute( const TsString& name );
private:
    TsString m_name;							//! エレメントの名前
    TsString m_data;							//! エレメントが所持するテキストデータ
    TsVector<TsXMLAttribute*> m_attributeList;	//! アトリビュートのリスト

    TsXMLElement*		  m_parent;				//! 親エレメント
    TsXMLElement*		  m_subling;			//! 兄弟エレメント
    TsXMLElement*		  m_firstChild;			//! 第一子エレメント

    tinyxml2::XMLElement* m_element;			//! tinyxml2の本体
private:
    TsBool SetElement(tinyxml2::XMLElement*);	//! tinyxml2のデータを設定する
};

//==================================================
//! xml ファイルデータ
//==================================================
class TsXML
{
    friend class TsXMLElement;
public:
    //! コンストラクタ
    TsXML();
    TsXML(const TsString& filename);

    //デストラクタ
    virtual ~TsXML();

    //! XMLを読み込む
    TsBool LoadXML(const TsString& filename);

    //! ルートのエレメントを取得する
    TsXMLElement* GetRootNode();

    //! 名前で検索し最初に見つかったエレメントを取得する
    TsXMLElement* FindFirst(const TsString& name);

    //! XMLデータを保存する。
    //  todo 未実装
    TsBool SaveXml(const TsString& filename);

private:
    tinyxml2::XMLDocument m_docment;	// tinyxml2の本体
    TsXMLElement* m_rootNode;			// ルートエレメント
};
