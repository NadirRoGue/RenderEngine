/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "Renderer.h"
#include "MeshInstanceTable.h"
#include "ProgramTable.h"
#include "PostProcessProgram.h"

#include <iostream>

using namespace Engine;

unsigned int ScreenManager::SCREEN_HEIGHT = 500;
unsigned int ScreenManager::SCREEN_WIDTH = 500;

unsigned int ScreenManager::REAL_SCREEN_HEIGHT = 500;
unsigned int ScreenManager::REAL_SCREEN_WIDTH = 500;

// ==================================================================

Renderer::Renderer()
{
	activeCam = 0;
}

Renderer::~Renderer()
{
}

void Renderer::initialize()
{
	renderFromCamera(SceneManager::getInstance().getActiveScene()->getCamera());
}

void Renderer::renderFromCamera(Camera * cam)
{
	activeCam = cam;
}

// ==================================================================

ForwardRenderer::ForwardRenderer()
	:Renderer()
{
}

ForwardRenderer::~ForwardRenderer()
{
}

void ForwardRenderer::doRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Scene * scene = SceneManager::getInstance().getActiveScene();

	if (scene == 0)
		return;

	Camera * camera = activeCam;

	const std::map<std::string, ProgramRenderables *> renders = scene->getObjects();

	const std::map<std::string, PointLight *> pointLights = scene->getPointLights();
	const std::map<std::string, SpotLight *> spotLights = scene->getSpotLights();
	const std::map<std::string, DirectionalLight *> directionalLights = scene->getDirectionalLight();

	std::map<std::string, ProgramRenderables *>::const_iterator renderableIt;
	for (renderableIt = renders.cbegin(); renderableIt != renders.cend(); renderableIt++)
	{
		// Stablish program to use
		// changing program is expensive -> https://www.opengl.org/discussion_boards/showthread.php/185615-cheep-expensive-calls
		glUseProgram(renderableIt->second->program->getProgramId());

		if (RenderManager::getInstance().isForwardRendering())
		{
			std::map<std::string, PointLight *>::const_iterator it = pointLights.cbegin();
			while (it != pointLights.end())
			{
				renderableIt->second->program->onRenderLight(it->second->getModelMatrix(), camera->getViewMatrix());
				it++;
			}

			std::map<std::string, SpotLight *>::const_iterator slIt = spotLights.cbegin();
			while (slIt != spotLights.end())
			{
				renderableIt->second->program->onRenderSpotLight(slIt->second->getModelMatrix(), slIt->second->getDirModelMatrix(), camera->getViewMatrix());
				slIt++;
			}

			std::map<std::string, DirectionalLight *>::const_iterator dlIt = directionalLights.cbegin();
			while (dlIt != directionalLights.end())
			{
				renderableIt->second->program->onRenderDirectionalLight(dlIt->second->getModelMatrix(), camera->getViewMatrix());
				dlIt++;
			}
		}

		renderProgram(camera, renderableIt->second);
	}
}

void ForwardRenderer::renderProgram(Camera * camera, ProgramRenderables * renderables)
{
	glm::mat4 & viewMatrix = camera->getViewMatrix();
	glm::mat4 & projMatrix = camera->getProjectionMatrix();

	Program * program = renderables->program;

	std::map<unsigned int, std::list<Object *>>::const_iterator it;
	for (it = renderables->objects.cbegin(); it != renderables->objects.cend(); it++)
	{
		// Stablish vao to use
		glBindVertexArray(it->first);

		std::list<Object *> meshes = it->second;
		std::list<Object *>::iterator listIt;

		// foreach texture change (set a texture and render all object with that texture)

		for (listIt = meshes.begin(); listIt != meshes.end(); listIt++)
		{
			program->onRenderObject(*listIt, viewMatrix, projMatrix);

			glDrawElements(GL_TRIANGLES, (*listIt)->getMesh()->getNumFaces() * 3, GL_UNSIGNED_INT, (void*)0);
		}
	}
}

void ForwardRenderer::onResize(unsigned int w, unsigned int h)
{

}

// ==================================================================

DeferredRenderer::DeferredRenderer()
	:Renderer()
{
	forwardPass = new ForwardRenderer();
	initialized = false;
}

DeferredRenderer::~DeferredRenderer()
{
	if (forwardPass != 0)
	{
		delete forwardPass;
	}
}

