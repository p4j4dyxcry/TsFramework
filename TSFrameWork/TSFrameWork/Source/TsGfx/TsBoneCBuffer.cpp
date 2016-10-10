#include "TsGfx.h"
TsBoneCBuffer::TsBoneCBuffer() :m_pRootBone(nullptr)
{
}
TsBool TsBoneCBuffer::UpdateCBuffer( TsDeviceContext * pContext )
{
    UpdateBones( m_pRootBone );
    pContext->ChangeCBuffer( this , &m_boneCBuffer , sizeof( m_boneCBuffer ) );
    return TS_TRUE;
}
TsBool TsBoneCBuffer::ApplyCBuffer( TsDeviceContext * pContext )
{
    pContext->SetCBuffer( this );
    return TS_TRUE;
}
TsBool TsBoneCBuffer::SetRootBoneTransform( TsBoneTransForm* pTransform )
{
    m_pRootBone = pTransform;
    m_worldTransform = m_pRootBone;
    return TS_TRUE;
}

TsBool TsBoneCBuffer::SetWorldTransform( TsTransForm * pTransform)
{
  return TS_TRUE;
}

TsBool TsBoneCBuffer::CreateBoneCBuffer( TsDevice * pDev )
{
    size_t sz = sizeof( m_boneCBuffer );

    ID3D11Buffer* buffer = pDev->CreateBuffer( &m_boneCBuffer ,
                                               sz ,
                                               D3D11_CPU_ACCESS_WRITE ,
                                               D3D11_BIND_CONSTANT_BUFFER );


    SetD3DBufferAndSize( buffer , sz );
    SetRegisterIndex( 1 );
    BindShaderType( TS_SHADER_TYPE::VERTEX_SHADER );

    return TS_TRUE;
}

void TsBoneCBuffer::UpdateBones( TsBoneTransForm* pBone)
{
    TsMatrix world = m_worldTransform->ToWorldMatrix();
    TsMatrix bone = pBone->ToBoneMatrix( world );
    m_boneCBuffer.bone[pBone->GetID()] = bone;
    m_boneCBuffer.bone[pBone->GetID()].Transpose();

    if( pBone->GetFirstChild() &&
        pBone->GetFirstChild()->GetClassTypeName() == "TsBoneTransForm" )
    {
        UpdateBones( ((TsBoneTransForm*)pBone->GetFirstChild()) );
    }

    if( pBone->GetSubling() &&
        pBone->GetSubling()->GetClassTypeName() == "TsBoneTransForm" )
    {
        UpdateBones( ( ( TsBoneTransForm* )pBone->GetSubling() ) );
    }
}