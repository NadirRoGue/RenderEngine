#include "postprocessprograms/SSAAProgram.h"

#include "Renderer.h"

Engine::SSAAProgram::SSAAProgram(std::string name)
	:Engine::PostProcessProgram(name)
{
	fShaderFile = "shaders/postProcessing.SSAA.frag";
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