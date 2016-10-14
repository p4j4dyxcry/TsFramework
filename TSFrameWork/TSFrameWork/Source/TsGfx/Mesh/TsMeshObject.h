#pragma once

class TsMeshObject : public TsNameObject
{
public:
    TsBool AddGeometry(TsGeometryObject* pGeometry);
    TsGeometryObject* GetGeometry( TsInt index );
    TsInt GetGeometryCount();
protected:
    TsVector<TsGeometryObject*> m_pGeometryList;
};