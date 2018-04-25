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
	};
}