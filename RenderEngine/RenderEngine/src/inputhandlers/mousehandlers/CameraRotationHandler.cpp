#include "inputhandlers/mousehandlers/CameraRotationHandler.h"

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

			camera->rotateView(glm::vec3(vAngle, hAngle, 0.0f));
		}
	}
}

void Engine::CameraRotationHandler::finishMotion()
{
	previousX = previousY = -1;
}