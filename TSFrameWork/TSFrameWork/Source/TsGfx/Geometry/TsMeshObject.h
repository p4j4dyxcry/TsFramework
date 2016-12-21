//!*******************************************************
//! TsMeshObject.h
//!
//! 複数のgeometryから成るメッシュオブジェクト
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsMeshObject : public TsNameObject
{
public:
    //--------------------------------------------------
    //! public method
    //--------------------------------------------------
    TsBool AddGeometry(TsGeometryObject* pGeometry);
    TsGeometryObject*   GetGeometry( TsInt index )const;
    TsVector<TsGeometryObject*>  GetGeometrys()const;
    TsInt GetGeometryCount()const;
    TsSkeleton*                 GetSkeleton(){ return m_pSkeleton; }
    TsBool  SetSkeleton(TsSkeleton* pSkeleton);

    TsVector<TsDefaultMaterial*> GetMaterials()const;

    TsTransForm* GetRootTransform()const;


protected:
    //--------------------------------------------------
    //! propery
    //--------------------------------------------------
    TsVector<TsGeometryObject*> m_pGeometryList;
    TsTransForm*                m_pRootTransform;
    TsSkeleton*                 m_pSkeleton;
};