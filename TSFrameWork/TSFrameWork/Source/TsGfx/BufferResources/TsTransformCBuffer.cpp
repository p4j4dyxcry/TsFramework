#include "../TsGfx.h"

TsTransformCBuffer::TsTransformCBuffer()
    :m_pTransform( nullptr )
{}

TsBool TsTransformCBuffer::UpdateCBuffer( TsDeviceContext * pContext )
{
    if( m_pTransform )
    {
        TsMatrix mtxWorld;

        if (m_matrixConvertOrder == MatrixConvertOrder::MTX_CVT_SRT)
            mtxWorld = m_pTransform->ToWorldMatrix();   // scale * translate * rotate
        if (m_matrixConvertOrder == MatrixConvertOrder::MTX_CVT_TRS)
            mtxWorld = m_pTransform->ToLocalTRSMatrix();// translate * rotate * scale

        if (m_matrixCash != mtxWorld)
        {
			m_CBufferData.m_MtxWorld = mtxWorld.Transposed();

			ChangedCBuffer( pContext , &m_CBufferData );
            m_matrixCash = mtxWorld;
        }		
    }
    else
    {
        return TS_FALSE;
    }

    return TS_TRUE;

}

TsBool TsTransformCBuffer::CreateTransformCBuffer( TsDevice* pDev )
{
	CreateCBuffer( pDev , &m_CBufferData , TS_CBUFFER_REGISTER::InstanceCB , TS_SHADER_TYPE::VERTEX_SHADER );

    return TS_TRUE;
}

TsBool TsTransformCBuffer::SetTransform( TsTransForm* pTransform )
{
    m_pTransform = pTransform;
    return TS_TRUE;
}