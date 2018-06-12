/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Program.h"

namespace Engine
{
	/**
	 * Class in charge of managing the procedural terrain generation program
	 */
	class ProceduralTerrainProgram : public Program
	{
	public:
		// Unique program name
		static const std::string PROGRAM_NAME;

		// UBER Shader parameters
		// Draw as wireframe mode
		static const unsigned long long WIRE_DRAW_MODE;
		// Render shadow map depth mode
		static const unsigned long long SHADOW_MAP;
	protected:
		// Tessellation control shader path
		std::string tcsShaderFile;
		// Tesselation evaluation shader path
		std::string tevalShaderFile;
		// Geometry evaluation shader path
		std::string gShaderFile;

		// Tesselation and geometry shaders id
		unsigned int tcsShader, tevalShader, gShader;

		// vertex position attribute id
		unsigned int uInPos;
		// Vertex texture coordinates attribute id
		unsigned int uInUV;

		// Model view matrix id
		unsigned int uModelView;
		// Model view projection matrix id
		unsigned int uModelViewProj;
		// Normal (transpose(inverse(modelView)) matrix id
		unsigned int uNormal;

		// Cascade Shadow map level 0 projection matrix id
		unsigned int uLightDepthMatrix;
		// Cascade shadow map level 1 projection matrix id
		unsigned int uLightDepthMatrix1;
		// Cascade shadow map level 0 depth texture
		unsigned int uDepthTexture;
		// Cascade shadow map level 1 depth texture
		unsigned int uDepthTexture1;
		// Light direction id
		unsigned int uLightDirection;

		// Normalized water height (0 - 1) id
		unsigned int uWaterLevel;

		// Time passed id
		unsigned int uTime;

		// World grid position id
		unsigned int uGridPos;

		// Perlin amplitude id
		unsigned int uAmplitude;
		// Perlin frequency id
		unsigned int uFrecuency;
		// Perlin scale id
		unsigned int uScale;
		// Perlin octaves id
		unsigned int uOctaves;

		// Grass coverage cut id (grass probability is the dot product of world up and surface normal)
		unsigned int uGrassCoverage;
		// Grass color id
		unsigned int uGrassColor;
		// Sand color id
		unsigned int uSandColor;
		// Rock color id
		unsigned int uRockColor;
	public:
		ProceduralTerrainProgram(std::string name, unsigned long long params);
		ProceduralTerrainProgram(const ProceduralTerrainProgram & other);

		// Loads and compiles the shaders, apply the uber shader method
		void initialize();
		virtual void configureProgram();
		void configureMeshBuffers(Mesh * mesh);

		// Apply all uniforms which are constants to all object instances using this shader
		void applyGlobalUniforms();
		void onRenderObject(const Object * obj, Camera * camera);

		void destroy();

		// Set current world grid position
		void setUniformGridPosition(unsigned int i, unsigned int j);
		// Sets cascade shadow maps level 0 light depth matrix
		void setUniformLightDepthMatrix(const glm::mat4 & ldm);
		// Sets cascade shadow maps level 1 light depth matrix
		void setUniformLightDepthMatrix1(const glm::mat4 & ldm);
	};

	// ===================================================================================
	// Creates new procedural terrain programs
	class ProceduralTerrainProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}