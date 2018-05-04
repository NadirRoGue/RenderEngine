#include "textures/TextureCubemap.h"

Engine::TextureCubemap::TextureCubemap(std::string name, unsigned int tileWidth, unsigned int tileHeight)
	:Engine::AbstractTexture(name),tileWidth(tileWidth), tileHeight(tileHeight)
{
	internalFormat = GL_RGBA;
	formatType = GL_RGBA;
	pixelType = GL_UNSIGNED_BYTE;
	generateMipMaps = false;
	glEnable(GL_TEXTURE_CUBE_MAP);

	unsigned int size = tileWidth * tileHeight * 4;
	for (unsigned int i = 0; i < 6; i++)
	{
		data[i] = new unsigned char[size];
	}
}

Engine::TextureCubemap::~TextureCubemap()
{
	for (unsigned int i = 0; i < 6; i++)
	{
		if (data[i] != 0)
		{
			delete[] data[i];
		}
	}
}

void Engine::TextureCubemap::setTileData(unsigned int index, unsigned char * data)
{
	if (index < 6)
	{
		memcpy(this->data[index], data, tileWidth * tileHeight * sizeof(unsigned char) * 4);
	}
}

const unsigned char * Engine::TextureCubemap::getPixelsFromTile(unsigned int index) const
{
	if (index < 6)
	{
		return data[index];
	}

	return NULL;
}

const unsigned int Engine::TextureCubemap::getTileWidth() const
{
	return tileWidth;
}

const unsigned int Engine::TextureCubemap::getTileHeight() const
{
	return tileHeight;
}

void Engine::TextureCubemap::setSize(unsigned int w, unsigned int h)
{
	tileWidth = w;
	tileHeight = h;
	unsigned int size = w * h * 4;

	for (unsigned int i = 0; i < 6; i++)
	{
		delete[] data[i];
		data[i] = new unsigned char[size];
	}
}

void Engine::TextureCubemap::uploadTexture()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, tileWidth, tileHeight, 0, formatType, GL_UNSIGNED_BYTE, (GLvoid*)data[i]);
		delete[] data[i];
	}

	if (generateMipMaps)
	{
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}
}

GLenum Engine::TextureCubemap::getTextureType()
{
	return GL_TEXTURE_CUBE_MAP;
}