#pragma once

#include "Program.h"

namespace Engine
{
	class ProceduralTerrainProgram : public Program
	{
	public:
		static const std::string PROGRAM_NAME;

		static const unsigned long long WIRE_DRAW_MODE;
		static const unsigned long long SHADOW_MAP;
	protected:
		std::string tcsShaderFile;
		std::string tevalShaderFile;
		std::string gShaderFile;

		unsigned int tcsShader, tevalShader, gShader;

		unsigned int uInPos;
		unsigned int uInUV;

		unsigned int uModelView;
		unsigned int uModelViewProj;
		unsigned int uNormal;

		unsigned int uLightDepthMatrix;
		unsigned int uLightDepthMatrix1;
		unsigned int uDepthTexture;
		unsigned int uDepthTexture1;
		unsigned int uLightDirection;

		unsigned int uWaterLevel;

		unsigned int uTime;

		unsigned int uGridPos;

		unsigned int uAmplitude;
		unsigned int uFrecuency;
		unsigned int uScale;
		unsigned int uOctaves;

		unsigned int uGrassCoverage;
		unsigned int uGrassColor;
		unsigned int uSandColor;
		unsigned int uRockColor;
	public:
		ProceduralTerrainProgram(std::string name, unsigned long long params);
		ProceduralTerrainProgram(const ProceduralTerrainProgram & other);

		void initialize();
		virtual void configureProgram();
		void configureMeshBuffers(Mesh * mesh);

		void applyGlobalUniforms();
		void onRenderObject(const Object * obj, Camera * camera);

		void destroy();

		void setUniformGridPosition(unsigned int i, unsigned int j);
		void setUniformLightDepthMatrix(const glm::mat4 & ldm);
		void setUniformLightDepthMatrix1(const glm::mat4 & ldm);
	};

	class ProceduralTerrainProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}