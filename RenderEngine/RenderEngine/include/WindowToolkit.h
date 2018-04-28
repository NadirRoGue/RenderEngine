#pragma once

#include <string>

namespace Engine
{
	namespace Window
	{
		class WindowToolkit
		{
		public:
			virtual void initializeOGL() = 0;
			virtual void initializeContext(std::string title, unsigned int width, unsigned int height) = 0;

			virtual void registerResizeCallback(void * funcPtr) = 0;
			virtual void registerKeyboardInputCallback(void * funcPtr) = 0;
			virtual void registerMouseInputCallback(void * funcPtr) = 0;
			virtual void registerMouseMovementCallback(void * funcPtr) = 0;
			virtual void registerRenderLoopCallback(void * funcPtr) = 0;
			virtual void registerIdleCallback(void * funcPtr) = 0;
		};
	}
}