#pragma once

#include "WindowToolkit.h"
#include <memory>

namespace Engine
{
	namespace Window
	{
		class WindowManager
		{
		private:
			static WindowManager * INSTANCE;
		private:
			std::unique_ptr<WindowToolkit> currentToolkit;
		public:
			static WindowManager & getInstance();
		private:
			WindowManager();
		public:
			~WindowManager();
			
			void setToolkit(std::unique_ptr<WindowToolkit> toolkit);
			WindowToolkit * getWindowToolkit();
		};
	}
}