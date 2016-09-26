//!*******************************************************
//!	CMeshRenderer.h
//!
//!	メッシュの描画機構
//!
//!	著作権	2016 Yuki Tsuneyama
//!	作成日時	16.01.15

#pragma once

#include "Renderer.h"

class CMeshBase;

class CMeshRenderer : public CRenderer
{
public :
	void Render()override;
};