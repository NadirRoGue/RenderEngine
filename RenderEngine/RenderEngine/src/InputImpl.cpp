#include "InputImpl.h"

#include "PostProcessManager.h"

Engine::TestImplementation::LightMovement::LightMovement(Engine::PointLight * light)
{
	lightSource = light;
	usedKeys = { 'g', 'b', 'v', 'n', 'i', 'o' };
}

void Engine::TestImplementation::LightMovement::onKeyPressed(unsigned char key, int x, int y)
{
	if (lightSource == NULL)
		return;

	glm::vec3 t;

	switch (key)
	{
	case 'g':  // Move light up
		t = glm::vec3(0.0f, 0.2f, 0.0f);
		break;
	case 'b':  // Move light down
		t = glm::vec3(0.0f, -0.2f, 0.0f);
		break;
	case 'v':  // Move light to the left
		t = glm::vec3(-0.2f, 0.0f, 0.0f);
		break;
	case 'n':  // Move light to the right
		t = glm::vec3(0.2f, 0.0f, 0.0f);
		break;
	case 'i':  // Move light forward (i = inside)
		t = glm::vec3(0.0f, 0.0f, 0.2f);
		break;
	case 'o':  // Move light backward (o = outside)
		t = glm::vec3(0.0f, 0.0f, -0.2f);
		break;
	}

	lightSource->translate(t);
	Engine::SceneManager::getInstance().getActiveScene()->triggerLightUpdate(lightSource);
}

// ======================================================================================

Engine::TestImplementation::LightIntensityMod::LightIntensityMod(Engine::PointLight * light)
{
	lightSource = light;
	usedKeys = { '+', '-' };
}

void Engine::TestImplementation::LightIntensityMod::onKeyPressed(unsigned char key, int x, int y)
{
	if (lightSource == NULL)
		return;

	float r = 0.0f, g = 0.0f, b = 0.0f;

	switch (key)
	{
	case '+':  // Light up
		r = g = b = 0.1f;
		break;
	case '-':  // Light down
		r = g = b = -0.1f;
		break;
	}

	lightSource->modifyIntensity(r, g, b);
	Engine::SceneManager::getInstance().getActiveScene()->triggerLightUpdate(lightSource);
}

// ======================================================================================

Engine::TestImplementation::CameraMovement::CameraMovement(Engine::Camera * cam)
{
	camera = cam;
	usedKeys = { 'w', 'a', 's', 'd' };
}

void Engine::TestImplementation::CameraMovement::onKeyPressed(unsigned char key, int x, int y)
{
	if (camera == nullptr)
		return;

	glm::vec3 t;
	switch (key)
	{
	case 'w':	// Forward
		t = glm::vec3(0.0f, 0.0f, 0.2f);
		break;
	case 's':	// Backward
		t = glm::vec3(0.0f, 0.0f, -0.2f);
		break;
	case 'a':	// Left
		t = glm::vec3(0.2f, 0.0f, 0.0f);
		break;
	case 'd':	// Right
		t = glm::vec3(-0.2f, 0.0f, 0.0f);
		break;
	}

	camera->translateView(t);
}

// ======================================================================================

Engine::TestImplementation::MotionBlurManagement::MotionBlurManagement(Engine::MotionBlurImpl * m)
{
	mbi = m;
	usedKeys = { 'e', '1', '2', '3', '4', '5' };
}

void Engine::TestImplementation::MotionBlurManagement::onKeyPressed(unsigned char key, int x, int y)
{
	if (mbi == NULL)
		return;

	glm::vec4 stats = mbi->getMotionBlurStats();
	float alpha = stats.w;
	switch (key)
	{
	case 'e':
		mbi->switchEnabled();
		break;
	case '1':
		alpha += 0.1f;
		alpha = glm::clamp(alpha, 0.0f, 1.0f);
		mbi->setAlpha(alpha);
		break;
	case '2':
		alpha -= 0.1f;
		alpha = glm::clamp(alpha, 0.0f, 1.0f);
		mbi->setAlpha(alpha);
		break;
	case '3':
		if (stats.r == 0.5f)
			mbi->setColorMask(0.0f, stats.g, stats.b);
		else
			mbi->setColorMask(0.5f, stats.g, stats.b);
		break;
	case '4':
		if (stats.g == 0.5f)
			mbi->setColorMask(stats.r, 0.0f, stats.b);
		else
			mbi->setColorMask(stats.r, 0.5f, stats.b);
		break;
	case '5':
		if (stats.b == 0.5f)
			mbi->setColorMask(stats.r, stats.g, 0.0f);
		else
			mbi->setColorMask(stats.r, stats.g, 0.5f);
		break;
	}
}

