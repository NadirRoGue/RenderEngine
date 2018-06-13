#include "postprocessprograms/CloudFilterProgram.h"

#include "Renderer.h"
#include "WorldConfig.h"
#include "TimeAccesor.h"

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
	uFrame = other.uFrame;
	uEvenFrame = other.uEvenFrame;
	uOddFrame = other.uOddFrame;
}

Engine::CloudFilterProgram::~CloudFilterProgram()
{
}

void Engine::CloudFilterProgram::configureProgram()
{
	PostProcessProgram::configureProgram();

	uTexelSize = glGetUniformLocation(glProgram, "texelSize");
	uEvenFrame = glGetUniformLocation(glProgram, "evenFrame");
	uOddFrame = glGetUniformLocation(glProgram, "oddFrame");
	uFrame = glGetUniformLocation(glProgram, "frame");
}

void Engine::CloudFilterProgram::onRenderObject(const Engine::Object * obj, Engine::Camera * camera)
{
	glUniform2f(uTexelSize, 1.0f / ((float)ScreenManager::SCREEN_WIDTH), 1.0f / ((float)ScreenManager::SCREEN_HEIGHT));
	glUniform1i(uFrame, (GLint)Engine::Time::frame);
}

void Engine::CloudFilterProgram::setBufferInput(const Engine::TextureInstance * color,  const Engine::TextureInstance * color2)
{
	glUniform1i(uEvenFrame, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, color->getTexture()->getTextureId());

	glUniform1i(uOddFrame, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, color2->getTexture()->getTextureId());
}

// ===============================================================================

Engine::Program * Engine::CloudFilterProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::CloudFilterProgram * program = new Engine::CloudFilterProgram(Engine::CloudFilterProgram::PROGRAM_NAME, parameters);
	program->initialize();
	return program;
}