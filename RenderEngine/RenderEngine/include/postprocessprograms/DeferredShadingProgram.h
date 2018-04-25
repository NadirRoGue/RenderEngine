#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	class DeferredShadingProgram : public PostProcessProgram
	{
	private:
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

		unsigned int uBackground;

	public:
		DeferredShadingProgram(std::string name);
		DeferredShadingProgram(const DeferredShadingProgram & other);

		void onRenderLight(const glm::mat4 & model, const glm::mat4 & view);
		void onRenderSpotLight(const glm::mat4 & modelPos, const glm::mat4 & modelDir, const glm::mat4 & view);
		void onRenderDirectionalLight(const glm::mat4 & model, const glm::mat4 & view);
		//void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);

		void configureProgram();
		void configureClearColor(const glm::vec3 & cc);
		void configurePointLightBuffer(const PointLight *pl);
		void configureSpotLightBuffer(const SpotLight *pl);
		void configureDirectionalLightBuffer(const DirectionalLight *dl);

	};
}