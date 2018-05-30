#pragma once

#include <string>

namespace Engine
{
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

		void dispatch(unsigned int xDim, unsigned int yDim, unsigned int zDim, unsigned int barrier);

		void destroy();
	private:
		unsigned int loadShaderFile();
	};
}