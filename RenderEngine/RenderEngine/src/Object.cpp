/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "Object.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace Engine;

std::string Object::ALBEDO_TEX = "albedo";
std::string Object::NORMAL_TEX = "normal";
std::string Object::EMISSIVE_TEX = "emissive";
std::string Object::SPECULAR_TEX = "specular";

Object::Object(MeshInstance * mi)
	:meshInstance(mi)
{
	modelMatrix = glm::mat4(1.0f);
	translation = glm::vec3(0,0,0);
	rotation = glm::vec3(1, 1, 1);
	scaleVector = glm::vec3(1, 1, 1);
	angleR = 0.0f;

	albedo = normal = specular = emissive = 0;
}

Object::~Object()
{

}

const glm::mat4 & Object::getModelMatrix() const
{
	return modelMatrix;
}

MeshInstance * Object::getMeshInstance()
{
	return meshInstance;
}

Mesh * Object::getMesh()
{
	return meshInstance->getMesh();
}

void Object::rotate(float angle, glm::vec3 r)
{
	rotation += r;
	angleR = angle;

	/*modelMatrix = glm::mat4(1.0f);
	glm::mat4 transMat = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), angleR, rotation);

	modelMatrix = transMat * rotMat;*/
	updateModelMatrix();
}

void Object::translate(glm::vec3 t)
{
	translation += t;

	/*modelMatrix = glm::mat4(1.0f);
	glm::mat4 transMat = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), angleR, rotation);

	modelMatrix = transMat * rotMat;*/
	updateModelMatrix();
}

void Object::scale(glm::vec3 s)
{
	scaleVector = s;
	updateModelMatrix();
}

void Object::setModelMatrix(const glm::mat4 & matrix)
{
	modelMatrix = glm::mat4(matrix);
}

void Object::updateModelMatrix()
{
	/*glm::quat q = glm::quat(rotation);
	q = glm::normalize(q);
	glm::mat4 rotMat = glm::mat4_cast(q);

	glm::mat4 transMat = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scaleVector);

	modelMatrix = transMat * rotMat * scaleMat;*/
	modelMatrix = glm::mat4(1.0f);

	glm::mat4 transMat = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), angleR, rotation);
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scaleVector);

	modelMatrix = transMat * rotMat * scaleMat;
}

void Object::addTexture(std::string name, TextureInstance * t)
{
	std::map<std::string, TextureInstance *>::iterator it = textures.find(name);
	if (it != textures.end())
	{
		TextureInstance * t2 = it->second;
		t2->~TextureInstance();
		delete t2;
	}

	textures[name] = t;
}

TextureInstance * Object::getTexture(std::string name)
{
	std::map<std::string, TextureInstance *>::iterator it = textures.find(name);
	if (it != textures.end())
	{
		return NULL;
	}

	return textures[name];
}

const std::map<std::string, TextureInstance *> & Object::getAllCustomTextures() const
{
	return textures;
}

void Object::setAlbedoTexture(TextureInstance * t)
{
	if (albedo != 0)
	{
		albedo->~TextureInstance();
		delete albedo;
	}

	albedo = t;
}

void Object::setEmissiveTexture(TextureInstance * t)
{
	if (emissive != 0)
	{
		emissive->~TextureInstance();
		delete emissive;
	}

	emissive = t;
}

void Object::setNormalMapTexture(TextureInstance * t)
{
	if (normal != 0)
	{
		normal->~TextureInstance();
		delete normal;
	}

	normal = t;
}

void Object::setSpecularMapTexture(TextureInstance * t)
{
	if (specular != 0)
	{
		specular->~TextureInstance();
		delete specular;
	}

	specular = t;
}

const TextureInstance * Object::getAlbedoTexture() const
{
	return albedo;
}

const TextureInstance * Object::getEmissiveTexture() const
{
	return emissive;
}

const TextureInstance * Object::getNormalMapTexture() const
{
	return normal;
}

const TextureInstance * Object::getSpecularMapTexture() const
{
	return specular;
}

