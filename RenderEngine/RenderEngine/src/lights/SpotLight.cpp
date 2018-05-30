#include "lights/SpotLight.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

Engine::SpotLight::SpotLight(std::string name)
	:Engine::Light(name)
{
}

Engine::SpotLight::~SpotLight()
{
}

const glm::mat4 & Engine::SpotLight::getDirModelMatrix() const
{
	return dirMatrix;
}

void Engine::SpotLight::translate(const glm::vec3 & translation)
{
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, translation);
}

void Engine::SpotLight::directTo(const glm::vec3 & dir)
{
	dirMatrix = glm::mat4(1.0f);
	dirMatrix = glm::translate(dirMatrix, dir);
	dirMatrix[3][3] = 0.0f;
}

void Engine::SpotLight::setPosition(const glm::vec3 & pos)
{
	translate(pos);
}

void Engine::SpotLight::setDirection(const glm::vec3 & dir)
{
	directTo(dir);
}

void Engine::SpotLight::setAttenuation(const glm::vec3 & attenuation)
{
	memcpy(&shaderData.attenuation[0], &attenuation[0], sizeof(float) * 3);
	update = true;
}

void Engine::SpotLight::setApperture(float app)
{
	shaderData.direction[3] = app;
	update = true;
}

void Engine::SpotLight::setM(float m)
{
	shaderData.position[3] = m;
	update = true;
}

void Engine::SpotLight::setColor(const glm::vec3 & color)
{
	memcpy(shaderData.color, &color[0], sizeof(float) * 3);
	update = true;
}

void Engine::SpotLight::setKa(float a)
{
	shaderData.kFactors[0] = a;
	update = true;
}

void Engine::SpotLight::setKd(float d)
{
	shaderData.kFactors[1] = d;
	update = true;
}

void Engine::SpotLight::setKs(float s)
{
	shaderData.kFactors[2] = s;
	update = true;
}

Engine::SpotLightData & Engine::SpotLight::getData()
{
	return shaderData;
}