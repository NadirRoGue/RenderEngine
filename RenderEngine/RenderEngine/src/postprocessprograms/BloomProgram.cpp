#include "postprocessprograms/BloomProgram.h"

const std::string Engine::BloomProgram::PROGRAM_NAME = "BloomProgram";

Engine::BloomProgram::BloomProgram(std::string name, unsigned long long parameters)
	:Engine::PostProcessProgram(name, parameters)
{
	fShaderFile = "shaders/postprocess/Bloom.frag";
}

Engine::BloomProgram::BloomProgram(const Engine::BloomProgram & other)
	: PostProcessProgram(other)
{
}

// ==================================================================

Engine::Program * Engine::BloomProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::BloomProgram * bp = new Engine::BloomProgram(Engine::BloomProgram::PROGRAM_NAME, parameters);
	bp->initialize();
	return bp;
}