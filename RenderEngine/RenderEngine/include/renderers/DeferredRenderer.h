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

	public:
		DeferredRenderer();
		~DeferredRenderer();

		void addPostProcess(PostProcessChainNode * object);
		void initialize();
		void doRender();
		void onResize(unsigned int w, unsigned int h);
		
		DeferredRenderObject * getGBuffer();
	private:
		void runPostProcesses();
	};
}