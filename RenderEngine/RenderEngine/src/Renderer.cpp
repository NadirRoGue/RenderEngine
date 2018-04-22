/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "Renderer.h"
#include "MeshInstanceTable.h"
#include "ProgramTable.h"
#include "PostProcessProgram.h"

#include <iostream>


unsigned int Engine::ScreenManager::SCREEN_HEIGHT = 500;
unsigned int Engine::ScreenManager::SCREEN_WIDTH = 500;

unsigned int Engine::ScreenManager::REAL_SCREEN_HEIGHT = 500;
unsigned int Engine::ScreenManager::REAL_SCREEN_WIDTH = 500;

// ==================================================================

Engine::Renderer::Renderer()
{
	activeCam = 0;
}

Engine::Renderer::~Renderer()
{
}

void Engine::Renderer::initialize()
{
	renderFromCamera(Engine::SceneManager::getInstance().getActiveScene()->getCamera());
}

void Engine::Renderer::renderFromCamera(Engine::Camera * cam)
{
	activeCam = cam;
}

// ==================================================================

Engine::ForwardRenderer::ForwardRenderer()
	:Engine::Renderer()
{
}

Engine::ForwardRenderer::~ForwardRenderer()
{
}

void Engine::ForwardRenderer::doRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Engine::Scene * scene = Engine::SceneManager::getInstance().getActiveScene();

	if (scene == 0)
		return;

	if (scene->getTerrain() != NULL)
	{
		scene->getTerrain()->render(activeCam);
	}

	const std::map<std::string, Engine::ProgramRenderables *> renders = scene->getObjects();

	const std::map<std::string, Engine::PointLight *> pointLights = scene->getPointLights();
	const std::map<std::string, Engine::SpotLight *> spotLights = scene->getSpotLights();
	const std::map<std::string, Engine::DirectionalLight *> directionalLights = scene->getDirectionalLight();

	std::map<std::string, Engine::ProgramRenderables *>::const_iterator renderableIt;
	for (renderableIt = renders.cbegin(); renderableIt != renders.cend(); renderableIt++)
	{
		// Stablish program to use
		// changing program is expensive -> https://www.opengl.org/discussion_boards/showthread.php/185615-cheep-expensive-calls
		glUseProgram(renderableIt->second->program->getProgramId());

		if (Engine::RenderManager::getInstance().isForwardRendering())
		{
			std::map<std::string, Engine::PointLight *>::const_iterator it = pointLights.cbegin();
			while (it != pointLights.end())
			{
				renderableIt->second->program->onRenderLight(it->second->getModelMatrix(), activeCam->getViewMatrix());
				it++;
			}

			std::map<std::string, Engine::SpotLight *>::const_iterator slIt = spotLights.cbegin();
			while (slIt != spotLights.end())
			{
				renderableIt->second->program->onRenderSpotLight(slIt->second->getModelMatrix(), slIt->second->getDirModelMatrix(), activeCam->getViewMatrix());
				slIt++;
			}

			std::map<std::string, Engine::DirectionalLight *>::const_iterator dlIt = directionalLights.cbegin();
			while (dlIt != directionalLights.end())
			{
				renderableIt->second->program->onRenderDirectionalLight(dlIt->second->getModelMatrix(), activeCam->getViewMatrix());
				dlIt++;
			}
		}

		renderProgram(activeCam, renderableIt->second);
	}
}

void Engine::ForwardRenderer::renderProgram(Engine::Camera * camera, Engine::ProgramRenderables * renderables)
{
	glm::mat4 & viewMatrix = camera->getViewMatrix();
	glm::mat4 & projMatrix = camera->getProjectionMatrix();

	Engine::Program * program = renderables->program;

	std::map<unsigned int, std::list<Engine::Object *>>::const_iterator it;
	for (it = renderables->objects.cbegin(); it != renderables->objects.cend(); it++)
	{
		// Stablish vao to use
		glBindVertexArray(it->first);

		std::list<Engine::Object *> meshes = it->second;
		std::list<Engine::Object *>::iterator listIt;

		for (listIt = meshes.begin(); listIt != meshes.end(); listIt++)
		{
			Object * objToRender = *listIt;
			program->onRenderObject(objToRender, viewMatrix, projMatrix);

			unsigned int vertexPerFace = objToRender->getMesh()->getNumVerticesPerFace();
			glDrawElements(objToRender->getRenderMode(), objToRender->getMesh()->getNumFaces() * vertexPerFace, GL_UNSIGNED_INT, (void*)0);
		}
	}
}

