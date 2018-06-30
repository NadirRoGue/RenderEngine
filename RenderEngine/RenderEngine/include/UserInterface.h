/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <glm/glm.hpp>

namespace Engine
{
	namespace Window
	{
		// UI Styles based on ImGUI default styles
		enum UIStyle
		{
			CLASSIC,
			DARK,
			LIGHT
		};

		// Parent class of all the user inferfaces which might be registered
		// to the engine's window toolkits
		class UserInterface
		{
		private:
			unsigned int g_ShaderHandle;
			unsigned int g_VertHandle;
			unsigned int g_FragHandle;
			
			unsigned int g_AttribLocationTex;
			unsigned int g_AttribLocationProjMtx;
			unsigned int g_AttribLocationPosition;
			unsigned int g_AttribLocationUV;
			unsigned int g_AttribLocationColor;

			unsigned int g_VaoHandle;
			unsigned int g_VboHandle;
			unsigned int g_ElementsHandle;

			unsigned int g_FontTexture;

			bool g_MousePressed[3];

			GLFWwindow * surface;

		protected:
			std::string font;
			UIStyle uistyle;
		public:
			UserInterface(GLFWwindow * surf);
			~UserInterface();
			void initialize();
			void render(double deltaTime);
			virtual void drawGraphics() = 0;
			void release();

			void updateMouseButtonPressed(int button, bool pressed);

			void setFont(std::string name);
			void setUIStyle(UIStyle style);
			glm::ivec2 getWindowSize();

			std::string getFont();
		private:
			void initializeGraphics();
			void createFontsTexture();
			void updateGraphics();
		};
	}
}