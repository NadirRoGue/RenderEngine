/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "KeyboardHandler.h"

#include <iostream>

using namespace Engine;

KeyboardHandlersTable::KeyboardHandlersTable()
{
	for (int i = 0; i < 256; i++)
		handlers[i] = 0;
}

KeyboardHandlersTable::~KeyboardHandlersTable()
{
	destroy();
}

void KeyboardHandlersTable::registerHandler(KeyboardHandler *handler)
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

void KeyboardHandlersTable::handleKeyPress(unsigned char key, int x, int y)
{
	key = tolower(key);

	KeyboardHandler * handler = handlers[key];
	
	if (handler != 0)
	{
		handler->onKeyPressed(key, x, y);
	}
}

void KeyboardHandlersTable::destroy()
{
	for (int i = 0; i < 256; i++)
	{
		delete handlers[i];
	}
}

// ======================================================================================

KeyboardHandler::KeyboardHandler()
{
}

const std::list<unsigned char> & KeyboardHandler::getUsedKeys() const
{
	return usedKeys;
}

void KeyboardHandler::onKeyPressed(unsigned char key, int x, int y)
{
}