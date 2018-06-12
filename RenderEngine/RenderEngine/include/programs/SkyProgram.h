/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Program.h"

namespace Engine
{
	/**
	 * Class in charge to manage the sky render on cubemap program
	 */
	class SkyProgram : public Program
	{
	public:
		// Unique program name
		static std::string PROGRAM_NAME;
	private:
		// Projection matrix id
		unsigned int uProjMatrix;
		// Light direction id
		unsigned int uLightDir;
		// Light color id
		unsigned int uLightColor;

		// Zenit color id
		unsigned int uSkyZenitColor;
		// Sky color id
		unsigned int uSkyHorizonColor;

		// Position attribute id
		unsigned int inPos;

	public:
		SkyProgram(std::string name, unsigned long long params);
		SkyProgram(const SkyProgram & other);

		void configureProgram();
		void configureMeshBuffers(Mesh * mesh);
		void onRenderObject(const Object * obj, Camera * camera);
	};

	// =======================================================================
	// Creates new sky programs
	class SkyProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}