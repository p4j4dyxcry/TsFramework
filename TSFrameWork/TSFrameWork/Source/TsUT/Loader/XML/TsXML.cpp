#include "../../TsUT.h"

#include <sstream>
#include <functional>
#include "TsXML.h"

namespace TsXmlUT
{
    //========================================================================================
    // string データから　数値の配列を取得します。
    //
    //	型とデータがマッチングしない場合は不正な結果が返ります、
    //
    //　例　Int の配列を取得するときに 0.0などの値が含まれていると不正な結果になり得ます
    //
    //========================================================================================
    template<typename Array>
    static Array ValueToArray4Type(TsString value)
    {
        Array data;
        std::stringstream ss(value);
        ss >>	data.x >>
                data.y >>
                data.z >>
                data.w;
        return data;
    }

    TsXMLElement* ElementSearchFromChildren(TsXMLElement* elm , TsString name)
    {
        if( elm == nullptr )
            return nullptr;

        if (elm->GetName() == name)
            return elm;
        auto child = elm->GetFirstChild();
        auto subling = elm->Next();

        if (child)
        {
            auto res = ElementSearchFromChildren(child,name);
            if (res)return res;
        }

        if (subling)
        {
            auto res = ElementSearchFromChildren(subling, name);
            if (res)return res;
        }

        return (TsXMLElement*)nullptr;
    };
}

TsXMLAttribute::TsXMLAttribute() :
m_attribute(nullptr)
{
    
};

//==============================================
//  TsXMLAttribute::GetInt4Value()
//  値をint4つとして解釈し取得します
//==============================================
TsInt4 TsXMLAttribute::GetInt4Value()
{
    if (m_attribute == nullptr)
        return TsInt4();
    return TsXmlUT::ValueToArray4Type<TsInt4>(GetStringValue());
}

//==============================================
//  TsXMLAttribute::GetInt3Value()
//  値をint3つとして解釈し取得します
//==============================================
TsInt3 TsXMLAttribute::GetInt3Value()
{
    if (m_attribute == nullptr)
        return TsInt3();
    return TsInt3(TsXmlUT::ValueToArray4Type<TsInt4>(GetStringValue()));
}

//==============================================
//  TsXMLAttribute::GetInt2Value()
//  値をint2つとして解釈し取得します
//==============================================
TsInt2 TsXMLAttribute::GetInt2Value()
{
    if (m_attribute == nullptr)
        return TsInt2();
    return TsInt2(TsXmlUT::ValueToArray4Type<TsInt4>(GetStringValue()));
}

//==============================================
//  TsXMLAttribute::GetIntValue()
//  値をintとして解釈し取得します
//==============================================
TsInt  TsXMLAttribute::GetIntValue()
{
    if (m_attribute == nullptr)
        return 0;
    return TsXmlUT::ValueToArray4Type<TsInt4>(GetStringValue()).x;
}

//==============================================
//  TsXMLAttribute::GetFloat4Value()
//  値をFloat4つとして解釈し取得します
//==============================================
TsFloat4 TsXMLAttribute::GetFloat4Value()
{
    if (m_attribute == nullptr)
        return TsFloat4();
    return TsXmlUT::ValueToArray4Type<TsFloat4>(GetStringValue());
}

//==============================================
//  TsXMLAttribute::GetFloat3Value()
//  値をFloat3つとして解釈し取得します
//==============================================
TsFloat3 TsXMLAttribute::GetFloat3Value()
{
    if (m_attribute == nullptr)
        return TsFloat3();
    return TsFloat3(TsXmlUT::ValueToArray4Type<TsFloat4>(GetStringValue()));
}

//==============================================
//  TsXMLAttribute::GetFloat2Value()
//  値をFloat2つとして解釈し取得します
//==============================================
TsFloat2 TsXMLAttribute::GetFloat2Value()
{
    if (m_attribute == nullptr)
        return TsFloat2();
    return TsFloat2(TsXmlUT::ValueToArray4Type<TsFloat4>(GetStringValue()));
}

//==============================================
//  TsXMLAttribute::GetFloatValue()
//  値をFloatとして解釈し取得します
//==============================================
TsF32	 TsXMLAttribute::GetFloatValue()
{
    if (m_attribute == nullptr)
        return 0;
    return TsXmlUT::ValueToArray4Type<TsFloat4>(GetStringValue()).x;
}

//==============================================
//  TsXMLAttribute::GetStringValue()
//  値をStringとして解釈し取得します
//==============================================
TsString TsXMLAttribute::GetStringValue()
{
    if (m_attribute == nullptr)
        return "nullptr";
    return m_attribute->Value();
}

//==============================================
//  TsXMLAttribute::GetBoolValue()
//  値をBoolとして解釈し取得します
//==============================================
TsBool TsXMLAttribute::GetBoolValue()
{
    if (m_attribute == nullptr)
        return TS_FALSE;
    return m_attribute->BoolValue();
}

TsXMLElement::TsXMLElement() :
m_element(nullptr),
m_firstChild(nullptr),
m_parent(nullptr),
m_subling(nullptr)
{
    m_attributeList.reserve( 1024 );
};

