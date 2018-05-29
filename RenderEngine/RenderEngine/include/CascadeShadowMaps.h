#pragma once

#include <glm/glm.hpp>

#include "Camera.h"
#include "instances/TextureInstance.h"
#include "DeferredRenderObject.h"

namespace Engine
{
	class CascadeShadowMaps
	{
	private:
		typedef struct ShadowMap
		{
			glm::mat4 proj;
			glm::mat4 depth;
			DeferredRenderObject * rtt;
			TextureInstance * shadowMap;
		} ShadowMap;
	private:
		static CascadeShadowMaps * INSTANCE;
	private:
		ShadowMap shadowMaps[2];
		int currentLevel;
		
		// Shadow bias
		glm::mat4 biasMatrix;

		// Previous frame buffer before starting to render shadows to custom rtt
		int previousFrameBuffer;
	public:
		static CascadeShadowMaps & getInstance();
	private:
		CascadeShadowMaps();
		void initializeShadowMap(int level, float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
	public:
		void init();
		void initializeFrame(Camera * eye);
		void saveCurrentFBO();
		void beginShadowRender(int level);
		void endShadorRender();

		const glm::mat4 & getShadowProjectionMat();
		const glm::mat4 & getBiasMat();

		unsigned int getCascadeLevels();

		const glm::mat4 & getDepthMatrix0();
		const glm::mat4 & getDepthMatrix1();

		const TextureInstance * getDepthTexture0();
		const TextureInstance * getDepthTexture1();
	};
}