/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <list>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <memory>

#include "instances/MeshInstance.h"
#include "lights/PointLight.h"
#include "lights/SpotLight.h"
#include "lights/DirectionalLight.h"
#include "Object.h"

namespace Engine
{
	class Program
	{
	protected:
		unsigned int glProgram;
		unsigned int vShader;
		unsigned int fShader;

		std::string vShaderFile;
		std::string fShaderFile;

	private:
		std::string name;

	public:
		Program(std::string name, unsigned long long params = 0);
		Program(const Program & other);
		~Program();

		std::string getName() const;
		unsigned int getProgramId() const;
		virtual void initialize();

		virtual void configureProgram() = 0;
		virtual void configureMeshBuffers(MeshInstance * mesh) = 0;

		virtual void configurePointLightBuffer(const PointLight *pl);
		virtual void configureSpotLightBuffer(const SpotLight *pl);
		virtual void configureDirectionalLightBuffer(const DirectionalLight *dl);
		virtual void configureClearColor(const glm::vec3 & cc) {};

		virtual void onRenderLight(const glm::mat4 & model, const glm::mat4 & view) {};
		virtual void onRenderSpotLight(const glm::mat4 & modelPos, const glm::mat4 & modelDir, const glm::mat4 & view) {};
		virtual void onRenderDirectionalLight(const glm::mat4 & model, const glm::mat4 & view) {};
		virtual void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj) = 0;

		virtual void destroy();
	protected:
		virtual unsigned int loadShader(std::string fileName, GLenum type);
	};

	// ===================================================================================================
}