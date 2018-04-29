#pragma once

#include "UserInterface.h"

namespace Engine
{
	namespace Window
	{
		class WorldControllerUI : public UserInterface
		{
		public:
			WorldControllerUI(GLFWwindow * surface);
			void drawGraphics();
		};
	}
}