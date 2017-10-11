#include "..\TsGfx.h"

TsInstanceCBuffer::TsInstanceCBuffer()
{

};

TsBool TsInstanceCBuffer::UpdateCBuffer(TsDeviceContext * pContext)
{
	return ChangedCBuffer( pContext , &m_CBufferData );
}


TsBool TsInstanceCBuffer::CreateInstanceCBuffer(TsDevice* pDev)
{
	return CreateCBuffer( pDev , &m_CBufferData , InstanceCB );
}
TsInt TsInstanceCBuffer::SetMatrix(TsInt index, const TsMatrix& mtx)
{
	m_CBufferData.instance[index] = mtx.Transposed();
    return TS_TRUE;
}
