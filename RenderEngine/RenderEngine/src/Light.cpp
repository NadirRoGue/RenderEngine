/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#define GLM_FORCE_RADIANS

#include "Light.h"

#include <glm/gtc/matrix_transform.hpp>

Engine::Light::Light(std::string name)
{
	setAmbientIntensity(0.0f, 0.0f, 0.0f);
	setDiffuseIntensity(0.0f, 0.0f, 0.0f);
	setSpecularIntensity(0.0f, 0.0f, 0.0f);
	setEnabled(true);

	this->name = std::string(name);
	modelMatrix = glm::mat4(1.0f);
}

Engine::Light::~Light()
{
}

const std::string & Engine::Light::getName() const
{
	return name;
}

void Engine::Light::setEnabled(bool val)
{
	enabled = val;
}

const bool Engine::Light::isEnabled() const
{
	return enabled;
}

void Engine::Light::setAmbientIntensity(float r, float g, float b)
{
	ambientIntensity[0] = r;
	ambientIntensity[1] = g;
	ambientIntensity[2] = b;
}

void Engine::Light::setDiffuseIntensity(float r, float g, float b)
{
	diffuseIntensity[0] = r;
	diffuseIntensity[1] = g;
	diffuseIntensity[2] = b;
}

void Engine::Light::setSpecularIntensity(float r, float g, float b)
{
	specularIntensity[0] = r;
	specularIntensity[1] = g;
	specularIntensity[2] = b;
}

const float * Engine::Light::getAmbientIntensity() const
{
	return &ambientIntensity[0];
}

const float * Engine::Light::getDiffuseIntensity() const
{
	return &diffuseIntensity[0];
}

const float * Engine::Light::getSpecularIntensity() const
{
	return &specularIntensity[0];
}

void Engine::Light::modifyIntensity(float deltaR, float deltaG, float deltaB)
{
	diffuseIntensity[0] += deltaR;
	diffuseIntensity[1] += deltaG;
	diffuseIntensity[2] += deltaB;
} 

const glm::mat4 & Engine::Light::getModelMatrix() const
{
	return modelMatrix;
}

// ========================================================================

Engine::PointLight::PointLight(std::string name, glm::vec3 pos)
	:Engine::Light(name)
{
	attenuationFactor[0] = 1.0f;
	attenuationFactor[1] = attenuationFactor[2] = 0.0f;
	translate(pos);
}

Engine::PointLight::PointLight(std::string name, glm::vec3 pos, glm::vec3 attenuation)
	:Engine::Light(name)
{
	attenuationFactor[1] = attenuation.y;
	attenuationFactor[2] = attenuation.z;
	attenuationFactor[0] = attenuation.x < 1 ? 1 : attenuation.x;
	translate(pos);
}

Engine::PointLight::~PointLight()
{

}

const glm::vec3 Engine::PointLight::getPositioin() const
{
	return glm::vec3(modelMatrix[3][0],modelMatrix[3][1],modelMatrix[3][2]);
}

const float * Engine::PointLight::getAttenuationFactor() const
{
	return &attenuationFactor[0];
}

void Engine::PointLight::translate(const glm::vec3 & translation)
{
	glm::vec3 position = getPositioin() + translation;
	modelMatrix = glm::translate(glm::mat4(1.0f), position);
}

// =========================================================================

Engine::SpotLight::SpotLight(std::string name, glm::vec3 pos, glm::vec3 dir, float apperture, float m)
	:Engine::Light(name)
{
	direction = dir;
	appertureAngle = apperture;
	this->m = m;

	attenuationFactor[0] = 1.0f;
	attenuationFactor[1] = attenuationFactor[2] = 0.0f;

	translate(pos);
	directTo(dir);
}

Engine::SpotLight::SpotLight(std::string name, glm::vec3 pos, glm::vec3 dir, float apperture, float m, glm::vec3 attenuation)
	:Engine::Light(name)
{
	dirMatrix = glm::mat4(1.0f);

	attenuationFactor[0] = attenuation.x;
	attenuationFactor[1] = attenuation.y;
	attenuationFactor[2] = attenuation.z;
	direction = dir;
	appertureAngle = apperture;
	this->m = m;

	translate(pos);
	directTo(dir);
}

Engine::SpotLight::~SpotLight()
{
}

const glm::mat4 & Engine::SpotLight::getDirModelMatrix() const
{
	return dirMatrix;
}

const glm::vec3 Engine::SpotLight::getPosition() const
{
	return glm::vec3(modelMatrix[0][3], modelMatrix[1][3], modelMatrix[2][3]);
}

const glm::vec3 Engine::SpotLight::getDirection() const
{
	return glm::vec3(dirMatrix[0][3], dirMatrix[1][3], dirMatrix[2][3]);
}

const float Engine::SpotLight::getAppertureAngle() const
{
	return appertureAngle;
}

const float Engine::SpotLight::getM() const
{
	return m;
}

const float * Engine::SpotLight::getAttenuationFactor() const
{
	return &attenuationFactor[0];
}

void Engine::SpotLight::translate(const glm::vec3 & translation)
{
	glm::vec3 position = getPosition() + translation;
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
}

void Engine::SpotLight::directTo(const glm::vec3 & dir)
{
	dirMatrix = glm::mat4(1.0f);
	dirMatrix = glm::translate(dirMatrix, dir);
	dirMatrix[3][3] = 0.0f;
}

// ====================================================================

Engine::DirectionalLight::DirectionalLight(std::string name, glm::vec3 dir)
	:Engine::Light(name)
{
	translate(dir);
}

Engine::DirectionalLight::~DirectionalLight()
{
}

const glm::vec3 Engine::DirectionalLight::getDirection() const
{
	return glm::vec3(modelMatrix[0][3], modelMatrix[1][3], modelMatrix[2][3]);
}

void Engine::DirectionalLight::translate(const glm::vec3 & translation)
{
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, translation);
}