TsXMLElement::~TsXMLElement()
{
    TsSafeDelete(m_subling);
    TsSafeDelete(m_firstChild);

    for (auto it : m_attributeList)
        TsSafeDelete(it);
}

//==============================================
//  TsXMLElement::GetTextData()
//  エレメントをテキストとしてすべて取得します
//==============================================
TsString TsXMLElement::GetTextData()
{
    if (m_element == nullptr)
        return "nullptr";
    return m_element->GetText();
}

//==============================================
//  TsXMLElement::GetName()
//  エレメントの名前を取得します
//==============================================
TsString TsXMLElement::GetName()
{
    return m_name;
}

//==============================================
//  子エレメントを取得します。
//  無い場合はnullが返ります。
//==============================================
TsXMLElement* TsXMLElement::GetFirstChild()
{
    if (m_element == nullptr)
        return nullptr;
    return m_firstChild;
}

TsBool TsXMLElement::SetElement(tinyxml2::XMLElement* elm)
{
    m_element = elm;
    m_name = elm->Name();
    m_data = elm->GetText() != nullptr ? elm->GetText() : "";

    auto attribute = m_element->FirstAttribute();

    while (attribute != nullptr)
    {
        TsXMLAttribute* a = TsNew(TsXMLAttribute);
        a->m_attribute = (tinyxml2::XMLAttribute*)attribute;
        m_attributeList.push_back(a);
        attribute =	attribute->Next();
    }

    return TS_TRUE;
}

//==============================================
//  子エレメントを名前で取得
//==============================================
TsXMLElement* TsXMLElement::FindChild(const TsString& name)
{
    return TsXmlUT::ElementSearchFromChildren(m_firstChild,name);
}

TsXMLElement* TsXMLElement::FindChildForAttribute(const TsString& attribute, TsString value)
{

    std::function<TsXMLElement*(TsXMLElement*,TsString)>  Search = [&](TsXMLElement* elm, TsString name)
    {
        auto at = elm->GetAttribute(attribute);

        if (at != nullptr && at->GetStringValue() == name )
        {
            return elm;
        }

        auto child = elm->GetFirstChild();
        auto subling = elm->Next();

        if (child)
        {
            auto res = Search(child, name);
            if (res)return res;
        }

        if (subling)
        {
            auto res = Search(subling, name);
            if (res)return res;
        }

        return (TsXMLElement*)nullptr;
    };

    return Search(this, value);
}

//==============================================
//  attributeを取得
//==============================================
TsXMLAttribute* TsXMLElement::GetAttribute(const TsString& name)
{
    for each (auto var in m_attributeList)
    {
        if (name == var->m_attribute->Name())
            return var;
    }
    return nullptr;
}


//==============================================
//  同階層の次のエレメントを取得します。
//  無い場合はnullが返ります。
//==============================================
TsXMLElement* TsXMLElement::Next()
{
    return m_subling;
}

//==============================================
//  TsXMLElement::TsXML()
//  constructor
//==============================================
TsXML::TsXML()
:m_rootNode(nullptr)
{

};

TsXML::TsXML(const TsString& filename)
{
    TsXML::TsXML();
    LoadXML(filename);
}

TsBool TsXML::LoadXML(const TsString& filename)
{
    if (m_docment.LoadFile(filename.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
    {
        TsDebugLogLoadError(filename.c_str());
        return TS_FALSE;
    }

    m_rootNode = TsNew(TsXMLElement);
    m_rootNode->SetElement( m_docment.RootElement() );

    std::function<void(TsXMLElement*)> CreateTree = [&](TsXMLElement* tselm)
    {
        if (tselm == nullptr || tselm->m_element == nullptr)
            return;

        tinyxml2::XMLElement* child = tselm->m_element->FirstChildElement();
        if (child != nullptr)
        {
            tselm->m_firstChild = TsNew(TsXMLElement);
            tselm->m_firstChild->SetElement( child );
            tselm->m_firstChild->m_parent = tselm;
            CreateTree(tselm->m_firstChild);
        }

        tinyxml2::XMLElement* sub = tselm->m_element->NextSiblingElement();
        if (sub != nullptr)
        {
            tselm->m_subling = TsNew(TsXMLElement);
            tselm->m_subling->SetElement(sub);
            CreateTree(tselm->m_subling);
        }
    };
    CreateTree(m_rootNode);

    TsDebugLogLoadSuccess(filename.c_str());
    return TS_TRUE;
}

TsXML::~TsXML()
{
    TsSafeDelete(m_rootNode);
}

TsXMLElement* TsXML::GetRootNode()
{
    return m_rootNode;
}

TsXMLElement* TsXML::FindFirst(const TsString& name)
{
    return TsXmlUT::ElementSearchFromChildren(m_rootNode, name);
}

TsBool TsXML::SaveXml( const TsString& filename )
{
    if (m_docment.SaveFile(filename.c_str(), TS_TRUE) != tinyxml2::XMLError::XML_SUCCESS)
    {
        TsDebugLogError("%s.xmlファイルの書き出しに失敗\n", filename.c_str());
        return TS_FALSE;
    }
    return TS_TRUE;
}