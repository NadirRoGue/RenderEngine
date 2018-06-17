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

namespace Engine
{
	namespace CloudSystem
	{
		/**
		 * Class which represents the volumetric clouds. Is in charge
		 * of rendering the clouds, applying temporal reprojection, and
		 * performing a blur to the end result
		 */
		class VolumetricClouds
		{
		private:
			// Volume rendering clouds program
			VolumetricCloudProgram * shader;
			// Image blurring program
			CloudFilterProgram * filterShader;
			// Screen space quad to render
			Mesh * renderPlane;

			// Render target for temporal reprojection
			DeferredRenderObject * reprojectionBuffer[4];
			// Texture instances holding the previous frames
			TextureInstance * reproBuffer[4];
		public:
			VolumetricClouds();
			void render(Camera * cam);
		};
	}
}