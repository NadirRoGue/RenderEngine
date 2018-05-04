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

	class DeferredRenderObject
	{
	private:
		static GLenum COLOR_ATTACHMENTS[8];
	public:
		static const std::string G_BUFFER_COLOR;
		static const std::string G_BUFFER_POS;
		static const std::string G_BUFFER_NORMAL;
		static const std::string G_BUFFER_SPECULAR;
		static const std::string G_BUFFER_DEPTH;
		static const std::string G_BUFFER_EMISSIVE;
	private:
		unsigned int fbo;
		
		int usedColorBuffers;
		unsigned int colorBuffersSize;
		BufferInfo * colorBuffers;
		BufferInfo depthBuffer;
		std::map<std::string, TextureInstance *> gBufferMap;

		bool renderDepth;

	public:
		DeferredRenderObject(unsigned int numColorBuffers, bool renderDepth);
		~DeferredRenderObject();

		unsigned int getFrameBufferId();

		TextureInstance * addColorBuffer(unsigned int index, GLenum gpuTextureFormat, GLenum inputTextureFormat, GLenum pixelFormat, unsigned int w, unsigned int h, std::string name = "", int filterMethod = GL_NEAREST);
		TextureInstance * addDepthBuffer24(unsigned int w, unsigned int h);
		TextureInstance * addDepthBuffer32(unsigned int w, unsigned int h);
		TextureInstance * getBufferByName(std::string name);

		void initialize();
		void resizeFBO(unsigned int w, unsigned int h);

		void populateDeferredObject(PostProcessObject * obj);
	};
}