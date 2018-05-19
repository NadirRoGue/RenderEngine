#include "ComputeProgram.h"

#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>

//#include "util/IOUtils.h"

char * loadStringFromFile(const char * fileName, unsigned long long & fileLen)
{
	std::ifstream file;
	file.open(fileName, std::ios::in);
	if (!file)
	{
		return 0;
	}

	file.seekg(0, std::ios::end);
	fileLen = file.tellg();
	file.seekg(std::ios::beg);

	char * content = new char[fileLen + 1];

	int i = 0;
	while (file.good())
	{
		content[i] = char(file.get());
		if (!file.eof()) i++;
		else fileLen = i;
	}

	content[fileLen] = '\0';
	file.close();

	return content;
}

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
	size_t fileLen;
	std::string temp = computeShaderFile;
	char * source = loadStringFromFile(temp.c_str(), fileLen);

	std::string buf(source);

	char * result = new char[fileLen];
	memcpy(result, buf.c_str(), fileLen);

	GLuint shader;
	shader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shader, 1, (const GLchar **)&result, (const GLint *)&fileLen);
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