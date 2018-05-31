#include "windowmanagers/GLUTWindow.h"

#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 
#include <string>
#include <chrono>

#include "Renderer.h"
#include "Scene.h"
#include "windowmanagers/WindowManager.h"
#include "TimeAccesor.h"

void Engine::Window::defaultResizeCallback(int width, int height)
{
	Engine::SceneManager::getInstance().getActiveScene()->onViewportResize(width, height);
	Engine::RenderManager::getInstance().doResize(width, height);
	glViewport(0, 0, width, height);
}

void Engine::Window::defaultKeyboardInputCallback(unsigned char key, int x, int y)
{
	Engine::KeyboardHandlersTable * table = Engine::SceneManager::getInstance().getActiveScene()->getKeyboardHandler();
	if (table != NULL)
	{
		table->handleKeyPress(key, x, y, Engine::KeyboardHandler::MODE_PRESS);
	}

	Engine::Window::WindowManager::getInstance().getWindowToolkit()->onKeyboardInput(key);
}

void Engine::Window::defaultMouseInputCallback(int button, int state, int x, int y)
{
	// Left button = 0
	// Pressed = 0
	// Released = 0
	Engine::MouseEventManager * manager = Engine::SceneManager::getInstance().getActiveScene()->getMouseHandler();
	if (manager != NULL)
	{
		manager->handleMouseClick(button, state, x, y);
	}

	// Opposite than GLFW...
	Engine::Window::WindowManager::getInstance().getWindowToolkit()->onMouseClick(button, state == 0 ? 1 : 0);
}

void Engine::Window::defaultMouseMovementCallback(int x, int y)
{
	// xMin, yMin = 0,0 (top-left corner)
	// xMax, yMax = width,height (bottom-right corner)
	Engine::MouseEventManager * manager = Engine::SceneManager::getInstance().getActiveScene()->getMouseHandler();
	if (manager != NULL)
	{
		manager->handleMouseMotion(x, y);
	}

	Engine::Window::WindowManager::getInstance().getWindowToolkit()->onMouseMove(x, y);
}

void Engine::Window::defaultRenderLoopCallback()
{
	Engine::RenderManager::getInstance().doRender();
	glutSwapBuffers();
}

void Engine::Window::defaultIdleCallback()
{
	Engine::Window::WindowManager::getInstance().getWindowToolkit()->onRenderEnd();
	Engine::SceneManager::getInstance().getActiveScene()->getAnimationHandler()->tick();
	glutPostRedisplay();
}

// ===============================================================================================================================

Engine::Window::GLUTWindow::GLUTWindow(std::string title, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	:Engine::Window::WindowToolkit(title, x, y, width, height)
{
	rszCb = Engine::Window::defaultResizeCallback;
	kbCb = Engine::Window::defaultKeyboardInputCallback;
	mCb = Engine::Window::defaultMouseInputCallback;
	mmCb = Engine::Window::defaultMouseMovementCallback;
	renderCb = Engine::Window::defaultRenderLoopCallback;
	idleCb = Engine::Window::defaultIdleCallback;
}
/*
void Engine::Window::GLUTWindow::initializeOGL()
{

}
*/
void Engine::Window::GLUTWindow::initializeContext()
{
	int zero = 0;
	glutInit(&zero, NULL);
	glutInitContextVersion(oglMajorV, oglMinorV);
	glutInitContextProfile(contextProfile);

	glutInitDisplayMode(displayFlags);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(windowPosX, windowPosY);
	glutCreateWindow(windowTitle.c_str());

	initGlew();

	glutReshapeFunc(rszCb);
	glutDisplayFunc(renderCb);
	glutIdleFunc(idleCb);
	glutKeyboardFunc(kbCb);
	glutMouseFunc(mCb);
	glutMotionFunc(mmCb);
}

void Engine::Window::GLUTWindow::setDisplayFlags(unsigned int dispFlags)
{
	displayFlags = dispFlags;
}

void Engine::Window::GLUTWindow::mainLoop()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
	renderStartTime = double(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());

	glutMainLoop();
}

void Engine::Window::GLUTWindow::registerResizeCallback(resizeCallback funcPtr)
{
	rszCb = funcPtr;
}

void Engine::Window::GLUTWindow::registerKeyboardInputCallback(keyboardCallback funcPtr)
{
	kbCb = funcPtr;
}

void Engine::Window::GLUTWindow::registerMouseInputCallback(mouseCallback funcPtr)
{
	mCb = funcPtr;
}

void Engine::Window::GLUTWindow::registerMouseMovementCallback(mouseMotionCallback funcPtr)
{
	mmCb = funcPtr;
}

void Engine::Window::GLUTWindow::registerRenderLoopCallback(genericCallback funcPtr)
{
	renderCb = funcPtr;
}

void Engine::Window::GLUTWindow::registerIdleCallback(genericCallback funcPtr)
{
	idleCb = funcPtr;
}

void Engine::Window::GLUTWindow::onRenderEnd()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
	double currentAbsTime = double(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());
	//currentAbsTime -= renderStartTime;

	Engine::Time::update(currentAbsTime);
}