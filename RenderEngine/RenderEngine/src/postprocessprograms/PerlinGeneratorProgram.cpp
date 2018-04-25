#include "postprocessprograms/PerlinGeneratorProgram.h"

Engine::PerlinGeneratorProgram::PerlinGeneratorProgram(std::string name)
	:PostProcessProgram(name)
{
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