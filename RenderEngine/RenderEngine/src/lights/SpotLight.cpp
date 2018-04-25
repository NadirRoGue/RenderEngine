#include "lights/SpotLight.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

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