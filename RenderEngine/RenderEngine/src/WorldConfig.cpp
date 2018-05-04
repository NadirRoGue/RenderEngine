#include "WorldConfig.h"

unsigned int Engine::Settings::drawingMethod = 0;
unsigned int Engine::Settings::travelMethod = 0;

glm::vec3 Engine::Settings::lightColor = glm::vec3(1, 1, 1);
glm::vec3 Engine::Settings::lightDirection = glm::vec3(1, 0.515, 0);

float Engine::Settings::terrainAmplitude = 0.6f;
float Engine::Settings::terrainFrecuency = 0.5f;
float Engine::Settings::terrainScale = 0.9f;
unsigned int Engine::Settings::terrainOctaves = 8;

glm::vec3 Engine::Settings::waterColor = glm::vec3(0.f, 0.1f, 0.2f);
float Engine::Settings::waterSpeed = 0.005f;
float Engine::Settings::waterHeight = 0.09f;

bool Engine::Settings::showUI = false;

// =====================================================================

Engine::RenderableNotifier * Engine::RenderableNotifier::INSTANCE = new Engine::RenderableNotifier();

Engine::RenderableNotifier & Engine::RenderableNotifier::getInstance()
{
	return *INSTANCE;
}

Engine::RenderableNotifier::RenderableNotifier()
{
	previousDrawingMethod = Engine::Settings::drawingMethod;
	previousLightColor = Engine::Settings::lightColor;
	previousLightDir = Engine::Settings::lightDirection;
}

void Engine::RenderableNotifier::registerRenderable(Engine::IRenderable * renderable)
{
	renderables.push_back(renderable);
}

void Engine::RenderableNotifier::registerLight(Engine::Light * light)
{
	lights.push_back(light);
}

void Engine::RenderableNotifier::checkUpdatedConfig()
{
	if (previousDrawingMethod != Engine::Settings::drawingMethod)
	{
		previousDrawingMethod = Engine::Settings::drawingMethod;
		RenderMode mode = static_cast<RenderMode>(previousDrawingMethod);
		for (auto render : renderables)
		{
			render->notifyRenderModeUpdate(mode);
		}
	}

	if (previousLightColor != Engine::Settings::lightColor
		|| previousLightDir != Engine::Settings::lightDirection)
	{
		previousLightColor = Engine::Settings::lightColor;
		previousLightDir = Engine::Settings::lightDirection;

		for (auto light : lights)
		{
			light->translate(previousLightDir);
			light->setColor(previousLightColor);
		}
	}
}