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
                                 TsMesh * pMesh ,
                                 TsMaterial* pMaterial )override;

    virtual TsBool UpdateTransform( TsDeviceContext* context )override;
    virtual TsBool ApplyTransForm( TsDeviceContext * context )override;

    TsBool SetSkeleton(TsSkeleton* pSkeleton);
private:
    TsBoneCBuffer*     m_boneCBuffer;
    TsSkeleton*        m_pSkeleton;
};