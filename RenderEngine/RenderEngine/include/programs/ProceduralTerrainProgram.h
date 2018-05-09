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
		unsigned int uDepthTexture;
		unsigned int uLightDirection;

		unsigned int uWaterLevel;

		unsigned int uGridPos;

		unsigned int uAmplitude;
		unsigned int uFrecuency;
		unsigned int uScale;
		unsigned int uOctaves;
	public:
		ProceduralTerrainProgram(std::string name, unsigned long long params);
		ProceduralTerrainProgram(const ProceduralTerrainProgram & other);

		void initialize();
		virtual void configureProgram();
		void configureMeshBuffers(Mesh * mesh);

		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);

		void destroy();

		void setUniformGridPosition(unsigned int i, unsigned int j);
		void setUniformLightDepthMatrix(const glm::mat4 & ldm);
		void setUniformDepthTexture(TextureInstance * depthTexture);
		void setUniformLightDirection(const glm::vec3 & lightDir);
	};

	class ProceduralTerrainProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}