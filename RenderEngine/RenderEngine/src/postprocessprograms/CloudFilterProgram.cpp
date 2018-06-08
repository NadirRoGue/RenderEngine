#include "postprocessprograms/CloudFilterProgram.h"

#include "Renderer.h"
#include "WorldConfig.h"

std::string Engine::CloudFilterProgram::PROGRAM_NAME = "CloudFilterProgram";

Engine::CloudFilterProgram::CloudFilterProgram(std::string name, unsigned long long params)
	:Engine::PostProcessProgram(name, params)
{
	fShaderFile = "shaders/clouds/cloudfilter.frag";
}

Engine::CloudFilterProgram::CloudFilterProgram(const Engine::CloudFilterProgram & other)
	: PostProcessProgram(other)
{
	uTexelSize = other.uTexelSize;

	uGColor = other.uGColor;
}

Engine::CloudFilterProgram::~CloudFilterProgram()
{
}

void Engine::CloudFilterProgram::configureProgram()
{
	PostProcessProgram::configureProgram();

	uTexelSize = glGetUniformLocation(glProgram, "texelSize");
	uGColor = glGetUniformLocation(glProgram, "gColor");
}

void Engine::CloudFilterProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	glUniform2f(uTexelSize, 1.0f / ((float)ScreenManager::SCREEN_WIDTH), 1.0f / ((float)ScreenManager::SCREEN_HEIGHT));
}

void Engine::CloudFilterProgram::setBufferInput(const Engine::TextureInstance * color)
{
	glUniform1i(uGColor, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, color->getTexture()->getTextureId());
}

// ===============================================================================

Engine::Program * Engine::CloudFilterProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::CloudFilterProgram * program = new Engine::CloudFilterProgram(Engine::CloudFilterProgram::PROGRAM_NAME, parameters);
	program->initialize();
	return program;
}