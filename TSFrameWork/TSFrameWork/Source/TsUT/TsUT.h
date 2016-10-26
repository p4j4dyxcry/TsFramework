//!*******************************************************
//!	TsUT.h
//!
//!	TsFramework Utility Header
//!
//!	©	2016 Yuki Tsuneyama

#pragma once
#include <stdint.h>

#include "TsEASTL.h"
#include <string>

#include <unordered_map>
#include <vector>
#include <stack>
#include <list>

#include "TsType.h"
#include "TsArrayType.h"
#include "TsAlloc.h"
#include "TsLogger.h"

#include "TsFileUT.h"
#include "TsHelper.h"
#include "TsNameObject.h"
#include "../TsMath/TsMath.h"

#include "Intarface/ITsStaticNameObjectList.h"
#include "Intarface/ITsHasTransform.h"
#include "TsBinaryFile.h"

#include "TsMeshLoader.h"
#include "TsXLoader.h"
#include "TsMqoLoader.h"
#include "TsColladaLoader.h"

#include "ModelLoader\Fbx\TsFbxLoader.h"

#include "TsHDRTextureAnalizer.h"

#pragma comment(lib,"winmm.lib")