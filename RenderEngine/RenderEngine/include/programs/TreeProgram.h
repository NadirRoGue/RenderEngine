/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Program.h"

namespace Engine
{
	/**
	 * Class in charge to manage the tree shading program
	 */
	class TreeProgram : public Program
	{
	public:
		// Unique program name
		static const std::string PROGRAM_NAME;

		// UBER Shader parameters
		// Render shadow map depth
		const static unsigned long long SHADOW_MAP;
		// Render as wireframe mode
		const static unsigned long long WIRE_MODE;
	private:
		// Geometry shader file path
		std::string gShaderFile;

		// Geometry shader id
		unsigned int gShader;

		// Model view projection matrix id
		unsigned int uModelViewProj;
		// Model view matrix id
		unsigned int uModelView;
		// Normal matrix id
		unsigned int uNormal;
		// Cascade shadow map level 0 light depth matrix
		unsigned int uLightDepthMat0;
		// Cascade shadow map level 1 light depth matrix
		unsigned int uLightDepthMat1;
		// Cascade shadow map level 0 depth texture
		unsigned int uDepthMap0;
		// Cascade shadow map level 1 depth texture
		unsigned int uDepthMap1;
		// Normalized 2D position within the current World grid cell id
		unsigned int uGridUV;

		// Terrain data, used to emulate terrain to compute the vegetation height 
		// and accept/drop tree depending on its position
		// (discarded: on water or over the max vegetaion height)
		// Perlin amplitude id
		unsigned int uAmplitude;
		// Perlin frequency id
		unsigned int uFrecuency;
		// Perlin scale id
		unsigned int uScale;
		// Perlin octaves id
		unsigned int uOctaves;
		// Light direction id
		unsigned int uLightDir;
		// Normalized water height id
		unsigned int uWaterLevel;
		// Normalized max vegetation spawn height id
		unsigned int uMaxHeight;

		// Vetex position attribute id
		unsigned int uInPos;
		// Vertex color attribute id
		unsigned int uInColor;
		// Vertex normal attribute id
		unsigned int uInNormal;
		// Vertex emission attribute id
		unsigned int uInEmissive;
	public:
		TreeProgram(std::string name, unsigned long long params);
		TreeProgram(const TreeProgram & other);

		void initialize();

		void configureProgram();
		void configureMeshBuffers(Mesh * mesh);

		// Apply all uniform data which is constant across all instances using this program
		void applyGlobalUniforms();
		void onRenderObject(const Object * obj, Camera * camera);

		// Sets the normalized position within the current world grid cell
		void setUniformTileUV(float u, float v);
		// Sets the cascade shadow map level 0 light projection matrix
		void setUniformLightDepthMat(const glm::mat4 & ldp);
		// Sets the cascade shadow map level 1 light projection matrix
		void setUniformLightDepthMat1(const glm::mat4 & ldp);

		void destroy();
	};

	// ===============================================================
	// Create new tree programs
	class TreeProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long params);
	};
}