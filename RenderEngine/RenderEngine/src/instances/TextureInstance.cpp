#include "instances/TextureInstance.h"

#include "datatables/TextureTable.h"

Engine::TextureInstance::TextureInstance(Engine::AbstractTexture * texture)
	:texture(texture)
{
	minificationFilter = GL_LINEAR_MIPMAP_LINEAR;
	magnificationFilter = GL_LINEAR;
	tComponentWrapType = GL_CLAMP_TO_EDGE;
	sComponentWrapType = GL_CLAMP_TO_EDGE;
	rComponentWrapType = GL_CLAMP_TO_EDGE;
	applyAnisotropicFiltering = TextureTable::getInstance().isAnisotropicFilteringSupported();
}

Engine::TextureInstance::~TextureInstance()
{

}

const Engine::AbstractTexture * Engine::TextureInstance::getTexture() const
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

void Engine::TextureInstance::setMinificationFilterType(const int min)
{
	minificationFilter = min;
}

void Engine::TextureInstance::setMagnificationFilterType(const int mag)
{
	magnificationFilter = mag;
}

void Engine::TextureInstance::setTComponentWrapType(const int wrapT)
{
	tComponentWrapType = wrapT;
}

void Engine::TextureInstance::setSComponentWrapType(const int wrapS)
{
	sComponentWrapType = wrapS;
}

void Engine::TextureInstance::setRComponentWrapType(const int wrapR)
{
	rComponentWrapType = wrapR;
}

void Engine::TextureInstance::setAnisotropicFilterEnabled(bool val)
{
	applyAnisotropicFiltering = applyAnisotropicFiltering && val;
}

const int Engine::TextureInstance::getMificationFilterType() const
{
	return minificationFilter;
}

const int Engine::TextureInstance::getMagnificationFilterType() const
{
	return magnificationFilter;
}

const int Engine::TextureInstance::getTWrapType() const
{
	return tComponentWrapType;
}

const int Engine::TextureInstance::getSWrapType() const
{
	return sComponentWrapType;
}

const int Engine::TextureInstance::getRWrapType() const
{
	return rComponentWrapType;
}

const bool Engine::TextureInstance::isAnisotropicFilteringEnabled() const
{
	return applyAnisotropicFiltering;
}

void Engine::TextureInstance::resize(unsigned int w, unsigned int h)
{
	texture->setSize(w, h);
	texture->uploadTexture();
	configureTexture();
}

void Engine::TextureInstance::configureTexture() const
{
	GLenum type = texture->getTextureType();
	glBindTexture(type, texture->getTextureId());
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, minificationFilter);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, magnificationFilter);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, tComponentWrapType);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, sComponentWrapType);
	glTexParameteri(type, GL_TEXTURE_WRAP_R, rComponentWrapType);

	if (applyAnisotropicFiltering)
	{
		float level = 1.0f * Engine::TextureTable::getInstance().getMaxAnisotropicFilterLevel();
		glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, level);
	}
}