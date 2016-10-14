#pragma once

class TsMeshObject : public TsDrawObject
{
public:
    TsBool AddGeometry(TsGeometryObject* pGeometry);

    virtual TsBool UpdateTransform(TsDeviceContext* context)override;
    virtual TsBool UpdateMaterial(TsDeviceContext* context)override;
    virtual TsBool UpdateIndexBuffer(TsDeviceContext* context)override;
    virtual TsBool UpdateVertexBuffer(TsDeviceContext* context)override;

    virtual TsBool ApplyTransForm(TsDeviceContext * context)override;
    virtual TsBool ApplyMaterial(TsDeviceContext* context)override;
    virtual TsBool ApplyIndexBuffer(TsDeviceContext* context)override;
    virtual TsBool ApplyVertexBuffer(TsDeviceContext* context)override;
    virtual TsBool Draw(TsDeviceContext*)override;
protected:
    TsVector<TsGeometryObject*> m_pGeometryList;
};