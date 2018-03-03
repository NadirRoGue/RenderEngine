/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <list>
#include <gl/glew.h>

#include "Texture.h"
#include "Object.h"

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

		unsigned int fbo;
		
		int usedColorBuffers;
		unsigned int colorBuffersSize;
		BufferInfo * colorBuffers;

		BufferInfo depthBuffer;

		bool renderDepth;

	public:
		DeferredRenderObject(unsigned int numColorBuffers, bool renderDepth);
		~DeferredRenderObject();

		unsigned int getFrameBufferId();

		void addColorBuffer(unsigned int index, GLenum gpuTextureFormat, GLenum inputTextureFormat, GLenum pixelFormat, unsigned int w, unsigned int h, float filterMethod = GL_NEAREST);
		void addDepthBuffer24(unsigned int w, unsigned int h);
		void addDepthBuffer32(unsigned int w, unsigned int h);

		void initialize();
		void resizeFBO(unsigned int w, unsigned int h);

		void populateDeferredObject(Object * obj);
	};
}