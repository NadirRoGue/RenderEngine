/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include "renderers/DeferredRenderer.h"

namespace Engine
{
	/**
	 * Class in charge of performing a side-by-side rendering throught a deferred
	 * renderer for stereoscopic rendering.
	 * LEGACY CLASS, not used in the procedural world
	 */
	class SideBySideRenderer : public Renderer
	{
	private:
		// Left side renderer
		DeferredRenderer * leftRenderer;
		// Right side renderer
		DeferredRenderer * rightRenderer;

		bool initialized;
		bool clearScreen;
		int c;
	public:
		SideBySideRenderer();
		~SideBySideRenderer();

		void addLeftPostProcess(PostProcessChainNode * node);
		void addRightPostProcess(PostProcessChainNode * node);

		void setClearScreen(bool val);
		void initialize();
		void doRender();
		void onResize(unsigned int w, unsigned int h);
	};
}