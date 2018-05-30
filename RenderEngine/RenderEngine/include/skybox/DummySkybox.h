#pragma once

#include "skybox/AbstractSkyBox.h"

namespace Engine
{
	class DummySkyBox : public AbstractSkyBox
	{
	public:
		void render(Camera * camera)
		{

		}
	};
}