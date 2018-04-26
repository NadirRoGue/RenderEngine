#pragma once

#include "Texture.h"
#include "programs/SkyProgram.h"
#include "Camera.h"
#include "Object.h"

namespace Engine
{
	class SkyBox
	{
	private:
		SkyProgram * shader;
		TextureInstance * skyCubeMap;
		Object * cubeMesh;

	public:
		SkyBox(TextureInstance * skyTextureCubeMap);
		~SkyBox();

		void render(Camera * camera);

	private:
		void initialize();
	};
}