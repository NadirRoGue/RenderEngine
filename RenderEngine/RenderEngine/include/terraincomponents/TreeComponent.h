#pragma once

#include "TerrainComponent.h"

#include "programs/TreeProgram.h"

#include <vector>

namespace Engine
{
	class TreeComponent : public TerrainComponent
	{
	private:
		TreeProgram * fillShader;
		TreeProgram * wireShader;
		TreeProgram * shadowShader;

		TreeProgram * activeShader;

		std::vector<Object *> treeTypes;
		size_t treesToSpawn;
		size_t equalAmountOfTrees;
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
		void initTrees();
	};
}