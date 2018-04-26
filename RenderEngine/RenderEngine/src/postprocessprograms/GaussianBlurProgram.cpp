#include "postprocessprograms/GaussianBlurProgram.h"

#include "Renderer.h"

std::string Engine::GaussianBlurProgram::PROGRAM_NAME = "GaussianBlurProgram";

Engine::GaussianBlurProgram::GaussianBlurProgram(std::string name, unsigned long long params)
	:Engine::PostProcessProgram(name, params)
{
	fShaderFile = "shaders/postProcessing.GaussianBlur.frag";
	affectedTexels = kernel = 0;
}

Engine::GaussianBlurProgram::GaussianBlurProgram(const Engine::GaussianBlurProgram & other)
	: PostProcessProgram(other)
{
	uAffetedTexels = other.uAffetedTexels;
	uMaskSize = other.uMaskSize;
	uMask = other.uMask;
	uTexelSize = other.uTexelSize;

	affectedTexels = kernel = 0;
}

Engine::GaussianBlurProgram::~GaussianBlurProgram()
{
	if (affectedTexels != 0)
	{
		delete[] affectedTexels;
	}
}

void Engine::GaussianBlurProgram::configureProgram()
{
	PostProcessProgram::configureProgram();

	uAffetedTexels = glGetUniformLocation(glProgram, "affectedTexels");
	uMaskSize = glGetUniformLocation(glProgram, "maskSize");
	uMask = glGetUniformLocation(glProgram, "kernel");
	uTexelSize = glGetUniformLocation(glProgram, "texelSize");
}

void Engine::GaussianBlurProgram::setMaskSize(unsigned int ms)
{
	maskSize = ms;
}

void Engine::GaussianBlurProgram::setKernel(float * k)
{
	if (kernel != 0)
	{
		delete[] kernel;
	}

	kernel = new float[maskSize];

	memcpy(kernel, k, sizeof(float) * maskSize);
}

void Engine::GaussianBlurProgram::setAffectedTexels(glm::vec2 * at)
{
	if (affectedTexels != 0)
	{
		delete[] affectedTexels;
	}

	affectedTexels = new float[maskSize * 2];

	for (unsigned int i = 0; i < maskSize; i++)
	{
		glm::vec2 v = at[i];
		const unsigned int start = i * 2;
		affectedTexels[start] = v.x;
		affectedTexels[start + 1] = v.y;
	}
}

void Engine::GaussianBlurProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	Engine::PostProcessProgram::onRenderObject(obj, view, proj);

	glUniform1ui(uMaskSize, maskSize);
	glUniform1fv(uMask, maskSize, &kernel[0]);
	glUniform2fv(uAffetedTexels, maskSize, &affectedTexels[0]);
	float texelSize[2];
	texelSize[0] = 1.0f / ScreenManager::SCREEN_WIDTH;
	texelSize[1] = 1.0f / ScreenManager::SCREEN_HEIGHT;

	glUniform2fv(uTexelSize, 1, &texelSize[0]);
}

// ===============================================================================

Engine::Program * Engine::GaussianBlurProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::GaussianBlurProgram * program = new Engine::GaussianBlurProgram(Engine::GaussianBlurProgram::PROGRAM_NAME, parameters);
	program->initialize();
	return program;
}