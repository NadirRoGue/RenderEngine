#include "renderers/SideBySideRenderer.h"

#include "renderers/DeferredRenderer.h"
#include "datatables/MeshTable.h"
#include "datatables/ProgramTable.h"

#include "Scene.h"
#include "PostProcessProgram.h"

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

Engine::DeferredRenderObject * Engine::SideBySideRenderer::getGBuffer()
{
	if (leftRenderer != NULL)
	{
		return leftRenderer->getGBuffer();
	}

	return NULL;
}

void Engine::SideBySideRenderer::initialize()
{
	if (initialized)
		return;

	initialized = true;

	Engine::Program * sourcePostProcess = Engine::ProgramTable::getInstance().getProgramByName("post_processing_program");
	Engine::Mesh * miL = Engine::MeshTable::getInstance().getMesh("leftplane");

	leftRenderer->initialize();
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