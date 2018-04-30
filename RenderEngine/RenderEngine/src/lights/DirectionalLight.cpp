#include "lights/DirectionalLight.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "WorldConfig.h"

Engine::DirectionalLight::DirectionalLight(std::string name)
	:Engine::Light(name)
{
	Engine::RenderableNotifier::getInstance().registerLight(this);
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
	modelMatrix[3][3] = 0;
}

void Engine::DirectionalLight::setDirection(const glm::vec3 & direction)
{
	translate(direction);
}

void Engine::DirectionalLight::setColor(float r, float g, float b)
{
	shaderData.color[0] = r;
	shaderData.color[1] = g;
	shaderData.color[2] = b;
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