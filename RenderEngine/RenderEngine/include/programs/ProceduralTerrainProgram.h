#pragma once

#include "Program.h"

namespace Engine
{
	class ProceduralTerrainProgram : public Program
	{
	public:
		static std::string PROGRAM_NAME;

		static unsigned long long WIRE_DRAW_MODE;
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
	public:
		ProceduralTerrainProgram(std::string name, unsigned long long params);
		ProceduralTerrainProgram(const ProceduralTerrainProgram & other);
		void initialize();
		void configureProgram();
		void setUniformGridPosition(unsigned int i, unsigned int j);
		void configureMeshBuffers(Mesh * mesh);
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);
		void destroy();
	};

	class ProceduralTerrainProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}