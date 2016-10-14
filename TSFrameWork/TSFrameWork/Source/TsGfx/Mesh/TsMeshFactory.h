#pragma once

class TsGeometryObject;

class TsMeshFactory 
{
public:
    TsMeshFactory();
    virtual ~TsMeshFactory();

    TsBool LoadModelFromFile(TsDevice* pDev, TsString filename);

    TsMaterial* GetMaterialArray();

    TsGeometryObject* GetGeometryObject( TsInt index);

    TsInt GetPrimtiveNum(){ return m_pObjects.size(); };
protected:
    TsVector<TsGeometryObject*> m_pObjects;
};