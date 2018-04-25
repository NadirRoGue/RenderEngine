#pragma once

#include "MouseHandler.h"
#include "Camera.h"

namespace Engine
{
	class CameraRotationHandler : public MouseMotionHandler
	{
	private:
		Camera * camera;

		int previousX, previousY;

		float horizontalSpeed;
		float verticalSpeed;
	public:
		CameraRotationHandler(std::string nam, Camera * cam);

		void handleMotion(int x, int y);
		void finishMotion();
	};
}