/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "Object.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

std::string Engine::Object::ALBEDO_TEX = "albedo";
std::string Engine::Object::NORMAL_TEX = "normal";
std::string Engine::Object::EMISSIVE_TEX = "emissive";
std::string Engine::Object::SPECULAR_TEX = "specular";

Engine::Object::Object(Engine::MeshInstance * mi)
	:meshInstance(mi)
{
	modelMatrix = glm::mat4(1.0f);
	translation = glm::vec3(0,0,0);
	rotation = glm::vec3(1, 1, 1);
	scaleVector = glm::vec3(1, 1, 1);
	angleR = 0.0f;

	albedo = normal = specular = emissive = 0;
}

Engine::Object::~Object()
{

}

const glm::mat4 & Engine::Object::getModelMatrix() const
{
	return modelMatrix;
}

Engine::MeshInstance * Engine::Object::getMeshInstance()
{
	return meshInstance;
}

Engine::Mesh * Engine::Object::getMesh()
{
	return meshInstance->getMesh();
}

void Engine::Object::rotate(float angle, glm::vec3 r)
{
	rotation += r;
	angleR = angle;

	updateModelMatrix();
}

void Engine::Object::translate(glm::vec3 t)
{
	translation += t;

	updateModelMatrix();
}

void Engine::Object::scale(glm::vec3 s)
{
	scaleVector = s;
	updateModelMatrix();
}

void Engine::Object::setModelMatrix(const glm::mat4 & matrix)
{
	modelMatrix = glm::mat4(matrix);
}

void Engine::Object::updateModelMatrix()
{
	modelMatrix = glm::mat4(1.0f);

	glm::mat4 transMat = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), angleR, rotation);
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scaleVector);

	modelMatrix = transMat * rotMat * scaleMat;
}

void Engine::Object::addTexture(std::string name, Engine::TextureInstance * t)
{
	std::map<std::string, Engine::TextureInstance *>::iterator it = textures.find(name);
	if (it != textures.end())
	{
		Engine::TextureInstance * t2 = it->second;
		t2->~TextureInstance();
		delete t2;
	}

	textures[name] = t;
}

Engine::TextureInstance * Engine::Object::getTexture(std::string name)
{
	std::map<std::string, Engine::TextureInstance *>::iterator it = textures.find(name);
	if (it != textures.end())
	{
		return NULL;
	}

	return textures[name];
}

const std::map<std::string, Engine::TextureInstance *> & Engine::Object::getAllCustomTextures() const
{
	return textures;
}

void Engine::Object::setAlbedoTexture(Engine::TextureInstance * t)
{
	if (albedo != 0)
	{
		albedo->~TextureInstance();
		delete albedo;
	}

	albedo = t;
}

void Engine::Object::setEmissiveTexture(Engine::TextureInstance * t)
{
	if (emissive != 0)
	{
		delete emissive;
	}

	emissive = t;
}

void Engine::Object::setNormalMapTexture(Engine::TextureInstance * t)
{
	if (normal != 0)
	{
		delete normal;
	}

	normal = t;
}

void Engine::Object::setSpecularMapTexture(TextureInstance * t)
{
	if (specular != 0)
	{
		delete specular;
	}

	specular = t;
}

const Engine::TextureInstance * Engine::Object::getAlbedoTexture() const
{
	return albedo;
}

const Engine::TextureInstance * Engine::Object::getEmissiveTexture() const
{
	return emissive;
}

const Engine::TextureInstance * Engine::Object::getNormalMapTexture() const
{
	return normal;
}

const Engine::TextureInstance * Engine::Object::getSpecularMapTexture() const
{
	return specular;
}

