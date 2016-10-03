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

#include "TsFbxManager.h"
#include "TsFbxNode.h"
#include "TsFbxScene.h"