/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "PostProcessManager.h"

#include "ProgramTable.h"
#include "MeshInstanceTable.h"
#include "Object.h"
#include "PostProcessProgram.h"
#include "DeferredNodeCallbacks.h"
#include "KeyboardHandler.h"

#include <iostream>

using namespace Engine;
using namespace Engine::DefaultDeferredConfiguration;

DeferredRenderer * Engine::DefaultDeferredConfiguration::createDeferredRendererWithDoF()
{
	DeferredRenderer * renderer = new DeferredRenderer();

	DeferredRenderObject * start = createForwardBufferWithDepth();
	PostProcessChainNode * dS = createDeferredShadingNode();
	PostProcessChainNode * AA = createScreenAntiAliasing();
	PostProcessChainNode * gauss = createDepthOfField(true, true);
	PostProcessChainNode * end = createFinalLink(true, true);

	renderer->setForwardPassBuffers(start);
	renderer->addPostProcess(dS);
	renderer->addPostProcess(AA);
	renderer->addPostProcess(gauss);
	renderer->setFinalPostProcess(end);

	return renderer;
}

DeferredRenderer * Engine::DefaultDeferredConfiguration::createDeferredDepthRenderer()
{
	DeferredRenderer * renderer = new DeferredRenderer();

	DeferredRenderObject * forward = createDepthOnlyBuffer();
	PostProcessChainNode * end = createDepthRenderNode();

	renderer->setForwardPassBuffers(forward);
	renderer->setFinalPostProcess(end);

	return renderer;
}

DeferredRenderer * Engine::DefaultDeferredConfiguration::createNormalRenderer()
{
	DeferredRenderer * renderer = new DeferredRenderer();

	DeferredRenderObject * forward = createForwardBufferWithDepth();
	PostProcessChainNode * end = createNormalRenderNode();

	renderer->setForwardPassBuffers(forward);
	renderer->setFinalPostProcess(end);

	return renderer;
}

DeferredRenderer * Engine::DefaultDeferredConfiguration::createColorRenderer()
{
	DeferredRenderer * renderer = new DeferredRenderer();

	DeferredRenderObject * forward = createForwardBufferWithDepth();

	PostProcessChainNode * end = createColorRenderNode();

	renderer->setForwardPassBuffers(forward);
	renderer->setFinalPostProcess(end);

	return renderer;
}

DeferredRenderer * Engine::DefaultDeferredConfiguration::createSpecularRenderer()
{
	DeferredRenderer * renderer = new DeferredRenderer();

	DeferredRenderObject * forward = createForwardBufferWithDepth();
	PostProcessChainNode * end = createSpecularRenderNode();

	renderer->setForwardPassBuffers(forward);
	renderer->setFinalPostProcess(end);

	return renderer;
}

DeferredRenderer * Engine::DefaultDeferredConfiguration::createToonShaderRenderer()
{
	DeferredRenderer * renderer = new DeferredRenderer();

	DeferredRenderObject * forward = createForwardBufferWithDepth();
	PostProcessChainNode * toonShader = createToonShadingNode();
	PostProcessChainNode * end = createColorRenderNode();

	renderer->setForwardPassBuffers(forward);
	renderer->addPostProcess(toonShader);
	renderer->setFinalPostProcess(end);

	return renderer;
}

// ===============================================================================================
// ===============================================================================================
// ===============================================================================================

DeferredRenderObject * Engine::DefaultDeferredConfiguration::createForwardBuffer()
{
	DeferredRenderObject * buffer = new DeferredRenderObject(5, false);
	buffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500);
	buffer->addColorBuffer(1, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500);
	buffer->addColorBuffer(2, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500);
	buffer->addColorBuffer(3, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500);
	buffer->addColorBuffer(4, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, 500, 500);
	buffer->addDepthBuffer24(500, 500);
	return buffer;
}

