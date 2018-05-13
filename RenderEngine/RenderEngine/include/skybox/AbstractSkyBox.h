#pragma once

#include "Camera.h"

namespace Engine
{
	class AbstractSkyBox
	{
	public:
		virtual void render(Camera * camera) = 0;
	};
}