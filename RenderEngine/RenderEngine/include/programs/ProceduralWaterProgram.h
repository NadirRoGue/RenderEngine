#pragma once

#include "programs/ProceduralTerrainProgram.h"

namespace Engine
{
	class ProceduralWaterProgram : public Program
	{
	public:
		static std::string PROGRAM_NAME;
	private:
		std::string tcsShaderFile;
		std::string tevalShaderFile;
		std::string gShaderFile;

		unsigned int tcsShader, tevalShader, gShader;

		unsigned int uInPos;
		unsigned int uInUV;

		unsigned int uModelView;
		unsigned int uModelViewProj;
		unsigned int uNormal;
		unsigned int uGridPos;
		unsigned int uTime;
		unsigned int uWaterColor;
		unsigned int uWaterSpeed;
	public:
		ProceduralWaterProgram(std::string name, unsigned long long parameters);
		ProceduralWaterProgram(const ProceduralWaterProgram & other);

		void initialize();
		void configureProgram();

		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);
		void destroy();

		void configureMeshBuffers(Mesh * mesh);
		void setTimeUniform(float sinTime);
		void setUniformGridPosition(unsigned int i, unsigned int j);
	};

	// =========================================================

	class ProceduralWaterProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}