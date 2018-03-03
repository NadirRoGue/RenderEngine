/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "KeyboardHandler.h"
#include "PostProcessManager.h"

#include <iostream>

using namespace Engine;

KeyboardHandlersTable * KeyboardHandlersTable::INSTANCE = new KeyboardHandlersTable();

KeyboardHandlersTable & KeyboardHandlersTable::getInstance()
{
	return *INSTANCE;
}

KeyboardHandlersTable::KeyboardHandlersTable()
{
	for (int i = 0; i < 256; i++)
		handlers[i] = 0;
}

KeyboardHandlersTable::~KeyboardHandlersTable()
{
	destroy();
}

void KeyboardHandlersTable::registerHandler(KeyboardHandler *handler)
{
	if (handler == nullptr)
		return;

	std::list<unsigned char> keys = handler->getUsedKeys();
	std::list<unsigned char>::const_iterator cIt = keys.cbegin();
	while (cIt != keys.cend())
	{
		handlers[*cIt] = handler;
		cIt++;
	}
}

void KeyboardHandlersTable::handleKeyPress(unsigned char key, int x, int y)
{
	key = tolower(key);

	KeyboardHandler * handler = handlers[key];
	
	if (handler != 0)
	{
		handler->onKeyPressed(key, x, y);
	}
}

void KeyboardHandlersTable::destroy()
{
	for (int i = 0; i < 256; i++)
	{
		delete handlers[i];
	}
}

// ======================================================================================

KeyboardHandler::KeyboardHandler()
{
}

const std::list<unsigned char> & KeyboardHandler::getUsedKeys() const
{
	return usedKeys;
}

void KeyboardHandler::onKeyPressed(unsigned char key, int x, int y)
{
}

// ======================================================================================

LightMovement::LightMovement(PointLight * light)
{
	lightSource = light;
	usedKeys = { 'g', 'b', 'v', 'n', 'i', 'o' };
}

void LightMovement::onKeyPressed(unsigned char key, int x, int y)
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
	SceneManager::getInstance().getActiveScene()->triggerLightUpdate(lightSource);
}

// ======================================================================================

LightIntensityMod::LightIntensityMod(PointLight * light)
{
	lightSource = light;
	usedKeys = { '+', '-' };
}

void LightIntensityMod::onKeyPressed(unsigned char key, int x, int y)
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
	SceneManager::getInstance().getActiveScene()->triggerLightUpdate(lightSource);
}

// ======================================================================================

CameraMovement::CameraMovement(Camera * cam)
{
	camera = cam;
	usedKeys = { 'w', 'a', 's', 'd' };
}

void CameraMovement::onKeyPressed(unsigned char key, int x, int y)
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

MotionBlurManagement::MotionBlurManagement(MotionBlurImpl * m)
{
	mbi = m;
	usedKeys = { 'e', '1', '2', '3', '4', '5' };
}

void MotionBlurManagement::onKeyPressed(unsigned char key, int x, int y)
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

DepthOfFieldManagement::DepthOfFieldManagement(DepthOfFieldProgram * dof)
{
	dofShader = dof;
	usedKeys = { '6', '7', '8', '9' };
}

void DepthOfFieldManagement::onKeyPressed(unsigned char key, int x, int y)
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


ConvolutionMaskManagement::ConvolutionMaskManagement(GaussianBlurProgram * gauss)
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

void ConvolutionMaskManagement::onKeyPressed(unsigned char key, int x, int y)
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

RendererSwitcher::RendererSwitcher()
{
	usedKeys = { 'f', 'p', 'z', 'x', 'c', 'r', 't', 'y', 'u', 'q'};

	standarFR = RenderManager::getInstance().getCurrentForwardRenderer();
	standarDR = RenderManager::getInstance().getCurrentDeferredRenderer();

	depthDR = DefaultDeferredConfiguration::createDeferredDepthRenderer();
	colorDR = DefaultDeferredConfiguration::createColorRenderer();
	normalDR = DefaultDeferredConfiguration::createNormalRenderer();
	specularDR = DefaultDeferredConfiguration::createSpecularRenderer();
	toonDR = DefaultDeferredConfiguration::createToonShaderRenderer();
	
	initialize3GaussRenderer();
	initializeFullPPRenderer();
	initializeSideBySideRenderer();
}

