#include "postprocessprograms/DepthOfFieldProgram.h"

std::string Engine::DepthOfFieldProgram::PROGRAM_NAME = "DepthOfFieldProgram";

Engine::DepthOfFieldProgram::DepthOfFieldProgram(std::string name, unsigned long long params)
	:Engine::GaussianBlurProgram(name, params)
{
	fShaderFile = "shaders/postprocess/DepthOfFieldZBuffer.frag";
}

Engine::DepthOfFieldProgram::DepthOfFieldProgram(const Engine::DepthOfFieldProgram & other)
	: Engine::GaussianBlurProgram(other)
{
	uFocalDistance = other.uFocalDistance;
	uMaxDistanceFactor = other.uMaxDistanceFactor;
	uInverseProj = other.uInverseProj;
}

Engine::DepthOfFieldProgram::~DepthOfFieldProgram()
{
}

void Engine::DepthOfFieldProgram::configureProgram()
{
	Engine::GaussianBlurProgram::configureProgram();

	uFocalDistance = glGetUniformLocation(glProgram, "focalDistance");
	uMaxDistanceFactor = glGetUniformLocation(glProgram, "maxDistanceFactor");
	uInverseProj = glGetUniformLocation(glProgram, "invProj");
}

void Engine::DepthOfFieldProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	Engine::GaussianBlurProgram::onRenderObject(obj, view, proj);

	glm::mat4 invProj = glm::inverse(proj);

	glUniform1f(uFocalDistance, focalDistance);
	glUniform1f(uMaxDistanceFactor, maxDistanceFactor);
	glUniformMatrix4fv(uInverseProj, 1, GL_FALSE, &invProj[0][0]);
}

void Engine::DepthOfFieldProgram::setFocalDistance(float fd)
{
	focalDistance = fd;
}

void Engine::DepthOfFieldProgram::setMaxDistanceFactor(float mdf)
{
	maxDistanceFactor = mdf;
}

float Engine::DepthOfFieldProgram::getFocalDistance()
{
	return focalDistance;
}

float Engine::DepthOfFieldProgram::getMaxDistanceFactor()
{
	return maxDistanceFactor;
}

// ==================================================================

Engine::Program * Engine::DepthOfFieldProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::DepthOfFieldProgram * program = new Engine::DepthOfFieldProgram(Engine::DepthOfFieldProgram::PROGRAM_NAME, parameters);
	program->initialize();
	return program;
}