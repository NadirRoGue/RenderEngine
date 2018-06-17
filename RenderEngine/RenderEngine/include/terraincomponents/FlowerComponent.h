/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include "TerrainComponent.h"

#include "programs/TreeProgram.h"

namespace Engine
{
	/**
	 * Terrain component in charge of render flowers across the terrain
	 * Wont cast shadows
	 */
	class FlowerComponent : public TerrainComponent
	{
	private:
		// Program version to shade the flowers
		TreeProgram * fillShader;
		// Program version to render as wireframe
		TreeProgram * wireShader;

		// Active render shader
		TreeProgram * activeShader;

		// Flower instance
		Object * flower;
		// Number of flowers per terrain tile
		size_t flowersToSpawn;
	public:
		FlowerComponent();

		unsigned int getRenderRadius();

		void initialize();
		void preRenderComponent();
		void renderComponent(int i, int j, Engine::Camera * camera);
		void renderShadow(const glm::mat4 & projection, int i, int j, Engine::Camera * cam);
		void notifyRenderModeChange(Engine::RenderMode mode);

		Program * getActiveShader();
		Program * getShadowMapShader();
	};
}