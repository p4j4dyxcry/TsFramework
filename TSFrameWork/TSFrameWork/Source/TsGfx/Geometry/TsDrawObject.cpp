#include "../TsGfx.h"

//----------------------------------------------------------
//! ジオメトリを作成します。
//  継承して利用
//----------------------------------------------------------
TsBool TsDrawObject::Create( TsDevice* pDev)
{
    ( void )pDev;
    return TS_FALSE;
}

//----------------------------------------------------------
//! オブジェクトのDrawCallです作成します。
//  継承して利用
//----------------------------------------------------------
TsBool TsDrawObject::Draw( TsDeviceContext* context)
{
    ( void )context;
    return TS_FALSE;
}

//----------------------------------------------------------
//! オブジェクトのTransformを更新します。
//  継承して利用
//----------------------------------------------------------
TsBool TsDrawObject::UpdateTransform( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//----------------------------------------------------------
//! オブジェクトのMaterialを更新します。
//  継承して利用
//----------------------------------------------------------
TsBool TsDrawObject::UpdateMaterial( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//----------------------------------------------------------
//! オブジェクトのインデックスバッファを更新します。
//  継承して利用
//----------------------------------------------------------
TsBool TsDrawObject::UpdateIndexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//----------------------------------------------------------
//! オブジェクトの頂点バッファを更新します。
//  継承して利用
//----------------------------------------------------------
TsBool TsDrawObject::UpdateVertexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//----------------------------------------------------------
//! オブジェクトの頂点バッファを確定します。
//  継承して利用
//----------------------------------------------------------
TsBool TsDrawObject::ApplyVertexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//----------------------------------------------------------
//! オブジェクトのTransofromを確定します。
//  継承して利用
//----------------------------------------------------------
TsBool TsDrawObject::ApplyTransForm( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//----------------------------------------------------------
//! オブジェクトのIndexBufferを確定します。
//  継承して利用
//----------------------------------------------------------
TsBool TsDrawObject::ApplyIndexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//----------------------------------------------------------
//! オブジェクトのMaterialを確定します。
//  継承して利用
//----------------------------------------------------------
TsBool TsDrawObject::ApplyMaterial( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//----------------------------------------------------------
//! オブジェクトのWorldMatrixを確定します。
//  継承して利用
//----------------------------------------------------------
TsMatrix TsDrawObject::GetWorldMatrix()const
{
    return TsMatrix::identity;
}


//----------------------------------------------------------
//! オブジェクトのLocalMatrixを確定します。
//  継承して利用
//----------------------------------------------------------
TsMatrix TsDrawObject::GetLocalMatrix()const
{
    return TsMatrix::identity;
}