#pragma once

#include <glm/glm.hpp>

#include "Camera.h"

namespace Engine
{
	class ShadowCaster
	{
	public:
		virtual void renderShadow(Camera * camera, const glm::mat4 & projectionMatrix) = 0;
	};
}