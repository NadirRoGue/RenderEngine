#pragma once
/*
#include "Program.h"

namespace Engine
{
	namespace GPUProgram
	{
		const unsigned long TEXTURE_ABLEDO_FLAG = 0x01;
		const unsigned long TEXTURE_NORMAL_FLAG = 0x02;
		const unsigned long TEXTURE_SPECULAR_FLAG = 0x04;
		const unsigned long TEXTURE_EMISSIVE_FLAG = 0x08;

		class StandarProgram : public Program
		{
		private:
			// Uniform variables
			unsigned int uNormalMat;
			unsigned int uModelViewMat;
			unsigned int uModelViewProjMat;

			// Attributes
			unsigned int inPos;
			unsigned int inNormal;
			unsigned int inTexCoord;
			unsigned int inTangent;

			// uniforms
			unsigned int uAlbedoTexture;
			unsigned int uNormalTexture;
			unsigned int uSpecularTexture;
			unsigned int uEmissiveTexture;

		public:
			StandarProgram(std::string name);
			StandarProgram(const StandarProgram & other);

			void configureProgram();
			virtual void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);
			virtual void configureMeshBuffers(MeshInstance * mesh);


			void releaseProgramBuffers(MeshInstance *mi);
		};
	}
}
*/