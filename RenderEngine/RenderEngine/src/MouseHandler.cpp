/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "MouseHandler.h"

Engine::MouseEventManager::MouseEventManager()
{
	lastButtonPressed = -1;

	activeMouseHandler[0] = activeMouseHandler[1] = activeMouseHandler[2] = nullptr;
	activeMotionHandler[0] = activeMotionHandler[1] = activeMotionHandler[2] = nullptr;
}

Engine::MouseEventManager::~MouseEventManager()
{

}

void Engine::MouseEventManager::registerMouseHandler(Engine::MouseHandler * handler)
{
	std::map <std::string, Engine::MouseHandler *>::iterator it = mouseHandlers.find(handler->getName());
	if (it != mouseHandlers.end())
	{
		delete mouseHandlers[handler->getName()];
	}

	mouseHandlers[handler->getName()] = handler;

	activateMouseHandler(handler);
}

void Engine::MouseEventManager::registerMouseMotionHandler(Engine::MouseMotionHandler * handler)
{
	std::map <std::string, Engine::MouseMotionHandler *>::iterator it = motionHandlers.find(handler->getName());
	if (it != motionHandlers.end())
	{
		delete mouseHandlers[handler->getName()];
	}

	motionHandlers[handler->getName()] = handler;

	activateMouseMotionHandler(handler);
}

void Engine::MouseEventManager::activateMouseHandler(const std::string & name)
{
	std::map<std::string, Engine::MouseHandler *>::iterator it = mouseHandlers.find(name);
	if (it != mouseHandlers.end())
	{
		activateMouseHandler(it->second);
	}
}

void Engine::MouseEventManager::activateMouseHandler(Engine::MouseHandler * handler)
{
	if (handler == nullptr)
	{
		return;
	}

	std::list<int> usedButtons = handler->getUsedButtons();
	std::list<int>::iterator buttonIterator = usedButtons.begin();

	while (buttonIterator != usedButtons.end())
	{
		int button = *buttonIterator;

		if (button > -1 && button < 3)
		{
			activeMouseHandler[button] = handler;
		}

		buttonIterator++;
	}
}

void Engine::MouseEventManager::activateMouseMotionHandler(const std::string & name)
{
	std::map<std::string, Engine::MouseMotionHandler *>::iterator it = motionHandlers.find(name);
	if (it != motionHandlers.end())
	{
		activateMouseMotionHandler(it->second);
	}
}

void Engine::MouseEventManager::activateMouseMotionHandler(Engine::MouseMotionHandler * handler)
{
	if (handler == nullptr)
	{
		return;
	}

	std::list<int> usedButtons = handler->getUsedButtons();
	std::list<int>::iterator buttonIterator = usedButtons.begin();

	while (buttonIterator != usedButtons.end())
	{
		int button = *buttonIterator;

		if (button > -1 && button < 3)
		{
			activeMotionHandler[button] = handler;
		}

		buttonIterator++;
	}
}

void Engine::MouseEventManager::handleMouseClick(int button, int state, int x, int y)
{
	if (button < 0 || button > 2)
		return;

	if (state == 0)
	{
		lastButtonPressed = button;
		if (activeMouseHandler[button] != nullptr)
		{
			activeMouseHandler[button]->handleMouseClick(button, state, x, y);
		}
	}
	else
	{
		if (activeMotionHandler[button] != nullptr)
		{
			activeMotionHandler[button]->finishMotion();
		}
		lastButtonPressed = -1;
	}
}

void Engine::MouseEventManager::handleMouseMotion(int x, int y)
{
	if (lastButtonPressed > -1 && activeMotionHandler[lastButtonPressed] != nullptr)
	{
		activeMotionHandler[lastButtonPressed]->handleMotion(x, y);
	}
}

// =========================================================================

Engine::MouseHandler::MouseHandler(std::string nam)
{
	name = std::string(nam);
}

Engine::MouseHandler::~MouseHandler()
{
}

const std::string & Engine::MouseHandler::getName() const
{
	return name;
}

const std::list<int> & Engine::MouseHandler::getUsedButtons() const
{
	return usedButtons;
}

void Engine::MouseHandler::handleMouseClick(int button, int state, int x, int y)
{
}

// =========================================================================

Engine::MouseMotionHandler::MouseMotionHandler(std::string nam)
{
	name = std::string(nam);
}

Engine::MouseMotionHandler::~MouseMotionHandler()
{
}

const std::string & Engine::MouseMotionHandler::getName() const
{
	return name;
}

const std::list<int> & Engine::MouseMotionHandler::getUsedButtons() const
{
	return usedButtons;
}

void Engine::MouseMotionHandler::handleMotion(int x, int y)
{
}

void Engine::MouseMotionHandler::finishMotion()
{

}