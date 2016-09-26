#include "TsGfx.h"

void TsViewport::Create( const TsInt width ,
						 const TsInt height ,
						 const TsInt _topLeftX /*= 0   */ ,
						 const TsInt _topLeftY /*= 0   */ ,
						 const TsF32 _minDpeth /*= 0.0f*/ ,
						 const TsF32 _maxDpeth /*= 1.0f*/ )
{
	m_d3dviewport.Width = ( TsF32 )width;
	m_d3dviewport.Height = ( TsF32 )height;
	m_d3dviewport.TopLeftX = ( TsF32 )_topLeftX;
	m_d3dviewport.TopLeftY = ( TsF32 )_topLeftY;
	m_d3dviewport.MinDepth = _minDpeth;
	m_d3dviewport.MaxDepth = _maxDpeth;
}

TsInt2 TsViewport::GetSize()
{
	return TsInt2( (TsInt)m_d3dviewport.Width , (TsInt)m_d3dviewport.Height );
}

const D3D11_VIEWPORT* TsViewport::GetD3DViewport()const
{
	return &m_d3dviewport;
}