#pragma once
#pragma once

#include "Program.h"

namespace Engine
{
	class StandarProgram : public Program
	{
	public:
		static std::string PROGRAM_NAME;
	private:
		// Uniform variables
		unsigned int uNormalMat;
		unsigned int uModelViewMat;
		unsigned int uModelViewProjMat;

		unsigned int uPointLightPos;
		unsigned int uIa;
		unsigned int uId;
		unsigned int uIs;
		unsigned int uPLattenuation;

		unsigned int uSpotLightPos;
		unsigned int uSpotLightDir;
		unsigned int uSLIa;
		unsigned int uSLId;
		unsigned int uSLIs;
		unsigned int uSLapperture;
		unsigned int uSLm;
		unsigned int uSLattenuation;

		unsigned int uDirectionalLightDir;
		unsigned int uDLIa;
		unsigned int uDLId;
		unsigned int uDLIs;

		unsigned int uComputeShading;
		unsigned int uBackground;

		// Attributes
		unsigned int inPos;
		unsigned int inColor;
		unsigned int inNormal;
		unsigned int inTexCoord;
		unsigned int inTangent;
	public:
		StandarProgram(std::string name, unsigned long long params);
		StandarProgram(const StandarProgram & other);
		virtual void configureProgram();
		void onRenderLight(const glm::mat4 & model, const glm::mat4 & view);
		void onRenderSpotLight(const glm::mat4 & modelPos, const glm::mat4 & modelDir, const glm::mat4 & view);
		void onRenderDirectionalLight(const glm::mat4 & model, const glm::mat4 & view);
		virtual void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);
		void configureMeshBuffers(Mesh * mesh);

		void configureClearColor(const glm::vec3 & cc);
		void configurePointLightBuffer(const PointLight *pl);
		void configureSpotLightBuffer(const SpotLight *pl);
		void configureDirectionalLightBuffer(const DirectionalLight *dl);
	};

	class StandarProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};

	// =============================================================================

	class TextureProgram : public StandarProgram
	{
	public:
		static std::string PROGRAM_NAME;
	private:
		unsigned int uAlbedoTex;
		unsigned int uEmissiveTex;
		unsigned int uSpecTex;
		unsigned int uNormalTex;
	public:
		TextureProgram(std::string name, unsigned long long params);
		TextureProgram(const TextureProgram & other);
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 & proj);
		void configureProgram();
	};

	class TextureProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}