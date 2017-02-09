#include"../../TsCore/TsCore.h"
#include"../TsGfx.h"

TsGeometryObject::TsGeometryObject():
m_pMaterial(TsResourceManager::Find<TsMaterial>(TS_HASH_nullptr)),
m_pVertexElement(nullptr),
m_pTransform(nullptr),
m_pTransformCBuffer(nullptr){}
    
TsGeometryObject::~TsGeometryObject()
{
    TsSafeDelete( m_pTransformCBuffer );
}

TsBool TsGeometryObject::CreateGeometryObject( TsDevice* pDev ,
                                               TsVertexElement * pMesh ,
                                               TsMaterial* pMaterial )
{
    SetVertexElement( pMesh );
    SetMaterial( pMaterial );

    m_pTransformCBuffer = TsNew( TsTransformCBuffer );
    m_pTransformCBuffer->CreateTransformCBuffer( pDev );
    
    return TS_TRUE;
}

TsBool TsGeometryObject::UpdateTransform( TsDeviceContext* context )
{
    if( m_pTransformCBuffer )
        m_pTransformCBuffer->UpdateCBuffer( context );
    else
        return TS_FALSE;
    return TS_TRUE;
}
TsBool TsGeometryObject::UpdateMaterial( TsDeviceContext* context )
{
    if( m_pMaterial )
        m_pMaterial->UpdateMaterial( context );
    else
        return TS_FALSE;
    return TS_TRUE;
}
TsBool TsGeometryObject::UpdateIndexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_TRUE;
}
TsBool TsGeometryObject::UpdateVertexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_TRUE;
}

TsBool TsGeometryObject::ApplyTransForm( TsDeviceContext * context )
{
    if( m_pTransformCBuffer )
        m_pTransformCBuffer->ApplyCBuffer( context );
    else
        return TS_FALSE;
    return TS_TRUE;
}
TsBool TsGeometryObject::ApplyMaterial( TsDeviceContext* context )
{
    if( m_pMaterial )
        m_pMaterial->ApplyMaterial( context );
    else
        return TS_FALSE;
    return TS_TRUE;
}
TsBool TsGeometryObject::ApplyIndexBuffer( TsDeviceContext* context )
{
    if( m_pVertexElement )
    {
        TsIndexBuffer * pIb = m_pVertexElement->GetIndexBuffer();
        if( pIb )
            context->SetIndexBuffer( pIb );
        else
            return TS_FALSE;
    }
    else
        return TS_FALSE;
    return TS_TRUE;
}
TsBool TsGeometryObject::ApplyVertexBuffer( TsDeviceContext* context )
{
    if( m_pVertexElement )
        context->SetVertexBuffer( m_pVertexElement->GetVertexBuffer() );
    else
        return TS_FALSE;
    return TS_TRUE;
}
TsBool TsGeometryObject::Draw( TsDeviceContext* context )
{
    if( m_pVertexElement )
    {
        context->SetTopology( D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        if (m_pVertexElement->GetIndexNum() == 0)
            context->Draw(m_pVertexElement->GetVertexNum());
        else
            context->DrawIndex(m_pVertexElement->GetIndexNum());
    }
    else
        return TS_FALSE;
    return TS_TRUE;
}

TsBool TsGeometryObject::SetMaterial( TsMaterial* pMaterial )
{
    if (pMaterial == nullptr)
        m_pMaterial = TsResourceManager::Find<TsMaterial>(TS_HASH_nullptr);
    else 
        m_pMaterial = pMaterial;
    return TS_TRUE;
}
TsBool TsGeometryObject::SetVertexElement( TsVertexElement* pMesh)
{
    m_pVertexElement = pMesh;
    return TS_TRUE;
}

TsBool TsGeometryObject::SetTransform( TsTransForm* pTransform )
{
    m_pTransform = pTransform;
    m_pTransformCBuffer->SetTransform( pTransform );
    return TS_TRUE;
}

TsTransForm* TsGeometryObject::GetTransform()const
{
    return m_pTransform;
}

TsMatrix TsGeometryObject::GetWorldMatrix()const
{
    return m_pTransform->ToWorldMatrix();
}
TsMatrix TsGeometryObject::GetLocalMatrix()const
{
    return m_pTransform->ToLocalMatrix();
}
