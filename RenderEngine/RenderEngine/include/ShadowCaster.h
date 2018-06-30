/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include <glm/glm.hpp>

#include "Camera.h"

namespace Engine
{
	// Class that represents an element that can cast shadows
	class ShadowCaster
	{
	public:
		virtual void renderShadow(Camera * camera, const glm::mat4 & projectionMatrix) = 0;
	};
}