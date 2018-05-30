#include "inputhandlers/mousehandlers/CameraRotationHandler.h"

#include "WorldConfig.h"
#include "Time.h"

const float Engine::CameraRotationHandler::ROTATION_SPEED = 50.0f;

Engine::CameraRotationHandler::CameraRotationHandler(std::string nam, Engine::Camera * cam)
	:MouseMotionHandler(nam)
{
	camera = cam;
	previousX = previousY = -1;

	horizontalSpeed = 0.01f;
	verticalSpeed = 0.01f;

	usedButtons = { 0 };
}

void Engine::CameraRotationHandler::handleMotion(int x, int y)
{
	if (Engine::Settings::showUI || static_cast<Engine::TravelMethod>(Engine::Settings::travelMethod) != Engine::TravelMethod::TRAVEL_MANUAL)
	{
		finishMotion();
		return;
	}

	if (camera != nullptr)
	{
		if (previousX == -1 && previousY == -1)
		{
			previousX = x;
			previousY = y;
		}
		else
		{
			int deltaX = x - previousX;
			int deltaY = y - previousY;

			previousX = x;
			previousY = y;

			float hAngle = deltaX * horizontalSpeed;
			float vAngle = deltaY * verticalSpeed;

			float speed = ROTATION_SPEED * Engine::Time::deltaTime;
			camera->rotateView(glm::vec3(vAngle * speed, hAngle * speed, 0.0f));
		}
	}
}

void Engine::CameraRotationHandler::finishMotion()
{
	previousX = previousY = -1;
}