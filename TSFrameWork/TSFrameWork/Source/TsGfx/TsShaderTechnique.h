#pragma once

class TsShaderTechnique
{
public:
	TsShaderTechnique();
	virtual ~TsShaderTechnique();
	virtual TsBool Initialize( TsDevice* , TsShaderEffect* );
	virtual TsBool UpdateCBuffer( TsDeviceContext * pDC );
	virtual TsBool ApplyCBuffer( TsDeviceContext * pDC );
protected:
	TsVector<TsCBuffer*> m_pCBuffers;
private:
};