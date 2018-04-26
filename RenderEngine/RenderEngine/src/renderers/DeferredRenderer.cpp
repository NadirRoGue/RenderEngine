#include "renderers/DeferredRenderer.h"

#include <iostream>

#include "Scene.h"

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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	forwardPass->renderFromCamera(cam);
	forwardPass->doRender();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	std::list<Engine::PostProcessChainNode *>::iterator it = postProcessChain.begin();
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
		
		glBindVertexArray(node->obj->getMesh()->vao);

		prog->onRenderObject(node->obj, cam->getViewMatrix(), cam->getProjectionMatrix());

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		it++;
	}
	
	// Enable default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Enable depth test to draw skybox + gbuffers
	glEnable(GL_DEPTH_TEST);

	if (finalLink->callBack != 0)
	{
		finalLink->callBack->execute(finalLink->obj, finalLink->postProcessProgram, finalLink->renderBuffer, cam);
	}

	glUseProgram(finalLink->postProcessProgram->getProgramId());
	glBindVertexArray(finalLink->obj->getMesh()->vao);

	finalLink->postProcessProgram->onRenderObject(finalLink->obj, cam->getViewMatrix(), cam->getProjectionMatrix());

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDepthFunc(GL_LEQUAL);
	scene->getSkyBox()->render(cam);
	glDepthFunc(GL_LESS);
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