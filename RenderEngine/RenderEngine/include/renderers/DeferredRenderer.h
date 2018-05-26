#pragma once

#include "Renderer.h"
#include "Program.h"
#include "Object.h"
#include "DeferredNodeCallbacks.h"

#include "postprocessprograms/DeferredShadingProgram.h"

#include "renderers/ForwardRenderer.h"

namespace Engine
{
	struct PostProcessChainNode
	{
		Program *postProcessProgram;
		PostProcessObject * obj;
		DeferredRenderObject * renderBuffer;
		DeferredCallback * callBack;
	} typedef PostProcessChainNode;

	// ==================================================================

	class DeferredRenderer : public Renderer
	{
	private:
		ForwardRenderer * forwardPass;

		DeferredRenderObject * forwardPassBuffer;
		DeferredRenderObject * deferredPassBuffer;

		DeferredShadingProgram * deferredShading;
		PostProcessProgram * screenOutput;

		PostProcessObject * deferredDrawSurface;
		PostProcessObject * chainEnd;

		std::list<PostProcessChainNode *> postProcessChain;

		bool initialized;

		void (DeferredRenderer::*renderFunc)();

		TextureInstance * gBufferPos;
		TextureInstance * gBufferNormal;
		TextureInstance * gBufferEmissive;
		TextureInstance * gBufferSpecular;
		TextureInstance * gBufferColor;
		TextureInstance * gBufferDepth;
		TextureInstance * gBufferInfo;
	public:
		DeferredRenderer();
		~DeferredRenderer();

		void addPostProcess(PostProcessChainNode * object);
		void initialize();
		void doRender();
		void onResize(unsigned int w, unsigned int h);
		
		const TextureInstance * getGBufferPos();
		const TextureInstance * getGBufferNormal();
		const TextureInstance * getGBufferEmissive();
		const TextureInstance * getGBufferSpecular();
		const TextureInstance * getGBufferColor();
		const TextureInstance * getGBufferDepth();
		const TextureInstance * getGBufferInfo();
	private:
		void initializeLoop();
		void renderLoop();
		void runPostProcesses();
	};
}