void DeferredRenderer::setForwardPassBuffers(DeferredRenderObject * buffers)
{
	forwardPassBuffer = buffers;
}

void DeferredRenderer::addPostProcess(PostProcessChainNode * object)
{
	postProcessChain.push_back(object);
}

void DeferredRenderer::setFinalPostProcess(PostProcessChainNode * object)
{
	finalLink = object;
}

void DeferredRenderer::initialize()
{
	Renderer::initialize();

	if (initialized)
		return;

	initialized = true;

	forwardPassBuffer->initialize();

	std::list<PostProcessChainNode *>::iterator it = postProcessChain.begin();
	DeferredRenderObject * previousLink = forwardPassBuffer;
	while (it != postProcessChain.end())
	{
		PostProcessChainNode * node = (*it);
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

void DeferredRenderer::doRender()
{
	Scene * scene = SceneManager::getInstance().getActiveScene();

	if (scene == 0)
		return;

	Camera * cam = activeCam;

	glBindFramebuffer(GL_FRAMEBUFFER, forwardPassBuffer->getFrameBufferId());
	
	forwardPass->renderFromCamera(cam);
	forwardPass->doRender();

	//glDisable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);
	std::list<PostProcessChainNode *>::iterator it = postProcessChain.begin();
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	while (it != postProcessChain.end())
	{
		PostProcessChainNode * node = (*it);

		DeferredRenderObject * buffer = node->renderBuffer;
		glBindFramebuffer(GL_FRAMEBUFFER, buffer->getFrameBufferId());

		Program * prog = node->postProcessProgram;
		glUseProgram(prog->getProgramId());

		if (node->callBack != 0)
		{
			node->callBack->execute(node->obj, node->postProcessProgram, node->renderBuffer, cam);
		}

		glBindVertexArray(node->obj->getMeshInstance()->vao);

		prog->onRenderObject(node->obj, cam->getViewMatrix(), cam->getProjectionMatrix());

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		it++;
	}

	// Enable default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (finalLink->callBack != 0)
	{
		finalLink->callBack->execute(finalLink->obj, finalLink->postProcessProgram, finalLink->renderBuffer, cam);
	}

	glUseProgram(finalLink->postProcessProgram->getProgramId());
	glBindVertexArray(finalLink->obj->getMeshInstance()->vao);

	finalLink->postProcessProgram->onRenderObject(finalLink->obj, cam->getViewMatrix(), cam->getProjectionMatrix());

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void DeferredRenderer::onResize(unsigned int w, unsigned int h)
{
	forwardPassBuffer->resizeFBO(w, h);

	std::list<PostProcessChainNode *>::iterator it = postProcessChain.begin();
	while (it != postProcessChain.end())
	{
		(*it)->renderBuffer->resizeFBO(w, h);
		it++;
	}
}

// =========================================================================

SideBySideRenderer::SideBySideRenderer()
	:Renderer()
{
	leftRenderer = new DeferredRenderer();
	rightRenderer = new DeferredRenderer();
	initialized = false;
	clearScreen = true;
	c = 0;
}

SideBySideRenderer::~SideBySideRenderer()
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

void SideBySideRenderer::setLeftForwardPassBuffer(DeferredRenderObject * buffer)
{
	leftRenderer->setForwardPassBuffers(buffer);
}

void SideBySideRenderer::setRightForwardPassBuffer(DeferredRenderObject * buffer)
{
	rightRenderer->setForwardPassBuffers(buffer);
}

void SideBySideRenderer::addLeftPostProcess(PostProcessChainNode * node)
{
	leftRenderer->addPostProcess(node);
}

void SideBySideRenderer::addRightPostProcess(PostProcessChainNode * node)
{
	rightRenderer->addPostProcess(node);
}

void SideBySideRenderer::setClearScreen(bool val)
{
	clearScreen = val;
}

void SideBySideRenderer::initialize()
{
	if (initialized)
		return;

	initialized = true;

	Program * sourcePostProcess = ProgramTable::getInstance().getProgramByName("post_processing_program");
	MeshInstance * mi = MeshInstanceTable::getInstance().getMeshInstance("left_plane", "post_processing_program");

	PostProcessChainNode * lNode = new PostProcessChainNode;
	lNode->postProcessProgram = new PostProcessProgram(*dynamic_cast<PostProcessProgram*>(sourcePostProcess));
	lNode->renderBuffer = 0;
	lNode->callBack = new MotionBlurImpl();
	lNode->obj = new Object(mi);

	leftRenderer->setFinalPostProcess(lNode);
	leftRenderer->initialize();

	MeshInstance * miR = MeshInstanceTable::getInstance().getMeshInstance("right_plane", "post_processing_program");
	PostProcessChainNode * rNode = new PostProcessChainNode;
	rNode->postProcessProgram = new PostProcessProgram(*dynamic_cast<PostProcessProgram*>(sourcePostProcess));
	rNode->renderBuffer = 0;
	rNode->callBack = new MotionBlurImpl();
	rNode->obj = new Object(miR);
	
	rightRenderer->setFinalPostProcess(rNode);
	rightRenderer->initialize();
}

void SideBySideRenderer::doRender()
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

	Camera * leftCam = SceneManager::getInstance().getActiveScene()->getCamera();

	Camera rightCamera(*leftCam);
	rightCamera.translateView(glm::vec3(-0.4f, 0.0f, 0.0f));

	leftRenderer->renderFromCamera(leftCam);
	leftRenderer->doRender();

	rightRenderer->renderFromCamera(&rightCamera);
	rightRenderer->doRender();
}

void SideBySideRenderer::onResize(unsigned int w, unsigned int h)
{
	leftRenderer->onResize(w, h);
	rightRenderer->onResize(w, h);
}

// =========================================================================
// =========================================================================

RenderManager * RenderManager::INSTANCE = new RenderManager();

RenderManager & RenderManager::getInstance()
{
	return *INSTANCE;
}

RenderManager::RenderManager()
{
	forwardRenderer = 0;
	deferredRenderer = 0;
	sideBySideRenderer = 0;
	activeRender = 0;
}

RenderManager::~RenderManager()
{
	if (forwardRenderer != 0)
		delete forwardRenderer;

	if (deferredRenderer != 0)
		delete deferredRenderer;

	activeRender = 0;
}

void RenderManager::doRender()
{
	activeRender->doRender();
}

void RenderManager::forwardRender()
{
	if (forwardRenderer != 0)
	{
		activeRender = forwardRenderer;
		forwardRendering = true;
	}
}

void RenderManager::deferredRender()
{
	if (deferredRenderer != 0)
	{
		activeRender = deferredRenderer;
		forwardRendering = false;
	}
}

void RenderManager::sideBySideRender()
{
	if (sideBySideRenderer != 0)
	{
		sideBySideRenderer->setClearScreen(true);
		activeRender = sideBySideRenderer;
		forwardRendering = false;
	}
}

void RenderManager::setForwardRenderer(ForwardRenderer * fr)
{
	forwardRenderer = fr;
	forwardRenderer->initialize();
	forwardRenderer->onResize(ScreenManager::REAL_SCREEN_WIDTH, ScreenManager::REAL_SCREEN_HEIGHT);
}

void RenderManager::setDeferredRenderer(DeferredRenderer * dr)
{
	deferredRenderer = dr;
	deferredRenderer->initialize();
	deferredRenderer->onResize(ScreenManager::REAL_SCREEN_WIDTH, ScreenManager::REAL_SCREEN_HEIGHT);
}

void RenderManager::setSideBySideRenderer(SideBySideRenderer * renderer)
{
	sideBySideRenderer = renderer;
	sideBySideRenderer->initialize();
	sideBySideRenderer->onResize(ScreenManager::REAL_SCREEN_WIDTH, ScreenManager::REAL_SCREEN_HEIGHT);
}

void RenderManager::doResize(unsigned int w, unsigned int h)
{
	ScreenManager::SCREEN_WIDTH = ScreenManager::REAL_SCREEN_WIDTH = w;
	ScreenManager::SCREEN_HEIGHT = ScreenManager::REAL_SCREEN_HEIGHT = h;
	activeRender->onResize(w, h);
}

bool RenderManager::isForwardRendering()
{
	return forwardRendering;
}

DeferredRenderer * RenderManager::getCurrentDeferredRenderer()
{
	return deferredRenderer;
}

ForwardRenderer * RenderManager::getCurrentForwardRenderer()
{
	return forwardRenderer;
}