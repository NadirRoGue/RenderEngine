/**
 * @author Nadir Román Guerrero
 * @email nadir.ro.gue@gmail.com
 */

#include "Program.h"

#include <iostream>

#include "util/IOUtils.h"

const size_t VERSION_HEADER_LENGHT = 17;

// ================================================================================

Engine::Program::Program(std::string name, unsigned long long params)
	:name(name),parameters(params)
{
}

Engine::Program::Program(const Engine::Program & other)
{
	name = other.name;
	parameters = other.parameters;

	glProgram = other.glProgram;
	vShader = other.vShader;
	fShader = other.fShader;

	vShaderFile = other.vShaderFile;
	fShaderFile = other.fShaderFile;
}

void Engine::Program::initialize()
{
	if (vShaderFile.empty() || fShaderFile.empty())
	{
		std::cout << name << ": Failed to initialize - no shader files specified" << std::endl;
		return;
	}

	vShader = loadShader(vShaderFile, GL_VERTEX_SHADER);
	fShader = loadShader(fShaderFile, GL_FRAGMENT_SHADER);

	glProgram = glCreateProgram();

	glAttachShader(glProgram, vShader);
	glAttachShader(glProgram, fShader);

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

Engine::Program::~Program()
{
}

unsigned int Engine::Program::getProgramId() const
{
	return glProgram;
}

unsigned int Engine::Program::loadShader(std::string fileName, GLenum type, std::string configString, bool outputToFile, std::string outputFileName)
{
	size_t fileLen;
	char *source = Engine::IO::loadStringFromFile(fileName.c_str(), fileLen);
	
	std::string result(source);

	if (!configString.empty())
	{
		std::string header = result.substr(0, VERSION_HEADER_LENGHT);
		std::string body = result.substr(VERSION_HEADER_LENGHT, result.size() - VERSION_HEADER_LENGHT);
		result = header + "\n" + configString + "\n" + body;
	}
	
	char * finalSourceCStr = new char[result.size()];
	memcpy(finalSourceCStr, result.c_str(), result.size() * sizeof(char));
	fileLen = result.size();
	
	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar **)&finalSourceCStr, (const GLint *)&fileLen);
	glCompileShader(shader);
	delete[] source;
	delete[] finalSourceCStr;

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetShaderInfoLog(shader, logLen, NULL, logString);
		std::cout << "Error on " << fileName << ": " << logString << std::endl;
		delete[] logString;
		glDeleteShader(shader); 
		exit(-1);
	}

	return shader;
}

std::string Engine::Program::getName() const
{
	return name;
}

void Engine::Program::destroy()
{
	glDetachShader(glProgram, vShader);
	glDeleteShader(vShader);

	glDetachShader(glProgram, fShader);
	glDeleteShader(fShader);

	glDeleteProgram(glProgram);
}

// ===========================================================

Engine::Program * Engine::ProgramFactory::instantiateProgram(unsigned long long parameters)
{
	std::map<unsigned long long, Program*>::iterator it = cache.find(parameters);
	if (it != cache.end())
	{
		return it->second;
	}
	else
	{
		Engine::Program * program = createProgram(parameters);
		cache[parameters] = program;
		return program;
	}
}

void Engine::ProgramFactory::clean()
{
	std::map<unsigned long long, Program*>::iterator it = cache.begin();
	while (it != cache.end())
	{
		it->second->destroy();
		delete it->second;
		it++;
	}

	cache.clear();
}