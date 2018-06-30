/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "KeyboardHandler.h"
#include "Camera.h"

namespace Engine
{
	/**
	 * Class in charge to handle the camera movement input (by default, w,a,s,d)
	 */
	class CameraMovement : public KeyboardHandler
	{
	private:
		// Advance speed
		const static float MOVE_SPEED;
	private:
		// Camera to move
		Camera * camera;
	public:
		CameraMovement(Camera * cam);
		void onKeyPressed(unsigned char key, int x, int y, int mode);
	};
}