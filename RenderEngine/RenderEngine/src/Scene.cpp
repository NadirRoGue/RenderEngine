/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "Scene.h"
#include <gl/glew.h>

#include <iostream>

#include "ProgramTable.h"

using namespace Engine;

Scene::Scene()
{
	clearColor = glm::vec3(0, 0, 0);

	keyboardHandlers = new KeyboardHandlersTable();
	mouseHandlers = new MouseEventManager();
}

Scene::~Scene()
{
	if (keyboardHandlers != NULL)
	{
		delete keyboardHandlers;
	}

	if (mouseHandlers != NULL)
	{
		delete mouseHandlers;
	}
}

void Scene::setClearColor(glm::vec3 cc)
{
	clearColor = cc;
	std::map<std::string, ProgramRenderables *>::const_iterator renderableIt;
	for (renderableIt = renders.cbegin(); renderableIt != renders.cend(); renderableIt++)
	{
		renderableIt->second->program->configureClearColor(clearColor);
	}
}

const glm::vec3 & Scene::getClearColor() const
{
	return clearColor;
}

void Scene::addObject(Object *obj)
{
	std::string material = obj->getMeshInstance()->getMaterial();
	unsigned int vaoIndex = obj->getMeshInstance()->vao;

	std::map<std::string, ProgramRenderables *>::iterator renderIt = renders.find(material);

	if (renderIt == renders.end())
	{
		Program * prog = ProgramTable::getInstance().getProgramByName(material);

		if(prog == nullptr)
		{
			return;
		}

		configureNewProgramLights(prog);

		ProgramRenderables * renderable = new ProgramRenderables(prog);
		renderable->objects[vaoIndex].push_back(obj);
		renders[material] = renderable;
	}
	else
	{
		renderIt->second->objects[vaoIndex].push_back(obj);
	}
}

void Scene::addLightDependentProgram(Program * prog)
{
	lightDependentPrograms.push_back(prog);
}

void Scene::configureNewProgramLights(Program * p)
{
	std::map<std::string, PointLight *>::iterator it = pointLights.begin();
	while (it != pointLights.end())
	{
		p->configurePointLightBuffer(it->second);
		it++;
	}

	std::map<std::string, SpotLight *>::iterator sIt = spotLights.begin();
	while (sIt != spotLights.end())
	{
		p->configureSpotLightBuffer(sIt->second);
		sIt++;
	}

	std::map<std::string, DirectionalLight *>::iterator dIt = directionalLights.begin();
	while (dIt != directionalLights.end())
	{
		p->configureDirectionalLightBuffer(dIt->second);
		dIt++;
	}
}

void Scene::addPointLight(PointLight * pl)
{
	pointLights[pl->getName()] = pl;

	triggerLightUpdate(pl);
}

void Scene::addSpotLight(SpotLight * sl)
{
	spotLights[sl->getName()] = sl;

	std::map<std::string, ProgramRenderables *>::iterator it = renders.begin();
	while (it != renders.end())
	{
		it->second->program->configureSpotLightBuffer(sl);
		it++;
	}
}

void Scene::addDirectionalLight(DirectionalLight * dl)
{
	directionalLights[dl->getName()] = dl;

	std::map<std::string, ProgramRenderables *>::iterator it = renders.begin();
	while (it != renders.end())
	{
		it->second->program->configureDirectionalLightBuffer(dl);
		it++;
	}
}

void Scene::triggerLightUpdate(PointLight *pl)
{
	std::map<std::string, ProgramRenderables *>::iterator it = renders.begin();
	while (it != renders.end())
	{
		it->second->program->configurePointLightBuffer(pl);
		it++;
	}

	std::list<Program *>::iterator it2 = lightDependentPrograms.begin();
	while (it2 != lightDependentPrograms.end())
	{
		(*it2)->configurePointLightBuffer(pl);
		it2++;
	}
}

PointLight * Scene::getLightByName(std::string name)
{
	std::map<std::string, PointLight *>::iterator it = pointLights.find(name);
	if (it != pointLights.end())
	{
		return it->second;
	}

	return NULL;
}

SpotLight * Scene::getSpotLightByName(std::string name)
{
	std::map<std::string, SpotLight *>::iterator it = spotLights.find(name);
	if (it != spotLights.end())
	{
		return it->second;
	}

	return NULL;
}

DirectionalLight * Scene::getDirectionalLightByName(std::string name)
{
	std::map<std::string, DirectionalLight *>::iterator it = directionalLights.find(name);
	if (it != directionalLights.end())
	{
		return it->second;
	}

	return NULL;
}

void Scene::setCamera(Camera * cam)
{
	camera = cam;
}

Camera * Scene::getCamera()
{
	return camera;
}

const std::map<std::string, ProgramRenderables *> & Scene::getObjects() const
{
	return renders;
}

const std::map<std::string, PointLight *> & Scene::getPointLights() const
{
	return pointLights;
}

const std::map<std::string, SpotLight *> & Scene::getSpotLights() const
{
	return spotLights;
}

const std::map<std::string, DirectionalLight *> Scene::getDirectionalLight() const
{
	return directionalLights;
}

void Scene::onViewportResize(int width, int height)
{
	camera->onWindowResize(width, height);
}

KeyboardHandlersTable * Scene::getKeyboardHandler() const
{
	return keyboardHandlers;
}

MouseEventManager * Scene::getMouseHandler() const
{
	return mouseHandlers;
}

// ===========================================================================================

ProgramRenderables::ProgramRenderables(Program * prog)
{
	program = prog;
}

// ===========================================================================================

SceneManager * SceneManager::INSTANCE = new SceneManager();

SceneManager & SceneManager::getInstance()
{
	return *INSTANCE;
}

SceneManager::SceneManager()
{
	activeScene = 0;
}

SceneManager::~SceneManager()
{
}

bool SceneManager::registerScene(std::string name, Scene * s)
{
	std::map<std::string, Scene *>::iterator it = scenes.find(name);
	if (it != scenes.end())
	{
		return false;
	}

	scenes[name] = s;
	return true;
}

Scene * SceneManager::getScene(std::string name)
{
	std::map<std::string, Scene *>::iterator it = scenes.find(name);
	if (it != scenes.end())
	{
		return NULL;
	}

	return scenes[name];
}

void SceneManager::activateScene(std::string name)
{
	std::map<std::string, Scene *>::iterator it = scenes.find(name);
	if (it != scenes.end())
	{
		activeScene = it->second;
	}
}

Scene * SceneManager::getActiveScene()
{
	return activeScene;
}