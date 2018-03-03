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
#include "InputImpl.h"

#include <iostream>

Engine::DeferredRenderer * Engine::TestImplementation::createDeferredRendererWithDoF()
{
	Engine::DeferredRenderer * renderer = new Engine::DeferredRenderer();

	Engine::DeferredRenderObject * start = createForwardBufferWithDepth();
	Engine::PostProcessChainNode * dS = createDeferredShadingNode();
	Engine::PostProcessChainNode * AA = createScreenAntiAliasing();
	Engine::PostProcessChainNode * gauss = createDepthOfField(true, true);
	Engine::PostProcessChainNode * end = createFinalLink(true, true);

	renderer->setForwardPassBuffers(start);
	renderer->addPostProcess(dS);
	renderer->addPostProcess(AA);
	renderer->addPostProcess(gauss);
	renderer->setFinalPostProcess(end);

	return renderer;
}

Engine::DeferredRenderer * Engine::TestImplementation::createDeferredDepthRenderer()
{
	Engine::DeferredRenderer * renderer = new Engine::DeferredRenderer();

	Engine::DeferredRenderObject * forward = createDepthOnlyBuffer();
	Engine::PostProcessChainNode * end = createDepthRenderNode();

	renderer->setForwardPassBuffers(forward);
	renderer->setFinalPostProcess(end);

	return renderer;
}

Engine::DeferredRenderer * Engine::TestImplementation::createNormalRenderer()
{
	Engine::DeferredRenderer * renderer = new Engine::DeferredRenderer();

	Engine::DeferredRenderObject * forward = createForwardBufferWithDepth();
	Engine::PostProcessChainNode * end = createNormalRenderNode();

	renderer->setForwardPassBuffers(forward);
	renderer->setFinalPostProcess(end);

	return renderer;
}

Engine::DeferredRenderer * Engine::TestImplementation::createColorRenderer()
{
	Engine::DeferredRenderer * renderer = new Engine::DeferredRenderer();

	Engine::DeferredRenderObject * forward = createForwardBufferWithDepth();

	Engine::PostProcessChainNode * end = createColorRenderNode();

	renderer->setForwardPassBuffers(forward);
	renderer->setFinalPostProcess(end);

	return renderer;
}

Engine::DeferredRenderer * Engine::TestImplementation::createSpecularRenderer()
{
	Engine::DeferredRenderer * renderer = new Engine::DeferredRenderer();

	Engine::DeferredRenderObject * forward = createForwardBufferWithDepth();
	Engine::PostProcessChainNode * end = createSpecularRenderNode();

	renderer->setForwardPassBuffers(forward);
	renderer->setFinalPostProcess(end);

	return renderer;
}

Engine::DeferredRenderer * Engine::TestImplementation::createToonShaderRenderer()
{
	Engine::DeferredRenderer * renderer = new Engine::DeferredRenderer();

	Engine::DeferredRenderObject * forward = createForwardBufferWithDepth();
	Engine::PostProcessChainNode * toonShader = createToonShadingNode();
	Engine::PostProcessChainNode * end = createColorRenderNode();

	renderer->setForwardPassBuffers(forward);
	renderer->addPostProcess(toonShader);
	renderer->setFinalPostProcess(end);

	return renderer;
}

// ===============================================================================================
// ===============================================================================================
// ===============================================================================================

Engine::DeferredRenderObject * Engine::TestImplementation::createForwardBuffer()
{
	Engine::DeferredRenderObject * buffer = new Engine::DeferredRenderObject(5, false);
	buffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500);
	buffer->addColorBuffer(1, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500);
	buffer->addColorBuffer(2, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500);
	buffer->addColorBuffer(3, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500);
	buffer->addColorBuffer(4, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, 500, 500);
	buffer->addDepthBuffer24(500, 500);
	return buffer;
}

