#pragma once

#include "Program.h"

namespace Engine
{
	class ProceduralWaterProgram : public Program
	{
	public:
		static std::string PROGRAM_NAME;

		static const unsigned long long WIRE_DRAW_MODE;
		static const unsigned long long SHADOW_MAP;
	private:
		std::string gShaderFile;

		unsigned int gShader;

		unsigned int uInPos;
		unsigned int uInUV;

		unsigned int uModelView;
		unsigned int uModelViewProj;
		unsigned int uNormal;

		unsigned int uInInfo;
		unsigned int uScreenSize;

		unsigned int uLightDepthMatrix;
		unsigned int uLightDepthMatrix1;
		unsigned int uDepthTexture;
		unsigned int uDepthTexture1;
		unsigned int uLightDirection;

		unsigned int uGridPos;
		unsigned int uTime;
		unsigned int uWaterColor;
		unsigned int uWaterSpeed;
	public:
		ProceduralWaterProgram(std::string name, unsigned long long parameters);
		ProceduralWaterProgram(const ProceduralWaterProgram & other);

		void initialize();
		void configureProgram();
		void configureMeshBuffers(Mesh * mesh);

		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);

		void destroy();
		
		void setUniformGBufferInfo();
		void setTimeUniform(float sinTime);
		void setUniformGridPosition(unsigned int i, unsigned int j);
		void setUniformLightDepthMatrix(const glm::mat4 & ldm);
		void setUniformLightDepthMatrix1(const glm::mat4 & ldm);
		void setUniformDepthTexture(const TextureInstance * depthTexture);
		void setUniformDepthTexture1(const TextureInstance * depthTexture);
		void setUniformLightDirection(const glm::vec3 & lightDir);
	};

	// =========================================================

	class ProceduralWaterProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}