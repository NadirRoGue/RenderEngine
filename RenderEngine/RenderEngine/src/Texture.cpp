/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "Texture.h"

#include <iostream>

using namespace Engine;

Texture::Texture(std::string name, unsigned char *data, unsigned int width, unsigned int height)
	:fileName(name), width(width), height(height), data(data)
{
	// Default settings
	internalFormat = GL_RGBA8;
	formatType = GL_RGBA;
	pixelType = GL_UNSIGNED_BYTE;
	generateMipMaps = true;
}

Texture::~Texture()
{
	if (data != 0)
	{
		delete[] data;
	}
}

const std::string & Texture::getName() const
{
	return fileName;
}

const unsigned char * Texture::getTexturePixels() const
{
	return data;
}

const unsigned int Texture::getWidth() const
{
	return width;
}

const unsigned int Texture::getHeight() const
{
	return height;
}

const unsigned int Texture::getTextureId() const
{
	return textureId;
}

void Texture::setMemoryLayoutFormat(const int format)
{
	internalFormat = format;
}

void Texture::setImageFormatType(const GLenum type)
{
	formatType = type;
}

void Texture::setPixelFormatType(const GLenum type)
{
	pixelType = type;
}

void Texture::setGenerateMipMaps(bool val)
{
	generateMipMaps = val;
}

const int Texture::getMemoryLayoutFormat() const
{
	return internalFormat;
}

const GLenum Texture::getImageFormat() const
{
	return formatType;
}

const GLenum Texture::getPixelFormat() const
{
	return pixelType;
}

void Texture::generateTexture()
{
	glGenTextures(1, &textureId);
}

void Texture::setSize(unsigned int w, unsigned int h)
{
	width = w;
	height = h;
}

void Texture::uploadTexture()
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

TextureInstance::TextureInstance(Texture * texture)
	:texture(texture)
{
	minificationFilter = GL_LINEAR_MIPMAP_LINEAR;
	magnificationFilter = GL_LINEAR;
	tComponentWrapType = GL_CLAMP;
	sComponentWrapType = GL_CLAMP;
	textureIndex = 0;
	applyAnisotropicFiltering = TextureTable::getInstance().isAnisotropicFilteringSupported();
}

TextureInstance::~TextureInstance()
{

}

const Texture * TextureInstance::getTexture() const
{
	return texture;
}

void TextureInstance::generateTexture()
{
	texture->generateTexture();
}

void TextureInstance::uploadTexture()
{
	texture->uploadTexture();
}

void TextureInstance::setMinificationFilterType(const float min)
{
	minificationFilter = min;
}

void TextureInstance::setMagnificationFilterType(const float mag)
{
	magnificationFilter = mag;
}

void TextureInstance::setTComponentWrapType(const float wrapT)
{
	tComponentWrapType = wrapT;
}

void TextureInstance::setSComponentWrapType(const float wrapS)
{
	sComponentWrapType = wrapS;
}

void TextureInstance::setTextureIndex(unsigned int index)
{
	textureIndex = index;
}

void TextureInstance::setAnisotropicFilterEnabled(bool val)
{
	applyAnisotropicFiltering = applyAnisotropicFiltering && val;
}

const float TextureInstance::getMificationFilterType() const
{
	return minificationFilter;
}

const float TextureInstance::getMagnificationFilterType() const
{
	return magnificationFilter;
}

const float TextureInstance::getTWrapType() const
{
	return tComponentWrapType;
}

const float TextureInstance::getSWrapType() const
{
	return sComponentWrapType;
}

const unsigned int TextureInstance::getTextureIndex() const
{
	return textureIndex;
}

const bool TextureInstance::isAnisotropicFilteringEnabled() const
{
	return applyAnisotropicFiltering;
}

void TextureInstance::reseize(unsigned int w, unsigned int h)
{
	texture->setSize(w, h);
	texture->uploadTexture();
	configureTexture();
}

void TextureInstance::configureTexture() const
{
	//glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minificationFilter); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnificationFilter); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tComponentWrapType); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sComponentWrapType);

	if (applyAnisotropicFiltering)
	{
		float level = 1.0f * TextureTable::getInstance().getMaxAnisotropicFilterLevel();
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, level);
	}
}

// ========================================================================

TextureTable * TextureTable::INSTANCE = new TextureTable();

TextureTable & TextureTable::getInstance()
{
	return *INSTANCE;
}

TextureTable::TextureTable()
{
}

TextureTable::~TextureTable()
{
	destroy();
}

void TextureTable::checkForAnisotropicFilterSupport()
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

void TextureTable::cacheTexture(std::string filename)
{
	std::map<std::string, Texture *>::iterator it = textureTable.find(filename);
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

	Texture * texture = new Texture(filename, data, width, height);
	texture->generateTexture();
	texture->uploadTexture();
	textureTable[filename] = texture;
}

TextureInstance * TextureTable::instantiateTexture(std::string fileName)
{
	std::map<std::string, Texture *>::iterator it = textureTable.find(fileName);

	if (it == textureTable.end())
	{
		std::cout << "TextureTable: Could not instantiate texture " << fileName << " - does not exist on cache" << std::endl;
		return NULL;
	}

	return new TextureInstance(it->second);
}

const bool TextureTable::isAnisotropicFilteringSupported() const
{
	return anisotropicFilteringSupport;
}

const float TextureTable::getMaxAnisotropicFilterLevel() const
{
	return maxAnisotropicFilterLevel;
}

void TextureTable::destroy()
{
	std::map<std::string, Texture *>::iterator it = textureTable.begin();
	while (it != textureTable.end())
	{
		unsigned int id = it->second->getTextureId();
		glDeleteTextures(1, &id);
		it++;
	}
}