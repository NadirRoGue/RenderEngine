#include "lights/DirectionalLight.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "WorldConfig.h"

Engine::DirectionalLight::DirectionalLight(std::string name, glm::vec3 dir)
	:Engine::Light(name)
{
	translate(dir);

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
}