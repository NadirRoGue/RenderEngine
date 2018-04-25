#include "renderers/SideBySideRenderer.h"

#include "renderers/DeferredRenderer.h"
#include "datatables/MeshInstanceTable.h"
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