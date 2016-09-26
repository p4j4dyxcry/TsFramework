//!*******************************************************
//!	TsResourceManager.h
//!
//!	Resource Manager.
//!
//!	© 2016 Yuki Tsuneyama

#pragma once

class TsGfxResouceManager : protected TsSingleton<TsGfxResouceManager>
{
public:
	virtual ~TsGfxResouceManager();
	static TsTexture*		CreateTexture();

};