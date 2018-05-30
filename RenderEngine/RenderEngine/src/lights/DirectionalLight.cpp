#include "lights/DirectionalLight.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "WorldConfig.h"

#include <iostream>

Engine::DirectionalLight::DirectionalLight(std::string name)
	:Engine::Light(name)
{
	Engine::RenderableNotifier::getInstance().registerLight(this);
	setKa(0.15f);
	setKd(1.0f);
	setKs(1.0f);

	setBufferIndex(0);
}

Engine::DirectionalLight::~DirectionalLight()
{
}

const glm::vec3 Engine::DirectionalLight::getDirection() const
{
	return direction;
}

void Engine::DirectionalLight::translate(const glm::vec3 & translation)
{
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, translation);
	modelMatrix[3][3] = 0;

	direction = glm::normalize(glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]));
}

void Engine::DirectionalLight::setDirection(const glm::vec3 & direction)
{
	translate(direction);
}

void Engine::DirectionalLight::setColor(const glm::vec3 & color)
{
	memcpy(shaderData.color, &color[0], sizeof(float) * 3);
	update = true;
}

void Engine::DirectionalLight::setKa(float a)
{
	shaderData.kFactors[0] = a;
	update = true;
}

void Engine::DirectionalLight::setKd(float d)
{
	shaderData.kFactors[1] = d;
	update = true;
}

void Engine::DirectionalLight::setKs(float s)
{
	shaderData.kFactors[2] = s;
	update = true;
}

Engine::DirectionalLightData & Engine::DirectionalLight::getData()
{
	return shaderData;
}