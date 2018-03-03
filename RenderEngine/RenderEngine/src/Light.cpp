/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#define GLM_FORCE_RADIANS

#include "Light.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace Engine;

Light::Light(std::string name)
{
	setAmbientIntensity(0.0f, 0.0f, 0.0f);
	setDiffuseIntensity(0.0f, 0.0f, 0.0f);
	setSpecularIntensity(0.0f, 0.0f, 0.0f);
	setEnabled(true);

	this->name = std::string(name);
	modelMatrix = glm::mat4(1.0f);
}

Light::~Light()
{
}

const std::string & Light::getName() const
{
	return name;
}

void Light::setEnabled(bool val)
{
	enabled = val;
}

const bool Light::isEnabled() const
{
	return enabled;
}

void Light::setAmbientIntensity(float r, float g, float b)
{
	ambientIntensity[0] = r;
	ambientIntensity[1] = g;
	ambientIntensity[2] = b;
}

void Light::setDiffuseIntensity(float r, float g, float b)
{
	diffuseIntensity[0] = r;
	diffuseIntensity[1] = g;
	diffuseIntensity[2] = b;
}

void Light::setSpecularIntensity(float r, float g, float b)
{
	specularIntensity[0] = r;
	specularIntensity[1] = g;
	specularIntensity[2] = b;
}

const float * Light::getAmbientIntensity() const
{
	return &ambientIntensity[0];
}

const float * Light::getDiffuseIntensity() const
{
	return &diffuseIntensity[0];
}

const float * Light::getSpecularIntensity() const
{
	return &specularIntensity[0];
}

void Light::modifyIntensity(float deltaR, float deltaG, float deltaB)
{
	diffuseIntensity[0] += deltaR;
	diffuseIntensity[1] += deltaG;
	diffuseIntensity[2] += deltaB;
} 

const glm::mat4 & Light::getModelMatrix() const
{
	return modelMatrix;
}

// ========================================================================

PointLight::PointLight(std::string name, glm::vec3 pos)
	:Light(name)
{
	attenuationFactor[0] = 1.0f;
	attenuationFactor[1] = attenuationFactor[2] = 0.0f;
	translate(pos);
}

PointLight::PointLight(std::string name, glm::vec3 pos, glm::vec3 attenuation)
	:Light(name)
{
	attenuationFactor[1] = attenuation.y;
	attenuationFactor[2] = attenuation.z;
	attenuationFactor[0] = attenuation.x < 1 ? 1 : attenuation.x;
	translate(pos);
}

PointLight::~PointLight()
{

}

const glm::vec3 PointLight::getPositioin() const
{
	return glm::vec3(modelMatrix[3][0],modelMatrix[3][1],modelMatrix[3][2]);
}

const float * PointLight::getAttenuationFactor() const
{
	return &attenuationFactor[0];
}

void PointLight::translate(const glm::vec3 & translation)
{
	//position[0] += translation.x;
	//position[1] += translation.y;
	//position[2] += translation.z;
	glm::vec3 position = getPositioin() + translation;
	modelMatrix = glm::translate(glm::mat4(1.0f), position);
}

// =========================================================================

SpotLight::SpotLight(std::string name, glm::vec3 pos, glm::vec3 dir, float apperture, float m)
	:Light(name)
{
	direction = dir;
	appertureAngle = apperture;
	this->m = m;

	attenuationFactor[0] = 1.0f;
	attenuationFactor[1] = attenuationFactor[2] = 0.0f;

	translate(pos);
	directTo(dir);
}

SpotLight::SpotLight(std::string name, glm::vec3 pos, glm::vec3 dir, float apperture, float m, glm::vec3 attenuation)
	:Light(name)
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

SpotLight::~SpotLight()
{
}

const glm::mat4 & SpotLight::getDirModelMatrix() const
{
	return dirMatrix;
}

const glm::vec3 SpotLight::getPosition() const
{
	return glm::vec3(modelMatrix[0][3], modelMatrix[1][3], modelMatrix[2][3]);
}

const glm::vec3 SpotLight::getDirection() const
{
	return glm::vec3(dirMatrix[0][3], dirMatrix[1][3], dirMatrix[2][3]);
}

const float SpotLight::getAppertureAngle() const
{
	return appertureAngle;
}

const float SpotLight::getM() const
{
	return m;
}

const float * SpotLight::getAttenuationFactor() const
{
	return &attenuationFactor[0];
}

void SpotLight::translate(const glm::vec3 & translation)
{
	glm::vec3 position = getPosition() + translation;
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
}

void SpotLight::directTo(const glm::vec3 & dir)
{
	dirMatrix = glm::mat4(1.0f);
	dirMatrix = glm::translate(dirMatrix, dir);
	dirMatrix[3][3] = 0.0f;
}

// ====================================================================

DirectionalLight::DirectionalLight(std::string name, glm::vec3 dir)
	:Light(name)
{
	translate(dir);
}

DirectionalLight::~DirectionalLight()
{
}

const glm::vec3 DirectionalLight::getDirection() const
{
	return glm::vec3(modelMatrix[0][3], modelMatrix[1][3], modelMatrix[2][3]);
}

void DirectionalLight::translate(const glm::vec3 & translation)
{
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, translation);
}