/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "MouseHandler.h"
#include "Camera.h"

namespace Engine
{
	/**
	 * Class in charge to manage the camera view rotation (by default, MLB + drag)
	 */
	class CameraRotationHandler : public MouseMotionHandler
	{
	private:
		// Rotation speed
		static const float ROTATION_SPEED;
	private:
		// Camera to move
		Camera * camera;

		// Flags holding the previous screen x and y position where we started
		// computing the rotation, using them to compute the movement relative
		// to the speed, and not the position on screen. Avoid abrupt rotations
		// and jumps
		int previousX, previousY;

		// Controls the speed scale in horizontal rotations (around Y axis)
		float horizontalSpeed;
		// Controls the speed scale in vertical rotations (around X axis)
		float verticalSpeed;
	public:
		CameraRotationHandler(std::string nam, Camera * cam);

		void handleMotion(int x, int y);

		// Reset previous position flags to prevent rotation jumps
		void finishMotion();
	};
}