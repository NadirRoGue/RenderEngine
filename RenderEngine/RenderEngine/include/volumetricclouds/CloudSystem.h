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
		class VolumetricClouds
		{
		private:
			VolumetricCloudProgram * shader;
			CloudFilterProgram * filterShader;
			Mesh * renderPlane;

			DeferredRenderObject * filterBuffer;
			TextureInstance * colorB;
		public:
			VolumetricClouds();
			void render(Camera * cam);
		};
	}
}