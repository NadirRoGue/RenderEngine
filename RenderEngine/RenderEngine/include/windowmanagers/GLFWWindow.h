/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include "WindowToolkit.h"

#include <GLFW/glfw3.h>

#include <string>

namespace Engine
{
	namespace Window
	{
		/**
		 * GLFW Driver interface implementation
		 */

		// Default keyboard callback function
		void defaultGLFWKeyboardCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
		// Default mouse input callback function
		void defaultGLFWMouseInputCallback(GLFWwindow * window, int button, int action, int mods);
		// Default mouse movement callback function
		void defaultGLFWMouseMovementCallback(GLFWwindow * window, double xpos, double ypos);
		// Defautl window resize callback function
		void defaultGLFWResizeCallback(GLFWwindow * window, int width, int height);

		typedef void(*glfwKeyboardCallback)(GLFWwindow*, int, int, int, int);
		typedef void(*glfwMouseInputCallback)(GLFWwindow*, int, int, int);
		typedef void(*glfwMouseMovementCallback)(GLFWwindow*, double, double);
		typedef void(*glfwResizeCallback)(GLFWwindow*, int, int);

		/**
		 * Class used to create an OpenGL context and window manipulation
		 * using GLFW library
		 */
		class GLFWWindow : public WindowToolkit
		{
		private:
			// Surface pointer
			GLFWwindow * window;

			// Callbacks
			glfwKeyboardCallback kbCb;
			glfwMouseInputCallback mCb;
			glfwMouseMovementCallback mmCb;
			glfwResizeCallback resizeCb;

		public:
			GLFWWindow(std::string title, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
			void initializeContext() override;
			void mainLoop() override;

			// Callback setters
			void setKeyboardCallback(glfwKeyboardCallback keyboardCb);
			void setMouseInputCallback(glfwMouseInputCallback mInputCb);
			void setMouseMovementCallback(glfwMouseMovementCallback mMovementCb);
			void setResizeCallback(glfwResizeCallback resCb);

			void onMouseClick(int button, int state);
		};
	}
}