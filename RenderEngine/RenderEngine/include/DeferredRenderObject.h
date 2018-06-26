/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <map>
#include <gl/glew.h>

#include "Object.h"
#include "instances/TextureInstance.h"

namespace Engine
{
	struct BufferInfo
	{
		GLenum bufferType;
		TextureInstance * texture;
	} typedef BufferInfo;

	// FBO Wrapper class. Eases the access, use, and manipulation of FBOs
	class DeferredRenderObject
	{
	private:
		// Color attachments
		static GLenum COLOR_ATTACHMENTS[8];
	public:
		// Default texture names
		static const std::string G_BUFFER_COLOR;
		static const std::string G_BUFFER_POS;
		static const std::string G_BUFFER_NORMAL;
		static const std::string G_BUFFER_SPECULAR;
		static const std::string G_BUFFER_DEPTH;
		static const std::string G_BUFFER_EMISSIVE;
	private:
		// fbo id
		unsigned int fbo;
		
		// Dynamic texture target configuration
		int usedColorBuffers;
		unsigned int colorBuffersSize;
		BufferInfo * colorBuffers;
		BufferInfo depthBuffer;
		std::map<std::string, TextureInstance *> gBufferMap;

		// Wether to render depth or not
		bool renderDepth;

		// Width and height mod to apply on every resize
		float widthMod, heightMod;
	public:
		DeferredRenderObject(unsigned int numColorBuffers, bool renderDepth);
		~DeferredRenderObject();

		unsigned int getFrameBufferId();

		TextureInstance * addColorBuffer(unsigned int index, GLenum gpuTextureFormat, GLenum inputTextureFormat, GLenum pixelFormat, unsigned int w, unsigned int h, std::string name = "", int filterMethod = GL_NEAREST);
		TextureInstance * addDepthBuffer24(unsigned int w, unsigned int h);
		TextureInstance * addDepthBuffer32(unsigned int w, unsigned int h);
		TextureInstance * getBufferByName(std::string name);

		void initialize();
		void setResizeMod(float widthMod = 1.0f, float heightMod = 1.0f);
		void resizeFBO(unsigned int w, unsigned int h);

		// Attaches the textures of this FBO as inputs for the object representing the
		// post-process executed after the current one (this method is linked to the architecture
		// of the deferred renderer of the engine)
		void populateDeferredObject(PostProcessObject * obj);
	};
}