#pragma once

#include "KeyboardHandler.h"
#include "Camera.h"

namespace Engine
{
	class CameraMovement : public KeyboardHandler
	{
	private:
		const static float MOVE_SPEED;
	private:
		Camera * camera;
	public:
		CameraMovement(Camera * cam);
		void onKeyPressed(unsigned char key, int x, int y, int mode);
	};
}