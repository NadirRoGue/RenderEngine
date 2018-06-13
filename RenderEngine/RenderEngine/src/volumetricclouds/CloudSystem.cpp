#include "volumetricclouds/CloudSystem.h"

#include "TimeAccesor.h"

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

	for (int i = 0; i < 2; i++)
	{
		reprojectionBuffer[i] = new Engine::DeferredRenderObject(1, false);
		reprojectionBuffer[i]->setResizeMod(0.5f);
		reproBuffer[i] = reprojectionBuffer[i]->addColorBuffer(0, GL_RGBA16F, GL_RGBA, GL_FLOAT, 512, 1024, "", GL_NEAREST);
		reprojectionBuffer[i]->addDepthBuffer24(1024, 1024);
		reprojectionBuffer[i]->initialize();
	}
}

void Engine::CloudSystem::VolumetricClouds::render(Engine::Camera * cam)
{
	int prevFBO;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFBO);
	glDisable(GL_DEPTH_TEST);

	renderPlane->use();

	int frameMod = Engine::Time::frame % 2;

	// Render clouds
	glBindFramebuffer(GL_FRAMEBUFFER, reprojectionBuffer[frameMod]->getFrameBufferId());
	//glClear(GL_COLOR_BUFFER_BIT);
	shader->use();
	shader->onRenderObject(NULL, cam);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Filter clouds
	glBindFramebuffer(GL_FRAMEBUFFER, prevFBO);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	filterShader->use();
	filterShader->onRenderObject(NULL, cam);
	filterShader->setBufferInput(&reproBuffer[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
}