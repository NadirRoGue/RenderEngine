#include "terraincomponents/LandscapeComponent.h"

#include "datatables/ProgramTable.h"
#include "datatables/MeshTable.h"

#include "CascadeShadowMaps.h"

Engine::LandscapeComponent::LandscapeComponent()
	:Engine::TerrainComponent()
{

}

unsigned int Engine::LandscapeComponent::getRenderRadius()
{
	return 12;
}

void Engine::LandscapeComponent::initialize()
{
	fillShader = Engine::ProgramTable::getInstance().getProgram<Engine::ProceduralTerrainProgram>();

	wireShader = Engine::ProgramTable::getInstance().getProgram<Engine::ProceduralTerrainProgram>(
		Engine::ProceduralTerrainProgram::WIRE_DRAW_MODE);

	shadowShader = Engine::ProgramTable::getInstance().getProgram<Engine::ProceduralTerrainProgram>(
		Engine::ProceduralTerrainProgram::SHADOW_MAP);

	Engine::Mesh * tile = Engine::MeshTable::getInstance().getMesh("terrain_tile");
	landscapeTile = new Engine::Object(tile);
	landscapeTile->setScale(glm::vec3(scale));

	fillShader->configureMeshBuffers(tile);
	wireShader->configureMeshBuffers(tile);
	shadowShader->configureMeshBuffers(tile);

	activeShader = fillShader;
}

void Engine::LandscapeComponent::preRenderComponent()
{
	glBindVertexArray(landscapeTile->getMesh()->vao);
}

void Engine::LandscapeComponent::renderComponent(int i, int j, Engine::Camera * cam)
{
	float poxX = i * scale;
	float posZ = j * scale;
	landscapeTile->setTranslation(glm::vec3(poxX, 0.0f, posZ));

	activeShader->setUniformGridPosition(i, j);
	activeShader->setUniformLightDepthMatrix(Engine::CascadeShadowMaps::getInstance().getDepthMatrix0() * landscapeTile->getModelMatrix());
	activeShader->setUniformLightDepthMatrix1(Engine::CascadeShadowMaps::getInstance().getDepthMatrix1() * landscapeTile->getModelMatrix());

	activeShader->onRenderObject(landscapeTile, cam);

	glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, (void*)0);
}

void Engine::LandscapeComponent::renderShadow(const glm::mat4 & projection, int i, int j, Engine::Camera * cam)
{
	float poxX = i * scale;
	float posZ = j * scale;
	landscapeTile->setTranslation(glm::vec3(poxX, 0.0f, posZ));

	shadowShader->setUniformGridPosition(i, j);
	shadowShader->setUniformLightDepthMatrix(projection * landscapeTile->getModelMatrix());

	shadowShader->onRenderObject(landscapeTile, cam);

	glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, (void*)0);
}

void Engine::LandscapeComponent::notifyRenderModeChange(Engine::RenderMode mode)
{
	switch (mode)
	{
	case Engine::RenderMode::RENDER_MODE_SHADED:
		activeShader = fillShader;
		break;
	case Engine::RenderMode::RENDER_MODE_WIRE:
		activeShader = wireShader;
	}
}

Engine::Program * Engine::LandscapeComponent::getActiveShader()
{
	return activeShader;
}

Engine::Program * Engine::LandscapeComponent::getShadowMapShader()
{
	return shadowShader;
}