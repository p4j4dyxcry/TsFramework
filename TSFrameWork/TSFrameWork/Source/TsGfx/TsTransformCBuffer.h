#pragma once

class TsTransformCBuffer : public TsCBuffer
{
public:
	TsTransformCBuffer();
	virtual TsBool UpdateCBuffer( TsDeviceContext * pContext ) override;
	virtual TsBool ApplyCBuffer( TsDeviceContext * pContext) override;
	TsBool CreateTransformCBuffer( TsDevice* pDev );
	TsBool SetTransform( TsTransForm* pTransform );
protected:
	struct MatrixCBuffer
	{
		TsMatrix m_MtxWorld;
		TsMatrix m_MtxInvWorld;
	};

	TsTransForm* m_pTransform;
	TsMatrix	 m_matrixCash;
	MatrixCBuffer m_matrixCBuffer;
};