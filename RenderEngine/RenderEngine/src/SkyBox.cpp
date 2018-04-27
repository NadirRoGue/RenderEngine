#include "SkyBox.h"

#include "datatables/ProgramTable.h"
#include "datatables/MeshTable.h"
#include "Scene.h"

Engine::SkyBox::SkyBox(Engine::TextureInstance * skyboxTextureCubeMap)
	:skyCubeMap(skyboxTextureCubeMap)
{
	skyCubeMap->setMinificationFilterType(GL_NEAREST);
	skyCubeMap->setMagnificationFilterType(GL_NEAREST);
	skyCubeMap->setAnisotropicFilterEnabled(false);
	skyCubeMap->configureTexture();
	initialize();

	renderMode = GL_TRIANGLES;
}

Engine::SkyBox::~SkyBox()
{
	if (skyCubeMap != NULL)
	{
		delete skyCubeMap;
	}

	if (cubeMesh != NULL)
	{
		delete cubeMesh;
	}
}

void Engine::SkyBox::render(Engine::Camera * camera)
{
	const Engine::Mesh * data = cubeMesh->getMesh();

	glUseProgram(shader->getProgramId());
	glBindVertexArray(data->vao);

	const glm::vec3 pos = camera->getPosition();
	glm::vec3 cubePos(pos);
	cubeMesh->setTranslation(cubePos * -1.0f);
	shader->onRenderObject(cubeMesh, camera->getViewMatrix(), camera->getProjectionMatrix());
	shader->setCubemapUniform(skyCubeMap);

	Engine::DirectionalLight * dl = Engine::SceneManager::getInstance().getActiveScene()->getDirectionalLight();

	const glm::mat4 & modelCopy = dl->getModelMatrix();
	//modelCopy[3][3] = 0.0f;
	//glm::mat4 resultPos = camera->getViewMatrix() * modelCopy;

	glm::vec3 direction(modelCopy[3][0], modelCopy[3][1], modelCopy[3][2]);
	direction = -glm::normalize(direction);

	shader->setLightDirUniform(direction);

	glDrawElements(renderMode, data->getNumFaces() * data->getNumVerticesPerFace(), GL_UNSIGNED_INT, (void*)0);
}

void Engine::SkyBox::initialize()
{
	shader = dynamic_cast<SkyProgram*>(Engine::ProgramTable::getInstance().getProgramByName(Engine::SkyProgram::PROGRAM_NAME));
	Engine::Mesh * mesh = Engine::MeshTable::getInstance().getMesh("cube");

	shader->configureMeshBuffers(mesh);

	cubeMesh = new Engine::Object(mesh);
	//cubeMesh->setScale(glm::vec3(1, 1, 1));
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