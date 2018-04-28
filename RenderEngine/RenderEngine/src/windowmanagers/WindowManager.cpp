#include "windowmanagers/WindowManager.h"

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

const Engine::Window::WindowToolkit * Engine::Window::WindowManager::getWindowToolkit() const
{
	return currentToolkit.get();
}

void Engine::Window::WindowManager::initialize()
{

}