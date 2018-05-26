#pragma once

#include "Camera.h"
#include "Mesh.h"
#include "postprocessprograms/VolumetricCloudProgram.h"

namespace Engine
{
	namespace CloudSystem
	{
		class VolumetricClouds
		{
		private:
			VolumetricCloudProgram * shader;
			Mesh * renderPlane;

		public:
			VolumetricClouds();
			void render(Camera * cam);
		};
	}
}