#pragma once
#include<fbxsdk.h>
#ifdef _DEBUG 
#pragma comment(lib,"debug/libfbxsdk-md.lib")
#else
#pragma comment(lib,"release/libfbxsdk-md.lib")
#endif

#define TsFbxNodeAttributeType FbxNodeAttribute::EType

template<typename FbxType>
void FbxSafeRelease( FbxType* pFbx )
{
	if( pFbx )
		pFbx->Destroy();
	pFbx = nullptr;
}
static TsMatrix FbxMatrixToTsMatrix( const FbxMatrix& mtx )
{
	TsMatrix m;

	for( int i = 0; i < 16; ++i )
		m.m[0][i] = static_cast<float>( mtx[0][i] );

	return m;
}


#include "TsFbxManager.h"
#include "TsFbxNode.h"
#include "TsFbxMaterial.h"
#include "TsFbxScene.h"

