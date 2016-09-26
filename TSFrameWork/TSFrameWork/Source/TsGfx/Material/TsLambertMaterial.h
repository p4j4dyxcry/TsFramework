#pragma once

class TsLambertMatrial : public TsMaterial
{
public:
	TsLambertMatrial();
	virtual ~TsLambertMatrial();
	struct LambertMatrial
	{
		TsFloat4 diffuse;
		TsF32	 ambient;
		LambertMatrial()
		{
			diffuse = TsFloat4( 1 , 1 , 1 , 1 );
			ambient = 0.2f;
		}
	};

	TsBool SetColor( TsFloat4 color );
	virtual TsBool ApplyMaterial( TsDeviceContext* ) override;
	virtual TsBool CreateMaterial( TsDevice* ) override;
	virtual TsBool UpdateMaterial( TsDeviceContext* )override;
protected:
	LambertMatrial m_material;
};