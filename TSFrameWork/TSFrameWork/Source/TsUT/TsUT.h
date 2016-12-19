﻿//!*******************************************************
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
#include <typeinfo>

#include "TsType.h"
#include "TsArrayType.h"
#include "TsLogger.h"

#include "TsFileUT.h"
#include "TsHelper.h"
#include "TsNameObject.h"
#include "../TsMath/TsMath.h"

#include "Intarface/ITsStaticNameObjectList.h"
#include "Intarface/ITsHasTransform.h"
#include "TsBinaryFile.h"

#include "Loader\Base\Ts3DModelFileLoader.h"
#include "Loader\virtual\TsMeshLoader.h"
#include "Loader\XML\TsXML.h"

#include "Loader\dae\TsColladaLoader.h"
#include "Loader\Fbx\TsFbxLoader.h"
#include "Loader\hdr\TsHDRTextureAnalizer.h"
#include "Loader\Mqo\TsMqoLoader.h"
#include "Loader\x\TsXLoader.h"
#include "Loader\stl\TsSTLLoader.h"
#include "Loader\obj\TsObjLoader.h"

#pragma comment(lib,"winmm.lib")