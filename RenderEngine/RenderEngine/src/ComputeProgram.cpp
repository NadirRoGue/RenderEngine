#include "ComputeProgram.h"

#include <iostream>
#include <GL/glew.h>

#include "util/IOUtils.h"

Engine::ComputeProgram::ComputeProgram(std::string shaderFile)
	:computeShaderFile(shaderFile)
{
}

Engine::ComputeProgram::ComputeProgram(const ComputeProgram & other)
{
	computeShaderFile = other.computeShaderFile;

	glProgram = other.glProgram;
	computeShader = other.computeShader;
}

unsigned int Engine::ComputeProgram::getProgramId()
{
	return glProgram;
}

void Engine::ComputeProgram::initialize()
{
	computeShader = loadShaderFile();
	
	glProgram = glCreateProgram();

	glAttachShader(glProgram, computeShader);
	
	glLinkProgram(glProgram);

	int linked;
	glGetProgramiv(glProgram, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint logLen;
		glGetProgramiv(glProgram, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetProgramInfoLog(glProgram, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		exit(-1);
	}

	configureProgram();
}

void Engine::ComputeProgram::dispatch(unsigned int xDim, unsigned int yDim, unsigned int zDim, unsigned int barrier)
{
	glDispatchCompute(xDim, yDim, zDim);
	glMemoryBarrier(barrier);
}

void Engine::ComputeProgram::destroy()
{
	glDetachShader(glProgram, computeShader);
	glDeleteShader(computeShader);

	glDeleteProgram(glProgram);
}

unsigned int Engine::ComputeProgram::loadShaderFile()
{
	if (computeShaderFile.empty())
	{
		return -1;
	}

	size_t fileLen;
	char *source = Engine::IO::loadStringFromFile(computeShaderFile.c_str(), fileLen);

	GLuint shader;
	shader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shader, 1, (const GLchar **)&source, (const GLint *)&fileLen);
	glCompileShader(shader);
	delete[] source;

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetShaderInfoLog(shader, logLen, NULL, logString);
		std::cout << "Error on " << computeShaderFile << ": " << logString << std::endl;
		delete[] logString;
		glDeleteShader(shader);
		exit(-1);
	}

	return shader;
}