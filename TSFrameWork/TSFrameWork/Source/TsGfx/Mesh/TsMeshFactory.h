#pragma once

class TsGeometryObject;

class TsMeshFactory 
{
public:
    TsMeshFactory();
    virtual ~TsMeshFactory();

    TsBool LoadFromFile( TsDevice* pDev, TsString filename );

    TsMaterial* GetMaterial( TsInt index );
    TsMesh* GetMesh( TsInt index );

    TsMesh* GetMeshArray();
    TsMaterial* GetMaterialArray();

    TsGeometryObject* CreateGeometryObject( TsInt index ,TsDevice* pDev);

    TsInt GetPrimtiveNum(){ return m_pMeshList.size(); };
protected:
    TsVector<TsMesh*>       m_pMeshList;
    TsVector<TsMaterial*>   m_pMaterial;
    TsVector<TsTransForm*>  m_pTransform;
};