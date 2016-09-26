//!*******************************************************
//!	TsShaderUT.h
//!
//!	Shader Utility.
//!
//!	© 2016 Yuki Tsuneyama
#pragma once

enum TS_SHADER_TYPE
{
	UNKNOWN_SHADER = 0x00 ,

	VERTEX_SHADER = 0x01 ,
	PIXEL_SHADER = 0x02 ,
	GEOMETRY_SHADER = 0x04 ,
	HULL_SHADER = 0x08 ,
	DOMAIN_SHADER = 0x10 ,
	COMPUTE_SHADER = 0x20 ,

	// vertex & pixel
	VP_SHADER = 0x03 ,
	ALL_SHADER = 0x3f ,
};

enum TS_SHADER_INDEX
{
	VS_IDX = 0x00,
	PS_IDX = 0x01,
	GS_IDX = 0x02,
	HS_IDX = 0x03,
	DS_IDX = 0x04,
	CS_IDX = 0x05,

	MAX_IDX = 0x06,
};

namespace TSShaderUT
{
	inline TsString TsComvertShaderType( TS_SHADER_TYPE shaderType )
	{
		switch( shaderType )
		{
			case TS_SHADER_TYPE::VERTEX_SHADER:return "VetexShader";
			case TS_SHADER_TYPE::PIXEL_SHADER:return "PixlShader";
			case TS_SHADER_TYPE::GEOMETRY_SHADER:return "GiometoryShader";
			case TS_SHADER_TYPE::HULL_SHADER:return "HullShader";
			case TS_SHADER_TYPE::DOMAIN_SHADER:return "DomainShader";
			default:
			case TS_SHADER_TYPE::UNKNOWN_SHADER: return "UnknownShader";
		}
	}

	inline TsBool IsVS( TS_SHADER_TYPE type )
	{
		return (type & VERTEX_SHADER) > 0;
	}

	inline TsBool IsPS( TS_SHADER_TYPE type )
	{
		return (type & PIXEL_SHADER) > 0;
	}

	inline TsBool IsGS( TS_SHADER_TYPE type )
	{
		return (type & GEOMETRY_SHADER) > 0;
	}

	inline TsBool IsHS( TS_SHADER_TYPE type )
	{
		return (type & HULL_SHADER) > 0;
	}

	inline TsBool IsDS( TS_SHADER_TYPE type )
	{
		return (type & DOMAIN_SHADER) > 0;
	}

	inline TsBool IsCS( TS_SHADER_TYPE type )
	{
		return (type & COMPUTE_SHADER) > 0;
	}
}