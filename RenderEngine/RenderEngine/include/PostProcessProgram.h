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
		unsigned int planeVAO;
		unsigned int planeVerticesVBO;

		unsigned int inPos;
		unsigned int inTexCoord;

		// OpenGL ensures 8 color attachments in any implementation
		// plus 1 for depth/stencil/depth & stencil
	protected:
		unsigned int uRenderedTextures[9];

	public:
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