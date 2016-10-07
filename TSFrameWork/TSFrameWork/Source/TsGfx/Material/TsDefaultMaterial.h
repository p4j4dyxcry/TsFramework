#pragma once

class TsDefaultMaterial
{
public:

    TsDefaultMaterial();

    TsBool ApplyMaterial();
    TsBool UpdateMaterial();
protected:
    struct DefaultMaterial
    {
        TsFloat4 diffuse;       //rgba 
        TsFloat3 specular;
        TsFloat3 ambient;
        TsF32	 specluarPower;
    };
    TsCBuffer *                 m_pCBuffer;
    static TsShaderEffect *     m_pShaderEffect;


};