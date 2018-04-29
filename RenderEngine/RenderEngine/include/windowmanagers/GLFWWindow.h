#pragma once

#include "WindowToolkit.h"

#include <GLFW/glfw3.h>

#include <string>

namespace Engine
{
	namespace Window
	{
		void defaultGLFWKeyboardCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
		void defaultGLFWMouseInputCallback(GLFWwindow * window, int button, int action, int mods);
		void defaultGLFWMouseMovementCallback(GLFWwindow * window, double xpos, double ypos);
		void defaultGLFWResizeCallback(GLFWwindow * window, int width, int height);

		typedef void(*glfwKeyboardCallback)(GLFWwindow*, int, int, int, int);
		typedef void(*glfwMouseInputCallback)(GLFWwindow*, int, int, int);
		typedef void(*glfwMouseMovementCallback)(GLFWwindow*, double, double);
		typedef void(*glfwResizeCallback)(GLFWwindow*, int, int);

		class GLFWWindow : public WindowToolkit
		{
		private:
			GLFWwindow * window;

			glfwKeyboardCallback kbCb;
			glfwMouseInputCallback mCb;
			glfwMouseMovementCallback mmCb;
			glfwResizeCallback resizeCb;

		public:
			GLFWWindow(std::string title, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
			void initializeContext() override;
			void mainLoop() override;

			void setKeyboardCallback(glfwKeyboardCallback keyboardCb);
			void setMouseInputCallback(glfwMouseInputCallback mInputCb);
			void setMouseMovementCallback(glfwMouseMovementCallback mMovementCb);
			void setResizeCallback(glfwResizeCallback resCb);

			void onMouseClick(int button, int state);
		};
	}
}