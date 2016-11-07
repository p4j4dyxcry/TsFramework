#include "..\TsGfx.h"
#include "../ShadowMap/TsLisPSM.h"
TsLightSetCBuffer::TsLightSetCBuffer()
:m_isUpdate(TS_TRUE)

{
    m_pLightRefList.reserve(TsLightSetCBuffer::LIGHT_CB_MAX);
}
TsLightSetCBuffer::~TsLightSetCBuffer()
{
    m_pLightRefList.clear();
}

TsBool TsLightSetCBuffer::CreateLightSetCBuffer( TsDevice* pDev )
{
    size_t sz = sizeof( m_lightSetCBuffer );

    ID3D11Buffer* buffer = pDev->CreateBuffer( &m_lightSetCBuffer ,
                                               sz ,
                                               D3D11_CPU_ACCESS_WRITE ,
                                               D3D11_BIND_CONSTANT_BUFFER );


    SetD3DBufferAndSize( buffer , sz );
    SetRegisterIndex( TS_CBUFFER_REGISTER::LightSet );
    BindShaderType( ( TS_SHADER_TYPE )( TS_SHADER_TYPE::VERTEX_SHADER |
        TS_SHADER_TYPE::PIXEL_SHADER ) );

    return TS_TRUE;
}

TsBool TsLightSetCBuffer::ApplyCBuffer(TsDeviceContext* pDevContext)
{
    if (pDevContext)
        pDevContext->SetCBuffer(this);
    else
        return TS_FALSE;
    return TS_TRUE;
}
TsBool TsLightSetCBuffer::UpdateCBuffer(TsDeviceContext* pDevContext)
{
    TsBool isUpdate = m_isUpdate;
    for each(auto p in m_pLightRefList)
        isUpdate |= p->IsUpdate();

    // for debug
    //if (isUpdate)
    {
        m_lightSetCBuffer.lightNum = m_pLightRefList.size();
        for (TsUint i = 0; i > m_pLightRefList.size(); ++i)
        {
            TsLight* pLight = m_pLightRefList[i];
            m_lightSetCBuffer.lightData[i].type = pLight->GetLightType();
            m_lightSetCBuffer.lightData[i].pos = pLight->GetWorldPosition();
            m_lightSetCBuffer.lightData[i].dir = TsVector3::zero;
            m_lightSetCBuffer.lightData[i].color = pLight->GetColor();
            if (pLight->GetLightType() == TsLight::LIGHT_TYPE::LIGHT_DIRECTIONAL)
            {
                TsVector3 v = pLight->GetWorldMatrix().TransformCoord(TsVector3(0, 0, 1));
                m_lightSetCBuffer.lightData[i].dir = v.Normalized();
            }

            m_lightSetCBuffer.lightData[i].intensity = pLight->GetIntensity();
            
            m_lightSetCBuffer.lightData[i].angle = 0;
            if (pLight->GetLightType() == TsLight::LIGHT_TYPE::LIGHT_SPOT)
            {
                // todo Set Spot Light Angle
            }

            m_lightSetCBuffer.lightData[i].range = 0;
            if (pLight->GetLightType() == TsLight::LIGHT_TYPE::LIGHT_SPOT || 
                pLight->GetLightType() == TsLight::LIGHT_TYPE::LIGHT_POINT)
            {
                // todo Set Spot Light And PointLight Range
            }

            // Compute Shadow Map Matrix
            if (pLight->GetLightType() == TsLight::LIGHT_TYPE::LIGHT_DIRECTIONAL)
            {
                TsComputeLisPSM lispsm;
                TsCamera* pCamera = pDevContext->GetMainCamera();
                lispsm.SetEyePos(pCamera->GetWorldPosition());
                lispsm.SetEyeProjection(pCamera->GetLinearProjMatrix());
                lispsm.SetNearClip(pCamera->GetNear());
                lispsm.SetViewDir( pCamera->GetZAxis() );
                lispsm.SetLightDir(m_lightSetCBuffer.lightData[i].dir);
                lispsm.UpdateShadowMatrix();
                m_lightSetCBuffer.lightData[i].worldToShadowMatrix = lispsm.GetLVPMatrix();
            }
            else
                m_lightSetCBuffer.lightData[i].worldToShadowMatrix = TsMatrix::identity;
        }
    }
    m_isUpdate = TS_FALSE;
    return TS_TRUE;
}

TsBool TsLightSetCBuffer::AddLight(TsLight * pLight)
{
    m_pLightRefList.push_back(pLight);

    return TS_TRUE;
}
TsBool TsLightSetCBuffer::RemoveLight(TsLight *pLight)
{
    auto it = tstl::find(m_pLightRefList.begin(), m_pLightRefList.end(), pLight);

   m_pLightRefList.erase(it);

   return TS_TRUE;

}
TsBool TsLightSetCBuffer::RemoveLightByName(const TsString& name)
{
    TS_HASH hash = TSUT::StringToHash(name);

    for (auto it = m_pLightRefList.begin(); it != m_pLightRefList.end(); ++it)
    {
        if ((*it) && (*it)->GetHashCode() == hash)
        {
            m_pLightRefList.erase(it);
            return TS_TRUE;
        }
    }

    return TS_FALSE;
}