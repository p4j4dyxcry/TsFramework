//!*******************************************************
//!	TsRenderSystem.h
//!
//!	
//!
//!	©	2016 Yuki Tsuneyama

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

	TsXMLElement();
	virtual ~TsXMLElement();

	TsString	  GetTextData();
	TsString	  GetName();
	TsXMLElement* Next();
	TsXMLElement* GetFirstChild();
	TsXMLElement* FindChild( const TsString& name );

	TsXMLElement* FindChildForAttribute(const TsString& attribute ,TsString value);
	TsXMLAttribute* GetAttribute( const TsString& name );
private:
	TsString m_name;
	TsString m_data;
	TsVector<TsXMLAttribute*> m_attributeList;

	TsXMLElement*		  m_parent;
	TsXMLElement*		  m_subling;
	TsXMLElement*		  m_firstChild;

	tinyxml2::XMLElement* m_element;
private:
	TsBool SetElement(tinyxml2::XMLElement*);
};

//==================================================
//! xml ファイルデータ
//==================================================
class TsXML
{
	friend class TsXMLElement;
public:
	TsXML();
	TsXML(const TsString&  filename);	//コンストラクタ

	TsBool LoadXML(const TsString& filename);

	virtual ~TsXML();

	TsXMLElement* GetRootNode();
	TsXMLElement* FindFirst(const TsString& name);
	TsBool SaveXml(const TsString& filename);

private:
	tinyxml2::XMLDocument m_docment;
	TsXMLElement* m_rootNode;
};
