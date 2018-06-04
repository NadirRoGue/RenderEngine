#include "volumetricclouds/CloudSystem.h"

#include "datatables/ProgramTable.h"
#include "datatables/MeshTable.h"

Engine::CloudSystem::VolumetricClouds::VolumetricClouds()
{
	shader = dynamic_cast<Engine::VolumetricCloudProgram*>
	(
		Engine::ProgramTable::getInstance().getProgramByName(Engine::VolumetricCloudProgram::PROGRAM_NAME, 0)
	);

	filterShader = dynamic_cast<Engine::CloudFilterProgram*>
	(
		Engine::ProgramTable::getInstance().getProgramByName(Engine::CloudFilterProgram::PROGRAM_NAME, 0)
	);

	renderPlane = Engine::MeshTable::getInstance().getMesh("plane");

	shader->configureMeshBuffers(renderPlane);
	filterShader->configureMeshBuffers(renderPlane);

	filterBuffer = new Engine::DeferredRenderObject(1, false);
	colorB = filterBuffer->addColorBuffer(0, GL_RGBA16F, GL_RGBA, GL_FLOAT, 1024, 1024, "", GL_NEAREST);
	filterBuffer->addDepthBuffer24(1024, 1024);
	filterBuffer->initialize();
}

void Engine::CloudSystem::VolumetricClouds::render(Engine::Camera * cam)
{
	int prevFBO;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFBO);

	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(renderPlane->vao);

	// Render clouds
	glBindFramebuffer(GL_FRAMEBUFFER, filterBuffer->getFrameBufferId());
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shader->getProgramId());
	shader->onRenderObject(NULL, cam->getViewMatrix(), cam->getProjectionMatrix());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Filter clouds
	glBindFramebuffer(GL_FRAMEBUFFER, prevFBO);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(filterShader->getProgramId());
	filterShader->onRenderObject(NULL, cam->getViewMatrix(), cam->getProjectionMatrix());
	filterShader->setBufferInput(colorB);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
}