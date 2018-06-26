/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include <vector>

#include "Camera.h"
#include "TerrainComponent.h"
#include "IRenderable.h"
#include "ShadowCaster.h"

namespace Engine
{
	// Represents the terrain. Manages and renders all terrain
	// components registered to it
	class Terrain : public IRenderable, public ShadowCaster
	{
	private:
		float tileWidth;
		unsigned int renderRadius;

		std::vector<TerrainComponent*> renderableComponents;
		std::vector<TerrainComponent*> shadowableComponents;
	public:
		Terrain();
		Terrain(float tileWidth, unsigned int renderRadius);
		~Terrain();

		void registerComponent(TerrainComponent * comp);

		void render(Camera * camera);
		void renderShadow(Camera * camera, const glm::mat4 & projectionMatrix);

		void notifyRenderModeUpdate(RenderMode mode);

		float getTileScale();
		unsigned int getRenderRadius();
	private:
		void initialize();
		void createTileMesh();

		void renderTiledComponent(TerrainComponent * component, Camera * cam);
		void renderTiledComponentShadow(TerrainComponent * component, Camera * cam, const glm::mat4 & proj);
	};
}
