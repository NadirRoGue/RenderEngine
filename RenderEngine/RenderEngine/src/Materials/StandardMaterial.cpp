/*
#include "Materials\StandardMaterial.h"

Engine::Material::StandardMaterial::StandardMaterial() : AbstractMaterial()
{
	flags = 0;
}

Engine::Material::StandardMaterial::~StandardMaterial()
{

}

void Engine::Material::StandardMaterial::setColor(glm::vec3 & color)
{
	this->color = color;
}

void Engine::Material::StandardMaterial::setSpecularColor(glm::vec3 & color)
{
	this->specularColor = color;
}

void Engine::Material::StandardMaterial::setEmissiveColor(glm::vec3 & color)
{
	this->emissiveColor = color;
}

void Engine::Material::StandardMaterial::setAlbedoTexture(TextureInstance * texture)
{
	if (texture != NULL)
	{
		this->albedoTextureID = texture->getTexture()->getTextureId();
		flags |= Engine::GPUProgram::TEXTURE_ABLEDO_FLAG;
	}
}

void Engine::Material::StandardMaterial::setSpecularTexture(TextureInstance * texture)
{
	if (texture != NULL)
	{
		this->specularTextureID = texture->getTexture()->getTextureId();
		flags |= Engine::GPUProgram::TEXTURE_SPECULAR_FLAG;
	}
}

void Engine::Material::StandardMaterial::setEmissiveTexture(TextureInstance * texture)
{
	if (texture != NULL)
	{
		this->emissiveTextureID = texture->getTexture()->getTextureId();
		flags |= Engine::GPUProgram::TEXTURE_EMISSIVE_FLAG;
	}
}

void Engine::Material::StandardMaterial::setNormalTexture(TextureInstance * texture)
{
	if (texture != NULL)
	{
		this->normalTextureID = texture->getTexture()->getTextureId();
		flags |= Engine::GPUProgram::TEXTURE_NORMAL_FLAG;
	}
}


void Engine::Material::StandardMaterial::buildShader()
{
	shader = factory.create(flags);
}


void Engine::Material::StandardMaterial::populateShader()
{

}
*/