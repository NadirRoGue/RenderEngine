/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "MouseHandler.h"

using namespace Engine;

MouseEventManager::MouseEventManager()
{
	lastButtonPressed = -1;

	activeMouseHandler[0] = activeMouseHandler[1] = activeMouseHandler[2] = nullptr;
	activeMotionHandler[0] = activeMotionHandler[1] = activeMotionHandler[2] = nullptr;
}

MouseEventManager::~MouseEventManager()
{

}

void MouseEventManager::registerMouseHandler(MouseHandler * handler)
{
	std::map <std::string, MouseHandler *>::iterator it = mouseHandlers.find(handler->getName());
	if (it != mouseHandlers.end())
	{
		delete mouseHandlers[handler->getName()];
	}

	mouseHandlers[handler->getName()] = handler;

	activateMouseHandler(handler);
}

void MouseEventManager::registerMouseMotionHandler(MouseMotionHandler * handler)
{
	std::map <std::string, MouseMotionHandler *>::iterator it = motionHandlers.find(handler->getName());
	if (it != motionHandlers.end())
	{
		delete mouseHandlers[handler->getName()];
	}

	motionHandlers[handler->getName()] = handler;

	activateMouseMotionHandler(handler);
}

void MouseEventManager::activateMouseHandler(const std::string & name)
{
	std::map<std::string, MouseHandler *>::iterator it = mouseHandlers.find(name);
	if (it != mouseHandlers.end())
	{
		activateMouseHandler(it->second);
	}
}

void MouseEventManager::activateMouseHandler(MouseHandler * handler)
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

void MouseEventManager::activateMouseMotionHandler(const std::string & name)
{
	std::map<std::string, MouseMotionHandler *>::iterator it = motionHandlers.find(name);
	if (it != motionHandlers.end())
	{
		activateMouseMotionHandler(it->second);
	}
}

void MouseEventManager::activateMouseMotionHandler(MouseMotionHandler * handler)
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

void MouseEventManager::handleMouseClick(int button, int state, int x, int y)
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

void MouseEventManager::handleMouseMotion(int x, int y)
{
	if (lastButtonPressed > -1 && activeMotionHandler[lastButtonPressed] != nullptr)
	{
		activeMotionHandler[lastButtonPressed]->handleMotion(x, y);
	}
}

// =========================================================================

MouseHandler::MouseHandler(std::string nam)
{
	name = std::string(nam);
}

MouseHandler::~MouseHandler()
{
}

const std::string & MouseHandler::getName() const
{
	return name;
}

const std::list<int> & MouseHandler::getUsedButtons() const
{
	return usedButtons;
}

void MouseHandler::handleMouseClick(int button, int state, int x, int y)
{
}

// =========================================================================

MouseMotionHandler::MouseMotionHandler(std::string nam)
{
	name = std::string(nam);
}

MouseMotionHandler::~MouseMotionHandler()
{
}

const std::string & MouseMotionHandler::getName() const
{
	return name;
}

const std::list<int> & MouseMotionHandler::getUsedButtons() const
{
	return usedButtons;
}

void MouseMotionHandler::handleMotion(int x, int y)
{
}

void MouseMotionHandler::finishMotion()
{

}