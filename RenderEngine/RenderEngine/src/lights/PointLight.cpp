#include "lights/PointLight.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

Engine::PointLight::PointLight(std::string name)
	:Engine::Light(name)
{
}

Engine::PointLight::~PointLight()
{

}

void Engine::PointLight::translate(const glm::vec3 & translation)
{
	modelMatrix = glm::translate(glm::mat4(1.0f), translation);
}

void Engine::PointLight::setPosition(const glm::vec3 & pos)
{
	translate(pos);
}

void Engine::PointLight::setAttenuation(const glm::vec3 & att)
{
	memcpy(&shaderData.attenuation[0], &att[0], sizeof(float) * 3);
	update = true;
}

void Engine::PointLight::setColor(float r, float g, float b)
{
	shaderData.color[0] = r;
	shaderData.color[1] = g;
	shaderData.color[2] = b;
	update = true;
}

void Engine::PointLight::setKa(float a)
{
	shaderData.kFactors[0] = a;
	update = true;
}

void Engine::PointLight::setKd(float d)
{
	shaderData.kFactors[1] = d;
	update = true;
}

void Engine::PointLight::setKs(float s)
{
	shaderData.kFactors[2] = s;
	update = true;
}

Engine::PointLightData & Engine::PointLight::getData()
{
	return shaderData;
}