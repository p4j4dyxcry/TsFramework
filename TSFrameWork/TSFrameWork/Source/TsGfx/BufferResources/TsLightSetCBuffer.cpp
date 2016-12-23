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
    CreateCBuffer(pDev, &m_lightSetCBuffer, TS_CBUFFER_REGISTER::LightSetCB, TS_SHADER_TYPE::VP_SHADER );

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
        for (TsUint i = 0; i < m_pLightRefList.size(); ++i)
        {
            TsLight* pLight = m_pLightRefList[i];
            m_lightSetCBuffer.lightData[i].type = (TsUint)pLight->GetLightType();
            m_lightSetCBuffer.lightData[i].pos = pLight->GetWorldPosition();
            m_lightSetCBuffer.lightData[i].dir = TsVector3::zero;
            m_lightSetCBuffer.lightData[i].color = pLight->GetColor().m_color;
            if (pLight->GetLightType() == TsLight::LIGHT_TYPE::LIGHT_DIRECTIONAL)
            {
                TsVector3 v = TsVector3::front;
                v *= pLight->GetWorldMatrix();
                v.Normalize();
                m_lightSetCBuffer.lightData[i].dir = v;
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
                lispsm.SetViewMatrix( pCamera->GetViewMatrix() );
                lispsm.SetEyePos(pCamera->GetWorldPosition());
                lispsm.SetProjection(pCamera->GetLinearProjMatrix());
                lispsm.SetLightDir(m_lightSetCBuffer.lightData[i].dir);
                lispsm.SetSceneBoundingBox( TsAABB3D( TsVector3( -250 , -10 , -250 ) , TsVector3( 250 , 300 , 250 ) ) );
                lispsm.UpdateShadowMatrix();
                m_lightSetCBuffer.lightData[i].worldToShadowMatrix = lispsm.GetLVPMatrix().Transposed();
            }
            else
                m_lightSetCBuffer.lightData[i].worldToShadowMatrix = TsMatrix::identity;
        }
        ChangedCBuffer(pDevContext, &m_lightSetCBuffer);
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