void Engine::ForwardRenderer::onResize(unsigned int w, unsigned int h)
{

}

// ==================================================================

Engine::DeferredRenderer::DeferredRenderer()
	:Engine::Renderer()
{
	preProcess = nullptr;
	forwardPass = new Engine::ForwardRenderer();
	initialized = false;
}

Engine::DeferredRenderer::~DeferredRenderer()
{
	if (forwardPass != 0)
	{
		delete forwardPass;
	}
}

void Engine::DeferredRenderer::setForwardPassBuffers(Engine::DeferredRenderObject * buffers)
{
	forwardPassBuffer = buffers;
}

void Engine::DeferredRenderer::setPreProcess(Engine::PostProcessChainNode * node)
{
	preProcess = node;
}

void Engine::DeferredRenderer::addPostProcess(Engine::PostProcessChainNode * object)
{
	postProcessChain.push_back(object);
}

void Engine::DeferredRenderer::setFinalPostProcess(Engine::PostProcessChainNode * object)
{
	finalLink = object;
}

void Engine::DeferredRenderer::initialize()
{
	Engine::Renderer::initialize();

	if (initialized)
		return;

	initialized = true;

	forwardPassBuffer->initialize();

	std::list<Engine::PostProcessChainNode *>::iterator it = postProcessChain.begin();
	Engine::DeferredRenderObject * previousLink = forwardPassBuffer;
	while (it != postProcessChain.end())
	{
		Engine::PostProcessChainNode * node = (*it);
		// Initialize FBO and textures
		node->renderBuffer->initialize();
		// Set the output of the previous pass as the input of the next
		previousLink->populateDeferredObject(node->obj);
		previousLink = node->renderBuffer;

		if (node->callBack != 0)
		{
			node->callBack->initialize(node->obj, node->postProcessProgram, node->renderBuffer);
		}

		it++;
	}

	// Close the final link (will output to screen)
	previousLink->populateDeferredObject(finalLink->obj);
}

