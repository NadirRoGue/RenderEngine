/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include "TerrainComponent.h"

#include "programs/ProceduralTerrainProgram.h"

namespace Engine
{
	/**
	 * Terrain component in charge of rendering the terrain mesh
	 * Cast shadows
	 */
	class LandscapeComponent : public TerrainComponent
	{
	private:
		// Shading program
		ProceduralTerrainProgram * fillShader;
		// Wireframe mode program
		ProceduralTerrainProgram * wireShader;
		// Point mode program
		ProceduralTerrainProgram * pointShader;
		// Shadow map render program
		ProceduralTerrainProgram * shadowShader;

		// Active program (shading or wire)
		ProceduralTerrainProgram * activeShader;

		// Tile instance
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