/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once 

#include "WindowToolkit.h"

namespace Engine
{
	namespace Window
	{
		/**
		 * GLUT driver interface implementation
		 */

		typedef void(*resizeCallback)(int, int);
		typedef void(*keyboardCallback)(unsigned char, int, int);
		typedef void(*mouseCallback)(int, int, int, int);
		typedef void(*mouseMotionCallback)(int, int);
		typedef void(*genericCallback)();

		// Default callbacks
		void defaultResizeCallback(int width, int height);
		void defaultKeyboardInputCallback(unsigned char key, int x, int y);
		void defaultMouseInputCallback(int button, int state, int x, int y);
		void defaultMouseMovementCallback(int x, int y);
		void defaultRenderLoopCallback();
		void defaultIdleCallback();

		/**
		* Class used to create an OpenGL context and window manipulation
		* using GLUT library
		*/
		class GLUTWindow : public WindowToolkit
		{
		private:
			// OpenGL display flags
			unsigned int displayFlags;

			// Callbacks
			resizeCallback rszCb;
			keyboardCallback kbCb;
			mouseCallback mCb;
			mouseMotionCallback mmCb;
			genericCallback renderCb;
			genericCallback idleCb;
			
			// Starting rendering time to compute delta time and passed time
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