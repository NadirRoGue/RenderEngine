#include "renderers/DeferredRenderer.h"

#include <iostream>

#include "Scene.h"
#include "datatables/DeferredObjectsTable.h"
#include "datatables/MeshTable.h"
#include "datatables/ProgramTable.h"

Engine::DeferredRenderer::DeferredRenderer()
	:Engine::Renderer()
{
	forwardPass = new Engine::ForwardRenderer();
	initialized = false;
}

Engine::DeferredRenderer::~DeferredRenderer()
{
}

void Engine::DeferredRenderer::addPostProcess(Engine::PostProcessChainNode * object)
{
	postProcessChain.push_back(object);
}

Engine::DeferredRenderObject * Engine::DeferredRenderer::getGBuffer()
{
	return forwardPassBuffer;
}

void Engine::DeferredRenderer::initialize()
{
	Engine::Renderer::initialize();

	if (initialized)
		return;

	initialized = true;

	// Create G Buffers
	forwardPassBuffer = new Engine::DeferredRenderObject(5, true);
	forwardPassBuffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, Engine::DeferredRenderObject::G_BUFFER_COLOR, GL_LINEAR);
	forwardPassBuffer->addColorBuffer(1, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, 500, 500, Engine::DeferredRenderObject::G_BUFFER_NORMAL, GL_LINEAR);
	forwardPassBuffer->addColorBuffer(2, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, Engine::DeferredRenderObject::G_BUFFER_SPECULAR, GL_LINEAR);
	forwardPassBuffer->addColorBuffer(3, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, Engine::DeferredRenderObject::G_BUFFER_EMISSIVE, GL_LINEAR);
	forwardPassBuffer->addColorBuffer(4, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, 500, 500, Engine::DeferredRenderObject::G_BUFFER_POS, GL_LINEAR);
	forwardPassBuffer->addDepthBuffer24(500, 500);
	forwardPassBuffer->initialize();

	// Instantiate deferred shading program
	deferredShading = dynamic_cast<Engine::DeferredShadingProgram*>(Engine::ProgramTable::getInstance().getProgramByName(Engine::DeferredShadingProgram::PROGRAM_NAME));
	Engine::Mesh * mi = Engine::MeshTable::getInstance().getMesh("plane");
	deferredShading->configureMeshBuffers(mi);
	deferredDrawSurface = new Engine::PostProcessObject(mi);
	// Link G-Buffers to deferred shading surface input
	forwardPassBuffer->populateDeferredObject(deferredDrawSurface);

	// Creage deferred shading buffer
	deferredPassBuffer = new Engine::DeferredRenderObject(3, true);
	deferredPassBuffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, "", GL_LINEAR);	// Color info
	deferredPassBuffer->addColorBuffer(1, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, "", GL_LINEAR);	// Depth info
	deferredPassBuffer->addColorBuffer(2, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, "", GL_LINEAR);	// Emission info
	deferredPassBuffer->addDepthBuffer24(500, 500);
	deferredPassBuffer->initialize();

	// Linke post processes as a chain
	std::list<Engine::PostProcessChainNode *>::iterator it = postProcessChain.begin();
	Engine::DeferredRenderObject * previousLink = deferredPassBuffer;
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

	// Create buffers to screen shader
	screenOutput = dynamic_cast<Engine::PostProcessProgram*>(Engine::ProgramTable::getInstance().getProgramByName(Engine::PostProcessProgram::PROGRAM_NAME));
	screenOutput->configureMeshBuffers(mi);
	chainEnd = new Engine::PostProcessObject(mi);

	// Close the final link (will output to screen)
	previousLink->populateDeferredObject(chainEnd);
}

void Engine::DeferredRenderer::doRender()
{
	Engine::Scene * scene = Engine::SceneManager::getInstance().getActiveScene();

	if (scene == 0)
		return;

	// Do forward pass
	glBindFramebuffer(GL_FRAMEBUFFER, forwardPassBuffer->getFrameBufferId());
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	forwardPass->renderFromCamera(activeCam);
	forwardPass->doRender();
	
	// Do deferred shading pass
	glDisable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, deferredPassBuffer->getFrameBufferId());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(deferredShading->getProgramId());
	glBindVertexArray(deferredDrawSurface->getMesh()->vao);
	deferredShading->onRenderObject(deferredDrawSurface, activeCam->getViewMatrix(), activeCam->getProjectionMatrix());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	if (scene->getSkyBox() != NULL)
	{
		glDepthFunc(GL_LEQUAL);
		scene->getSkyBox()->render(activeCam);
		glDepthFunc(GL_LESS);
	}
	// Run the post-process chain
	runPostProcesses();

	// Enable default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Output the final result to screen
	glUseProgram(screenOutput->getProgramId());
	glBindVertexArray(chainEnd->getMesh()->vao);
	screenOutput->onRenderObject(chainEnd, activeCam->getViewMatrix(), activeCam->getProjectionMatrix());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Engine::DeferredRenderer::runPostProcesses()
{
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
			node->callBack->execute(node->obj, node->postProcessProgram, node->renderBuffer, activeCam);
		}

		glBindVertexArray(node->obj->getMesh()->vao);

		prog->onRenderObject(node->obj, activeCam->getViewMatrix(), activeCam->getProjectionMatrix());

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		it++;
	}
	glEnable(GL_DEPTH_TEST);
}

void Engine::DeferredRenderer::onResize(unsigned int w, unsigned int h)
{
	Engine::DeferredObjectsTable::getInstance().onResize(int(w), int(h));
	/*
	forwardPassBuffer->resizeFBO(w, h);

	std::list<Engine::PostProcessChainNode *>::iterator it = postProcessChain.begin();
	while (it != postProcessChain.end())
	{
		(*it)->renderBuffer->resizeFBO(w, h);
		it++;
	}
	*/
}