#include "computeprograms/VolumeTextureProgram.h"

#include <GL/glew.h>

Engine::VolumeTextureProgram::VolumeTextureProgram(std::string shaderFile)
	:Engine::ComputeProgram(shaderFile)
{

}

Engine::VolumeTextureProgram::VolumeTextureProgram(const Engine::VolumeTextureProgram & other)
	: Engine::ComputeProgram(other)
{

}

void Engine::VolumeTextureProgram::configureProgram()
{
	uOutput = glGetUniformLocation(glProgram, "output");
}

void Engine::VolumeTextureProgram::bindOutput(const Engine::TextureInstance * ti)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, ti->getTexture()->getTextureId());
	glUniform1i(uOutput, 0);
}