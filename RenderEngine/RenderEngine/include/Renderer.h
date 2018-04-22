/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Scene.h"
#include "DeferredRenderObject.h"
#include "DeferredNodeCallbacks.h"

#include <list>

namespace Engine
{
	class ScreenManager
	{
	public:
		static unsigned int SCREEN_WIDTH;
		static unsigned int SCREEN_HEIGHT;

		static unsigned int REAL_SCREEN_WIDTH;
		static unsigned int REAL_SCREEN_HEIGHT;
	};

	// ==================================================================

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

	class ForwardRenderer: public Renderer
	{
	public:
		ForwardRenderer();
		~ForwardRenderer();
		void doRender();
		void onResize(unsigned int w, unsigned int h);
		void renderProgram(Camera * camera, ProgramRenderables * pr);
	};

	// ==================================================================

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

		/**
		 * initialize() must be called after the post processing chain has been constructed
		 * and before start the rendering loop
		 */
		void initialize();
		void doRender();

		void onResize(unsigned int w, unsigned int h);
	};

	// ==================================================================

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

	// ==================================================================

	class RenderManager
	{
	private:
		static RenderManager * INSTANCE;

	public:
		static RenderManager & getInstance();

	private:
		Renderer * activeRender;

		ForwardRenderer * forwardRenderer;
		DeferredRenderer * deferredRenderer;
		SideBySideRenderer * sideBySideRenderer;
		
		bool forwardRendering;
	private:
		RenderManager();
	public:
		~RenderManager();

		void forwardRender();
		void deferredRender();
		void sideBySideRender();

		void setForwardRenderer(ForwardRenderer * renderer);
		void setDeferredRenderer(DeferredRenderer * renderer);
		void setSideBySideRenderer(SideBySideRenderer * renderer);

		DeferredRenderer * getCurrentDeferredRenderer();
		ForwardRenderer * getCurrentForwardRenderer();
		SideBySideRenderer * getCurrentSideBySideRenderer();

		void doRender();
		void doResize(unsigned int w, unsigned int h);

		bool isForwardRendering();
	};
}