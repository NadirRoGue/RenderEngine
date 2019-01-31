#include "windowmanagers/GLFWWindow.h"

#include <iostream>

#include "imgui/imgui.h"

#include "Renderer.h"
#include "Scene.h"

#include "userinterfaces/WorldControllerUI.h"
#include "WorldConfig.h"
#include "TimeAccesor.h"

double lastMouseXPos = 0.0, lastMouseYPos = 0.0;

void Engine::Window::defaultGLFWKeyboardCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	Engine::KeyboardHandlersTable * table = Engine::SceneManager::getInstance().getActiveScene()->getKeyboardHandler();
	if (table != NULL)
	{
		int x = int(floor(lastMouseXPos));
		int y = int(floor(lastMouseYPos));
		table->handleKeyPress(key, x, y, action);
	}

	ImGuiIO& io = ImGui::GetIO();
	if (action == GLFW_PRESS)
	{
		io.KeysDown[key] = true;
		io.AddInputCharacter(unsigned short(key));
	}
	if (action == GLFW_RELEASE)
		io.KeysDown[key] = false;
}

void Engine::Window::defaultGLFWMouseInputCallback(GLFWwindow * window, int button, int action, int mods)
{
	// Left button = 0
	// Pressed = 0
	// Released = 0
	Engine::MouseEventManager * manager = Engine::SceneManager::getInstance().getActiveScene()->getMouseHandler();
	if (manager != NULL)
	{
		int state = action == 1 ? 0 : 1;
		int x = int(floor(lastMouseXPos));
		int y = int(floor(lastMouseYPos));
		manager->handleMouseClick(button, state, x, y);
	}
}

void Engine::Window::defaultGLFWMouseMovementCallback(GLFWwindow * window, double xpos, double ypos)
{
	lastMouseXPos = xpos;
	lastMouseYPos = ypos;

	Engine::MouseEventManager * manager = Engine::SceneManager::getInstance().getActiveScene()->getMouseHandler();
	if (manager != NULL)
	{
		int x = int(floor(lastMouseXPos));
		int y = int(floor(lastMouseYPos));
		manager->handleMouseMotion(x, y);
	}
}

void Engine::Window::defaultGLFWResizeCallback(GLFWwindow * window, int width, int height)
{
	Engine::SceneManager::getInstance().getActiveScene()->onViewportResize(width, height);
	Engine::RenderManager::getInstance().doResize(width, height);
	glViewport(0, 0, width, height);
}

// ======================================================================================

Engine::Window::GLFWWindow::GLFWWindow(std::string title, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	:Engine::Window::WindowToolkit(title, x, y, width, height)
{
	kbCb = Engine::Window::defaultGLFWKeyboardCallback;
	mCb = Engine::Window::defaultGLFWMouseInputCallback;
	mmCb = Engine::Window::defaultGLFWMouseMovementCallback;
	resizeCb = Engine::Window::defaultGLFWResizeCallback;
}

void Engine::Window::GLFWWindow::initializeContext()
{
	if (!glfwInit())
	{
		std::cerr << "GLFW: Couldnt initialize" << std::endl;
		exit(-1);
	}

	//glfwWindowHint(GLFW_SAMPLES, 4);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, oglMajorV);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, oglMinorV);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, contextProfile); //GLFW_OPENGL_CORE_PROFILE

	window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);
	if (!window)
	{
		std::cerr << "GLFW: Couldnt create window" << std::endl;
		glfwTerminate();
		exit(-1);
	}

	glfwSetKeyCallback(window, kbCb);
	glfwSetCursorPosCallback(window, mmCb);
	glfwSetMouseButtonCallback(window, mCb);
	glfwSetFramebufferSizeCallback(window, resizeCb);

	glfwMakeContextCurrent(window);

	glfwSetWindowPos(window, windowPosX, windowPosY);

	glfwSwapInterval(0);

	initGlew();

	addUserInterface(new Engine::Window::WorldControllerUI(window));
}

void Engine::Window::GLFWWindow::mainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		// Update secondary settings based on main settings changes
		Engine::Settings::update();

		// Render scene
		Engine::RenderManager::getInstance().doRender();

		// Update user interface
		updateUI();

		Engine::RenderableNotifier::getInstance().checkUpdatedConfig();

		// Process inputs
		glfwPollEvents();
		glfwSwapBuffers(window);

		// Update animations
		Engine::SceneManager::getInstance().getActiveScene()->getAnimationHandler()->tick();

		Engine::Time::update(glfwGetTime());
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}

void Engine::Window::GLFWWindow::setKeyboardCallback(Engine::Window::glfwKeyboardCallback keyboardCb)
{
	kbCb = keyboardCb;
}

void Engine::Window::GLFWWindow::setMouseInputCallback(Engine::Window::glfwMouseInputCallback mInputCb)
{
	mCb = mInputCb;
}

void Engine::Window::GLFWWindow::setMouseMovementCallback(Engine::Window::glfwMouseMovementCallback mMovementCb)
{
	mmCb = mMovementCb;
}

void Engine::Window::GLFWWindow::setResizeCallback(Engine::Window::glfwResizeCallback resCb)
{
	resizeCb = resCb;
}

void Engine::Window::GLFWWindow::onMouseClick(int button, int state)
{
	for (auto ui : userInterfaces)
	{
		ui->updateMouseButtonPressed(button, state == GLFW_PRESS);
	}
}
