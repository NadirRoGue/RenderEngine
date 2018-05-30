#include "postprocessprograms/SSReflectionProgram.h"

#include "renderers/DeferredRenderer.h"
#include "WorldConfig.h"


const std::string Engine::SSReflectionProgram::PROGRAM_NAME = "SSReflectionProgram";

Engine::SSReflectionProgram::SSReflectionProgram(std::string name, unsigned long long params)
	:Engine::PostProcessProgram(name, params)
{
	fShaderFile = "shaders/postprocess/SSReflections.frag";
}

Engine::SSReflectionProgram::SSReflectionProgram(const Engine::SSReflectionProgram & other)
	: Engine::PostProcessProgram(other)
{
	uProjMat = other.uProjMat;
	uPosBuffer = other.uPosBuffer;
	uNormalBuffer = other.uNormalBuffer;
	uDepthBuffer = other.uDepthBuffer;
	uSpecularBuffer = other.uSpecularBuffer;
	uLightDir = other.uLightDir;
}

void Engine::SSReflectionProgram::configureProgram()
{
	Engine::PostProcessProgram::configureProgram();

	uProjMat = glGetUniformLocation(glProgram, "projMat");
	uPosBuffer = glGetUniformLocation(glProgram, "posBuffer");
	uNormalBuffer = glGetUniformLocation(glProgram, "normalBuffer");
	uDepthBuffer = glGetUniformLocation(glProgram, "depthBuffer");
	uSpecularBuffer = glGetUniformLocation(glProgram, "specularBuffer");
	uLightDir = glGetUniformLocation(glProgram, "lightDirection");
}

void Engine::SSReflectionProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 & proj)
{
	Engine::PostProcessProgram::onRenderObject(obj, view, proj);

	Engine::DeferredRenderer * deferred = static_cast<Engine::DeferredRenderer*>(Engine::RenderManager::getInstance().getRenderer());

	glUniformMatrix4fv(uProjMat, 1, GL_FALSE, &(proj[0][0]));
	glUniform1i(uPosBuffer, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, deferred->getGBufferPos()->getTexture()->getTextureId());
	glUniform1i(uNormalBuffer, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, deferred->getGBufferNormal()->getTexture()->getTextureId());
	glUniform1i(uDepthBuffer, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, deferred->getGBufferDepth()->getTexture()->getTextureId());
	glUniform1i(uSpecularBuffer, 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, deferred->getGBufferSpecular()->getTexture()->getTextureId());

	glm::vec3 normalDir = glm::normalize(Engine::Settings::lightDirection);
	glUniform3fv(uLightDir, 1, &normalDir[0]);
}

// =====================================================================

Engine::Program * Engine::SSReflectionProgramFactory::createProgram(unsigned long long params)
{
	Engine::SSReflectionProgram * prog = new Engine::SSReflectionProgram(Engine::SSReflectionProgram::PROGRAM_NAME, params);
	prog->initialize();
	return prog;
}