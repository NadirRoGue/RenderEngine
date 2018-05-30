#pragma once

#include "skybox/AbstractSkyBox.h"

#include "programs/SkyProgram.h"
#include "Camera.h"
#include "Object.h"
#include "volumetricclouds/CloudSystem.h"

namespace Engine
{
	class SkyBox : public AbstractSkyBox, public IRenderable
	{
	private:
		SkyProgram * shader;
		Object * cubeMesh;

		CloudSystem::VolumetricClouds * clouds;

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