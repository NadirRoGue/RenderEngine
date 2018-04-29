#pragma once

#include "KeyboardHandler.h"

namespace Engine
{
	class ToggleUIHandler : public KeyboardHandler
	{
	public:
		ToggleUIHandler();
		void onKeyPressed(unsigned char key, int x, int y, int mode);
	};
}