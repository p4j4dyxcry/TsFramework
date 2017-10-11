#pragma once

class TsImGui
{
public:
	void Initialize( TsDevice * pDevice );
	void Render( TsDeviceContext * pDeviceContext );
protected:

	struct VERTEX_CONSTANT_BUFFER
	{
		float        mvp[4][4];
	};

	void InitializeShader( TsDevice * pDevice );

	TsShaderEffect * m_pShaderEffect = nullptr;
	TsBlendState   * m_pBlendState = nullptr;
};