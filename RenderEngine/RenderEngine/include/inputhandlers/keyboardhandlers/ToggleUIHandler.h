/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "KeyboardHandler.h"

namespace Engine
{
	/**
	 * Class in charge to toggle visible the user interface (by default 'u' key)
	 */
	class ToggleUIHandler : public KeyboardHandler
	{
	public:
		ToggleUIHandler();
		void onKeyPressed(unsigned char key, int x, int y, int mode);
	};
}