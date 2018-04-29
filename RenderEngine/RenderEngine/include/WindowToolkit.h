#pragma once

#include <string>
#include <GL/glew.h>

#include <list>

#include "UserInterface.h"

namespace Engine
{
	namespace Window
	{
		class WindowToolkit
		{
		protected:
			std::list<UserInterface *> userInterfaces;

			unsigned int contextProfile;
			unsigned int oglMajorV, oglMinorV;
			unsigned int windowPosX, windowPosY;
			unsigned int windowWidth, windowHeight;
			std::string windowTitle;

			double lastFrameTime;
			double deltaTime;

		public:
			WindowToolkit(std::string title, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
			~WindowToolkit();
			void setOGLVersion(unsigned int major, unsigned int minor);
			void initializeOGL();
			void setContextProfile(unsigned int contxtProfile);

			void addUserInterface(UserInterface * ui);
			void updateUI();

			double getDeltaTime();

			virtual void initializeContext() = 0;
			virtual void mainLoop() = 0;

			// Optional
			virtual void onMouseClick(int button, int state) {}
			virtual void onMouseMove(int x, int y) {}
			virtual void onKeyboardInput(unsigned char key) { }
			virtual void onResize(int width, int height) { }
			virtual void onRenderStart() { }
			virtual void onRenderEnd() { }
		protected:
			void initGlew();
		};
	}
}