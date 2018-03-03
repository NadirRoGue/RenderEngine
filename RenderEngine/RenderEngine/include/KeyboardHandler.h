/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <list>

namespace Engine
{
	// =========================================================================

	class KeyboardHandler
	{
	protected:
		std::list<unsigned char> usedKeys;
	public:
		KeyboardHandler();
		const std::list<unsigned char> & getUsedKeys() const;
		virtual void onKeyPressed(unsigned char key, int x, int y);
	};

	// =========================================================================
	// =========================================================================

	class KeyboardHandlersTable
	{
	private:
		KeyboardHandler *handlers[256];
	
	public:
		KeyboardHandlersTable();
		
		~KeyboardHandlersTable();

		void registerHandler(KeyboardHandler *handler);
		void handleKeyPress(unsigned char key, int x, int y);

		void destroy();
	};
}