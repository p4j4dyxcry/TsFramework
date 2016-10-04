#include"TsGfx.h"

TsGeometryObject::TsGeometryObject():
m_material(nullptr),
m_mesh(nullptr),
m_transform(nullptr),
m_transformCBuffer(nullptr){}
	
TsGeometryObject::~TsGeometryObject()
{
	TsSafeDelete( m_transformCBuffer );
}

TsBool TsGeometryObject::CreateGeometryObject( TsDevice* pDev ,
											   TsMesh * pMesh ,
											   TsMaterial* pMaterial )
{
	SetMesh( pMesh );
	SetMaterial( pMaterial );

	m_transformCBuffer = TsNew( TsTransformCBuffer );
	m_transformCBuffer->CreateTransformCBuffer( pDev );
	
	return TS_TRUE;
}

TsBool TsGeometryObject::UpdateTransform( TsDeviceContext* context )
{
	if( m_transformCBuffer )
		m_transformCBuffer->UpdateCBuffer( context );
	else
		return TS_FALSE;
	return TS_TRUE;
}
TsBool TsGeometryObject::UpdateMaterial( TsDeviceContext* context )
{
	if( m_material )
		m_material->UpdateMaterial( context );
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
	if( m_transformCBuffer )
		m_transformCBuffer->ApplyCBuffer( context );
	else
		return TS_FALSE;
	return TS_TRUE;
}
TsBool TsGeometryObject::ApplyMaterial( TsDeviceContext* context )
{
	if( m_material )
		m_material->ApplyMaterial( context );
	else
		return TS_FALSE;
	return TS_TRUE;
}
TsBool TsGeometryObject::ApplyIndexBuffer( TsDeviceContext* context )
{
	if( m_mesh )
	{
		TsIndexBuffer * pIb = m_mesh->GetIndexBuffer();
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
	if( m_mesh )
		context->SetVertexBuffer( m_mesh->GetVertexBuffer() );
	else
		return TS_FALSE;
	return TS_TRUE;
}
TsBool TsGeometryObject::Draw( TsDeviceContext* context )
{
	if( m_mesh )
	{
		//todo debug
		context->SetTopology( D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->Draw( m_mesh->GetVertexNum() );
	}
	else
		return TS_FALSE;
	return TS_TRUE;
}

TsBool TsGeometryObject::SetMaterial( TsMaterial* pMaterial )
{
	m_material = pMaterial;
	return TS_TRUE;
}
TsBool TsGeometryObject::SetMesh( TsMesh* pMesh)
{
	m_mesh = pMesh;
	return TS_TRUE;
}

TsBool TsGeometryObject::SetTransform( TsTransForm* pTransform )
{
	m_transform = pTransform;
	m_transformCBuffer->SetTransform( pTransform );
	return TS_TRUE;
}