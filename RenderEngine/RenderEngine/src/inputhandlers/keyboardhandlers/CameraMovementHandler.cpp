#include "inputhandlers/keyboardhandlers/CameraMovementHandler.h"

Engine::CameraMovement::CameraMovement(Engine::Camera * cam)
{
	camera = cam;
	usedKeys = { 'w', 'a', 's', 'd' };
}

void Engine::CameraMovement::onKeyPressed(unsigned char key, int x, int y, int mode)
{
	if (camera == nullptr)
		return;

	glm::vec3 t;
	switch (key)
	{
	case 'w':	// Forward
		t = glm::vec3(0.0f, 0.0f, 0.2f);
		break;
	case 's':	// Backward
		t = glm::vec3(0.0f, 0.0f, -0.2f);
		break;
	case 'a':	// Left
		t = glm::vec3(0.2f, 0.0f, 0.0f);
		break;
	case 'd':	// Right
		t = glm::vec3(-0.2f, 0.0f, 0.0f);
		break;
	}

	camera->translateView(t);
}