#pragma once
#include<fbxsdk.h>
#ifdef _DEBUG 
#pragma comment(lib,"debug/libfbxsdk-md.lib")
#else
#pragma comment(lib,"release/libfbxsdk-md.lib")
#endif

#include "TsFbxType.h"
#include "TsFbxContext.h"
#include "TsFbxNode.h"
#include "TsFbxBlendShape.h"
#include "TsFbxMesh.h"
#include "TsFbxMaterial.h"

#include "TsFbxScene.h"
#include "TsFbxAnimation.h"
