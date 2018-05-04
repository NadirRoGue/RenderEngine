/**
 * @author Nadir Román Guerrero
 * @email nadir.ro.gue@gmail.com
 */
#include "DeferredRenderObject.h"
#include "textures/Texture2D.h"

#include "datatables/DeferredObjectsTable.h"

#include <iostream>

const std::string Engine::DeferredRenderObject::G_BUFFER_COLOR = "g_buffer_color";
const std::string Engine::DeferredRenderObject::G_BUFFER_DEPTH = "g_buffer_depth";
const std::string Engine::DeferredRenderObject::G_BUFFER_NORMAL = "g_buffer_normal";
const std::string Engine::DeferredRenderObject::G_BUFFER_POS = "g_buffer_pos";
const std::string Engine::DeferredRenderObject::G_BUFFER_SPECULAR = "g_buffer_depth";
const std::string Engine::DeferredRenderObject::G_BUFFER_EMISSIVE = "g_buffer_emissive";

GLenum Engine::DeferredRenderObject::COLOR_ATTACHMENTS[8] =
{
	GL_COLOR_ATTACHMENT0,
	GL_COLOR_ATTACHMENT1,
	GL_COLOR_ATTACHMENT2,
	GL_COLOR_ATTACHMENT3,
	GL_COLOR_ATTACHMENT4,
	GL_COLOR_ATTACHMENT5,
	GL_COLOR_ATTACHMENT6,
	GL_COLOR_ATTACHMENT7
};

Engine::DeferredRenderObject::DeferredRenderObject(unsigned int numBuffers, bool renderDepth)
	:colorBuffersSize(numBuffers),renderDepth(renderDepth)
{
	colorBuffers = 0;
	if (numBuffers > 0)
	{
		colorBuffers = new Engine::BufferInfo[numBuffers];
	}
	usedColorBuffers = 0;

	depthBuffer.texture = 0;

	Engine::DeferredObjectsTable::getInstance().registerDeferredObject(this);
}

Engine::DeferredRenderObject::~DeferredRenderObject()
{
	if (colorBuffers != 0)
	{
		for (unsigned int i = 0; i < colorBuffersSize; i++)
		{
			BufferInfo bi = colorBuffers[i];
			if (bi.texture != NULL)
			{
				if (bi.texture->getTexture() != NULL)
				{
					delete bi.texture->getTexture();
				}
				delete bi.texture;
			}
		}

		delete[] colorBuffers;
	}

	if (depthBuffer.texture != NULL)
	{
		if (depthBuffer.texture->getTexture() != NULL)
		{
			delete depthBuffer.texture->getTexture();
		}
		delete depthBuffer.texture;
	}
}

unsigned int Engine::DeferredRenderObject::getFrameBufferId()
{
	return fbo;
}

Engine::TextureInstance * Engine::DeferredRenderObject::addColorBuffer(unsigned int index, GLenum gpuTextureFormat, GLenum inputTextureFormat, GLenum pixelFormat, unsigned int w, unsigned int h, std::string name, int filterMethod)
{
	if (index < 0 || index > colorBuffersSize || usedColorBuffers >= 8)
		exit(-1);

	GLenum colorAttachment = COLOR_ATTACHMENTS[usedColorBuffers];
	usedColorBuffers++;

	Engine::Texture2D * texture = new Engine::Texture2D(name, 0, w, h);
	texture->setGenerateMipMaps(false);
	texture->setMemoryLayoutFormat(gpuTextureFormat);
	texture->setImageFormatType(inputTextureFormat);
	texture->setPixelFormatType(pixelFormat);

	Engine::TextureInstance * ti = new Engine::TextureInstance(texture);
	ti->setMagnificationFilterType(filterMethod);
	ti->setMinificationFilterType(filterMethod);
	ti->setAnisotropicFilterEnabled(false);
	ti->setSComponentWrapType(GL_REPEAT);
	ti->setTComponentWrapType(GL_REPEAT);

	colorBuffers[index].bufferType = colorAttachment;
	colorBuffers[index].texture = ti;

	gBufferMap[name] = ti;

	return ti;
}

