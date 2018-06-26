/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Camera.h"
#include "instances/TextureInstance.h"
#include "DeferredRenderObject.h"
#include "ShadowCaster.h"

namespace Engine
{
	// Handles all shadow casters and give access to the shadow map textures to
	// any shaders which need them
	class CascadeShadowMaps
	{
	private:
		// Cascade level information
		typedef struct ShadowMap
		{
			// Projection matrix (holds the projection volume)
			glm::mat4 proj;
			// Light depth projection matrix
			glm::mat4 depth;
			// FBO
			DeferredRenderObject * rtt;
			// Pointer to the shadow map texture
			TextureInstance * shadowMap;
		} ShadowMap;
	private:
		static CascadeShadowMaps * INSTANCE;
	private:
		// Cascade shadow maps (2 by default)
		ShadowMap shadowMaps[2];
		// varying variable used during runtime
		int currentLevel;
		
		// Shadow bias
		glm::mat4 biasMatrix;

		// Previous frame buffer before starting to render shadows to custom rtt
		int previousFrameBuffer;

		// List of renderable objects which cast shadows
		std::vector<ShadowCaster *> shadowCasters;
	public:
		static CascadeShadowMaps & getInstance();
	private:
		CascadeShadowMaps();
		void initializeShadowMap(int level, float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
	public:
		// Init all static data not changed throught the execution
		void init();
		// Computes the depth matrices given the camera position at the beginning of each frame (only once per frame)
		// We want the shadow maps to be projected around the camera
		void initializeFrame(Camera * eye);
		// Prepares the system to render to the shadow map
		void beginShadowRender(int level);
		// Restores engine previous state
		void endShadowRender();
		// Register a element which can cast shadows
		void registerShadowCaster(ShadowCaster * caster);

		// Calls the shadow render code of each registered shadowcaster
		void renderShadows(Camera * cam);

		const glm::mat4 & getShadowProjectionMat();
		const glm::mat4 & getBiasMat();

		unsigned int getCascadeLevels();

		const glm::mat4 & getDepthMatrix0();
		const glm::mat4 & getDepthMatrix1();

		const TextureInstance * getDepthTexture0();
		const TextureInstance * getDepthTexture1();
	};
}