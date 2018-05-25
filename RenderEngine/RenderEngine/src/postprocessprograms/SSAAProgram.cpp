#include "postprocessprograms/SSAAProgram.h"

#include "Renderer.h"

#include <iostream>

std::string Engine::SSAAProgram::PROGRAM_NAME = "SSAAProgram";

Engine::SSAAProgram::SSAAProgram(std::string name, unsigned long long params)
	:Engine::PostProcessProgram(name, params)
{
	fShaderFile = "shaders/postprocess/SSAA.frag";
}

Engine::SSAAProgram::SSAAProgram(const Engine::SSAAProgram & other)
	: Engine::PostProcessProgram(other)
{
	uTexelSize = other.uTexelSize;
}

Engine::SSAAProgram::~SSAAProgram()
{

}

void Engine::SSAAProgram::configureProgram()
{
	Engine::PostProcessProgram::configureProgram();

	uTexelSize = glGetUniformLocation(glProgram, "texelSize");
}

void Engine::SSAAProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	Engine::PostProcessProgram::onRenderObject(obj, view, proj);

	float texelSize[2];
	texelSize[0] = 1.0f / ScreenManager::SCREEN_WIDTH;
	texelSize[1] = 1.0f / ScreenManager::SCREEN_HEIGHT;

	glUniform2fv(uTexelSize, 1, &texelSize[0]);
}

// ===============================================================================

Engine::Program * Engine::SSAAProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::SSAAProgram * program = new Engine::SSAAProgram(Engine::SSAAProgram::PROGRAM_NAME, parameters);
	program->initialize();
	return program;
}