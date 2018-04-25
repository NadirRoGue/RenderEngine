/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "Light.h"

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