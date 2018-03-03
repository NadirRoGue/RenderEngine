/*
* @author Nadir Rom�n Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "KeyboardHandler.h"

#include <iostream>

Engine::KeyboardHandlersTable::KeyboardHandlersTable()
{
	for (int i = 0; i < 256; i++)
		handlers[i] = 0;
}

Engine::KeyboardHandlersTable::~KeyboardHandlersTable()
{
	destroy();
}

void Engine::KeyboardHandlersTable::registerHandler(Engine::KeyboardHandler *handler)
{
	if (handler == nullptr)
		return;

	std::list<unsigned char> keys = handler->getUsedKeys();
	std::list<unsigned char>::const_iterator cIt = keys.cbegin();
	while (cIt != keys.cend())
	{
		handlers[*cIt] = handler;
		cIt++;
	}
}

void Engine::KeyboardHandlersTable::handleKeyPress(unsigned char key, int x, int y)
{
	key = tolower(key);

	Engine::KeyboardHandler * handler = handlers[key];
	
	if (handler != 0)
	{
		handler->onKeyPressed(key, x, y);
	}
}

void Engine::KeyboardHandlersTable::destroy()
{
	for (int i = 0; i < 256; i++)
	{
		delete handlers[i];
	}
}

// ======================================================================================

Engine::KeyboardHandler::KeyboardHandler()
{
}

const std::list<unsigned char> & Engine::KeyboardHandler::getUsedKeys() const
{
	return usedKeys;
}

void Engine::KeyboardHandler::onKeyPressed(unsigned char key, int x, int y)
{
}