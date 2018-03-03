/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Program.h"

namespace Engine
{
	class PostProcessProgram : public Program
	{
	private:
		unsigned int planeVAO;
		unsigned int planeVerticesVBO;

		unsigned int inPos;
		unsigned int inTexCoord;

		// OpenGL ensures 8 color attachments in any implementation
		// plus 1 for depth/stencil/depth & stencil
		unsigned int uRenderedTextures[9];

	public:
		PostProcessProgram(std::string name);
		PostProcessProgram(const PostProcessProgram & other);
		~PostProcessProgram();

		virtual void configureProgram();
		void configureMeshBuffers(MeshInstance * mesh);
		virtual void configureClearColor(const glm::vec3 & cc);

		virtual void configurePointLightBuffer(const PointLight *pl);
		virtual void configureSpotLightBuffer(const SpotLight *pl);
		virtual void configureDirectionalLightBuffer(const DirectionalLight *dl);

		virtual void onRenderLight(const glm::mat4 & model, const glm::mat4 & view);
		virtual void onRenderSpotLight(const glm::mat4 & modelPos, const glm::mat4 & modelDir, const glm::mat4 & view);
		virtual void onRenderDirectionalLight(const glm::mat4 & model, const glm::mat4 & view);
		virtual void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);


		void releaseProgramBuffers(MeshInstance * mi);
	};

	// ==========================================================================

	class GaussianBlurProgram : public PostProcessProgram
	{
	private:
		unsigned int uAffetedTexels;
		unsigned int uMaskSize;
		unsigned int uMask;
		unsigned int uTexelSize;

		unsigned int maskSize;
		float * kernel;
		float * affectedTexels;

	public:
		GaussianBlurProgram(std::string name);
		GaussianBlurProgram(const GaussianBlurProgram & other);
		~GaussianBlurProgram();

		virtual void configureProgram();
		virtual void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);

		void setMaskSize(unsigned int ms);
		void setKernel(float * k);
		void setAffectedTexels(glm::vec2 * at);
	};

	// ==========================================================================

	class DepthOfFieldProgram : public GaussianBlurProgram
	{
	private:
		unsigned int uFocalDistance;
		unsigned int uMaxDistanceFactor;
		unsigned int uInverseProj;

		float focalDistance;
		float maxDistanceFactor;

	public:
		DepthOfFieldProgram(std::string name);
		DepthOfFieldProgram(const DepthOfFieldProgram & other);
		~DepthOfFieldProgram();

		void configureProgram();
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);

		float getFocalDistance();
		float getMaxDistanceFactor();

		void setFocalDistance(float fd);
		void setMaxDistanceFactor(float mdf);
	};

	// ==========================================================================

	class DepthRenderProgram : public PostProcessProgram
	{
	private:
		unsigned int uInverseProj;

	public:
		DepthRenderProgram(std::string name);
		DepthRenderProgram(const DepthRenderProgram & other);
		~DepthRenderProgram();

		void configureProgram();
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);
	};

	// ==========================================================================

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

	// ==========================================================================

	class EdgeBasedProgram : public PostProcessProgram
	{
	private:
		unsigned int uTexelSize;
	public:
		EdgeBasedProgram(std::string name);
		EdgeBasedProgram(const EdgeBasedProgram & other);
		~EdgeBasedProgram();

		void configureProgram();
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);
	};
}