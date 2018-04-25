/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Program.h"

#include "lights/PointLight.h"
#include "lights/SpotLight.h"
#include "lights/DirectionalLight.h"

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

	class PerlinGeneratorProgram : public PostProcessProgram
	{
	private:
		float scale;
		float amplitude;
		float frequency;
		float octaves;

		unsigned int perlinScale;
		unsigned int perlinAmplitude;
		unsigned int perlinFrequency;
		unsigned int perlinOctaves;
	public:
		PerlinGeneratorProgram(std::string name);
		PerlinGeneratorProgram(const PerlinGeneratorProgram & other);
		~PerlinGeneratorProgram();

		void configureProgram();
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);
	};
}