/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <list>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <memory>

#include "Camera.h"
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

		unsigned long long parameters;

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
		virtual void configureMeshBuffers(Mesh * mesh) = 0;

		void use();
		virtual void applyGlobalUniforms();
		virtual void onRenderObject(const Object * obj, Camera * camera) = 0;

		virtual void destroy();
	protected:
		unsigned int loadShader(std::string fileName, GLenum type, std::string configString = "", bool outputToFile = false, std::string outputFileName = "");
	};

	// ===================================================================================================

	class ProgramFactory
	{
	private:
		std::map<unsigned long long, Program*> cache;
	protected:
		virtual Program * createProgram(unsigned long long parameters) = 0;
	public:
		Program * instantiateProgram(unsigned long long parameters);

		void clean();
	};
}