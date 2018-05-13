#pragma once

#include "skybox/AbstractSkyBox.h"

#include "Texture.h"
#include "programs/SkyProgram.h"
#include "Camera.h"
#include "Object.h"

namespace Engine
{
	class SkyBox : public AbstractSkyBox, public IRenderable
	{
	private:
		SkyProgram * shader;
		Object * cubeMesh;

		GLenum renderMode;
	public:
		SkyBox();
		~SkyBox();

		void render(Camera * camera);
		void notifyRenderModeUpdate(RenderMode mode);
	private:
		void initialize();
	};
}