DeferredRenderObject * Engine::DefaultDeferredConfiguration::createForwardBufferWithDepth()
{
	DeferredRenderObject * buffer = new DeferredRenderObject(5, true);
	buffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	buffer->addColorBuffer(1, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, 500, 500, GL_LINEAR);
	buffer->addColorBuffer(2, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	buffer->addColorBuffer(3, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	buffer->addColorBuffer(4, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, 500, 500, GL_LINEAR);
	buffer->addDepthBuffer24(500, 500);
	return buffer;
}

DeferredRenderObject * Engine::DefaultDeferredConfiguration::createDepthOnlyBuffer()
{
	DeferredRenderObject * buffer = new DeferredRenderObject(0, true);
	buffer->addDepthBuffer24(500, 500);
	return buffer;
}

PostProcessChainNode * Engine::DefaultDeferredConfiguration::createGaussianBlur(bool addConvolutionControl)
{
	PostProcessChainNode * node = new PostProcessChainNode;
	Program * gaussSource = ProgramTable::getInstance().getProgramByName("gaussian_blur_post_processing_program");
	node->postProcessProgram = new GaussianBlurProgram(*dynamic_cast<GaussianBlurProgram*>(gaussSource));

	if (addConvolutionControl)
	{
		KeyboardHandlersTable::getInstance().registerHandler(new ConvolutionMaskManagement(dynamic_cast<GaussianBlurProgram *>(node->postProcessProgram)));
	}

	float maskFactor = 1.0f / 14.0f;

	float mask[9] = {
		float(1.0*maskFactor), float(2.0*maskFactor), float(1.0*maskFactor),
		float(2.0*maskFactor), float(2.0*maskFactor), float(2.0*maskFactor),
		float(1.0*maskFactor), float(2.0*maskFactor), float(1.0*maskFactor)
	};

	glm::vec2 texIdx[9] = {
		glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f),
		glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f),
		glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, -1.0f), glm::vec2(1.0f, -1.0f)
	};

	GaussianBlurProgram * gauss = dynamic_cast<GaussianBlurProgram *>(node->postProcessProgram);
	gauss->setMaskSize(9);
	gauss->setAffectedTexels(texIdx);
	gauss->setKernel(mask);
	
	node->renderBuffer = new DeferredRenderObject(1, false);
	node->renderBuffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	node->renderBuffer->addDepthBuffer24(500, 500);
	node->callBack = 0;

	MeshInstance * mi = MeshInstanceTable::getInstance().getMeshInstance("plane", "gaussian_blur_post_processing_program");
	if (mi != 0)
	{
		node->obj = new Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}

