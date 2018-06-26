/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include <string>

namespace Engine
{
	// Parent of classes in charge of controlling compute shaders
	// within the engine
	// Definition split from Program given the execution differences
	class ComputeProgram
	{
	protected:
		unsigned int glProgram;
		unsigned int computeShader;

	protected:
		std::string computeShaderFile;

	public:
		ComputeProgram(std::string shaderFile);
		ComputeProgram(const ComputeProgram & other);
		
		unsigned int getProgramId();

		void initialize();
		virtual void configureProgram() = 0;

		// Dispatch the compute shader with the given grid configuration, and stablishing the
		// synchronization barrier type
		void dispatch(unsigned int xDim, unsigned int yDim, unsigned int zDim, unsigned int barrier);

		void destroy();
	private:
		unsigned int loadShaderFile();
	};
}