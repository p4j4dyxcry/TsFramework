#pragma once

class TsShadowMapRenderer 
{
public:
    TsLight* m_pLight;
protected:
    TsShaderEffect* m_pShader;
    static TsRenderTarget* m_pShadewmapTexture;
};