PostProcessChainNode * Engine::DefaultDeferredConfiguration::createScreenAntiAliasing()
{
	PostProcessChainNode * node = new PostProcessChainNode;
	Program * gaussSource = ProgramTable::getInstance().getProgramByName("screen_space_anti_aliasing");
	node->postProcessProgram = new EdgeBasedProgram(*dynamic_cast<EdgeBasedProgram*>(gaussSource));

	node->renderBuffer = new DeferredRenderObject(2, false);
	node->renderBuffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	node->renderBuffer->addColorBuffer(1, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	node->renderBuffer->addDepthBuffer24(500, 500);
	node->callBack = 0;

	MeshInstance * mi = MeshInstanceTable::getInstance().getMeshInstance("plane", "screen_space_anti_aliasing");
	if (mi != 0)
	{
		node->obj = new Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}

PostProcessChainNode * Engine::DefaultDeferredConfiguration::create5x5GaussianBlur(float * convolutionMask, bool addConvolutionControl)
{
	PostProcessChainNode * node = new PostProcessChainNode;
	Program * gaussianBlur = ProgramTable::getInstance().getProgramByName("gaussian_blur_post_processing_program");
	node->postProcessProgram = new GaussianBlurProgram(*dynamic_cast<GaussianBlurProgram *>(gaussianBlur));

	if (addConvolutionControl)
	{
		KeyboardHandlersTable::getInstance().registerHandler(new ConvolutionMaskManagement(dynamic_cast<GaussianBlurProgram *>(node->postProcessProgram)));
	}

	glm::vec2 texIdx[25] = {
		glm::vec2(-2.0,2.0f), glm::vec2(-1.0,2.0f), glm::vec2(0.0,2.0f), glm::vec2(1.0,2.0f), glm::vec2(2.0,2.0),
		glm::vec2(-2.0,1.0f), glm::vec2(-1.0,1.0f), glm::vec2(0.0,1.0f), glm::vec2(1.0,1.0f), glm::vec2(2.0,1.0),
		glm::vec2(-2.0,0.0f), glm::vec2(-1.0,0.0f), glm::vec2(0.0,0.0f), glm::vec2(1.0,0.0f), glm::vec2(2.0,0.0),
		glm::vec2(-2.0,-1.0f), glm::vec2(-1.0,-1.0f), glm::vec2(0.0,-1.0f), glm::vec2(1.0,-1.0f), glm::vec2(2.0,-1.0),
		glm::vec2(-2.0,-2.0f), glm::vec2(-1.0,-2.0f), glm::vec2(0.0,-2.0f), glm::vec2(1.0,-2.0f), glm::vec2(2.0,-2.0)
	};

	GaussianBlurProgram * gauss = dynamic_cast<GaussianBlurProgram *>(node->postProcessProgram);
	gauss->setMaskSize(25);
	gauss->setAffectedTexels(texIdx);
	gauss->setKernel(convolutionMask);

	node->renderBuffer = new DeferredRenderObject(1, false);
	node->renderBuffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500);
	node->renderBuffer->addDepthBuffer24(500, 500);
	node->callBack = 0;

	MeshInstance * mi = MeshInstanceTable::getInstance().getMeshInstance("plane", "gaussian_blur_post_processing_program");
	if (mi != 0)
	{
		node->obj = new Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}

PostProcessChainNode * Engine::DefaultDeferredConfiguration::createDepthOfField(bool addDofControl, bool addConvolutionControl)
{
	PostProcessChainNode * node = new PostProcessChainNode;

	Program * dofSource = ProgramTable::getInstance().getProgramByName("depth_of_field_post_processing_program");
	node->postProcessProgram = new DepthOfFieldProgram(*dynamic_cast<DepthOfFieldProgram*>(dofSource));

	if (addDofControl)
	{
		KeyboardHandlersTable::getInstance().registerHandler(new DepthOfFieldManagement(dynamic_cast<DepthOfFieldProgram *>(node->postProcessProgram)));
	}

	if (addConvolutionControl)
	{
		KeyboardHandlersTable::getInstance().registerHandler(new ConvolutionMaskManagement(dynamic_cast<GaussianBlurProgram *>(node->postProcessProgram)));
	}

	float maskFactor = 1.0f / 50.0f;

	float mask[49] = {
		0.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor, 1.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor,
		0.0f*maskFactor, 0.0f*maskFactor, 1.0f*maskFactor, 2.0f*maskFactor, 1.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor,
		0.0f*maskFactor, 1.0f*maskFactor, 2.0f*maskFactor, 3.0f*maskFactor, 2.0f*maskFactor, 1.0f*maskFactor, 0.0f*maskFactor,
		1.0f*maskFactor, 2.0f*maskFactor, 3.0f*maskFactor, 10.0f*maskFactor, 3.0f*maskFactor, 2.0f*maskFactor, 1.0f*maskFactor,
		0.0f*maskFactor, 1.0f*maskFactor, 2.0f*maskFactor, 3.0f*maskFactor, 2.0f*maskFactor, 1.0f*maskFactor, 0.0f*maskFactor,
		0.0f*maskFactor, 0.0f*maskFactor, 1.0f*maskFactor, 2.0f*maskFactor, 1.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor,
		0.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor, 1.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor, 0.0f*maskFactor
	};

	glm::vec2 texIdx[49] = {
		glm::vec2(-3.0,3.0f), glm::vec2(-2.0,3.0f), glm::vec2(-1.0,3.0f), glm::vec2(0.0,3.0f), glm::vec2(1.0,3.0), glm::vec2(2.0,3.0), glm::vec2(3.0,3.0),
		glm::vec2(-3.0,2.0f), glm::vec2(-2.0,2.0f), glm::vec2(-1.0,2.0f), glm::vec2(0.0,2.0f), glm::vec2(1.0,2.0), glm::vec2(2.0,2.0), glm::vec2(3.0,2.0),
		glm::vec2(-3.0,1.0f), glm::vec2(-2.0,1.0f), glm::vec2(-1.0,1.0f), glm::vec2(0.0,1.0f), glm::vec2(1.0,1.0), glm::vec2(2.0,1.0), glm::vec2(3.0,1.0),
		glm::vec2(-3.0,0.0f), glm::vec2(-2.0,0.0f), glm::vec2(-1.0,0.0f), glm::vec2(0.0,0.0f), glm::vec2(1.0,0.0), glm::vec2(2.0,0.0), glm::vec2(3.0,0.0),
		glm::vec2(-3.0,-1.0f), glm::vec2(-2.0,-1.0f), glm::vec2(-1.0,-1.0f), glm::vec2(0.0,-1.0f), glm::vec2(1.0,-1.0), glm::vec2(2.0,-1.0), glm::vec2(3.0,-1.0),
		glm::vec2(-3.0,-2.0f), glm::vec2(-2.0,-2.0f), glm::vec2(-1.0,-2.0f), glm::vec2(0.0,-2.0f), glm::vec2(1.0,-2.0), glm::vec2(2.0,-2.0), glm::vec2(3.0,-2.0),
		glm::vec2(-3.0,-3.0f), glm::vec2(-2.0,-3.0f), glm::vec2(-1.0,-3.0f), glm::vec2(0.0,-3.0f), glm::vec2(1.0,-3.0), glm::vec2(2.0,-3.0), glm::vec2(3.0,-3.0)
	};

	DepthOfFieldProgram * dof = dynamic_cast<DepthOfFieldProgram *>(node->postProcessProgram);
	dof->setMaskSize(49);
	dof->setAffectedTexels(texIdx);
	dof->setKernel(mask);
	dof->setFocalDistance(-5.0f);
	dof->setMaxDistanceFactor(1.0f / 2.0f);

	node->renderBuffer = new DeferredRenderObject(1, false);
	node->renderBuffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	node->renderBuffer->addDepthBuffer24(500, 500);
	node->callBack = 0;

	MeshInstance * mi = MeshInstanceTable::getInstance().getMeshInstance("plane", "depth_of_field_post_processing_program");
	if (mi != 0)
	{
		node->obj = new Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}

PostProcessChainNode * Engine::DefaultDeferredConfiguration::createFinalLink(bool addMB, bool addMBControl)
{
	PostProcessChainNode * node = new PostProcessChainNode;
	Program * sourcePostProcess = ProgramTable::getInstance().getProgramByName("post_processing_program");
	node->postProcessProgram = new PostProcessProgram(*dynamic_cast<PostProcessProgram*>(sourcePostProcess));
	node->renderBuffer = 0;

	MeshInstance * mi = MeshInstanceTable::getInstance().getMeshInstance("plane", "post_processing_program");
	if (mi != 0)
	{
		node->obj = new Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	node->callBack = 0;
	if (addMB)
	{
		MotionBlurImpl * mbi = new MotionBlurImpl();
		node->callBack = mbi;

		if (addMBControl)
		{
			KeyboardHandlersTable::getInstance().registerHandler(new MotionBlurManagement(mbi));
		}
	}

	return node;
}

PostProcessChainNode * Engine::DefaultDeferredConfiguration::createDepthRenderNode()
{
	PostProcessChainNode * node = new PostProcessChainNode;
	Program * sourcePostProcess = ProgramTable::getInstance().getProgramByName("depth_render_post_processing_program");
	node->postProcessProgram = new DepthRenderProgram(*dynamic_cast<DepthRenderProgram*>(sourcePostProcess));
	node->renderBuffer = 0;
	node->callBack = 0;

	MeshInstance * mi = MeshInstanceTable::getInstance().getMeshInstance("plane", "depth_render_post_processing_program");
	if (mi != 0)
	{
		node->obj = new Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}

PostProcessChainNode * Engine::DefaultDeferredConfiguration::createNormalRenderNode()
{
	PostProcessChainNode * node = new PostProcessChainNode;
	Program * sourcePostProcess = ProgramTable::getInstance().getProgramByName("normal_render_post_processing_program");
	node->postProcessProgram = new PostProcessProgram(*dynamic_cast<PostProcessProgram*>(sourcePostProcess));
	node->renderBuffer = 0;
	node->callBack = 0;

	MeshInstance * mi = MeshInstanceTable::getInstance().getMeshInstance("plane", "normal_render_post_processing_program");
	if (mi != 0)
	{
		node->obj = new Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}

PostProcessChainNode * Engine::DefaultDeferredConfiguration::createColorRenderNode()
{
	PostProcessChainNode * node = new PostProcessChainNode;
	Program * sourcePostProcess = ProgramTable::getInstance().getProgramByName("color_render_post_processing_program");
	node->postProcessProgram = new PostProcessProgram(*dynamic_cast<PostProcessProgram*>(sourcePostProcess));
	node->renderBuffer = 0;
	node->callBack = 0;

	MeshInstance * mi = MeshInstanceTable::getInstance().getMeshInstance("plane", "color_render_post_processing_program");
	if (mi != 0)
	{
		node->obj = new Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}

PostProcessChainNode * Engine::DefaultDeferredConfiguration::createSpecularRenderNode()
{
	PostProcessChainNode * node = new PostProcessChainNode;
	Program * sourcePostProcess = ProgramTable::getInstance().getProgramByName("specular_render_post_processing_program");
	node->postProcessProgram = new PostProcessProgram(*dynamic_cast<PostProcessProgram*>(sourcePostProcess));
	node->renderBuffer = 0;
	node->callBack = 0;

	MeshInstance * mi = MeshInstanceTable::getInstance().getMeshInstance("plane", "specular_render_post_processing_program");
	if (mi != 0)
	{
		node->obj = new Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}

PostProcessChainNode * Engine::DefaultDeferredConfiguration::createDeferredShadingNode()
{
	PostProcessChainNode * node = new PostProcessChainNode;
	Program * sourcePostProcess = ProgramTable::getInstance().getProgramByName("deferred_shading");
	node->postProcessProgram = new DeferredShadingProgram(*dynamic_cast<DeferredShadingProgram*>(sourcePostProcess));

	DeferredRenderObject * buffer = new DeferredRenderObject(2, false);
	buffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	buffer->addColorBuffer(1, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	buffer->addDepthBuffer24(500, 500);

	node->renderBuffer = buffer;
	node->callBack = new DeferredShadingLightPopulation();

	MeshInstance * mi = MeshInstanceTable::getInstance().getMeshInstance("plane", "deferred_shading");
	if (mi != 0)
	{
		node->obj = new Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}

PostProcessChainNode * Engine::DefaultDeferredConfiguration::createToonShadingNode()
{
	PostProcessChainNode * node = new PostProcessChainNode;
	Program * sourcePostProcess = ProgramTable::getInstance().getProgramByName("toon_shading_program");
	node->postProcessProgram = new EdgeBasedProgram(*dynamic_cast<EdgeBasedProgram*>(sourcePostProcess));

	DeferredRenderObject * buffer = new DeferredRenderObject(2, false);
	buffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	buffer->addColorBuffer(1, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	buffer->addDepthBuffer24(500, 500);

	node->renderBuffer = buffer;
	node->callBack = 0;

	MeshInstance * mi = MeshInstanceTable::getInstance().getMeshInstance("plane", "toon_shading_program");
	if (mi != 0)
	{
		node->obj = new Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}
