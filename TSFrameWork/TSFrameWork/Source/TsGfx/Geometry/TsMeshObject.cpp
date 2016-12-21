#include "../TsGfx.h"

TsBool TsMeshObject::AddGeometry(TsGeometryObject* pGeometry)
{
    m_pGeometryList.push_back(pGeometry);
    return TS_TRUE;
}

TsInt TsMeshObject::GetGeometryCount()const
{
    return m_pGeometryList.size();
}

TsGeometryObject* TsMeshObject::GetGeometry(TsInt index)const
{
    return m_pGeometryList[index];
}

TsVector<TsGeometryObject*> TsMeshObject::GetGeometrys()const
{
    return m_pGeometryList;
}


TsBool  TsMeshObject::SetSkeleton(TsSkeleton* pSkeleton)
{
    m_pSkeleton = pSkeleton;
    return TS_TRUE;
}

TsVector<TsDefaultMaterial*> TsMeshObject::GetMaterials()const
{
    TsVector<TsDefaultMaterial*> mList;
    mList.reserve(m_pGeometryList.size());
    for (auto p : m_pGeometryList)
        mList.push_back((TsDefaultMaterial*)p->GetMaterial());

    return mList;
}

TsTransForm* TsMeshObject::GetRootTransform()const
{
    return m_pGeometryList[0]->GetTransform()->GetRootTransform();
}