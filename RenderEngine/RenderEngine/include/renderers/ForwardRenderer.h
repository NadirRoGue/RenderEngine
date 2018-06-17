/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include "Renderer.h"
#include "Scene.h"

namespace Engine
{
	/**
	 * Class in charge of performing a forward rendering
	 * LEGACY CLASS, not used in the procedural world rendering
	 */
	class ForwardRenderer : public Renderer
	{
	public:
		ForwardRenderer();
		~ForwardRenderer();
		void doRender();
		void onResize(unsigned int w, unsigned int h);
		void renderProgram(Camera * camera, ProgramRenderables * pr);
	};
}