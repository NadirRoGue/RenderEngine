#pragma once 

#include "WindowToolkit.h"

namespace Engine
{
	namespace Window
	{
		typedef void(*resizeCallback)(int, int);
		typedef void(*keyboardCallback)(unsigned char, int, int);
		typedef void(*mouseCallback)(int, int, int, int);
		typedef void(*mouseMotionCallback)(int, int);
		typedef void(*genericCallback)();

		void defaultResizeCallback(int width, int height);
		void defaultKeyboardInputCallback(unsigned char key, int x, int y);
		void defaultMouseInputCallback(int button, int state, int x, int y);
		void defaultMouseMovementCallback(int x, int y);
		void defaultRenderLoopCallback();
		void defaultIdleCallback();

		class GLUTWindow : public WindowToolkit
		{
		private:
			unsigned int displayFlags;

			resizeCallback rszCb;
			keyboardCallback kbCb;
			mouseCallback mCb;
			mouseMotionCallback mmCb;
			genericCallback renderCb;
			genericCallback idleCb;
			
			double renderStartTime;
		public:
			GLUTWindow(std::string title, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
			//void initializeOGL();
			void initializeContext();

			void setDisplayFlags(unsigned int dispFlags);

			void mainLoop();

			void registerResizeCallback(resizeCallback funcPtr = NULL);
			void registerKeyboardInputCallback(keyboardCallback funcPtr = NULL);
			void registerMouseInputCallback(mouseCallback funcPtr = NULL);
			void registerMouseMovementCallback(mouseMotionCallback funcPtr = NULL);
			void registerRenderLoopCallback(genericCallback funcPtr = NULL);
			void registerIdleCallback(genericCallback funcPtr = NULL);

			void onRenderEnd();
		};
	}
}