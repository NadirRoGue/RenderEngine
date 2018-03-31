/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <list>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "MeshInstance.h"
#include "Light.h"
#include "Object.h"

namespace Engine
{
	class Program
	{
	protected:
		unsigned int glProgram;

		unsigned int vShader;
		unsigned int fShader;
	private:
		std::string name;

	public:
		Program(std::string name);
		Program(const Program & other);
		~Program();

		std::string getName() const;
		unsigned int getProgramId() const;

		virtual void configurePointLightBuffer(const PointLight *pl);
		virtual void configureSpotLightBuffer(const SpotLight *pl);
		virtual void configureDirectionalLightBuffer(const DirectionalLight *dl);
		virtual void configureClearColor(const glm::vec3 & cc) = 0;

		virtual void onRenderLight(const glm::mat4 & model, const glm::mat4 & view);
		virtual void onRenderSpotLight(const glm::mat4 & modelPos, const glm::mat4 & modelDir, const glm::mat4 & view);
		virtual void onRenderDirectionalLight(const glm::mat4 & model, const glm::mat4 & view);
		virtual void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);

		virtual void initialize() = 0;
		virtual void configureProgram() = 0;
		virtual void configureMeshBuffers(MeshInstance * mesh) = 0;
		virtual void releaseProgramBuffers(MeshInstance * mi) = 0;
		virtual void destroy() = 0;
	protected:
		unsigned int loadShader(std::string fileName, GLenum type, std::string config);
	};

	// =============================================================================

	class StandarProgram : public Program
	{
	public:
		static unsigned long ALBEDO_BIT;
		static unsigned long NORMAL_MAP_BIT;
		static unsigned long EMISSIVE_MAP_BIT;
		static unsigned long SPECULAR_MAP_BIT;
		static unsigned long OCCLUSION_MAP_BIT;
		static unsigned long DISPLACEMENT_MAP_BIT;
		static unsigned long AUTOLOD_BIT;
	private:
		unsigned long flags;

		unsigned int tcsShader;
		unsigned int tevalShader;
		unsigned int gShader;

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
		StandarProgram(std::string name, unsigned long flags);
		StandarProgram(const StandarProgram & other);
		~StandarProgram();

		void initialize();
		void destroy();

		void configureProgram();
		void onRenderLight(const glm::mat4 & model, const glm::mat4 & view);
		void onRenderSpotLight(const glm::mat4 & modelPos, const glm::mat4 & modelDir, const glm::mat4 & view);
		void onRenderDirectionalLight(const glm::mat4 & model, const glm::mat4 & view);
		virtual void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);
		virtual void configureMeshBuffers(MeshInstance * mesh);
		
		void configureClearColor(const glm::vec3 & cc);
		void configurePointLightBuffer(const PointLight *pl);
		void configureSpotLightBuffer(const SpotLight *pl);
		void configureDirectionalLightBuffer(const DirectionalLight *dl);

		void releaseProgramBuffers(MeshInstance *mi);
	private:
		std::string createConfigString();
	};

	// =============================================================================

	class TextureProgram : public StandarProgram
	{
	private:
		unsigned int uAlbedoTex;
		unsigned int uEmissiveTex;
		unsigned int uSpecTex;
		unsigned int uNormalTex;
	public:
		TextureProgram(std::string name);
		TextureProgram(const TextureProgram & other);
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 & proj);
		void configureMeshBuffers(MeshInstance * mesh);
	};

	// =============================================================================

	
}