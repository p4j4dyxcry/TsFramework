#include "..\TsGfx.h"

TsBool TsLightSetCBuffer::CreateLightSetCBuffer( TsDevice* pDev )
{
    size_t sz = sizeof( m_lightSetCBuffer );

    ID3D11Buffer* buffer = pDev->CreateBuffer( &m_lightSetCBuffer ,
                                               sz ,
                                               D3D11_CPU_ACCESS_WRITE ,
                                               D3D11_BIND_CONSTANT_BUFFER );


    SetD3DBufferAndSize( buffer , sz );
    SetRegisterIndex( TS_CBUFFER_REGISTER::LightSet );
    BindShaderType( ( TS_SHADER_TYPE )( TS_SHADER_TYPE::VERTEX_SHADER |
        TS_SHADER_TYPE::PIXEL_SHADER ) );

    return TS_TRUE;
}