// ======================================================================================

Engine::TestImplementation::DepthOfFieldManagement::DepthOfFieldManagement(Engine::DepthOfFieldProgram * dof)
{
	dofShader = dof;
	usedKeys = { '6', '7', '8', '9' };
}

void Engine::TestImplementation::DepthOfFieldManagement::onKeyPressed(unsigned char key, int x, int y)
{
	if (dofShader == NULL)
		return;

	switch (key)
	{
	case '6':
		dofShader->setFocalDistance(dofShader->getFocalDistance() + 1.0f);
		break;
	case '7':
		dofShader->setFocalDistance(dofShader->getFocalDistance() - 1.0f);
		break;
	case '8':
		dofShader->setMaxDistanceFactor(dofShader->getMaxDistanceFactor() + 0.1f);
		break;
	case '9':
		dofShader->setMaxDistanceFactor(dofShader->getMaxDistanceFactor() - 0.1f);
		break;
	}
}

// ======================================================================================


Engine::TestImplementation::ConvolutionMaskManagement::ConvolutionMaskManagement(Engine::GaussianBlurProgram * gauss)
{
	gaussShader = gauss;

	maskSize = 25;

	glm::vec2 tempTexels[] =
	{
		glm::vec2(-2.0,2.0f), glm::vec2(-1.0,2.0f), glm::vec2(0.0,2.0f), glm::vec2(1.0,2.0f), glm::vec2(2.0,2.0),
		glm::vec2(-2.0,1.0f), glm::vec2(-1.0,1.0f), glm::vec2(0.0,1.0f), glm::vec2(1.0,1.0f), glm::vec2(2.0,1.0),
		glm::vec2(-2.0,0.0f), glm::vec2(-1.0,0.0f), glm::vec2(0.0,0.0f), glm::vec2(1.0,0.0f), glm::vec2(2.0,0.0),
		glm::vec2(-2.0,-1.0f), glm::vec2(-1.0,-1.0f), glm::vec2(0.0,-1.0f), glm::vec2(1.0,-1.0f), glm::vec2(2.0,-1.0),
		glm::vec2(-2.0,-2.0f), glm::vec2(-1.0,-2.0f), glm::vec2(0.0,-2.0f), glm::vec2(1.0,-2.0f), glm::vec2(2.0,-2.0)
	};
	memcpy(affectedTexels, tempTexels, sizeof(glm::vec2) * 25);

	maskFactor = 1.0f / 65.0f;
	float tempMask1[] =
	{
		1.0f*maskFactor, 2.0f*maskFactor, 3.0f*maskFactor,2.0f*maskFactor, 1.0f*maskFactor,
		2.0f*maskFactor, 3.0f*maskFactor, 4.0f*maskFactor,3.0f*maskFactor, 2.0f*maskFactor,
		3.0f*maskFactor, 4.0f*maskFactor, 5.0f*maskFactor,4.0f*maskFactor, 3.0f*maskFactor,
		2.0f*maskFactor, 3.0f*maskFactor, 4.0f*maskFactor,3.0f*maskFactor, 2.0f*maskFactor,
		1.0f*maskFactor, 2.0f*maskFactor, 3.0f*maskFactor,2.0f*maskFactor, 1.0f*maskFactor
	};
	memcpy(&convolutionMask1[0], &tempMask1[0], sizeof(float) * 25);

	maskFactor = 1.0f;
	float tempMask2[] =
	{
		0.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor,0.0f*maskFactor, 0.0f*maskFactor,
		0.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor,0.0f*maskFactor, 0.0f*maskFactor,
		0.0f*maskFactor, 0.0f*maskFactor, 1.0f*maskFactor,0.0f*maskFactor, 0.0f*maskFactor,
		0.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor,0.0f*maskFactor, 0.0f*maskFactor,
		0.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor,0.0f*maskFactor, 0.0f*maskFactor
	};
	memcpy(&convolutionMask2[0], &tempMask2[0], sizeof(float) * 25);

	maskFactor = 1.0f / 20.0f;
	float tempMask3[] =
	{
		0.0f*maskFactor, 0.0f*maskFactor, 5.0f*maskFactor,0.0f*maskFactor, 0.0f*maskFactor,
		0.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor,0.0f*maskFactor, 0.0f*maskFactor,
		5.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor,0.0f*maskFactor, 5.0f*maskFactor,
		0.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor,0.0f*maskFactor, 0.0f*maskFactor,
		0.0f*maskFactor, 0.0f*maskFactor, 5.0f*maskFactor,0.0f*maskFactor, 0.0f*maskFactor
	};
	memcpy(&convolutionMask3[0], &tempMask3[0], sizeof(float) * 25);

	usedKeys = { 'h', 'j', 'k' };
}

