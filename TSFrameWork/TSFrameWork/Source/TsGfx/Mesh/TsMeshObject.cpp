#include "../TsGfx.h"

TsBool TsMeshObject::AddGeometry(TsGeometryObject* pGeometry)
{
    m_pGeometryList.push_back(pGeometry);
    return TS_TRUE;
}

TsInt TsMeshObject::GetGeometryCount()
{
    return m_pGeometryList.size();
}

TsGeometryObject* TsMeshObject::GetGeometry(TsInt index)
{
    return m_pGeometryList[index];
}
