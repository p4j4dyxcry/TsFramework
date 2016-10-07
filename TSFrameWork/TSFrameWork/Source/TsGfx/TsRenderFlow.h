//!*******************************************************
//! TsRenderFlow.h
//!
//! The class is renderer manage.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsRenderFlow : public TsNameObject
{
public:
    virtual TsBool Render( TsDeviceContext* pDC,  TsDrawQueue* pQue );
    TsRenderPass*  GetRenderPass(TsString);
    TsRenderPass*  GetRenderPass(TsInt index);
    TsInt          GetFlowSize();
    TsBool         SetRenderPass(TsRenderPass *, TsInt pass = -1);

    TsBool         LoadFlowFromXML( TsDevice* pDev , const TsString& file );
protected:
    TsVector<TsRenderPass*> m_renderPass;
};