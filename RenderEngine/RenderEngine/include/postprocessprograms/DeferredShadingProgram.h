#pragma once

#include "PostProcessProgram.h"

#include "Scene.h"

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

		unsigned int uWorldUp;

		unsigned int uSkyZenitColor;
		unsigned int uSkyHorizonColor;
	public:
		DeferredShadingProgram(std::string name, unsigned long long params);
		DeferredShadingProgram(const DeferredShadingProgram & other);

		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);
		void configureProgram();
	private:
		void processDirectionalLights(DirectionalLight * dl, const glm::mat4 & viewMatrix);
	};

	class DeferredShadingProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}