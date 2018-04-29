/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "Object.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

#include "instances/TextureInstance.h"
#include "WorldConfig.h"

std::string Engine::Object::ALBEDO_TEX = "albedo";
std::string Engine::Object::NORMAL_TEX = "normal";
std::string Engine::Object::EMISSIVE_TEX = "emissive";
std::string Engine::Object::SPECULAR_TEX = "specular";

Engine::Object::Object(Engine::Mesh * m)
	:mesh(m)
{
	modelMatrix = glm::mat4(1.0f);
	translation = glm::vec3(0,0,0);
	rotation = glm::vec3(1, 1, 1);
	scaleVector = glm::vec3(1, 1, 1);
	angleR = 0.0f;

	albedo = normal = specular = emissive = 0;

	renderMode = GL_TRIANGLES;

	Engine::RenderableNotifier::getInstance().registerRenderable(this);
}

Engine::Object::~Object()
{

}

const glm::mat4 & Engine::Object::getModelMatrix() const
{
	return modelMatrix;
}

const Engine::Mesh * Engine::Object::getMesh() const
{
	return mesh;
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
	scaleVector += s;
	updateModelMatrix();
}

void Engine::Object::setTranslation(glm::vec3 t)
{
	translation = t;
	updateModelMatrix();
}

void Engine::Object::setScale(glm::vec3 s)
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

void Engine::Object::setRenderMode(GLenum renderMode)
{
	this->renderMode = renderMode;
}

GLenum Engine::Object::getRenderMode()
{
	return renderMode;
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

// ======================================================================================================

Engine::PostProcessObject::PostProcessObject(Engine::Mesh * mi)
	:Engine::Object(mi)
{
}

void Engine::PostProcessObject::addTexture(std::string name, Engine::TextureInstance * instance)
{
	inputBuffers[name] = instance;
}

Engine::TextureInstance * Engine::PostProcessObject::getTexture(std::string name)
{
	std::map<std::string, Engine::TextureInstance *>::iterator it = inputBuffers.find(name);
	if (it == inputBuffers.end())
	{
		return NULL;
	}

	return inputBuffers[name];
}

const std::map<std::string, Engine::TextureInstance *> & Engine::PostProcessObject::getAllCustomTextures() const
{
	return inputBuffers;
}

void Engine::Object::notifyRenderModeUpdate(Engine::RenderMode mode)
{
	switch (mode)
	{
	case Engine::RenderMode::RENDER_MODE_WIRE:
		setRenderMode(GL_LINES);
		break;
	default:
		setRenderMode(GL_TRIANGLES);
		break;
	}
}