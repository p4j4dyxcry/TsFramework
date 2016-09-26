#pragma once

class TsScreenSpaceMaterial :public TsMaterial
{
public:
	TsScreenSpaceMaterial();
	virtual ~TsScreenSpaceMaterial();
	virtual TsBool ApplyMaterial( TsDeviceContext* context );
	virtual TsBool UpdateMaterial( TsDeviceContext* context );
	virtual TsBool CreateMaterial( TsDevice* pDevice );
};