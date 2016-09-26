//!*******************************************************
//!	TsVertex.h
//!
//!	Vertex Shader Input Vertex type.
//!
//!	© 2016 Yuki Tsuneyama
#pragma once

#include "../TsMath/TsVector.h"

// default Vertex
struct TsVertexDefault
{
	TsVector3 pos;
	TsVector2 uv;	
	TsVector3 normal;
};

// SkinMesh Vertex
struct TsVertexSkin
{
	TsVector3 pos;
	TsVector2 uv;
	TsVector3 normal;
	TsVector4 weight;
	TsInt4	  boneIndex;
};

struct TSVertexScreenSpace
{
	// x & y = pos
	// z & w = uv;
	TsVector4 data;
};
