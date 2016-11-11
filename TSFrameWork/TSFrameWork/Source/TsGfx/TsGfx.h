//!*******************************************************
//! TsGfx.h
//!
//! Use Directx11 gfx Api lappers.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

#include "../TsUT/TsUT.h"
#include "../TsMath/TsMath.h"
#include "Core\TsDx11.h"

#include "Shader/TsShaderUT.h"

#include "ShaderResources\TsResourceDesc.h"
#include "Shader\TsShaderReflection.h"
#include "Shader\TsShader.h"

#include "Core\TsDevice.h"
#include "Core\TsDeviceContext.h"

#include "Shader\TsShaderEffect.h"

#include "Extlibs\TsDirectXTex.h"

#include "ShaderResources\TsBlendStae.h"
#include "ShaderResources\TsRasterizerState.h"
#include "ShaderResources\TsDepthStencilState.h"
#include "ShaderResources\TsShaderResource.h"
#include "BufferResources\TsBuffer.h"
#include "BufferResources\TsVertexBuffer.h"
#include "BufferResources\TsIndexBuffer.h"
#include "BufferResources\TsCBuffer.h"
#include "BufferResources\TsTransformCBuffer.h"
#include "BufferResources\TsBoneCBuffer.h"

#include "ShaderResources\TsSamplerState.h"
#include "ShaderResources\TsTexture.h"
#include "ShaderResources\TsRenderTarget.h"
#include "ShaderResources\TsDepthStencilView.h"
#include "Camera\TsViewport.h"

#include "Light\TsLight.h"
#include "BufferResources\TsLightSetCBuffer.h"
#include "ShadowMap\TsShadowMapRenderer.h"

#include "Camera\TsCamera.h"
#include "BufferResources\TsVertex.h"
#include "Material\TsMaterial.h"
#include "Material\TsLambertMaterial.h"
#include "Material\TsScreeSpaceMaterial.h"
#include "Material\TsDefaultMaterial.h"

#include "Geometry\TsVertexElement.h"
#include "Geometry\TsMeshFactory.h"
#include "Geometry\TsDrawObject.h"
#include "Geometry\TsGeometryObject.h"
#include "Geometry\TsMeshObject.h"
#include "Geometry\TsSkyBox.h"
#include "Geometry\TsSkinGeometryObject.h"
#include "Geometry\TsScreenObject.h"

#include "RenderSystem\TsDrawQueue.h"
#include "RenderSystem\TsRenderPass.h"
#include "RenderSystem\TsClearPass.h"
#include "RenderSystem\TsRenderFlow.h"
#include "RenderSystem\TsRenderSystem.h"

#include "Animation\TsTransformBakeAnimation.h"


