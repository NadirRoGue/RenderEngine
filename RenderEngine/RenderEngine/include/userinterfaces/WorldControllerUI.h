#pragma once

#include "UserInterface.h"

#include "GLFW/glfw3.h"

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