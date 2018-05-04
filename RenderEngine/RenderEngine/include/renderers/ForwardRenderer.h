#pragma once

#include "Renderer.h"
#include "Scene.h"

namespace Engine
{
	class ForwardRenderer : public Renderer
	{
	public:
		ForwardRenderer();
		~ForwardRenderer();
		void doRender();
		void onResize(unsigned int w, unsigned int h);
		void renderProgram(Camera * camera, ProgramRenderables * pr);

		DeferredRenderObject * getGBuffer();
	};
}