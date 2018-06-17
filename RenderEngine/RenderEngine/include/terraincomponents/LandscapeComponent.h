#pragma once

#include "TerrainComponent.h"

#include "programs/ProceduralTerrainProgram.h"

namespace Engine
{
	class LandscapeComponent : public TerrainComponent
	{
	private:
		ProceduralTerrainProgram * fillShader;
		ProceduralTerrainProgram * wireShader;
		ProceduralTerrainProgram * shadowShader;

		ProceduralTerrainProgram * activeShader;

		Object * landscapeTile;
	public:
		LandscapeComponent();

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