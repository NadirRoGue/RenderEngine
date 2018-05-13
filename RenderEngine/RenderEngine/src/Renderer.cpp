/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "Renderer.h"
#include "PostProcessProgram.h"
#include "Scene.h"

#include <iostream>


unsigned int Engine::ScreenManager::SCREEN_HEIGHT = 500;
unsigned int Engine::ScreenManager::SCREEN_WIDTH = 500;

unsigned int Engine::ScreenManager::REAL_SCREEN_HEIGHT = 500;
unsigned int Engine::ScreenManager::REAL_SCREEN_WIDTH = 500;

// ==================================================================

Engine::Renderer::Renderer()
{
	activeCam = 0;
}

Engine::Renderer::~Renderer()
{
}

void Engine::Renderer::initialize()
{
	renderFromCamera(Engine::SceneManager::getInstance().getActiveScene()->getCamera());
}

void Engine::Renderer::renderFromCamera(Engine::Camera * cam)
{
	activeCam = cam;
}

// =========================================================================

Engine::RenderManager * Engine::RenderManager::INSTANCE = new Engine::RenderManager();

Engine::RenderManager & Engine::RenderManager::getInstance()
{
	return *INSTANCE;
}

Engine::RenderManager::RenderManager()
{
	activeRender = 0;
}

Engine::RenderManager::~RenderManager()
{
	if (activeRender != NULL)
	{
		delete activeRender;
	}
}

void Engine::RenderManager::doRender()
{
	activeRender->doRender();
}

void Engine::RenderManager::setRenderer(Engine::Renderer * renderer)
{
	if (activeRender != NULL)
	{
		delete activeRender;
	}

	if (Engine::SceneManager::getInstance().getActiveScene() == NULL)
	{
		std::cerr << "RenderManager: No active scene found" << std::endl;
		exit(-1);
	}

	activeRender = renderer;
	activeRender->initialize();
}

Engine::Renderer * Engine::RenderManager::getRenderer()
{
	return activeRender;
}

void Engine::RenderManager::doResize(unsigned int w, unsigned int h)
{
	Engine::ScreenManager::SCREEN_WIDTH = Engine::ScreenManager::REAL_SCREEN_WIDTH = w;
	Engine::ScreenManager::SCREEN_HEIGHT = Engine::ScreenManager::REAL_SCREEN_HEIGHT = h;
	activeRender->onResize(w, h);
}