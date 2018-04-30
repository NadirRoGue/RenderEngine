#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	class DeferredShadingProgram : public PostProcessProgram
	{
	public:
		static std::string PROGRAM_NAME;
	private:

		unsigned int uDirectionalLightDir;
		unsigned int uDLIa;
		unsigned int uDLId;
		unsigned int uDLIs;

		unsigned int uDLBuffer;
		unsigned int uPLBuffer;
		unsigned int uSLBuffer;

		unsigned int uBackground;

		bool lightBufferInitialized;

	public:
		DeferredShadingProgram(std::string name, unsigned long long params);
		DeferredShadingProgram(const DeferredShadingProgram & other);

		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);
		void configureProgram();

		// DEPRECATED
		//void onRenderLight(const glm::mat4 & model, const glm::mat4 & view);
		//void onRenderSpotLight(const glm::mat4 & modelPos, const glm::mat4 & modelDir, const glm::mat4 & view);
		//void onRenderDirectionalLight(const glm::mat4 & model, const glm::mat4 & view);
	
		//void configureClearColor(const glm::vec3 & cc);
		//void configurePointLightBuffer(const PointLight *pl);
		//void configureSpotLightBuffer(const SpotLight *pl);
		//void configureDirectionalLightBuffer(const DirectionalLight *dl);
	private:
		void initializeLightBuffers();
	};

	class DeferredShadingProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}