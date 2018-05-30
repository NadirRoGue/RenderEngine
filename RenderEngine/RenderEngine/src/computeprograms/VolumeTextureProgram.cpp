#include "computeprograms/VolumeTextureProgram.h" 

#include <GL/glew.h>
#include <iostream>

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
	uOutput = glGetUniformLocation(glProgram, "outVolTex");
}

void Engine::VolumeTextureProgram::bindOutput(const Engine::TextureInstance * ti)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, ti->getTexture()->getTextureId());
	glBindImageTexture(0, ti->getTexture()->getTextureId(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
	glUniform1i(uOutput, 0);
}