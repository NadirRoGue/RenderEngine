#include "WorldConfig.h"

unsigned int Engine::Settings::worldSeed = 0;

unsigned int Engine::Settings::drawingMethod = 0;
unsigned int Engine::Settings::travelMethod = 0;

glm::vec3 Engine::Settings::lightColor = glm::vec3(1, 1, 1);
float Engine::Settings::lightFactor = 1.0f;
glm::vec3 Engine::Settings::realLightColor = glm::vec3(1, 1, 1);
glm::vec3 Engine::Settings::lightDirection = glm::vec3(1, 1, 0);

float Engine::Settings::worldTileScale = 7.0f;
unsigned int Engine::Settings::worldRenderRadius = 12;

glm::vec3 Engine::Settings::windDirection = glm::vec3(1, 0, 0);
float Engine::Settings::windStrength = 0.5f;

float Engine::Settings::terrainAmplitude = 0.732f;
float Engine::Settings::terrainFrecuency = 0.244f;
float Engine::Settings::terrainScale = 0.9f;
unsigned int Engine::Settings::terrainOctaves = 10;
float Engine::Settings::vegetationMaxHeight = 0.1f;
float Engine::Settings::grassCoverage = 0.5f;
glm::vec3 Engine::Settings::grassColor = glm::vec3(0.1f, 0.3f, 0.0f);
glm::vec3 Engine::Settings::sandColor = glm::vec3(0.94f, 0.89f, 0.5f);
glm::vec3 Engine::Settings::rockColor = glm::vec3(0.40f);

glm::vec3 Engine::Settings::skyZenitColor = glm::vec3(0.0f, 0.5f, 1.0f);
glm::vec3 Engine::Settings::skyHorizonColor = glm::vec3(0.8f, 0.85f, 1.0f);

glm::vec3 Engine::Settings::waterColor = glm::vec3(0.06f, 0.52f, 0.337f);
float Engine::Settings::waterSpeed = 0.0025f;
float Engine::Settings::waterHeight = 0.09f;

unsigned int Engine::Settings::drawClouds = 0;
float Engine::Settings::cloudType = 0.5f;
float Engine::Settings::coverageMultiplier = 1.0f;
float Engine::Settings::innerSphereRadius	= 10000000.0f * 2.0;
float Engine::Settings::outerSphereRadius	= 10900000.0f * 2.0;
float Engine::Settings::sphereYOffset		= -9600000.0f * 2.0;
float Engine::Settings::cloudMaxRenderDistance = 6000000.0f;
float Engine::Settings::cloudTopOffset = 0.0f;
float Engine::Settings::weatherTextureScale = 1.0f;
float Engine::Settings::baseNoiseScale = 1.0f;
float Engine::Settings::highFrequencyNoiseScale = 1.0f;
float Engine::Settings::highFrequencyNoiseUVScale = 150.0f;
float Engine::Settings::highFrequencyNoiseHScale = 4.0f;
glm::vec3 Engine::Settings::cloudColor = glm::vec3(1, 1, 1);

float Engine::Settings::dofFocalDist = 70.0f;
float Engine::Settings::dofMaxDist = 0.01f;

float Engine::Settings::hdrExposure = 6.0f;
float Engine::Settings::hdrGamma = 0.368f;
glm::vec3 Engine::Settings::hdrTint = glm::vec3(1.0f);

float Engine::Settings::godRaysDecay = 0.904f;
float Engine::Settings::godRaysDensity = 0.318f;
float Engine::Settings::godRaysExposure = 0.515f;
float Engine::Settings::godRaysWeight = 0.2f;

bool Engine::Settings::showUI = false;

void Engine::Settings::update()
{
	lightFactor = glm::clamp(glm::dot(glm::vec3(0, 1, 0), glm::normalize(lightDirection)), 0.0f, 1.0f);
	realLightColor = lightColor;
	realLightColor.y *= lightFactor * 0.85f;
	realLightColor.z *= lightFactor * 0.55f;

	Engine::Settings::hdrExposure = glm::mix(8.5f, 3.235f, lightFactor);
	Engine::Settings::hdrGamma = glm::mix(0.25f, 0.3f, lightFactor);
}

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