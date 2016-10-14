#include "../../../TsAfx.h"

TsLambertMatrial::TsLambertMatrial() 
{

};

TsLambertMatrial::~TsLambertMatrial()
{

};

TsBool TsLambertMatrial::SetColor( TsFloat4 color )
{
    m_material.diffuse = color;
    m_isUpdate = TS_TRUE;
    return TS_TRUE;
}

TsBool TsLambertMatrial::CreateMaterial( TsDevice* pDev )
{
    ID3D11Buffer* buffer = pDev->CreateBuffer( &m_material , sizeof( m_material ) , 0 , D3D11_BIND_CONSTANT_BUFFER );
    this->SetD3DBufferAndSize( buffer , sizeof( m_material ) );

    SetRegisterIndex( 1 );  //register Index = [c1]
    BindShaderType( TS_SHADER_TYPE::PIXEL_SHADER );
    return TS_TRUE;
}

TsBool TsLambertMatrial::ApplyMaterial( TsDeviceContext* pContext)
{
    pContext->SetCBuffer( this );
    pContext->SetTexture( 0 , m_pTexture , TS_SHADER_TYPE::PIXEL_SHADER );
    pContext->SetSamplerState( TsResourceManager::Find<TsSamplerState>( "Default" ) , 0 );
    return TS_TRUE;
}

TsBool TsLambertMatrial::UpdateMaterial( TsDeviceContext* pContext )
{
    if( m_isUpdate )
        pContext->ChangeCBuffer( this , &m_material , GetBufferSize() );
    m_isUpdate = TS_FALSE;
    return TS_TRUE;
}