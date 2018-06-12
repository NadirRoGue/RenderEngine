/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "PostProcessProgram.h"

#include "Scene.h"

namespace Engine
{
	/**
	 * Class in charge to manage the deferred shading program
	 */
	class DeferredShadingProgram : public PostProcessProgram
	{
	public:
		// Program unique name
		static std::string PROGRAM_NAME;
	private:
		// Directinal light data buffer
		unsigned int uDLBuffer;
		// Point lights data buffer
		unsigned int uPLBuffer;
		// Spot lights data buffer
		unsigned int uSLBuffer;
		// Camera space world up vector
		unsigned int uWorldUp;

		// Zenit and horizon color (used for atmospheric fog)
		unsigned int uSkyZenitColor;
		unsigned int uSkyHorizonColor;
	public:
		DeferredShadingProgram(std::string name, unsigned long long params);
		DeferredShadingProgram(const DeferredShadingProgram & other);

		void onRenderObject(const Object * obj, Camera * camera);
		void configureProgram();
	private:
		// Updates the needed data from the directional light buffer
		void processDirectionalLights(DirectionalLight * dl, const glm::mat4 & viewMatrix);
	};

	// =================================================================================
	// Deferred shading factory: creates new deferred shading programs
	class DeferredShadingProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}