#pragma once

#include "Texture.h"
#include "programs/SkyProgram.h"
#include "Camera.h"
#include "Object.h"

namespace Engine
{
	class SkyBox : public IRenderable
	{
	private:
		SkyProgram * shader;
		TextureInstance * skyCubeMap;
		Object * cubeMesh;

		GLenum renderMode;
	public:
		SkyBox(TextureInstance * skyTextureCubeMap);
		~SkyBox();

		void render(Camera * camera);
		void notifyRenderModeUpdate(RenderMode mode);
	private:
		void initialize();
	};
}