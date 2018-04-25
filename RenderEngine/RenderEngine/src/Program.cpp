/**
 * @author Nadir Román Guerrero
 * @email nadir.ro.gue@gmail.com
 */

#include "Program.h"

#include <iostream>
#include <fstream>

char *loadStringFromFile(const char *fileName, unsigned int &fileLen)
{
	//Se carga el fichero
	std::ifstream file;
	file.open(fileName, std::ios::in);
	if (!file) return 0;

	//Se calcula la longitud del fichero
	file.seekg(0, std::ios::end);
	fileLen = unsigned int(file.tellg());
	file.seekg(std::ios::beg);

	//Se lee el fichero
	char *source = new char[fileLen + 1];

	int i = 0;
	while (file.good())
	{
		source[i] = char(file.get());
		if (!file.eof()) i++;
		else fileLen = i;
	}
	source[fileLen] = '\0';
	file.close();

	return source;
}

// ================================================================================

Engine::Program::Program(std::string name, unsigned long long params)
{
	this->name = std::string(name);
}

Engine::Program::Program(const Engine::Program & other)
{
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

unsigned int Engine::Program::loadShader(std::string fileName, GLenum type)
{
	unsigned int fileLen;
	char *source = loadStringFromFile(fileName.c_str(), fileLen);

	GLuint shader;
	shader = glCreateShader(type);
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
		std::cout << "Error on " << fileName << ": " << logString << std::endl;
		delete[] logString;
		glDeleteShader(shader); 
		exit(-1);
	}

	return shader;
}

void Engine::Program::configureDirectionalLightBuffer(const Engine::DirectionalLight *dl)
{
}

void Engine::Program::configurePointLightBuffer(const Engine::PointLight *pl)
{
}

void Engine::Program::configureSpotLightBuffer(const Engine::SpotLight *sl)
{
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