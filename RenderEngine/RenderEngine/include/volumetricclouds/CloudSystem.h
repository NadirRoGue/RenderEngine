/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include "Camera.h"
#include "Mesh.h"
#include "DeferredRenderObject.h"
#include "postprocessprograms/VolumetricCloudProgram.h"
#include "postprocessprograms/CloudFilterProgram.h"
#include "programs/CloudShadowProgram.h"

#include "ShadowCaster.h"

namespace Engine
{
	namespace CloudSystem
	{
		/**
		 * Class which represents the volumetric clouds. Is in charge
		 * of rendering the clouds, applying temporal reprojection, and
		 * performing a blur to the end result
		 */
		class VolumetricClouds : public ShadowCaster
		{
		private:
			// Volume rendering clouds program
			VolumetricCloudProgram * shader;
			// Image blurring program
			CloudFilterProgram * filterShader;
			// Shadow cast program
			CloudShadowProgram * shadowShader;
			// Screen space quad to render
			Mesh * renderPlane;
			// World space plane to render shadows
			Object * skyPlane;

			// Render target for temporal reprojection
			DeferredRenderObject * reprojectionBuffer[4];
			// Texture instances holding the previous frames
			TextureInstance * reproBuffer[4];
			TextureInstance * pixelVelocityMap[4];
		public:
			VolumetricClouds();
			void render(Camera * cam);
			void renderShadow(Camera * camera, const glm::mat4 & projectionMatrix);
		private:
			void createTileMesh();
		};
	}
}