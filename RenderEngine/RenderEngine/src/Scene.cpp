/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "Scene.h"
#include <gl/glew.h>

#include <iostream>

#include "datatables/ProgramTable.h"

Engine::Scene::Scene()
{
	clearColor = glm::vec3(0, 0, 0);

	keyboardHandlers = new Engine::KeyboardHandlersTable();
	mouseHandlers = new Engine::MouseEventManager();
	animations = new Engine::AnimationTable();
	directionalLight = NULL;
	terrain = NULL;
	skybox = NULL;
}

Engine::Scene::~Scene()
{
	if (keyboardHandlers != NULL)
	{
		delete keyboardHandlers;
	}

	if (mouseHandlers != NULL)
	{
		delete mouseHandlers;
	}

	if (animations != NULL)
	{
		delete animations;
	}

	if (terrain != NULL)
	{
		delete terrain;
	}

	if (skybox != NULL)
	{
		delete skybox;
	}
}

void Engine::Scene::setClearColor(glm::vec3 cc)
{
	clearColor = cc;
	std::map<std::string, Engine::ProgramRenderables *>::const_iterator renderableIt;
	for (renderableIt = renders.cbegin(); renderableIt != renders.cend(); renderableIt++)
	{
		renderableIt->second->program->configureClearColor(clearColor);
	}
}

const glm::vec3 & Engine::Scene::getClearColor() const
{
	return clearColor;
}

void Engine::Scene::setTerrain(Terrain * terrain)
{
	this->terrain = terrain;
}

Engine::Terrain * Engine::Scene::getTerrain()
{
	return this->terrain;
}

void Engine::Scene::setSkybox(Engine::SkyBox * sb)
{
	skybox = sb;
}

Engine::SkyBox * Engine::Scene::getSkyBox()
{
	return skybox;
}

void Engine::Scene::addObject(Engine::Object *obj)
{
	std::string material = "";// obj->getMeshInstance()->getMaterial();
	unsigned int vaoIndex = obj->getMesh()->vao;

	std::map<std::string, Engine::ProgramRenderables *>::iterator renderIt = renders.find(material);

	if (renderIt == renders.end())
	{
		Program * prog = Engine::ProgramTable::getInstance().getProgramByName(material);

		if(prog == nullptr)
		{
			return;
		}

		configureNewProgramLights(prog);

		Engine::ProgramRenderables * renderable = new Engine::ProgramRenderables(prog);
		renderable->objects[vaoIndex].push_back(obj);
		renders[material] = renderable;
	}
	else
	{
		renderIt->second->objects[vaoIndex].push_back(obj);
	}
}

void Engine::Scene::addLightDependentProgram(Engine::Program * prog)
{
	lightDependentPrograms.push_back(prog);
}

void Engine::Scene::configureNewProgramLights(Engine::Program * p)
{
	std::map<std::string, Engine::PointLight *>::iterator it = pointLights.begin();
	while (it != pointLights.end())
	{
		p->configurePointLightBuffer(it->second);
		it++;
	}

	std::map<std::string, Engine::SpotLight *>::iterator sIt = spotLights.begin();
	while (sIt != spotLights.end())
	{
		p->configureSpotLightBuffer(sIt->second);
		sIt++;
	}

	p->configureDirectionalLightBuffer(directionalLight);
}

void Engine::Scene::addPointLight(Engine::PointLight * pl)
{
	pointLights[pl->getName()] = pl;

	triggerLightUpdate(pl);
}

void Engine::Scene::addSpotLight(Engine::SpotLight * sl)
{
	spotLights[sl->getName()] = sl;

	std::map<std::string, Engine::ProgramRenderables *>::iterator it = renders.begin();
	while (it != renders.end())
	{
		it->second->program->configureSpotLightBuffer(sl);
		it++;
	}
}

void Engine::Scene::setDirectionalLight(Engine::DirectionalLight * dl)
{
	if (directionalLight != NULL)
	{
		delete directionalLight;
	}

	directionalLight = dl;
}

void Engine::Scene::triggerLightUpdate(PointLight *pl)
{
	std::map<std::string, Engine::ProgramRenderables *>::iterator it = renders.begin();
	while (it != renders.end())
	{
		it->second->program->configurePointLightBuffer(pl);
		it++;
	}

	std::list<Engine::Program *>::iterator it2 = lightDependentPrograms.begin();
	while (it2 != lightDependentPrograms.end())
	{
		(*it2)->configurePointLightBuffer(pl);
		it2++;
	}
}

Engine::PointLight * Engine::Scene::getLightByName(std::string name)
{
	std::map<std::string, Engine::PointLight *>::iterator it = pointLights.find(name);
	if (it != pointLights.end())
	{
		return it->second;
	}

	return NULL;
}

Engine::SpotLight * Engine::Scene::getSpotLightByName(std::string name)
{
	std::map<std::string, Engine::SpotLight *>::iterator it = spotLights.find(name);
	if (it != spotLights.end())
	{
		return it->second;
	}

	return NULL;
}

Engine::DirectionalLight * Engine::Scene::getDirectionalLight()
{
	return directionalLight;
}

void Engine::Scene::setCamera(Engine::Camera * cam)
{
	camera = cam;
}

Engine::Camera * Engine::Scene::getCamera()
{
	return camera;
}

const std::map<std::string, Engine::ProgramRenderables *> & Engine::Scene::getObjects() const
{
	return renders;
}

const std::map<std::string, Engine::PointLight *> & Engine::Scene::getPointLights() const
{
	return pointLights;
}

const std::map<std::string, Engine::SpotLight *> & Engine::Scene::getSpotLights() const
{
	return spotLights;
}

void Engine::Scene::onViewportResize(int width, int height)
{
	camera->onWindowResize(width, height);
}

Engine::KeyboardHandlersTable * Engine::Scene::getKeyboardHandler() const
{
	return keyboardHandlers;
}

Engine::MouseEventManager * Engine::Scene::getMouseHandler() const
{
	return mouseHandlers;
}

Engine::AnimationTable * Engine::Scene::getAnimationHandler() const
{
	return animations;
}

// ===========================================================================================

Engine::ProgramRenderables::ProgramRenderables(Program * prog)
{
	program = prog;
}

// ===========================================================================================

Engine::SceneManager * Engine::SceneManager::INSTANCE = new Engine::SceneManager();

Engine::SceneManager & Engine::SceneManager::getInstance()
{
	return *INSTANCE;
}

Engine::SceneManager::SceneManager()
{
	activeScene = 0;
}

Engine::SceneManager::~SceneManager()
{
}

bool Engine::SceneManager::registerScene(std::string name, Engine::Scene * s)
{
	std::map<std::string, Scene *>::iterator it = scenes.find(name);
	if (it != scenes.end())
	{
		return false;
	}

	scenes[name] = s;
	return true;
}

Engine::Scene * Engine::SceneManager::getScene(std::string name)
{
	std::map<std::string, Engine::Scene *>::iterator it = scenes.find(name);
	if (it != scenes.end())
	{
		return NULL;
	}

	return scenes[name];
}

void Engine::SceneManager::activateScene(std::string name)
{
	std::map<std::string, Engine::Scene *>::iterator it = scenes.find(name);
	if (it != scenes.end())
	{
		activeScene = it->second;
	}
}

Engine::Scene * Engine::SceneManager::getActiveScene()
{
	return activeScene;
}