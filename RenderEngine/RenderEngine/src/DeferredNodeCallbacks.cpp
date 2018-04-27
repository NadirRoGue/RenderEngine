/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "DeferredNodeCallbacks.h"

#include "Scene.h"
#include "PostProcessProgram.h"
#include "postprocessprograms/DeferredShadingProgram.h"
#include <iostream>

Engine::DeferredCallback::DeferredCallback()
{

}

Engine::DeferredCallback::~DeferredCallback()
{

}

void Engine::DeferredCallback::initialize(Engine::Object *obj, Program * prog, Engine::DeferredRenderObject * buffer)
{

}

// =========================================================

Engine::MotionBlurImpl::MotionBlurImpl()
{
	r = g = b = 0.5f;
	alpha = 0.6f;
	enabled = true;
}

Engine::MotionBlurImpl::~MotionBlurImpl()
{
}

void Engine::MotionBlurImpl::execute(Engine::Object * obj, Program * prog, Engine::DeferredRenderObject * buffer, Engine::Camera * cam)
{
	if (enabled)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA);
		glBlendColor(r, g, b, alpha);
		glBlendEquation(GL_FUNC_ADD);
	}
}

void Engine::MotionBlurImpl::setAlpha(float a)
{
	alpha = a;
}

void Engine::MotionBlurImpl::setColorMask(float r, float g, float b)
{
	this->r = r;
	this->b = b;
	this->g = g;
}

void Engine::MotionBlurImpl::switchEnabled()
{
	enabled = !enabled;
}

glm::vec4 Engine::MotionBlurImpl::getMotionBlurStats()
{
	return glm::vec4(r, g, b, alpha);
}

// =========================================================

Engine::DeferredShadingLightPopulation::DeferredShadingLightPopulation()
{

}

Engine::DeferredShadingLightPopulation::~DeferredShadingLightPopulation()
{

}

void Engine::DeferredShadingLightPopulation::initialize(Engine::Object *obj, Engine::Program * prog, Engine::DeferredRenderObject * buffer)
{
	Engine::Scene * scene = Engine::SceneManager::getInstance().getActiveScene();

	if (scene == 0)
	{
		return;
	}

	scene->addLightDependentProgram(prog);

	Engine::DeferredShadingProgram * dSP = dynamic_cast<Engine::DeferredShadingProgram *>(prog);

	const std::map<std::string, Engine::PointLight *> pointLights = scene->getPointLights();
	const std::map<std::string, Engine::SpotLight *> spotLights = scene->getSpotLights();
	Engine::DirectionalLight * dl = scene->getDirectionalLight();

	std::map<std::string, Engine::PointLight *>::const_iterator it = pointLights.cbegin();
	while (it != pointLights.end())
	{
		dSP->configurePointLightBuffer(it->second);
		it++;
	}

	std::map<std::string, Engine::SpotLight *>::const_iterator slIt = spotLights.cbegin();
	while (slIt != spotLights.end())
	{
		dSP->configureSpotLightBuffer(slIt->second);
		slIt++;
	}

	dSP->configureDirectionalLightBuffer(dl);

	dSP->configureClearColor(scene->getClearColor());
}

void Engine::DeferredShadingLightPopulation::execute(Engine::Object * obj, Engine::Program * program, Engine::DeferredRenderObject * buffer, Engine::Camera * camera)
{
	Engine::Scene * scene = Engine::SceneManager::getInstance().getActiveScene();

	if (scene == 0)
	{
		return;
	}

	Engine::DeferredShadingProgram * dSP = dynamic_cast<Engine::DeferredShadingProgram *>(program);

	const std::map<std::string, Engine::PointLight *> pointLights = scene->getPointLights();
	const std::map<std::string, Engine::SpotLight *> spotLights = scene->getSpotLights();
	Engine::DirectionalLight * dl = scene->getDirectionalLight();

	std::map<std::string, Engine::PointLight *>::const_iterator it = pointLights.cbegin();
	while (it != pointLights.end())
	{
		dSP->onRenderLight(it->second->getModelMatrix(), camera->getViewMatrix());
		it++;
	}

	std::map<std::string, Engine::SpotLight *>::const_iterator slIt = spotLights.cbegin();
	while (slIt != spotLights.end())
	{
		dSP->onRenderSpotLight(slIt->second->getModelMatrix(), slIt->second->getDirModelMatrix(), camera->getViewMatrix());
		slIt++;
	}

	dSP->onRenderDirectionalLight(dl->getModelMatrix(), camera->getViewMatrix());
	

	dSP->configureClearColor(scene->getClearColor());
}