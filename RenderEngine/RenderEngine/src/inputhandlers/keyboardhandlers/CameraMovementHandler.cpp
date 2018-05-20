#include "inputhandlers/keyboardhandlers/CameraMovementHandler.h"

#include "WorldConfig.h"
#include "Time.h"

const float Engine::CameraMovement::MOVE_SPEED = 100.0f;

Engine::CameraMovement::CameraMovement(Engine::Camera * cam)
{
	camera = cam;
	usedKeys = { 'w', 'a', 's', 'd' };
}

void Engine::CameraMovement::onKeyPressed(unsigned char key, int x, int y, int mode)
{
	if (camera == nullptr || static_cast<Engine::TravelMethod>(Engine::Settings::travelMethod) != Engine::TravelMethod::TRAVEL_MANUAL)
		return;

	glm::vec3 t;
	float dt = Engine::Time::deltaTime;
	switch (key)
	{
	case 'w':	// Forward
		t = glm::vec3(0.0f, 0.0f, MOVE_SPEED) * dt;
		break;
	case 's':	// Backward
		t = glm::vec3(0.0f, 0.0f, -MOVE_SPEED) * dt;
		break;
	case 'a':	// Left
		t = glm::vec3(MOVE_SPEED, 0.0f, 0.0f) * dt;
		break;
	case 'd':	// Right
		t = glm::vec3(-MOVE_SPEED, 0.0f, 0.0f) * dt;
		break;
	}

	camera->translateView(t);
}