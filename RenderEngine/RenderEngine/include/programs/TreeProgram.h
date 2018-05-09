#pragma once

#include "Program.h"

namespace Engine
{
	class TreeProgram : public Program
	{
	public:
		static const std::string PROGRAM_NAME;

		const static unsigned long long SHADOW_MAP;
		const static unsigned long long WIRE_MODE;
	private:
		std::string gShaderFile;

		unsigned int gShader;

		unsigned int uModelViewProj;
		unsigned int uModelView;
		unsigned int uNormal;
		unsigned int uLightDepthMat;
		unsigned int uGridUV;
		unsigned int uAmplitude;
		unsigned int uFrecuency;
		unsigned int uScale;
		unsigned int uOctaves;
		unsigned int uLightDir;
		unsigned int uDepthMap;
		unsigned int uWaterLevel;

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
		void setUniformLightDepthMat(const glm::mat4 & ldp);
		void setUniformLightDir(const glm::vec3 & ld);
		void setUniformDepthMap(TextureInstance * ti);

		void destroy();
	};

	// ===============================================================

	class TreeProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long params);
	};
}