#pragma once

#include "TerrainComponent.h"

#include "programs/TreeProgram.h"

namespace Engine
{
	class FlowerComponent : public TerrainComponent
	{
	private:
		TreeProgram * fillShader;
		TreeProgram * wireShader;

		TreeProgram * activeShader;

		Object * flower;
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