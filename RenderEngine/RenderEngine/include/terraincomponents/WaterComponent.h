#pragma once

#include "TerrainComponent.h"

#include "programs/ProceduralWaterProgram.h"

namespace Engine
{
	class WaterComponent : public TerrainComponent
	{
	private:
		ProceduralWaterProgram * fillShader;
		ProceduralWaterProgram * wireShader;
		ProceduralWaterProgram * shadowShader;

		ProceduralWaterProgram * activeShader;

		Object * waterTile;
	public:
		WaterComponent();

		unsigned int getRenderRadius();

		void initialize();

		void preRenderComponent();
		void renderComponent(int i, int j, Engine::Camera * camera);
		void renderShadow(const glm::mat4 & projection, int i, int j, Engine::Camera * cam);
		void postRenderComponent();
		void notifyRenderModeChange(Engine::RenderMode mode);

		Program * getActiveShader();
		Program * getShadowMapShader();
	};
}