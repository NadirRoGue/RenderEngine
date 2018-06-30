/**
* Class used to create an OpenGL context and window manipulation
* using GLFW library
*/
#pragma once

#include "WindowToolkit.h"
#include <memory>

namespace Engine
{
	namespace Window
	{
		/**
		 * Window driver interface manager. Holds, give access, and manipulate
		 * all available and registered window toolkits
		 */
		class WindowManager
		{
		private:
			static WindowManager * INSTANCE;
		private:
			// Currently used window toolkit
			std::unique_ptr<WindowToolkit> currentToolkit;
		public:
			static WindowManager & getInstance();
		private:
			WindowManager();
		public:
			~WindowManager();
			
			// Setter/getter
			void setToolkit(std::unique_ptr<WindowToolkit> toolkit);
			WindowToolkit * getWindowToolkit();
		};
	}
}