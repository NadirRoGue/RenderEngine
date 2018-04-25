#include "lights/PointLight.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

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
	return glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
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