Engine::DeferredRenderObject * Engine::TestImplementation::createForwardBufferWithDepth()
{
	Engine::DeferredRenderObject * buffer = new Engine::DeferredRenderObject(5, true);
	buffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	buffer->addColorBuffer(1, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, 500, 500, GL_LINEAR);
	buffer->addColorBuffer(2, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	buffer->addColorBuffer(3, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	buffer->addColorBuffer(4, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, 500, 500, GL_LINEAR);
	buffer->addDepthBuffer24(500, 500);
	return buffer;
}

Engine::DeferredRenderObject * Engine::TestImplementation::createDepthOnlyBuffer()
{
	Engine::DeferredRenderObject * buffer = new Engine::DeferredRenderObject(0, true);
	buffer->addDepthBuffer24(500, 500);
	return buffer;
}

Engine::PostProcessChainNode * Engine::TestImplementation::createGaussianBlur(bool addConvolutionControl)
{
	Engine::PostProcessChainNode * node = new Engine::PostProcessChainNode;
	Engine::Program * gaussSource = Engine::ProgramTable::getInstance().getProgramByName("gaussian_blur_post_processing_program");
	node->postProcessProgram = new Engine::GaussianBlurProgram(*dynamic_cast<Engine::GaussianBlurProgram*>(gaussSource));

	if (addConvolutionControl)
	{
		Engine::KeyboardHandlersTable * table = Engine::SceneManager::getInstance().getActiveScene()->getKeyboardHandler();
		if (table != NULL)
		{
			table->registerHandler(new Engine::TestImplementation::ConvolutionMaskManagement(dynamic_cast<Engine::GaussianBlurProgram *>(node->postProcessProgram)));
		}
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

	Engine::GaussianBlurProgram * gauss = dynamic_cast<Engine::GaussianBlurProgram *>(node->postProcessProgram);
	gauss->setMaskSize(9);
	gauss->setAffectedTexels(texIdx);
	gauss->setKernel(mask);
	
	node->renderBuffer = new Engine::DeferredRenderObject(1, false);
	node->renderBuffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	node->renderBuffer->addDepthBuffer24(500, 500);
	node->callBack = 0;

	Engine::MeshInstance * mi = Engine::MeshInstanceTable::getInstance().getMeshInstance("plane", "gaussian_blur_post_processing_program");
	if (mi != 0)
	{
		node->obj = new Engine::Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}

Engine::PostProcessChainNode * Engine::TestImplementation::createScreenAntiAliasing()
{
	Engine::PostProcessChainNode * node = new Engine::PostProcessChainNode;
	Engine::Program * gaussSource = Engine::ProgramTable::getInstance().getProgramByName("screen_space_anti_aliasing");
	node->postProcessProgram = new Engine::EdgeBasedProgram(*dynamic_cast<Engine::EdgeBasedProgram*>(gaussSource));

	node->renderBuffer = new Engine::DeferredRenderObject(2, false);
	node->renderBuffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	node->renderBuffer->addColorBuffer(1, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	node->renderBuffer->addDepthBuffer24(500, 500);
	node->callBack = 0;

	Engine::MeshInstance * mi = Engine::MeshInstanceTable::getInstance().getMeshInstance("plane", "screen_space_anti_aliasing");
	if (mi != 0)
	{
		node->obj = new Engine::Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}

Engine::PostProcessChainNode * Engine::TestImplementation::create5x5GaussianBlur(float * convolutionMask, bool addConvolutionControl)
{
	Engine::PostProcessChainNode * node = new Engine::PostProcessChainNode;
	Engine::Program * gaussianBlur = Engine::ProgramTable::getInstance().getProgramByName("gaussian_blur_post_processing_program");
	node->postProcessProgram = new Engine::GaussianBlurProgram(*dynamic_cast<Engine::GaussianBlurProgram *>(gaussianBlur));


	if (addConvolutionControl)
	{
		Engine::KeyboardHandlersTable * table = Engine::SceneManager::getInstance().getActiveScene()->getKeyboardHandler();
		if (table != NULL)
		{
			table->registerHandler(new Engine::TestImplementation::ConvolutionMaskManagement(dynamic_cast<Engine::GaussianBlurProgram *>(node->postProcessProgram)));
		}
	}

	glm::vec2 texIdx[25] = {
		glm::vec2(-2.0,2.0f), glm::vec2(-1.0,2.0f), glm::vec2(0.0,2.0f), glm::vec2(1.0,2.0f), glm::vec2(2.0,2.0),
		glm::vec2(-2.0,1.0f), glm::vec2(-1.0,1.0f), glm::vec2(0.0,1.0f), glm::vec2(1.0,1.0f), glm::vec2(2.0,1.0),
		glm::vec2(-2.0,0.0f), glm::vec2(-1.0,0.0f), glm::vec2(0.0,0.0f), glm::vec2(1.0,0.0f), glm::vec2(2.0,0.0),
		glm::vec2(-2.0,-1.0f), glm::vec2(-1.0,-1.0f), glm::vec2(0.0,-1.0f), glm::vec2(1.0,-1.0f), glm::vec2(2.0,-1.0),
		glm::vec2(-2.0,-2.0f), glm::vec2(-1.0,-2.0f), glm::vec2(0.0,-2.0f), glm::vec2(1.0,-2.0f), glm::vec2(2.0,-2.0)
	};

	Engine::GaussianBlurProgram * gauss = dynamic_cast<Engine::GaussianBlurProgram *>(node->postProcessProgram);
	gauss->setMaskSize(25);
	gauss->setAffectedTexels(texIdx);
	gauss->setKernel(convolutionMask);

	node->renderBuffer = new Engine::DeferredRenderObject(1, false);
	node->renderBuffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500);
	node->renderBuffer->addDepthBuffer24(500, 500);
	node->callBack = 0;

	Engine::MeshInstance * mi = Engine::MeshInstanceTable::getInstance().getMeshInstance("plane", "gaussian_blur_post_processing_program");
	if (mi != 0)
	{
		node->obj = new Engine::Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}

Engine::PostProcessChainNode * Engine::TestImplementation::createDepthOfField(bool addDofControl, bool addConvolutionControl)
{
	Engine::PostProcessChainNode * node = new Engine::PostProcessChainNode;

	Engine::Program * dofSource = Engine::ProgramTable::getInstance().getProgramByName("depth_of_field_post_processing_program");
	node->postProcessProgram = new Engine::DepthOfFieldProgram(*dynamic_cast<Engine::DepthOfFieldProgram*>(dofSource));

	if (addDofControl)
	{
		Engine::KeyboardHandlersTable * table = Engine::SceneManager::getInstance().getActiveScene()->getKeyboardHandler();
		if (table != NULL)
		{
			table->registerHandler(new Engine::TestImplementation::DepthOfFieldManagement(dynamic_cast<Engine::DepthOfFieldProgram *>(node->postProcessProgram)));
		}
	}

	if (addConvolutionControl)
	{
		Engine::KeyboardHandlersTable * table = Engine::SceneManager::getInstance().getActiveScene()->getKeyboardHandler();
		if (table != NULL)
		{
			table->registerHandler(new Engine::TestImplementation::ConvolutionMaskManagement(dynamic_cast<Engine::GaussianBlurProgram *>(node->postProcessProgram)));
		}
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

	Engine::DepthOfFieldProgram * dof = dynamic_cast<Engine::DepthOfFieldProgram *>(node->postProcessProgram);
	dof->setMaskSize(49);
	dof->setAffectedTexels(texIdx);
	dof->setKernel(mask);
	dof->setFocalDistance(-5.0f);
	dof->setMaxDistanceFactor(1.0f / 2.0f);

	node->renderBuffer = new Engine::DeferredRenderObject(1, false);
	node->renderBuffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	node->renderBuffer->addDepthBuffer24(500, 500);
	node->callBack = 0;

	Engine::MeshInstance * mi = Engine::MeshInstanceTable::getInstance().getMeshInstance("plane", "depth_of_field_post_processing_program");
	if (mi != 0)
	{
		node->obj = new Engine::Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}

Engine::PostProcessChainNode * Engine::TestImplementation::createFinalLink(bool addMB, bool addMBControl)
{
	Engine::PostProcessChainNode * node = new Engine::PostProcessChainNode;
	Engine::Program * sourcePostProcess = Engine::ProgramTable::getInstance().getProgramByName("post_processing_program");
	node->postProcessProgram = new Engine::PostProcessProgram(*dynamic_cast<Engine::PostProcessProgram*>(sourcePostProcess));
	node->renderBuffer = 0;

	Engine::MeshInstance * mi = Engine::MeshInstanceTable::getInstance().getMeshInstance("plane", "post_processing_program");
	if (mi != 0)
	{
		node->obj = new Engine::Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	node->callBack = 0;
	if (addMB)
	{
		Engine::MotionBlurImpl * mbi = new Engine::MotionBlurImpl();
		node->callBack = mbi;

		if (addMBControl)
		{
			Engine::KeyboardHandlersTable * table = Engine::SceneManager::getInstance().getActiveScene()->getKeyboardHandler();
			if (table != NULL)
			{
				table->registerHandler(new Engine::TestImplementation::MotionBlurManagement(mbi));
			}
		}
	}

	return node;
}

Engine::PostProcessChainNode * Engine::TestImplementation::createDepthRenderNode()
{
	Engine::PostProcessChainNode * node = new Engine::PostProcessChainNode;
	Engine::Program * sourcePostProcess = Engine::ProgramTable::getInstance().getProgramByName("depth_render_post_processing_program");
	node->postProcessProgram = new Engine::DepthRenderProgram(*dynamic_cast<Engine::DepthRenderProgram*>(sourcePostProcess));
	node->renderBuffer = 0;
	node->callBack = 0;

	Engine::MeshInstance * mi = Engine::MeshInstanceTable::getInstance().getMeshInstance("plane", "depth_render_post_processing_program");
	if (mi != 0)
	{
		node->obj = new Engine::Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}

Engine::PostProcessChainNode * Engine::TestImplementation::createNormalRenderNode()
{
	Engine::PostProcessChainNode * node = new Engine::PostProcessChainNode;
	Engine::Program * sourcePostProcess = Engine::ProgramTable::getInstance().getProgramByName("normal_render_post_processing_program");
	node->postProcessProgram = new Engine::PostProcessProgram(*dynamic_cast<Engine::PostProcessProgram*>(sourcePostProcess));
	node->renderBuffer = 0;
	node->callBack = 0;

	Engine::MeshInstance * mi = Engine::MeshInstanceTable::getInstance().getMeshInstance("plane", "normal_render_post_processing_program");
	if (mi != 0)
	{
		node->obj = new Engine::Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}

Engine::PostProcessChainNode * Engine::TestImplementation::createColorRenderNode()
{
	Engine::PostProcessChainNode * node = new Engine::PostProcessChainNode;
	Engine::Program * sourcePostProcess = Engine::ProgramTable::getInstance().getProgramByName("color_render_post_processing_program");
	node->postProcessProgram = new Engine::PostProcessProgram(*dynamic_cast<Engine::PostProcessProgram*>(sourcePostProcess));
	node->renderBuffer = 0;
	node->callBack = 0;

	Engine::MeshInstance * mi = Engine::MeshInstanceTable::getInstance().getMeshInstance("plane", "color_render_post_processing_program");
	if (mi != 0)
	{
		node->obj = new Engine::Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}

Engine::PostProcessChainNode * Engine::TestImplementation::createSpecularRenderNode()
{
	Engine::PostProcessChainNode * node = new Engine::PostProcessChainNode;
	Engine::Program * sourcePostProcess = Engine::ProgramTable::getInstance().getProgramByName("specular_render_post_processing_program");
	node->postProcessProgram = new Engine::PostProcessProgram(*dynamic_cast<Engine::PostProcessProgram*>(sourcePostProcess));
	node->renderBuffer = 0;
	node->callBack = 0;

	Engine::MeshInstance * mi = Engine::MeshInstanceTable::getInstance().getMeshInstance("plane", "specular_render_post_processing_program");
	if (mi != 0)
	{
		node->obj = new Engine::Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}

Engine::PostProcessChainNode * Engine::TestImplementation::createDeferredShadingNode()
{
	Engine::PostProcessChainNode * node = new Engine::PostProcessChainNode;
	Engine::Program * sourcePostProcess = ProgramTable::getInstance().getProgramByName("deferred_shading");
	node->postProcessProgram = new Engine::DeferredShadingProgram(*dynamic_cast<Engine::DeferredShadingProgram*>(sourcePostProcess));

	Engine::DeferredRenderObject * buffer = new Engine::DeferredRenderObject(2, false);
	buffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	buffer->addColorBuffer(1, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	buffer->addDepthBuffer24(500, 500);

	node->renderBuffer = buffer;
	node->callBack = new Engine::DeferredShadingLightPopulation();

	Engine::MeshInstance * mi = Engine::MeshInstanceTable::getInstance().getMeshInstance("plane", "deferred_shading");
	if (mi != 0)
	{
		node->obj = new Engine::Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}

Engine::PostProcessChainNode * Engine::TestImplementation::createToonShadingNode()
{
	Engine::PostProcessChainNode * node = new Engine::PostProcessChainNode;
	Engine::Program * sourcePostProcess = Engine::ProgramTable::getInstance().getProgramByName("toon_shading_program");
	node->postProcessProgram = new Engine::EdgeBasedProgram(*dynamic_cast<Engine::EdgeBasedProgram*>(sourcePostProcess));

	Engine::DeferredRenderObject * buffer = new Engine::DeferredRenderObject(2, false);
	buffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	buffer->addColorBuffer(1, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	buffer->addDepthBuffer24(500, 500);

	node->renderBuffer = buffer;
	node->callBack = 0;

	Engine::MeshInstance * mi = Engine::MeshInstanceTable::getInstance().getMeshInstance("plane", "toon_shading_program");
	if (mi != 0)
	{
		node->obj = new Engine::Object(mi);
	}
	else
	{
		std::cout << "Could not instantiate panel" << std::endl;
	}

	return node;
}
