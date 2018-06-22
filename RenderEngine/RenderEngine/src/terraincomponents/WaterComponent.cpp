#include "terraincomponents/WaterComponent.h"

#include "datatables/ProgramTable.h"
#include "datatables/MeshTable.h"

#include "CascadeShadowMaps.h"

Engine::WaterComponent::WaterComponent()
	:Engine::TerrainComponent()
{

}

unsigned int Engine::WaterComponent::getRenderRadius()
{
	return 12;
}

void Engine::WaterComponent::initialize()
{
	fillShader = Engine::ProgramTable::getInstance().getProgram<Engine::ProceduralWaterProgram>();

	wireShader = Engine::ProgramTable::getInstance().getProgram<Engine::ProceduralWaterProgram>(Engine::ProceduralWaterProgram::WIRE_DRAW_MODE);

	/*shadowShader = Engine::ProgramTable::getInstance().getProgram<Engine::ProceduralTerrainProgram>(
		Engine::ProceduralTerrainProgram::PROGRAM_NAME,
		Engine::ProceduralTerrainProgram::SHADOW_MAP);*/

	Engine::Mesh * tile = Engine::MeshTable::getInstance().getMesh("terrain_tile");
	waterTile = new Engine::Object(tile);
	waterTile->setScale(glm::vec3(scale));

	fillShader->configureMeshBuffers(tile);
	wireShader->configureMeshBuffers(tile);
	//shadowShader->configureMeshBuffers(tile);

	activeShader = fillShader;
}

void Engine::WaterComponent::preRenderComponent()
{
	glBindVertexArray(waterTile->getMesh()->vao);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Engine::WaterComponent::renderComponent(int i, int j, Engine::Camera * cam)
{
	float poxX = i * scale;
	float posZ = j * scale;
	waterTile->setTranslation(glm::vec3(poxX, Engine::Settings::waterHeight * scale * 1.5f, posZ));

	activeShader->setUniformGridPosition(i, j);
	activeShader->setUniformLightDepthMatrix(Engine::CascadeShadowMaps::getInstance().getDepthMatrix0() * waterTile->getModelMatrix());
	activeShader->setUniformLightDepthMatrix1(Engine::CascadeShadowMaps::getInstance().getDepthMatrix1() * waterTile->getModelMatrix());
	activeShader->onRenderObject(waterTile, cam);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}

void Engine::WaterComponent::postRenderComponent()
{
	glDisable(GL_BLEND);
}

void Engine::WaterComponent::renderShadow(const glm::mat4 & projection, int i, int j, Engine::Camera * cam)
{
	/*
	float poxX = i * scale;
	float posZ = j * scale;
	waterTile->setTranslation(glm::vec3(poxX, 0.0f, posZ));

	shadowShader->setUniformGridPosition(i, j);
	shadowShader->setUniformLightDepthMatrix(Engine::CascadeShadowMaps::getInstance().getShadowProjectionMat() * waterTile->getModelMatrix());

	shadowShader->onRenderObject(waterTile, cam);

	glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, (void*)0);
	*/
}

void Engine::WaterComponent::notifyRenderModeChange(Engine::RenderMode mode)
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

Engine::Program * Engine::WaterComponent::getActiveShader()
{
	return activeShader;
}

Engine::Program * Engine::WaterComponent::getShadowMapShader()
{
	return NULL;// shadowShader;
}