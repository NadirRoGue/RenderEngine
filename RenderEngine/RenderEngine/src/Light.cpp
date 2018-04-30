/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "Light.h"

Engine::Light::Light(std::string name)
	:name(name)
{
	setEnabled(true);
	update = true;

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

const glm::mat4 & Engine::Light::getModelMatrix() const
{
	return modelMatrix;
}

bool Engine::Light::requiresUpdate()
{
	return update;
}

void Engine::Light::clearUpdateFlag()
{
	update = false;
}

void Engine::Light::setBufferIndex(unsigned int bi)
{
	bufferIndex = bi;
}

unsigned int Engine::Light::getBufferIndex()
{
	return bufferIndex;
}