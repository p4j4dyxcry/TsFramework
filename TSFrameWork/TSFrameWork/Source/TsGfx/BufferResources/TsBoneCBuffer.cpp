#include "../TsGfx.h"
TsBoneCBuffer::TsBoneCBuffer() :m_pSkeleton(nullptr)
{
}
TsBool TsBoneCBuffer::UpdateCBuffer( TsDeviceContext * pContext )
{
    if( m_pSkeleton == nullptr )
        return TS_FALSE;

   //m_pSkeleton->UpdateSkeleton();

    TsVector<TsBone*>& boneList = m_pSkeleton->GetBoneList();
    for each( auto pBone in boneList )
        m_boneCBuffer.bone[pBone->GetBoneID()] = pBone->GetBoneMatrix().Transposed();

    pContext->ChangeCBuffer( this , &m_boneCBuffer , sizeof( m_boneCBuffer ) );
    return TS_TRUE;
}
TsBool TsBoneCBuffer::ApplyCBuffer( TsDeviceContext * pContext )
{
    if (m_pSkeleton == nullptr)
        return TS_FALSE;

    pContext->SetCBuffer( this );
    return TS_TRUE;
}
TsBool TsBoneCBuffer::SetSkeleton( TsSkeleton* pSkeleton )
{
    m_pSkeleton = pSkeleton;
//    m_worldTransform = m_pRootBone;
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