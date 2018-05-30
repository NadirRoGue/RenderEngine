#include "inputhandlers/keyboardhandlers/ToggleUIHandler.h"

#include "WorldConfig.h"

Engine::ToggleUIHandler::ToggleUIHandler()
{
	usedKeys = { 'u' };
}

void Engine::ToggleUIHandler::onKeyPressed(unsigned char key, int x, int y, int mode)
{
	if (mode == Engine::KeyboardHandler::MODE_PRESS)
	{
		Engine::Settings::showUI = !Engine::Settings::showUI;
	}
}