void RendererSwitcher::onKeyPressed(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'f':
		RenderManager::getInstance().setForwardRenderer(standarFR);
		RenderManager::getInstance().forwardRender();
		break;
	case 'p':
		RenderManager::getInstance().setDeferredRenderer(standarDR);
		RenderManager::getInstance().deferredRender();
		break;
	case 'z':
		RenderManager::getInstance().setDeferredRenderer(depthDR);
		RenderManager::getInstance().deferredRender();
		break;
	case 'x':
		RenderManager::getInstance().setDeferredRenderer(threeGaussFilterDR);
		RenderManager::getInstance().deferredRender();
		break;
	case 'c':
		RenderManager::getInstance().setDeferredRenderer(fullPostProcessDR);
		RenderManager::getInstance().deferredRender();
		break;
	case 'r':
		RenderManager::getInstance().setSideBySideRenderer(sideBySideRenderer);
		RenderManager::getInstance().sideBySideRender();
		break;
	case 't':
		RenderManager::getInstance().setDeferredRenderer(colorDR);
		RenderManager::getInstance().deferredRender();
		break;
	case 'y':
		RenderManager::getInstance().setDeferredRenderer(normalDR);
		RenderManager::getInstance().deferredRender();
		break;
	case 'u':
		RenderManager::getInstance().setDeferredRenderer(specularDR);
		RenderManager::getInstance().deferredRender();
	case 'q':
		RenderManager::getInstance().setDeferredRenderer(toonDR);
		RenderManager::getInstance().deferredRender();
		break;
	}
}

void RendererSwitcher::initializeFullPPRenderer()
{
	fullPostProcessDR = new DeferredRenderer();

	DeferredRenderObject * start = DefaultDeferredConfiguration::createForwardBuffer();

	PostProcessChainNode * dS = DefaultDeferredConfiguration::createDeferredShadingNode();
	PostProcessChainNode * toon = DefaultDeferredConfiguration::createToonShadingNode();
	PostProcessChainNode * dof = DefaultDeferredConfiguration::createDepthOfField(false, false);
	PostProcessChainNode * end = DefaultDeferredConfiguration::createFinalLink(false, false);

	fullPostProcessDR->setForwardPassBuffers(start);
	fullPostProcessDR->addPostProcess(dS);
	fullPostProcessDR->addPostProcess(toon);
	fullPostProcessDR->addPostProcess(dof);
	fullPostProcessDR->setFinalPostProcess(end);
}

void RendererSwitcher::initialize3GaussRenderer()
{
	threeGaussFilterDR = new DeferredRenderer();

	DeferredRenderObject * start = DefaultDeferredConfiguration::createForwardBuffer();

	float maskFactor = 1.0f / 20.0f;
	float tempMask3[] =
	{
		0.0f*maskFactor, 0.0f*maskFactor, 5.0f*maskFactor,0.0f*maskFactor, 0.0f*maskFactor,
		0.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor,0.0f*maskFactor, 0.0f*maskFactor,
		5.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor,0.0f*maskFactor, 5.0f*maskFactor,
		0.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor,0.0f*maskFactor, 0.0f*maskFactor,
		0.0f*maskFactor, 0.0f*maskFactor, 5.0f*maskFactor,0.0f*maskFactor, 0.0f*maskFactor
	};

	PostProcessChainNode * dS = DefaultDeferredConfiguration::createDeferredShadingNode();
	PostProcessChainNode * gauss1 = DefaultDeferredConfiguration::create5x5GaussianBlur(tempMask3, false);
	PostProcessChainNode * gauss2 = DefaultDeferredConfiguration::create5x5GaussianBlur(tempMask3, false);
	PostProcessChainNode * gauss3 = DefaultDeferredConfiguration::create5x5GaussianBlur(tempMask3, false);
	PostProcessChainNode * end = DefaultDeferredConfiguration::createFinalLink(false, false);

	threeGaussFilterDR->setForwardPassBuffers(start);
	threeGaussFilterDR->addPostProcess(dS);
	threeGaussFilterDR->addPostProcess(gauss1);
	threeGaussFilterDR->addPostProcess(gauss2);
	threeGaussFilterDR->addPostProcess(gauss3);
	threeGaussFilterDR->setFinalPostProcess(end);
}

void RendererSwitcher::initializeSideBySideRenderer()
{
	sideBySideRenderer = new SideBySideRenderer();

	DeferredRenderObject * startL = DefaultDeferredConfiguration::createForwardBufferWithDepth();
	DeferredRenderObject * startR = DefaultDeferredConfiguration::createForwardBufferWithDepth();
	PostProcessChainNode * leftAA = DefaultDeferredConfiguration::createScreenAntiAliasing();
	PostProcessChainNode * rightAA = DefaultDeferredConfiguration::createScreenAntiAliasing();
	PostProcessChainNode * dSR = DefaultDeferredConfiguration::createDeferredShadingNode();
	PostProcessChainNode * dSL = DefaultDeferredConfiguration::createDeferredShadingNode();
	PostProcessChainNode * gaussL = DefaultDeferredConfiguration::createDepthOfField(false, false);
	PostProcessChainNode * gaussR = DefaultDeferredConfiguration::createDepthOfField(false, false);

	sideBySideRenderer->setLeftForwardPassBuffer(startL);
	sideBySideRenderer->addLeftPostProcess(dSL);
	sideBySideRenderer->addLeftPostProcess(leftAA);
	sideBySideRenderer->addLeftPostProcess(gaussL);
	sideBySideRenderer->setRightForwardPassBuffer(startR);
	sideBySideRenderer->addRightPostProcess(dSR);
	sideBySideRenderer->addRightPostProcess(rightAA);
	sideBySideRenderer->addRightPostProcess(gaussR);
}