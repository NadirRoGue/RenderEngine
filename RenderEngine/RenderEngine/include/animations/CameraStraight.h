#pragma once

#include "Animation.h"
#include "Camera.h"

#include <vector>

namespace Engine
{
	class CameraStraight : public Animation
	{
	private:
		Camera * cam;

		float moveSpeed;
		glm::vec3 moveDirection;
	public:
		CameraStraight(Camera * cam, glm::vec3 & moveDir);
		void update();
	};
}