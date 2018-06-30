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
		// Program id
		unsigned int glProgram;
		// Vertex shader id
		unsigned int vShader;
		// Fragment shader id
		unsigned int fShader;

		// Vertex shader file path
		std::string vShaderFile;
		// Fragment shader file path
		std::string fShaderFile;

		// Uber shader configuration bit mask
		unsigned long long parameters;

	private:
		// Program name (identifier)
		std::string name;

	public:
		// Constructor
		Program(std::string name, unsigned long long params = 0);
		// Copy constructor
		Program(const Program & other);
		~Program();

		// Returns shader name
		std::string getName() const;
		// Returns program id 
		unsigned int getProgramId() const;
		// Initializes shader (load shader source, apply UBER shader technique, compiles and links the program)
		virtual void initialize();

		// Initialize uniforms and attributes ids
		virtual void configureProgram() = 0;
		// Configures vbos with shader attributes
		virtual void configureMeshBuffers(Mesh * mesh) = 0;

		// Enables this program
		void use();
		// Apply uniforms which are common to all program instances (once per frame)
		virtual void applyGlobalUniforms();
		// Execute shader/object specific code before launch the drawing command
		virtual void onRenderObject(const Object * obj, Camera * camera) = 0;

		// Clean up
		virtual void destroy();
	protected:
		// Loads a shader source code and applies UBER Shader technique
		unsigned int loadShader(std::string fileName, GLenum type, std::string configString = "", bool outputToFile = false, std::string outputFileName = "");
	};

	// ===================================================================================================
	// Class in charge to create new programs or return existing ones if the engine is requesting an already
	// instanced program
	class ProgramFactory
	{
	private:
		// Program cache
		std::map<unsigned long long, Program*> cache;
	protected:
		// Each Program derived class will have its own factory which will have to implement this method
		virtual Program * createProgram(unsigned long long parameters) = 0;
	public:
		// Returns a program from the cache if present, or creates a new one and stores it
		Program * instantiateProgram(unsigned long long parameters);
		// Cleans cache
		void clean();
	};
}