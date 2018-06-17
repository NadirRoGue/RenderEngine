/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include "UserInterface.h"

namespace Engine
{
	namespace Window
	{
		/**
		 * User interface class to display the world controller menu ('u' key during execution)
		 */
		class WorldControllerUI : public UserInterface
		{
		public:
			WorldControllerUI(GLFWwindow * surface);
			void drawGraphics();
		};
	}
}