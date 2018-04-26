#include "postprocessprograms/PerlinGeneratorProgram.h"

std::string Engine::PerlinGeneratorProgram::PROGRAM_NAME = "PerlinGeneratorProgram";

Engine::PerlinGeneratorProgram::PerlinGeneratorProgram(std::string name, unsigned long long params)
	:PostProcessProgram(name, params)
{
	fShaderFile = "shaders/terrain/perlin.frag";
	scale = 5.0f;
	frequency = 1.0f;
	amplitude = 0.5f;
	octaves = 8;
}

Engine::PerlinGeneratorProgram::PerlinGeneratorProgram(const PerlinGeneratorProgram & other)
	: PostProcessProgram(other)
{
	perlinAmplitude = other.perlinAmplitude;
	perlinFrequency = other.perlinFrequency;
	perlinScale = other.perlinScale;
	perlinOctaves = other.perlinOctaves;

	scale = other.scale;
	frequency = other.frequency;
	amplitude = other.amplitude;
	octaves = other.octaves;
}

Engine::PerlinGeneratorProgram::~PerlinGeneratorProgram()
{

}

void Engine::PerlinGeneratorProgram::configureProgram()
{
	Engine::PostProcessProgram::configureProgram();
	perlinAmplitude = glGetUniformLocation(glProgram, "amplitude");
	perlinFrequency = glGetUniformLocation(glProgram, "frecuency");
	perlinScale = glGetUniformLocation(glProgram, "scale");
	perlinOctaves = glGetUniformLocation(glProgram, "octaves");
}

void Engine::PerlinGeneratorProgram::onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	Engine::PostProcessProgram::onRenderObject(obj, view, proj);
	glUniform1f(perlinAmplitude, amplitude);
	glUniform1f(perlinFrequency, frequency);
	glUniform1f(perlinOctaves, octaves);
	glUniform1f(perlinScale, scale);
}

// ================================================================================

Engine::Program * Engine::PerlinGeneratorProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::PerlinGeneratorProgram * program = new Engine::PerlinGeneratorProgram(Engine::PerlinGeneratorProgram::PROGRAM_NAME, parameters);
	program->initialize();
	return program;
}