void Engine::DeferredRenderer::doRender()
{
	Engine::Scene * scene = Engine::SceneManager::getInstance().getActiveScene();

	if (scene == 0)
		return;

	Engine::Camera * cam = activeCam;
	/*
	if (preProcess != nullptr)
	{
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		glBindFramebuffer(GL_FRAMEBUFFER, preProcess->renderBuffer->getFrameBufferId());
		glUseProgram(preProcess->postProcessProgram->getProgramId());
		glBindVertexArray(preProcess->obj->getMeshInstance()->getMesh()->vao);
		preProcess->postProcessProgram->onRenderObject(preProcess->obj, cam->getViewMatrix(), cam->getProjectionMatrix());
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	}
	*/
	glBindFramebuffer(GL_FRAMEBUFFER, forwardPassBuffer->getFrameBufferId());
	
	forwardPass->renderFromCamera(cam);
	forwardPass->doRender();
	
	std::list<Engine::PostProcessChainNode *>::iterator it = postProcessChain.begin();
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	while (it != postProcessChain.end())
	{
		Engine::PostProcessChainNode * node = (*it);

		Engine::DeferredRenderObject * buffer = node->renderBuffer;
		glBindFramebuffer(GL_FRAMEBUFFER, buffer->getFrameBufferId());

		Engine::Program * prog = node->postProcessProgram;
		glUseProgram(prog->getProgramId());

		if (node->callBack != 0)
		{
			node->callBack->execute(node->obj, node->postProcessProgram, node->renderBuffer, cam);
		}

		glBindVertexArray(node->obj->getMeshInstance()->getMesh()->vao);

		prog->onRenderObject(node->obj, cam->getViewMatrix(), cam->getProjectionMatrix());

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		it++;
	}

	// Enable default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	if (finalLink->callBack != 0)
	{
		finalLink->callBack->execute(finalLink->obj, finalLink->postProcessProgram, finalLink->renderBuffer, cam);
	}

	glUseProgram(finalLink->postProcessProgram->getProgramId());
	glBindVertexArray(finalLink->obj->getMeshInstance()->getMesh()->vao);

	finalLink->postProcessProgram->onRenderObject(finalLink->obj, cam->getViewMatrix(), cam->getProjectionMatrix());

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void Engine::DeferredRenderer::onResize(unsigned int w, unsigned int h)
{
	if (preProcess != nullptr)
		preProcess->renderBuffer->resizeFBO(w, h);
	forwardPassBuffer->resizeFBO(w, h);

	std::list<Engine::PostProcessChainNode *>::iterator it = postProcessChain.begin();
	while (it != postProcessChain.end())
	{
		(*it)->renderBuffer->resizeFBO(w, h);
		it++;
	}
}

// =========================================================================

Engine::SideBySideRenderer::SideBySideRenderer()
	:Engine::Renderer()
{
	leftRenderer = new Engine::DeferredRenderer();
	rightRenderer = new Engine::DeferredRenderer();
	initialized = false;
	clearScreen = true;
	c = 0;
}

Engine::SideBySideRenderer::~SideBySideRenderer()
{
	if (leftRenderer != 0)
	{
		delete leftRenderer;
	}

	if (rightRenderer != 0)
	{
		delete rightRenderer;
	}
}

void Engine::SideBySideRenderer::setLeftForwardPassBuffer(Engine::DeferredRenderObject * buffer)
{
	leftRenderer->setForwardPassBuffers(buffer);
}

void Engine::SideBySideRenderer::setRightForwardPassBuffer(Engine::DeferredRenderObject * buffer)
{
	rightRenderer->setForwardPassBuffers(buffer);
}

void Engine::SideBySideRenderer::addLeftPostProcess(Engine::PostProcessChainNode * node)
{
	leftRenderer->addPostProcess(node);
}

void Engine::SideBySideRenderer::addRightPostProcess(Engine::PostProcessChainNode * node)
{
	rightRenderer->addPostProcess(node);
}

void Engine::SideBySideRenderer::setClearScreen(bool val)
{
	clearScreen = val;
}

void Engine::SideBySideRenderer::initialize()
{
	if (initialized)
		return;

	initialized = true;

	Engine::Program * sourcePostProcess = Engine::ProgramTable::getInstance().getProgramByName("post_processing_program");
	Engine::MeshInstance * mi = Engine::MeshInstanceTable::getInstance().getMeshInstance("left_plane", "post_processing_program");

	Engine::PostProcessChainNode * lNode = new Engine::PostProcessChainNode;
	lNode->postProcessProgram = new Engine::PostProcessProgram(*dynamic_cast<Engine::PostProcessProgram*>(sourcePostProcess));
	lNode->renderBuffer = 0;
	lNode->callBack = new Engine::MotionBlurImpl();
	lNode->obj = new Engine::PostProcessObject(mi);

	leftRenderer->setFinalPostProcess(lNode);
	leftRenderer->initialize();

	Engine::MeshInstance * miR = Engine::MeshInstanceTable::getInstance().getMeshInstance("right_plane", "post_processing_program");
	Engine::PostProcessChainNode * rNode = new Engine::PostProcessChainNode;
	rNode->postProcessProgram = new Engine::PostProcessProgram(*dynamic_cast<Engine::PostProcessProgram*>(sourcePostProcess));
	rNode->renderBuffer = 0;
	rNode->callBack = new Engine::MotionBlurImpl();
	rNode->obj = new Engine::PostProcessObject(miR);
	
	rightRenderer->setFinalPostProcess(rNode);
	rightRenderer->initialize();
}

void Engine::SideBySideRenderer::doRender()
{
	if (clearScreen)
	{
		// Needed to clear screen when switching from a full screen renderer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (c == 1)
		{
			clearScreen = false;
			c = 0;
		}
		c++;
	}

	Engine::Camera * leftCam = Engine::SceneManager::getInstance().getActiveScene()->getCamera();

	Engine::Camera rightCamera(*leftCam);
	rightCamera.translateView(glm::vec3(-0.4f, 0.0f, 0.0f));

	leftRenderer->renderFromCamera(leftCam);
	leftRenderer->doRender();

	rightRenderer->renderFromCamera(&rightCamera);
	rightRenderer->doRender();
}

void Engine::SideBySideRenderer::onResize(unsigned int w, unsigned int h)
{
	leftRenderer->onResize(w, h);
	rightRenderer->onResize(w, h);
}

// =========================================================================
// =========================================================================

Engine::RenderManager * Engine::RenderManager::INSTANCE = new Engine::RenderManager();

Engine::RenderManager & Engine::RenderManager::getInstance()
{
	return *INSTANCE;
}

Engine::RenderManager::RenderManager()
{
	forwardRenderer = 0;
	deferredRenderer = 0;
	sideBySideRenderer = 0;
	activeRender = 0;
}

Engine::RenderManager::~RenderManager()
{
	if (forwardRenderer != 0)
		delete forwardRenderer;

	if (deferredRenderer != 0)
		delete deferredRenderer;

	activeRender = 0;
}

void Engine::RenderManager::doRender()
{
	activeRender->doRender();
}

void Engine::RenderManager::forwardRender()
{
	if (forwardRenderer != 0)
	{
		activeRender = forwardRenderer;
		forwardRendering = true;
	}
}

void Engine::RenderManager::deferredRender()
{
	if (deferredRenderer != 0)
	{
		activeRender = deferredRenderer;
		forwardRendering = false;
	}
}

void Engine::RenderManager::sideBySideRender()
{
	if (sideBySideRenderer != 0)
	{
		sideBySideRenderer->setClearScreen(true);
		activeRender = sideBySideRenderer;
		forwardRendering = false;
	}
}

void Engine::RenderManager::setForwardRenderer(Engine::ForwardRenderer * fr)
{
	forwardRenderer = fr;
	forwardRenderer->initialize();
	forwardRenderer->onResize(ScreenManager::REAL_SCREEN_WIDTH, ScreenManager::REAL_SCREEN_HEIGHT);
}

void Engine::RenderManager::setDeferredRenderer(Engine::DeferredRenderer * dr)
{
	deferredRenderer = dr;
	deferredRenderer->initialize();
	deferredRenderer->onResize(ScreenManager::REAL_SCREEN_WIDTH, ScreenManager::REAL_SCREEN_HEIGHT);
}

void Engine::RenderManager::setSideBySideRenderer(Engine::SideBySideRenderer * renderer)
{
	sideBySideRenderer = renderer;
	sideBySideRenderer->initialize();
	sideBySideRenderer->onResize(ScreenManager::REAL_SCREEN_WIDTH, ScreenManager::REAL_SCREEN_HEIGHT);
}

void Engine::RenderManager::doResize(unsigned int w, unsigned int h)
{
	Engine::ScreenManager::SCREEN_WIDTH = Engine::ScreenManager::REAL_SCREEN_WIDTH = w;
	Engine::ScreenManager::SCREEN_HEIGHT = Engine::ScreenManager::REAL_SCREEN_HEIGHT = h;
	activeRender->onResize(w, h);
}

bool Engine::RenderManager::isForwardRendering()
{
	return forwardRendering;
}

Engine::DeferredRenderer * Engine::RenderManager::getCurrentDeferredRenderer()
{
	return deferredRenderer;
}

Engine::ForwardRenderer * Engine::RenderManager::getCurrentForwardRenderer()
{
	return forwardRenderer;
}