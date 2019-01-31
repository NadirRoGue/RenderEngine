#include "volumetricclouds/CloudSystem.h"

#include "TimeAccesor.h"

#include "datatables/ProgramTable.h"
#include "datatables/MeshTable.h"
#include "WorldConfig.h"
#include "CascadeShadowMaps.h"

#include <iostream>

Engine::CloudSystem::VolumetricClouds::VolumetricClouds()
{
	//Engine::CascadeShadowMaps::getInstance().registerShadowCaster(this);

	shader = Engine::ProgramTable::getInstance().getProgram<Engine::VolumetricCloudProgram>();
	filterShader = Engine::ProgramTable::getInstance().getProgram<Engine::CloudFilterProgram>();
	shadowShader = Engine::ProgramTable::getInstance().getProgram<Engine::CloudShadowProgram>();

	renderPlane = Engine::MeshTable::getInstance().getMesh("plane");

	createTileMesh();

	shader->configureMeshBuffers(renderPlane);
	filterShader->configureMeshBuffers(renderPlane);

	for (int i = 0; i < 2; i++)
	{
		reprojectionBuffer[i] = new Engine::DeferredRenderObject(2, false);
		reprojectionBuffer[i]->setResizeMod(0.5f);
		reproBuffer[i] = reprojectionBuffer[i]->addColorBuffer(0, GL_RGBA16F, GL_RGBA, GL_FLOAT, 512, 1024, "", GL_NEAREST);
		//reproBuffer[i]->setSComponentWrapType(GL_CLAMP_TO_EDGE);
		//reproBuffer[i]->setTComponentWrapType(GL_CLAMP_TO_EDGE);
		pixelVelocityMap[i] = reprojectionBuffer[i]->addColorBuffer(1, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, 512, 1024, "", GL_NEAREST);
		reprojectionBuffer[i]->addDepthBuffer24(512, 1024);
		reprojectionBuffer[i]->initialize();
	}
}

void dbg(int point)
{
	GLenum error = glGetError();
	if(error != 0)
	{
		std::cout << point << " error: " << error << std::endl;
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

	glClear(GL_COLOR_BUFFER_BIT);
	shader->use();
	shader->onRenderObject(NULL, cam);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Filter clouds
	glBindFramebuffer(GL_FRAMEBUFFER, prevFBO);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	filterShader->use();
	filterShader->setBufferInput(&reproBuffer[0]);
	filterShader->setVelocityInput(&pixelVelocityMap[0]);
	filterShader->onRenderObject(NULL, cam);

	//dbg(11);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Engine::CloudSystem::VolumetricClouds::renderShadow(Camera * camera, const glm::mat4 & projectionMatrix)
{
	skyPlane->getMesh()->use();
	const glm::vec3 & cameraPosition = camera->getPosition();
	float x = 0.0f;// -cameraPosition.x;
	float z = 0.0f;// -cameraPosition.z;

	skyPlane->setTranslation(glm::vec3(x, 0.0f, z));

	shadowShader->use();
	shadowShader->setUniformLightProjMatrix(projectionMatrix * skyPlane->getModelMatrix());
	shadowShader->onRenderObject(skyPlane, camera);
	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, (void*)0);
}

void Engine::CloudSystem::VolumetricClouds::createTileMesh()
{
	float vertices[12];
	vertices[0] = -1062.5f; vertices[1] = 0.0f; vertices[2] = -1062.0f;
	vertices[3] = 1062.5f; vertices[4] = 0.0f; vertices[5] = -1062.5f;
	vertices[6] = -1062.5f; vertices[7] = 0.0f; vertices[8] = 1062.5f;
	vertices[9] = 1062.5f; vertices[10] = 0.0f; vertices[11] = 1062.5f;

	unsigned int faces[6];
	faces[0] = 0; faces[1] = 2; faces[2] = 1;
	faces[3] = 1; faces[4] = 2; faces[5] = 3;

	float normals[12];
	normals[0] = 0.0f; normals[1] = 1.0f; normals[2] = 0.0f;
	normals[3] = 0.0f; normals[4] = 1.0f; normals[5] = 0.0f;
	normals[6] = 0.0f; normals[7] = 1.0f; normals[8] = 0.0f;
	normals[9] = 0.0f; normals[10] = 1.0f; normals[11] = 0.0f;

	float uv[8];
	uv[0] = 0.0f; uv[1] = 0.0f;
	uv[2] = 1.0f; uv[3] = 0.0f;
	uv[4] = 0.0f; uv[5] = 1.0f;
	uv[6] = 1.0f; uv[7] = 1.0f;

	Engine::Mesh plane(2, 4, faces, vertices, 0, normals, uv, 0);
	Engine::MeshTable::getInstance().addMeshToCache("sky_tile", plane);
	Engine::Mesh * planeInstance = Engine::MeshTable::getInstance().getMesh("sky_tile");

	shadowShader->configureMeshBuffers(planeInstance);

	skyPlane = new Engine::Object(planeInstance);
}