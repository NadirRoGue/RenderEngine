#include "renderers/DeferredRenderer.h"

#include <iostream>

#include "Scene.h"
#include "datatables/DeferredObjectsTable.h"
#include "datatables/MeshTable.h"
#include "datatables/ProgramTable.h"

#include "volumetricclouds/NoiseInitializer.h"

Engine::DeferredRenderer::DeferredRenderer()
	:Engine::Renderer()
{
	forwardPass = new Engine::ForwardRenderer();
	initialized = false;

	renderFunc = &DeferredRenderer::initializeLoop;
}

Engine::DeferredRenderer::~DeferredRenderer()
{
}

void Engine::DeferredRenderer::addPostProcess(Engine::PostProcessChainNode * object)
{
	postProcessChain.push_back(object);
}

const Engine::TextureInstance * Engine::DeferredRenderer::getGBufferPos()
{
	return gBufferPos;
}

const Engine::TextureInstance * Engine::DeferredRenderer::getGBufferColor()
{
	return gBufferColor;
}

const Engine::TextureInstance * Engine::DeferredRenderer::getGBufferEmissive()
{
	return gBufferEmissive;
}

const Engine::TextureInstance * Engine::DeferredRenderer::getGBufferNormal()
{
	return gBufferNormal;
}

const Engine::TextureInstance * Engine::DeferredRenderer::getGBufferSpecular()
{
	return gBufferSpecular;
}

const Engine::TextureInstance * Engine::DeferredRenderer::getGBufferDepth()
{
	return gBufferDepth;
}

void Engine::DeferredRenderer::initialize()
{
	Engine::Renderer::initialize();

	if (initialized)
		return;

	initialized = true;

	// Create G Buffers
	forwardPassBuffer = new Engine::DeferredRenderObject(5, true);
	gBufferColor = forwardPassBuffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, Engine::DeferredRenderObject::G_BUFFER_COLOR, GL_NEAREST);
	gBufferNormal = forwardPassBuffer->addColorBuffer(1, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, 500, 500, Engine::DeferredRenderObject::G_BUFFER_NORMAL, GL_NEAREST);
	gBufferSpecular = forwardPassBuffer->addColorBuffer(2, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, Engine::DeferredRenderObject::G_BUFFER_SPECULAR, GL_NEAREST);
	gBufferEmissive = forwardPassBuffer->addColorBuffer(3, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, Engine::DeferredRenderObject::G_BUFFER_EMISSIVE, GL_NEAREST);
	gBufferPos = forwardPassBuffer->addColorBuffer(4, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, 500, 500, Engine::DeferredRenderObject::G_BUFFER_POS, GL_NEAREST);
	gBufferDepth = forwardPassBuffer->addDepthBuffer24(500, 500);
	forwardPassBuffer->initialize();

	// Instantiate deferred shading program
	deferredShading = dynamic_cast<Engine::DeferredShadingProgram*>(Engine::ProgramTable::getInstance().getProgramByName(Engine::DeferredShadingProgram::PROGRAM_NAME));
	Engine::Mesh * mi = Engine::MeshTable::getInstance().getMesh("plane");
	deferredShading->configureMeshBuffers(mi);
	deferredDrawSurface = new Engine::PostProcessObject(mi);
	// Link G-Buffers to deferred shading surface input
	forwardPassBuffer->populateDeferredObject(deferredDrawSurface);

	// Creage deferred shading buffer
	deferredPassBuffer = new Engine::DeferredRenderObject(2, false);
	deferredPassBuffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, "", GL_NEAREST);	// Color info
	deferredPassBuffer->addColorBuffer(1, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, "", GL_NEAREST);	// Emission info
	deferredPassBuffer->addDepthBuffer24(500, 500);
	deferredPassBuffer->initialize();

	clouds = new Engine::VolumetricCloudProgram("VolumetricCloudProgram", 0);
	clouds->initialize();
	clouds->configureMeshBuffers(mi);

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
	(this->*renderFunc)();
}

void Engine::DeferredRenderer::initializeLoop()
{
	Engine::CloudSystem::NoiseInitializer::getInstance().render();

	renderFunc = &Engine::DeferredRenderer::renderLoop;
}

void Engine::DeferredRenderer::renderLoop()
{
	Engine::Scene * scene = Engine::SceneManager::getInstance().getActiveScene();

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

	// Render the skybox after shading is performed
	scene->getSkyBox()->render(activeCam);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(clouds->getProgramId());
	glBindVertexArray(deferredDrawSurface->getMesh()->vao);
	clouds->onRenderObject(deferredDrawSurface, activeCam->getViewMatrix(), activeCam->getProjectionMatrix());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisable(GL_BLEND);

	// Run the post-process chain
	runPostProcesses();
	
	// Enable default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

		//if (node->callBack != 0)
		//{
		//	node->callBack->execute(node->obj, node->postProcessProgram, node->renderBuffer, activeCam);
		//}

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
}