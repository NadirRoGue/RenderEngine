#include "volumetricclouds/CloudSystem.h"

#include "datatables/ProgramTable.h"
#include "datatables/MeshTable.h"

Engine::CloudSystem::VolumetricClouds::VolumetricClouds()
{
	shader = dynamic_cast<Engine::VolumetricCloudProgram*>
	(
		Engine::ProgramTable::getInstance().getProgramByName(Engine::VolumetricCloudProgram::PROGRAM_NAME, 0)
	);

	renderPlane = Engine::MeshTable::getInstance().getMesh("plane");
	shader->configureMeshBuffers(renderPlane);
}

void Engine::CloudSystem::VolumetricClouds::render(Engine::Camera * cam)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(shader->getProgramId());
	glBindVertexArray(renderPlane->vao);
	shader->onRenderObject(NULL, cam->getViewMatrix(), cam->getProjectionMatrix());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisable(GL_BLEND);
}