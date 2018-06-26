/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Camera.h"
#include "DeferredRenderObject.h"

namespace Engine
{
	// Holds screen dimensions
	class ScreenManager
	{
	public:
		static unsigned int SCREEN_WIDTH;
		static unsigned int SCREEN_HEIGHT;

		static unsigned int REAL_SCREEN_WIDTH;
		static unsigned int REAL_SCREEN_HEIGHT;
	};

	// ==================================================================
	// Parent class of all renderer types presents on the engine
	class Renderer
	{
	protected:
		Camera * activeCam;
	public:
		Renderer();
		~Renderer();

		void renderFromCamera(Camera * cam);

		virtual void initialize();
		virtual void doRender() = 0;

		virtual void onResize(unsigned int w, unsigned int h) = 0;
	};	

	// ==================================================================
	// Manages the avaliable renderers at runtime
	class RenderManager
	{
	private:
		static RenderManager * INSTANCE;
	public:
		static RenderManager & getInstance();
	private:
		Renderer * activeRender;
	private:
		RenderManager();
	public:
		~RenderManager();

		void setRenderer(Renderer * render);
		Renderer * getRenderer();
		void doRender();
		void doResize(unsigned int w, unsigned int h);
	};
}