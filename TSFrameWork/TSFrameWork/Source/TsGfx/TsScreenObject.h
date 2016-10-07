
#pragma once

class TsScreenObject : public TsDrawObject
{
public:
    TsScreenObject();
    virtual ~TsScreenObject();

    virtual TsBool Create( TsDevice* pDev ) override;
    virtual TsBool Draw( TsDeviceContext* context ) override;
    virtual TsBool ApplyVertexBuffer( TsDeviceContext* context )override;
protected:
    TsVertexBuffer*         m_vertexBuffer;
    TSVertexScreenSpace     m_vertex[4];
};