#include "Terrain.h"

#include "Mesh.h"

#include "datatables/MeshTable.h"

#include "terraincomponents/LandscapeComponent.h"
#include "terraincomponents/WaterComponent.h"
#include "terraincomponents/TreeComponent.h"
#include "terraincomponents/FlowerComponent.h"

#include <iostream>

#include "CascadeShadowMaps.h"

Engine::Terrain::Terrain()
{
	tileWidth = 1.0f;
	renderRadius = 7;
	initialize();
}

Engine::Terrain::Terrain(float tileWidth, unsigned int renderRadius)
{
	this->tileWidth = tileWidth;
	this->renderRadius = renderRadius;
	initialize();
}

Engine::Terrain::~Terrain()
{
}

void Engine::Terrain::registerComponent(Engine::TerrainComponent * comp)
{
	if (comp != NULL)
	{
		renderableComponents.push_back(comp);
		if (comp->castShadows())
		{
			shadowableComponents.push_back(comp);
		}
	}
}

// ====================================================================================================================

void Engine::Terrain::render(Engine::Camera * camera)
{
	for (auto & tc : renderableComponents)
	{
		renderTiledComponent(tc, camera);
	}
}

void Engine::Terrain::renderShadow(Camera * cam, const glm::mat4 & projectionMatrix)
{
	for (auto & sc : shadowableComponents)
	{
		renderTiledComponentShadow(sc, cam, projectionMatrix);
	}
}

// ====================================================================================================================

void Engine::Terrain::renderTiledComponent(Engine::TerrainComponent * component, Engine::Camera * cam)
{
	glm::vec3 cameraPosition = cam->getPosition();

	int x = -int((floor(cameraPosition.x)) / tileWidth);
	int y = -int((floor(cameraPosition.z)) / tileWidth);

	unsigned int rr = component->getRenderRadius();
	int xStart = x - rr;
	int xEnd = x + rr;
	int yStart = y - rr;
	int yEnd = y + rr;

	component->preRenderComponent();

	Program * prog = component->getActiveShader();
	prog->use();
	prog->applyGlobalUniforms();

	// Culling parameters
	glm::vec3 fwd = cam->getForwardVector();
	fwd.y = 0;
	fwd = -glm::normalize(fwd);
	int px = -int(renderRadius), py = px;

	for (int i = xStart; i < xEnd; i++, px++)
	{
		for (int j = yStart; j < yEnd; j++, py++)
		{
			// Culling (skips almost half)
			glm::vec3 test(i - x, 0, j - y);
			if (abs(px) > 2 && abs(py) > 2 && glm::dot(glm::normalize(test), fwd) < 0.1f)
				continue;

			component->renderComponent(i, j, cam);
		}
	}

	component->postRenderComponent();
}

void Engine::Terrain::renderTiledComponentShadow(Engine::TerrainComponent * component, Engine::Camera * cam, const glm::mat4 & proj)
{
	glm::vec3 cameraPosition = cam->getPosition();

	int x = -int((floor(cameraPosition.x)) / tileWidth);
	int y = -int((floor(cameraPosition.z)) / tileWidth);

	unsigned int rr = component->getRenderRadius();
	int xStart = x - rr;
	int xEnd = x + rr;
	int yStart = y - rr;
	int yEnd = y + rr;

	component->preRenderComponent();

	Program * prog = component->getShadowMapShader();
	prog->use();
	prog->applyGlobalUniforms();

	// Culling parameters
	glm::vec3 fwd = cam->getForwardVector();
	fwd.y = 0;
	fwd = -glm::normalize(fwd);
	int px = -int(renderRadius), py = px;

	for (int i = xStart; i < xEnd; i++, px++)
	{
		for (int j = yStart; j < yEnd; j++, py++)
		{
			// Culling (skips almost half)
			glm::vec3 test(i - x, 0, j - y);
			if (abs(px) > 2 && abs(py) > 2 && glm::dot(glm::normalize(test), fwd) < 0.1f)
				continue;

			component->renderShadow(proj, i, j, cam);
		}
	}

	component->postRenderComponent();
}

// ====================================================================================================================

void Engine::Terrain::initialize()
{
	Engine::RenderableNotifier::getInstance().registerRenderable(this);
	Engine::CascadeShadowMaps::getInstance().registerShadowCaster(this);

	// MESHES INSTANCING
	createTileMesh();

	Engine::LandscapeComponent * landscape = new Engine::LandscapeComponent();
	landscape->init(tileWidth, true);
	registerComponent(landscape);

	Engine::WaterComponent * water = new Engine::WaterComponent();
	water->init(tileWidth, false);
	registerComponent(water);

	Engine::TreeComponent * trees = new Engine::TreeComponent();
	trees->init(tileWidth, true);
	registerComponent(trees);

	Engine::FlowerComponent * flowers = new Engine::FlowerComponent();
	flowers->init(tileWidth, false);
	registerComponent(flowers);
}

void Engine::Terrain::createTileMesh()
{
	float vertices[12];
	vertices[0] = 0.0f; vertices[1] = 0.0f; vertices[2] = 0.0f;
	vertices[3] = 1.0f; vertices[4] = 0.0f; vertices[5] = 0.0f;
	vertices[6] = 0.0f; vertices[7] = 0.0f; vertices[8] = 1.0f;
	vertices[9] = 1.0f; vertices[10] = 0.0f; vertices[11] = 1.0f;

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
	Engine::MeshTable::getInstance().addMeshToCache("terrain_tile", plane);
}

// ====================================================================================================================

void Engine::Terrain::notifyRenderModeUpdate(Engine::RenderMode mode)
{
	for (auto & v : renderableComponents)
		v->notifyRenderModeChange(mode);
}

float Engine::Terrain::getTileScale()
{
	return tileWidth;
}

unsigned int Engine::Terrain::getRenderRadius()
{
	return renderRadius;
}