/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "Texture.h"

#include <iostream>

Engine::Texture::Texture(std::string name, unsigned char *data, unsigned int width, unsigned int height)
	:fileName(name), width(width), height(height), data(data)
{
	// Default settings
	internalFormat = GL_RGBA8;
	formatType = GL_RGBA;
	pixelType = GL_UNSIGNED_BYTE;
	generateMipMaps = true;
}

Engine::Texture::~Texture()
{
	if (data != 0)
	{
		delete[] data;
	}
}

const std::string & Engine::Texture::getName() const
{
	return fileName;
}

const unsigned char * Engine::Texture::getTexturePixels() const
{
	return data;
}

const unsigned int Engine::Texture::getWidth() const
{
	return width;
}

const unsigned int Engine::Texture::getHeight() const
{
	return height;
}

const unsigned int Engine::Texture::getTextureId() const
{
	return textureId;
}

void Engine::Texture::setMemoryLayoutFormat(const int format)
{
	internalFormat = format;
}

void Engine::Texture::setImageFormatType(const GLenum type)
{
	formatType = type;
}

void Engine::Texture::setPixelFormatType(const GLenum type)
{
	pixelType = type;
}

void Engine::Texture::setGenerateMipMaps(bool val)
{
	generateMipMaps = val;
}

const int Engine::Texture::getMemoryLayoutFormat() const
{
	return internalFormat;
}

const GLenum Engine::Texture::getImageFormat() const
{
	return formatType;
}

const GLenum Engine::Texture::getPixelFormat() const
{
	return pixelType;
}

void Engine::Texture::generateTexture()
{
	glGenTextures(1, &textureId);
}

void Engine::Texture::setSize(unsigned int w, unsigned int h)
{
	width = w;
	height = h;
}

void Engine::Texture::uploadTexture()
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

// ========================================================================

Engine::TextureInstance::TextureInstance(Engine::Texture * texture)
	:texture(texture)
{
	minificationFilter = GL_LINEAR_MIPMAP_LINEAR;
	magnificationFilter = GL_LINEAR;
	tComponentWrapType = GL_CLAMP;
	sComponentWrapType = GL_CLAMP;
	textureIndex = 0;
	applyAnisotropicFiltering = TextureTable::getInstance().isAnisotropicFilteringSupported();
}

Engine::TextureInstance::~TextureInstance()
{

}

const Engine::Texture * Engine::TextureInstance::getTexture() const
{
	return texture;
}

void Engine::TextureInstance::generateTexture()
{
	texture->generateTexture();
}

void Engine::TextureInstance::uploadTexture()
{
	texture->uploadTexture();
}

void Engine::TextureInstance::setMinificationFilterType(const float min)
{
	minificationFilter = min;
}

void Engine::TextureInstance::setMagnificationFilterType(const float mag)
{
	magnificationFilter = mag;
}

void Engine::TextureInstance::setTComponentWrapType(const float wrapT)
{
	tComponentWrapType = wrapT;
}

void Engine::TextureInstance::setSComponentWrapType(const float wrapS)
{
	sComponentWrapType = wrapS;
}

void Engine::TextureInstance::setTextureIndex(unsigned int index)
{
	textureIndex = index;
}

void Engine::TextureInstance::setAnisotropicFilterEnabled(bool val)
{
	applyAnisotropicFiltering = applyAnisotropicFiltering && val;
}

const float Engine::TextureInstance::getMificationFilterType() const
{
	return minificationFilter;
}

const float Engine::TextureInstance::getMagnificationFilterType() const
{
	return magnificationFilter;
}

const float Engine::TextureInstance::getTWrapType() const
{
	return tComponentWrapType;
}

const float Engine::TextureInstance::getSWrapType() const
{
	return sComponentWrapType;
}

const unsigned int Engine::TextureInstance::getTextureIndex() const
{
	return textureIndex;
}

const bool Engine::TextureInstance::isAnisotropicFilteringEnabled() const
{
	return applyAnisotropicFiltering;
}

void Engine::TextureInstance::reseize(unsigned int w, unsigned int h)
{
	texture->setSize(w, h);
	texture->uploadTexture();
	configureTexture();
}

void Engine::TextureInstance::configureTexture() const
{
	//glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minificationFilter); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnificationFilter); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tComponentWrapType); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sComponentWrapType);

	if (applyAnisotropicFiltering)
	{
		float level = 1.0f * Engine::TextureTable::getInstance().getMaxAnisotropicFilterLevel();
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, level);
	}
}

// ========================================================================

Engine::TextureTable * Engine::TextureTable::INSTANCE = new Engine::TextureTable();

Engine::TextureTable & Engine::TextureTable::getInstance()
{
	return *INSTANCE;
}

Engine::TextureTable::TextureTable()
{
}

Engine::TextureTable::~TextureTable()
{
	destroy();
}

void Engine::TextureTable::checkForAnisotropicFilterSupport()
{
	maxAnisotropicFilterLevel = 0.0f;
	anisotropicFilteringSupport = glewIsSupported("GL_EXT_texture_filter_anisotropic");
	if (anisotropicFilteringSupport)
	{
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropicFilterLevel);
		std::cout << "TextureTable: Maximun anisotropic level supported: " << maxAnisotropicFilterLevel << std::endl;
	}
	else
	{
		std::cout << "TextureTable: Texture anisotropic filtering is not supported" << std::endl;
	}
}

void Engine::TextureTable::cacheTexture(std::string filename)
{
	std::map<std::string, Engine::Texture *>::iterator it = textureTable.find(filename);
	if (it != textureTable.end())
	{
		std::cout << "TextureTable: attempt to load duplicate texture " << filename << std::endl;
		return;
	}

	unsigned int width, height;
	unsigned char *data = loadTexture(filename.c_str(), width, height);

	if (!data)
	{
		std::cout << "TextureTable: error reading " << filename << std::endl;
		return;
	}

	Engine::Texture * texture = new Engine::Texture(filename, data, width, height);
	texture->generateTexture();
	texture->uploadTexture();
	textureTable[filename] = texture;
}

Engine::TextureInstance * Engine::TextureTable::instantiateTexture(std::string fileName)
{
	std::map<std::string, Engine::Texture *>::iterator it = textureTable.find(fileName);

	if (it == textureTable.end())
	{
		std::cout << "TextureTable: Could not instantiate texture " << fileName << " - does not exist on cache" << std::endl;
		return NULL;
	}

	return new Engine::TextureInstance(it->second);
}

const bool Engine::TextureTable::isAnisotropicFilteringSupported() const
{
	return anisotropicFilteringSupport;
}

const float Engine::TextureTable::getMaxAnisotropicFilterLevel() const
{
	return maxAnisotropicFilterLevel;
}

void Engine::TextureTable::destroy()
{
	std::map<std::string, Engine::Texture *>::iterator it = textureTable.begin();
	while (it != textureTable.end())
	{
		unsigned int id = it->second->getTextureId();
		glDeleteTextures(1, &id);
		it++;
	}
}