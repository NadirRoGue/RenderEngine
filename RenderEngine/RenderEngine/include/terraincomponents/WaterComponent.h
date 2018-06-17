/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include "TerrainComponent.h"

#include "programs/ProceduralWaterProgram.h"

namespace Engine
{
	/**
	 * Terrain component in charge of render procedural water
	 * Wont cast shadows
	 */
	class WaterComponent : public TerrainComponent
	{
	private:
		// Shading program
		ProceduralWaterProgram * fillShader;
		// Wireframe program
		ProceduralWaterProgram * wireShader;
		//ProceduralWaterProgram * shadowShader;

		// Active shader
		ProceduralWaterProgram * activeShader;

		// Tile instance to render
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