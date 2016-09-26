//!*******************************************************
//!	TsDrawQueue.h
//!
//!	RenderQueue. the class is geomtry list.
//!
//!	© 2016 Yuki Tsuneyama
#pragma once

class TsDrawQueue
{
public:
	TsDrawQueue();
	virtual ~TsDrawQueue();

	TsBool Add( TsDrawObject* );
	TsDrawObject* Find( TsString name );
	TsBool Remove( TsDrawObject* );

	TsBool Begin( TsDeviceContext * pDc ,
				  TsBool zEnable ,
				  TsBool zWriteEnable ,
				  D3D11_CULL_MODE cullMopde );

	TsBool End();

	TsBool Render( TsDeviceContext* );

private:
	TsDrawQueue& operator = ( const TsDrawQueue& ) = delete;
	TsDrawQueue( const TsDrawQueue& ) = delete;
	TsVector<TsDrawObject*> m_drawObjList;
};