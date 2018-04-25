#pragma once

#include "Renderer.h"
#include "Program.h"
#include "Object.h"
#include "DeferredNodeCallbacks.h"

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

		PostProcessChainNode * preProcess;
		std::list<PostProcessChainNode *> postProcessChain;
		PostProcessChainNode * finalLink;

		glm::mat4 dummyMatrix;

		bool initialized;

	public:
		DeferredRenderer();
		~DeferredRenderer();

		void setForwardPassBuffers(DeferredRenderObject * buffers);
		void setPreProcess(PostProcessChainNode * node);
		void addPostProcess(PostProcessChainNode * object);
		void setFinalPostProcess(PostProcessChainNode * object);
		void initialize();
		void doRender();
		void onResize(unsigned int w, unsigned int h);
	};
}