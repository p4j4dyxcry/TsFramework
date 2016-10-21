//!*******************************************************
//! TsClearPass.h
//!
//! RTV & DSV をクリアするパス
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsClearPass : public TsRenderPass
{
public:
    TsClearPass();
    virtual ~TsClearPass();

    virtual TsBool Begin( TsDeviceContext* )override;
    virtual TsBool Render( TsDrawQueue* pQue , TsDeviceContext* )override;

    virtual TsBool LoadShaderFromXMLElement( TsDevice* pDev , TsXMLElement * pElement )override;
protected:
    TsFloat4 m_clearColor;
};