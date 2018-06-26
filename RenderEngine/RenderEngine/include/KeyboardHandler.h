/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <list>

namespace Engine
{
	// =========================================================================
	// Parent class of every class that will handle user input from the keyboard
	class KeyboardHandler
	{
	public:
		static const int MODE_RELEASE;
		static const int MODE_PRESS;
		static const int MODE_REPEAT;
	protected:
		// Keys to which the handler should be linked must be stored in this
		// list on the handler constructor
		std::list<unsigned char> usedKeys;
	public:
		KeyboardHandler();
		const std::list<unsigned char> & getUsedKeys() const;
		virtual void onKeyPressed(unsigned char key, int x, int y, int mode);
	};

	// =========================================================================
	// =========================================================================
	// Holds, updates, and manages in general all keyboard handlers
	class KeyboardHandlersTable
	{
	private:
		KeyboardHandler *handlers[256];
	
	public:
		KeyboardHandlersTable();
		
		~KeyboardHandlersTable();

		void registerHandler(KeyboardHandler *handler);
		void handleKeyPress(unsigned char key, int x, int y, int mode);

		void destroy();
	};
}