#pragma once

class TsMaterial : public TsCBuffer
{
public:
	TsMaterial();
	virtual ~TsMaterial();
	virtual TsBool ApplyMaterial( TsDeviceContext* context ) ;
	virtual TsBool UpdateMaterial( TsDeviceContext* context );
	virtual TsBool CreateMaterial( TsDevice* pDevice );

	virtual TsBool LoadTextureFromFile( TsDevice* pDevice );
	virtual TsTexture* GetTexture()const;

public:
	TsString m_textureName;

protected:
	TsBool m_isUpdate;
	TsTexture2D*   m_pTexture;
};