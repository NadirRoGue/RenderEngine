/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "DeferredRenderObject.h"
#include "Renderer.h"

namespace Engine
{
	namespace TestImplementation
	{
		DeferredRenderer * createDeferredRendererWithDoF();
		DeferredRenderer * createDeferredDepthRenderer();
		DeferredRenderer * createNormalRenderer();
		DeferredRenderer * createColorRenderer();
		DeferredRenderer * createSpecularRenderer();
		DeferredRenderer * createToonShaderRenderer();

		// =================================================================

		DeferredRenderObject * createForwardBuffer();
		DeferredRenderObject * createForwardBufferWithDepth();
		DeferredRenderObject * createDepthOnlyBuffer();

		PostProcessChainNode * createGaussianBlur(bool addConvolutionControl);
		PostProcessChainNode * createScreenAntiAliasing();
		PostProcessChainNode * create5x5GaussianBlur(float * convolutionMask, bool addConvolutionControl);

		PostProcessChainNode * createDepthOfField(bool addDofControl, bool addConvolutionControl);

		PostProcessChainNode * createDepthRenderNode();
		PostProcessChainNode * createNormalRenderNode();
		PostProcessChainNode * createColorRenderNode();
		PostProcessChainNode * createSpecularRenderNode();

		PostProcessChainNode * createFinalLink(bool addMB, bool addMBControl);

		PostProcessChainNode * createDeferredShadingNode();

		PostProcessChainNode * createToonShadingNode();
	}
}