Engine::TextureInstance * Engine::DeferredRenderObject::addDepthBuffer24(unsigned int w, unsigned int h)
{
	if (depthBuffer.texture != 0)
	{
		depthBuffer.texture->~TextureInstance();
		delete depthBuffer.texture;
	}

	Engine::Texture2D * texture = new Engine::Texture2D(Engine::DeferredRenderObject::G_BUFFER_DEPTH, 0, w, h);
	texture->setGenerateMipMaps(false);
	texture->setMemoryLayoutFormat(GL_DEPTH_COMPONENT24);
	texture->setImageFormatType(GL_DEPTH_COMPONENT);
	texture->setPixelFormatType(GL_FLOAT);
	
	Engine::TextureInstance * textureInstance = new Engine::TextureInstance(texture);
	textureInstance->setAnisotropicFilterEnabled(false);
	textureInstance->setMagnificationFilterType(GL_NEAREST);
	textureInstance->setMinificationFilterType(GL_NEAREST);
	textureInstance->setSComponentWrapType(GL_CLAMP_TO_EDGE);
	textureInstance->setTComponentWrapType(GL_CLAMP_TO_EDGE);
	
	depthBuffer.bufferType = GL_DEPTH_ATTACHMENT;
	depthBuffer.texture = textureInstance;

	gBufferMap[Engine::DeferredRenderObject::G_BUFFER_DEPTH] = textureInstance;

	return textureInstance;
}

Engine::TextureInstance * Engine::DeferredRenderObject::addDepthBuffer32(unsigned int w, unsigned int h)
{
	if (depthBuffer.texture != 0)
	{
		delete depthBuffer.texture;
	}

	Engine::Texture2D * texture = new Engine::Texture2D(Engine::DeferredRenderObject::G_BUFFER_DEPTH, 0, w, h);
	texture->setGenerateMipMaps(false);
	texture->setMemoryLayoutFormat(GL_DEPTH_COMPONENT32);
	texture->setImageFormatType(GL_DEPTH_COMPONENT);
	texture->setPixelFormatType(GL_FLOAT);

	Engine::TextureInstance * textureInstance = new Engine::TextureInstance(texture);
	textureInstance->setAnisotropicFilterEnabled(false);
	textureInstance->setMagnificationFilterType(GL_NEAREST);
	textureInstance->setMinificationFilterType(GL_NEAREST);
	textureInstance->setSComponentWrapType(GL_CLAMP_TO_EDGE);
	textureInstance->setTComponentWrapType(GL_CLAMP_TO_EDGE);

	depthBuffer.bufferType = GL_DEPTH_ATTACHMENT;
	depthBuffer.texture = textureInstance;

	gBufferMap[Engine::DeferredRenderObject::G_BUFFER_DEPTH] = textureInstance;
	
	return textureInstance;
}

Engine::TextureInstance * Engine::DeferredRenderObject::getBufferByName(std::string name)
{
	std::map<std::string, Engine::TextureInstance*>::iterator it = gBufferMap.find(name);
	if (it != gBufferMap.end())
	{
		return it->second;
	}
	return NULL;
}

void Engine::DeferredRenderObject::initialize()
{
	glGenFramebuffers(1, &fbo);
	for (unsigned int i = 0; i < colorBuffersSize; i++)
	{
		colorBuffers[i].texture->generateTexture();
	}

	depthBuffer.texture->generateTexture();
}

void Engine::DeferredRenderObject::resizeFBO(unsigned int w, unsigned int h)
{
	for (unsigned int i = 0; i < colorBuffersSize; i++)
	{
		colorBuffers[i].texture->resize(w, h);
	}

	depthBuffer.texture->resize(w, h);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLenum * buffers = new GLenum[colorBuffersSize];
	for (unsigned int i = 0; i < colorBuffersSize; i++)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, colorBuffers[i].bufferType, GL_TEXTURE_2D, colorBuffers[i].texture->getTexture()->getTextureId(), 0);
		buffers[i] = colorBuffers[i].bufferType;
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer.texture->getTexture()->getTextureId(), 0); 
	
	if (colorBuffersSize > 0)
	{
		glDrawBuffers(colorBuffersSize, buffers);
	}
	else
	{
		glDrawBuffer(GL_NONE);
	}

	if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
		exit(-1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Engine::DeferredRenderObject::populateDeferredObject(Engine::PostProcessObject * object)
{
	for (unsigned int i = 0; i < colorBuffersSize; i++)
	{
		object->addTexture("color_" + std::to_string(i), colorBuffers[i].texture);
	}

	if (renderDepth)
	{
		object->addTexture("depth", depthBuffer.texture);
	}
}