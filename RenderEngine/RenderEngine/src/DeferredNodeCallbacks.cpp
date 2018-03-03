/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "DeferredNodeCallbacks.h"

#include "Scene.h"
#include "PostProcessProgram.h"
#include <iostream>
using namespace Engine;

DeferredCallback::DeferredCallback()
{

}

DeferredCallback::~DeferredCallback()
{

}

void DeferredCallback::initialize(Object *obj, Program * prog, DeferredRenderObject * buffer)
{

}

// =========================================================

MotionBlurImpl::MotionBlurImpl()
{
	r = g = b = 0.5f;
	alpha = 0.6f;
	enabled = true;
}

MotionBlurImpl::~MotionBlurImpl()
{
}

void MotionBlurImpl::execute(Object * obj, Program * prog, DeferredRenderObject * buffer, Camera * cam)
{
	if (enabled)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA);
		glBlendColor(r, g, b, alpha);
		glBlendEquation(GL_FUNC_ADD);
	}
}

void MotionBlurImpl::setAlpha(float a)
{
	alpha = a;
}

void MotionBlurImpl::setColorMask(float r, float g, float b)
{
	this->r = r;
	this->b = b;
	this->g = g;
}

void MotionBlurImpl::switchEnabled()
{
	enabled = !enabled;
}

glm::vec4 MotionBlurImpl::getMotionBlurStats()
{
	return glm::vec4(r, g, b, alpha);
}

// =========================================================

DeferredShadingLightPopulation::DeferredShadingLightPopulation()
{

}

DeferredShadingLightPopulation::~DeferredShadingLightPopulation()
{

}

void DeferredShadingLightPopulation::initialize(Object *obj, Program * prog, DeferredRenderObject * buffer)
{
	Scene * scene = SceneManager::getInstance().getActiveScene();

	if (scene == 0)
	{
		return;
	}

	scene->addLightDependentProgram(prog);

	DeferredShadingProgram * dSP = dynamic_cast<DeferredShadingProgram *>(prog);

	const std::map<std::string, PointLight *> pointLights = scene->getPointLights();
	const std::map<std::string, SpotLight *> spotLights = scene->getSpotLights();
	const std::map<std::string, DirectionalLight *> directionalLights = scene->getDirectionalLight();

	std::map<std::string, PointLight *>::const_iterator it = pointLights.cbegin();
	while (it != pointLights.end())
	{
		dSP->configurePointLightBuffer(it->second);
		it++;
	}

	std::map<std::string, SpotLight *>::const_iterator slIt = spotLights.cbegin();
	while (slIt != spotLights.end())
	{
		dSP->configureSpotLightBuffer(slIt->second);
		slIt++;
	}

	std::map<std::string, DirectionalLight *>::const_iterator dlIt = directionalLights.cbegin();
	while (dlIt != directionalLights.end())
	{
		dSP->configureDirectionalLightBuffer(dlIt->second);
		dlIt++;
	}

	dSP->configureClearColor(scene->getClearColor());
}

void DeferredShadingLightPopulation::execute(Object * obj, Program * program, DeferredRenderObject * buffer, Camera * camera)
{
	Scene * scene = SceneManager::getInstance().getActiveScene();

	if (scene == 0)
	{
		return;
	}

	DeferredShadingProgram * dSP = dynamic_cast<DeferredShadingProgram *>(program);

	const std::map<std::string, PointLight *> pointLights = scene->getPointLights();
	const std::map<std::string, SpotLight *> spotLights = scene->getSpotLights();
	const std::map<std::string, DirectionalLight *> directionalLights = scene->getDirectionalLight();

	std::map<std::string, PointLight *>::const_iterator it = pointLights.cbegin();
	while (it != pointLights.end())
	{
		dSP->onRenderLight(it->second->getModelMatrix(), camera->getViewMatrix());
		it++;
	}

	std::map<std::string, SpotLight *>::const_iterator slIt = spotLights.cbegin();
	while (slIt != spotLights.end())
	{
		dSP->onRenderSpotLight(slIt->second->getModelMatrix(), slIt->second->getDirModelMatrix(), camera->getViewMatrix());
		slIt++;
	}

	std::map<std::string, DirectionalLight *>::const_iterator dlIt = directionalLights.cbegin();
	while (dlIt != directionalLights.end())
	{
		dSP->onRenderDirectionalLight(dlIt->second->getModelMatrix(), camera->getViewMatrix());
		dlIt++;
	}
}