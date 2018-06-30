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
	public:
		static std::string PROGRAM_NAME;
	private:
		// Screen space quad to render over
		unsigned int planeVAO;
		// Screen space quad vertices vbo
		unsigned int planeVerticesVBO;

		// Screen space vertices attributes (position and texture coordinats)
		unsigned int inPos;
		unsigned int inTexCoord;

		// OpenGL ensures 8 color attachments in any implementation
		// plus 1 for depth/stencil/depth & stencil
	protected:
		// Automatic filled and passed post-process textures
		unsigned int uRenderedTextures[9];

	public:
		// Constructors
		PostProcessProgram(std::string name, unsigned long long params);
		PostProcessProgram(unsigned long long params);
		PostProcessProgram(const PostProcessProgram & other);
		~PostProcessProgram();

		virtual void configureProgram();
		void configureMeshBuffers(Mesh * mesh);

		virtual void onRenderObject(const Object * obj, Camera * camera);
	};

	// =======================================================

	class PostProcessProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}