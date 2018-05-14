#include "textures/Texture3D.h"

Engine::Texture3D::Texture3D(std::string name, unsigned int w, unsigned int h, unsigned int d)
	:Engine::AbstractTexture(name), width(w), height(h), depth(d)
{
	data = 0;
}

Engine::Texture3D::~Texture3D()
{
	if (data != 0)
	{
		delete[] data;
	}
}

const unsigned char * Engine::Texture3D::getTexturePixels() const
{
	return data;
}

const unsigned int Engine::Texture3D::getWidth() const
{
	return width;
}

const unsigned int Engine::Texture3D::getHeight() const
{
	return height;
}

const unsigned int Engine::Texture3D::getLayers() const
{
	return depth;
}

void Engine::Texture3D::setSize(unsigned int w, unsigned int h, unsigned int d)
{
	width = w;
	height = h;
	depth = d;
}

void Engine::Texture3D::uploadTexture()
{
	glBindTexture(GL_TEXTURE_3D, textureId);
	glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, width, height, depth, 0, formatType, pixelType, data);

	if (generateMipMaps)
	{
		glGenerateMipmap(GL_TEXTURE_3D);
	}

	if (data != 0)
	{
		delete[] data;
	}
}

GLenum Engine::Texture3D::getTextureType()
{
	return GL_TEXTURE_3D;
}