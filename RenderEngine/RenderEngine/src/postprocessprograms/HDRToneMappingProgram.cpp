#include "postprocessprograms/HDRToneMappingProgram.h"

#include "WorldConfig.h"

const std::string Engine::HDRToneMappingProgram::PROGRAM_NAME = "HDRToneMappingProgram";

Engine::HDRToneMappingProgram::HDRToneMappingProgram(std::string name, unsigned long long params)
	:Engine::PostProcessProgram(name, params)
{
	fShaderFile = "shaders/postprocess/HDRToneMapping.frag";
}

Engine::HDRToneMappingProgram::HDRToneMappingProgram(const Engine::HDRToneMappingProgram & other)
	: Engine::PostProcessProgram(other)
{
	uExposure = other.uExposure;
	uGamma = other.uGamma;
}

void Engine::HDRToneMappingProgram::configureProgram()
{
	Engine::PostProcessProgram::configureProgram();

	uExposure = glGetUniformLocation(glProgram, "exposure");
	uGamma = glGetUniformLocation(glProgram, "gamma");
}

void Engine::HDRToneMappingProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 & proj)
{
	Engine::PostProcessProgram::onRenderObject(obj, view, proj);

	glUniform1f(uExposure, Engine::Settings::hdrExposure);
	glUniform1f(uGamma, Engine::Settings::hdrGamma);
}

// =========================================================================================================================

Engine::Program * Engine::HDRToneMappingProgramFactory::createProgram(unsigned long long params)
{
	Engine::HDRToneMappingProgram * prog = new HDRToneMappingProgram(Engine::HDRToneMappingProgram::PROGRAM_NAME, params);
	prog->initialize();
	return prog;
}