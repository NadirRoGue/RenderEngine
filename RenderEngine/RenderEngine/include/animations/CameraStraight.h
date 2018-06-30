/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Animation.h"
#include "Camera.h"

#include <vector>

namespace Engine
{
	/*
	 * Animation in charge of automatically animate the camera to
	 * advance in a straight in the view direction (parallel to the world surface)
	 */
	class CameraStraight : public Animation
	{
	private:
		// Camera to animate
		Camera * cam;

		// Speed movement
		float moveSpeed;
	public:
		CameraStraight(Camera * cam);
		void update();
	};
}