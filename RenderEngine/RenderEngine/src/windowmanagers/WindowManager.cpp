#include "windowmanagers/WindowManager.h"

#include <memory>

Engine::Window::WindowManager * Engine::Window::WindowManager::INSTANCE = new Engine::Window::WindowManager();

Engine::Window::WindowManager & Engine::Window::WindowManager::getInstance()
{
	return *Engine::Window::WindowManager::INSTANCE;
}

Engine::Window::WindowManager::WindowManager()
{
}

Engine::Window::WindowManager::~WindowManager()
{
	Engine::Window::WindowToolkit * cleanUp = currentToolkit.release();
	if (cleanUp != nullptr)
	{
		delete cleanUp;
	}
}

Engine::Window::WindowToolkit * Engine::Window::WindowManager::getWindowToolkit()
{
	return currentToolkit.get();
}

void Engine::Window::WindowManager::setToolkit(std::unique_ptr<Engine::Window::WindowToolkit> win)
{
	Engine::Window::WindowToolkit * cleanUp = currentToolkit.release();
	if (cleanUp != nullptr)
	{
		delete cleanUp;
	}

	currentToolkit = std::move(win);
	currentToolkit->initializeContext();
	currentToolkit->initializeOGL();
}