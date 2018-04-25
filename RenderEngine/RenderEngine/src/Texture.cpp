/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "Texture.h"

const unsigned int Engine::AbstractTexture::getTextureId() const
{
	return textureId;
}

void Engine::AbstractTexture::setMemoryLayoutFormat(const int format)
{
	internalFormat = format;
}

void Engine::AbstractTexture::setImageFormatType(const GLenum type)
{
	formatType = type;
}

void Engine::AbstractTexture::setPixelFormatType(const GLenum type)
{
	pixelType = type;
}

void Engine::AbstractTexture::setGenerateMipMaps(bool val)
{
	generateMipMaps = val;
}

const int Engine::AbstractTexture::getMemoryLayoutFormat() const
{
	return internalFormat;
}

const GLenum Engine::AbstractTexture::getImageFormat() const
{
	return formatType;
}

const GLenum Engine::AbstractTexture::getPixelFormat() const
{
	return pixelType;
}

void Engine::AbstractTexture::generateTexture()
{
	glGenTextures(1, &textureId);
}