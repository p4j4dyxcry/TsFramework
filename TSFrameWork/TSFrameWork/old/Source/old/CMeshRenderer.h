//!*******************************************************
//!	CMeshRenderer.h
//!
//!	���b�V���̕`��@�\
//!
//!	���쌠	2016 Yuki Tsuneyama
//!	�쐬����	16.01.15

#pragma once

#include "Renderer.h"

class CMeshBase;

class CMeshRenderer : public CRenderer
{
public :
	void Render()override;
};