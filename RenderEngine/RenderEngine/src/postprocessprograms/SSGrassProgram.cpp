#include "postprocessprograms/SSGrassProgram.h"

#include "renderers/DeferredRenderer.h"

const std::string Engine::SSGrassProgram::PROGRAM_NAME = "SSGrassProgram";

Engine::SSGrassProgram::SSGrassProgram(std::string name, unsigned long long params)
	:Engine::PostProcessProgram(name, params)
{
	fShaderFile = "shaders/postprocess/SSGrass.frag";
}

Engine::SSGrassProgram::SSGrassProgram(const Engine::SSGrassProgram & other)
	: Engine::PostProcessProgram(other)
{
	uScreenSize = other.uScreenSize;
	uGrassInfoBuffer = other.uGrassInfoBuffer;
	uPosBuffer = other.uPosBuffer;
}

void Engine::SSGrassProgram::configureProgram()
{
	Engine::PostProcessProgram::configureProgram();

	uScreenSize = glGetUniformLocation(glProgram, "screenSize");
	uGrassInfoBuffer = glGetUniformLocation(glProgram, "grassBuffer");
	uPosBuffer = glGetUniformLocation(glProgram, "posBuffer");
}

void Engine::SSGrassProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 & proj)
{
	Engine::PostProcessProgram::onRenderObject(obj, view, proj);

	Engine::DeferredRenderer * dr = static_cast<Engine::DeferredRenderer*>(Engine::RenderManager::getInstance().getRenderer());

	glm::vec2 ss(Engine::ScreenManager::SCREEN_WIDTH, Engine::ScreenManager::SCREEN_HEIGHT);
	glUniform2fv(uScreenSize, 1, &ss[0]);
	
	glUniform1i(uGrassInfoBuffer, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, dr->getGBufferInfo()->getTexture()->getTextureId());

	glUniform1i(uPosBuffer, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dr->getGBufferPos()->getTexture()->getTextureId());
}

// ======================================================================================

Engine::Program * Engine::SSGrassProgramFactory::createProgram(unsigned long long params)
{
	Engine::SSGrassProgram * p = new Engine::SSGrassProgram(Engine::SSGrassProgram::PROGRAM_NAME, params);
	p->initialize();
	return p;
}