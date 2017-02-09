//!*******************************************************
//! TsSkinGeometryObject.h
//!
//! Mesh base.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsSkinGeometryObject : virtual public TsGeometryObject
{
public:
    TsSkinGeometryObject();
    virtual ~TsSkinGeometryObject();
    TsBool CreateGeometryObject( TsDevice* pDev ,
                                 TsVertexElement * pMesh ,
                                 TsMaterial* pMaterial = nullptr )override;

    virtual TsBool UpdateTransform( TsDeviceContext* context )override;
    virtual TsBool ApplyTransForm( TsDeviceContext * context )override;
    TsSkeleton* GetSkeleton( ){ return m_pSkeleton; }
    TsBool SetSkeleton(TsSkeleton* pSkeleton);
private:
    TsBoneCBuffer*     m_boneCBuffer;
    TsSkeleton*        m_pSkeleton;
    TsCBuffer*         m_pdumyTranform;
};