void Engine::TestImplementation::ConvolutionMaskManagement::onKeyPressed(unsigned char key, int x, int y)
{
	if (gaussShader == NULL)
	{
		return;
	}

	switch (key)
	{
	case 'h':
		gaussShader->setMaskSize(maskSize);
		gaussShader->setAffectedTexels(affectedTexels);
		gaussShader->setKernel(&convolutionMask1[0]);
		break;
	case 'j':
		gaussShader->setMaskSize(maskSize);
		gaussShader->setAffectedTexels(affectedTexels);
		gaussShader->setKernel(&convolutionMask2[0]);
		break;
	case 'k':
		gaussShader->setMaskSize(maskSize);
		gaussShader->setAffectedTexels(affectedTexels);
		gaussShader->setKernel(&convolutionMask3[0]);
		break;
	}
}

// ======================================================================================

Engine::TestImplementation::RendererSwitcher::RendererSwitcher()
{
	usedKeys = { 'f', 'p', 'z', 'x', 'c', 'r', 't', 'y', 'u', 'q' };

	standarFR = RenderManager::getInstance().getCurrentForwardRenderer();
	standarDR = RenderManager::getInstance().getCurrentDeferredRenderer();

	depthDR = Engine::DefaultDeferredConfiguration::createDeferredDepthRenderer();
	colorDR = Engine::DefaultDeferredConfiguration::createColorRenderer();
	normalDR = Engine::DefaultDeferredConfiguration::createNormalRenderer();
	specularDR = Engine::DefaultDeferredConfiguration::createSpecularRenderer();
	toonDR = Engine::DefaultDeferredConfiguration::createToonShaderRenderer();

	initialize3GaussRenderer();
	initializeFullPPRenderer();
	initializeSideBySideRenderer();
}

void Engine::TestImplementation::RendererSwitcher::onKeyPressed(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'f':
		Engine::RenderManager::getInstance().setForwardRenderer(standarFR);
		Engine::RenderManager::getInstance().forwardRender();
		break;
	case 'p':
		Engine::RenderManager::getInstance().setDeferredRenderer(standarDR);
		Engine::RenderManager::getInstance().deferredRender();
		break;
	case 'z':
		Engine::RenderManager::getInstance().setDeferredRenderer(depthDR);
		Engine::RenderManager::getInstance().deferredRender();
		break;
	case 'x':
		Engine::RenderManager::getInstance().setDeferredRenderer(threeGaussFilterDR);
		Engine::RenderManager::getInstance().deferredRender();
		break;
	case 'c':
		Engine::RenderManager::getInstance().setDeferredRenderer(fullPostProcessDR);
		Engine::RenderManager::getInstance().deferredRender();
		break;
	case 'r':
		Engine::RenderManager::getInstance().setSideBySideRenderer(sideBySideRenderer);
		Engine::RenderManager::getInstance().sideBySideRender();
		break;
	case 't':
		Engine::RenderManager::getInstance().setDeferredRenderer(colorDR);
		Engine::RenderManager::getInstance().deferredRender();
		break;
	case 'y':
		Engine::RenderManager::getInstance().setDeferredRenderer(normalDR);
		Engine::RenderManager::getInstance().deferredRender();
		break;
	case 'u':
		Engine::RenderManager::getInstance().setDeferredRenderer(specularDR);
		Engine::RenderManager::getInstance().deferredRender();
	case 'q':
		Engine::RenderManager::getInstance().setDeferredRenderer(toonDR);
		Engine::RenderManager::getInstance().deferredRender();
		break;
	}
}

void Engine::TestImplementation::RendererSwitcher::initializeFullPPRenderer()
{
	fullPostProcessDR = new Engine::DeferredRenderer();

	Engine::DeferredRenderObject * start = Engine::DefaultDeferredConfiguration::createForwardBuffer();

	Engine::PostProcessChainNode * dS = Engine::DefaultDeferredConfiguration::createDeferredShadingNode();
	Engine::PostProcessChainNode * toon = Engine::DefaultDeferredConfiguration::createToonShadingNode();
	Engine::PostProcessChainNode * dof = Engine::DefaultDeferredConfiguration::createDepthOfField(false, false);
	Engine::PostProcessChainNode * end = Engine::DefaultDeferredConfiguration::createFinalLink(false, false);

	fullPostProcessDR->setForwardPassBuffers(start);
	fullPostProcessDR->addPostProcess(dS);
	fullPostProcessDR->addPostProcess(toon);
	fullPostProcessDR->addPostProcess(dof);
	fullPostProcessDR->setFinalPostProcess(end);
}

