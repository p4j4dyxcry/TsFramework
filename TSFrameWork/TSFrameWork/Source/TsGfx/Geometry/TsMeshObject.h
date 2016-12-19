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
    TsBool AddMaterial(TsMaterial* pGeometry);
    TsBool AddTransform(TsTransForm* pTransform);
    TsBool AddGeometry(TsGeometryObject* pGeometry);
    TsGeometryObject* GetGeometry( TsInt index );
    TsInt GetGeometryCount();
    TsSkeleton*                 GetSkeleton(){ return m_pSkeleton; }
    TsBool  SetSkeleton(TsSkeleton* pSkeleton);

protected:
    //--------------------------------------------------
    //! propery
    //--------------------------------------------------
    TsVector<TsGeometryObject*> m_pGeometryList;
    TsVector<TsTransForm*>      m_pTransformsList;

    TsSkeleton*                 m_pSkeleton;
};