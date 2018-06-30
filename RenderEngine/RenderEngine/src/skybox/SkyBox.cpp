#include "skybox/SkyBox.h"

#include "datatables/ProgramTable.h"
#include "datatables/MeshTable.h"
#include "Scene.h"

#include <iostream>

Engine::SkyBox::SkyBox()
{
	// Initialize the skybox parameters
	initialize();

	// Initialize the clouds. They dont depend on the skybox, but its meaningful to 
	// render them from the skybox
	clouds = new Engine::CloudSystem::VolumetricClouds();

	// Default render mode
	renderMode = GL_TRIANGLES;

	// Register the object to be notified when drawing method changes
	//Engine::RenderableNotifier::getInstance().registerRenderable(this);
}

Engine::SkyBox::~SkyBox()
{
	if (cubeMesh != NULL)
	{
		delete cubeMesh;
	}
}

void Engine::SkyBox::render(Engine::Camera * camera)
{
	// Switch to less-equal depth testing to be able to render the skybox
	// on the max depth, behind everything
	glDepthFunc(GL_LEQUAL);

	const Engine::Mesh * data = cubeMesh->getMesh();
	data->use();

	shader->use();

	// Move the skybox with the camera
	const glm::vec3 pos = camera->getPosition();
	glm::vec3 cubePos(-pos);
	cubeMesh->setTranslation(cubePos);
	shader->onRenderObject(cubeMesh, camera);

	glDrawElements(renderMode, data->getNumFaces() * data->getNumVerticesPerFace(), GL_UNSIGNED_INT, (void*)0);

	glDepthFunc(GL_LESS);

	// Render clouds
	clouds->render(camera);
}

void Engine::SkyBox::initialize()
{
	// Instance shaders and meshes
	shader = Engine::ProgramTable::getInstance().getProgram<Engine::SkyProgram>();
	Engine::Mesh * mesh = Engine::MeshTable::getInstance().getMesh("cube");

	shader->configureMeshBuffers(mesh);

	cubeMesh = new Engine::Object(mesh);
}

void Engine::SkyBox::notifyRenderModeUpdate(Engine::RenderMode mode)
{
	switch (mode)
	{
	case Engine::RenderMode::RENDER_MODE_WIRE:
		renderMode = GL_LINES;
		break;
	default:
		renderMode = GL_TRIANGLES;
		break;
	}
}