void Engine::TestImplementation::RendererSwitcher::initialize3GaussRenderer()
{
	threeGaussFilterDR = new Engine::DeferredRenderer();

	Engine::DeferredRenderObject * start = Engine::DefaultDeferredConfiguration::createForwardBuffer();

	float maskFactor = 1.0f / 20.0f;
	float tempMask3[] =
	{
		0.0f*maskFactor, 0.0f*maskFactor, 5.0f*maskFactor,0.0f*maskFactor, 0.0f*maskFactor,
		0.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor,0.0f*maskFactor, 0.0f*maskFactor,
		5.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor,0.0f*maskFactor, 5.0f*maskFactor,
		0.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor,0.0f*maskFactor, 0.0f*maskFactor,
		0.0f*maskFactor, 0.0f*maskFactor, 5.0f*maskFactor,0.0f*maskFactor, 0.0f*maskFactor
	};

	Engine::PostProcessChainNode * dS = Engine::DefaultDeferredConfiguration::createDeferredShadingNode();
	Engine::PostProcessChainNode * gauss1 = Engine::DefaultDeferredConfiguration::create5x5GaussianBlur(tempMask3, false);
	Engine::PostProcessChainNode * gauss2 = Engine::DefaultDeferredConfiguration::create5x5GaussianBlur(tempMask3, false);
	Engine::PostProcessChainNode * gauss3 = Engine::DefaultDeferredConfiguration::create5x5GaussianBlur(tempMask3, false);
	Engine::PostProcessChainNode * end = Engine::DefaultDeferredConfiguration::createFinalLink(false, false);

	threeGaussFilterDR->setForwardPassBuffers(start);
	threeGaussFilterDR->addPostProcess(dS);
	threeGaussFilterDR->addPostProcess(gauss1);
	threeGaussFilterDR->addPostProcess(gauss2);
	threeGaussFilterDR->addPostProcess(gauss3);
	threeGaussFilterDR->setFinalPostProcess(end);
}

void Engine::TestImplementation::RendererSwitcher::initializeSideBySideRenderer()
{
	sideBySideRenderer = new Engine::SideBySideRenderer();

	Engine::DeferredRenderObject * startL = DefaultDeferredConfiguration::createForwardBufferWithDepth();
	Engine::DeferredRenderObject * startR = DefaultDeferredConfiguration::createForwardBufferWithDepth();
	Engine::PostProcessChainNode * leftAA = DefaultDeferredConfiguration::createScreenAntiAliasing();
	Engine::PostProcessChainNode * rightAA = DefaultDeferredConfiguration::createScreenAntiAliasing();
	Engine::PostProcessChainNode * dSR = DefaultDeferredConfiguration::createDeferredShadingNode();
	Engine::PostProcessChainNode * dSL = DefaultDeferredConfiguration::createDeferredShadingNode();
	Engine::PostProcessChainNode * gaussL = DefaultDeferredConfiguration::createDepthOfField(false, false);
	Engine::PostProcessChainNode * gaussR = DefaultDeferredConfiguration::createDepthOfField(false, false);

	sideBySideRenderer->setLeftForwardPassBuffer(startL);
	sideBySideRenderer->addLeftPostProcess(dSL);
	sideBySideRenderer->addLeftPostProcess(leftAA);
	sideBySideRenderer->addLeftPostProcess(gaussL);
	sideBySideRenderer->setRightForwardPassBuffer(startR);
	sideBySideRenderer->addRightPostProcess(dSR);
	sideBySideRenderer->addRightPostProcess(rightAA);
	sideBySideRenderer->addRightPostProcess(gaussR);
}

// ====================================================================

Engine::TestImplementation::CameraMotion::CameraMotion(std::string nam, Engine::Camera * cam)
	:MouseMotionHandler(nam)
{
	camera = cam;
	previousX = previousY = -1;

	horizontalSpeed = 0.01f;
	verticalSpeed = 0.01f;

	usedButtons = { 0 };
}

void Engine::TestImplementation::CameraMotion::handleMotion(int x, int y)
{
	if (camera != nullptr)
	{
		if (previousX == -1 && previousY == -1)
		{
			previousX = x;
			previousY = y;
		}
		else
		{
			int deltaX = x - previousX;
			int deltaY = y - previousY;

			previousX = x;
			previousY = y;

			float hAngle = deltaX * horizontalSpeed;
			float vAngle = deltaY * verticalSpeed;

			camera->rotateView(glm::vec3(vAngle, hAngle, 0.0f));
		}
	}
}

void Engine::TestImplementation::CameraMotion::finishMotion()
{
	previousX = previousY = -1;
}