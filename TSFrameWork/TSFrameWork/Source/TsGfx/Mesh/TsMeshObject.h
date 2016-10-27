#pragma once

class TsMeshObject : public TsNameObject
{
public:
    TsBool AddGeometry(TsGeometryObject* pGeometry);
    TsGeometryObject* GetGeometry( TsInt index );
    TsInt GetGeometryCount();
    TsSkeleton*                 GetSkeleton(){ return m_pSkeleton; }
    TsBool  SetSkeleton(TsSkeleton* pSkeleton)
    {
        m_pSkeleton = pSkeleton;
        return TS_TRUE;
    }
protected:
    TsVector<TsGeometryObject*> m_pGeometryList;
    TsSkeleton*                 m_pSkeleton;
};