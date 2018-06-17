/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Renderer.h"
#include "Object.h"
#include "DeferredNodeCallbacks.h"

#include "postprocessprograms/DeferredShadingProgram.h"

namespace Engine
{
	/*
	 * Utility struct which holds all required data to execute a post process pass 
	 */
	struct PostProcessChainNode
	{
		// Shader to use
		Program *postProcessProgram;
		// Mesh to render (typcipally a screen quad)
		PostProcessObject * obj;
		// Render target
		DeferredRenderObject * renderBuffer;
		// Optional initialization & execution code callback
		DeferredCallback * callBack;
	} typedef PostProcessChainNode;

	// ==================================================================

	/**
	 * Class in charge to perform a deferred rendering with multiple passes throught the GPU
	 * and giving support to chain image space post processes
	 */
	class DeferredRenderer : public Renderer
	{
	private:
		// Scene forward render RT (g - buffers)
		DeferredRenderObject * forwardPassBuffer;
		// Post process RT
		// 1 buffer for color
		// 1 buffer for emission info
		// 1 buffer for general-purpose info
		DeferredRenderObject * deferredPassBuffer;

		// Deferred shading program instance
		DeferredShadingProgram * deferredShading;

		// Chain end which outputs the result to the default frame buffer
		PostProcessProgram * screenOutput;

		// Screen space quad to perform deferred shading
		PostProcessObject * deferredDrawSurface;
		// Screen space quad to draw the final result
		PostProcessObject * chainEnd;

		// List of image space post processes
		std::list<PostProcessChainNode *> postProcessChain;

		// Renderer initialization flag (prevents multiple initialization)
		bool initialized;

		// Function pointer that points to the current render code
		void (DeferredRenderer::*renderFunc)();

		// G-Buffer textures
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

		// Adds the given post-process at the end of the post-process chain
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
		// Render function executed once at the beggining of the execution
		// used to execute baking passes on the GPU
		void initializeLoop();
		// Actual render loop function
		void renderLoop();
		void runPostProcesses();
	};
}