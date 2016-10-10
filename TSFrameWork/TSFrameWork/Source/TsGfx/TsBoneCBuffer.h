#pragma once

class TsBoneCBuffer : public TsCBuffer
{
public:
    TsBoneCBuffer();
    virtual TsBool UpdateCBuffer( TsDeviceContext * pContext ) override;
    virtual TsBool ApplyCBuffer( TsDeviceContext * pContext ) override;
    TsBool SetRootBoneTransform( TsBoneTransForm* pTransform );
    TsBool SetWorldTransform( TsTransForm* pTransform );

    TsBool CreateBoneCBuffer( TsDevice* pDev );

protected:
    void UpdateBones( TsBoneTransForm* );
    struct BoneCBuffer
    {
        TsMatrix bone[512];
    };

    TsBoneTransForm*    m_pRootBone;
    TsTransForm*        m_worldTransform;
    BoneCBuffer         m_boneCBuffer;
    
};