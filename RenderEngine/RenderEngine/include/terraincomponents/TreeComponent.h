/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include "TerrainComponent.h"

#include "programs/TreeProgram.h"

#include <vector>

namespace Engine
{
	/**
	 * Terrain component in charge of rendering trees across the terrain
	 * Cast shadows
	 */
	class TreeComponent : public TerrainComponent
	{
	private:
		// Shading program
		TreeProgram * fillShader;
		// Wireframe mode program
		TreeProgram * wireShader;
		// Point mode program
		TreeProgram * pointShader;
		// Shadow map render program
		TreeProgram * shadowShader;

		// Active shader (shading or wireframe)
		TreeProgram * activeShader;

		// List of type of trees
		std::vector<Object *> treeTypes;
		// Number of trees to spawn per terrain tile
		size_t treesToSpawn;
		// Equal amount of each tree type to spawn
		size_t equalAmountOfTrees;
		// shuffled jitter pattern to ensure trees are spread
		glm::vec2 * jitterPattern;
	public:
		TreeComponent();

		unsigned int getRenderRadius();

		void initialize();
		void renderComponent(int i, int j, Engine::Camera * camera);
		void renderShadow(const glm::mat4 & projection, int i, int j, Engine::Camera * cam);
		void notifyRenderModeChange(Engine::RenderMode mode);

		Program * getActiveShader();
		Program * getShadowMapShader();
	private:
		// Run the fractal tree generator to build a fixed number of different procedural trees
		void initTrees();
	};
}