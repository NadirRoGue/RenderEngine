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
	uLightColor = other.uLightColor;
	uRepro1 = other.uRepro1;
	uRepro2 = other.uRepro2;
	uRepro3 = other.uRepro3;
	uRepro4 = other.uRepro4;
}

Engine::CloudFilterProgram::~CloudFilterProgram()
{
}

void Engine::CloudFilterProgram::configureProgram()
{
	PostProcessProgram::configureProgram();

	uTexelSize = glGetUniformLocation(glProgram, "texelSize");
	uLightColor = glGetUniformLocation(glProgram, "realLightColor");
	uRepro1 = glGetUniformLocation(glProgram, "repro1");
	uRepro2 = glGetUniformLocation(glProgram, "repro2");
	uRepro3 = glGetUniformLocation(glProgram, "repro3");
	uRepro4 = glGetUniformLocation(glProgram, "repro4");
}

void Engine::CloudFilterProgram::onRenderObject(const Engine::Object * obj, Engine::Camera * camera)
{
	glUniform2f(uTexelSize, 1.0f / ((float)ScreenManager::SCREEN_WIDTH), 1.0f / ((float)ScreenManager::SCREEN_HEIGHT));
	glUniform3fv(uLightColor, 1, &Engine::Settings::realLightColor[0]);
}

void Engine::CloudFilterProgram::setBufferInput(Engine::TextureInstance ** buffer)
{
	glUniform1i(uRepro1, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, buffer[0]->getTexture()->getTextureId());

	glUniform1i(uRepro2, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, buffer[1]->getTexture()->getTextureId());
	/*
	glUniform1i(uRepro3, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, buffer[2]->getTexture()->getTextureId());

	glUniform1i(uRepro4, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, buffer[3]->getTexture()->getTextureId());
	*/
}

// ===============================================================================

Engine::Program * Engine::CloudFilterProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::CloudFilterProgram * program = new Engine::CloudFilterProgram(Engine::CloudFilterProgram::PROGRAM_NAME, parameters);
	program->initialize();
	return program;
}