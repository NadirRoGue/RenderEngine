#pragma once

#include "Renderer.h"
#include "renderers/DeferredRenderer.h"

namespace Engine
{
	class SideBySideRenderer : public Renderer
	{
	private:
		DeferredRenderer * leftRenderer;
		DeferredRenderer * rightRenderer;

		bool initialized;
		bool clearScreen;
		int c;
	public:
		SideBySideRenderer();
		~SideBySideRenderer();

		void setLeftForwardPassBuffer(DeferredRenderObject * buffer);
		void setRightForwardPassBuffer(DeferredRenderObject * buffer);
		void addLeftPostProcess(PostProcessChainNode * node);
		void addRightPostProcess(PostProcessChainNode * node);

		void setClearScreen(bool val);
		void initialize();
		void doRender();
		void onResize(unsigned int w, unsigned int h);
	};
}