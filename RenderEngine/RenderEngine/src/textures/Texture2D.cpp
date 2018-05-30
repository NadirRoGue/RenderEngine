#include "textures/Texture2D.h"

Engine::Texture2D::Texture2D(std::string name, unsigned char *data, unsigned int width, unsigned int height)
	:Engine::AbstractTexture(name),width(width), height(height)
{
	// Default settings
	internalFormat = GL_RGBA8;
	formatType = GL_RGBA;
	pixelType = GL_UNSIGNED_BYTE;
	generateMipMaps = true;

	if (data != 0)
	{
		this->data = new unsigned char[width * height];
		memcpy(this->data, data, width * height * sizeof(unsigned char));
	}
	else
	{
		this->data = 0;
	}
}

Engine::Texture2D::~Texture2D()
{
	if (data != 0)
	{
		delete[] data;
	}
}

const unsigned char * Engine::Texture2D::getTexturePixels() const
{
	return data;
}

const unsigned int Engine::Texture2D::getWidth() const
{
	return width;
}

const unsigned int Engine::Texture2D::getHeight() const
{
	return height;
}

void Engine::Texture2D::setSize(unsigned int w, unsigned int h, unsigned int d)
{
	width = w;
	height = h;
}

void Engine::Texture2D::uploadTexture()
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, formatType, pixelType, (GLvoid*)data);

	if (generateMipMaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	if (data != 0)
	{
		delete[] data;
	}
}

GLenum Engine::Texture2D::getTextureType()
{
	return GL_TEXTURE_2D;
}