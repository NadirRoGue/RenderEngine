/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include "skybox/AbstractSkyBox.h"

#include "programs/SkyProgram.h"
#include "Camera.h"
#include "Object.h"
#include "volumetricclouds/CloudSystem.h"

namespace Engine
{
	/**
	 * Skybox implementation using a cubemap with a color gradient as sky,
	 * and volumetric post-process rendering for clouds
	 * Sun disk is rendered procedurally
	 */
	class SkyBox : public AbstractSkyBox, public IRenderable
	{
	private:
		// Sky cubemap program
		SkyProgram * shader;
		// Cube mesh
		Object * cubeMesh;

		// Cloud system
		CloudSystem::VolumetricClouds * clouds;

		// Render mode
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