#include "CascadeShadowMaps.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Scene.h"

Engine::CascadeShadowMaps * Engine::CascadeShadowMaps::INSTANCE = new Engine::CascadeShadowMaps();

Engine::CascadeShadowMaps & Engine::CascadeShadowMaps::getInstance()
{
	return *Engine::CascadeShadowMaps::INSTANCE;
}

Engine::CascadeShadowMaps::CascadeShadowMaps()
{
}

void Engine::CascadeShadowMaps::init()
{
	// Shadow bias
	biasMatrix = glm::mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	// Initialize shadow maps
	initializeShadowMap(0, -5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 50.0f);
	initializeShadowMap(1, -20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

void Engine::CascadeShadowMaps::initializeShadowMap(int level, float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
{
	shadowMaps[level].proj = glm::ortho<float>(minX, maxX, minY, maxY, minZ, maxZ);
	shadowMaps[level].rtt = new Engine::DeferredRenderObject(0, true);
	shadowMaps[level].shadowMap = shadowMaps[level].rtt->addDepthBuffer24(1024, 1024);
	shadowMaps[level].rtt->initialize();
}

void Engine::CascadeShadowMaps::initializeFrame(Engine::Camera * eye)
{
	Engine::DirectionalLight * dl = Engine::SceneManager::getInstance().getActiveScene()->getDirectionalLight();
	const glm::vec3 & cameraPosition = eye->getPosition();
	glm::vec3 target = glm::vec3(-cameraPosition.x, 0, -cameraPosition.z);
	glm::mat4 depthViewMatrix = glm::lookAt(target + (dl->getDirection() * 30.0f), target, glm::vec3(0, 1, 0));

	for (unsigned int i = 0; i < getCascadeLevels(); i++)
	{
		shadowMaps[i].depth = shadowMaps[i].proj * depthViewMatrix;
	}
}

void Engine::CascadeShadowMaps::saveCurrentFBO()
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);
}

void Engine::CascadeShadowMaps::beginShadowRender(int level)
{
	currentLevel = level;
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMaps[currentLevel].rtt->getFrameBufferId());
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Engine::CascadeShadowMaps::endShadorRender()
{
	shadowMaps[currentLevel].depth = biasMatrix * shadowMaps[currentLevel].depth;
	glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);
}

const glm::mat4 & Engine::CascadeShadowMaps::getBiasMat()
{
	return biasMatrix;
}

const glm::mat4 & Engine::CascadeShadowMaps::getShadowProjectionMat()
{
	return shadowMaps[currentLevel].depth;
}

unsigned int Engine::CascadeShadowMaps::getCascadeLevels()
{
	return 2;
}

const glm::mat4 & Engine::CascadeShadowMaps::getDepthMatrix0()
{
	return shadowMaps[0].depth;
}

const glm::mat4 & Engine::CascadeShadowMaps::getDepthMatrix1()
{
	return shadowMaps[1].depth;
}

const Engine::TextureInstance * Engine::CascadeShadowMaps::getDepthTexture0()
{
	return shadowMaps[0].shadowMap;
}

const Engine::TextureInstance * Engine::CascadeShadowMaps::getDepthTexture1()
{
	return shadowMaps[1].shadowMap;
}