#include "TsUT.h"
#include "TsColladaLoader.h"



TsColladaLoader::TsColladaLoader()
{

}
TsColladaLoader::~TsColladaLoader()
{

}
TsBool TsColladaLoader::LoadFromFile(TsString filename)
{
	m_xmlData.LoadXML(filename);

	TsXMLElement* geometryLib = m_xmlData.FindFirst("library_geometries");

	TsXMLElement* geometry = geometryLib->FindChild("geometry");
	TsXMLElement* mesh = geometry->FindChild("mesh");
	TsXMLElement* source = mesh->FindChild("source");

//	TsString str = geometry->GetAttribute("name")->GetStringValue() +"-Position";
	TsString type = source->GetAttribute("id")->GetStringValue();
	type.erase(geometry->GetAttribute("name")->GetStringValue().length());
	//switch ( type )

	auto a = m_xmlData.FindFirst("library_geometries")->FindChildForAttribute("id", "miku-Vertex");

	a;

	return 1;
}
TsBool TsColladaLoader::LoadFromMemory(void * memory, size_t sz)
{
	return TS_FALSE;
}
TsInt  TsColladaLoader::GetMeshNum()
{
	return 0;
}
TsInt  TsColladaLoader::GetVertexSize(TsInt index)
{
	return 0;
}
void*  TsColladaLoader::GetVertexBuffer(TsInt index)
{
	return nullptr;
}
size_t TsColladaLoader::GetVertexStride()
{
	return 0;
}

TsInt TsColladaLoader::GetMaterialNum()const
{
	return 0;
}
TsString TsColladaLoader::GetTexturePass(TsInt index)const
{
	return 0;
}