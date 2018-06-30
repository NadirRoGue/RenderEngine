/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Program.h"

namespace Engine
{
	/**
	 * Class in charge to manage the procedural water program
	 */
	class ProceduralWaterProgram : public Program
	{
	public:
		// Unique program name
		static std::string PROGRAM_NAME;

		// UBER Shader parameters
		// Draw as wireframe mode
		static const unsigned long long WIRE_DRAW_MODE;
		// Draw as point mode
		static const unsigned long long POINT_DRAW_MODE;
		// Render shadow map depth mode (unused, discared to render water shadows)
		static const unsigned long long SHADOW_MAP;
	private:
		// Geometry shader file (we need geomtry shader to draw as wireframe, even though its just 2 triangles)
		std::string gShaderFile;

		// Geometry shader id
		unsigned int gShader;

		// Position attribute id
		unsigned int uInPos;
		// Texture coordinates attribute id
		unsigned int uInUV;

		// Model view matrix id
		unsigned int uModelView;
		// Model view projection matrix id
		unsigned int uModelViewProj;
		// Normal matrix
		unsigned int uNormal;

		// G-Buffer info texture id (used to adjust blending between water and bottom based on depth)
		unsigned int uInInfo;
		// Screen size id
		unsigned int uScreenSize;

		// Shadow render has been disabled for water
		// Data is kept though
		// Cascade shadow maps level 0 light depth matrix
		unsigned int uLightDepthMatrix;
		// Cascade shadow maps level 1 light depth matrix
		unsigned int uLightDepthMatrix1;
		// Cascade shadow maps level 0 depth texture
		unsigned int uDepthTexture;
		// Cascade shadow maps level 1 depth texture
		unsigned int uDepthTexture1;
		// Light direction id
		unsigned int uLightDirection;

		// World grid position id
		unsigned int uGridPos;
		// Time passed id
		unsigned int uTime;
		// Water color id
		unsigned int uWaterColor;
		// Water movement speed id
		unsigned int uWaterSpeed;
	public:
		ProceduralWaterProgram(std::string name, unsigned long long parameters);
		ProceduralWaterProgram(const ProceduralWaterProgram & other);

		void initialize();
		void configureProgram();
		void configureMeshBuffers(Mesh * mesh);

		// Apply uniform data which is common to all instances using this shader
		void applyGlobalUniforms();
		void onRenderObject(const Object * obj, Camera * camera);

		void destroy();
		
		// Sets the world grid position
		void setUniformGridPosition(unsigned int i, unsigned int j);
		// Sets the cascade shadow map level 0 light projection matrix
		void setUniformLightDepthMatrix(const glm::mat4 & ldm);
		// Sets the cascade shadow map level 1 light projection matrix
		void setUniformLightDepthMatrix1(const glm::mat4 & ldm);
	};

	// =========================================================
	// Creates new procedural water programs
	class ProceduralWaterProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}