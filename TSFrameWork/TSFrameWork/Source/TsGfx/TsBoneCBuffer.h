#pragma once

class TsBoneCBuffer : public TsCBuffer
{
public:
    TsBoneCBuffer();
    virtual TsBool UpdateCBuffer( TsDeviceContext * pContext ) override;
    virtual TsBool ApplyCBuffer( TsDeviceContext * pContext ) override;
    TsBool SetSkeleton( TsSkeleton* pTransform );
    TsBool SetWorldTransform( TsTransForm* pTransform );

    TsBool CreateBoneCBuffer( TsDevice* pDev );

protected:
    struct BoneCBuffer
    {
        TsMatrix bone[512];
    };

    TsSkeleton*         m_pSkeleton;
    TsTransForm*        m_worldTransform;
    BoneCBuffer         m_boneCBuffer;
    
};