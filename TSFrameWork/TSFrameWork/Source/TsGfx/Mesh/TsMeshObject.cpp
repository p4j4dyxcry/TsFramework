#include "../TsGfx.h"

TsBool TsMeshObject::AddGeometry(TsGeometryObject* pGeometry)
{
    m_pGeometryList.push_back(pGeometry);
    return TS_TRUE;
}

TsBool TsMeshObject::UpdateTransform(TsDeviceContext* context)
{
    for each(auto p in m_pGeometryList)
        p->UpdateTransform(context);
    return TS_TRUE;
}
TsBool TsMeshObject::UpdateMaterial(TsDeviceContext* context)
{
    for each(auto p in m_pGeometryList)
        p->UpdateMaterial(context);
    return TS_TRUE;
}
TsBool TsMeshObject::UpdateIndexBuffer(TsDeviceContext* context)
{
    for each(auto p in m_pGeometryList)
        p->UpdateIndexBuffer(context);
    return TS_TRUE;
}
TsBool TsMeshObject::UpdateVertexBuffer(TsDeviceContext* context)
{
    for each(auto p in m_pGeometryList)
        p->UpdateVertexBuffer(context);
    return TS_TRUE;
}

TsBool TsMeshObject::ApplyTransForm(TsDeviceContext * context)
{
    for each(auto p in m_pGeometryList)
        p->ApplyTransForm(context);
    return TS_TRUE;
}
TsBool TsMeshObject::ApplyMaterial(TsDeviceContext* context)
{
    for each(auto p in m_pGeometryList)
        p->ApplyMaterial(context);
    return TS_TRUE;
}
TsBool TsMeshObject::ApplyIndexBuffer(TsDeviceContext* context)
{
    for each(auto p in m_pGeometryList)
        p->ApplyIndexBuffer(context);
    return TS_TRUE;
}
TsBool TsMeshObject::ApplyVertexBuffer(TsDeviceContext* context)
{
    for each(auto p in m_pGeometryList)
        p->ApplyVertexBuffer(context);
    return TS_TRUE;
}

TsBool TsMeshObject::Draw(TsDeviceContext* context)
{
    for each(auto p in m_pGeometryList)
        p->Draw(context);
    return TS_TRUE;
}