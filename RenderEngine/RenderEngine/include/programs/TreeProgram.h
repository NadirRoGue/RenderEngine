#pragma once

#include "Program.h"

namespace Engine
{
	class TreeProgram : public Program
	{
	public:
		static const std::string PROGRAM_NAME;
	private:
		std::string gShaderFile;

		unsigned int gShader;

		unsigned int uModelViewProj;
		unsigned int uModelView;
		unsigned int uNormal;
		unsigned int uGridUV;
		unsigned int uAmplitude;
		unsigned int uFrecuency;
		unsigned int uScale;
		unsigned int uOctaves;

		unsigned int uInPos;
		unsigned int uInColor;
		unsigned int uInNormal;
		unsigned int uInEmissive;
	public:
		TreeProgram(std::string name, unsigned long long params);
		TreeProgram(const TreeProgram & other);

		void initialize();

		void configureProgram();
		void configureMeshBuffers(Mesh * mesh);
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);

		void setUniformTileUV(float u, float v);

		void destroy();
	};

	// ===============================================================

	class TreeProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long params);
	};
}