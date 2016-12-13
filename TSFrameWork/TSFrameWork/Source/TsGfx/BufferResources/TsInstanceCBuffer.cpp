#include "..\TsGfx.h"

TsInstanceCBuffer::TsInstanceCBuffer()
{

};

TsBool TsInstanceCBuffer::UpdateCBuffer(TsDeviceContext * pContext)
{
   return ChangedCBuffer(pContext, &m_instanceCBuffer);
}


TsBool TsInstanceCBuffer::CreateInstanceCBuffer(TsDevice* pDev)
{
    return CreateCBuffer(pDev, &m_instanceCBuffer, InstanceCB);
}
TsInt TsInstanceCBuffer::SetMatrix(TsInt index, const TsMatrix& mtx)
{
    m_instanceCBuffer.instance[index] = mtx.Transposed();
    return TS_TRUE;
}
